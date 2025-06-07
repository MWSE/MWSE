#include "PatchUtil.h"

#include "Log.h"
#include "MemoryUtil.h"
#include "mwOffsets.h"

#include "TES3Actor.h"
#include "TES3ActorAnimationController.h"
#include "TES3AudioController.h"
#include "TES3BodyPartManager.h"
#include "TES3Cell.h"
#include "TES3Class.h"
#include "TES3Creature.h"
#include "TES3CutscenePlayer.h"
#include "TES3DataHandler.h"
#include "TES3Dialogue.h"
#include "TES3Game.h"
#include "TES3GameFile.h"
#include "TES3GameSetting.h"
#include "TES3InputController.h"
#include "TES3ItemData.h"
#include "TES3Light.h"
#include "TES3LoadScreenManager.h"
#include "TES3MagicEffectInstance.h"
#include "TES3Misc.h"
#include "TES3MobilePlayer.h"
#include "TES3MobileProjectile.h"
#include "TES3MobManager.h"
#include "TES3Reference.h"
#include "TES3Script.h"
#include "TES3Sound.h"
#include "TES3UIElement.h"
#include "TES3UIInventoryTile.h"
#include "TES3UIMenuController.h"
#include "TES3VFXManager.h"
#include "TES3WorldController.h"

#include "NICollisionSwitch.h"
#include "NIFlipController.h"
#include "NILinesData.h"
#include "NIPick.h"
#include "NIPointLight.h"
#include "NISortAdjustNode.h"
#include "NiTriShape.h"
#include "NiTriShapeData.h"
#include "NIUVController.h"

#include "BitUtil.h"
#include "ScriptUtil.h"
#include "TES3Util.h"
#include "WindowsUtil.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "BuildDate.h"
#include "CodePatchUtil.h"
#include "MWSEConfig.h"
#include "MWSEDefs.h"
#include "CrashLogExceptionHandler.hpp"

namespace mwse::patch {

#ifndef _DEBUG
	// Release builds.
	constexpr auto INSTALL_MINIDUMP_HOOK = true;
#else
	// Debug builds.
	constexpr auto INSTALL_MINIDUMP_HOOK = false;
#endif

	const char* SafeGetObjectId(const TES3::BaseObject* object) {
		__try {
			return object->getObjectID();
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			return nullptr;
		}
	}

	const char* SafeGetSourceFile(const TES3::BaseObject* object) {
		__try {
			return object->getSourceFilename();
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			return nullptr;
		}
	}

	template <typename T>
	void safePrintObjectToLog(const char* title, const T* object) {
		if (object) {
			auto id = SafeGetObjectId(object);
			auto source = SafeGetSourceFile(object);
			log::getLog() << "  " << title << ": " << (id ? id : "<memory corrupted>") << " (" << (source ? source : "<memory corrupted>") << ")" << std::endl;
			if (id) {
				log::prettyDump(object);
			}
		}
		else {
			log::getLog() << "  " << title << ": nullptr" << std::endl;
		}
	}

	//
	// Patch: Enable
	//

	void PatchScriptOpEnable() {
		auto scriptVars = mwscript::getLocalScriptVariables();
		if (scriptVars) {
			scriptVars->unknown_0xC &= 0xFE;
		}
	}

	//
	// Patch: Disable
	//

	static bool PatchScriptOpDisable_ForceCollisionUpdate = false;

	void PatchScriptOpDisable() {
		auto scriptVars = mwscript::getLocalScriptVariables();
		if (scriptVars) {
			scriptVars->unknown_0xC |= 0x1;
		}

		// Determine if we need to force update collision.
		auto reference = mwscript::getScriptTargetReference();
		if (reference) {
			PatchScriptOpDisable_ForceCollisionUpdate = !reference->getDisabled();
		}
		else {
			PatchScriptOpDisable_ForceCollisionUpdate = false;
		}
	}

	void* __fastcall PatchScriptOpDisableCollision(TES3::Reference* reference) {
		// Force update collision.
		if (PatchScriptOpDisable_ForceCollisionUpdate) {
			TES3::DataHandler::get()->updateCollisionGroupsForActiveCells();
		}

		// Return overwritten code.
		return &reference->baseObject;
	}

	//
	// Patch: Unify athletics training.
	//

	void PatchUnifyAthleticsTraining() {
		auto worldController = TES3::WorldController::get();
		auto mobilePlayer = worldController->getMobilePlayer();

		auto athletics = &TES3::DataHandler::get()->nonDynamicData->skills[TES3::SkillID::Athletics];

		// If we're running, use the first progress.
		if (mobilePlayer->getMovementFlagRunning()) {
			mobilePlayer->exerciseSkill(TES3::SkillID::Athletics, athletics->progressActions[0] * worldController->deltaTime);
		}

		// If we're swimming, use the second progress.
		if (mobilePlayer->getMovementFlagSwimming()) {
			mobilePlayer->exerciseSkill(TES3::SkillID::Athletics, athletics->progressActions[1] * worldController->deltaTime);
		}
	}

	//
	// Patch: Unify sneak training.
	//

	void PatchUnifySneakTraining() {
		auto nonDynamicData = TES3::DataHandler::get()->nonDynamicData;

		// Decrement sneak use delay counter.
		*reinterpret_cast<float*>(0x7D16E0) = *reinterpret_cast<float*>(0x7D16E0) - nonDynamicData->GMSTs[TES3::GMST::fSneakUseDelay]->value.asFloat;

		// Excercise sneak.
		TES3::WorldController::get()->getMobilePlayer()->exerciseSkill(TES3::SkillID::Sneak, nonDynamicData->skills[TES3::SkillID::Sneak].progressActions[0]);
	}

	//
	// Patch: Fix crash with paper doll equipping/unequipping.
	//
	// In this patch, the tile associated with the stack may have been deleted, but the property to the TES3::ItemData 
	// remains. If the memory is reallocated it will fill with garbage, and cause a crash. The tile property, however,
	// is properly deleted. So we look for that instead, and return its associated item data (which is the same value).
	//! TODO: Find out where it's being deleted, and also delete the right property.
	//

	TES3::UI::PropertyValue* __fastcall PatchPaperdollTooltipCrashFix(TES3::UI::Element* self, DWORD _UNUSUED_, TES3::UI::PropertyValue* propValue, TES3::UI::Property prop, TES3::UI::PropertyType propType, const TES3::UI::Element* element = nullptr, bool checkInherited = false) {
		auto tileProp = self->getProperty(TES3::UI::PropertyType::Pointer, *reinterpret_cast<TES3::UI::Property*>(0x7D3A70));
		auto tile = reinterpret_cast<TES3::UI::InventoryTile*>(tileProp.ptrValue);

		if (tile == nullptr) {
			propValue->ptrValue = nullptr;
		}
		else {
			propValue->ptrValue = tile->itemData;
		}

		return propValue;
	}

	//
	// Patch: Allow the game to correctly close when quit with a messagebox popup.
	//
	// The game holds up a TES3::UI messagebox and runs its own infinite loop waiting for a response
	// when a critical error has occurred. This does not respect the WorldController's stopGameLoop
	// flag, which is set when the user attempts to close the window.
	//
	// Here we check if that flag is set, and if it is, force a choice on the "no" dialogue option,
	// which stops the deadlock.
	//

	int __cdecl SafeQuitGetMessageChoice() {
		if (TES3::WorldController::get()->stopGameLoop) {
			log::getLog() << "[MWSE] Prevented rogue Morrowind.exe instance." << std::endl;
			*reinterpret_cast<int*>(0x7B88C0) = 1;
		}

		return *reinterpret_cast<int*>(0x7B88C0);
	}

	//
	// Patch: Optimize DontThreadLoad, prevent multi-thread loading from lua.
	//
	// Every time the game wants to load, it checks the ini file from disk for the DontThreadLoad value.
	// This patch caches the value so it only needs to be read once.
	//
	// Additionally, this provides a way to suppress thread loading from lua, if it is causing an issue in
	// a script (namely, a lua state deadlock).
	//

	UINT WINAPI	OverrideDontThreadLoad(LPCSTR lpAppName, LPCSTR lpKeyName, INT nDefault, LPCSTR lpFileName) {
		return TES3::DataHandler::suppressThreadLoad || TES3::DataHandler::dontThreadLoad;
	}

	//
	// Patch: Make Morrowind believe that it is always the front window in the main gameplay loop block.
	//

	static HWND lastActiveWindow = 0;
	HWND __stdcall PatchGetMorrowindMainWindow() {
		auto worldController = TES3::WorldController::get();
		auto mainWindowHandle = worldController->Win32_hWndParent;

		// Check to see if we've become inactive.
		auto activeWindow = GetActiveWindow();
		if (activeWindow != mainWindowHandle && activeWindow != lastActiveWindow) {
			// Reset mouse deltas so it stops moving.
			auto inputController = worldController->inputController;
			inputController->mouseState.lX = 0;
			inputController->mouseState.lY = 0;
			inputController->mouseState.lZ = 0;

			memset(inputController->keyboardState, 0, sizeof(inputController->keyboardState));
			memset(inputController->previousKeyboardState, 0, sizeof(inputController->previousKeyboardState));
		}

		lastActiveWindow = activeWindow;

		return mainWindowHandle;
	}

	void __fastcall PatchGetMorrowindMainWindow_NoBackgroundInput(TES3::InputController* inputController) {
		if (GetActiveWindow() != TES3::WorldController::get()->Win32_hWndParent) {
			return;
		}

		inputController->readKeyState();
	}

	int __fastcall PatchGetMorrowindMainWindow_NoBufferReading(TES3::InputController* inputController, DWORD _EDX_, DWORD* key) {
		if (GetActiveWindow() != TES3::WorldController::get()->Win32_hWndParent) {
			// Read in the input so it doesn't get buffered when we alt-tab back in.
			inputController->readButtonPressed(key);

			// But pretend that nothing was found.
			*key = 0;
			return 0;
		}

		auto result = inputController->readButtonPressed(key);
		TES3::UI::MenuInputController::lastKeyPressDIK = result ? *key : 0xFF;
		return result;
	}

	//
	// Patch: Optimize access to global variables. Access them in a hashmap instead of linear searching.
	//

	auto __fastcall DataHandlerCreateGlobalsContainer(void* garbage) {
		mwse::tes3::_delete(garbage);
		return new TES3::GlobalHashContainer();
	}

	void __fastcall DataHandlerDestroyGlobalsContainer(TES3::GlobalHashContainer* container) {
		delete container;
	}

	const auto TES3_WorldController_InitGlobals = reinterpret_cast<void(__thiscall*)(TES3::WorldController*)>(0x40E920);
	void __fastcall WorldControllerInitGlobals(TES3::WorldController* worldController) {
		// Call original code.
		TES3_WorldController_InitGlobals(worldController);

		// New variables.
		auto globals = TES3::DataHandler::get()->nonDynamicData->globals;
		globals->addVariableCacheOnly(worldController->gvarGameHour);
		globals->addVariableCacheOnly(worldController->gvarYear);
		globals->addVariableCacheOnly(worldController->gvarMonth);
		globals->addVariableCacheOnly(worldController->gvarDay);
		globals->addVariableCacheOnly(worldController->gvarDaysPassed);
		globals->addVariableCacheOnly(worldController->gvarTimescale);
		globals->addVariableCacheOnly(worldController->gvarCharGenState);
		globals->addVariableCacheOnly(worldController->gvarMonthsToRespawn);
	}

	//
	// Patch: Support loading existing moved references.
	//
	// The following records have been modified:
	//  - CELL.FRMR
	//  - CELL.MVRF
	//  - REFR.FRMR
	//  - SCPT.RNAM
	//

#if MWSE_RAISED_FILE_LIMIT
	namespace PatchRaiseESXLimit {
		// Vanilla offsets and masks.
		constexpr DWORD ModBitsVanilla = 8;
		constexpr DWORD FormBitsVanilla = sizeof(DWORD) * CHAR_BIT - ModBitsVanilla;
		constexpr DWORD ModMaskVanilla = ((1 << ModBitsVanilla) - 1) << FormBitsVanilla;
		constexpr DWORD FormMaskVanilla = (1 << FormBitsVanilla) - 1;
		constexpr DWORD ModCountVanilla = 1 << ModBitsVanilla;

		// New offsets and masks.
		constexpr DWORD ModBitsMWSE = 10;
		constexpr DWORD FormBitsMWSE = sizeof(DWORD) * CHAR_BIT - ModBitsMWSE;
		constexpr DWORD ModMaskMWSE = ((1 << ModBitsMWSE) - 1) << FormBitsMWSE;
		constexpr DWORD FormMaskMWSE = (1 << FormBitsMWSE) - 1;
		constexpr DWORD ModCountMWSE = 1 << ModBitsMWSE;
		static_assert(1 << ModBitsMWSE == sizeof(TES3::NonDynamicData::activeMods) / sizeof(TES3::GameFile*), "Reference FormID bit assignment does not match active game file array size.");

		struct SerializedFormId {
			DWORD modIndex; // 0x0
			DWORD formId; // 0x4
		};

		void __fastcall LoadFormId(TES3::GameFile* file, DWORD edx, DWORD* out_movedFormId, size_t size) {
			// Loading the new format?
			SerializedFormId data;
			if (file->currentChunkHeader.size == sizeof(SerializedFormId)) {
				file->readChunkData(&data);
			}
			else {
				// If it's not the new format, we need to convert.
				DWORD oldFormId = 0;
				file->readChunkData(&oldFormId);

				data.modIndex = (oldFormId >> FormBitsVanilla);
				data.formId = (oldFormId & FormMaskVanilla);
			}

			*out_movedFormId = (data.modIndex << FormBitsMWSE) + data.formId;
		}

		void __fastcall SaveFormId(TES3::GameFile* file, DWORD edx, unsigned int tag, DWORD* movedRefId, size_t size) {
			// Split out the bitmasked field.
			SerializedFormId data = {};
			data.modIndex = *movedRefId >> FormBitsMWSE;
			data.formId = *movedRefId & FormMaskMWSE;

			// If the mod index is higher than the vanilla limit, save the new format.
			if (data.modIndex > 255) {
				file->writeChunkData(tag, &data, sizeof(data));
			}
			// If the mod index is <255, use the vanilla save format and masks for compatibility.
			else {
				DWORD refId = (data.modIndex << FormBitsVanilla) + data.formId;
				file->writeChunkData(tag, &refId, 4);
			}
		}
	}
#endif

	//
	// Patch: Fix RemoveItem crash.
	//
	// Seen with StarFire's StarfireNPCA_nightlife script. Doesn't seem to actually call RemoveItem.
	// Mostly here to gather more information to help diagnose the crash.
	//
	// referenceToThis is only accessed for clones.
	//

	TES3::Reference::ReferenceData* __fastcall PatchFixupActorSelfReference(TES3::Reference* self) {
		auto isClone = self->baseObject->isActor() && static_cast<TES3::Actor*>(self->baseObject)->isClone();

		if (isClone && self->baseObject->referenceToThis == nullptr) {
			self->baseObject->referenceToThis = self;

			using namespace mwse::log;
			auto& log = getLog();
			log << "[MWSE] Fixed crash with invalid RemoveItem call. Report this to the #mwse channel in the Morrowind Modding Community Discord so we can narrow this down more. Dumping related objects." << std::endl;

			log << "Reference: " << self->getObjectID() << std::endl;
			prettyDump(self);

			log << "Object: " << self->baseObject->getObjectID() << std::endl;
			prettyDump(static_cast<TES3::Actor*>(self->baseObject));

			auto script = TES3::Script::currentlyExecutingScript;
			if (script) {
				log << "Script: " << script->getObjectID() << std::endl;
				prettyDump(script);
			}

			auto cell = self->getCell();
			if (cell) {
				log << "Cell: " << cell->getEditorName() << std::endl;
				prettyDump(cell);

				auto playerCell = TES3::DataHandler::get()->currentCell;
				if (playerCell && playerCell != cell) {
					log << "Player cell differs: " << playerCell->getEditorName() << std::endl;
				}
			}

			log << "mwscript data: OpCode: " << std::hex << *reinterpret_cast<DWORD*>(0x7A91C4) << "; Cursor offset: " << *reinterpret_cast<DWORD*>(0x7CEBB0) << "; Look ahead token: " << int(*reinterpret_cast<unsigned char*>(0x7CEBA8)) << std::endl;
		}
		return &self->referenceData;
	}

