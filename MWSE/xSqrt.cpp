#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

namespace mwse {
	class xSqrt : InstructionInterface_t {
	public:
		xSqrt();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xSqrt xSqrtInstance;

	xSqrt::xSqrt() : mwse::InstructionInterface_t(OpCode::xSqrt) {}

	float xSqrt::execute(mwse::VMExecuteInterface& virtualMachine) {
		mwse::Stack::getInstance().pushFloat(std::sqrt(mwse::Stack::getInstance().popFloat()));
		return 0.0f;
	}
}
