#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

namespace mwse {
	class xSqrt : InstructionInterface_t {
	public:
		xSqrt();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xSqrt xSqrtInstance;

	xSqrt::xSqrt() : InstructionInterface_t(OpCode::xSqrt) {}

	float xSqrt::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		stack.pushFloat(std::sqrt(stack.popFloat()));
		return 0.0f;
	}
}
