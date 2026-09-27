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

	xClearArray::xClearArray() : mwse::InstructionInterface_t(OpCode::xClearArray) {}

	float xClearArray::execute(mwse::VMExecuteInterface& virtualMachine) {
		if (mwse::Stack::getInstance().size() < 1) {
			mwse::log::getLog() << "xClearArray: Function called with no arguments." << std::endl;
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		long id = mwse::Stack::getInstance().popLong();

		long status = mwse::Arrays::getInstance().clear("xClearArray", id);

		mwse::Stack::getInstance().pushLong(status);

		return 0.0f;
	}
}
