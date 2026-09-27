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

	xModProgressSkill::xModProgressSkill() : mwse::InstructionInterface_t(OpCode::xModProgressSkill) {}

	float xModProgressSkill::execute(mwse::VMExecuteInterface& virtualMachine) {
		if (mwse::Stack::getInstance().size() < 3) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xModProgressSkill: Function called with too few arguments." << std::endl;
			}
			return 0.0f;
		}

		long skillId = mwse::Stack::getInstance().popLong();
		float modValue = mwse::Stack::getInstance().popFloat();
		long normalize = mwse::Stack::getInstance().popLong();

		// Verify attribute range.
		if (skillId < TES3::SkillID::FirstSkill || skillId > TES3::SkillID::LastSkill) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xModProgressSkill: Invalid skill id: " << skillId << std::endl;
			}
			mwse::Stack::getInstance().pushLong(0);
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
		mwse::Stack::getInstance().pushLong(1);

		return 0.0f;
	}
}
