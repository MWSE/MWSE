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
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xStringParse xStringParseInstance;

	xStringParse::xStringParse() : mwse::InstructionInterface_t(OpCode::xStringParse) {}

	float xStringParse::execute(mwse::VMExecuteInterface& virtualMachine) {
		mwseString& format = virtualMachine.getString(Stack::getInstance().popLong());

		// We have to hijack this function for version checking, to make it backwards-compatible.
		if (format == "MWSE_VERSION") {
			long checkVersionAgainst = mwse::Stack::getInstance().popLong();
			mwse::Stack::getInstance().pushLong(MWSE_VERSION_INTEGER >= checkVersionAgainst);
			mwse::Stack::getInstance().pushLong(MWSE_VERSION_INTEGER);
			return 0.0f;
		}

		// If we're not doing an actual version check, we'll want the string.
		mwseString& string = virtualMachine.getString(Stack::getInstance().popLong());

		int resultCount = 0;
		bool eolMode = false;
		se::string::enumerate(format.c_str(), resultCount, eolMode);
		resultCount++;

		long* results = new long[resultCount];
		se::string::secernate(format.c_str(), string.c_str(), results, resultCount);

		while (resultCount--) {
			mwse::Stack::getInstance().pushLong(results[resultCount]);
		}

		delete[] results;

		return 0.0f;
	}
}
