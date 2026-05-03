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
		mwse::Stack::getInstance().pushFloat(static_cast<float>(mwse::Stack::getInstance().popFloat() / math::M_PI * 180.0));
		return 0.0f;
	}
}
