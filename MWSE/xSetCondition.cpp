#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3ItemData.h"
#include "TES3Reference.h"

namespace mwse {
	class xSetCondition : InstructionInterface_t {
	public:
		xSetCondition();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xSetCondition xSetConditionInstance;

	xSetCondition::xSetCondition() : InstructionInterface_t(OpCode::xSetCondition) {}

	float xSetCondition::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get parameter.
		long value = stack.popLong();

		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xSetCondition: No reference provided." << std::endl;
			}
			stack.pushShort(0);
			return 0.0f;
		}

		// Get associated varnode, and the condition from it.
		auto varNode = reference->getAttachedItemData();
		if (varNode != nullptr) {
			varNode->condition = value;
		}
		else {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xSetCondition: Could not get attached VARNODE." << std::endl;
			}
			stack.pushShort(0);
			return 0.0f;
		}

		stack.pushShort(1);

		return 0.0f;
	}
}
