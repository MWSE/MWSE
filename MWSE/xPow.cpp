#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

namespace mwse {
	class xPow : InstructionInterface_t {
	public:
		xPow();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xPow xPowInstance;

	xPow::xPow() : InstructionInterface_t(OpCode::xPow) {}

	float xPow::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		float base = stack.popFloat();
		float exponent = stack.popFloat();

		stack.pushFloat(std::powf(base, exponent));

		return 0.0f;
	}
}
