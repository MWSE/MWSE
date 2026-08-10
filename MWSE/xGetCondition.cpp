#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3ItemData.h"
#include "TES3Reference.h"

namespace mwse {
	class xGetCondition : InstructionInterface_t {
	public:
		xGetCondition();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xGetCondition xGetConditionInstance;

	xGetCondition::xGetCondition() : mwse::InstructionInterface_t(OpCode::xGetCondition) {}

	float xGetCondition::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetCondition: No reference provided." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		long value = 0;

		// Get associated varnode, and the condition from it.
		auto varNode = reference->getAttachedItemData();
		if (varNode != NULL) {
			value = varNode->condition;
		}
		else {
			value = reference->baseObject->getDurability();
		}

		mwse::Stack::getInstance().pushLong(value);

		return 0.0f;
	}
}
