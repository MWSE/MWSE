#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "FileUtil.h"

namespace mwse {
	class xFileSeek : InstructionInterface_t {
	public:
		xFileSeek();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xFileSeek xFileSeekInstance;

	xFileSeek::xFileSeek() : InstructionInterface_t(OpCode::xFileSeek) {}

	float xFileSeek::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		if (stack.size() < 2) {
			log::getLog() << "xFileSeek: Function called with too few arguments." << std::endl;
			return 0.0f;
		}

		mwseString& fileName = virtualMachine.getString(stack.popLong());
		long position = stack.popLong();

		FileSystem::getInstance().seek(fileName, position);

		return 0.0f;
	}
}
