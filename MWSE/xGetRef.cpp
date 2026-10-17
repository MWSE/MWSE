#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

namespace mwse {
	class xGetRef : InstructionInterface_t
	{
	public:
		xGetRef();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xGetRef xGetRefInstance;

	xGetRef::xGetRef() : InstructionInterface_t(OpCode::xGetRef) {}

	float xGetRef::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get the parameter.
		mwseString& id = virtualMachine.getString(stack.popLong());

		// Get its reference.
		TES3::Reference* ref = virtualMachine.getReference(id.c_str());

		// Push back as long.
		stack.pushLong((long)ref);

		return 0.0f;
	}
}
