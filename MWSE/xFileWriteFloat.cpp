#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "FileUtil.h"

namespace mwse {
	class xFileWriteFloat : InstructionInterface_t {
	public:
		xFileWriteFloat();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xFileWriteFloat xFileWriteFloatInstance;

	xFileWriteFloat::xFileWriteFloat() : mwse::InstructionInterface_t(OpCode::xFileWriteFloat) {}

	float xFileWriteFloat::execute(mwse::VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		if (stack.size() < 2) {
			mwse::log::getLog() << "xFileWriteFloat: Function called with too few arguments." << std::endl;
			return 0.0f;
		}

		mwseString& fileName = virtualMachine.getString(stack.popLong());
		float value = stack.popFloat();

		mwse::FileSystem::getInstance().writeFloat(fileName, value);

		return 0.0f;
	}
}
