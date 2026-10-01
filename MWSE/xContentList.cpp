#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3Actor.h"
#include "TES3Inventory.h"
#include "TES3Reference.h"


namespace mwse {
	class xContentList : InstructionInterface_t {
	public:
		xContentList();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xContentList xContentListInstance;

	xContentList::xContentList() : InstructionInterface_t(OpCode::xContentList) {}

	float xContentList::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get parameters.
		NI::IteratedList<TES3::ItemStack*>::Node* node = reinterpret_cast<NI::IteratedList<TES3::ItemStack*>::Node*>(stack.popLong());

		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xContentList: Called on invalid reference." << std::endl;
			}
			stack.pushLong(0);
			stack.pushLong(0);
			stack.pushFloat(0.0f);
			stack.pushLong(0);
			stack.pushLong(0);
			stack.pushLong(0);
			stack.pushLong(0);
			return 0.0f;
		}

		// Results.
		const char* id = nullptr;
		long count = 0;
		long type = 0;
		long value = 0;
		float weight = 0;
		const char* name = nullptr;
		NI::IteratedList<TES3::ItemStack*>::Node* next = nullptr;

		// If we aren't given a node, get the first one.
		if (node == nullptr && reference->baseObject->isActor()) {
			node = static_cast<TES3::Actor*>(reference->baseObject)->inventory.itemStacks.head;
		}

		// Validate the node we've obtained.
		if (node && node->data && node->data->object) {
			TES3::Object* object = node->data->object;

			id = object->getObjectID();
			count = node->data->count;
			type = object->objectType;
			value = object->getValue();
			weight = object->getWeight();
			name = object->getName();

			next = node->next;
		}

		// Push values to the stack.
		stack.pushLong((long)next);
		stack.pushString(name);
		stack.pushFloat(weight);
		stack.pushLong(value);
		stack.pushLong(type);
		stack.pushLong(count);
		stack.pushString(id);

		return 0.0f;
	}
}
