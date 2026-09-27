#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

namespace mwse {
	class xBitOr : InstructionInterface_t {
	public:
		xBitOr();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xBitOr xBitOrInstance;

	xBitOr::xBitOr() : mwse::InstructionInterface_t(OpCode::xBitOr) {}

	float xBitOr::execute(mwse::VMExecuteInterface& virtualMachine) {
		long param1 = mwse::Stack::getInstance().popLong();
		long param2 = mwse::Stack::getInstance().popLong();

		mwse::Stack::getInstance().pushLong(param1 | param2);

		return 0.0f;
	}
}
