#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3DataHandler.h"
#include "TES3Ingredient.h"

namespace mwse {
	class xGetIngredientEffect : InstructionInterface_t {
	public:
		xGetIngredientEffect();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xGetIngredientEffect xGetIngredientEffectInstance;

	xGetIngredientEffect::xGetIngredientEffect() : InstructionInterface_t(OpCode::xGetIngredientEffect) {}

	float xGetIngredientEffect::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get parameters.
		mwseString& id = virtualMachine.getString(stack.popLong());
		long index = stack.popLong() - 1;

		// Return values.
		long effectEnumId = TES3::EffectID::None;
		long skillAttributeId = -1;

		// Get the ingredient.
		const auto ingredient = TES3::DataHandler::get()->nonDynamicData->resolveObjectByType<TES3::Ingredient>(id);
		if (ingredient == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xGetIngredientEffect: No ingredient record found with id '" << id << "'." << std::endl;
			}
			stack.pushLong(skillAttributeId);
			stack.pushLong(effectEnumId);
			return 0.0f;
		}

		// Validate index.
		if (index < 0 || index > 3) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xGetIngredientEffect: Invalid index. Value must be between 1 and 4." << std::endl;
			}
			stack.pushLong(skillAttributeId);
			stack.pushLong(effectEnumId);
			return 0.0f;
		}

		// Get the ingredient effect information.
		effectEnumId = ingredient->effects[index];
		if (effectEnumId == TES3::EffectID::DrainAttribute || effectEnumId == TES3::EffectID::DamageAttribute
			|| effectEnumId == TES3::EffectID::RestoreAttribute || effectEnumId == TES3::EffectID::FortifyAttribute
			|| effectEnumId == TES3::EffectID::AbsorbAttribute) {
			skillAttributeId = ingredient->effectAttributeIds[index];
		}
		else if (effectEnumId == TES3::EffectID::DrainSkill || effectEnumId == TES3::EffectID::DamageSkill
			|| effectEnumId == TES3::EffectID::RestoreSkill || effectEnumId == TES3::EffectID::FortifySkill
			|| effectEnumId == TES3::EffectID::AbsorbSkill) {
			skillAttributeId = ingredient->effectSkillIds[index];
		}

		stack.pushLong(skillAttributeId);
		stack.pushLong(effectEnumId);

		return 0.0f;
	}
}
