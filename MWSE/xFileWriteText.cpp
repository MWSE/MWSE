#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "FileUtil.h"
#include "StringUtil.h"

namespace mwse {
	class xFileWriteText : InstructionInterface_t {
	public:
		xFileWriteText();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xFileWriteText xFileWriteTextInstance;

	xFileWriteText::xFileWriteText() : InstructionInterface_t(OpCode::xFileWriteText) {}

	float xFileWriteText::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		if (stack.size() < 2) {
			log::getLog() << "xFileWriteText: Function called with too few arguments." << std::endl;
			return 0.0f;
		}

		mwseString& fileName = virtualMachine.getString(stack.popLong());
		mwseString& format = virtualMachine.getString(stack.popLong());

		bool suppressNull = false;
		std::string badCodes;
		std::string value = se::string::interpolate(format, virtualMachine, &suppressNull, &badCodes);
		if (!badCodes.empty()) {
			log::getLog() << "xFileWriteText: bad format \"" << badCodes << "\" in \"" << format << "\" generating \"" << value << "\"" << badCodes << std::endl;
		}

		FileSystem::getInstance().writeString(fileName, value, suppressNull);

		return 0.0f;
	}
}
