#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3ItemData.h"
#include "TES3Reference.h"
#include "TES3Enchantment.h"

namespace mwse {
	class xSetMaxCharge : InstructionInterface_t {
	public:
		xSetMaxCharge();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xSetMaxCharge xSetMaxChargeInstance;

	xSetMaxCharge::xSetMaxCharge() : mwse::InstructionInterface_t(OpCode::xSetMaxCharge) {}

	float xSetMaxCharge::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get parameter from the stack.
		float maxCharge = mwse::Stack::getInstance().popFloat();
		bool success = false;

		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xSetMaxCharge: No reference provided." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(false);
			return 0.0f;
		}

		// Get the base record.
		TES3::Object* object = reference->baseObject;
		if (object == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xSetMaxCharge: No record found for reference." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(false);
			return 0.0f;
		}

		// If we found an enchantment record, set the max charge.
		TES3::Enchantment* enchantment = object->vTable.object->getEnchantment(object);
		if (enchantment) {
			enchantment->maxCharge = static_cast<unsigned short>(maxCharge);

			// If there's charge data in an attached node, update it.
			auto varNode = reference->getAttachedItemData();
			if (varNode) {
				if (varNode->charge >= maxCharge) {
					varNode->charge = maxCharge;
				}
			}

			success = true;
		}

		// Push success state.
		mwse::Stack::getInstance().pushLong(success);

		return 0.0f;
	}
}
