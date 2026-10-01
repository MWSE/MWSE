#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

namespace mwse {
	class xArcTan2 : InstructionInterface_t {
	public:
		xArcTan2();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xArcTan2 xArcTan2Instance;

	xArcTan2::xArcTan2() : InstructionInterface_t(OpCode::xArcTan2) {}

	float xArcTan2::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		float param1 = stack.popFloat();
		float param2 = stack.popFloat();

		stack.pushFloat(std::atan2(param1, param2));

		return 0.0f;
	}
}
