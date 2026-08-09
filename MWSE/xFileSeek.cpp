#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "FileUtil.h"

namespace mwse {
	class xFileSeek : InstructionInterface_t {
	public:
		xFileSeek();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xFileSeek xFileSeekInstance;

	xFileSeek::xFileSeek() : mwse::InstructionInterface_t(OpCode::xFileSeek) {}

	float xFileSeek::execute(mwse::VMExecuteInterface& virtualMachine) {
		if (mwse::Stack::getInstance().size() < 2) {
			mwse::log::getLog() << "xFileSeek: Function called with too few arguments." << std::endl;
			return 0.0f;
		}

		mwseString& fileName = virtualMachine.getString(mwse::Stack::getInstance().popLong());
		long position = mwse::Stack::getInstance().popLong();

		mwse::FileSystem::getInstance().seek(fileName, position);

		return 0.0f;
	}
}
