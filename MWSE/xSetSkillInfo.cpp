#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3DataHandler.h"
#include "TES3MobilePlayer.h"
#include "TES3Skill.h"
#include "TES3WorldController.h"

namespace mwse {
	class xSetSkillInfo : InstructionInterface_t {
	public:
		xSetSkillInfo();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xSetSkillInfo xSetSkillInfoInstance;

	xSetSkillInfo::xSetSkillInfo() : mwse::InstructionInterface_t(OpCode::xSetSkillInfo) {}

	float xSetSkillInfo::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get parameter.
		long skillIndex = mwse::Stack::getInstance().popLong();

		// Return values.
		long attributeId = mwse::Stack::getInstance().popLong();
		long specialization = mwse::Stack::getInstance().popLong();
		float action1 = mwse::Stack::getInstance().popFloat();
		float action2 = mwse::Stack::getInstance().popFloat();
		float action3 = mwse::Stack::getInstance().popFloat();
		float action4 = mwse::Stack::getInstance().popFloat();

		// Validate skill index.
		if (skillIndex < TES3::SkillID::FirstSkill || skillIndex > TES3::SkillID::LastSkill) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xSetSkillInfo: Skill index out of range." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(false);
			return 0.0f;
		}

		// Validate attribute.
		if (attributeId < TES3::Attribute::FirstAttribute || attributeId > TES3::Attribute::LastAttribute) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xSetSkillInfo: Attribute id out of range." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(false);
			return 0.0f;
		}

		// Validate specialization.
		if (specialization < TES3::SkillSpecialization::FirstSpecialization || specialization > TES3::SkillSpecialization::LastSpecialization) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xSetSkillInfo: Specialization out of range." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(false);
			return 0.0f;
		}

		// Get skill info.
		TES3::Skill& skillInfo = TES3::DataHandler::get()->nonDynamicData->skills[skillIndex];

		// Store old specialization for future check.
		long oldSpecialization = skillInfo.specialization;

		// Set skill info values.
		skillInfo.governingAttribute = attributeId;
		skillInfo.specialization = specialization;
		skillInfo.progressActions[0] = action1;
		skillInfo.progressActions[1] = action2;
		skillInfo.progressActions[2] = action3;
		skillInfo.progressActions[3] = action4;

		// If our specialization changed, check for a skill level up.
		if (oldSpecialization != specialization) {
			TES3::WorldController::get()->getMobilePlayer()->progressSkillLevelIfRequirementsMet(skillIndex);
		}

		mwse::Stack::getInstance().pushLong(true);

		return 0.0f;
	}
}
