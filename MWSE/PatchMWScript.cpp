#include "PatchMWScript.h"

#include "Log.h"
#include "MemoryUtil.h"
#include "ScriptUtil.h"

#include "TES3Actor.h"
#include "TES3Cell.h"
#include "TES3DataHandler.h"
#include "TES3Reference.h"
#include "TES3Script.h"

namespace mwse::patch::mwscript {

	//
	// Patch: Enable
	//

	void PatchScriptOpEnable() {
		auto scriptVars = mwse::mwscript::getLocalScriptVariables();
		if (scriptVars) {
			scriptVars->unknown_0xC &= 0xFE;
		}
	}

	//
	// Patch: Disable
	//

	static bool PatchScriptOpDisable_ForceCollisionUpdate = false;

	void PatchScriptOpDisable() {
		auto scriptVars = mwse::mwscript::getLocalScriptVariables();
		if (scriptVars) {
			scriptVars->unknown_0xC |= 0x1;
		}

		// Determine if we need to force update collision.
		auto reference = mwse::mwscript::getScriptTargetReference();
		if (reference) {
			PatchScriptOpDisable_ForceCollisionUpdate = !reference->getDisabled();
		}
		else {
			PatchScriptOpDisable_ForceCollisionUpdate = false;
		}
	}

	void* __fastcall PatchScriptOpDisableCollision(TES3::Reference* reference) {
		// Force update collision.
		if (PatchScriptOpDisable_ForceCollisionUpdate && reference->getUpdatesCollisionGroups()) {
			TES3::DataHandler::get()->updateCollisionGroupsForActiveCells();
		}

		// Return overwritten code.
		return &reference->baseObject;
	}

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

	void install() {
		using se::memory::genCallEnforced;
		using se::memory::genCallUnprotected;
		using se::memory::overrideVirtualTableEnforced;
		using se::memory::writeValueEnforced;

		// Patch: Enable/Disable.
		genCallUnprotected(0x508FEB, reinterpret_cast<DWORD>(PatchScriptOpEnable), 0x9);
		genCallUnprotected(0x5090DB, reinterpret_cast<DWORD>(PatchScriptOpDisable), 0x9);
		genCallUnprotected(0x50912F, reinterpret_cast<DWORD>(PatchScriptOpDisableCollision));

		// Patch: Improve performance of script reloading.
		{
			auto Script_ctor = &TES3::Script::ctor;
			genCallEnforced(0x40E95B, 0x4FD830, *reinterpret_cast<DWORD*>(&Script_ctor));
			genCallEnforced(0x40E963, 0x4FD830, *reinterpret_cast<DWORD*>(&Script_ctor));
			genCallEnforced(0x4C0842, 0x4FD830, *reinterpret_cast<DWORD*>(&Script_ctor));
			writeValueEnforced<BYTE>(0x4C0824, 0x70, sizeof(TES3::Script));
			auto Script_loadRecordSpecific = &TES3::Script::loadRecordSpecific;
			overrideVirtualTableEnforced(0x74A990, 0x4, 0x4FF700, *reinterpret_cast<DWORD*>(&Script_loadRecordSpecific));
			auto Script_reloadScript = &TES3::Script::reloadScript;
			genCallEnforced(0x4C78A3, 0x4FF9D0, *reinterpret_cast<DWORD*>(&Script_reloadScript));

			// MCP adds a second script for the compiler. We need to patch that as a special case.
			if (writeValueEnforced<BYTE>(0x50E593, 0x70, sizeof(TES3::Script))) {
				writeValueEnforced<BYTE>(0x40E93F, 0xE0, sizeof(TES3::Script) * 2);
			}
			else {
				writeValueEnforced<BYTE>(0x40E93F, 0x70, sizeof(TES3::Script));
			}
		}

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
	}
}