	//
	// Patch: Player animation idles.
	//
	// Update animations for third person and first person player reference when idle mode is flagged.
	//

	const auto TES3_DataHandler_UpdateAllIdles = reinterpret_cast<void(__thiscall*)(TES3::DataHandler*)>(0x48AED0);
	const auto TES3_Reference_AnimIdleUpdate = reinterpret_cast<void(__thiscall*)(TES3::Reference*)>(0x4E6E20);
	void __stdcall PatchUpdateAllIdles() {
		TES3_DataHandler_UpdateAllIdles(TES3::DataHandler::get());

		auto worldController = TES3::WorldController::get();
		auto mobilePlayer = worldController->getMobilePlayer();
		if (mobilePlayer->actorFlags & TES3::MobileActorFlag::IdleAnim) {
			TES3_Reference_AnimIdleUpdate(mobilePlayer->reference);
			TES3_Reference_AnimIdleUpdate(mobilePlayer->firstPersonReference);
		}
	}

	//
	// Patch: Correctly initialize MobileProjectile tag/objectType
	// 
	// The copy constructor for MobileProjectiles fails to correctly set the object type correctly. This
	// ensures that it is set to the right value, instead of 0.
	//

	void __fastcall PatchInitializeMobileProjectileType(TES3::ObjectType::ObjectType* type) {
		*type = TES3::ObjectType::MobileProjectile;
	}

	//
	// Helper function for raised mod limit.
	//
	// Raise C runtime fopen limit from 512 to 2048. This covers the case where all mods are open during game load.
	// Otherwise, fopen will fail and Morrowind will ignore the error, causing issues.
	//
	bool raiseStdioFileLimit() {
		// Use stdio function from Morrowind's C runtime.
		HINSTANCE hMSVCRT = GetModuleHandleA("msvcrt.dll");
		if (hMSVCRT != NULL) {
			auto msvcrt_setmaxstdio = reinterpret_cast<int(*)(int)>(GetProcAddress(hMSVCRT, "_setmaxstdio"));
			if (msvcrt_setmaxstdio(2048) == 2048) {
				return true;
			}
			else {
				mwse::log::getLog() << "MWSE_RAISED_FILE_LIMIT: msvcrt_setmaxstdio(2048) failed." << std::endl;
			}
		}
		else {
			mwse::log::getLog() << "MWSE_RAISED_FILE_LIMIT: GetModuleHandleA(\"msvcrt.dll\") failed." << std::endl;
		}
		return false;
	}

	//
	// Patch: Fix crash when saving menu position if the derived key name is too long.
	//

	__declspec(naked) void PatchSaveMenuPositionRightPad() {
		__asm {
			// Clamp eax <= 32
			cmp eax, 32
			jbe clamped
			mov eax, 32
		clamped:
			// Null terminate padding so that key length+padding length is at least 32
			lea ecx, [esp+4+0x64]
			sub ecx, eax
			mov byte ptr [ecx], 0
			ret
		}
	}

	//
	// Patch: Fix enchantment copying on books and weapons.
	//

	__declspec(naked) void PatchCopyBookEnchantmentCaller() {
		__asm {
			push ebp
			mov ecx, ebx
		}
	}
	__declspec(naked) void PatchCopyWeaponEnchantmentCaller() {
		__asm {
			push ebx
			mov ecx, ebp
		}
	}
	constexpr size_t PatchCopyEnchantmentCaller_size = 0x3;

	void __fastcall PatchCopyEnchantment(TES3::Item* item, DWORD _EDX_, const TES3::Item* from) {
		// Free existing enchantment ID string if available.
		if (item->getEnchantment() && !item->getLinksResolved()) {
			tes3::free(item->getEnchantment());
		}
		item->setEnchantment(nullptr);

		if (from->getEnchantment()) {
			if (from->getLinksResolved()) {
				item->setEnchantment(from->getEnchantment());
			}
			else {
				// Helper union so we don't have to reinterpret memory all the time.
				union EnchantUnion { TES3::Enchantment* enchantment; char* id; };
				EnchantUnion toEnchantment = {}, fromEnchantment = {};

				// Make a copy of the enchantment's ID.
				fromEnchantment.enchantment = from->getEnchantment();
				const auto enchantmentIDLength = strnlen_s(fromEnchantment.id, 32) + 1;
				toEnchantment.id = reinterpret_cast<char*>(tes3::malloc(enchantmentIDLength));
				strncpy_s(toEnchantment.id, enchantmentIDLength, fromEnchantment.id, enchantmentIDLength);
				item->setEnchantment(toEnchantment.enchantment);
			}
		}
	}

	//
	// Patch: Letterbox movies.
	//

	const auto TES3_DrawMovieFrame = reinterpret_cast<int(__cdecl*)(void*, float, float, float, float, int, int)>(0x64FE20);
	int __cdecl PatchDrawLetterboxMovieFrame(void* device, float left, float top, float scaleWidth, float scaleHeight, int textureWidth, int textureHeight) {
		if (Configuration::LetterboxMovies) {
			auto game = TES3::Game::get();
			auto bink = game->loadScreenManager->cutscenePlayer->binkHandle;
			if (scaleHeight < scaleWidth) {
				left = (game->windowWidth - (scaleHeight * bink->width)) / 2.0f;
				scaleWidth = scaleHeight;
			}
			else if (scaleWidth < scaleHeight) {
				top = (game->windowHeight - (scaleWidth * bink->height)) / 2.0f;
				scaleHeight = scaleWidth;
			}
		}

		return TES3_DrawMovieFrame(device, left, top, scaleWidth, scaleHeight, textureWidth, textureHeight);
	}

	//
	// Patch: Slight optimization to journal updating.
	//

	__declspec(naked) void PatchSwapJournalUpdateCheckForSpeakerOrder() {
		__asm {
			// Check speaker first.
			mov eax, [edi + 0x28]            // Size: 0x3
			test eax, eax                    // Size: 0x2
			jnz $ + 0xE5                     // Size: 0x6

			// Then bother to check to see if we have text.
			mov ecx, edi                     // Size: 0x2
			nop							     // Size: 0x5. Replaced with a call generation. Can't do so here, because offsets aren't accurate.
			nop							     // ^
			nop							     // ^
			nop							     // ^
			nop							     // ^
			test eax, eax                    // Size: 0x2
			jz $ + 0xD6                      // Size: 0x6
		}
	}
	constexpr auto PatchSwapJournalUpdateCheckForSpeakerOrder_size = (0x4B2FF1u - 0x4B2FD7u);

	//
	// Patch: Fix issue when serializing no effects.
	//

	const auto TES3_SaveVisualEffects = reinterpret_cast<void(__thiscall*)(TES3::VFXManager*, TES3::GameFile*)>(0x469CC0);
	void __fastcall PatchSaveVisualEffects(TES3::VFXManager* vfxManager, DWORD _EDX_, TES3::GameFile* file) {
		// Ensure that we have serializable VFXs.
		for (const auto& vfx : vfxManager->vfxNodes) {
			if (vfx->effectObject && !vfx->createdFromNode && !vfx->expired) {
				// Call original code.
				TES3_SaveVisualEffects(vfxManager, file);
				return;
			}
		}
	}

	//
	// Patch: Fix crash when releasing a clone of a light with no reference.
	//        Also fix crash when the attachment scenegraph light pointer has been cleared.
	//
	// The first fix is mostly useful for creating VFXs using a light object as a base.
	// The second fix is to prevent a crash and try to identify the cause of the cleared pointer.
	//

	TES3::Attachment* __fastcall PatchReleaseLightEntityForReference(const TES3::Reference* reference) {
		if (reference == nullptr) {
			return nullptr;
		}

		auto attachment = static_cast<TES3::LightAttachment*>(reference->getAttachment(TES3::AttachmentType::Light));

		if (attachment && attachment->data->light == nullptr) {
			log::getLog() << "[MWSE] Crash prevented while cleaning up light reference to object '" <<
				reference->baseObject->objectID << "' in cell '" << reference->getCell()->getEditorName() << "'. " <<
				"Please report this to the #mwse channel in the Morrowind Modding Community discord." << std::endl;
			return nullptr;
		}

		return attachment;
	}

	//
	// Patch: Cache values between dialogue filters.
	// 
	// Current additional caching:
	//  * speaker's dialogue
	//

	int __fastcall PatchDialogueFilterCacheGetDisposition(TES3::MobileNPC* npc) {
		if (TES3::Dialogue::cachedActorDisposition) {
			return TES3::Dialogue::cachedActorDisposition.value();
		}

		// Always allow the default behavior if something we hit a weird context.
		return npc->getDisposition();
	}

	//
	// Patch: Support custom class images.
	//

	__declspec(naked) void PatchAddCustomClassImageSupportSetup() {
		__asm {
			mov ecx, esi			// Size: 0x2. The Class*.
			mov edx, ebx			// Size: 0x2. The parent element pointer.
			nop						// Size: 0x5. Replaced with a call generation. Can't do so here, because offsets aren't accurate.
			nop						// ^
			nop						// ^
			nop						// ^
			nop						// ^
		}
	}
	constexpr auto PatchAddCustomClassImageSupport_size = 0x9;

	TES3::UI::Element* __fastcall PatchAddCustomClassImageSupport(const TES3::Class* charClass, TES3::UI::Element* parent) {
		auto result = charClass->getLevelUpImage();
		return parent->createImage(TES3::UI::ID_NULL, result.c_str(), false);
	}

	//
	// Patch: Unsummoned actor cleanup
	//

	TES3::MobileActor* __fastcall cleanupUnsummonedActor(TES3::Reference* reference) {
		TES3::MobileActor* mobileActor = reference->getAttachedMobileActor();
		auto worldController = TES3::WorldController::get();
		worldController->mobManager->removeMob(reference);
		return mobileActor;
	}

	//
	// Patch: Set ActiveMagicEffect.isIllegalSummon correctly on loading a savegame.
	//

	// Patches ActiveMagicManager::addLoadedMagicSourceInstance.
	__declspec(naked) void PatchLoadActiveMagicEffect() {
		__asm {
			mov edx, eax
			shr eax, 4				// eax >>= HarmfulBit
			and al, 1
			mov [esp+0x2C], al		// activeMagicEffect.isHarmful = al
			shr edx, 15				// edx >>= IllegalDaedraBit
			and dl, 1
			mov [esp+0x2D], dl		// activeMagicEffect.isIllegalSummon = dl
			mov ecx, esi
			call $ + 0x42763		// call MagicSourceCombo__getEffects
			mov cx, [eax+ebp+0xC]	// cx = effect[ebp]->duration
			mov [esp+0x2E], cx		// activeMagicEffect.duration = cx
			mov dx, [eax+ebp+0x10]	// dx = effect[ebp]->magnitudeMin
			mov [esp+0x30], dx		// activeMagicEffect.magnitudeMin = dx
			nop
			nop
		}
	}
	const size_t PatchLoadActiveMagicEffect_size = 0x32;

	//
	// Patch: Fix crash in NPC flee logic when trying to pick a random node from a pathgrid with 0 nodes.
	// 

	TES3::PathGrid* __fastcall PatchCellGetPathGridWithNodes(TES3::Cell* cell) {
		auto pathGrid = cell->pathGrid;
		if (pathGrid && pathGrid->nodeCount == 0) {
			return nullptr;
		}
		return pathGrid;
	}

	//
	// Patch: UI element image mirroring on negative image scale.
	// 

	// Mirror image texcoords with negative image scale.
	void __cdecl PatchUIElementTexcoordWrite(TES3::UI::Element* element, TES3::Vector2* texCoords) {
		float left = 0.0f, top = 0.0f, right = 1.0f, bottom = 1.0f;

		if (element->imageScaleX < 0) {
			std::swap(left, right);
		}
		if (element->imageScaleY < 0) {
			std::swap(top, bottom);
		}
		texCoords[0].x = left;
		texCoords[0].y = top;
		texCoords[1].x = left;
		texCoords[1].y = bottom;
		texCoords[2].x = right;
		texCoords[2].y = top;
		texCoords[3].x = right;
		texCoords[3].y = bottom;
	}

	// Change pixel width/height calculation to floor(abs(imageScale{X,Y} * texture{Width,Height}) + 0.5)
	const float f_half = 0.5f;
	__declspec(naked) void PatchUIUpdateLayoutImageContent1() {
		__asm {
			fmulp	st(1), st			// imageScale * textureDimension
			fabs						// abs
			fadd	[f_half]			// + 0.5
			fstp	qword ptr [esp]		// double argument for floor
		}
	}
	const size_t PatchUIUpdateLayoutImageContent1_size = 0xD;

	// Replace texcoord writer.
	__declspec(naked) void PatchUIUpdateLayoutImageContent2() {
		__asm {
			push eax		// texcoord data pointer
			push esi		// UiElement pointer
			nop				// call to patch placeholder
			nop
			nop
			nop
			nop
			add esp, 8
			xor ecx, ecx
			jmp $ + 0x51
		}
	}
	const size_t PatchUIUpdateLayoutImageContent2_size = 0x11;

	//
	// Patch: When adjusting effects mix volume, update looping audio volume correctly.
	//
	
	void __fastcall PatchSetLoopingSoundBufferVolume(TES3::AudioController* audio, DWORD unused, TES3::SoundEvent* soundEvent, unsigned char volume) {
		if (soundEvent->sound) {
			volume = (unsigned char)(float(volume) * float(soundEvent->sound->volume) / 255.0f);
		}
		audio->setSoundBufferVolume(soundEvent->soundBuffer, volume);
	}

	//
	// Patch: Add deterministic subtree ordering mode to NiSortAdjustNode. Fix cloning with no accumulator.
	//

	const auto NI_SortAdjustNode_Display = reinterpret_cast<void(__thiscall*)(NI::SortAdjustNode*, NI::Camera*)>(0x6DE030);
	const auto NI_ClusterAccumulator_RegisterObject = reinterpret_cast<void(__thiscall*)(NI::Accumulator*, NI::AVObject*)>(0x6CF200);

	void __fastcall PatchNISortAdjustNodeDisplay(NI::SortAdjustNode* node, DWORD unused, NI::Camera* camera) {
		// Add extra sort adjust mode for accumulating a node instead of geom.
		auto accumulator = camera->renderer->accumulator.get();
		if (node->sortingMode == NI::SortAdjustMode::SORTING_ORDERED_SUBTREE_MWSE
			&& accumulator != nullptr
			&& accumulator->isInstanceOfType(NI::RTTIStaticPtr::NiAlphaAccumulator)) {
			NI_ClusterAccumulator_RegisterObject(accumulator, node);
		}
		else {
			NI_SortAdjustNode_Display(node, camera);
		}
	}

	NI::Object* __fastcall PatchNISortAdjustNodeCloneAccumulator(NI::Accumulator* accumulator) {
		// Only call createClone if accumulator exists.
		return accumulator ? accumulator->vTable.asObject->createClone(accumulator) : nullptr;
	}

	//
	// Patch: Improve error reporting by including the source mod next to object IDs in load error messages.
	//

	static char tempErrorMessageObjectID[256];

	const char* __fastcall PatchGetImprovedObjectIdentifier(TES3::Object* object) {
		const auto id = object->getObjectID();
		const auto source = object->sourceMod ? object->sourceMod->filename : "no source";
		std::snprintf(tempErrorMessageObjectID, sizeof(tempErrorMessageObjectID), "%s' (%s)", id, source);
		return tempErrorMessageObjectID;
	}

	//
	// Patch: Do not load VFX with maxAge <= 0.001f from save games.
	//

	const auto TES3_VFXManager_createFromSaveData = reinterpret_cast<TES3::VFX* (__thiscall*)(TES3::VFXManager*, TES3::PhysicalObject*, TES3::Reference*, TES3::VFXSerialized*, float)>(0x468620);

