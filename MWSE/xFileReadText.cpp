#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "FileUtil.h"
#include "StringUtil.h"

namespace mwse {
	class xFileReadText : InstructionInterface_t {
	public:
		xFileReadText();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xFileReadText xFileReadTextInstance;

	xFileReadText::xFileReadText() : InstructionInterface_t(OpCode::xFileReadText) {}

	float xFileReadText::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		if (stack.size() < 2) {
			log::getLog() << "xFileReadText: Function called with too few arguments." << std::endl;
			return 0.0f;
		}

		mwseString& fileName = virtualMachine.getString(stack.popLong());
		mwseString& format = virtualMachine.getString(stack.popLong());

		// Count how many results there should be based on the format string
		bool stopAtEndOfLine = false;
		int maxResults = 0;
		se::string::enumerate(format.c_str(), maxResults, stopAtEndOfLine);
		maxResults++;

		// Store results so we can push them on in reverse.
		std::vector<long> results(maxResults);

		// Read the string from the file. If we can't read a string back, push 0s.
		std::string readString = FileSystem::getInstance().readString(fileName, stopAtEndOfLine);
		if (readString.empty()) {
			while (maxResults--) {
				stack.pushLong(0);
			}
			return 0.0f;
		}

		// If we did get a string back, secernate and return.
		se::string::secernate(format.c_str(), readString.c_str(), results.data(), maxResults);
		while (maxResults--) {
			stack.pushLong(results[maxResults]);
		}

		return 0.0f;
	}
}
