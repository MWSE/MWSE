#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

namespace mwse {
	class xLongToFloats : InstructionInterface_t {
	public:
		xLongToFloats();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xLongToFloats xLongToFloatsInstance;

	xLongToFloats::xLongToFloats() : InstructionInterface_t(OpCode::xLongToFloats) {}

	float xLongToFloats::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		long param = stack.popLong();

		stack.pushFloat(static_cast<float>((param >> 16) + 0x10000));
		stack.pushFloat(static_cast<float>(param & 0xFFFF));

		return 0.0f;
	}
}