	TES3::VFX* __fastcall PatchVFXManagerCreateFromSaveData(TES3::VFXManager* vfxManager, DWORD unused, TES3::PhysicalObject* effect, TES3::Reference* reference, TES3::VFXSerialized* serializedVFX, float verticalOffset) {
		// Do not load VFX with maxAge <= 0.001f, as they are persistent and may have accumulated in saves from before these fixes.
		if (serializedVFX->maxAge <= 0.001f) {
			return nullptr;
		}

		return TES3_VFXManager_createFromSaveData(vfxManager, effect, reference, serializedVFX, verticalOffset);
	}

	// Patch: Land textures loading/unloading flag array overflow bug. Increase array from 500 to 4096 elements and fix bounds checks.

	const unsigned short Land_LTEX_isLoaded_size = 4096;
	bool Land_LTEX_isLoaded[Land_LTEX_isLoaded_size];

	__declspec(naked) void PatchLandUnloadTexturesBoundsCheck() {
		__asm {
			// Replace index >= 500 and index != -1 with a single unsigned comparison against the new size.
			cmp ax, 4096 // immediate arg = Land_LTEX_isLoaded_size
			jnb $ + 0xAB
			nop
			nop
			nop
			nop
			nop
			nop
		}
	}
	const size_t PatchLandUnloadTexturesBoundsCheck_size = 0x10;
	
	__declspec(naked) void PatchLandLoadTexturesBoundsCheck() {
		__asm {
			// Replace index >= 500 and index != -1 with a single unsigned comparison against the new size.
			cmp cx, 4096 // immediate arg = Land_LTEX_isLoaded_size
			jnb $ + 0xF5
			nop
			nop
			nop
			nop
			nop
			nop
		}
	}
	const size_t PatchLandLoadTexturesBoundsCheck_size = 0x11;

	//
	// Patch: Fall back to reference rotation values when initializing animation controllers without a scene node.
	// 
	// Leaving here since the reporter couldn't reproduce the crash on a new save, but we'll want information next time this happens.
	//

	void __fastcall PatchSetAnimControllerMobile(TES3::ActorAnimationController* animController, DWORD _EDX_, TES3::MobileActor* mobile) {
		if (mobile == nullptr) {
			return;
		}

		// Try to get more information about this crash.
		if (mobile->reference->getSceneGraphNode() == nullptr) {
			log::getLog() << "[MWSE] No scene graph found when attempting to add animation controller to reference. Doing what we can with the reference. Please report this to the #mwse channel in the Morrowind Modding Community discord." << std::endl;
			safePrintObjectToLog("Reference", mobile->reference);
		}

		// Perform overwritten code, but use getRotationMatrix to fall back to reference rotation values.
		animController->mobileActor = mobile;
		animController->animationData = mobile->getAnimationData();
		animController->groundPlaneRotation = mobile->reference->getRotationMatrix();
	}

	//
	// Patch: Log stack traces of problematic UI pointer issues.
	//

	void __cdecl PatchLogUIMemoryPointerErrors(const char* message) {
		lua::logStackTrace("Lua traceback at time of invalid access:");
		tes3::logErrorAndSavePoint(message);
	}

	//
	// Patch: Respect symbolic links.
	// 
	// Unlike most of the Win32 API, FindFirstFileA and FindNextFileA don't respect symbolic links and
	// instead return information about the link itself instead of its target.
	// 
	// This patch makes it return the file size of the target file, rather than the symlink itself (0).
	//

	static std::unordered_map<HANDLE, std::string> findFilePaths;
	static std::recursive_mutex findFileMutex;

	std::optional<std::string> getFindFilePath(HANDLE hFindFile) {
		findFileMutex.lock();
		const auto itt = findFilePaths.find(hFindFile);
		if (itt == findFilePaths.end()) {
			findFileMutex.unlock();
			return {};
		}

		findFileMutex.unlock();
		return itt->second;
	}

	void PatchFixSymlinkData(HANDLE hFindFile, LPWIN32_FIND_DATAA lpFindFileData) {
		const auto path = getFindFilePath(hFindFile);
		if (!path) {
			return;
		}

		const auto fullPath = std::filesystem::path(path.value()) / lpFindFileData->cFileName;
		if (!std::filesystem::exists(fullPath)) {
			return;
		}

		const auto fileSize = std::filesystem::file_size(fullPath);
		lpFindFileData->nFileSizeHigh = unsigned int(fileSize / std::numeric_limits<unsigned int>::max());
		lpFindFileData->nFileSizeLow = unsigned int(fileSize);
	}

	HANDLE __stdcall PatchFindFirstFileA(LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData) {
		auto result = FindFirstFileA(lpFileName, lpFindFileData);
		if (result == INVALID_HANDLE_VALUE) {
			return result;
		}

		findFileMutex.lock();
		findFilePaths[result] = std::filesystem::path(lpFileName).parent_path().string();
		findFileMutex.unlock();

		// Check to see if it resolved to a symbolic link.
		if (lpFindFileData->dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT && lpFindFileData->dwReserved0 == IO_REPARSE_TAG_SYMLINK) {
			PatchFixSymlinkData(result, lpFindFileData);
		}

		return result;
	}

	BOOL __stdcall PatchFindNextFileA(HANDLE hFindFile, LPWIN32_FIND_DATAA lpFindFileData) {
		auto result = FindNextFileA(hFindFile, lpFindFileData);
		if (!result) {
			return result;
		}

		// Check to see if it resolved to a symbolic link.
		if (lpFindFileData->dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT && lpFindFileData->dwReserved0 == IO_REPARSE_TAG_SYMLINK) {
			PatchFixSymlinkData(hFindFile, lpFindFileData);
		}

		return result;
	}

	BOOL __stdcall PatchFindClose(HANDLE hFindFile) {
		findFileMutex.lock();
		const auto itt = findFilePaths.find(hFindFile);
		if (itt != findFilePaths.end()) {
			findFilePaths.erase(itt);
		}
		findFileMutex.unlock();

		return FindClose(hFindFile);
	}

	//
	// Patch: Fix crash when updating lights for a reference that has had a light unassigned.
	//

	static TES3::LightAttachmentNode* __fastcall PatchGetLightAttachmentIfItHasALight(TES3::Reference* reference) {
		const auto result = reference->getAttachedDynamicLight();
		if (result == nullptr) {
			return nullptr;
		}

		if (result->light == nullptr) {
			reference->deleteDynamicLightAttachment();
			return nullptr;
		}

		return result;
	}

	//
	// Patch: Guard against invalid light flicker/pulse updates.
	//

	const auto TES3_Light_UpdateFlickerPulse = reinterpret_cast<void(__thiscall*)(TES3::Light*, NI::Node*, float*, TES3::ItemData*)>(0x4D33D0);
	void __fastcall PatchEntityLightFlickerPulseUpdate(TES3::Light* light, DWORD _EDX_, NI::Node* sgNode, float* flickerPhase, TES3::ItemData* itemData) {
		if (sgNode == nullptr) {
#if _DEBUG
			log::getLog() << "[MWSE] Warning: Light '" << light->getObjectID() << "' attempting to update update flicker/phase without a scene graph node." << std::endl;
#endif
			return;
		}

		TES3_Light_UpdateFlickerPulse(light, sgNode, flickerPhase, itemData);
	}

	//
	// Patch: Resolve node count mismatch when loading pathgrid records with missing subrecords.
	// 

	__declspec(naked) void PatchPathGridLoader() {
		__asm {
			pop esi
			pop ebx
			mov ecx, ebp
			nop			// Replace with call
			nop
			nop
			nop
			nop
			jmp $ + 0x15
		}
	}
	const size_t PatchPathGridLoader_size = 0xE;

	void __fastcall PatchPathGridLoaderCheckNodeData(TES3::PathGrid* pathGrid) {
		// Check node count from record matches node data. Reset node count on mismatch.
		if (pathGrid->nodeCount != pathGrid->nodes.count) {
			log::getLog() << "[MWSE] Warning: Pathgrid in cell '" << pathGrid->parentCell->getEditorName() <<
				"' has mismatching path node count. nodeCount=" << pathGrid->nodeCount << ", node data count=" << pathGrid->nodes.count << std::endl;

			pathGrid->nodeCount = pathGrid->nodes.count;
		}

		// Perform overwritten code.
		if (TES3::WorldController::get()->menuController->gameplayFlags & 0x800000) {
			pathGrid->show();
		}
	}

	//
	// Patch: Allow bound armour function to also summon bracers and pauldrons.
	//

	const auto TES3_SwapBoundArmor = reinterpret_cast<bool (__cdecl*)(TES3::MagicEffectInstance*, const char*, const char*)>(0x465DE0);
	const auto TES3_UI_PostAddAndEquipBoundItem = reinterpret_cast<void(__cdecl*)(TES3::Item*, TES3::ItemData*, int)>(0x5D1F00);

	TES3::EquipmentStack* createEquipBoundItem(TES3::Item* item, TES3::Actor* actor, TES3::MobileActor* mobile) {
		// Create and equip bound item. Excerpted from bound gauntlet code.
		TES3::EquipmentStack* equipped = nullptr;
		auto itemData = TES3::ItemData::createForBoundItem(item);

		actor->inventory.addItem(mobile, item, 1, false, &itemData);
		actor->equipItem(item, itemData, &equipped, mobile);
		if (mobile->actorType == TES3::MobileActorType::Player) {
			TES3_UI_PostAddAndEquipBoundItem(item, itemData, 1);
		}
		mobile->wearItem(item, itemData, false, false, true);
		return equipped->canonicalCopy();
	}

	bool __cdecl PatchSwapBoundArmor(TES3::MagicEffectInstance* effectInstance, const char* armorId1, const char* armorId2) {
		auto records = TES3::DataHandler::get()->nonDynamicData;

		auto armor1 = static_cast<TES3::Armor*>(records->resolveObject(armorId1));
		auto armor2 = armorId2 ? static_cast<TES3::Armor*>(records->resolveObject(armorId2)) : nullptr;

		if (armor1 == nullptr || armor1->objectType != TES3::ObjectType::Armor) {
			return false;
		}

		if (armor1->slot == TES3::ArmorSlot::LeftBracer) {
			auto mobile = effectInstance->target->getAttachedMobileActor();
			auto actor = static_cast<TES3::Actor*>(effectInstance->target->baseObject);
			auto mcpGlovesWithBracers = mcp::getFeatureEnabled(mcp::feature::AllowGlovesWithBracers);

			// Left hand.
			// Un-equip and memorize any item in the same location.
			auto equipLeftHand = actor->getEquippedArmorBySlot(TES3::ArmorSlot::LeftGauntlet);
			if (!equipLeftHand) {
				equipLeftHand = actor->getEquippedArmorBySlot(TES3::ArmorSlot::LeftBracer);
			}
			if (!equipLeftHand && !mcpGlovesWithBracers) {
				equipLeftHand = actor->getEquippedClothingBySlot(TES3::ClothingSlot::LeftGlove);
			}

			if (equipLeftHand) {
				// The original left hand item is memorized in the lastUsedArmor member.
				effectInstance->lastUsedArmor = equipLeftHand->canonicalCopy();
				if (equipLeftHand->object == mobile->currentEnchantedItem.object) {
					effectInstance->lastUsedEnchItem = mobile->currentEnchantedItem.canonicalCopy();
				}
			}

			// Create bound item and record created stack.
			effectInstance->createdData.equipmentOrSummon = createEquipBoundItem(armor1, actor, mobile);

			// Right hand.
			if (armor2) {
				// Un-equip and memorize any item in the same location.
				auto equipRightHand = actor->getEquippedArmorBySlot(TES3::ArmorSlot::RightGauntlet);
				if (!equipRightHand) {
					equipRightHand = actor->getEquippedArmorBySlot(TES3::ArmorSlot::RightBracer);
				}
				if (!equipRightHand && !mcpGlovesWithBracers) {
					equipRightHand = actor->getEquippedClothingBySlot(TES3::ClothingSlot::RightGlove);
				}

				if (equipRightHand) {
					// The original right hand item is memorized in the lastUsedWeapon member.
					effectInstance->lastUsedWeapon = equipRightHand->canonicalCopy();
					if (equipRightHand->object == mobile->currentEnchantedItem.object) {
						effectInstance->lastUsedEnchItem = mobile->currentEnchantedItem.canonicalCopy();
					}
				}

				// Create bound item and record created stack.
				effectInstance->createdData2 = createEquipBoundItem(armor2, actor, mobile);
			}

			return true;
		}
		else if (armor1->slot == TES3::ArmorSlot::LeftPauldron) {
			auto mobile = effectInstance->target->getAttachedMobileActor();
			auto actor = static_cast<TES3::Actor*>(effectInstance->target->baseObject);

			// Left shoulder.
			// Un-equip and memorize any item in the same location.
			auto equipLeftPauldron = actor->getEquippedArmorBySlot(TES3::ArmorSlot::LeftPauldron);
			if (equipLeftPauldron) {
				// The original left side item is memorized in the lastUsedArmor member.
				effectInstance->lastUsedArmor = equipLeftPauldron->canonicalCopy();
				if (equipLeftPauldron->object == mobile->currentEnchantedItem.object) {
					effectInstance->lastUsedEnchItem = mobile->currentEnchantedItem.canonicalCopy();
				}
			}

			// Create bound item and record created stack.
			effectInstance->createdData.equipmentOrSummon = createEquipBoundItem(armor1, actor, mobile);

			// Right shoulder.
			if (armor2) {
				// Un-equip and memorize any item in the same location.
				auto equipRightPauldron = actor->getEquippedArmorBySlot(TES3::ArmorSlot::RightPauldron);
				if (equipRightPauldron) {
					// The original right side item is memorized in the lastUsedWeapon member.
					effectInstance->lastUsedWeapon = equipRightPauldron->canonicalCopy();
					if (equipRightPauldron->object == mobile->currentEnchantedItem.object) {
						effectInstance->lastUsedEnchItem = mobile->currentEnchantedItem.canonicalCopy();
					}
				}

				// Create bound item and record created stack.
				effectInstance->createdData2 = createEquipBoundItem(armor2, actor, mobile);
			}

			return true;
		}
		else {
			// Use original code for all other slots.
			return TES3_SwapBoundArmor(effectInstance, armorId1, armorId2);
		}
	}

	//
	// Patch: Modify proximity movement speed matching of AI followers to limit the speed match from going to zero on immobilized follow targets.
	//

	float __stdcall PatchGetAnimDataMovementSpeedCapped(TES3::AnimationData* animData) {
		// Restrict speed matching to be at least 60% of base animation speed.
		return std::max(0.6f, animData->movementSpeed);
	}

	__declspec(naked) void PatchMovementAnimSpeedMatching() {
		__asm {
			push eax
			call $					// Replace with call PatchGetAnimDataMovementSpeedCapped
			fstp [esp + 0x14]		// fst [targetMoveSpeed]
			fld [esp + 0x10]		// fld [finalMovementSpeed]
		}
	}
	const size_t PatchMovementAnimSpeedMatching_size = 0xE;

	//
	// Patch: Allow per-shape control of whether software or hardware skinning is used.
	//

	// Define a constant usable in inline asm.
	#define Const_SoftwareSkinningFlag 0x200
	static_assert(Const_SoftwareSkinningFlag == NI::TriShapeFlags::SoftwareSkinningFlag);

	__declspec(naked) void PatchNITriBasedGeom_Ctor1() {
		__asm {
			movzx eax, word ptr [esp + 0x1C]	// eax = zero extended triangleCount
			mov dword ptr [esi], 0x751268		// Set NiTriBasedGeom vtable
			mov [esi + 0x34], eax				// Initialize triangleCount and patchRenderFlags together
			nop
		}
	}
	const size_t PatchNITriBasedGeom_Ctor1_size = 0xF;

	__declspec(naked) void PatchNITriBasedGeom_Ctor2() {
		__asm {
			xor edx, edx
			mov [esi + 0x34], edx				// Initialize triangleCount and patchRenderFlags together
			nop
		}
	}
	const size_t PatchNITriBasedGeom_Ctor2_size = 0x6;

