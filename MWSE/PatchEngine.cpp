#include "PatchEngine.h"

#include "Log.h"
#include "MemoryUtil.h"
#include "MWSEConfig.h"
#include "TES3Util.h"
#include "WindowsUtil.h"

#include "TES3Cell.h"
#include "TES3CutscenePlayer.h"
#include "TES3GameFile.h"
#include "TES3DataHandler.h"
#include "TES3Game.h"
#include "TES3LoadScreenManager.h"
#include "TES3Reference.h"
#include "TES3WorldController.h"
#include "ReferenceTracker.h"

namespace mwse::patch::engine {

#ifndef _DEBUG
	// Release builds.
	constexpr auto VALIDATE_RECORDS_HANDLER_REFERENCE_LOOKUP = false;
#else
	// Debug builds.
	constexpr auto VALIDATE_RECORDS_HANDLER_REFERENCE_LOOKUP = true;
#endif

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
	// Patch: Optimize access to global variables. Access them in a hashmap instead of linear searching.
	//

	auto __fastcall DataHandlerCreateGlobalsContainer(void* garbage) {
		se::memory::_delete(garbage);
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

	// Replacement for the Sleep() call inside DataHandler::sub_48F5F0's
	// background-load polling loop. Ignores the value originally pushed by the
	// call site and re-reads the configured interval on every invocation, so
	// the MCM slider can be tweaked live without restarting the game.
	static void __stdcall PatchBackgroundLoadSleep(DWORD) {
		Sleep(Configuration::BackgroundLoadPollIntervalMs);
	}

	//
	// Patch: Implement map-based lookup of cells by ID.
	//

	static TES3::Cell* __fastcall PatchRecordsHandlerGetCellByName(TES3::NonDynamicData* self, DWORD, const char* name) {
		return self->getCellByName(name);
	}

	//
	// Patch: Implement map-based lookup of references.
	//

	static const char* getReferenceLookupLogId(const TES3::BaseObject* object) {
		__try {
			const auto id = object ? object->getObjectID() : "<nullptr>";
			return id ? id : "<invalid>";
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			return "<exception>";
		}
	}

	static TES3::Cell* getReferenceLookupLogCell(const TES3::Reference* reference) {
		__try {
			return reference ? reference->getCell() : nullptr;
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			return nullptr;
		}
	}

	static std::string getReferenceLookupLogCellName(const TES3::Reference* reference) {
		const auto cell = getReferenceLookupLogCell(reference);
		if (cell == nullptr) {
			return "<nullptr>";
		}

		return cell->getEditorName();
	}

	static void logReferenceLookupResultDetails(const char* label, const TES3::Reference* reference) {
		const auto cell = getReferenceLookupLogCell(reference);
		log::getLog()
			<< "  " << label << ": ref=" << reference
			<< " id=" << getReferenceLookupLogId(reference)
			<< " cell=" << cell
			<< " cellName=" << getReferenceLookupLogCellName(reference)
			<< " sourceID=" << (reference ? reference->sourceID : 0)
			<< " targetID=" << (reference ? reference->targetID : 0)
			<< std::endl;
	}

	static void validateRecordsHandlerReferenceLookupResult(const char* id, const TES3::Reference* result, const TES3::Reference* expected) {
		if (result != expected) {
			log::getLog() << "[MWSE] Reference lookup regression! Set a breakpoint to debug." << std::endl
				<< "  ID: " << (id ? id : "<invalid>") << std::endl
				<< "  Result: " << getReferenceLookupLogId(result) << std::endl
				<< "  Expected: " << getReferenceLookupLogId(expected) << std::endl;
			logReferenceLookupResultDetails("Result details", result);
			logReferenceLookupResultDetails("Expected details", expected);
		}
	}

	const auto TES3_RecordsHandler_findFirstInstanceOfObjectId = reinterpret_cast<TES3::Reference * (__thiscall*)(TES3::NonDynamicData*, const char*)>(0x4B8F50);
	static TES3::Reference* __fastcall PatchRecordsHandlerFindFirstInstanceOfObjectId(TES3::NonDynamicData* self, DWORD, const char* id) {
		const auto result = self->findFirstInstanceOfObjectId(id);
		if constexpr (VALIDATE_RECORDS_HANDLER_REFERENCE_LOOKUP) {
			const auto vanillaResult = TES3_RecordsHandler_findFirstInstanceOfObjectId(self, id);
			validateRecordsHandlerReferenceLookupResult(id, result, vanillaResult);
		}
		return result;
	}

	const auto TES3_RecordsHandler_findEntityInWorld = reinterpret_cast<TES3::Reference * (__thiscall*)(TES3::NonDynamicData*, TES3::BaseObject*)>(0x4B90F0);
	static TES3::Reference* __fastcall PatchRecordsHandlerFindEntityInWorld(TES3::NonDynamicData* self, DWORD, TES3::BaseObject* object) {
		const auto result = self->findEntityInWorld(object);
		if constexpr (VALIDATE_RECORDS_HANDLER_REFERENCE_LOOKUP) {
			const auto vanillaResult = TES3_RecordsHandler_findEntityInWorld(self, object);
			validateRecordsHandlerReferenceLookupResult(object->getObjectID(), result, vanillaResult);
		}
		return result;
	}

	const auto TES3_RecordsHandler_findClosestReferenceOfObject = reinterpret_cast<TES3::Reference * (__thiscall*)(TES3::NonDynamicData*, TES3::BaseObject*, NI::Point3*, bool, int)>(0x4B96F0);
	static TES3::Reference* __fastcall PatchRecordsHandlerFindClosestReferenceOfObject(TES3::NonDynamicData* self, DWORD, TES3::BaseObject* object, NI::Point3* position, bool isExterior, int maxGridSearchRadius) {
		const auto result = self->findClosestExteriorReferenceOfObject(object ? object->asPhysicalObject() : nullptr, position, isExterior, maxGridSearchRadius);
		if constexpr (VALIDATE_RECORDS_HANDLER_REFERENCE_LOOKUP) {
			const auto vanillaResult = TES3_RecordsHandler_findClosestReferenceOfObject(self, object, position, isExterior, maxGridSearchRadius);
			validateRecordsHandlerReferenceLookupResult(object ? object->getObjectID() : nullptr, result, vanillaResult);
		}
		return result;
	}

	static bool isCellReferenceList(se::LinkedObjectList<TES3::Object>* list) {
		__try {
			const auto referenceList = reinterpret_cast<TES3::ReferenceList*>(list);
			const auto cell = referenceList->cell;
			if (cell == nullptr) {
				return false;
			}

			if (cell->objectType != TES3::ObjectType::Cell) {
				return false;
			}

			if (referenceList != &cell->actors
				&& referenceList != &cell->persistentRefs
				&& referenceList != &cell->temporaryRefs) {
				return false;
			}

			return true;
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			return false;
		}
	}

	static TES3::Object* __fastcall PatchEntityListInsertAfter(se::LinkedObjectList<TES3::Object>* self, DWORD, TES3::Object* insertAfter, TES3::Object* item) {
		item->previousInCollection = insertAfter;
		if (insertAfter) {
			item->nextInCollection = insertAfter->nextInCollection;
			if (insertAfter->nextInCollection) {
				insertAfter->nextInCollection->previousInCollection = item;
			}
			insertAfter->nextInCollection = item;
		}
		else {
			item->nextInCollection = nullptr;
		}

		if (!item->nextInCollection) {
			self->tail = item;
		}
		if (!item->previousInCollection) {
			self->head = item;
		}

		++self->count;
		item->owningCollection.asGenericList = self;

		if (item->objectType == TES3::ObjectType::Reference && isCellReferenceList(self)) {
			ReferenceTracker::trackReferenceForLookup(static_cast<TES3::Reference*>(item));
		}

		return item;
	}

	static void __fastcall PatchEntityListRemove(se::LinkedObjectList<TES3::Object>* self, DWORD, TES3::Object* item) {
		if (item->objectType == TES3::ObjectType::Reference && isCellReferenceList(self)) {
			ReferenceTracker::untrackReferenceForLookup(static_cast<TES3::Reference*>(item));
		}

		if (item == self->head) {
			self->head = item->nextInCollection;
		}
		if (item == self->tail) {
			self->tail = item->previousInCollection;
		}
		if (item->previousInCollection) {
			item->previousInCollection->nextInCollection = item->nextInCollection;
		}
		if (item->nextInCollection) {
			item->nextInCollection->previousInCollection = item->previousInCollection;
		}

		item->owningCollection.asReferenceList = nullptr;
		--self->count;
	}

	const auto TES3_Cell_static_loadReference = reinterpret_cast<bool(__cdecl*)(TES3::Reference*, TES3::GameFile*, bool, bool, TES3::Cell*)>(0x4DE380);
	static bool __cdecl PatchCellLoadReference(TES3::Reference* reference, TES3::GameFile* gameFile, bool mustBePersistent, bool insertNew, TES3::Cell* cell) {
		const auto previousLookupKey = ReferenceTracker::getLookupKey(reference);
		const auto result = TES3_Cell_static_loadReference(reference, gameFile, mustBePersistent, insertNew, cell);
		if (
			cell
			&& reference->baseObject
			&& reference->baseObject->isMobileCapableActor()
			&& reference->owningCollection.asReferenceList == &cell->temporaryRefs
			) {
			cell->temporaryRefs.remove(reference);
			cell->actors.insertAtEnd(reference);
			const auto objectId = reference->baseObject->getObjectID();
			mwse::log::getLog() << fmt::format(
				"[MWSE] Warning: Loaded actor '{}' from the temporary references list of cell '{}' in '{}'.",
				objectId ? objectId : "<invalid>",
				cell->getEditorName(),
				gameFile ? gameFile->getFilename() : "<unknown>"
			) << std::endl;
		}
		ReferenceTracker::rekeyReference(reference, previousLookupKey);
		return result;
	}

	void install() {
		using se::memory::genCallEnforced;
		using se::memory::genCallUnprotected;
		using se::memory::genJumpUnprotected;
		using se::memory::overrideVirtualTableEnforced;

		// Patch: Allow Morrowind.ini [Application] values to override registry-backed application settings.
		auto Game_readSettings = &TES3::Game::readSettings;
		overrideVirtualTableEnforced(0x74665C, offsetof(TES3::Game_vTable, ReadSettings), 0x4F54F0, *reinterpret_cast<DWORD*>(&Game_readSettings));

		// Patch: Post-simulate event just before tickClock.
		// Patch: Don't truncate hour when advancing time past midnight.
		// Also don't nudge time forward by small extra increments when resting.
		auto WorldController_tickClock = &TES3::WorldController::tickClock;
		genCallEnforced(0x41B857, 0x40FF50, *reinterpret_cast<DWORD*>(&WorldController_tickClock));
		auto WorldController_checkForDayWrapping = &TES3::WorldController::checkForDayWrapping;
		genCallEnforced(0x6350E9, 0x40FF50, *reinterpret_cast<DWORD*>(&WorldController_checkForDayWrapping));

		// Patch: Fix crash in NPC wander and flee logic when trying to pick a random node from a pathgrid with 0 nodes.
		genCallEnforced(0x5339D8, 0x4E2850, reinterpret_cast<DWORD>(PatchCellGetPathGridWithNodes));
		genCallEnforced(0x549E76, 0x4E2850, reinterpret_cast<DWORD>(PatchCellGetPathGridWithNodes));

		// Patch: Prevent error messageboxes from creating a rogue process.
		genCallEnforced(0x47731B, 0x5F2160, reinterpret_cast<DWORD>(SafeQuitGetMessageChoice));
		genCallEnforced(0x4779D9, 0x5F2160, reinterpret_cast<DWORD>(SafeQuitGetMessageChoice));
		genCallEnforced(0x477E6F, 0x5F2160, reinterpret_cast<DWORD>(SafeQuitGetMessageChoice));

		// Patch: Suppress sGeneralMastPlugMismatchMsg message.
		genCallUnprotected(0x477512, reinterpret_cast<DWORD>(GetCachedYesToAll), 0x477518 - 0x477512);
		genCallEnforced(0x4BB55D, 0x477400, reinterpret_cast<DWORD>(SuppressGeneralMastPlugMismatchMsg));


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

		// Patch: Letterbox movies.
		genCallEnforced(0x64FC55, 0x64FE20, reinterpret_cast<DWORD>(PatchDrawLetterboxMovieFrame));
		genCallEnforced(0x64FC9C, 0x64FE20, reinterpret_cast<DWORD>(PatchDrawLetterboxMovieFrame));
		genCallEnforced(0x64FCDF, 0x64FE20, reinterpret_cast<DWORD>(PatchDrawLetterboxMovieFrame));
		genCallEnforced(0x64FD23, 0x64FE20, reinterpret_cast<DWORD>(PatchDrawLetterboxMovieFrame));
		genCallEnforced(0x64FD69, 0x64FE20, reinterpret_cast<DWORD>(PatchDrawLetterboxMovieFrame));
		genCallEnforced(0x64FDA1, 0x64FE20, reinterpret_cast<DWORD>(PatchDrawLetterboxMovieFrame));
		genCallEnforced(0x64FDD2, 0x64FE20, reinterpret_cast<DWORD>(PatchDrawLetterboxMovieFrame));
		genCallEnforced(0x64FE03, 0x64FE20, reinterpret_cast<DWORD>(PatchDrawLetterboxMovieFrame));

		// Patch: Cache cell lookups by ID.
		genJumpUnprotected(0x4BA9B0, reinterpret_cast<DWORD>(PatchRecordsHandlerGetCellByName), 0x5);

		// Patch: Implement map-based lookup of references.
		genJumpUnprotected(0x4F19F0, reinterpret_cast<DWORD>(PatchEntityListInsertAfter), 0x5);
		genJumpUnprotected(0x4F19A0, reinterpret_cast<DWORD>(PatchEntityListRemove), 0x5);
		genCallEnforced(0x4A43A5, 0x4B8F50, reinterpret_cast<DWORD>(PatchRecordsHandlerFindFirstInstanceOfObjectId));
		genCallEnforced(0x4F8FBB, 0x4B8F50, reinterpret_cast<DWORD>(PatchRecordsHandlerFindFirstInstanceOfObjectId));
		genCallEnforced(0x4FA93D, 0x4B8F50, reinterpret_cast<DWORD>(PatchRecordsHandlerFindFirstInstanceOfObjectId));
		genCallEnforced(0x4FA9FD, 0x4B8F50, reinterpret_cast<DWORD>(PatchRecordsHandlerFindFirstInstanceOfObjectId));
		genCallEnforced(0x4FABB2, 0x4B8F50, reinterpret_cast<DWORD>(PatchRecordsHandlerFindFirstInstanceOfObjectId));
		genCallEnforced(0x4FC158, 0x4B8F50, reinterpret_cast<DWORD>(PatchRecordsHandlerFindFirstInstanceOfObjectId));
		genCallEnforced(0x4FD05D, 0x4B8F50, reinterpret_cast<DWORD>(PatchRecordsHandlerFindFirstInstanceOfObjectId));
		genCallEnforced(0x4FD1ED, 0x4B8F50, reinterpret_cast<DWORD>(PatchRecordsHandlerFindFirstInstanceOfObjectId));
		genCallEnforced(0x4FD2F0, 0x4B8F50, reinterpret_cast<DWORD>(PatchRecordsHandlerFindFirstInstanceOfObjectId));
		genCallEnforced(0x4FDC36, 0x4B8F50, reinterpret_cast<DWORD>(PatchRecordsHandlerFindFirstInstanceOfObjectId));
		genCallEnforced(0x4FDF93, 0x4B8F50, reinterpret_cast<DWORD>(PatchRecordsHandlerFindFirstInstanceOfObjectId));
		genCallEnforced(0x4FE07A, 0x4B8F50, reinterpret_cast<DWORD>(PatchRecordsHandlerFindFirstInstanceOfObjectId));
		genCallEnforced(0x4FF24C, 0x4B8F50, reinterpret_cast<DWORD>(PatchRecordsHandlerFindFirstInstanceOfObjectId));
		genCallEnforced(0x5008E6, 0x4B8F50, reinterpret_cast<DWORD>(PatchRecordsHandlerFindFirstInstanceOfObjectId));
		genCallEnforced(0x491E04, 0x4B90F0, reinterpret_cast<DWORD>(PatchRecordsHandlerFindEntityInWorld));
		genCallEnforced(0x50C2BC, 0x4B90F0, reinterpret_cast<DWORD>(PatchRecordsHandlerFindEntityInWorld));
		genCallEnforced(0x48EEC7, 0x4B96F0, reinterpret_cast<DWORD>(PatchRecordsHandlerFindClosestReferenceOfObject));
		genCallEnforced(0x48EFDE, 0x4B96F0, reinterpret_cast<DWORD>(PatchRecordsHandlerFindClosestReferenceOfObject));
		genCallEnforced(0x4C5AC7, 0x4B96F0, reinterpret_cast<DWORD>(PatchRecordsHandlerFindClosestReferenceOfObject));
		genCallEnforced(0x4C01C4, 0x4DE380, reinterpret_cast<DWORD>(PatchCellLoadReference));
		genCallEnforced(0x4DD38C, 0x4DE380, reinterpret_cast<DWORD>(PatchCellLoadReference));
		genCallEnforced(0x4DD45E, 0x4DE380, reinterpret_cast<DWORD>(PatchCellLoadReference));
		genCallEnforced(0x4DDB0B, 0x4DE380, reinterpret_cast<DWORD>(PatchCellLoadReference));
		genCallEnforced(0x4DDBD3, 0x4DE380, reinterpret_cast<DWORD>(PatchCellLoadReference));
		genCallEnforced(0x4DE2B4, 0x4DE380, reinterpret_cast<DWORD>(PatchCellLoadReference));
		genCallEnforced(0x4E0D04, 0x4DE380, reinterpret_cast<DWORD>(PatchCellLoadReference));
	}

	void installPostLua() {
		// Patch: Replace the Sleep(100) call inside the background loader's
		// progress-show polling loop with a wrapper that reads
		// Configuration::BackgroundLoadPollIntervalMs at every call. The site
		// is a 6-byte `FF 15 [IAT_Sleep]` indirect call at 0x48F88E; the
		// preceding `push 64h` is left in place and ignored by the wrapper.
		se::memory::genCallUnprotected(0x48F88E, reinterpret_cast<DWORD>(PatchBackgroundLoadSleep), 0x6);
	}

	void installPostInitialization() {
		// Patch: Give threads descriptions.
		const auto dataHandler = TES3::DataHandler::get();
		if (dataHandler) {
			se::windows::SetThreadDescription(dataHandler->mainThread, L"GameMainThread");
			se::windows::SetThreadDescription(dataHandler->backgroundThread, L"GameBackgroundThread");
		}
	}
}
