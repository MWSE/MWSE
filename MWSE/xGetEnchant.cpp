#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3Armor.h"
#include "TES3Book.h"
#include "TES3Clothing.h"
#include "TES3Enchantment.h"
#include "TES3ItemData.h"
#include "TES3Reference.h"
#include "TES3Weapon.h"

namespace mwse {
	class xGetEnchant : InstructionInterface_t {
	public:
		xGetEnchant();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xGetEnchant xGetEnchantInstance;

	xGetEnchant::xGetEnchant() : mwse::InstructionInterface_t(OpCode::xGetEnchant) {}

	float xGetEnchant::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Return values.
		char* enchId = nullptr;
		long type = 0;
		long cost = 0;
		float currCharge = 0.0f;
		long maxCharge = 0;
		long effects = 0;
		long autocalc = 0;

		// Get reference to what we're finding enchantment information for.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference) {
			// Get data from ENCH record.
			TES3::Enchantment* enchantment = reference->baseObject->getEnchantment();
			if (enchantment) {
				enchId = enchantment->objectID;
				type = int(enchantment->castType);
				cost = enchantment->chargeCost;
				maxCharge = enchantment->maxCharge;
				effects = enchantment->getActiveEffectCount();
				autocalc = enchantment->getAutoCalc();

				// Get the current charge.
				auto varNode = reference->getAttachedItemData();
				if (varNode) {
					currCharge = varNode->charge;
				}
				else {
					currCharge = static_cast<float>(maxCharge);
				}
			}
			else {
				if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
					log::getLog() << "xGetEnchant: Could not find enchant record of record type: " << reference->baseObject->objectType << std::endl;
				}
			}
		}
		else {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xGetEnchant: No reference found for function." << std::endl;
			}
		}

		// Push results to the stack.
		auto& stack = Stack::getInstance();
		stack.pushLong(autocalc);
		stack.pushLong(effects);
		stack.pushLong(maxCharge);
		stack.pushFloat(currCharge);
		stack.pushLong(cost);
		stack.pushLong(type);
		stack.pushString(enchId);

		return 0.0f;
	}
}
