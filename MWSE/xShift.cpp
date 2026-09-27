#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

namespace mwse {
	class xShift : InstructionInterface_t {
	public:
		xShift();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xShift xShiftInstance;

	xShift::xShift() : mwse::InstructionInterface_t(OpCode::xShift) {}

	float xShift::execute(mwse::VMExecuteInterface& virtualMachine) {
		long value = mwse::Stack::getInstance().popLong();
		long magnitude = mwse::Stack::getInstance().popLong();

		if (magnitude < 0) {
			mwse::Stack::getInstance().pushLong(value >> abs(magnitude));
		}
		else {
			mwse::Stack::getInstance().pushLong(value << magnitude);
		}

		return 0.0f;
	}
}
