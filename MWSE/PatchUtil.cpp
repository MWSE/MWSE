#include "PatchUtil.h"

#include "mwOffsets.h"
#include "MemoryUtil.h"
#include "Log.h"

#include "TES3Actor.h"
#include "TES3BodyPartManager.h"
#include "TES3Cell.h"
#include "TES3DataHandler.h"
#include "TES3Game.h"
#include "TES3GameFile.h"
#include "TES3GameSetting.h"
#include "TES3InputController.h"
#include "TES3Misc.h"
#include "TES3MobilePlayer.h"
#include "TES3Reference.h"
#include "TES3Script.h"
#include "TES3UIElement.h"
#include "TES3UIInventoryTile.h"
#include "TES3WorldController.h"

#include "NIFlipController.h"
#include "NILinesData.h"
#include "NIUVController.h"

#include "BitUtil.h"
#include "TES3Util.h"
#include "ScriptUtil.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "MWSEConfig.h"
#include "MWSEDefs.h"
#include "BuildDate.h"
#include "CodePatchUtil.h"

namespace mwse {
	namespace patch {

		//
		// Patch: Enable
		//

		void PatchScriptOpEnable() {
			TES3::ScriptVariables* scriptVars = mwscript::getLocalScriptVariables();
			if (scriptVars) {
				scriptVars->unknown_0xC &= 0xFE;
			}
		}

		//
		// Patch: Disable
		//

		static bool PatchScriptOpDisable_ForceCollisionUpdate = false;

		void PatchScriptOpDisable() {
			TES3::ScriptVariables* scriptVars = mwscript::getLocalScriptVariables();
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
			TES3::WorldController* worldController = TES3::WorldController::get();
			TES3::MobilePlayer* mobilePlayer = worldController->getMobilePlayer();

			TES3::Skill* athletics = &TES3::DataHandler::get()->nonDynamicData->skills[TES3::SkillID::Athletics];

			// If we're running, use the first progress.
			if (mobilePlayer->movementFlags & TES3::ActorMovement::Running) {
				mobilePlayer->exerciseSkill(TES3::SkillID::Athletics, athletics->progressActions[0] * worldController->deltaTime);
			}

			// If we're swimming, use the second progress.
			if (mobilePlayer->movementFlags & TES3::ActorMovement::Swimming) {
				mobilePlayer->exerciseSkill(TES3::SkillID::Athletics, athletics->progressActions[1] * worldController->deltaTime);
			}
		}

		//
		// Patch: Unify sneak training.
		//

