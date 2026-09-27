#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "Log.h"
#include "StringUtil.h"

namespace mwse {
	class xStringCompare : InstructionInterface_t {
	public:
		xStringCompare();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xStringCompare xStringCompareInstance;

	xStringCompare::xStringCompare() : mwse::InstructionInterface_t(OpCode::xStringCompare) {}

	float xStringCompare::execute(mwse::VMExecuteInterface& virtualMachine) {
		mwseString& string1 = virtualMachine.getString(Stack::getInstance().popLong());
		mwseString& string2 = virtualMachine.getString(Stack::getInstance().popLong());

		long result = strcmp(string1.c_str(), string2.c_str());

		mwse::Stack::getInstance().pushLong(result);

		return 0.0f;
	}
}
