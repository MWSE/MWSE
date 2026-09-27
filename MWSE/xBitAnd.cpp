#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

namespace mwse {
	class xBitAnd : InstructionInterface_t {
	public:
		xBitAnd();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xBitAnd xBitAndInstance;

	xBitAnd::xBitAnd() : mwse::InstructionInterface_t(OpCode::xBitAnd) {}

	float xBitAnd::execute(mwse::VMExecuteInterface& virtualMachine) {
		long param1 = mwse::Stack::getInstance().popLong();
		long param2 = mwse::Stack::getInstance().popLong();

		mwse::Stack::getInstance().pushLong(param1 & param2);

		return 0.0f;
	}
}
