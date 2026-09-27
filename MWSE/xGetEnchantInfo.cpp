#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3DataHandler.h"
#include "TES3Enchantment.h"

namespace mwse {
	class xGetEnchantInfo : InstructionInterface_t {
	public:
		xGetEnchantInfo();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xGetEnchantInfo xGetEnchantInfoInstance;

	xGetEnchantInfo::xGetEnchantInfo() : mwse::InstructionInterface_t(OpCode::xGetEnchantInfo) {}

	float xGetEnchantInfo::execute(mwse::VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get parameters.
		mwseString& enchantId = virtualMachine.getString(stack.popLong());

		// Return values.
		long type = 0;
		long cost = 0;
		long maxCharge = 0;
		long effects = 0;
		long autocalc = 0;

		// Validate effect index.
		const auto enchantment = TES3::DataHandler::get()->nonDynamicData->resolveObjectByType<TES3::Enchantment>(enchantId);
		if (enchantment != nullptr) {
			type = int(enchantment->castType);
			cost = enchantment->chargeCost;
			maxCharge = enchantment->maxCharge;
			effects = enchantment->getActiveEffectCount();
			autocalc = enchantment->getAutoCalc();
		}
		else {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetEnchantInfo: Could not find spell of id '" << enchantId << "'" << std::endl;
			}
		}
		stack.pushLong(autocalc);
		stack.pushLong(effects);
		stack.pushLong(maxCharge);
		stack.pushLong(cost);
		stack.pushLong(type);

		return 0.0f;
	}
}
