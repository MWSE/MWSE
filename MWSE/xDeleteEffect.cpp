#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3Alchemy.h"
#include "TES3DataHandler.h"
#include "TES3Enchantment.h"
#include "TES3Spell.h"

namespace mwse {
	class xDeleteEffect : InstructionInterface_t {
	public:
		xDeleteEffect();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xDeleteEffect xDeleteEffectInstance;

	xDeleteEffect::xDeleteEffect() : mwse::InstructionInterface_t(OpCode::xDeleteEffect) {}

	float xDeleteEffect::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get parameters.
		long type = mwse::Stack::getInstance().popLong();
		mwseString& id = virtualMachine.getString(mwse::Stack::getInstance().popLong());
		unsigned long effectIndex = mwse::Stack::getInstance().popLong() - 1; // 0-based index.
		size_t effectCount = 0;

		// Get the desired effect.
		TES3::Effect* effects = NULL;
		if (type == TES3::ObjectType::Spell) {
			const auto spell = TES3::DataHandler::get()->nonDynamicData->getSpellById(id.c_str());
			if (spell) {
				effects = spell->effects;
				effectCount = spell->getActiveEffectCount();
			}
			else {
				if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
					mwse::log::getLog() << "xDeleteEffect: No spell found with id '" << id << "'." << std::endl;
				}
				mwse::Stack::getInstance().pushLong(false);
				return 0.0f;
			}
		}
		else if (type == TES3::ObjectType::Enchantment) {
			const auto enchant = TES3::DataHandler::get()->nonDynamicData->resolveObjectByType<TES3::Enchantment>(id.c_str());
			if (enchant) {
				effects = enchant->effects;
				effectCount = enchant->getActiveEffectCount();
			}
			else {
				if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
					mwse::log::getLog() << "xDeleteEffect: No spell found with id '" << id << "'." << std::endl;
				}
				mwse::Stack::getInstance().pushLong(false);
				return 0.0f;
			}
		}
		else if (type == TES3::ObjectType::Alchemy) {
			const auto alchemy = TES3::DataHandler::get()->nonDynamicData->resolveObjectByType<TES3::Alchemy>(id.c_str());
			if (alchemy) {
				effects = alchemy->effects;
				effectCount = alchemy->getActiveEffectCount();
			}
			else {
				if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
					mwse::log::getLog() << "xDeleteEffect: No alchemy record found with id '" << id << "'." << std::endl;
				}
			}
		}
		else {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xDeleteEffect: Record type of " << type << " is not supported." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(false);
			return 0.0f;
		}

		// Verify that the effect can be deleted.
		if (effectCount == 1) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xDeleteEffect: Effect count must be at least one." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(false);
			return 0.0f;
		}
		else if (effectIndex >= effectCount) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xDeleteEffect: Effect index out of range for effect." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(false);
			return 0.0f;
		}

		// Disable the effect.
		effects[effectIndex].effectID = TES3::EffectID::None;

		// Move effects beyond the given index forward to fill the gap.
		for (size_t i = effectIndex + 1; i < effectCount; ++i) {
			effects[i - 1] = effects[i];
			effects[i].effectID = TES3::EffectID::None;
		}

		mwse::Stack::getInstance().pushLong(true);

		return 0.0f;
	}
}
