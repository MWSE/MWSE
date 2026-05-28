#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

namespace mwse {
	class xLongToFloats : InstructionInterface_t {
	public:
		xLongToFloats();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xLongToFloats xLongToFloatsInstance;

	xLongToFloats::xLongToFloats() : mwse::InstructionInterface_t(OpCode::xLongToFloats) {}

	float xLongToFloats::execute(mwse::VMExecuteInterface& virtualMachine) {
		long param = mwse::Stack::getInstance().popLong();

		mwse::Stack::getInstance().pushFloat(static_cast<float>((param >> 16) + 0x10000));
		mwse::Stack::getInstance().pushFloat(static_cast<float>(param & 0xFFFF));

		return 0.0f;
	}
}
