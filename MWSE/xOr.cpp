#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

namespace mwse {
	class xOr : InstructionInterface_t {
	public:
		xOr();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xOr xOrInstance;

	xOr::xOr() : InstructionInterface_t(OpCode::xOr) {}

	float xOr::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		long param1 = stack.popLong();
		long param2 = stack.popLong();

		stack.pushLong(param1 || param2);

		return 0.0f;
	}
}
