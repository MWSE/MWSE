#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

namespace mwse {
	class xGetPCTarget : InstructionInterface_t
	{
	public:
		xGetPCTarget();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xGetPCTarget xGetPCTargetInstance;

	xGetPCTarget::xGetPCTarget() : InstructionInterface_t(OpCode::xGetPCTarget) {}

	float xGetPCTarget::execute(VMExecuteInterface& virtualMachine) {
		//get the current target
		TES3::Reference* target = virtualMachine.getCurrentTarget();

		//push the Reference on the stack.
		Stack::getInstance().pushPointer(target);

		return 0.0f;
	}
}
