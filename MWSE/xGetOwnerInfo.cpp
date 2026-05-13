#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3Faction.h"
#include "TES3GlobalVariable.h"
#include "TES3ItemData.h"
#include "TES3Reference.h"

namespace mwse {
	class xGetOwnerInfo : InstructionInterface_t {
	public:
		xGetOwnerInfo();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xGetOwnerInfo xGetOwnerInfoInstance;

	xGetOwnerInfo::xGetOwnerInfo() : mwse::InstructionInterface_t(OpCode::xGetOwnerInfo) {}

	float xGetOwnerInfo::execute(mwse::VMExecuteInterface& virtualMachine) {
		const char* id = NULL;
		long rank = 0;
		long type = 0;

		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference) {
			// Get the attached varnode as owner information.
			auto varNode = reference->getAttachedItemData();
			if (varNode) {
				TES3::BaseObject* owner = varNode->owner;
				if (owner) {
					type = owner->objectType;
					if (type == TES3::ObjectType::NPC) {
						id = owner->vTable.object->getObjectID(owner);
						if (varNode->requiredVariable) {
							rank = se::string::store::getOrCreate(varNode->requiredVariable->name);
						}
					}
					else if (type == TES3::ObjectType::Faction) {
						TES3::Faction* faction = reinterpret_cast<TES3::Faction*>(owner);
						id = faction->objectID;
						rank = varNode->requiredRank;
					}
					else {
						if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
							mwse::log::getLog() << "xGetOwnerInfo: Owner was of unhandled type " << type << "." << std::endl;
						}
					}
				}
			}
			else {
				if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
					mwse::log::getLog() << "xGetOwnerInfo: Could not obtain attached VARNODE." << std::endl;
				}
			}
		}
		else {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetOwnerInfo: No reference provided." << std::endl;
			}
		}

		mwse::Stack::getInstance().pushLong(rank);
		mwse::Stack::getInstance().pushString(id);
		mwse::Stack::getInstance().pushLong(type);

		return 0.0f;
	}
}
