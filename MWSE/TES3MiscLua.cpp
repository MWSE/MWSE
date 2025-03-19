#include "TES3MiscLua.h"

#include "LuaManager.h"
#include "TES3ItemLua.h"

#include "TES3Misc.h"
#include "TES3Script.h"
#include "TES3SoulGemData.h"
#include "LuaObject.h"

namespace mwse::lua {
	auto createMiscItem(sol::table params)
	{
		return makeObjectCreator(TES3::ObjectType::Misc)->create(params, false);
	}

	void bindTES3Misc() {
		// Get our lua state.
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;

		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::SoulGemData>("tes3soulGemData");
			usertypeDefinition["new"] = sol::no_constructor;

			// Allow object to be converted to strings using their object ID.
			usertypeDefinition[sol::meta_function::to_string] = &TES3::SoulGemData::toString;

			// Allow object to be serialized to json.
			usertypeDefinition["__tojson"] = &TES3::SoulGemData::toJson;

			// Basic property binding.
			usertypeDefinition["id"] = sol::readonly_property(&TES3::SoulGemData::id);
			usertypeDefinition["mesh"] = sol::readonly_property(&TES3::SoulGemData::mesh);
			usertypeDefinition["name"] = sol::readonly_property(&TES3::SoulGemData::name);
			usertypeDefinition["texture"] = sol::readonly_property(&TES3::SoulGemData::texture);
			usertypeDefinition["value"] = sol::readonly_property(&TES3::SoulGemData::getValue);
			usertypeDefinition["weight"] = sol::readonly_property(&TES3::SoulGemData::getWeight);
			usertypeDefinition["item"] = sol::readonly_property(&TES3::SoulGemData::item);
			usertypeDefinition["capacity"] = sol::readonly_property(&TES3::SoulGemData::getCapacity);

			// TODO: Deprecated. Remove before 2.1-stable.
			usertypeDefinition["model"] = sol::readonly_property(&TES3::SoulGemData::mesh);
		}

		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::Misc>("tes3misc");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::Item, TES3::PhysicalObject, TES3::Object, TES3::BaseObject>();
			setUserdataForTES3Item(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["script"] = &TES3::Misc::script;
			usertypeDefinition["soulGemCapacity"] = sol::readonly_property(&TES3::Misc::getSoulGemCapacity);
			usertypeDefinition["value"] = &TES3::Misc::value;
			usertypeDefinition["weight"] = &TES3::Misc::weight;

			// Functions exposed as properties.
			usertypeDefinition["isGold"] = sol::readonly_property(&TES3::Misc::isGold);
			usertypeDefinition["isKey"] = sol::property(&TES3::Misc::getIsKey, &TES3::Misc::setIsKey);
			usertypeDefinition["isSoulGem"] = sol::readonly_property(&TES3::Misc::isSoulGem);
			usertypeDefinition["soulGemData"] = sol::readonly_property(&TES3::Misc::getSoulGemData);

			// utility function bindings
			usertypeDefinition["create"] = &createMiscItem;
			usertypeDefinition["createCopy"] = &TES3::Misc::createCopy_lua<TES3::Misc>;
		}
	}
}
