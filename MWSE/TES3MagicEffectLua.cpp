#include "TES3MagicEffectLua.h"

#include "LuaManager.h"
#include "TES3ObjectLua.h"

#include "TES3MagicEffect.h"

namespace mwse {
	namespace lua {
		void bindTES3MagicEffect() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Binding for TES3::MagicEffect
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<TES3::MagicEffect>("tes3magicEffect");
				usertypeDefinition["new"] = sol::no_constructor;

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition[sol::base_classes] = sol::bases<TES3::BaseObject>();
				setUserdataForTES3BaseObject(usertypeDefinition);

				// Basic property binding.
				usertypeDefinition["areaSoundEffect"] = sol::property(&TES3::MagicEffect::getAreaSoundEffect, &TES3::MagicEffect::setAreaSoundEffect);
				usertypeDefinition["areaVisualEffect"] = &TES3::MagicEffect::areaEffect;
				usertypeDefinition["baseMagickaCost"] = &TES3::MagicEffect::baseMagickaCost;
				usertypeDefinition["boltSoundEffect"] = sol::property(&TES3::MagicEffect::getBoltSoundEffect, &TES3::MagicEffect::setBoltSoundEffect);
				usertypeDefinition["boltVisualEffect"] = &TES3::MagicEffect::boltEffect;
				usertypeDefinition["castSoundEffect"] = sol::property(&TES3::MagicEffect::getCastSoundEffect, &TES3::MagicEffect::setCastSoundEffect);
				usertypeDefinition["castVisualEffect"] = &TES3::MagicEffect::castEffect;
				usertypeDefinition["description"] = sol::property(&TES3::MagicEffect::getDescription, &TES3::MagicEffect::setDescription);
				usertypeDefinition["flags"] = &TES3::MagicEffect::flags;
				usertypeDefinition["hitSoundEffect"] = sol::property(&TES3::MagicEffect::getHitSoundEffect, &TES3::MagicEffect::setHitSoundEffect);
				usertypeDefinition["hitVisualEffect"] = &TES3::MagicEffect::hitEffect;
				usertypeDefinition["icon"] = sol::property(&TES3::MagicEffect::getIcon, &TES3::MagicEffect::setIcon);
				usertypeDefinition["id"] = sol::readonly_property(&TES3::MagicEffect::id);
				usertypeDefinition["lightingBlue"] = &TES3::MagicEffect::lightingBlue;
				usertypeDefinition["lightingGreen"] = &TES3::MagicEffect::lightingGreen;
				usertypeDefinition["lightingRed"] = &TES3::MagicEffect::lightingRed;
				usertypeDefinition["name"] = sol::readonly_property(&TES3::MagicEffect::getName);
				usertypeDefinition["particleTexture"] = sol::property(&TES3::MagicEffect::getParticleTexture, &TES3::MagicEffect::setParticleTexture);
				usertypeDefinition["school"] = &TES3::MagicEffect::school;
				usertypeDefinition["size"] = &TES3::MagicEffect::size;
				usertypeDefinition["sizeCap"] = &TES3::MagicEffect::sizeCap;
				usertypeDefinition["skill"] = sol::readonly_property(&TES3::MagicEffect::getSkillForSchool);
				usertypeDefinition["speed"] = &TES3::MagicEffect::speed;

