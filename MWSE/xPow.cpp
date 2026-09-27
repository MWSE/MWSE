#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

namespace mwse {
	class xPow : InstructionInterface_t {
	public:
		xPow();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xPow xPowInstance;

	xPow::xPow() : mwse::InstructionInterface_t(OpCode::xPow) {}

	float xPow::execute(mwse::VMExecuteInterface& virtualMachine) {
		float base = mwse::Stack::getInstance().popFloat();
		float exponent = mwse::Stack::getInstance().popFloat();

		mwse::Stack::getInstance().pushFloat(std::powf(base, exponent));

		return 0.0f;
	}
}
