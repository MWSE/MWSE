#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3DataHandler.h"
#include "TES3Enchantment.h"

namespace mwse {
	class xGetEnchantEffectInfo : InstructionInterface_t {
	public:
		xGetEnchantEffectInfo();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xGetEnchantEffectInfo xGetEnchantEffectInfoInstance;

	xGetEnchantEffectInfo::xGetEnchantEffectInfo() : InstructionInterface_t(OpCode::xGetEnchantEffectInfo) {}

	float xGetEnchantEffectInfo::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get parameters.
		mwseString& effectId = virtualMachine.getString(stack.popLong());
		short effectIndex = stack.popShort();

		// Return values.
		long effectEnumId = TES3::EffectID::None;
		long rangeType = 0;
		long area = 0;
		long duration = 0;
		long magMin = 0;
		long magMax = 0;

		// Validate effect index.
		if (effectIndex >= 1 && effectIndex <= 8) {
			// Get the desired effect.
			const auto enchant = TES3::DataHandler::get()->nonDynamicData->resolveObjectByType<TES3::Enchantment>(effectId);
			if (enchant) {
				const auto effect = &enchant->effects[effectIndex - 1];
				// If we found an effect, set the values.
				if (effect && effect->effectID != TES3::EffectID::None) {
					effectEnumId = effect->effectID;
					rangeType = int(effect->rangeType);
					area = effect->radius;
					duration = effect->duration;
					magMin = effect->magnitudeMin;
					magMax = effect->magnitudeMax;
				}
			}
			else {
				if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
					log::getLog() << "xGetEnchantEffectInfo: No spell found with id '" << effectId << "'." << std::endl;
				}
			}
		}
		else {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xGetEnchantEffectInfo: Invalid effect index. Value must be between 1 and 8." << std::endl;
			}
		}

		stack.pushLong(magMax);
		stack.pushLong(magMin);
		stack.pushLong(duration);
		stack.pushLong(area);
		stack.pushLong(rangeType);
		stack.pushLong(effectEnumId);

		return 0.0f;
	}
}
