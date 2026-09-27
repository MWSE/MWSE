#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "ArrayUtil.h"

namespace mwse {
	class xGetArraySize : InstructionInterface_t {
	public:
		xGetArraySize();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xGetArraySize xGetArraySizeInstance;

	xGetArraySize::xGetArraySize() : mwse::InstructionInterface_t(OpCode::xGetArraySize) {}

	float xGetArraySize::execute(mwse::VMExecuteInterface& virtualMachine) {
		if (mwse::Stack::getInstance().size() < 1) {
			mwse::log::getLog() << "xGetArraySize: Function called with no arguments." << std::endl;
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		long id = mwse::Stack::getInstance().popLong();

		long size = mwse::Arrays::getInstance().getSize("xGetArraySize", id);

		mwse::Stack::getInstance().pushLong(size);

		return 0.0f;
	}
}
