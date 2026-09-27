#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "TES3NPC.h"
#include "TES3Creature.h"
#include "TES3Reference.h"

namespace mwse {
	class xSetBaseGold : InstructionInterface_t {
	public:
		xSetBaseGold();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xSetBaseGold xSetBaseGoldInstance;

	xSetBaseGold::xSetBaseGold() : InstructionInterface_t(OpCode::xSetBaseGold) {}

	float xSetBaseGold::execute(VMExecuteInterface& virtualMachine) {
		short gold = Stack::getInstance().popShort();

		// Get reference.
		TES3::Reference* reference = getReference(virtualMachine, __FUNCTION__);
		if (reference == nullptr) {
			return 0.0f;
		}

		// Get the gold based on the base record type.
		TES3::BaseObject* baseRecord = reference->baseObject;
		if (baseRecord->objectType == TES3::ObjectType::NPC) {
			TES3::NPCInstance* npc = reinterpret_cast<TES3::NPCInstance*>(baseRecord);
			if (npc->baseNPC) {
				npc->baseNPC->barterGold = gold;
			}
			else {
				log::getLog() << "xSetBaseGold: Could not get base NPC record for \"" << npc->objectID << "\"" << std::endl;
			}
		}
		else if (baseRecord->objectType == TES3::ObjectType::Creature) {
			TES3::CreatureInstance* creature = reinterpret_cast<TES3::CreatureInstance*>(baseRecord);
			if (creature->baseCreature) {
				creature->baseCreature->barterGold = gold;
			}
			else {
				log::getLog() << "xSetBaseGold: Could not get base creature record for \"" << creature->objectID << "\"" << std::endl;
			}
		}

		return 0.0f;
	}
}
