#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "mwAdapter.h"
#include "VirtualMachine.h"
#include "ScriptUtil.h"

namespace mwse {
	class xAITravel : InstructionInterface_t {
	public:
		xAITravel();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xAITravel xAITravelInstance;

	xAITravel::xAITravel() : InstructionInterface_t(OpCode::xAITravel) {}

	float xAITravel::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get parameters.
		float x = stack.popFloat();
		float y = stack.popFloat();
		float z = stack.popFloat();

		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xAITravel: Called on invalid reference." << std::endl;
			}
			return 0.0f;
		}

		// Call the original function.
		TES3::Script* script = virtualMachine.getScript();
		mwscript::AITravel(script, reference, x, y, z);

		return 0.0f;
	}
}
