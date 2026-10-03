#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "Log.h"
#include "StringUtil.h"

namespace mwse {
	class xStringMatch : InstructionInterface_t {
	public:
		xStringMatch();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xStringMatch xStringMatchInstance;

	xStringMatch::xStringMatch() : InstructionInterface_t(OpCode::xStringMatch) {}

	float xStringMatch::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		mwseString& string = virtualMachine.getString(stack.popLong());
		mwseString& pattern = virtualMachine.getString(stack.popLong());

		bool result = false;
		try {
			result = boost::regex_search(string, boost::regex(pattern));
		}
		catch (boost::regex_error&) {
			result = false;
		}

		stack.pushLong(result);

		return 0.0f;
	}
}
