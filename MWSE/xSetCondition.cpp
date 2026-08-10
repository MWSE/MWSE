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
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xSetCondition xSetConditionInstance;

	xSetCondition::xSetCondition() : mwse::InstructionInterface_t(OpCode::xSetCondition) {}

	float xSetCondition::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get parameter.
		long value = Stack::getInstance().popLong();

		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xSetCondition: No reference provided." << std::endl;
			}
			Stack::getInstance().pushShort(0);
			return 0.0f;
		}

		// Get associated varnode, and the condition from it.
		auto varNode = reference->getAttachedItemData();
		if (varNode != NULL) {
			varNode->condition = value;
		}
		else {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xSetCondition: Could not get attached VARNODE." << std::endl;
			}
			Stack::getInstance().pushShort(0);
			return 0.0f;
		}

		Stack::getInstance().pushShort(1);

		return 0.0f;
	}
}
