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

	xGetArrayValue::xGetArrayValue() : mwse::InstructionInterface_t(OpCode::xGetArrayValue) {}

	float xGetArrayValue::execute(mwse::VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		if (stack.size() < 2) {
			mwse::log::getLog() << "xGetArrayValue: Function requires 2 arguments." << std::endl;
			stack.pushLong(0);
			return 0.0f;
		}

		long id = stack.popLong();
		long index = stack.popLong();

		long value = mwse::Arrays::getInstance().getValue("xGetArrayValue", id, index);

		stack.pushLong(value);

		return 0.0f;
	}
}
