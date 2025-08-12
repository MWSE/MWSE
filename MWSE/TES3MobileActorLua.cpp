#include "TES3MobileActorLua.h"

#include "LuaManager.h"

#include "TES3MagicEffectInstance.h"
#include "TES3MagicInstanceController.h"
#include "TES3MobileActor.h"
#include "TES3WorldController.h"

namespace mwse::lua {
	void bindTES3MobileActor() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Binding for TES3::ActiveMagicEffect
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::ActiveMagicEffect>("tes3activeMagicEffectLegacy");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["attributeId"] = sol::readonly_property(&TES3::ActiveMagicEffect::skillOrAttributeID);
			usertypeDefinition["duration"] = sol::readonly_property(&TES3::ActiveMagicEffect::duration);
			usertypeDefinition["effectId"] = sol::readonly_property(&TES3::ActiveMagicEffect::magicEffectID);
			usertypeDefinition["effectIndex"] = sol::readonly_property(&TES3::ActiveMagicEffect::magicInstanceEffectIndex);
			usertypeDefinition["harmful"] = sol::readonly_property(&TES3::ActiveMagicEffect::isHarmful);
			usertypeDefinition["isIllegalSummon"] = sol::readonly_property(&TES3::ActiveMagicEffect::isIllegalSummon);
			usertypeDefinition["serial"] = sol::readonly_property(&TES3::ActiveMagicEffect::magicInstanceSerial);
			usertypeDefinition["skillId"] = sol::readonly_property(&TES3::ActiveMagicEffect::skillOrAttributeID);

			// Expose functions as properties.
			usertypeDefinition["instance"] = sol::readonly_property(&TES3::ActiveMagicEffect::getInstance);
			usertypeDefinition["magnitude"] = sol::readonly_property(&TES3::ActiveMagicEffect::getMagnitude);

			// Legacy support.
			usertypeDefinition["flag9"] = sol::readonly_property(&TES3::ActiveMagicEffect::isIllegalSummon);
			usertypeDefinition["magnitudeMin"] = sol::readonly_property(&TES3::ActiveMagicEffect::unresistedMagnitude);
			usertypeDefinition["next"] = sol::property(&TES3::ActiveMagicEffect::getNext_legacy);

		}

		// Binding for TES3::ActiveMagicEffectLua
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::ActiveMagicEffectLua>("tes3activeMagicEffect");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["attributeId"] = sol::readonly_property(&TES3::ActiveMagicEffectLua::skillOrAttributeID);
			usertypeDefinition["duration"] = sol::readonly_property(&TES3::ActiveMagicEffectLua::duration);
			usertypeDefinition["effectId"] = sol::readonly_property(&TES3::ActiveMagicEffectLua::magicEffectID);
			usertypeDefinition["effectIndex"] = sol::readonly_property(&TES3::ActiveMagicEffectLua::magicInstanceEffectIndex);
			usertypeDefinition["harmful"] = sol::readonly_property(&TES3::ActiveMagicEffectLua::isHarmful);
			usertypeDefinition["isBoundItem"] = sol::readonly_property(&TES3::ActiveMagicEffectLua::isBoundItem);
			usertypeDefinition["isIllegalSummon"] = sol::readonly_property(&TES3::ActiveMagicEffectLua::isIllegalSummon);
			usertypeDefinition["isSummon"] = sol::readonly_property(&TES3::ActiveMagicEffectLua::isSummon);
			usertypeDefinition["mobile"] = sol::readonly_property(&TES3::ActiveMagicEffectLua::mobile);
			usertypeDefinition["serial"] = sol::readonly_property(&TES3::ActiveMagicEffectLua::magicInstanceSerial);
			usertypeDefinition["skillId"] = sol::readonly_property(&TES3::ActiveMagicEffectLua::skillOrAttributeID);

			// Expose functions as properties.
			usertypeDefinition["effectInstance"] = sol::readonly_property(&TES3::ActiveMagicEffectLua::getEffectInstance);
			usertypeDefinition["instance"] = sol::readonly_property(&TES3::ActiveMagicEffectLua::getInstance);
			usertypeDefinition["magnitude"] = sol::readonly_property(&TES3::ActiveMagicEffectLua::getMagnitude);

			// Legacy bindings for mod support.
			usertypeDefinition["first"] = sol::readonly_property(&TES3::ActiveMagicEffectLua::getFirst_legacy);
			usertypeDefinition["next"] = sol::readonly_property(&TES3::ActiveMagicEffectLua::getNext_legacy);
		}
	}
}