	const auto NI_TriBasedGeometry_CopyMembers = reinterpret_cast<void(__thiscall*)(NI::TriBasedGeometry*, NI::TriBasedGeometry*)>(0x6F15B0);
	void __fastcall PatchNITriShapeCopyMembers(NI::TriShape* _this, DWORD _EDX_, NI::TriShape* to) {
		NI_TriBasedGeometry_CopyMembers(_this, to);

		// Ensure that if the geometry data has been deep cloned, that the render flags are copied too.
		if (to->modelData != _this->modelData) {
			to->getModelData()->patchRenderFlags = _this->getModelData()->patchRenderFlags;
		}
	}

	__declspec(naked) void PatchNIDX8Renderer_RenderShape() {
		__asm {
			nop
			test word ptr [esi + 0x36], Const_SoftwareSkinningFlag	// Skip hardware skinning if patchRenderFlags matches SoftwareSkinningFlag
			__asm _emit 0x75 __asm _emit 0x19						// jnz short $ + 0x1B (assembler can't output short offsets correctly)
		}
	}
	const size_t PatchNIDX8Renderer_RenderShape_size = 0x8;

	//
	// Patch: Fix cure spells incorrectly triggering MagicEffectState_Ending for magic that hasn't taken effect yet.
	//

	__declspec(naked) void PatchRemoveMagicsByEffect() {
		__asm {
			cmp byte ptr [esp + 0x4C], 5		// if (magicEffectInstance.state == MagicEffectState_Working)
			jnz done
			mov byte ptr [esp + 0x4C], 6		// magicEffectInstance.state = MagicEffectState_Ending
		done:
			ret
		}
	}

	//
	// Patch: Fix loading crashes where there are links to missing objects from mods that were removed.
	//

	// Prevent crashes when a casting item is no longer present.
	__declspec(naked) void PatchMagicSourceInstanceDtor() {
		__asm {
			test edi, edi						// if (!castingItem)
			__asm _emit 0x74 __asm _emit 0x45	// jz short $ + 0x47 (assembler can't output short offsets correctly)
			lea esi, [edi+4]					// esi = &castingItem.objectType
			mov eax, [esi]						// eax = castingItem.objectType
			nop
		}
	}
	const size_t PatchMagicSourceInstanceDtor_size = 0xA;

	// Prevent deleting itemData when a soul trapped creature is no longer present.
	__declspec(naked) void PatchSoulTrappedCreatureNotFound1() {
		__asm {
			add esp, 0x8
			__asm _emit 0xEB __asm _emit 0x0D	// jmp short $ + 0xF (assembler can't output short offsets correctly)
		}
	}
	const size_t PatchSoulTrappedCreatureNotFound1_size = 0x5;

	__declspec(naked) void PatchSoulTrappedCreatureNotFound2() {
		__asm {
			add esp, 0x8
			__asm _emit 0xEB __asm _emit 0x09	// jmp short $ + 0xB (assembler can't output short offsets correctly)
		}
	}
	const size_t PatchSoulTrappedCreatureNotFound2_size = 0x5;

	__declspec(naked) void PatchSoulTrappedCreatureNotFound3() {
		__asm {
			add esp, 0x8
			__asm _emit 0xEB __asm _emit 0x17	// jmp short $ + 0x19 (assembler can't output short offsets correctly)
		}
	}
	const size_t PatchSoulTrappedCreatureNotFound3_size = 0x5;

	//
	// Patch: Prevent crash with magic effects on invalid targets.
	//

	void __cdecl PatchMagicEffectFortifySkill(TES3::MagicSourceInstance* sourceInstance, float deltaTime, TES3::MagicEffectInstance* effectInstance, int effectIndex) {
		auto mobile = effectInstance->target->getAttachedMobileActor();
		if (mobile == nullptr) {
			return;
		}

		const auto MagicEffectFortifySkill = reinterpret_cast<void(__cdecl*)(TES3::MagicSourceInstance*, float, TES3::MagicEffectInstance*, int)>(0x4625F0);
		MagicEffectFortifySkill(sourceInstance, deltaTime, effectInstance, effectIndex);
	}

	//
	// Patch: Suppress sGeneralMastPlugMismatchMsg message.
	//

	std::optional<UINT> AllowYesToAll = {};

	static UINT __stdcall GetCachedYesToAll(LPCSTR lpAppName, LPCSTR lpKeyName, INT nDefault, LPCSTR lpFileName) {
		if (!AllowYesToAll.has_value()) {
			AllowYesToAll = GetPrivateProfileIntA(lpAppName, lpKeyName, nDefault, lpFileName);
		}

		return AllowYesToAll.value_or(FALSE);
	}

	static void __cdecl SuppressGeneralMastPlugMismatchMsg(const char* sGeneralMastPlugMismatchMsg) {
		// Prevent the message from even showing.
		if (Configuration::SuppressUselessWarnings) {
			return;
		}

		// Display the message, but prevent yes to all from being used.
		decltype(AllowYesToAll) cachedYesToAll = FALSE;
		std::swap(cachedYesToAll, AllowYesToAll);
		tes3::logAndShowError(sGeneralMastPlugMismatchMsg);
		std::swap(cachedYesToAll, AllowYesToAll);
	}

	//
	// Patch: Make checking the object type of nullptr objects return an invalid type instead of crashing.
	//

	TES3::ObjectType::ObjectType __fastcall PatchSafeGetObjectType(TES3::ObjectType::ObjectType* objectType) {
		if (objectType == nullptr || size_t(objectType) == offsetof(TES3::BaseObject, objectType)) {
			return TES3::ObjectType::Invalid;
		}
		return *objectType;
	}

	void DoPatchSafeGetObjectType() {
		constexpr DWORD patchAddresses[] = { 0x41106E, 0x41CB71, 0x41CBA8, 0x41CBBA, 0x41CC93, 0x41CD68, 0x41CD80, 0x41D7A3, 0x41F59F, 0x42103B, 0x421373, 0x455165, 0x45517A, 0x4551B9, 0x45FA0E, 0x45FA22, 0x45FA3A, 0x45FAA9, 0x45FBC9, 0x45FBDD, 0x45FBF5, 0x45FC62, 0x45FC99, 0x45FD79, 0x45FE80, 0x4607A0, 0x4607B4, 0x4608E1, 0x4608F5, 0x460941, 0x463416, 0x464D25, 0x464D67, 0x464DC0, 0x464E4D, 0x464EB4, 0x4657D6, 0x465802, 0x465907, 0x465E2D, 0x465E45, 0x465FA2, 0x465FAF, 0x4666E7, 0x4667EB, 0x46F59C, 0x46FBA1, 0x46FBD4, 0x472F15, 0x47319A, 0x473263, 0x473334, 0x47367C, 0x47369B, 0x473787, 0x47393F, 0x4739A0, 0x473CCE, 0x473D84, 0x473DC5, 0x473E21, 0x473F7B, 0x474021, 0x474174, 0x474238, 0x484C62, 0x484DC2, 0x484ED0, 0x485FFD, 0x486365, 0x486433, 0x488B2F, 0x48B16E, 0x48B20C, 0x48B233, 0x48B253, 0x48B2E8, 0x48B4B1, 0x48B548, 0x48B712, 0x48B73A, 0x48B764, 0x48B86B, 0x48BA9C, 0x48BCBF, 0x48BD64, 0x48BFB3, 0x48C089, 0x48C2D4, 0x48C3D2, 0x48C463, 0x4951E5, 0x49526F, 0x495335, 0x495380, 0x4954BE, 0x4954F5, 0x495561, 0x4955A6, 0x4956AE, 0x495811, 0x495854, 0x4958F1, 0x49595C, 0x4959B7, 0x495A50, 0x495AF0, 0x495B6D, 0x495BC9, 0x495C0B, 0x495C67, 0x495CB6, 0x495D22, 0x495DE2, 0x495E9C, 0x495F20, 0x495F97, 0x495FCE, 0x496096, 0x4960FB, 0x496123, 0x49617B, 0x4961D7, 0x496219, 0x4962C8, 0x496314, 0x4964ED, 0x496515, 0x49655B, 0x49672A, 0x4968C1, 0x496E1D, 0x496E7D, 0x496EC8, 0x496F18, 0x496F58, 0x497030, 0x497BEB, 0x497D37, 0x497E72, 0x49817A, 0x498598, 0x498764, 0x499182, 0x49938C, 0x4999CE, 0x499ABF, 0x499AD1, 0x499AE3, 0x499CB3, 0x499D7D, 0x49A1BD, 0x49A5A2, 0x49A5F9, 0x49A758, 0x49A7D8, 0x49A7EA, 0x49A8AD, 0x49A8BA, 0x49AA36, 0x49AA48, 0x49ABE8, 0x49ABFA, 0x49ACAF, 0x49ACC1, 0x49B51C, 0x49B595, 0x49B640, 0x49B6BD, 0x49B793, 0x49D1BB, 0x49DC83, 0x49E8BE, 0x49E8CC, 0x49EA7E, 0x49EB4E, 0x49EC91, 0x49EFA8, 0x49FD2B, 0x4A01EB, 0x4A0BFE, 0x4A171B, 0x4A24EB, 0x4A359E, 0x4A407B, 0x4A4A8B, 0x4A525E, 0x4A526C, 0x4A56BB, 0x4A5C7B, 0x4A61BB, 0x4A67AB, 0x4A6C7B, 0x4A716B, 0x4A74DB, 0x4AB36B, 0x4AC78E, 0x4AC79C, 0x4AC88B, 0x4ACAD6, 0x4B01BD, 0x4B01DC, 0x4B02FE, 0x4B0851, 0x4B0C16, 0x4B0C5D, 0x4B0CA4, 0x4B0CE9, 0x4B0ED0, 0x4B1076, 0x4B119F, 0x4B15D0, 0x4B1636, 0x4B166B, 0x4B172E, 0x4B1E68, 0x4B5B44, 0x4B5BD4, 0x4B5CF9, 0x4B5D50, 0x4B5DC6, 0x4B5DEF, 0x4B5E22, 0x4B5E49, 0x4B606D, 0x4B6076, 0x4B8880, 0x4B8ADD, 0x4B9007, 0x4B92E2, 0x4B9520, 0x4B9A9D, 0x4B9D79, 0x4B9D98, 0x4B9E0A, 0x4B9FEC, 0x4BA03C, 0x4BA0B8, 0x4BA108, 0x4BA183, 0x4BA1A6, 0x4BA489, 0x4BA946, 0x4BA95A, 0x4BAEAB, 0x4BAF53, 0x4BB7E8, 0x4BB9F1, 0x4BBDFE, 0x4BBE1E, 0x4BBE2B, 0x4BBE3B, 0x4C0BE7, 0x4C0BF1, 0x4C0C49, 0x4C0C70, 0x4C1188, 0x4C18FB, 0x4C1A4A, 0x4C1E6F, 0x4C2096, 0x4C21A5, 0x4C37ED, 0x4C3B28, 0x4C3BEB, 0x4C3CCF, 0x4C3D29, 0x4C55CE, 0x4C5744, 0x4C5960, 0x4C5B68, 0x4C604C, 0x4C6408, 0x4C6817, 0x4C6C0C, 0x4C71E1, 0x4C722B, 0x4C73DD, 0x4C74F3, 0x4C79E7, 0x4C7E83, 0x4C848B, 0x4C84A6, 0x4CF9A0, 0x4D0DC3, 0x4D212B, 0x4D280E, 0x4D2847, 0x4D2C7D, 0x4D2CE5, 0x4D2D80, 0x4D332E, 0x4D734B, 0x4D8A27, 0x4D987E, 0x4D988C, 0x4D9AE4, 0x4D9CF2, 0x4D9DA5, 0x4D9DDE, 0x4DA00F, 0x4DA0A2, 0x4DBBE6, 0x4DBD77, 0x4DBF2C, 0x4DBF82, 0x4DBFE7, 0x4DC046, 0x4DC1B2, 0x4DC2E9, 0x4DC81B, 0x4DDD7B, 0x4DDE1C, 0x4DDEAA, 0x4DDEF2, 0x4DE077, 0x4DE081, 0x4DE08B, 0x4DE944, 0x4DEB23, 0x4DEC21, 0x4DF596, 0x4DF84F, 0x4E0D48, 0x4E12FC, 0x4E1646, 0x4E1683, 0x4E173A, 0x4E17E4, 0x4E19C1, 0x4E1A46, 0x4E2B0F, 0x4E491A, 0x4E4928, 0x4E4936, 0x4E497F, 0x4E4AD9, 0x4E4DCC, 0x4E5687, 0x4E5786, 0x4E633D, 0x4E6E83, 0x4E7136, 0x4E7199, 0x4E77A3, 0x4E79CF, 0x4E7E0F, 0x4E7E21, 0x4E7E33, 0x4E7E9F, 0x4E7EFC, 0x4E7F60, 0x4E813B, 0x4E839A, 0x4E8503, 0x4E85C5, 0x4E85DB, 0x4E86CA, 0x4E8BC6, 0x4E8C94, 0x4E8D62, 0x4E8F9C, 0x4E90B9, 0x4E91D2, 0x4E9452, 0x4E9710, 0x4E9754, 0x4E994A, 0x4E9981, 0x4E9A8A, 0x4EAF5E, 0x4EB0ED, 0x4EB16D, 0x4EB183, 0x4EB199, 0x4EB1CD, 0x4EB62D, 0x4EB816, 0x4EBB16, 0x4EBB28, 0x4EBE4F, 0x4EBEA5, 0x4F260B, 0x4F27BF, 0x4F7229, 0x4F7468, 0x4F9FF1, 0x4FA0A6, 0x4FA0B4, 0x4FA0C2, 0x4FA22C, 0x4FA2DF, 0x4FE0BE, 0x4FE0CC, 0x4FE0E8, 0x4FE0F6, 0x4FE110, 0x4FE252, 0x4FE2AD, 0x4FE2BB, 0x4FE2C9, 0x4FE346, 0x4FE39B, 0x4FECD6, 0x4FECE4, 0x4FED04, 0x4FED12, 0x4FED30, 0x5057F2, 0x506753, 0x507303, 0x507323, 0x50733F, 0x507764, 0x5077BA, 0x508054, 0x5080B0, 0x50856E, 0x5085D5, 0x5086B6, 0x508741, 0x50893D, 0x50899D, 0x508A1A, 0x508A75, 0x508ACB, 0x508B23, 0x508CB5, 0x508CC8, 0x508D5A, 0x50904B, 0x509627, 0x509664, 0x509707, 0x5098EF, 0x509A52, 0x50A4CF, 0x50A5FD, 0x50A722, 0x50A858, 0x50A910, 0x50A9C8, 0x50AA32, 0x50AAD3, 0x50ABD0, 0x50BA30, 0x50BC8B, 0x50BE1C, 0x50BE88, 0x50BF8C, 0x50C196, 0x50C233, 0x50C248, 0x50C256, 0x50C643, 0x50EC97, 0x50ECC4, 0x50ED38, 0x50EFC6, 0x50F03E, 0x5116C0, 0x51248A, 0x512498, 0x51385A, 0x51415C, 0x514A22, 0x514D7B, 0x514FD1, 0x515052, 0x51522F, 0x5155D0, 0x51572B, 0x515DDB, 0x5165F7, 0x516E41, 0x517890, 0x51793C, 0x5179A9, 0x517C13, 0x518ADE, 0x518B6B, 0x518BC1, 0x5206EA, 0x520744, 0x520896, 0x5208F0, 0x5234C9, 0x525092, 0x527F5F, 0x528082, 0x52B241, 0x52B333, 0x52C078, 0x52C0AF, 0x52C1A5, 0x52C1B9, 0x52C844, 0x52CCD7, 0x52CDB4, 0x52CDC2, 0x52CDD0, 0x52CDDE, 0x52CDEC, 0x52CEE8, 0x533691, 0x533F61, 0x53753A, 0x53767E, 0x537ACF, 0x53836F, 0x53843E, 0x53AFF7, 0x53DEEA, 0x54C9D7, 0x54CB05, 0x54D342, 0x54FF4C, 0x550EB6, 0x551B06, 0x55A52B, 0x55F120, 0x55F219, 0x55F4E3, 0x55F5D8, 0x5634FD, 0x563755, 0x563819, 0x565FC4, 0x5677C9, 0x5699DF, 0x569A96, 0x569ACD, 0x569B9F, 0x56B1C2, 0x56B1D0, 0x56B1DE, 0x56B1EC, 0x56B233, 0x56B241, 0x56B2B6, 0x56B2C8, 0x56B2D6, 0x56B2E4, 0x56B32F, 0x56B33D, 0x56B3FF, 0x56B40D, 0x56B41B, 0x56B429, 0x56B470, 0x56B47E, 0x56B4F1, 0x56B503, 0x56B511, 0x56B51F, 0x56B56A, 0x56B578, 0x56C149, 0x56C208, 0x56C375, 0x56C42C, 0x56F05D, 0x573435, 0x5738F1, 0x5743B7, 0x574C92, 0x58FD49, 0x58FEA7, 0x590449, 0x590DFB, 0x590E5E, 0x590E73, 0x590E85, 0x590E97, 0x590EA9, 0x590EBB, 0x590EC9, 0x590ED7, 0x590EE5, 0x590EF3, 0x590F01, 0x590F0F, 0x590F1D, 0x590F2B, 0x590F3D, 0x590FE0, 0x5910B6, 0x591637, 0x5916C9, 0x5917FE, 0x591EB1, 0x592B54, 0x595749, 0x59575A, 0x59A16F, 0x59A19F, 0x59A1CF, 0x59A1FF, 0x59A22F, 0x59ACA8, 0x59D1A9, 0x5A3C98, 0x5A3CBF, 0x5A3CE6, 0x5A4704, 0x5A5492, 0x5A5F10, 0x5A63FF, 0x5A6414, 0x5B447B, 0x5B4489, 0x5B44DA, 0x5B4719, 0x5B5002, 0x5B502C, 0x5B5067, 0x5B51DD, 0x5B5409, 0x5B54CD, 0x5B56E8, 0x5B59DF, 0x5B5C6D, 0x5B5E06, 0x5B6012, 0x5B6028, 0x5B7224, 0x5B76C5, 0x5B7A8E, 0x5B7CA5, 0x5B7CD6, 0x5B7DCE, 0x5B7DE7, 0x5BF3CC, 0x5C47FD, 0x5C497A, 0x5C4A1E, 0x5C4AC5, 0x5C55DA, 0x5C615A, 0x5C6171, 0x5C61B2, 0x5C6B4E, 0x5CA998, 0x5CA9BF, 0x5CA9E6, 0x5CB836, 0x5CC782, 0x5CD901, 0x5CE309, 0x5D095E, 0x5D098F, 0x5D142B, 0x5D36B9, 0x5D36C7, 0x5D36D5, 0x5D3790, 0x5D3865, 0x5D3A5E, 0x5D3C61, 0x5D4069, 0x5D4505, 0x5D450F, 0x5D461F, 0x5D4628, 0x5D463B, 0x5D4644, 0x5E0090, 0x5E00D5, 0x5E0738, 0x5E212A, 0x5E2198, 0x5E31AA, 0x5E31EF, 0x5E38BE, 0x5E4355, 0x5ED061, 0x5ED073, 0x5ED0A0, 0x5ED0E4, 0x5ED0F6, 0x5ED123, 0x5ED157, 0x5F72E9, 0x5F72FA, 0x5FE2DE, 0x608759, 0x608AA7, 0x60AFF8, 0x60D7A7, 0x60D8F8, 0x60E365, 0x61527E, 0x615290, 0x631A50, 0x631A5C, 0x631B70, 0x631B7A, 0x631B91, 0x631B9B, 0x631CB8, 0x631CC4, 0x631DF8, 0x631E02, 0x631E1B, 0x631E25, 0x63261D, 0x633413, 0x6335FA, 0x63365D, 0x633740, 0x63384E, 0x633888, 0x633914, 0x6339A5, 0x633C98, 0x633CC1, 0x633D53, 0x633DB6, 0x635373, 0x6EA072, 0x6EA08C, 0x6EA097, 0x6EA0BA, 0x6EA1B3, 0x6EA721, 0x6EA794, 0x6EA7A0, 0x6EA7AB, 0x6EA7B7, 0x6EA7CB, 0x6EA7D7, 0x6EA7DF, 0x6EA8D8 };
		for (auto address : patchAddresses) {
			genCallEnforced(address, 0x4EE8B0, reinterpret_cast<DWORD>(PatchSafeGetObjectType));
		}
	}

