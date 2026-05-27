#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3Actor.h"
#include "TES3Inventory.h"
#include "TES3Reference.h"

namespace mwse {
	class xGetEncumb : InstructionInterface_t {
	public:
		xGetEncumb();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xGetEncumb xGetEncumbInstance;

	xGetEncumb::xGetEncumb() : mwse::InstructionInterface_t(OpCode::xGetEncumb) {}

	float xGetEncumb::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get reference to target.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetEncumb: No reference provided." << std::endl;
			}
			mwse::Stack::getInstance().pushFloat(0.0f);
			return 0.0f;
		}

		if (!reference->baseObject->isActor()) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetEncumb: Reference is not for an actor." << std::endl;
			}
			mwse::Stack::getInstance().pushFloat(0.0f);
			return 0.0f;
		}

		bool hasLeveledContent = false;
		float totalWeight = 0.0f;

		// Loop through the inventory nodes of the reference, adding weight for each item found.
		NI::IteratedList<TES3::ItemStack*>::Node* inventoryListNode = static_cast<TES3::Actor*>(reference->baseObject)->inventory.itemStacks.head;
		while (inventoryListNode) {
			TES3::ItemStack* inventoryNode = inventoryListNode->data;
			if (inventoryNode) {
				totalWeight += inventoryNode->object->vTable.object->getWeight(inventoryNode->object) * std::abs(inventoryNode->count);

				// Keep track if we've run across leveled content.
				if (!hasLeveledContent && inventoryNode->object->objectType == TES3::ObjectType::LeveledItem) {
					hasLeveledContent = true;
				}
			}

			inventoryListNode = inventoryListNode->next;
		}

		// If we ran into leveled content, make our weight negative.
		if (hasLeveledContent) {
			totalWeight *= -1;
		}

		mwse::Stack::getInstance().pushFloat(totalWeight);

		return 0.0f;
	}
}
