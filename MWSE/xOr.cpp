#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

namespace mwse {
	class xOr : InstructionInterface_t {
	public:
		xOr();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xOr xOrInstance;

	xOr::xOr() : mwse::InstructionInterface_t(OpCode::xOr) {}

	float xOr::execute(mwse::VMExecuteInterface& virtualMachine) {
		long param1 = mwse::Stack::getInstance().popLong();
		long param2 = mwse::Stack::getInstance().popLong();

		mwse::Stack::getInstance().pushLong(param1 || param2);

		return 0.0f;
	}
}
