#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

namespace mwse {
	class xArcCos : InstructionInterface_t {
	public:
		xArcCos();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xArcCos xArcCosInstance;

	xArcCos::xArcCos() : InstructionInterface_t(OpCode::xArcCos) {}

	float xArcCos::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		stack.pushFloat(std::acos(stack.popFloat()));
		return 0.0f;
	}
}
