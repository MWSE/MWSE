#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "FileUtil.h"

namespace mwse {
	class xFileWriteString : InstructionInterface_t {
	public:
		xFileWriteString();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xFileWriteString xFileWriteStringInstance;

	xFileWriteString::xFileWriteString() : mwse::InstructionInterface_t(OpCode::xFileWriteString) {}

	float xFileWriteString::execute(mwse::VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		if (stack.size() < 2) {
			mwse::log::getLog() << "xFileWriteString: Function called with too few arguments." << std::endl;
			return 0.0f;
		}

		mwseString& fileName = virtualMachine.getString(stack.popLong());
		mwseString& value = virtualMachine.getString(stack.popLong());

		mwse::FileSystem::getInstance().writeString(fileName, value);

		return 0.0f;
	}
}
