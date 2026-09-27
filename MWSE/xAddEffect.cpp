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
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xAddEffect xAddEffectInstance;

	xAddEffect::xAddEffect() : mwse::InstructionInterface_t(OpCode::xAddEffect) {}

	float xAddEffect::execute(mwse::VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get parameters.
		long type = stack.popLong();
		mwseString& id = virtualMachine.getString(stack.popLong());
		long effectId = stack.popLong();
		long skillAttributeId = stack.popLong();
		long range = stack.popLong();
		long area = stack.popLong();
		long duration = stack.popLong();
		long magMin = stack.popLong();
		long magMax = stack.popLong();
		size_t effectCount = 0;

		// Get the desired effect.
		TES3::Effect* effects = nullptr;
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
				stack.pushLong(false);
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
				stack.pushLong(false);
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
				stack.pushLong(false);
				return 0.0f;
			}
		}
		else {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xAddEffect: Record type of " << type << " is not supported." << std::endl;
			}
			stack.pushLong(false);
			return 0.0f;
		}

		// Get effect count.
		if (effectCount == 8) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xAddEffect: Record already contains 8 effects." << std::endl;
			}
			stack.pushLong(false);
			return 0.0f;
		}

		// Add effect.
		auto success = tes3::setEffect(effects, effectCount + 1, effectId, skillAttributeId, range, area, duration, magMin, magMax);
		stack.pushLong(success);

		return 0.0f;
	}
}
