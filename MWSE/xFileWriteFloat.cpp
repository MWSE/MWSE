#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "FileUtil.h"

namespace mwse {
	class xFileWriteFloat : InstructionInterface_t {
	public:
		xFileWriteFloat();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xFileWriteFloat xFileWriteFloatInstance;

	xFileWriteFloat::xFileWriteFloat() : mwse::InstructionInterface_t(OpCode::xFileWriteFloat) {}

	float xFileWriteFloat::execute(mwse::VMExecuteInterface& virtualMachine) {
		if (mwse::Stack::getInstance().size() < 2) {
			mwse::log::getLog() << "xFileWriteFloat: Function called with too few arguments." << std::endl;
			return 0.0f;
		}

		mwseString& fileName = virtualMachine.getString(mwse::Stack::getInstance().popLong());
		float value = mwse::Stack::getInstance().popFloat();

		mwse::FileSystem::getInstance().writeFloat(fileName, value);

		return 0.0f;
	}
}
