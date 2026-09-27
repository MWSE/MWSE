#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3Inventory.h"

namespace mwse {
	class xNextStack : InstructionInterface_t {
	public:
		xNextStack();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xNextStack xNextStackInstance;

	xNextStack::xNextStack() : mwse::InstructionInterface_t(OpCode::xNextStack) {}

	float xNextStack::execute(mwse::VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get the passed node.
		auto node = reinterpret_cast<NI::IteratedList<TES3::ItemStack*>::Node*>(stack.popLong());
		if (node == nullptr) {
			stack.pushLong(0);
			stack.pushLong(0);
			stack.pushLong(0);
			return 0.0f;
		}

		stack.pushLong((long)node->next);
		stack.pushLong(node->data->count);
		stack.pushString(node->data->object->getObjectID());

		return 0.0f;
	}
}
