#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "FileUtil.h"

namespace mwse {
	class xFileRewind : InstructionInterface_t {
	public:
		xFileRewind();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xFileRewind xFileRewindInstance;

	xFileRewind::xFileRewind() : mwse::InstructionInterface_t(OpCode::xFileRewind) {}

	float xFileRewind::execute(mwse::VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		if (stack.size() < 1) {
			mwse::log::getLog() << "xFileRewind: Function called with too few arguments." << std::endl;
			return 0.0f;
		}

		mwseString& fileName = virtualMachine.getString(stack.popLong());

		mwse::FileSystem::getInstance().seek(fileName, 0);

		return 0.0f;
	}
}
