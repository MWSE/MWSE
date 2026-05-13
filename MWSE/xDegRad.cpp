#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

#include "MathUtil.h"

namespace mwse {
	class xDegRad : InstructionInterface_t {
	public:
		xDegRad();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xDegRad xDegRadInstance;

	xDegRad::xDegRad() : mwse::InstructionInterface_t(OpCode::xDegRad) {}

	float xDegRad::execute(mwse::VMExecuteInterface& virtualMachine) {
		const auto degrees = mwse::Stack::getInstance().popFloat();
		const auto radians = se::math::degreesToRadians(degrees);
		mwse::Stack::getInstance().pushFloat(radians);
		return 0.0f;
	}
}