	//
	// Patch: Expand keyboard key translations
	//

	inline static void WritePatchKeyCharacter(unsigned int key, char character) {
		writeValueEnforced<char>(0x775148 + key, 0, character); // US, Unshifted
		writeValueEnforced<char>(0x775248 + key, 0, character); // US, Shifted
		writeValueEnforced<char>(0x775348 + key, 0, character); // DE, Unshifted
		writeValueEnforced<char>(0x775448 + key, 0, character); // DE, Shifted
		writeValueEnforced<char>(0x775548 + key, 0, character); // FR, Unshifted
		writeValueEnforced<char>(0x775648 + key, 0, character); // FR, Shifted
	}

	static void PatchExpandKeyboardCharacterTranslations() {
		WritePatchKeyCharacter(DIK_NUMPAD0, '0');
		WritePatchKeyCharacter(DIK_NUMPAD1, '1');
		WritePatchKeyCharacter(DIK_NUMPAD2, '2');
		WritePatchKeyCharacter(DIK_NUMPAD3, '3');
		WritePatchKeyCharacter(DIK_NUMPAD4, '4');
		WritePatchKeyCharacter(DIK_NUMPAD5, '5');
		WritePatchKeyCharacter(DIK_NUMPAD6, '6');
		WritePatchKeyCharacter(DIK_NUMPAD7, '7');
		WritePatchKeyCharacter(DIK_NUMPAD8, '8');
		WritePatchKeyCharacter(DIK_NUMPAD9, '9');
	}

	//
	// Patch: Fix crash when trying to unequip a nocked projectile item while still using an item index for its position.
	// 
	// Before serializing, the nocked projectile is converted into an item index. Some mods may try to unequip the index
	// before it is resolved back into an actual projectile. This will prevent the crash.
	//

	static void __fastcall PatchUnequipIndexedProjectile(TES3::MobileActor* mobile) {
		auto& actionData = mobile->actionData;

		// Only call the destructor if the value can reasonably be a pointer.
		if (size_t(actionData.nockedProjectile) > 0x70000u) {
			actionData.nockedProjectile->vTable.mobileObject->destructor(actionData.nockedProjectile, true);
		}

		actionData.nockedProjectile = nullptr;
	}

	__declspec(naked) void PatchUnequipIndexedProjectileSetup() {
		__asm {
			mov ecx, ebx // Size: 0x2
		}
	}
	constexpr size_t PatchUnequipIndexedProjectileSetup_size = 0x2;

	//
	// Patch: Improve lights
	//

	const auto TES3_DynamicLightingTest = reinterpret_cast<void(__cdecl*)(NI::PointLight * light, NI::Node * node, int radius, int lightFlags, bool isLand, bool highPriority)>(0x4D2F40);
	static void __cdecl PatchDynamicLightingTest(NI::PointLight* light, NI::Node* node, int radius, int lightFlags, bool isLand, bool highPriority) {
		if (!Configuration::ReplaceLightSorting) {
			TES3_DynamicLightingTest(light, node, radius, lightFlags, isLand, highPriority);
			return;
		}

		if (light == nullptr || node == nullptr) {
			return;
		}

		// Store information about the light into the light itself. Because that's what Morrowind does.
		light->setFlag(highPriority, 3u);
		light->specular = { float(radius), float(radius), float(radius) };

		node->updatePointLight(light, isLand);
	}

	//
	// Install all the patches.
	//

