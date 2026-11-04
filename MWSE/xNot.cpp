#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

namespace mwse {
	class xNot : InstructionInterface_t {
	public:
		xNot();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xNot xNotInstance;

	xNot::xNot() : InstructionInterface_t(OpCode::xNot) {}

	float xNot::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		long value = stack.popLong();
		stack.pushLong(!value);

		return 0.0f;
	}
}
