#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

namespace mwse {
	class xNot : InstructionInterface_t {
	public:
		xNot();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xNot xNotInstance;

	xNot::xNot() : mwse::InstructionInterface_t(OpCode::xNot) {}

	float xNot::execute(mwse::VMExecuteInterface& virtualMachine) {
		long value = mwse::Stack::getInstance().popLong();
		mwse::Stack::getInstance().pushLong(!value);

		return 0.0f;
	}
}
