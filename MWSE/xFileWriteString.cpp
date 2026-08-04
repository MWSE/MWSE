#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "FileUtil.h"

namespace mwse {
	class xFileWriteString : InstructionInterface_t {
	public:
		xFileWriteString();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xFileWriteString xFileWriteStringInstance;

	xFileWriteString::xFileWriteString() : mwse::InstructionInterface_t(OpCode::xFileWriteString) {}

	float xFileWriteString::execute(mwse::VMExecuteInterface& virtualMachine) {
		if (mwse::Stack::getInstance().size() < 2) {
			mwse::log::getLog() << "xFileWriteString: Function called with too few arguments." << std::endl;
			return 0.0f;
		}

		mwseString& fileName = virtualMachine.getString(mwse::Stack::getInstance().popLong());
		mwseString& value = virtualMachine.getString(mwse::Stack::getInstance().popLong());

		mwse::FileSystem::getInstance().writeString(fileName, value);

		return 0.0f;
	}
}
