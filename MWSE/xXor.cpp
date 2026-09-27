#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

namespace mwse {
	class xXor : InstructionInterface_t {
	public:
		xXor();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xXor xXorInstance;

	xXor::xXor() : mwse::InstructionInterface_t(OpCode::xXor) {}

	float xXor::execute(mwse::VMExecuteInterface &virtualMachine) {
		auto& stack = Stack::getInstance();
		long param1 = stack.popLong();
		long param2 = stack.popLong();

		stack.pushLong((param1 || param2) && !(param1 && param2));

		return 0.0f;
	}
}
