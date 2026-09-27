#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

namespace mwse {
	class xArcSin : InstructionInterface_t {
	public:
		xArcSin();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xArcSin xArcSinInstance;

	xArcSin::xArcSin() : mwse::InstructionInterface_t(OpCode::xArcSin) {}

	float xArcSin::execute(mwse::VMExecuteInterface& virtualMachine) {
		mwse::Stack::getInstance().pushFloat(std::asin(mwse::Stack::getInstance().popFloat()));
		return 0.0f;
	}
}
