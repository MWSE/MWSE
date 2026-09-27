#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

namespace mwse {
	class xCos : InstructionInterface_t {
	public:
		xCos();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xCos xCosInstance;

	xCos::xCos() : mwse::InstructionInterface_t(OpCode::xCos) {}

	float xCos::execute(mwse::VMExecuteInterface& virtualMachine) {
		mwse::Stack::getInstance().pushFloat(std::cos(mwse::Stack::getInstance().popFloat()));
		return 0.0f;
	}
}
