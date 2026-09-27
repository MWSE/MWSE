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

	xHypot::xHypot() : InstructionInterface_t(OpCode::xHypot) {}

	float xHypot::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		float param1 = stack.popFloat();
		float param2 = stack.popFloat();

		stack.pushFloat(std::hypotf(param1, param2));

		return 0.0f;
	}
}
