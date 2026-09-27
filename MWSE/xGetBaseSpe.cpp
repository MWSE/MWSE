#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "TES3MobileNPC.h"
#include "TES3Reference.h"

namespace mwse {
	class xGetBaseSpe : InstructionInterface_t {
	public:
		xGetBaseSpe();
		float execute(VMExecuteInterface& virtualMachine) override;
	private:
		const float INVALID_VALUE = -1.0f;
	};

	static xGetBaseSpe xGetBaseSpeInstance;

	xGetBaseSpe::xGetBaseSpe() : InstructionInterface_t(OpCode::xGetBaseSpe) {}

	float xGetBaseSpe::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get the associated MACP record.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xGetBaseSpe: No reference provided." << std::endl;
			}
			stack.pushFloat(INVALID_VALUE);
			return 0.0f;
		}

		auto mobileObject = reference->getAttachedMobileActor();
		if (mobileObject == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xGetBaseSpe: Could not find MACP record for reference." << std::endl;
			}
			stack.pushFloat(INVALID_VALUE);
			return 0.0f;
		}

		// Push the current value of that attribute.
		stack.pushFloat(mobileObject->attributes[TES3::Attribute::Speed].base);

		return 0.0f;
	}
}
