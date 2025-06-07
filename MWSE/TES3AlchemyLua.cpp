#include "TES3AlchemyLua.h"

#include "LuaManager.h"
#include "TES3ItemLua.h"

#include "TES3Alchemy.h"
#include "TES3DataHandler.h"
#include "TES3Script.h"
#include "TES3WorldController.h"

#include "MemoryUtil.h"

namespace mwse::lua {
	TES3::Alchemy* createAlchemy(sol::table params) {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;

		// Do we already have an object of this ID?
		auto ndd = TES3::DataHandler::get()->nonDynamicData;
		std::string id = getOptionalParam<std::string>(params, "id", std::string());
		if (!id.empty() && ndd->resolveObject(id.c_str()) != nullptr) {
			return nullptr;
		}

		// Create new alchemy object.
		TES3::Alchemy* alchemy = new TES3::Alchemy();

		// Set ID. It isn't required.
		if (!id.empty()) {
			alchemy->setID(id.c_str());
		}

		// Set name. Defaults to "Potion"
		std::string name = getOptionalParam<std::string>(params, "name", "Potion");
		alchemy->setName(name.c_str());

		// Set script.
		TES3::Script* script = getOptionalParamScript(params, "script");
		if (script != nullptr) {
			alchemy->script = script;
		}

		// Set model.
		std::string model = getOptionalParam<std::string>(params, "mesh", "m\\Misc_Potion_Bargain_01.nif");
		alchemy->setModelPath(model.c_str());

		// Set texture.
		std::string icon = getOptionalParam<std::string>(params, "icon", "m\\Tx_potion_bargain_01.tga");
		tes3::setDataString(&alchemy->icon, icon.c_str());

		// Get other simple values.
		alchemy->objectFlags = getOptionalParam<unsigned int>(params, "objectFlags", alchemy->objectFlags);
		alchemy->weight = getOptionalParam<double>(params, "weight", alchemy->weight);
		alchemy->value = getOptionalParam<double>(params, "value", alchemy->value);
		alchemy->flags = getOptionalParam<unsigned short>(params, "flags", alchemy->flags);

		// Flag the object as modified.
		alchemy->objectFlags |= TES3::ObjectFlag::Modified;

		// Assign effects.
		sol::optional<sol::table> effects = params["effects"];
		if (effects) {
			for (int i = 1; i <= 8; ++i) {
				sol::optional<sol::table> effectParams = effects.value()[i];
				if (!effectParams) {
					break;
				}

				TES3::Effect* effect = &alchemy->effects[i - 1];
				effect->effectID = getOptionalParam<int>(effectParams, "id", -1);
				effect->skillID = getOptionalParam<int>(effectParams, "skill", -1);
				effect->attributeID = getOptionalParam<int>(effectParams, "attribute", -1);
				effect->rangeType = static_cast<TES3::EffectRange>(getOptionalParam<int>(effectParams, "range", int(TES3::EffectRange::Self)));
				effect->radius = getOptionalParam<int>(effectParams, "radius", 0);
				effect->duration = getOptionalParam<int>(effectParams, "duration", 0);
				effect->magnitudeMin = getOptionalParam<int>(effectParams, "min", 0);
				effect->magnitudeMax = getOptionalParam<int>(effectParams, "max", 0);
			}
		}

		// If we don't have an effect, fill in the first one.
		if (alchemy->effects[0].effectID == TES3::EffectID::None) {
			alchemy->effects[0].effectID = TES3::EffectID::WaterBreathing;
		}

		// We have our alchemy object. But is it unique?
		auto matches = alchemy->findMatchingAlchemyItem();
		if (matches) {
			delete alchemy;
			return matches;
		}

		// All good? Add and return the object.
		if (!ndd->addNewObject(alchemy)) {
			return nullptr;
		}
		return alchemy;
	}

	void bindTES3Alchemy() {
		// Get our lua state.
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;

		// Start our usertype.
		auto usertypeDefinition = state.new_usertype<TES3::Alchemy>("tes3alchemy");
		usertypeDefinition["new"] = sol::no_constructor;

		// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
		usertypeDefinition[sol::base_classes] = sol::bases<TES3::Item, TES3::PhysicalObject, TES3::Object, TES3::BaseObject>();
		setUserdataForTES3Item(usertypeDefinition);

		// Basic property binding.
		usertypeDefinition["flags"] = &TES3::Alchemy::flags;
		usertypeDefinition["weight"] = &TES3::Alchemy::weight;
		usertypeDefinition["value"] = &TES3::Alchemy::value;

		// Indirect bindings to unions and arrays.
		usertypeDefinition["effects"] = sol::readonly_property(&TES3::Alchemy::getEffects);

		// Basic function binding.
		usertypeDefinition["create"] = &createAlchemy;
		usertypeDefinition["createCopy"] = &TES3::Alchemy::createCopy_lua<TES3::Alchemy>;
		usertypeDefinition["getActiveEffectCount"] = &TES3::Alchemy::getActiveEffectCount;
		usertypeDefinition["getFirstIndexOfEffect"] = &TES3::Alchemy::getFirstIndexOfEffect;
		usertypeDefinition["hasEffect"] = &TES3::Alchemy::hasEffect;

		// Functions exposed as properties.
		usertypeDefinition["autoCalc"] = sol::property(&TES3::Alchemy::getAutoCalc, &TES3::Alchemy::setAutoCalc);
		usertypeDefinition["script"] = &TES3::Alchemy::script;
	}
}
