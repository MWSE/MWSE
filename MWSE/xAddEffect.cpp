#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3Alchemy.h"
#include "TES3DataHandler.h"
#include "TES3Enchantment.h"
#include "TES3Spell.h"

namespace mwse {
	class xAddEffect : InstructionInterface_t {
	public:
		xAddEffect();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xAddEffect xAddEffectInstance;

	xAddEffect::xAddEffect() : mwse::InstructionInterface_t(OpCode::xAddEffect) {}

	float xAddEffect::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get parameters.
		long type = mwse::Stack::getInstance().popLong();
		mwseString& id = virtualMachine.getString(mwse::Stack::getInstance().popLong());
		long effectId = mwse::Stack::getInstance().popLong();
		long skillAttributeId = mwse::Stack::getInstance().popLong();
		long range = mwse::Stack::getInstance().popLong();
		long area = mwse::Stack::getInstance().popLong();
		long duration = mwse::Stack::getInstance().popLong();
		long magMin = mwse::Stack::getInstance().popLong();
		long magMax = mwse::Stack::getInstance().popLong();
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
					mwse::log::getLog() << "xAddEffect: No spell found with id '" << id << "'." << std::endl;
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
					mwse::log::getLog() << "xAddEffect: No spell found with id '" << id << "'." << std::endl;
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
					mwse::log::getLog() << "xAddEffect: No alchemy record found with id '" << id << "'." << std::endl;
				}
				mwse::Stack::getInstance().pushLong(false);
				return 0.0f;
			}
		}
		else {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xAddEffect: Record type of " << type << " is not supported." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(false);
			return 0.0f;
		}

		// Get effect count.
		if (effectCount == 8) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xAddEffect: Record already contains 8 effects." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(false);
			return 0.0f;
		}

		// Add effect.
		auto success = tes3::setEffect(effects, effectCount + 1, effectId, skillAttributeId, range, area, duration, magMin, magMax);
		mwse::Stack::getInstance().pushLong(success);

		return 0.0f;
	}
}
