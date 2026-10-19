#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "FileUtil.h"

namespace mwse {
	class xFileWriteShort : InstructionInterface_t {
	public:
		xFileWriteShort();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xFileWriteShort xFileWriteShortInstance;

	xFileWriteShort::xFileWriteShort() : InstructionInterface_t(OpCode::xFileWriteShort) {}

	float xFileWriteShort::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		if (stack.size() < 2) {
			log::getLog() << "xFileWriteShort: Function called with too few arguments." << std::endl;
			return 0.0f;
		}

		mwseString& fileName = virtualMachine.getString(stack.popLong());
		short value = stack.popShort();

		FileSystem::getInstance().writeShort(fileName, value);

		return 0.0f;
	}
}
