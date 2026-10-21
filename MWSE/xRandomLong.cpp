#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "RngUtil.h"

namespace mwse {
	class xRandomLong : InstructionInterface_t {
	public:
		xRandomLong();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xRandomLong xRandomLongInstance;

	xRandomLong::xRandomLong() : InstructionInterface_t(OpCode::xRandomLong) {}

	float xRandomLong::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		long min = stack.popLong();
		long max = stack.popLong();

		stack.pushLong(rng::getRandomLong(min, max));

		return 0.0f;
	}
}