		void PatchUnifySneakTraining() {
			TES3::NonDynamicData* nonDynamicData = TES3::DataHandler::get()->nonDynamicData;

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

		const auto TES3_WorldController_InitGlobals = reinterpret_cast<void (__thiscall*)(TES3::WorldController*)>(0x40E920);
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
				SerializedFormId data;
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
		// Note: This assumes the target reference is of an actor.
		//

		TES3::Reference::ReferenceData* __fastcall PatchFixupActorSelfReference(TES3::Reference* self) {
			if (self->baseObject->referenceToThis == nullptr) {
				self->baseObject->referenceToThis = self;

				using namespace mwse::log;
				auto& log = getLog();
				log << "[MWSE] Fixed crash with invalid RemoveItem call. Report this to the #mwse channel in the Morrowind Modding Community Discord so we can narrow this down more. Dumping related objects." << std::endl;

				log << "Reference: " << self->getObjectID() << std::endl;
				prettyDump(self);

				log << "Object: " << self->baseObject->getObjectID() << std::endl;
				prettyDump(static_cast<TES3::Actor*>(self->baseObject));

				auto script = self->baseObject->getScript();
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

			// First, raise C runtime fopen limit from 512 to 2048.
			bool canHandle2048OpenFiles = raiseStdioFileLimit();

			if (canHandle2048OpenFiles && mcp::getFeatureEnabled(mcp::feature::SavegameCorruptionFix)) {
				// Change hardcoded 256 checks to 1024.
				writeValueEnforced<DWORD>(0x4B7A22 + 0x1, PatchRaiseESXLimit::ModCountVanilla, PatchRaiseESXLimit::ModCountMWSE);
				writeValueEnforced<DWORD>(0x4BB4AE + 0x3, PatchRaiseESXLimit::ModCountVanilla, PatchRaiseESXLimit::ModCountMWSE);
				writeValueEnforced<DWORD>(0x4BB588 + 0x3, PatchRaiseESXLimit::ModCountVanilla, PatchRaiseESXLimit::ModCountMWSE);

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
			}
#endif

			// Patch: Fix crash when trying to draw cell markers that don't fit on the map.
			auto NonDynamicData_drawCellMapMarker = &TES3::NonDynamicData::drawCellMapMarker;
			genCallEnforced(0x4C840F, 0x4C8540, *reinterpret_cast<DWORD*>(&NonDynamicData_drawCellMapMarker));
			genCallEnforced(0x4C8520, 0x4C8540, *reinterpret_cast<DWORD*>(&NonDynamicData_drawCellMapMarker));

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

			// Patch: Update player first and third person animations when the idle flag is pausing the controller.
			genCallUnprotected(0x41B836, reinterpret_cast<DWORD>(PatchUpdateAllIdles));

			// Patch: Correctly initialize MobileProjectile tag/objectType
			genCallEnforced(0x572444, 0x4EE8A0, reinterpret_cast<DWORD>(PatchInitializeMobileProjectileType));
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

		void CreateMiniDump(EXCEPTION_POINTERS* pep) {
			log::getLog() << std::endl;
			log::getLog() << "Morrowind has crashed! To help improve game stability, send MWSE_Minidump.dmp and mwse.log to NullCascade@gmail.com or to NullCascade#1010 on Discord." << std::endl;
			log::getLog() << "Additional support can be found in the #mwse channel at the Morrowind Modding Community Discord: https://discord.me/mwmods" << std::endl;

#ifdef APPVEYOR_BUILD_NUMBER
			mwse::log::getLog() << "MWSE version: " << MWSE_VERSION_MAJOR << "." << MWSE_VERSION_MINOR << "." << MWSE_VERSION_PATCH << "-" << APPVEYOR_BUILD_NUMBER << std::endl;
#else
			mwse::log::getLog() << "MWSE version: " << MWSE_VERSION_MAJOR << "." << MWSE_VERSION_MINOR << "." << MWSE_VERSION_PATCH << std::endl;
#endif
			mwse::log::getLog() << "Build date: " << MWSE_BUILD_DATE << std::endl;

			// Display the memory usage in the log.
			PROCESS_MEMORY_COUNTERS_EX memCounter;
			GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&memCounter, sizeof(memCounter));
			log::getLog() << "Memory usage: " << std::dec << memCounter.PrivateUsage << " bytes." << std::endl;

			// Try to print the lua stack trace.
			log::getLog() << "Lua traceback at time of crash:" << std::endl;
			mwse::lua::logStackTrace();

			// Try to print any relevant mwscript information.
			if (TES3::Script::currentlyExecutingScript) {
				log::getLog() << "Currently executing mwscript context:" << std::endl;
				safePrintObjectToLog("Script", TES3::Script::currentlyExecutingScript);
				safePrintObjectToLog("Reference", TES3::Script::currentlyExecutingScriptReference);
				log::getLog() << "  OpCode: 0x" << std::hex << *reinterpret_cast<DWORD*>(0x7A91C4) << std::endl;
				log::getLog() << "  Cursor Offset: 0x" << std::hex << *reinterpret_cast<DWORD*>(0x7CEBB0) << std::endl;
			}

			// Open the file.
			HANDLE hFile = CreateFile("MWSE_MiniDump.dmp", GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

			if ((hFile != NULL) && (hFile != INVALID_HANDLE_VALUE)) {
				// Create the minidump.
				MINIDUMP_EXCEPTION_INFORMATION mdei;

				mdei.ThreadId = GetCurrentThreadId();
				mdei.ExceptionPointers = pep;
				mdei.ClientPointers = FALSE;

				MINIDUMP_CALLBACK_INFORMATION mci;

				mci.CallbackRoutine = (MINIDUMP_CALLBACK_ROUTINE)miniDumpCallback;
				mci.CallbackParam = 0;

				MINIDUMP_TYPE mdt = (MINIDUMP_TYPE)(MiniDumpWithDataSegs |
					MiniDumpWithHandleData |
					MiniDumpWithFullMemoryInfo |
					MiniDumpWithThreadInfo |
					MiniDumpWithUnloadedModules);

				BOOL rv = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, mdt, (pep != 0) ? &mdei : 0, 0, &mci);

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
				return reinterpret_cast<int(__stdcall *)(HINSTANCE, HINSTANCE, LPSTR, int)>(0x416E10)(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
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
#ifndef _DEBUG
			// Create our hook.
			return genCallEnforced(0x7279AD, 0x416E10, reinterpret_cast<DWORD>(onWinMain));
#else
			return true;
#endif
		}
	}
}
