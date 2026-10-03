#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

namespace mwse {
	class xBitNot : InstructionInterface_t {
	public:
		xBitNot();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xBitNot xBitNotInstance;

	xBitNot::xBitNot() : InstructionInterface_t(OpCode::xBitNot) {}

	float xBitNot::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		long param = stack.popLong();

		stack.pushLong(~param);

		return 0.0f;
	}
}
