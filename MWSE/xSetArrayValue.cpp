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

	xSetArrayValue::xSetArrayValue() : mwse::InstructionInterface_t(OpCode::xSetArrayValue) {}

	float xSetArrayValue::execute(mwse::VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		if (stack.size() < 3) {
			mwse::log::getLog() << "xSetArrayValue: Function called with too few arguments." << std::endl;
			stack.pushLong(0);
			return 0.0f;
		}

		long id = stack.popLong();
		long index = stack.popLong();
		long value = stack.popLong();

		short status = mwse::Arrays::getInstance().setValue("xSetArrayValue", id, index, value);

		stack.pushShort(status);

		return 0.0f;
	}
}
