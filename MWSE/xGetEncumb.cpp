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
		for (const auto stack : static_cast<TES3::Actor*>(reference->baseObject)->inventory) {
			totalWeight += stack->object->getWeight() * std::abs(stack->count);

			// Keep track if we've run across leveled content.
			if (!hasLeveledContent && stack->object->objectType == TES3::ObjectType::LeveledItem) {
				hasLeveledContent = true;
			}
		}

		// If we ran into leveled content, make our weight negative.
		if (hasLeveledContent) {
			totalWeight *= -1;
		}

		mwse::Stack::getInstance().pushFloat(totalWeight);

		return 0.0f;
	}
}
