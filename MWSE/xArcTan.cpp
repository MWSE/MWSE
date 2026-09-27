#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

namespace mwse {
	class xArcTan : InstructionInterface_t {
	public:
		xArcTan();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xArcTan xArcTanInstance;

	xArcTan::xArcTan() : mwse::InstructionInterface_t(OpCode::xArcTan) {}

	float xArcTan::execute(mwse::VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		stack.pushFloat(std::atan(stack.popFloat()));
		return 0.0f;
	}
}
