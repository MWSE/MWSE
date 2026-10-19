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
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xGetCondition xGetConditionInstance;

	xGetCondition::xGetCondition() : InstructionInterface_t(OpCode::xGetCondition) {}

	float xGetCondition::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get reference.
		TES3::Reference* reference = getReference(virtualMachine, __FUNCTION__);
		if (reference == nullptr) {
			stack.pushLong(0);
			return 0.0f;
		}

		long value = 0;

		// Get associated varnode, and the condition from it.
		auto varNode = reference->getAttachedItemData();
		if (varNode != nullptr) {
			value = varNode->condition;
		}
		else {
			value = reference->baseObject->getDurability();
		}

		stack.pushLong(value);

		return 0.0f;
	}
}
