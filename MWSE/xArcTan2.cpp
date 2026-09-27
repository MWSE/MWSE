#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

namespace mwse {
	class xArcTan2 : InstructionInterface_t {
	public:
		xArcTan2();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xArcTan2 xArcTan2Instance;

	xArcTan2::xArcTan2() : mwse::InstructionInterface_t(OpCode::xArcTan2) {}

	float xArcTan2::execute(mwse::VMExecuteInterface& virtualMachine) {
		float param1 = mwse::Stack::getInstance().popFloat();
		float param2 = mwse::Stack::getInstance().popFloat();

		mwse::Stack::getInstance().pushFloat(std::atan2(param1, param2));

		return 0.0f;
	}
}
