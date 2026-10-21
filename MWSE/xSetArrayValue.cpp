#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "ArrayUtil.h"

namespace mwse {
	class xSetArrayValue : InstructionInterface_t {
	public:
		xSetArrayValue();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xSetArrayValue xSetArrayValueInstance;

	xSetArrayValue::xSetArrayValue() : InstructionInterface_t(OpCode::xSetArrayValue) {}

	float xSetArrayValue::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		if (stack.size() < 3) {
			log::getLog() << "xSetArrayValue: Function called with too few arguments." << std::endl;
			stack.pushLong(0);
			return 0.0f;
		}

		long id = stack.popLong();
		long index = stack.popLong();
		long value = stack.popLong();

		if (index < 0) {
			log::getLog() << "xSetArrayValue: Array index out of bounds. id: " << id << " index: " << index << std::endl;
			stack.pushShort(0);
			return 0.0f;
		}
		short status = Arrays::getInstance().setValue("xSetArrayValue", id, index, value);

		stack.pushShort(status);

		return 0.0f;
	}
}
