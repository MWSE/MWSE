#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

#include "MathUtil.h"

namespace mwse {
	class xRadDeg : InstructionInterface_t {
	public:
		xRadDeg();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xRadDeg xRadDegInstance;

	xRadDeg::xRadDeg() : mwse::InstructionInterface_t(OpCode::xRadDeg) {}

	float xRadDeg::execute(mwse::VMExecuteInterface& virtualMachine) {
		const auto radians = mwse::Stack::getInstance().popFloat();
		const auto degrees = se::math::radiansToDegrees(radians);
		mwse::Stack::getInstance().pushFloat(degrees);
		return 0.0f;
	}
}
