#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

namespace mwse {
	class xSin : InstructionInterface_t {
	public:
		xSin();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xSin xSinInstance;

	xSin::xSin() : mwse::InstructionInterface_t(OpCode::xSin) {}

	float xSin::execute(mwse::VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		stack.pushFloat(std::sin(stack.popFloat()));
		return 0.0f;
	}
}
