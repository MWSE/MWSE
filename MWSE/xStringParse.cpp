#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "Log.h"
#include "StringUtil.h"
#include "MWSEDefs.h"
namespace mwse {
	class xStringParse : InstructionInterface_t {
	public:
		xStringParse();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xStringParse xStringParseInstance;

	xStringParse::xStringParse() : InstructionInterface_t(OpCode::xStringParse) {}

	float xStringParse::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		mwseString& format = virtualMachine.getString(stack.popLong());

		// We have to hijack this function for version checking, to make it backwards-compatible.
		if (format == "MWSE_VERSION") {
			long checkVersionAgainst = stack.popLong();
			stack.pushLong(MWSE_VERSION_INTEGER >= checkVersionAgainst);
			stack.pushLong(MWSE_VERSION_INTEGER);
			return 0.0f;
		}

		// If we're not doing an actual version check, we'll want the string.
		mwseString& string = virtualMachine.getString(stack.popLong());

		int resultCount = 0;
		bool eolMode = false;
		se::string::enumerate(format.c_str(), resultCount, eolMode);
		resultCount++;

		std::vector<long> results(resultCount);
		se::string::secernate(format.c_str(), string.c_str(), results.data(), resultCount);

		while (resultCount--) {
			stack.pushLong(results[resultCount]);
		}

		return 0.0f;
	}
}
