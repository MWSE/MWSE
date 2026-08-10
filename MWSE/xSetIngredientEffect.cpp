#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3DataHandler.h"
#include "TES3Ingredient.h"

namespace mwse {
	class xSetIngredientEffect : InstructionInterface_t {
	public:
		xSetIngredientEffect();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xSetIngredientEffect xSetIngredientEffectInstance;

	xSetIngredientEffect::xSetIngredientEffect() : mwse::InstructionInterface_t(OpCode::xSetIngredientEffect) {}

	float xSetIngredientEffect::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get parameters.
		mwseString& id = virtualMachine.getString(Stack::getInstance().popLong());
		long index = Stack::getInstance().popLong() - 1;
		long effectEnumId = Stack::getInstance().popLong();
		long skillAttributeId = Stack::getInstance().popLong();

		// Get the ingredient.
		const auto ingredient = TES3::DataHandler::get()->nonDynamicData->resolveObjectByType<TES3::Ingredient>(id);
		if (ingredient == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xSetIngredientEffect: No ingredient record found with id '" << id << "'." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(false);
			return 0.0f;
		}

		// Validate index.
		if (index < 0 || index > 3) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xSetIngredientEffect: Invalid index. Value must be between 1 and 4." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(false);
			return 0.0f;
		}

		// Validate effect id.
		if (effectEnumId < TES3::EffectID::FirstEffect || effectEnumId > TES3::EffectID::LastEffect) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xSetIngredientEffect: Invalid effect id." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(false);
			return 0.0f;
		}

		// Set the ingredient effect information.
		ingredient->effects[index] = effectEnumId;
		if (effectEnumId == TES3::EffectID::DrainAttribute || effectEnumId == TES3::EffectID::DamageAttribute
			|| effectEnumId == TES3::EffectID::RestoreAttribute || effectEnumId == TES3::EffectID::FortifyAttribute
			|| effectEnumId == TES3::EffectID::AbsorbAttribute) {
			ingredient->effectAttributeIds[index] = skillAttributeId;
			ingredient->effectSkillIds[index] = -1;
		}
		else if (effectEnumId == TES3::EffectID::DrainSkill || effectEnumId == TES3::EffectID::DamageSkill
			|| effectEnumId == TES3::EffectID::RestoreSkill || effectEnumId == TES3::EffectID::FortifySkill
			|| effectEnumId == TES3::EffectID::AbsorbSkill) {
			ingredient->effectAttributeIds[index] = -1;
			ingredient->effectSkillIds[index] = skillAttributeId;
		}
		else {
			ingredient->effectAttributeIds[index] = -1;
			ingredient->effectSkillIds[index] = -1;
		}

		mwse::Stack::getInstance().pushLong(true);

		return 0.0f;
	}
}
