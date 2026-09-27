#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "mwAdapter.h"
#include "VirtualMachine.h"
#include "ScriptUtil.h"

namespace mwse {
	class xPosition : InstructionInterface_t {
	public:
		xPosition();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xPosition xPositionInstance;

	xPosition::xPosition() : InstructionInterface_t(OpCode::xPosition) {}

	float xPosition::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get parameters.
		float x = stack.popFloat();
		float y = stack.popFloat();
		float z = stack.popFloat();
		float rotation = stack.popFloat();

		// Get other context information for original opcode.
		TES3::Reference* reference = getReference(virtualMachine, __FUNCTION__);
		if (reference == nullptr) {
			return 0.0f;
		}

		// Call the original function.
		TES3::Script* script = virtualMachine.getScript();
		mwscript::Position(script, reference, x, y, z, rotation);

		return 0.0f;
	}
}
