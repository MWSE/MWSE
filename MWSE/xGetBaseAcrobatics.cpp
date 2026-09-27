#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3Skill.h"
#include "TES3MobileNPC.h"
#include "TES3Reference.h"

namespace mwse {
	class xGetBaseAcrobatics : InstructionInterface_t {
	public:
		xGetBaseAcrobatics();
		float execute(VMExecuteInterface& virtualMachine) override;
	private:
		const float INVALID_VALUE = -1.0f;
	};

	static xGetBaseAcrobatics xGetBaseAcrobaticsInstance;

	xGetBaseAcrobatics::xGetBaseAcrobatics() : mwse::InstructionInterface_t(OpCode::xGetBaseAcrobatics) {}

	float xGetBaseAcrobatics::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get the associated MACP record.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetBaseAcrobatics: No reference provided." << std::endl;
			}
			mwse::Stack::getInstance().pushFloat(INVALID_VALUE);
			return 0.0f;
		}

		auto mobileObject = reference->getAttachedMobileNPC();
		if (mobileObject == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetBaseAcrobatics: Could not find mobile attachment for reference." << std::endl;
			}
			mwse::Stack::getInstance().pushFloat(INVALID_VALUE);
			return 0.0f;
		}

		// Push the base value of that skill.
		mwse::Stack::getInstance().pushFloat(mobileObject->skills[TES3::SkillID::Acrobatics].base);

		return 0.0f;
	}
}
