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
		float execute(VMExecuteInterface& virtualMachine) override;
	private:
		const float INVALID_VALUE = -1.0f;
	};

	static xGetMaxCharge xGetMaxChargeInstance;

	xGetMaxCharge::xGetMaxCharge() : InstructionInterface_t(OpCode::xGetMaxCharge) {}

	float xGetMaxCharge::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		float charge = 0.0f;

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
				log::getLog() << "xGetMaxCharge: No record found for reference." << std::endl;
			}
			stack.pushFloat(INVALID_VALUE);
			return 0.0f;
		}

		// Get  the maximum charge from the enchantment record.
		TES3::Enchantment* enchantment = object->getEnchantment();
		if (enchantment) {
			charge = enchantment->maxCharge;
		}

		stack.pushFloat(charge);

		return 0.0f;
	}
}
