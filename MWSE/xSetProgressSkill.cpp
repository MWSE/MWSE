#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3MobilePlayer.h"
#include "TES3Reference.h"
#include "TES3Skill.h"
#include "TES3WorldController.h"

namespace mwse {
	class xSetProgressSkill : InstructionInterface_t {
	public:
		xSetProgressSkill();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xSetProgressSkill xSetProgressSkillInstance;

	xSetProgressSkill::xSetProgressSkill() : InstructionInterface_t(OpCode::xSetProgressSkill) {}

	float xSetProgressSkill::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get parameters.
		long skillIndex = stack.popLong();
		float progress = stack.popFloat();
		long normalized = stack.popLong();

		// Get the associated MACP record.
		auto mobileObject = TES3::WorldController::get()->getMobilePlayer();
		if (mobileObject == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xSetProgressSkill: Could not find MACP record for reference." << std::endl;
			}
			stack.pushLong(false);
			return 0.0f;
		}

		// Verify skill index.
		if (skillIndex < TES3::SkillID::FirstSkill || skillIndex > TES3::SkillID::LastSkill) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xSetProgressSkill: Skill index out of bounds." << std::endl;
			}
			stack.pushLong(false);
			return 0.0f;
		}

		// Verify progress.
		if (progress < 0) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xSetProgressSkill: Progress cannot be negative." << std::endl;
			}
			stack.pushLong(false);
			return 0.0f;
		}

		// Normalize skill progress.
		if (normalized) {
			progress = mobileObject->getSkillRequirement(skillIndex) * progress / 100.0f;
		}

		// Set progress.
		mobileObject->skillProgress[skillIndex] = progress;

		// Check for skill level up.
		mobileObject->progressSkillLevelIfRequirementsMet(skillIndex);

		stack.pushLong(true);

		return 0.0f;
	}
}
