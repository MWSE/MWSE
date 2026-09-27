#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3MobilePlayer.h"
#include "TES3Reference.h"
#include "TES3Skill.h"
#include "TES3WorldController.h"

namespace mwse {
	class xGetProgressSkill : InstructionInterface_t {
	public:
		xGetProgressSkill();
		float execute(VMExecuteInterface& virtualMachine) override;
	private:
		const float INVALID_VALUE = -1.0f;
	};

	static xGetProgressSkill xGetProgressSkillInstance;

	xGetProgressSkill::xGetProgressSkill() : mwse::InstructionInterface_t(OpCode::xGetProgressSkill) {}

	float xGetProgressSkill::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get parameter off the stack.
		long skillIndex = mwse::Stack::getInstance().popLong();
		if (skillIndex < TES3::SkillID::FirstSkill || skillIndex > TES3::SkillID::LastSkill) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetProgressSkill: Invalid skill index provided." << std::endl;
			}
			mwse::Stack::getInstance().pushFloat(INVALID_VALUE);
			mwse::Stack::getInstance().pushFloat(INVALID_VALUE);
			return 0.0f;
		}

		// Get reference.
		auto macp = TES3::WorldController::get()->getMobilePlayer();

		float progress = macp->skillProgress[skillIndex];
		float normalized = 100.0f * progress / macp->getSkillRequirement(skillIndex);

		// Push the desired values.
		mwse::Stack::getInstance().pushFloat(normalized);
		mwse::Stack::getInstance().pushFloat(progress);

		return 0.0f;
	}
}
