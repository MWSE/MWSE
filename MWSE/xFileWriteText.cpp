#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "FileUtil.h"
#include "StringUtil.h"

namespace mwse {
	class xFileWriteText : InstructionInterface_t {
	public:
		xFileWriteText();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xFileWriteText xFileWriteTextInstance;

	xFileWriteText::xFileWriteText() : mwse::InstructionInterface_t(OpCode::xFileWriteText) {}

	float xFileWriteText::execute(mwse::VMExecuteInterface& virtualMachine) {
		if (mwse::Stack::getInstance().size() < 2) {
			mwse::log::getLog() << "xFileWriteText: Function called with too few arguments." << std::endl;
			return 0.0f;
		}

		mwseString& fileName = virtualMachine.getString(mwse::Stack::getInstance().popLong());
		mwseString& format = virtualMachine.getString(mwse::Stack::getInstance().popLong());

		bool suppressNull = false;
		std::string badCodes;
		std::string value = se::string::interpolate(format, virtualMachine, &suppressNull, &badCodes);
		if (!badCodes.empty()) {
			mwse::log::getLog() << "xFileWriteText: bad format \"" << badCodes << "\" in \"" << format << "\" generating \"" << value << "\"" << badCodes << std::endl;
		}

		mwse::FileSystem::getInstance().writeString(fileName.c_str(), value, suppressNull);

		return 0.0f;
	}
}
