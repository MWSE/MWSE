#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

namespace mwse {
	class xBitXor : InstructionInterface_t {
	public:
		xBitXor();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xBitXor xBitXorInstance;

	xBitXor::xBitXor() : mwse::InstructionInterface_t(OpCode::xBitXor) {}

	float xBitXor::execute(mwse::VMExecuteInterface& virtualMachine) {
		long param1 = mwse::Stack::getInstance().popLong();
		long param2 = mwse::Stack::getInstance().popLong();

		mwse::Stack::getInstance().pushLong(param1 ^ param2);

		return 0.0f;
	}
}
