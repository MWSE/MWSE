#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3MobilePlayer.h"
#include "TES3Reference.h"
#include "TES3Skill.h"
#include "TES3WorldController.h"

namespace mwse {
	class xModProgressSkill : InstructionInterface_t {
	public:
		xModProgressSkill();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xModProgressSkill xModProgressSkillInstance;

	xModProgressSkill::xModProgressSkill() : InstructionInterface_t(OpCode::xModProgressSkill) {}

	float xModProgressSkill::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		if (stack.size() < 3) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xModProgressSkill: Function called with too few arguments." << std::endl;
			}
			return 0.0f;
		}

		long skillId = stack.popLong();
		float modValue = stack.popFloat();
		long normalize = stack.popLong();

		// Verify attribute range.
		if (skillId < TES3::SkillID::FirstSkill || skillId > TES3::SkillID::LastSkill) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xModProgressSkill: Invalid skill id: " << skillId << std::endl;
			}
			stack.pushLong(0);
			return 0.0f;
		}

		//
		auto macp = TES3::WorldController::get()->getMobilePlayer();

		// Mod value.
		float progress = macp->skillProgress[skillId];

		// Normalize progress, then add mod, then convert back.
		// This avoids some floating point precision errors.
		if (normalize) {
			const float requirement = macp->getSkillRequirement(skillId);
			progress = 100.0f * progress / requirement + modValue;
			progress = requirement * progress / 100.0f;
		}
		else {
			progress += modValue;
		}

		if (progress < 0.0f) {
			progress = 0.0f;
		}
		macp->skillProgress[skillId] = progress;

		// Call Morrowind's native CheckForSkillUp function.
		macp->progressSkillLevelIfRequirementsMet(skillId);

		// Push to indicate success.
		stack.pushLong(1);

		return 0.0f;
	}
}
