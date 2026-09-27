#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "ArrayUtil.h"

namespace mwse {
	class xCreateArray : InstructionInterface_t {
	public:
		xCreateArray();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xCreateArray xCreateArrayInstance;

	xCreateArray::xCreateArray() : mwse::InstructionInterface_t(OpCode::xCreateArray) {}

	float xCreateArray::execute(mwse::VMExecuteInterface& virtualMachine) {
		long id = mwse::Arrays::getInstance().create("xCreateArray");

		mwse::Stack::getInstance().pushLong(id);

		return 0.0f;
	}
}
