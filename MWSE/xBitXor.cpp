#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

namespace mwse {
	class xBitXor : InstructionInterface_t {
	public:
		xBitXor();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xBitXor xBitXorInstance;

	xBitXor::xBitXor() : InstructionInterface_t(OpCode::xBitXor) {}

	float xBitXor::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		long param1 = stack.popLong();
		long param2 = stack.popLong();

		stack.pushLong(param1 ^ param2);

		return 0.0f;
	}
}
