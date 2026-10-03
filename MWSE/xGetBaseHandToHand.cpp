#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "TES3MobileNPC.h"
#include "TES3Skill.h"
#include "TES3Reference.h"


namespace mwse {
	class xGetBaseHandToHand : InstructionInterface_t {
	public:
		xGetBaseHandToHand();
		float execute(VMExecuteInterface& virtualMachine) override;
	private:
		const float INVALID_VALUE = -1.0f;
	};

	static xGetBaseHandToHand xGetBaseHandToHandInstance;

	xGetBaseHandToHand::xGetBaseHandToHand() : InstructionInterface_t(OpCode::xGetBaseHandToHand) {}

	float xGetBaseHandToHand::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get the associated MACP record.
		TES3::Reference* reference = getReference(virtualMachine, __FUNCTION__);
		if (reference == nullptr) {
			stack.pushFloat(INVALID_VALUE);
			return 0.0f;
		}

		auto mobileObject = reference->getAttachedMobileNPC();
		if (mobileObject == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xGetBaseHandToHand: Could not find MACP record for reference." << std::endl;
			}
			stack.pushFloat(INVALID_VALUE);
			return 0.0f;
		}

		// Push the base value of that skill.
		stack.pushFloat(mobileObject->skills[TES3::SkillID::HandToHand].base);

		return 0.0f;
	}
}
