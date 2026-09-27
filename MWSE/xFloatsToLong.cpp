#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

namespace mwse {
	class xFloatsToLong : InstructionInterface_t {
	public:
		xFloatsToLong();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xFloatsToLong xFloatsToLongInstance;

	xFloatsToLong::xFloatsToLong() : InstructionInterface_t(OpCode::xFloatsToLong) {}

	float xFloatsToLong::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		long param1 = static_cast<long>(stack.popFloat());
		long param2 = static_cast<long>(stack.popFloat());

		long high = 0;
		long low = 0;
		if (param1 >= 0x10000) {
			high = param1;
			low = param2;
		}
		else {
			high = param2;
			low = param1;
		}

		stack.pushLong(((high - 0x10000) << 16) + low);

		return 0.0f;
	}
}
