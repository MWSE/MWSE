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
		float param = mwse::Stack::getInstance().popFloat();

		mwse::Stack::getInstance().pushFloat(static_cast<float>(param * math::M_PI / 180.0));
		return 0.0f;
	}
}
