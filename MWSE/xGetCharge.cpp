#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3ItemData.h"
#include "TES3Reference.h"
#include "TES3Enchantment.h"

namespace mwse {
	class xGetCharge : InstructionInterface_t {
	public:
		xGetCharge();
		float execute(VMExecuteInterface& virtualMachine) override;
	private:
		const float INVALID_VALUE = -1.0f;
	};

	static xGetCharge xGetChargeInstance;

	xGetCharge::xGetCharge() : InstructionInterface_t(OpCode::xGetCharge) {}

	float xGetCharge::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		float charge = INVALID_VALUE;

		// Get reference.
		TES3::Reference* reference = getReference(virtualMachine, __FUNCTION__);
		if (reference == nullptr) {
			stack.pushFloat(INVALID_VALUE);
			return 0.0f;
		}

		// Get the base record.
		TES3::Object* object = reference->baseObject;
		if (object == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xGetCharge: No record found for reference." << std::endl;
			}
			stack.pushFloat(INVALID_VALUE);
			return 0.0f;
		}

		// Get the charge based on the record type. If the item doesn't have a varnode,
		// return the maximum charge from the enchantment record.
		auto varNode = reference->getAttachedItemData();
		if (varNode) {
			charge = varNode->charge;
		}
		else {
			TES3::Enchantment* enchantment = object->getEnchantment();
			if (enchantment) {
				charge = enchantment->maxCharge;
			}
			else {
				if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
					log::getLog() << "xGetCharge: Invalid call on record of type " << object->objectType << "." << std::endl;
				}
			}
		}

		stack.pushFloat(charge);

		return 0.0f;
	}
}
