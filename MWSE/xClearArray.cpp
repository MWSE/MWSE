#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "ArrayUtil.h"

namespace mwse {
	class xClearArray : InstructionInterface_t {
	public:
		xClearArray();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xClearArray xClearArrayInstance;

	xClearArray::xClearArray() : InstructionInterface_t(OpCode::xClearArray) {}

	float xClearArray::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		if (stack.size() < 1) {
			log::getLog() << "xClearArray: Function called with no arguments." << std::endl;
			stack.pushLong(0);
			return 0.0f;
		}

		long id = stack.popLong();

		long status = Arrays::getInstance().clear("xClearArray", id);

		stack.pushLong(status);

		return 0.0f;
	}
}
