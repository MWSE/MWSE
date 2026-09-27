#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3ItemData.h"
#include "TES3Reference.h"


namespace mwse {
	class xSetCharge : InstructionInterface_t {
	public:
		xSetCharge();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xSetCharge xSetChargeInstance;

	xSetCharge::xSetCharge() : InstructionInterface_t(OpCode::xSetCharge) {}

	float xSetCharge::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get charge from parameter.
		float charge = stack.popFloat();

		// Get reference.
		TES3::Reference* reference = getReference(virtualMachine, __FUNCTION__);
		if (reference == nullptr) {
			stack.pushShort(0);
			return 0.0f;
		}

		// Get the base record.
		TES3::BaseObject* record = reference->baseObject;
		if (record == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xSetCharge: No record found for reference." << std::endl;
			}
			stack.pushShort(0);
			return 0.0f;
		}

		// Get the charge based on the record type. If the item doesn't have a varnode,
		// return the maximum charge from the enchantment record.
		auto varNode = reference->getAttachedItemData();
		if (varNode) {
			varNode->charge = charge;
		}
		else {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xSetCharge: Could not get attached VARNODE." << std::endl;
			}
			stack.pushShort(0);
			return 0.0f;
		}

		stack.pushShort(1);

		return 0.0f;
	}
}
