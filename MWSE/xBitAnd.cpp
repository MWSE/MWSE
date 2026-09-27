#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

namespace mwse {
	class xBitAnd : InstructionInterface_t {
	public:
		xBitAnd();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xBitAnd xBitAndInstance;

	xBitAnd::xBitAnd() : mwse::InstructionInterface_t(OpCode::xBitAnd) {}

	float xBitAnd::execute(mwse::VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		long param1 = stack.popLong();
		long param2 = stack.popLong();

		stack.pushLong(param1 & param2);

		return 0.0f;
	}
}
