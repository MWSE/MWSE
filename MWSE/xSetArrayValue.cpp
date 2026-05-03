#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "ArrayUtil.h"

namespace mwse {
	class xSetArrayValue : InstructionInterface_t {
	public:
		xSetArrayValue();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xSetArrayValue xSetArrayValueInstance;

	xSetArrayValue::xSetArrayValue() : mwse::InstructionInterface_t(OpCode::xSetArrayValue) {}

	float xSetArrayValue::execute(mwse::VMExecuteInterface& virtualMachine) {
		if (mwse::Stack::getInstance().size() < 3) {
			mwse::log::getLog() << "xSetArrayValue: Function called with too few arguments." << std::endl;
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		long id = mwse::Stack::getInstance().popLong();
		long index = mwse::Stack::getInstance().popLong();
		long value = mwse::Stack::getInstance().popLong();

		short status = mwse::Arrays::getInstance().setValue("xSetArrayValue", id, index, value);

		mwse::Stack::getInstance().pushShort(status);

		return 0.0f;
	}
}