				// Allow access to base effect flags.
				usertypeDefinition["allowEnchanting"] = sol::property(&TES3::MagicEffect::getAllowEnchanting, &TES3::MagicEffect::setAllowEnchanting);
				usertypeDefinition["allowSpellmaking"] = sol::property(&TES3::MagicEffect::getAllowSpellmaking, &TES3::MagicEffect::setAllowSpellmaking);
				usertypeDefinition["appliesOnce"] = sol::property(&TES3::MagicEffect::getFlagAppliedOnce, &TES3::MagicEffect::setFlagAppliedOnce);
				usertypeDefinition["baseFlags"] = sol::property(&TES3::MagicEffect::getEffectFlags, &TES3::MagicEffect::setEffectFlags);
				usertypeDefinition["canCastSelf"] = sol::property(&TES3::MagicEffect::getFlagCanCastSelf, &TES3::MagicEffect::setFlagCanCastSelf);
				usertypeDefinition["canCastTarget"] = sol::property(&TES3::MagicEffect::getFlagCanCastTarget, &TES3::MagicEffect::setFlagCanCastTarget);
				usertypeDefinition["canCastTouch"] = sol::property(&TES3::MagicEffect::getFlagCanCastTouch, &TES3::MagicEffect::setFlagCanCastTouch);
				usertypeDefinition["casterLinked"] = sol::property(&TES3::MagicEffect::getFlagCasterLinked, &TES3::MagicEffect::setFlagCasterLinked);
				usertypeDefinition["hasContinuousVFX"] = sol::property(&TES3::MagicEffect::getFlagContinuousVFX, &TES3::MagicEffect::setFlagContinuousVFX);
				usertypeDefinition["hasNoDuration"] = sol::property(&TES3::MagicEffect::getFlagNoDuration, &TES3::MagicEffect::setFlagNoDuration);
				usertypeDefinition["hasNoMagnitude"] = sol::property(&TES3::MagicEffect::getFlagNoMagnitude, &TES3::MagicEffect::setFlagNoMagnitude);
				usertypeDefinition["illegalDaedra"] = sol::property(&TES3::MagicEffect::getFlagIllegalDaedra, &TES3::MagicEffect::setFlagIllegalDaedra);
				usertypeDefinition["isHarmful"] = sol::property(&TES3::MagicEffect::getFlagHarmful, &TES3::MagicEffect::setFlagHarmful);
				usertypeDefinition["nonRecastable"] = sol::property(&TES3::MagicEffect::getFlagNonRecastable, &TES3::MagicEffect::setFlagNonRecastable);
				usertypeDefinition["targetsAttributes"] = sol::property(&TES3::MagicEffect::getFlagTargetAttribute, &TES3::MagicEffect::setFlagTargetAttribute);
				usertypeDefinition["targetsSkills"] = sol::property(&TES3::MagicEffect::getFlagTargetSkill, &TES3::MagicEffect::setFlagTargetSkill);
				usertypeDefinition["unreflectable"] = sol::property(&TES3::MagicEffect::getFlagUnreflectable, &TES3::MagicEffect::setFlagUnreflectable);
				usertypeDefinition["usesNegativeLighting"] = sol::property(&TES3::MagicEffect::getFlagNegativeLighting, &TES3::MagicEffect::setFlagNegativeLighting);
			}

			// Binding for TES3::Effect
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<TES3::Effect>("tes3effect");
				usertypeDefinition["new"] = sol::no_constructor;

				// Convert to string.
				usertypeDefinition[sol::meta_function::to_string] = &TES3::Effect::toString;

				// Basic property binding.
				usertypeDefinition["attribute"] = sol::property(&TES3::Effect::geAttributeID, &TES3::Effect::seAttributeID_lua);
				usertypeDefinition["cost"] = sol::readonly_property(&TES3::Effect::calculateCost);
				usertypeDefinition["duration"] = &TES3::Effect::duration;
				usertypeDefinition["id"] = &TES3::Effect::effectID;
				usertypeDefinition["max"] = &TES3::Effect::magnitudeMax;
				usertypeDefinition["min"] = &TES3::Effect::magnitudeMin;
				usertypeDefinition["radius"] = &TES3::Effect::radius;
				usertypeDefinition["rangeType"] = &TES3::Effect::rangeType;
				usertypeDefinition["skill"] = sol::property(&TES3::Effect::getSkillID, &TES3::Effect::setSkillID_lua);

				// Allow easy access to the base magic effect.
				usertypeDefinition["object"] = sol::readonly_property(&TES3::Effect::getEffectData);
			}
		}
	}
}
