#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3MobileNPC.h"
#include "TES3Reference.h"
#include "TES3Skill.h"

namespace mwse {
	class xGetSkill : InstructionInterface_t {
	public:
		xGetSkill();
		float execute(VMExecuteInterface& virtualMachine) override;
	private:
		const float INVALID_VALUE = -1.0f;
	};

	static xGetSkill xGetSkillInstance;

	xGetSkill::xGetSkill() : InstructionInterface_t(OpCode::xGetSkill) {}

	float xGetSkill::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get skill id argument.
		long skillId = stack.popLong();
		if (skillId < TES3::SkillID::FirstSkill || skillId > TES3::SkillID::LastSkill) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xGetSkill: Invalid skill id: " << skillId << std::endl;
			}
			stack.pushFloat(INVALID_VALUE);
			return 0.0f;
		}

		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xGetSkill: Call on invalid reference." << std::endl;
			}
			stack.pushFloat(INVALID_VALUE);
			return 0.0f;
		}

		// Verify target record type.
		if (reference->baseObject->objectType != TES3::ObjectType::NPC && reference->baseObject->objectType != TES3::ObjectType::Creature) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xGetSkill: Reference is not a creature or NPC." << std::endl;
			}
			stack.pushFloat(INVALID_VALUE);
			return 0.0f;
		}

		// Get the associated MACP record.
		auto mobileObject = reference->getAttachedMobileNPC();
		if (mobileObject == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xGetSkill: Could not find MACP record for reference." << std::endl;
			}
			stack.pushFloat(INVALID_VALUE);
			return 0.0f;
		}

		// Push the current value of that skill.
		stack.pushFloat(mobileObject->skills[skillId].current);

		return 0.0f;
	}
}
