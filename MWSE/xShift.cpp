#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

namespace mwse {
	class xShift : InstructionInterface_t {
	public:
		xShift();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xShift xShiftInstance;

	xShift::xShift() : InstructionInterface_t(OpCode::xShift) {}

	float xShift::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		long value = stack.popLong();
		long magnitude = stack.popLong();

		if (magnitude < 0) {
			stack.pushLong(value >> abs(magnitude));
		}
		else {
			stack.pushLong(value << magnitude);
		}

		return 0.0f;
	}
}
