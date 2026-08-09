#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "FileUtil.h"

namespace mwse {
	class xFileWriteShort : InstructionInterface_t {
	public:
		xFileWriteShort();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xFileWriteShort xFileWriteShortInstance;

	xFileWriteShort::xFileWriteShort() : mwse::InstructionInterface_t(OpCode::xFileWriteShort) {}

	float xFileWriteShort::execute(mwse::VMExecuteInterface& virtualMachine) {
		if (mwse::Stack::getInstance().size() < 2) {
			mwse::log::getLog() << "xFileWriteShort: Function called with too few arguments." << std::endl;
			return 0.0f;
		}

		mwseString& fileName = virtualMachine.getString(mwse::Stack::getInstance().popLong());
		short value = mwse::Stack::getInstance().popShort();

		mwse::FileSystem::getInstance().writeShort(fileName, value);

		return 0.0f;
	}
}
