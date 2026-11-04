#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "ArrayUtil.h"

namespace mwse {
	class xGetArrayValue : InstructionInterface_t {
	public:
		xGetArrayValue();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xGetArrayValue xGetArrayValueInstance;

	xGetArrayValue::xGetArrayValue() : InstructionInterface_t(OpCode::xGetArrayValue) {}

	float xGetArrayValue::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		if (stack.size() < 2) {
			log::getLog() << "xGetArrayValue: Function requires 2 arguments." << std::endl;
			stack.pushLong(0);
			return 0.0f;
		}

		long id = stack.popLong();
		long index = stack.popLong();

		if (index < 0) {
			log::getLog() << "xGetArrayValue: Array index out of bounds. id: " << id << " index: " << index << std::endl;
			stack.pushShort(0);
			return 0.0f;
		}
		long value = Arrays::getInstance().getValue("xGetArrayValue", id, index);

		stack.pushLong(value);

		return 0.0f;
	}
}
