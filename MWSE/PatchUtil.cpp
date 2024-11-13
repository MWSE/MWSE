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
#include "NISortAdjustNode.h"
#include "NiTriBasedGeometry.h"
#include "NIUVController.h"

#include "BitUtil.h"
#include "ScriptUtil.h"
#include "TES3Util.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "BuildDate.h"
#include "CodePatchUtil.h"
#include "MWSEConfig.h"
#include "MWSEDefs.h"

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

		return inputController->readButtonPressed(key);
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
		unsigned char adjustedVolume = (unsigned char)(float(volume) * float(soundEvent->sound->volume) / 255.0f);
		audio->setSoundBufferVolume(soundEvent->soundBuffer, adjustedVolume);
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
	const auto TES3_UI_PostAddBoundItem = reinterpret_cast<void(__cdecl*)(TES3::Item*, TES3::ItemData*, int)>(0x5D1F00);

	TES3::EquipmentStack* createEquipBoundItem(TES3::Item* item, TES3::Actor* actor, TES3::MobileActor* mobile) {
		// Create and equip bound item. Excerpted from bound gauntlet code.
		TES3::EquipmentStack* equipped = nullptr;
		auto itemData = TES3::ItemData::createForObject(item);

		actor->inventory.addItem(mobile, item, 1, false, &itemData);
		actor->equipItem(item, itemData, &equipped, mobile);
		if (mobile->actorType == TES3::MobileActorType::Player) {
			TES3_UI_PostAddBoundItem(item, itemData, 1);
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
	}

	void installPostInitializationPatches() {

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

#ifdef APPVEYOR_BUILD_NUMBER
		log::getLog() << "MWSE version: " << MWSE_VERSION_MAJOR << "." << MWSE_VERSION_MINOR << "." << MWSE_VERSION_PATCH << "-" << APPVEYOR_BUILD_NUMBER << std::endl;
#else
		log::getLog() << "MWSE version: " << MWSE_VERSION_MAJOR << "." << MWSE_VERSION_MINOR << "." << MWSE_VERSION_PATCH << std::endl;
#endif
		log::getLog() << "Build date: " << MWSE_BUILD_DATE << std::endl;

		// Display the memory usage in the log.
		PROCESS_MEMORY_COUNTERS_EX memCounter = {};
		GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&memCounter, sizeof(memCounter));
		log::getLog() << "Memory usage: " << std::dec << memCounter.PrivateUsage << " bytes." << std::endl;
		if (memCounter.PrivateUsage > 3650722201) {
			log::getLog() << "  Memory usage is high. Crash is likely due to running out of memory." << std::endl;
		}

		// Try to print the lua stack trace.
		log::getLog() << "Lua traceback at time of crash:" << std::endl;
		lua::logStackTrace();

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
			return genCallEnforced(0x7279AD, 0x416E10, reinterpret_cast<DWORD>(onWinMain));
		}
		else {
			return true;
		}
	}
}
