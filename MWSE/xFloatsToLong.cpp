#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

namespace mwse {
	class xFloatsToLong : InstructionInterface_t {
	public:
		xFloatsToLong();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xFloatsToLong xFloatsToLongInstance;

	xFloatsToLong::xFloatsToLong() : mwse::InstructionInterface_t(OpCode::xFloatsToLong) {}

	float xFloatsToLong::execute(mwse::VMExecuteInterface& virtualMachine) {
		long param1 = static_cast<long>(mwse::Stack::getInstance().popFloat());
		long param2 = static_cast<long>(mwse::Stack::getInstance().popFloat());

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

		mwse::Stack::getInstance().pushLong(((high - 0x10000) << 16) + low);

		return 0.0f;
	}
}
