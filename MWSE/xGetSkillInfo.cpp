#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3DataHandler.h"
#include "TES3MobileActor.h"
#include "TES3Skill.h"

namespace mwse {
	class xGetSkillInfo : InstructionInterface_t {
	public:
		xGetSkillInfo();
		float execute(VMExecuteInterface& virtualMachine) override;
	private:
		const float INVALID_VALUE = -1.0f;
	};

	static xGetSkillInfo xGetSkillInfoInstance;

	xGetSkillInfo::xGetSkillInfo() : InstructionInterface_t(OpCode::xGetSkillInfo) {}

	float xGetSkillInfo::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get parameter.
		long skillIndex = stack.popLong();

		// Return values.
		long attributeId = TES3::Attribute::Invalid;
		long specialization = TES3::SkillSpecialization::Invalid;
		float actions[4] = { INVALID_VALUE, INVALID_VALUE, INVALID_VALUE, INVALID_VALUE };

		// Validate skill index.
		if (skillIndex >= TES3::SkillID::FirstSkill && skillIndex <= TES3::SkillID::LastSkill) {
			auto dataHandler = TES3::DataHandler::get();
			const TES3::Skill& skillRecord = dataHandler->nonDynamicData->skills[skillIndex];
			attributeId = skillRecord.governingAttribute;
			specialization = skillRecord.specialization;
			for (size_t i = 0; i < 4; ++i) {
				actions[i] = skillRecord.progressActions[i];
			}
		}
		else {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xGetSkillInfo: Skill index out of range." << std::endl;
			}
		}

		// Push desired values.
		stack.pushFloat(actions[3]);
		stack.pushFloat(actions[2]);
		stack.pushFloat(actions[1]);
		stack.pushFloat(actions[0]);
		stack.pushLong(specialization);
		stack.pushLong(attributeId);

		return 0.0f;
	}
}
