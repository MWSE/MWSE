#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

namespace mwse {
	class xAnd : InstructionInterface_t {
	public:
		xAnd();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xAnd xAndInstance;

	xAnd::xAnd() : mwse::InstructionInterface_t(OpCode::xAnd) {}

	float xAnd::execute(mwse::VMExecuteInterface& virtualMachine) {
		long param1 = mwse::Stack::getInstance().popLong();
		long param2 = mwse::Stack::getInstance().popLong();

		mwse::Stack::getInstance().pushLong(param1 && param2);

		return 0.0f;
	}
}
