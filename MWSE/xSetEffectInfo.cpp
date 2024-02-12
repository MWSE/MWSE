#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3Alchemy.h"
#include "TES3DataHandler.h"
#include "TES3Enchantment.h"
#include "TES3Spell.h"

namespace mwse {
	class xSetEffectInfo : InstructionInterface_t {
	public:
		xSetEffectInfo();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xSetEffectInfo xSetEffectInfoInstance;

	xSetEffectInfo::xSetEffectInfo() : mwse::InstructionInterface_t(OpCode::xSetEffectInfo) {}

	float xSetEffectInfo::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get parameters.
		long targetType = Stack::getInstance().popLong();
		mwseString& targetId = virtualMachine.getString(Stack::getInstance().popLong());
		long effectIndex = Stack::getInstance().popLong();
		long effectId = Stack::getInstance().popLong();
		long effectSkillAttributeId = Stack::getInstance().popLong();
		long effectRange = Stack::getInstance().popLong();
		long effectArea = Stack::getInstance().popLong();
		long effectDuration = Stack::getInstance().popLong();
		long effectMagMin = Stack::getInstance().popLong();
		long effectMagMax = Stack::getInstance().popLong();

		bool result = false;

		// Validate effect index.
		if (effectIndex >= 1 && effectIndex <= 8) {
			// Get the desired effect.
			TES3::Effect* effects = NULL;
			if (targetType == TES3::ObjectType::Spell) {
				TES3::Spell* spell = TES3::DataHandler::get()->nonDynamicData->resolveObjectByType<TES3::Spell>(targetId);
				if (spell) {
					effects = spell->effects;
				}
				else {
					if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
						mwse::log::getLog() << "xSetEffectInfo: No spell record found with id '" << targetId << "'." << std::endl;
					}
				}
			}
			else if (targetType == TES3::ObjectType::Enchantment) {
				const auto enchant = TES3::DataHandler::get()->nonDynamicData->resolveObjectByType<TES3::Enchantment>(targetId);
				if (enchant) {
					effects = enchant->effects;
				}
				else {
					if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
						mwse::log::getLog() << "xSetEffectInfo: No enchant record found with id '" << targetId << "'." << std::endl;
					}
				}
			}
			else if (targetType == TES3::ObjectType::Alchemy) {
				const auto alchemy = TES3::DataHandler::get()->nonDynamicData->resolveObjectByType<TES3::Alchemy>(targetId);
				if (alchemy) {
					effects = alchemy->effects;
				}
				else {
					if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
						mwse::log::getLog() << "xSetEffectInfo: No alchemy record found with id '" << targetId << "'." << std::endl;
					}
				}
			}
			else {
				if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
					mwse::log::getLog() << "xSetEffectInfo: Record type of " << targetType << " is not supported." << std::endl;
				}
			}

			// If we found an effect, set the values.
			if (effects) {
				result = tes3::setEffect(effects, effectIndex, effectId, effectSkillAttributeId, effectRange, effectArea, effectDuration, effectMagMin, effectMagMax);
			}
		}
		else {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xSetEffectInfo: Invalid effect index. Value must be between 1 and 8." << std::endl;
			}
		}

		mwse::Stack::getInstance().pushLong(result);
		return 0.0f;
	}
}
