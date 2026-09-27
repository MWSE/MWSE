#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "TES3MobileNPC.h"
#include "TES3Reference.h"

namespace mwse {
	class xGetMaxFatigue : InstructionInterface_t {
	public:
		xGetMaxFatigue();
		float execute(VMExecuteInterface& virtualMachine) override;
	private:
		const float INVALID_VALUE = -1.0f;
	};

	static xGetMaxFatigue xGetMaxFatigueInstance;

	xGetMaxFatigue::xGetMaxFatigue() : mwse::InstructionInterface_t(OpCode::xGetMaxFatigue) {}

	float xGetMaxFatigue::execute(mwse::VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get the associated MACP record.
		TES3::Reference* reference = virtualMachine.getReference();
		auto mobileObject = reference->getAttachedMobileActor();
		if (mobileObject == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetMaxFatigue: Could not find MACP record for reference." << std::endl;
			}
			stack.pushFloat(INVALID_VALUE);
			return 0.0f;
		}

		// Push the base value of the statistic.
		stack.pushFloat(mobileObject->fatigue.base);

		return 0.0f;
	}
}
