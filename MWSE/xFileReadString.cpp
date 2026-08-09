#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "FileUtil.h"

namespace mwse {
	class xFileReadString : InstructionInterface_t {
	public:
		xFileReadString();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xFileReadString xFileReadStringInstance;

	xFileReadString::xFileReadString() : mwse::InstructionInterface_t(OpCode::xFileReadString) {}

	float xFileReadString::execute(mwse::VMExecuteInterface& virtualMachine) {
		if (mwse::Stack::getInstance().size() < 1) {
			mwse::log::getLog() << "xFileReadString: Function called with too few arguments." << std::endl;
			return 0.0f;
		}

		// Get filename as an argument.
		mwseString& fileName = virtualMachine.getString(mwse::Stack::getInstance().popLong());

		// Read the string from the file.
		std::string readString = mwse::FileSystem::getInstance().readString(fileName, true);

		// Push the found string to the stack.
		if (!readString.empty()) {
			mwse::Stack::getInstance().pushString(readString);
		}
		else {
			// If we didn't read a string, "null" is expected.
			mwse::Stack::getInstance().pushString("null");
		}

		return 0.0f;
	}
}
