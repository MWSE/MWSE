#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "RngUtil.h"

namespace mwse {
	class xRandomFloat : InstructionInterface_t {
	public:
		xRandomFloat();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xRandomFloat xRandomFloatInstance;

	xRandomFloat::xRandomFloat() : mwse::InstructionInterface_t(OpCode::xRandomFloat) {}

	float xRandomFloat::execute(mwse::VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		float min = stack.popFloat();
		float max = stack.popFloat();

		stack.pushFloat(rng::getRandomFloat(min, max));

		return 0.0f;
	}
}
