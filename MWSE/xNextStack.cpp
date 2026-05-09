#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3Inventory.h"

namespace mwse {
	class xNextStack : InstructionInterface_t {
	public:
		xNextStack();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xNextStack xNextStackInstance;

	xNextStack::xNextStack() : mwse::InstructionInterface_t(OpCode::xNextStack) {}

	float xNextStack::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get the passed node.
		auto node = reinterpret_cast<NI::IteratedList<TES3::ItemStack*>::Node*>(mwse::Stack::getInstance().popLong());
		if (node == NULL) {
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		mwse::Stack::getInstance().pushLong((long)node->next);
		mwse::Stack::getInstance().pushLong(node->data->count);
		mwse::Stack::getInstance().pushString(node->data->object->vTable.object->getObjectID(node->data->object));

		return 0.0f;
	}
}
