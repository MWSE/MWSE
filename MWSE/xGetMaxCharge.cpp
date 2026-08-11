#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "TES3Reference.h"
#include "TES3Enchantment.h"

namespace mwse {
	class xGetMaxCharge : InstructionInterface_t {
	public:
		xGetMaxCharge();
		virtual float execute(VMExecuteInterface& virtualMachine);
	private:
		const float INVALID_VALUE = -1.0f;
	};

	static xGetMaxCharge xGetMaxChargeInstance;

	xGetMaxCharge::xGetMaxCharge() : mwse::InstructionInterface_t(OpCode::xGetMaxCharge) {}

	float xGetMaxCharge::execute(mwse::VMExecuteInterface& virtualMachine) {
		float charge = 0.0f;

		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetMaxCharge: No reference provided." << std::endl;
			}
			mwse::Stack::getInstance().pushFloat(INVALID_VALUE);
			return 0.0f;
		}

		// Get the base record.
		TES3::Object* object = reference->baseObject;
		if (object == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetMaxCharge: No record found for reference." << std::endl;
			}
			mwse::Stack::getInstance().pushFloat(INVALID_VALUE);
			return 0.0f;
		}

		// Get  the maximum charge from the enchantment record.
		TES3::Enchantment* enchantment = object->getEnchantment();
		if (enchantment) {
			charge = enchantment->maxCharge;
		}

		mwse::Stack::getInstance().pushFloat(charge);

		return 0.0f;
	}
}
