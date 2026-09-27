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

	xStringCompare::xStringCompare() : InstructionInterface_t(OpCode::xStringCompare) {}

	float xStringCompare::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		mwseString& string1 = virtualMachine.getString(stack.popLong());
		mwseString& string2 = virtualMachine.getString(stack.popLong());

		long result = strcmp(string1.c_str(), string2.c_str());

		stack.pushLong(result);

		return 0.0f;
	}
}
