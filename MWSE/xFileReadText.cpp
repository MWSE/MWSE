#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "FileUtil.h"
#include "StringUtil.h"

namespace mwse {
	class xFileReadText : InstructionInterface_t {
	public:
		xFileReadText();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xFileReadText xFileReadTextInstance;

	xFileReadText::xFileReadText() : mwse::InstructionInterface_t(OpCode::xFileReadText) {}

	float xFileReadText::execute(mwse::VMExecuteInterface& virtualMachine) {
		if (mwse::Stack::getInstance().size() < 2) {
			mwse::log::getLog() << "xFileReadText: Function called with too few arguments." << std::endl;
			return 0.0f;
		}

		mwseString& fileName = virtualMachine.getString(mwse::Stack::getInstance().popLong());
		mwseString& format = virtualMachine.getString(mwse::Stack::getInstance().popLong());

		// Count how many results there should be based on the format string
		bool stopAtEndOfLine = false;
		int maxResults = 0;
		se::string::enumerate(format.c_str(), maxResults, stopAtEndOfLine);
		maxResults++;

		// Store results so we can push them on in reverse.
		long* results = new long[maxResults];

		// Read the string from the file. If we can't read a string back, push 0s.
		std::string readString = mwse::FileSystem::getInstance().readString(fileName.c_str(), stopAtEndOfLine);
		if (readString.empty()) {
			while (maxResults--) {
				mwse::Stack::getInstance().pushLong(0);
			}
			return 0.0f;
		}

		// If we did get a string back, secernate and return.
		se::string::secernate(format.c_str(), readString.c_str(), results, maxResults);
		while (maxResults--) {
			mwse::Stack::getInstance().pushLong(results[maxResults]);
		}

		return 0.0f;
	}
}
