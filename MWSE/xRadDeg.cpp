#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

#include "MathUtil.h"

namespace mwse {
	class xRadDeg : InstructionInterface_t {
	public:
		xRadDeg();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xRadDeg xRadDegInstance;

	xRadDeg::xRadDeg() : InstructionInterface_t(OpCode::xRadDeg) {}

	float xRadDeg::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		const auto radians = stack.popFloat();
		const auto degrees = se::math::radiansToDegrees(radians);
		stack.pushFloat(degrees);
		return 0.0f;
	}
}
