#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "TES3MobileNPC.h"
#include "TES3Reference.h"

namespace mwse {
	class xGetMaxMagicka : InstructionInterface_t {
	public:
		xGetMaxMagicka();
		float execute(VMExecuteInterface& virtualMachine) override;
	private:
		const float INVALID_VALUE = -1.0f;
	};

	static xGetMaxMagicka xGetMaxMagickaInstance;

	xGetMaxMagicka::xGetMaxMagicka() : mwse::InstructionInterface_t(OpCode::xGetMaxMagicka) {}

	float xGetMaxMagicka::execute(mwse::VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get the associated MACP record.
		TES3::Reference* reference = virtualMachine.getReference();
		auto mobileObject = reference->getAttachedMobileActor();
		if (mobileObject == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetMaxMagicka: Could not find MACP record for reference." << std::endl;
			}
			stack.pushFloat(INVALID_VALUE);
			return 0.0f;
		}

		// Push the base value of the statistic.
		stack.pushFloat(mobileObject->magicka.base);

		return 0.0f;
	}
}
