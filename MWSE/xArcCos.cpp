#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

namespace mwse {
	class xArcCos : InstructionInterface_t {
	public:
		xArcCos();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xArcCos xArcCosInstance;

	xArcCos::xArcCos() : mwse::InstructionInterface_t(OpCode::xArcCos) {}

	float xArcCos::execute(mwse::VMExecuteInterface& virtualMachine) {
		mwse::Stack::getInstance().pushFloat(std::acos(mwse::Stack::getInstance().popFloat()));
		return 0.0f;
	}
}