	void installPatches() {
		// Patch: Enable/Disable.
		genCallUnprotected(0x508FEB, reinterpret_cast<DWORD>(PatchScriptOpEnable), 0x9);
		genCallUnprotected(0x5090DB, reinterpret_cast<DWORD>(PatchScriptOpDisable), 0x9);
		genCallUnprotected(0x50912F, reinterpret_cast<DWORD>(PatchScriptOpDisableCollision));

		// Patch: Unify athletics and sneak training.
		genCallUnprotected(0x569EE7, reinterpret_cast<DWORD>(PatchUnifyAthleticsTraining), 0xC6);
		genCallUnprotected(0x5683D0, reinterpret_cast<DWORD>(PatchUnifySneakTraining), 0x65);

		// Patch: Crash fix for help text for paperdolls.
		genCallEnforced(0x5CDFD0, 0x581440, reinterpret_cast<DWORD>(PatchPaperdollTooltipCrashFix));

		// Patch: Optimize GetDeadCount and associated dialogue filtering/logic.
		auto killCounter_increment = &TES3::KillCounter::incrementMobile;
		genCallEnforced(0x523D73, 0x55D820, *reinterpret_cast<DWORD*>(&killCounter_increment));
		auto killCounter_getCount = &TES3::KillCounter::getKillCount;
		genCallEnforced(0x4B0B2E, 0x55D900, *reinterpret_cast<DWORD*>(&killCounter_getCount));
		genCallEnforced(0x50AC85, 0x55D900, *reinterpret_cast<DWORD*>(&killCounter_getCount));
		genCallEnforced(0x50ACAB, 0x55D900, *reinterpret_cast<DWORD*>(&killCounter_getCount));
		genCallEnforced(0x745FF0, 0x55D900, *reinterpret_cast<DWORD*>(&killCounter_getCount));
#if MWSE_CUSTOM_KILLCOUNTER
		auto killCounter_ctor = &TES3::KillCounter::ctor;
		genCallEnforced(0x40DE9B, 0x55D750, *reinterpret_cast<DWORD*>(&killCounter_ctor));
		auto killCounter_dtor = &TES3::KillCounter::dtor;
		genCallEnforced(0x40E049, 0x55D7D0, *reinterpret_cast<DWORD*>(&killCounter_dtor));
		auto killCounter_clear = &TES3::KillCounter::clear;
		genCallEnforced(0x4C6F76, 0x55DBD0, *reinterpret_cast<DWORD*>(&killCounter_clear));
		auto killCounter_load = &TES3::KillCounter::load;
		genCallEnforced(0x4C076C, 0x55DA90, *reinterpret_cast<DWORD*>(&killCounter_load));
		auto killCounter_save = &TES3::KillCounter::save;
		genCallEnforced(0x4BCB7E, 0x55D950, *reinterpret_cast<DWORD*>(&killCounter_save));
#endif

		// Patch: Post-simulate event just before tickClock.
		// Patch: Don't truncate hour when advancing time past midnight.
		// Also don't nudge time forward by small extra increments when resting.
		auto WorldController_tickClock = &TES3::WorldController::tickClock;
		genCallEnforced(0x41B857, 0x40FF50, *reinterpret_cast<DWORD*>(&WorldController_tickClock));
		auto WorldController_checkForDayWrapping = &TES3::WorldController::checkForDayWrapping;
		genCallEnforced(0x6350E9, 0x40FF50, *reinterpret_cast<DWORD*>(&WorldController_checkForDayWrapping));

		// Patch: Prevent error messageboxes from creating a rogue process.
		genCallEnforced(0x47731B, 0x5F2160, reinterpret_cast<DWORD>(SafeQuitGetMessageChoice));
		genCallEnforced(0x4779D9, 0x5F2160, reinterpret_cast<DWORD>(SafeQuitGetMessageChoice));
		genCallEnforced(0x477E6F, 0x5F2160, reinterpret_cast<DWORD>(SafeQuitGetMessageChoice));

		// Patch: Cache DontThreadLoad INI value and extend it with a suppression flag.
		TES3::DataHandler::dontThreadLoad = GetPrivateProfileIntA("General", "DontThreadLoad", 0, ".\\Morrowind.ini") != 0;
		genCallUnprotected(0x48539C, reinterpret_cast<DWORD>(OverrideDontThreadLoad), 0x6);
		genCallUnprotected(0x4869DB, reinterpret_cast<DWORD>(OverrideDontThreadLoad), 0x6);
		genCallUnprotected(0x48F489, reinterpret_cast<DWORD>(OverrideDontThreadLoad), 0x6);
		genCallUnprotected(0x4904D0, reinterpret_cast<DWORD>(OverrideDontThreadLoad), 0x6);

		// Patch: Fix NiLinesData binary loading.
		auto NiLinesData_loadBinary = &NI::LinesData::loadBinary;
		overrideVirtualTableEnforced(0x7501E0, offsetof(NI::Object_vTable, loadBinary), 0x6DA410, *reinterpret_cast<DWORD*>(&NiLinesData_loadBinary));

		// Patch: Try to catch bogus collisions.
		auto MobileObject_Collision_clone = &TES3::MobileObject::Collision::clone;
		genCallEnforced(0x537107, 0x405450, *reinterpret_cast<DWORD*>(&MobileObject_Collision_clone));
		genCallEnforced(0x55F7C4, 0x405450, *reinterpret_cast<DWORD*>(&MobileObject_Collision_clone));
		genCallEnforced(0x55F818, 0x405450, *reinterpret_cast<DWORD*>(&MobileObject_Collision_clone));

		// Patch: Fix up transparency.
		auto BodyPartManager_updateForReference = &TES3::BodyPartManager::updateForReference;
		genCallEnforced(0x46444C, 0x473EA0, *reinterpret_cast<DWORD*>(&BodyPartManager_updateForReference));
		genCallEnforced(0x4DA07C, 0x473EA0, *reinterpret_cast<DWORD*>(&BodyPartManager_updateForReference));

		// Patch: Decrease MO2 load times. Somehow...
		writeDoubleWordUnprotected(0x7462F4, reinterpret_cast<DWORD>(&_stat32));

		// Patch: Fix NiUVController losing its texture set on clone.
		auto UVController_clone = &NI::UVController::copy;
		genCallEnforced(0x722317, 0x722330, *reinterpret_cast<DWORD*>(&UVController_clone));

		// Patch: Make globals less slow to access.
#if MWSE_CUSTOM_GLOBALS
		genCallEnforced(0x4B7D74, 0x47E1E0, reinterpret_cast<DWORD>(DataHandlerCreateGlobalsContainer));
		genCallUnprotected(0x4B8154, reinterpret_cast<DWORD>(DataHandlerDestroyGlobalsContainer), 6);
		genCallEnforced(0x41A029, 0x40E920, reinterpret_cast<DWORD>(WorldControllerInitGlobals));
		genCallEnforced(0x4C6012, 0x40E920, reinterpret_cast<DWORD>(WorldControllerInitGlobals));
		genCallEnforced(0x5FB10F, 0x40E920, reinterpret_cast<DWORD>(WorldControllerInitGlobals));
		genCallEnforced(0x5FE91E, 0x40E920, reinterpret_cast<DWORD>(WorldControllerInitGlobals));
		auto GlobalHashContainer_addVariable = &TES3::GlobalHashContainer::addVariable;
		genCallEnforced(0x4BD8AF, 0x47E360, *reinterpret_cast<DWORD*>(&GlobalHashContainer_addVariable));
		genCallEnforced(0x4BD906, 0x47E360, *reinterpret_cast<DWORD*>(&GlobalHashContainer_addVariable));
		genCallEnforced(0x565E0B, 0x47E360, *reinterpret_cast<DWORD*>(&GlobalHashContainer_addVariable));
		genCallEnforced(0x565E9A, 0x47E360, *reinterpret_cast<DWORD*>(&GlobalHashContainer_addVariable));
		auto DataHandlerNonDynamicData_findGlobal = &TES3::NonDynamicData::findGlobalVariable;
		genCallEnforced(0x40C243, 0x4BA820, *reinterpret_cast<DWORD*>(&DataHandlerNonDynamicData_findGlobal));
		genCallEnforced(0x40E9AC, 0x4BA820, *reinterpret_cast<DWORD*>(&DataHandlerNonDynamicData_findGlobal));
		genCallEnforced(0x40EA4D, 0x4BA820, *reinterpret_cast<DWORD*>(&DataHandlerNonDynamicData_findGlobal));
		genCallEnforced(0x40EAEE, 0x4BA820, *reinterpret_cast<DWORD*>(&DataHandlerNonDynamicData_findGlobal));
		genCallEnforced(0x40EB8F, 0x4BA820, *reinterpret_cast<DWORD*>(&DataHandlerNonDynamicData_findGlobal));
		genCallEnforced(0x40EC30, 0x4BA820, *reinterpret_cast<DWORD*>(&DataHandlerNonDynamicData_findGlobal));
		genCallEnforced(0x40ECD1, 0x4BA820, *reinterpret_cast<DWORD*>(&DataHandlerNonDynamicData_findGlobal));
		genCallEnforced(0x40ED72, 0x4BA820, *reinterpret_cast<DWORD*>(&DataHandlerNonDynamicData_findGlobal));
		genCallEnforced(0x40EE13, 0x4BA820, *reinterpret_cast<DWORD*>(&DataHandlerNonDynamicData_findGlobal));
		genCallEnforced(0x49D893, 0x4BA820, *reinterpret_cast<DWORD*>(&DataHandlerNonDynamicData_findGlobal));
		genCallEnforced(0x4A4860, 0x4BA820, *reinterpret_cast<DWORD*>(&DataHandlerNonDynamicData_findGlobal));
		genCallEnforced(0x4AFB5C, 0x4BA820, *reinterpret_cast<DWORD*>(&DataHandlerNonDynamicData_findGlobal));
		genCallEnforced(0x4D85FE, 0x4BA820, *reinterpret_cast<DWORD*>(&DataHandlerNonDynamicData_findGlobal));
		genCallEnforced(0x4DF4F2, 0x4BA820, *reinterpret_cast<DWORD*>(&DataHandlerNonDynamicData_findGlobal));
		genCallEnforced(0x4F93B9, 0x4BA820, *reinterpret_cast<DWORD*>(&DataHandlerNonDynamicData_findGlobal));
		genCallEnforced(0x4FCCC3, 0x4BA820, *reinterpret_cast<DWORD*>(&DataHandlerNonDynamicData_findGlobal));
		genCallEnforced(0x4FDD53, 0x4BA820, *reinterpret_cast<DWORD*>(&DataHandlerNonDynamicData_findGlobal));
		genCallEnforced(0x4FEADD, 0x4BA820, *reinterpret_cast<DWORD*>(&DataHandlerNonDynamicData_findGlobal));
		genCallEnforced(0x500BE8, 0x4BA820, *reinterpret_cast<DWORD*>(&DataHandlerNonDynamicData_findGlobal));
		genCallEnforced(0x52D7B3, 0x4BA820, *reinterpret_cast<DWORD*>(&DataHandlerNonDynamicData_findGlobal));
		genCallEnforced(0x52D7C7, 0x4BA820, *reinterpret_cast<DWORD*>(&DataHandlerNonDynamicData_findGlobal));
		genCallEnforced(0x52D7DB, 0x4BA820, *reinterpret_cast<DWORD*>(&DataHandlerNonDynamicData_findGlobal));
		genCallEnforced(0x52D7F0, 0x4BA820, *reinterpret_cast<DWORD*>(&DataHandlerNonDynamicData_findGlobal));
		genCallEnforced(0x52D804, 0x4BA820, *reinterpret_cast<DWORD*>(&DataHandlerNonDynamicData_findGlobal));
		genCallEnforced(0x565D8E, 0x4BA820, *reinterpret_cast<DWORD*>(&DataHandlerNonDynamicData_findGlobal));
		genCallEnforced(0x565E1C, 0x4BA820, *reinterpret_cast<DWORD*>(&DataHandlerNonDynamicData_findGlobal));
#endif

#if MWSE_RAISED_FILE_LIMIT
		// Patch: Raise esm/esp limit from 256 to 1024.

		// Change hardcoded 256 checks to 1024.
		writeValueEnforced<DWORD>(0x4B7A22 + 0x1, PatchRaiseESXLimit::ModCountVanilla, PatchRaiseESXLimit::ModCountMWSE);
		if (raiseStdioFileLimit()) {
			// Actually only allow loading more than 256 mods if we were able to raise the fopen limit.
			writeValueEnforced<DWORD>(0x4BB4AE + 0x3, PatchRaiseESXLimit::ModCountVanilla, PatchRaiseESXLimit::ModCountMWSE);
			writeValueEnforced<DWORD>(0x4BB588 + 0x3, PatchRaiseESXLimit::ModCountVanilla, PatchRaiseESXLimit::ModCountMWSE);
		}

		// Fix accesses into the active mods list to point to the new array.
		writeValueEnforced<DWORD>(0x4B7A27 + 0x2, 0xAE64, offsetof(TES3::NonDynamicData, activeMods));
		writeValueEnforced<DWORD>(0x4B87A9 + 0x2, 0xAE64, offsetof(TES3::NonDynamicData, activeMods));
		writeValueEnforced<DWORD>(0x4BB498 + 0x3, 0xAE64, offsetof(TES3::NonDynamicData, activeMods));
		writeValueEnforced<DWORD>(0x4BB56F + 0x3, 0xAE64, offsetof(TES3::NonDynamicData, activeMods));
		writeValueEnforced<DWORD>(0x4BB5ED + 0x2, 0xAE64, offsetof(TES3::NonDynamicData, activeMods));
		writeValueEnforced<DWORD>(0x4BB650 + 0x3, 0xAE64, offsetof(TES3::NonDynamicData, activeMods));
		writeValueEnforced<DWORD>(0x4BBD21 + 0x2, 0xAE64, offsetof(TES3::NonDynamicData, activeMods));
		writeValueEnforced<DWORD>(0x4BD252 + 0x2, 0xAE64, offsetof(TES3::NonDynamicData, activeMods));
		writeValueEnforced<DWORD>(0x4C8B92 + 0x2, 0xAE64, offsetof(TES3::NonDynamicData, activeMods));

		// Change of form ID: 8 bit to 10 bit game file mask.
		writeValueEnforced<BYTE>(0x4DD03F + 0x2, PatchRaiseESXLimit::FormBitsVanilla, PatchRaiseESXLimit::FormBitsMWSE);
		writeValueEnforced<BYTE>(0x4DD2A7 + 0x2, PatchRaiseESXLimit::FormBitsVanilla, PatchRaiseESXLimit::FormBitsMWSE);
		writeValueEnforced<BYTE>(0x4DD31E + 0x2, PatchRaiseESXLimit::FormBitsVanilla, PatchRaiseESXLimit::FormBitsMWSE);
		writeValueEnforced<BYTE>(0x4DD813 + 0x2, PatchRaiseESXLimit::FormBitsVanilla, PatchRaiseESXLimit::FormBitsMWSE);
		writeValueEnforced<BYTE>(0x4DDA09 + 0x2, PatchRaiseESXLimit::FormBitsVanilla, PatchRaiseESXLimit::FormBitsMWSE);
		writeValueEnforced<BYTE>(0x4DDBB1 + 0x2, PatchRaiseESXLimit::FormBitsVanilla, PatchRaiseESXLimit::FormBitsMWSE);
		writeValueEnforced<BYTE>(0x7367A0 + 0x2, PatchRaiseESXLimit::FormBitsVanilla, PatchRaiseESXLimit::FormBitsMWSE);
		writeValueEnforced<BYTE>(0x736809 + 0x2, PatchRaiseESXLimit::FormBitsVanilla, PatchRaiseESXLimit::FormBitsMWSE);
		writeValueEnforced<BYTE>(0x73685A + 0x2, PatchRaiseESXLimit::FormBitsVanilla, PatchRaiseESXLimit::FormBitsMWSE);
		writeValueEnforced<BYTE>(0x736890 + 0x2, PatchRaiseESXLimit::FormBitsVanilla, PatchRaiseESXLimit::FormBitsMWSE);
		writeValueEnforced<BYTE>(0x7368D7 + 0x2, PatchRaiseESXLimit::FormBitsVanilla, PatchRaiseESXLimit::FormBitsMWSE);
		writeValueEnforced<BYTE>(0x736B56 + 0x2, PatchRaiseESXLimit::FormBitsVanilla, PatchRaiseESXLimit::FormBitsMWSE);
		writeValueEnforced<BYTE>(0x736B75 + 0x2, PatchRaiseESXLimit::FormBitsVanilla, PatchRaiseESXLimit::FormBitsMWSE);
		writeValueEnforced<DWORD>(0x4B54DD + 0x1, PatchRaiseESXLimit::FormMaskVanilla, PatchRaiseESXLimit::FormMaskMWSE);
		writeValueEnforced<DWORD>(0x4DD030 + 0x1, PatchRaiseESXLimit::ModMaskVanilla, PatchRaiseESXLimit::ModMaskMWSE);
		writeValueEnforced<DWORD>(0x4DD089 + 0x1, PatchRaiseESXLimit::FormMaskVanilla, PatchRaiseESXLimit::FormMaskMWSE);
		writeValueEnforced<DWORD>(0x4DD107 + 0x2, PatchRaiseESXLimit::FormMaskVanilla, PatchRaiseESXLimit::FormMaskMWSE);
		writeValueEnforced<DWORD>(0x4DD80B + 0x2, PatchRaiseESXLimit::ModMaskVanilla, PatchRaiseESXLimit::ModMaskMWSE);
		writeValueEnforced<DWORD>(0x4DD829 + 0x2, PatchRaiseESXLimit::FormMaskVanilla, PatchRaiseESXLimit::FormMaskMWSE);
		writeValueEnforced<DWORD>(0x4E0C8B + 0x2, PatchRaiseESXLimit::FormMaskVanilla, PatchRaiseESXLimit::FormMaskMWSE);
		writeValueEnforced<DWORD>(0x4E0C91 + 0x2, PatchRaiseESXLimit::FormMaskVanilla, PatchRaiseESXLimit::FormMaskMWSE);
		writeValueEnforced<DWORD>(0x7367A3 + 0x2, PatchRaiseESXLimit::FormMaskVanilla, PatchRaiseESXLimit::FormMaskMWSE);
		writeValueEnforced<DWORD>(0x73680C + 0x2, PatchRaiseESXLimit::FormMaskVanilla, PatchRaiseESXLimit::FormMaskMWSE);
		writeValueEnforced<DWORD>(0x736B78 + 0x2, PatchRaiseESXLimit::FormMaskVanilla, PatchRaiseESXLimit::FormMaskMWSE);

		// Patch loading to support either the old or new format.
		genCallEnforced(0x4C01B1, 0x4B6880, reinterpret_cast<DWORD>(PatchRaiseESXLimit::LoadFormId));
		genCallEnforced(0x4DCE01, 0x4B6880, reinterpret_cast<DWORD>(PatchRaiseESXLimit::LoadFormId));
		genCallEnforced(0x4DD027, 0x4B6880, reinterpret_cast<DWORD>(PatchRaiseESXLimit::LoadFormId));
		genCallEnforced(0x4DE197, 0x4B6880, reinterpret_cast<DWORD>(PatchRaiseESXLimit::LoadFormId));
		genCallEnforced(0x4E0C2F, 0x4B6880, reinterpret_cast<DWORD>(PatchRaiseESXLimit::LoadFormId));
		genCallEnforced(0x4E0C6D, 0x4B6880, reinterpret_cast<DWORD>(PatchRaiseESXLimit::LoadFormId));
		genJumpEnforced(0x7367BA, 0x4B6880, reinterpret_cast<DWORD>(PatchRaiseESXLimit::LoadFormId));
		genCallEnforced(0x736B48, 0x4B6880, reinterpret_cast<DWORD>(PatchRaiseESXLimit::LoadFormId));

		// Patch saving to try to use the old format if possible, and use the new format if it can't.
		genCallEnforced(0x4E1144, 0x4B6BA0, reinterpret_cast<DWORD>(PatchRaiseESXLimit::SaveFormId));
		genCallEnforced(0x4E14D5, 0x4B6BA0, reinterpret_cast<DWORD>(PatchRaiseESXLimit::SaveFormId));
		genCallEnforced(0x4E1B15, 0x4B6BA0, reinterpret_cast<DWORD>(PatchRaiseESXLimit::SaveFormId));
		genCallEnforced(0x4E1E78, 0x4B6BA0, reinterpret_cast<DWORD>(PatchRaiseESXLimit::SaveFormId));
		genCallEnforced(0x4FFB78, 0x4B6BA0, reinterpret_cast<DWORD>(PatchRaiseESXLimit::SaveFormId));
#endif

		// Patch: Fix crash when trying to draw cell markers that don't fit on the map.
		// Compatible with MCP map expansion.
		writeValueEnforced<DWORD>(0x4C85BC + 2, 0x200, 0x1F7);
		writeValueEnforced<DWORD>(0x4C85CC + 1, 0x200, 0x1F7);

		// Patch: Optimize ShowMap (and FillMap) mwscript command.
		auto NonDynamicData_showLocationOnMap = &TES3::NonDynamicData::showLocationOnMap;
		genCallEnforced(0x505374, 0x4C8480, *reinterpret_cast<DWORD*>(&NonDynamicData_showLocationOnMap));
		genCallEnforced(0x50CB22, 0x4C8480, *reinterpret_cast<DWORD*>(&NonDynamicData_showLocationOnMap));

		// Patch: Fix crash when trying to remove items from incomplete references.
		genCallEnforced(0x508D14, 0x45E5C0, reinterpret_cast<DWORD>(PatchFixupActorSelfReference));

		// Patch: Store last executed script for crash dump information.
		auto Script_execute = &TES3::Script::execute;
		genCallEnforced(0x40F679, 0x5028A0, *reinterpret_cast<DWORD*>(&Script_execute));
		genCallEnforced(0x40FC1D, 0x5028A0, *reinterpret_cast<DWORD*>(&Script_execute));
		genCallEnforced(0x49A5D7, 0x5028A0, *reinterpret_cast<DWORD*>(&Script_execute));
		genCallEnforced(0x4E71FE, 0x5028A0, *reinterpret_cast<DWORD*>(&Script_execute));
		genCallEnforced(0x50E6BD, 0x5028A0, *reinterpret_cast<DWORD*>(&Script_execute));

		// Patch: Always clone scene graph nodes.
		writeValueEnforced(0x4EF9FB, BYTE(0x02), BYTE(0x00));

		// Patch: Always copy all NiExtraData on clone, instead of only the first NiStringExtraData.
		genJumpUnprotected(0x4E8295, 0x4E82BB);
		genJumpUnprotected(0x4E82C4, 0x4E82CE);

		// Patch: Update player first and third person animations when the idle flag is pausing the controller.
		genCallUnprotected(0x41B836, reinterpret_cast<DWORD>(PatchUpdateAllIdles));

		// Patch: Correctly initialize MobileProjectile tag/objectType
		genCallEnforced(0x572444, 0x4EE8A0, reinterpret_cast<DWORD>(PatchInitializeMobileProjectileType));

		// Patch: Fix crash when saving menu position if the derived key name is too long.
		genCallUnprotected(0x597061, reinterpret_cast<DWORD>(PatchSaveMenuPositionRightPad), 0x6);
		genNOPUnprotected(0x59706C, 0x59706F - 0x59706C);

		// Patch: Fix book enchantment copying.
		genNOPUnprotected(0x4A2618, 0x4A26D8 - 0x4A2618);
		writePatchCodeUnprotected(0x4A2618, (BYTE*)&PatchCopyBookEnchantmentCaller, PatchCopyEnchantmentCaller_size);
		genCallUnprotected(0x4A2618 + PatchCopyEnchantmentCaller_size, reinterpret_cast<DWORD>(PatchCopyEnchantment));

		// Patch: Fix weapon enchantment copying.
		genNOPUnprotected(0x4F26FF, 0x4F27BC - 0x4F26FF);
		writePatchCodeUnprotected(0x4F26FF, (BYTE*)&PatchCopyWeaponEnchantmentCaller, PatchCopyEnchantmentCaller_size);
		genCallUnprotected(0x4F26FF + PatchCopyEnchantmentCaller_size, reinterpret_cast<DWORD>(PatchCopyEnchantment));

		// Patch: Letterbox movies.
		genCallEnforced(0x64FC55, 0x64FE20, reinterpret_cast<DWORD>(PatchDrawLetterboxMovieFrame));
		genCallEnforced(0x64FC9C, 0x64FE20, reinterpret_cast<DWORD>(PatchDrawLetterboxMovieFrame));
		genCallEnforced(0x64FCDF, 0x64FE20, reinterpret_cast<DWORD>(PatchDrawLetterboxMovieFrame));
		genCallEnforced(0x64FD23, 0x64FE20, reinterpret_cast<DWORD>(PatchDrawLetterboxMovieFrame));
		genCallEnforced(0x64FD69, 0x64FE20, reinterpret_cast<DWORD>(PatchDrawLetterboxMovieFrame));
		genCallEnforced(0x64FDA1, 0x64FE20, reinterpret_cast<DWORD>(PatchDrawLetterboxMovieFrame));
		genCallEnforced(0x64FDD2, 0x64FE20, reinterpret_cast<DWORD>(PatchDrawLetterboxMovieFrame));
		genCallEnforced(0x64FE03, 0x64FE20, reinterpret_cast<DWORD>(PatchDrawLetterboxMovieFrame));

		// Patch: Slight journal update optimization.
		writePatchCodeUnprotected(0x4B2FD7, (BYTE*)&PatchSwapJournalUpdateCheckForSpeakerOrder, PatchSwapJournalUpdateCheckForSpeakerOrder_size);
		genCallUnprotected(0x4B2FD7 + 0xD, 0x4B1B80);

		// Patch: Don't save VFX manager if there are no valid visual effects.
		genCallEnforced(0x4BD149, 0x469CC0, reinterpret_cast<DWORD>(PatchSaveVisualEffects));

		// Patch: Fix crash when releasing a clone of a light with no reference. Also fix crash when the attachment scenegraph light pointer has been cleared.
		genCallEnforced(0x4D260C, 0x4E5170, reinterpret_cast<DWORD>(PatchReleaseLightEntityForReference));

		// Patch: Cache values between dialogue filters. The actual override that makes use of this cache is in LuaManager for its hooks.
		genCallUnprotected(0x4B1646, reinterpret_cast<DWORD>(PatchDialogueFilterCacheGetDisposition), 0x6);
		genCallUnprotected(0x4B167B, reinterpret_cast<DWORD>(PatchDialogueFilterCacheGetDisposition), 0x6);

		// Patch: Support custom class images.
		genNOPUnprotected(0x5AF047, 0x5AF583 - 0x5AF047);
		writePatchCodeUnprotected(0x5AF047, (BYTE*)&PatchAddCustomClassImageSupportSetup, PatchAddCustomClassImageSupport_size);
		genCallUnprotected(0x5AF047 + 0x4, reinterpret_cast<DWORD>(PatchAddCustomClassImageSupport), 0x9);

		// Patch: Clean up unsummoned actors.
		genCallEnforced(0x466858, 0x4E5750, reinterpret_cast<DWORD>(cleanupUnsummonedActor));

		// Patch: Set ActiveMagicEffect.isIllegalSummon correctly on loading a savegame.
		writePatchCodeUnprotected(0x454826, (BYTE*)&PatchLoadActiveMagicEffect, PatchLoadActiveMagicEffect_size);

		// Patch: Fix crash in NPC wander and flee logic when trying to pick a random node from a pathgrid with 0 nodes.
		genCallEnforced(0x5339D8, 0x4E2850, reinterpret_cast<DWORD>(PatchCellGetPathGridWithNodes));
		genCallEnforced(0x549E76, 0x4E2850, reinterpret_cast<DWORD>(PatchCellGetPathGridWithNodes));

		// Patch: UI element image mirroring on negative image scale.
		writePatchCodeUnprotected(0x57DE02, (BYTE*)&PatchUIUpdateLayoutImageContent1, PatchUIUpdateLayoutImageContent1_size);
		writePatchCodeUnprotected(0x57DE3F, (BYTE*)&PatchUIUpdateLayoutImageContent1, PatchUIUpdateLayoutImageContent1_size);
		writePatchCodeUnprotected(0x57E1E8, (BYTE*)&PatchUIUpdateLayoutImageContent2, PatchUIUpdateLayoutImageContent2_size);
		genCallUnprotected(0x57E1E8 + 0x2, reinterpret_cast<DWORD>(PatchUIElementTexcoordWrite));

		// Patch: When adjusting effects mix volume, update looping audio volume correctly.
		writeValueEnforced<BYTE>(0x5A1F24, 0x52, 0x56);
		genCallEnforced(0x5A1F25, 0x4029F0, reinterpret_cast<DWORD>(PatchSetLoopingSoundBufferVolume));
		writeValueEnforced<BYTE>(0x5A1FC5, 0x52, 0x56);
		genCallEnforced(0x5A1FC6, 0x4029F0, reinterpret_cast<DWORD>(PatchSetLoopingSoundBufferVolume));
		// Fix Sound::changeVolume scaling constant to be 1/255.
		writeValueEnforced<DWORD>(0x510C6C, 0x74A9E4, 0x746910);

		// Patch: Add deterministic subtree ordering mode to NiSortAdjustNode. Fix cloning with no accumulator.
		overrideVirtualTableEnforced(0x750580, 0x78, 0x6DE030, reinterpret_cast<DWORD>(PatchNISortAdjustNodeDisplay));
		genCallUnprotected(0x6DE21B, reinterpret_cast<DWORD>(PatchNISortAdjustNodeCloneAccumulator));

		// Patch: Add pick proxy behaviour to NiCollisionSwitch.
		auto CollisionSwitch_linkObject = &NI::CollisionSwitch::linkObject;
		auto CollisionSwitch_findIntersectons = &NI::CollisionSwitch::findIntersections;
		overrideVirtualTableEnforced(0x74F418, 0x10, 0x6D7100, *reinterpret_cast<DWORD*>(&CollisionSwitch_linkObject));
		overrideVirtualTableEnforced(0x74F418, 0x88, 0x6D6E10, *reinterpret_cast<DWORD*>(&CollisionSwitch_findIntersectons));

		// Patch: Improve error reporting by including the source mod next to object IDs in load error messages.
		// In Cell::loadReference:
		const BYTE patchImprovedErrorIDArgs0[] = { 0x8B, 0xCE };
		const BYTE patchImprovedErrorIDArgs1[] = { 0x8B, 0xCD, 0x90 };
		const BYTE patchImprovedErrorIDArgs2[] = { 0x50, 0x8B, 0xCE };
		const BYTE patchImprovedErrorIDArgs3[] = { 0x51, 0x8B, 0xCA };
		const BYTE patchImprovedErrorIDArgs4[] = { 0x8B, 0x32 };
		const BYTE patchImprovedErrorIDArgs5[] = { 0x8B, 0xCA };
		const BYTE patchImprovedErrorIDArgs6[] = { 0x51, 0x8B, 0xCA };
		genCallUnprotected(0x4DE71A, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4DE78D, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4DE98C, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x4DEA86, patchImprovedErrorIDArgs0, sizeof(patchImprovedErrorIDArgs0));
		genCallUnprotected(0x4DEA88, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x4DED73, patchImprovedErrorIDArgs1, sizeof(patchImprovedErrorIDArgs1));
		genCallUnprotected(0x4DED76, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4DF1A5, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4DF21B, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x4DF42C, patchImprovedErrorIDArgs2, sizeof(patchImprovedErrorIDArgs2));
		genCallUnprotected(0x4DF42F, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x4DF454, patchImprovedErrorIDArgs3, sizeof(patchImprovedErrorIDArgs3));
		genCallUnprotected(0x4DF457, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x4DF551, patchImprovedErrorIDArgs4, sizeof(patchImprovedErrorIDArgs4));
		genCallUnprotected(0x4DF553, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4DF5FD, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x4DF7A4, patchImprovedErrorIDArgs5, sizeof(patchImprovedErrorIDArgs5));
		genCallUnprotected(0x4DF7A6, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x4DF7DB, patchImprovedErrorIDArgs6, sizeof(patchImprovedErrorIDArgs6));
		genCallUnprotected(0x4DF7DE, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4DF89A, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4DF922, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4DF9E4, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4DFAA3, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4DFB7C, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4DFC0E, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4DFCB5, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4DFDB3, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4DFEAA, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4DFF50, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4DFFC1, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4E035E, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4E037C, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4E08E7, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		// In link-resolve functions:
		const BYTE patchImprovedErrorIDArgs7[] = { 0x8B, 0xCE };
		const BYTE patchImprovedErrorIDArgs8[] = { 0x8B, 0xCF };
		const BYTE patchImprovedErrorIDArgs9[] = { 0x8B, 0xCE };
		const BYTE patchImprovedErrorIDArgs10[] = { 0x8B, 0xCD, 0x90 };
		const BYTE patchImprovedErrorIDArgs11[] = { 0x8B, 0xCB };
		const BYTE patchImprovedErrorIDArgs12[] = { 0x8B, 0xCD, 0x90 };
		const BYTE patchImprovedErrorIDArgs13[] = { 0x8B, 0xCE };
		const BYTE patchImprovedErrorIDArgs14[] = { 0x8B, 0xCE };
		const BYTE patchImprovedErrorIDArgs15[] = { 0x8B, 0xCE };
		const BYTE patchImprovedErrorIDArgs16[] = { 0x8B, 0xCE };
		const BYTE patchImprovedErrorIDArgs17[] = { 0x8B, 0xCF };
		const BYTE patchImprovedErrorIDArgs18[] = { 0x8B, 0xCE };
		const BYTE patchImprovedErrorIDArgs19[] = { 0x8B, 0xCE };
		const BYTE patchImprovedErrorIDArgs20[] = { 0x8B, 0xCB };
		genCallUnprotected(0x40FCA5, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x480DDF, patchImprovedErrorIDArgs7, sizeof(patchImprovedErrorIDArgs7));
		genCallUnprotected(0x480DE1, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x49D121, patchImprovedErrorIDArgs8, sizeof(patchImprovedErrorIDArgs8));
		genCallUnprotected(0x49D123, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x49FAEF, patchImprovedErrorIDArgs9, sizeof(patchImprovedErrorIDArgs9));
		genCallUnprotected(0x49FAF1, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x4A0686, patchImprovedErrorIDArgs10, sizeof(patchImprovedErrorIDArgs10));
		genCallUnprotected(0x4A0689, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x4A212E, patchImprovedErrorIDArgs11, sizeof(patchImprovedErrorIDArgs11));
		genCallUnprotected(0x4A2130, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x4A2F96, patchImprovedErrorIDArgs12, sizeof(patchImprovedErrorIDArgs12));
		genCallUnprotected(0x4A2F99, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x4A53EF, patchImprovedErrorIDArgs13, sizeof(patchImprovedErrorIDArgs13));
		genCallUnprotected(0x4A53F1, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x4A5995, patchImprovedErrorIDArgs14, sizeof(patchImprovedErrorIDArgs14));
		genCallUnprotected(0x4A5997, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x4A5F2F, patchImprovedErrorIDArgs15, sizeof(patchImprovedErrorIDArgs15));
		genCallUnprotected(0x4A5F31, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x4A64BF, patchImprovedErrorIDArgs16, sizeof(patchImprovedErrorIDArgs16));
		genCallUnprotected(0x4A64C1, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x4AC3FA, patchImprovedErrorIDArgs17, sizeof(patchImprovedErrorIDArgs17));
		genCallUnprotected(0x4AC3FC, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4CF800, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4D0B60, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x4D1D56, patchImprovedErrorIDArgs18, sizeof(patchImprovedErrorIDArgs18));
		genCallUnprotected(0x4D1D58, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x4D7284, patchImprovedErrorIDArgs19, sizeof(patchImprovedErrorIDArgs19));
		genCallUnprotected(0x4D7286, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		genCallUnprotected(0x4E6C0C, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));
		writeBytesUnprotected(0x4F2132, patchImprovedErrorIDArgs20, sizeof(patchImprovedErrorIDArgs20));
		genCallUnprotected(0x4F2134, reinterpret_cast<DWORD>(PatchGetImprovedObjectIdentifier));

		// Patch: Ensure VFX with maxAge <= 0.001f are cleared when clearing data on load game, instead of leaking.
		auto VFXManager_reset = &TES3::VFXManager::reset;
		genCallEnforced(0x4C6F00, 0x469390, *reinterpret_cast<DWORD*>(&VFXManager_reset));

		// Patch: Do not load VFX with maxAge <= 0.001f from save games.
		genCallEnforced(0x46A04B, 0x468620, reinterpret_cast<DWORD>(PatchVFXManagerCreateFromSaveData));

		// Patch: LTEX loading/unloading array overflow bug. Increase array from 500 to 4096 elements and fix bounds checks.
		writeValueEnforced(0x4CDF09, 500 / 4, Land_LTEX_isLoaded_size / 4);
		writeValueEnforced(0x4CDF0E, DWORD(0x7CA9E0), reinterpret_cast<DWORD>(Land_LTEX_isLoaded));
		writePatchCodeUnprotected(0x4CDF58, (BYTE*)&PatchLandUnloadTexturesBoundsCheck, PatchLandUnloadTexturesBoundsCheck_size);
		writeValueEnforced(0x4CDF6D, DWORD(0x7CA9E0), reinterpret_cast<DWORD>(Land_LTEX_isLoaded));
		writeValueEnforced(0x4CDF7E, DWORD(0x7CA9E0), reinterpret_cast<DWORD>(Land_LTEX_isLoaded));

		writeValueEnforced(0x4CECAE, 500 / 4, Land_LTEX_isLoaded_size / 4);
		writeValueEnforced(0x4CECB3, DWORD(0x7CA9E0), reinterpret_cast<DWORD>(Land_LTEX_isLoaded));
		writePatchCodeUnprotected(0x4CECD3, (BYTE*)&PatchLandLoadTexturesBoundsCheck, PatchLandLoadTexturesBoundsCheck_size);
		writeValueEnforced(0x4CECE9, DWORD(0x7CA9E0), reinterpret_cast<DWORD>(Land_LTEX_isLoaded));
		writeValueEnforced(0x4CEDB1, DWORD(0x7CA9E0), reinterpret_cast<DWORD>(Land_LTEX_isLoaded));

		// Patch: Fall back to reference rotation values when initializing animation controllers without a scene node.
		genCallEnforced(0x521773, 0x53DE70, reinterpret_cast<DWORD>(PatchSetAnimControllerMobile));

		// Provide lua stack traces with invalid UI access.
		genCallEnforced(0x581484, 0x476E20, reinterpret_cast<DWORD>(PatchLogUIMemoryPointerErrors));
		genCallEnforced(0x582DFA, 0x476E20, reinterpret_cast<DWORD>(PatchLogUIMemoryPointerErrors));

		// Patch: Improve raycast accuracy.
		auto NiTriBasedGeometry_FindIntersections = &NI::TriBasedGeometry::findIntersections;
		overrideVirtualTableEnforced(0x7508B0, offsetof(NI::TriBasedGeometry_vTable, findIntersections), 0x6F0350, *reinterpret_cast<DWORD*>(&NiTriBasedGeometry_FindIntersections)); // NiTriShape
		overrideVirtualTableEnforced(0x750A00, offsetof(NI::TriBasedGeometry_vTable, findIntersections), 0x6F0350, *reinterpret_cast<DWORD*>(&NiTriBasedGeometry_FindIntersections)); // NiTriStrips
		overrideVirtualTableEnforced(0x750CC0, offsetof(NI::TriBasedGeometry_vTable, findIntersections), 0x6F0350, *reinterpret_cast<DWORD*>(&NiTriBasedGeometry_FindIntersections)); // NiTriBasedGeometry

		// Patch: Respect targets when searching for symlinks.
		writeDoubleWordUnprotected(0x746114, reinterpret_cast<DWORD>(&PatchFindFirstFileA));
		writeDoubleWordUnprotected(0x746118, reinterpret_cast<DWORD>(&PatchFindNextFileA));
		writeDoubleWordUnprotected(0x74611C, reinterpret_cast<DWORD>(&PatchFindClose));

		// Patch: Guard against updating dynamic light attachments that have no actual light.
		genCallEnforced(0x485DA4, 0x4E5170, reinterpret_cast<DWORD>(PatchGetLightAttachmentIfItHasALight));
		genCallEnforced(0x485E87, 0x4E5170, reinterpret_cast<DWORD>(PatchGetLightAttachmentIfItHasALight));
		genCallEnforced(0x4D260C, 0x4E5170, reinterpret_cast<DWORD>(PatchGetLightAttachmentIfItHasALight));
		genCallEnforced(0x5243D6, 0x4E5170, reinterpret_cast<DWORD>(PatchGetLightAttachmentIfItHasALight));

		// Patch: Guard against invalid light flicker/pulse updates.
		genCallEnforced(0x49B75E, 0x4D33D0, reinterpret_cast<DWORD>(PatchEntityLightFlickerPulseUpdate));
		genCallEnforced(0x4D33BF, 0x4D33D0, reinterpret_cast<DWORD>(PatchEntityLightFlickerPulseUpdate));

		// Patch: Resolve node count mismatch when loading pathgrid records with missing subrecords.
		writePatchCodeUnprotected(0x4F444E, (BYTE*)&PatchPathGridLoader, PatchPathGridLoader_size);
		genCallUnprotected(0x4F444E + 4, reinterpret_cast<DWORD>(PatchPathGridLoaderCheckNodeData));

		// Patch: Allow bound armour function to also summon bracers and pauldrons.
		genCallEnforced(0x466457, 0x465DE0, reinterpret_cast<DWORD>(PatchSwapBoundArmor));

		// Patch: Modify proximity movement speed matching of AI followers to limit the speed match from going to zero on immobilized follow targets.
		writePatchCodeUnprotected(0x540DBA, (BYTE*)&PatchMovementAnimSpeedMatching, PatchMovementAnimSpeedMatching_size);
		genCallUnprotected(0x540DBA + 1, reinterpret_cast<DWORD>(PatchGetAnimDataMovementSpeedCapped));

		// Patch: Allow control of whether software or hardware skinning is used through TriShape flags.
		auto TriShape_linkObject = &NI::TriShape::linkObject;
		writePatchCodeUnprotected(0x6FF0A8, (BYTE*)&PatchNITriBasedGeom_Ctor1, PatchNITriBasedGeom_Ctor1_size);
		writePatchCodeUnprotected(0x6FF0F0, (BYTE*)&PatchNITriBasedGeom_Ctor2, PatchNITriBasedGeom_Ctor2_size);
		genCallEnforced(0x6E54C5, 0x6F15B0, reinterpret_cast<DWORD>(PatchNITriShapeCopyMembers));
		writePatchCodeUnprotected(0x6ACF1F, (BYTE*)&PatchNIDX8Renderer_RenderShape, PatchNIDX8Renderer_RenderShape_size);
		overrideVirtualTableEnforced(0x7508B0, offsetof(NI::TriShape_vTable, NI::TriShape_vTable::linkObject), 0x6E56D0, *reinterpret_cast<DWORD*>(&TriShape_linkObject));

		// Patch: Fix cure spells incorrectly triggering MagicEffectState_Ending for magic that hasn't taken effect yet.
		genCallUnprotected(0x4559B2, reinterpret_cast<DWORD>(PatchRemoveMagicsByEffect), 0x8);

		// Patch: Fix loading crashes where there are links to missing objects from mods that were removed.
		writePatchCodeUnprotected(0x512485, (BYTE*)&PatchMagicSourceInstanceDtor, PatchMagicSourceInstanceDtor_size);
		writePatchCodeUnprotected(0x49DEE1, (BYTE*)&PatchSoulTrappedCreatureNotFound1, PatchSoulTrappedCreatureNotFound1_size);
		writePatchCodeUnprotected(0x4A4BEC, (BYTE*)&PatchSoulTrappedCreatureNotFound2, PatchSoulTrappedCreatureNotFound2_size);
		writePatchCodeUnprotected(0x4D8DD7, (BYTE*)&PatchSoulTrappedCreatureNotFound3, PatchSoulTrappedCreatureNotFound3_size);

		// Patch: Prevent crash with magic effects on invalid targets.
		writeDoubleWordEnforced(0x7884B0 + (TES3::EffectID::FortifySkill * 4), 0x4625F0, reinterpret_cast<DWORD>(PatchMagicEffectFortifySkill));

		// Patch: Suppress sGeneralMastPlugMismatchMsg message.
		genCallUnprotected(0x477512, reinterpret_cast<DWORD>(GetCachedYesToAll), 0x477518 - 0x477512);
		genCallEnforced(0x4BB55D, 0x477400, reinterpret_cast<DWORD>(SuppressGeneralMastPlugMismatchMsg));

		// Patch: Clean up mobile collision data when a mobile is destroyed. Fixes probably a Todd-typo.
		genNOPUnprotected(0x55E55B, 0x55E55F - 0x55E55B);

		// Patch: Fix missing nullptr check when determining object types.
		DoPatchSafeGetObjectType();

		// Patch: Expand keyboard key translations
		PatchExpandKeyboardCharacterTranslations();

		// Patch: Fix crash when trying to unequip a nocked projectile item while still using an item index for its position.
		genNOPUnprotected(0x4968E1, 0x4968FB - 0x4968E1);
		writePatchCodeUnprotected(0x4968E1, (BYTE*)&PatchUnequipIndexedProjectileSetup, PatchUnequipIndexedProjectileSetup_size);
		genCallUnprotected(0x4968E1 + 0x2, reinterpret_cast<DWORD>(PatchUnequipIndexedProjectile));

#if false
		// Patch: Update dynamic lights to implement custom light sorting.
		genCallEnforced(0x485B60, 0x4D2F40, reinterpret_cast<DWORD>(PatchDynamicLightingTest));
		genCallEnforced(0x4D2C9C, 0x4D2F40, reinterpret_cast<DWORD>(PatchDynamicLightingTest));
		genCallEnforced(0x4D2D04, 0x4D2F40, reinterpret_cast<DWORD>(PatchDynamicLightingTest));
		genCallEnforced(0x4D2D9F, 0x4D2F40, reinterpret_cast<DWORD>(PatchDynamicLightingTest));
		genCallEnforced(0x4D2F10, 0x4D2F40, reinterpret_cast<DWORD>(PatchDynamicLightingTest));
		genCallEnforced(0x4D3350, 0x4D2F40, reinterpret_cast<DWORD>(PatchDynamicLightingTest));
#endif
	}

	void installPostLuaPatches() {
		// Patch: The window is never out of focus.
		if (Configuration::RunInBackground) {
			writeByteUnprotected(0x416BC3 + 0x2 + 0x4, 1);
			genCallUnprotected(0x41AB7D, reinterpret_cast<DWORD>(PatchGetMorrowindMainWindow), 0x6);
			genCallEnforced(0x425313, 0x4065E0, reinterpret_cast<DWORD>(PatchGetMorrowindMainWindow_NoBackgroundInput));
			genCallEnforced(0x4772CE, 0x4065E0, reinterpret_cast<DWORD>(PatchGetMorrowindMainWindow_NoBackgroundInput));
			genCallEnforced(0x47798C, 0x4065E0, reinterpret_cast<DWORD>(PatchGetMorrowindMainWindow_NoBackgroundInput));
			genCallEnforced(0x477E1E, 0x4065E0, reinterpret_cast<DWORD>(PatchGetMorrowindMainWindow_NoBackgroundInput));
			genCallEnforced(0x5BC9E1, 0x4065E0, reinterpret_cast<DWORD>(PatchGetMorrowindMainWindow_NoBackgroundInput));
			genCallEnforced(0x5BCA33, 0x4065E0, reinterpret_cast<DWORD>(PatchGetMorrowindMainWindow_NoBackgroundInput));
			genCallEnforced(0x58E8C6, 0x406950, reinterpret_cast<DWORD>(PatchGetMorrowindMainWindow_NoBufferReading));
			genCallEnforced(0x5BCA1D, 0x406950, reinterpret_cast<DWORD>(PatchGetMorrowindMainWindow_NoBufferReading));
		}

		// Patch: Fix NiFlipController losing its affectedMap on clone.
		if (Configuration::PatchNiFlipController) {
			auto NiFlipController_clone = &NI::FlipController::copy;
			genCallEnforced(0x715D26, DWORD(NI::FlipController::_copy), *reinterpret_cast<DWORD*>(&NiFlipController_clone));
		}

		// Patch: Allow global audio.
		if (Configuration::UseGlobalAudio) {
			constexpr auto DS_FLAGS_DEFAULT = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;
			constexpr auto DS_FLAGS_3D = DS_FLAGS_DEFAULT | DSBCAPS_CTRL3D | DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_MUTE3DATMAXDISTANCE;
			writeAddFlagEnforced(0x401FEA + 0x3, DS_FLAGS_DEFAULT | DSBCAPS_CTRLPAN, DSBCAPS_GLOBALFOCUS);
			writeAddFlagEnforced(0x401FE1 + 0x3, DS_FLAGS_3D, DSBCAPS_GLOBALFOCUS);
			writeAddFlagEnforced(0x401FF7 + 0x3, DS_FLAGS_DEFAULT, DSBCAPS_GLOBALFOCUS);
			writeAddFlagEnforced(0x40240E + 0x3, DS_FLAGS_DEFAULT | DSBCAPS_CTRLPAN, DSBCAPS_GLOBALFOCUS);
			writeAddFlagEnforced(0x402405 + 0x3, DS_FLAGS_3D, DSBCAPS_GLOBALFOCUS);
		}
	}

	void installPostInitializationPatches() {
		// Patch: Give threads descriptions.
		const auto dataHandler = TES3::DataHandler::get();
		if (dataHandler) {
			windows::SetThreadDescription(dataHandler->mainThread, L"GameMainThread");
			windows::SetThreadDescription(dataHandler->backgroundThread, L"GameBackgroundThread");
		}
	}

	//
	// Create minidumps.
	//

	bool isDataSectionNeeded(const WCHAR* pModuleName) {
		// Check parameters.
		if (pModuleName == 0) {
			return false;
		}

		// Extract the module name.
		WCHAR szFileName[_MAX_FNAME] = L"";
		_wsplitpath(pModuleName, NULL, NULL, szFileName, NULL);

		// Compare the name with the list of known names and decide.
		if (_wcsicmp(szFileName, L"Morrowind") == 0) {
			return true;
		}
		else if (_wcsicmp(szFileName, L"ntdll") == 0)
		{
			return true;
		}
		else if (_wcsicmp(szFileName, L"MWSE") == 0)
		{
			return true;
		}

		// Complete.
		return false;
	}

	BOOL CALLBACK miniDumpCallback(PVOID pParam, const PMINIDUMP_CALLBACK_INPUT pInput, PMINIDUMP_CALLBACK_OUTPUT pOutput) {
		BOOL bRet = FALSE;

		// Check parameters 
		if (pInput == 0) {
			return FALSE;
		}
		if (pOutput == 0) {
			return FALSE;
		}

		// Process the callbacks 
		switch (pInput->CallbackType) {
		case IncludeModuleCallback:
		case IncludeThreadCallback:
		case ThreadCallback:
		case ThreadExCallback:
		{
			// Include the thread into the dump 
			bRet = TRUE;
		}
		break;

		case MemoryCallback:
		{
			// We do not include any information here -> return FALSE 
			bRet = FALSE;
		}
		break;

		case ModuleCallback:
		{
			// Does the module have ModuleReferencedByMemory flag set? 
			if (pOutput->ModuleWriteFlags & ModuleWriteDataSeg) {
				if (!isDataSectionNeeded(pInput->Module.FullPath)) {
					pOutput->ModuleWriteFlags &= (~ModuleWriteDataSeg);
				}
			}

			bRet = TRUE;
		}
		break;
		}

		return bRet;
	}

	const char* GetThreadName(DWORD threadId) {
		const auto dataHandler = TES3::DataHandler::get();
		if (dataHandler) {
			if (threadId == dataHandler->mainThreadID) {
				return "Main";
			}
			else if (threadId == dataHandler->backgroundThreadID) {
				return "Background";
			}
		}

		return "Unknown";
	}

	const char* GetThreadName() {
		return GetThreadName(GetCurrentThreadId());
	}

	void CreateMiniDump(EXCEPTION_POINTERS* pep) {
		log::getLog() << std::dec << std::endl;
		log::getLog() << "Morrowind has crashed! To help improve game stability, send MWSE_Minidump.dmp and mwse.log to the #mwse channel at the Morrowind Modding Community Discord: https://discord.me/mwmods" << std::endl;

		// Try to print any relevant mwscript information.
		if (TES3::Script::currentlyExecutingScript) {
			log::getLog() << "Currently executing mwscript context:" << std::endl;
			safePrintObjectToLog("Script", TES3::Script::currentlyExecutingScript);
			safePrintObjectToLog("Reference", TES3::Script::currentlyExecutingScriptReference);
			log::getLog() << "  OpCode: 0x" << std::hex << *reinterpret_cast<DWORD*>(0x7A91C4) << std::endl;
			log::getLog() << "  Cursor Offset: 0x" << std::hex << *reinterpret_cast<DWORD*>(0x7CEBB0) << std::endl;
		}

		// Show if we failed to load a mesh.
		if (!TES3::DataHandler::currentlyLoadingMeshes.empty()) {
			TES3::DataHandler::currentlyLoadingMeshesMutex.lock();
			const auto worldController = TES3::WorldController::get();
			for (const auto& itt : TES3::DataHandler::currentlyLoadingMeshes) {
				log::getLog() << "Currently loading mesh: " << itt.second << "; Thread: " << GetThreadName(itt.first) << std::endl;
			}
			TES3::DataHandler::currentlyLoadingMeshesMutex.unlock();
		}

		// Open the file.
		auto hFile = CreateFile("MWSE_MiniDump.dmp", GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if ((hFile != NULL) && (hFile != INVALID_HANDLE_VALUE)) {
			// Create the minidump.
			MINIDUMP_EXCEPTION_INFORMATION mdei = {};

			mdei.ThreadId = GetCurrentThreadId();
			mdei.ExceptionPointers = pep;
			mdei.ClientPointers = FALSE;

			MINIDUMP_CALLBACK_INFORMATION mci = {};

			mci.CallbackRoutine = (MINIDUMP_CALLBACK_ROUTINE)miniDumpCallback;
			mci.CallbackParam = 0;

			auto mdt = (MINIDUMP_TYPE)(MiniDumpWithDataSegs |
				MiniDumpWithHandleData |
				MiniDumpWithFullMemoryInfo |
				MiniDumpWithThreadInfo |
				MiniDumpWithUnloadedModules);

			auto rv = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, mdt, (pep != 0) ? &mdei : 0, 0, &mci);

			if (!rv) {
				log::getLog() << "MiniDump creation failed. Error: 0x" << std::hex << GetLastError() << std::endl;
			}
			else {
				log::getLog() << "MiniDump creation successful." << std::endl;
			}

			// Close the file
			CloseHandle(hFile);
		}
		else {
			log::getLog() << "MiniDump creation failed. Could not get file handle. Error: " << GetLastError() << std::endl;
		}

		if constexpr (CrashLogger::DEBUG_LOGGER) {
			log::getLog() << "Attempting To Log Crash \n";
		}
		CrashLogger::AttemptLog(pep);
	}

	int __stdcall onWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
		__try {
			return reinterpret_cast<int(__stdcall*)(HINSTANCE, HINSTANCE, LPSTR, int)>(0x416E10)(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
		}
		__except (CreateMiniDump(GetExceptionInformation()), EXCEPTION_EXECUTE_HANDLER) {
			// Try to reset gamma.
			auto game = TES3::Game::get();
			if (game) {
				game->setGamma(1.0f);
			}

			return 0;
		}

	}

	bool installMiniDumpHook() {
		if constexpr (INSTALL_MINIDUMP_HOOK) {
			CrashLogger::Playtime::Init();
			return genCallEnforced(0x7279AD, 0x416E10, reinterpret_cast<DWORD>(onWinMain));
		}
		else {
			return true;
		}
	}
}
