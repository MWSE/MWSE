#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "Log.h"
#include "StringUtil.h"

namespace mwse {
	class xStringLength : InstructionInterface_t {
	public:
		xStringLength();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xStringLength xStringLengthInstance;

	xStringLength::xStringLength() : mwse::InstructionInterface_t(OpCode::xStringLength) {}

	float xStringLength::execute(mwse::VMExecuteInterface& virtualMachine) {
		mwseString& parameter = virtualMachine.getString(Stack::getInstance().popLong());

		long result = parameter.length();

		mwse::Stack::getInstance().pushLong(result);

		return 0.0f;
	}
}
