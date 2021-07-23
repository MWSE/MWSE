#include "TES3MagicSourceInstanceLua.h"

#include "LuaManager.h"
#include "TES3ObjectLua.h"

#include "TES3Item.h"
#include "TES3ItemData.h"
#include "TES3MagicEffectInstance.h"
#include "TES3MagicSourceInstance.h"
#include "TES3MobileProjectile.h"
#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		void bindTES3MagicSourceInstance() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.new_usertype<TES3::MagicSourceInstance>("tes3magicSourceInstance");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::BaseObject>();
			setUserdataForTES3BaseObject(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["castChanceOverride"] = &TES3::MagicSourceInstance::overrideCastChance;
			usertypeDefinition["itemData"] = sol::readonly_property(&TES3::MagicSourceInstance::castingItemCondition);
			usertypeDefinition["itemID"] = sol::readonly_property(&TES3::MagicSourceInstance::castingItemID);
			usertypeDefinition["magicID"] = sol::readonly_property(&TES3::MagicSourceInstance::magicID);
			usertypeDefinition["serialNumber"] = sol::readonly_property(&TES3::MagicSourceInstance::serialNumber);
			usertypeDefinition["sourceType"] = sol::readonly_property(&TES3::MagicSourceInstance::getSourceType);
			usertypeDefinition["state"] = &TES3::MagicSourceInstance::state;
			usertypeDefinition["timestampCastBegin"] = &TES3::MagicSourceInstance::timestampCastBegin;
			usertypeDefinition["target"] = sol::readonly_property(&TES3::MagicSourceInstance::target);
			usertypeDefinition["projectile"] = sol::readonly_property(&TES3::MagicSourceInstance::magicProjectile);
			usertypeDefinition["caster"] = sol::readonly_property(&TES3::MagicSourceInstance::caster);
			usertypeDefinition["item"] = sol::readonly_property(&TES3::MagicSourceInstance::castingItem);
			usertypeDefinition["source"] = sol::readonly_property(&TES3::MagicSourceInstance::getSourceObject);

			// Basic function binding.
			usertypeDefinition["getEffectInstance"] = &TES3::MagicSourceInstance::getEffectInstance;
			usertypeDefinition["getMagnitudeForIndex"] = &TES3::MagicSourceInstance::getMagnitude;
			usertypeDefinition["playVisualEffect"] = &TES3::MagicSourceInstance::playSpellVFX_lua;

			// Functions exposed as properties.
			usertypeDefinition["sourceEffects"] = sol::readonly_property(&TES3::MagicSourceInstance::getSourceEffects);
		}
	}
}
