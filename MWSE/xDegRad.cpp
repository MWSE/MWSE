#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

#include "MathUtil.h"

namespace mwse {
	class xDegRad : InstructionInterface_t {
	public:
		xDegRad();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xDegRad xDegRadInstance;

	xDegRad::xDegRad() : InstructionInterface_t(OpCode::xDegRad) {}

	float xDegRad::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		const auto degrees = stack.popFloat();
		const auto radians = se::math::degreesToRadians(degrees);
		stack.pushFloat(radians);
		return 0.0f;
	}
}
