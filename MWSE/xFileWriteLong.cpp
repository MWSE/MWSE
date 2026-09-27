#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "FileUtil.h"

namespace mwse {
	class xFileWriteLong : InstructionInterface_t {
	public:
		xFileWriteLong();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xFileWriteLong xFileWriteLongInstance;

	xFileWriteLong::xFileWriteLong() : mwse::InstructionInterface_t(OpCode::xFileWriteLong) {}

	float xFileWriteLong::execute(mwse::VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		if (stack.size() < 2) {
			mwse::log::getLog() << "xFileWriteLong: Function called with too few arguments." << std::endl;
			return 0.0f;
		}

		mwseString& fileName = virtualMachine.getString(stack.popLong());
		long value = stack.popLong();

		mwse::FileSystem::getInstance().writeLong(fileName, value);

		return 0.0f;
	}
}
