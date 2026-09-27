#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3Actor.h"
#include "TES3Inventory.h"
#include "TES3Reference.h"
namespace mwse {
	class xInventory : InstructionInterface_t {
	public:
		xInventory();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xInventory xInventoryInstance;

	xInventory::xInventory() : mwse::InstructionInterface_t(OpCode::xInventory) {}

	float xInventory::execute(mwse::VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xInventory: Invalid reference attachment." << std::endl;
			}
			stack.pushLong(0);
			stack.pushLong(0);
			stack.pushLong(0);
			return 0.0f;
		}

		if (!reference->baseObject->isActor()) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xInventory: Reference is not for an actor." << std::endl;
			}
			stack.pushFloat(0.0f);
			return 0.0f;
		}

		NI::IteratedList<TES3::ItemStack*>::Node* firstItem = static_cast<TES3::Actor*>(reference->baseObject)->inventory.itemStacks.head;
		if (firstItem == nullptr) {
			stack.pushLong(0);
			stack.pushLong(0);
			stack.pushLong(0);
			return 0.0f;
		}

		stack.pushLong((long)firstItem->next);
		stack.pushLong(firstItem->data->count);
		stack.pushString(firstItem->data->object->getObjectID());

		return 0.0f;
	}
}
