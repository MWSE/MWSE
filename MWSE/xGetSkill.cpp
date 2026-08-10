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
		virtual float execute(VMExecuteInterface& virtualMachine);
	private:
		const float INVALID_VALUE = -1.0f;
	};

	static xGetSkill xGetSkillInstance;

	xGetSkill::xGetSkill() : mwse::InstructionInterface_t(OpCode::xGetSkill) {}

	float xGetSkill::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get skill id argument.
		long skillId = mwse::Stack::getInstance().popLong();
		if (skillId < TES3::SkillID::FirstSkill || skillId > TES3::SkillID::LastSkill) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetSkill: Invalid skill id: " << skillId << std::endl;
			}
			mwse::Stack::getInstance().pushFloat(INVALID_VALUE);
			return 0.0f;
		}

		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetSkill: Call on invalid reference." << std::endl;
			}
			mwse::Stack::getInstance().pushFloat(INVALID_VALUE);
			return 0.0f;
		}

		// Verify target record type.
		if (reference->baseObject->objectType != TES3::ObjectType::NPC && reference->baseObject->objectType != TES3::ObjectType::Creature) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetSkill: Reference is not a creature or NPC." << std::endl;
			}
			mwse::Stack::getInstance().pushFloat(INVALID_VALUE);
			return 0.0f;
		}

		// Get the associated MACP record.
		auto mobileObject = reference->getAttachedMobileNPC();
		if (mobileObject == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetSkill: Could not find MACP record for reference." << std::endl;
			}
			mwse::Stack::getInstance().pushFloat(INVALID_VALUE);
			return 0.0f;
		}

		// Push the current value of that skill.
		mwse::Stack::getInstance().pushFloat(mobileObject->skills[skillId].current);

		return 0.0f;
	}
}
