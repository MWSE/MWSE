#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

namespace mwse {
	class xHypot : InstructionInterface_t {
	public:
		xHypot();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xHypot xHypotInstance;

	xHypot::xHypot() : mwse::InstructionInterface_t(OpCode::xHypot) {}

	float xHypot::execute(mwse::VMExecuteInterface& virtualMachine) {
		float param1 = mwse::Stack::getInstance().popFloat();
		float param2 = mwse::Stack::getInstance().popFloat();

		mwse::Stack::getInstance().pushFloat(std::hypotf(param1, param2));

		return 0.0f;
	}
}
