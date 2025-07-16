#include "TES3SpellLua.h"

#include "LuaManager.h"
#include "TES3ObjectLua.h"

#include "TES3MobileActor.h"
#include "TES3Reference.h"
#include "TES3Spell.h"
#include "TES3WorldController.h"

#include "MemoryUtil.h"

#include "LuaSpellCreatedEvent.h"

namespace mwse::lua {
	TES3::Spell* createSpell(std::string id, sol::optional<std::string> name) {
		// Make sure a spell doesn't already exist with this id.
		if (TES3::DataHandler::get()->nonDynamicData->objectExists(id)) {
			return nullptr;
		}

		// Limit name to 31 characters.
		if (name && name.value().length() > 31) {
			throw std::invalid_argument("Name must be 31 characters or fewer.");
		}

		// Get spell list.
		auto spellsList = TES3::DataHandler::get()->nonDynamicData->spellsList;
		TES3::Spell* spellListHead = *spellsList->begin();

		// Create new spell.
		TES3::Spell* newSpell = new TES3::Spell();
		newSpell->owningCollection.asSpellList = spellsList;
		newSpell->magickaCost = 1;

		// Set id/name.
		newSpell->setID(id.c_str());

		if (name) {
			newSpell->setName(name.value().c_str());
		}
		else {
			newSpell->setName("");
		}

		// Set the first effect just so that there is something? TODO: Why?
		tes3::setEffect(newSpell->effects, 1, TES3::EffectID::WaterBreathing, TES3::SkillID::Invalid, int(TES3::EffectRange::Self), 0, 1, 0, 0);

		// Add object to the game.
		TES3::DataHandler::get()->nonDynamicData->addNewObject(newSpell);

		// Fire off spell created event.
		if (event::SpellCreatedEvent::getEventEnabled()) {
			LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::SpellCreatedEvent(newSpell, "script"));
		}

		// Force the object as modified.
		newSpell->setObjectModified(true);

		// Finally return the spell.
		return newSpell;
	}

	void bindTES3Spell() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Start our usertype.
		auto usertypeDefinition = state.new_usertype<TES3::Spell>("tes3spell");
		usertypeDefinition["new"] = sol::no_constructor;

		// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
		usertypeDefinition[sol::base_classes] = sol::bases<TES3::Object, TES3::BaseObject>();
		setUserdataForTES3Object(usertypeDefinition);

		// Basic property binding.
		usertypeDefinition["basePurchaseCost"] = sol::readonly_property(&TES3::Spell::calculateBasePuchaseCost);
		usertypeDefinition["castType"] = &TES3::Spell::castType;
		usertypeDefinition["effects"] = sol::readonly_property(&TES3::Spell::getEffects);
		usertypeDefinition["flags"] = &TES3::Spell::spellFlags;
		usertypeDefinition["isActiveCast"] = sol::readonly_property(&TES3::Spell::isActiveCast);
		usertypeDefinition["magickaCost"] = &TES3::Spell::magickaCost;
		usertypeDefinition["name"] = sol::property(&TES3::Spell::getName, &TES3::Spell::setName);
		usertypeDefinition["value"] = sol::readonly_property(&TES3::Spell::getValue);

		// Basic function binding.
		usertypeDefinition["calculateCastChance"] = &TES3::Spell::calculateCastChance_lua;
		usertypeDefinition["create"] = &createSpell;
		usertypeDefinition["createCopy"] = &TES3::Spell::createCopy_lua<TES3::Spell>;
		usertypeDefinition["getActiveEffectCount"] = &TES3::Spell::getActiveEffectCount;
		usertypeDefinition["getAutoCalcMagickaCost"] = &TES3::Spell::getAutoCalcMagickaCost;
		usertypeDefinition["getFirstIndexOfEffect"] = &TES3::Spell::getFirstIndexOfEffect;
		usertypeDefinition["getLeastProficientEffect"] = &TES3::Spell::getLeastProficientEffect_lua;
		usertypeDefinition["getLeastProficientSchool"] = &TES3::Spell::getLeastProficientSchool_lua;
		usertypeDefinition["hasEffect"] = &TES3::Spell::hasEffect;

		// Convenient flag access.
		usertypeDefinition["alwaysSucceeds"] = sol::property(&TES3::Spell::getAlwaysSucceeds, &TES3::Spell::setAlwaysSucceeds);
		usertypeDefinition["autoCalc"] = sol::property(&TES3::Spell::getAutoCalc, &TES3::Spell::setAutoCalc);
		usertypeDefinition["playerStart"] = sol::property(&TES3::Spell::getPlayerStart, &TES3::Spell::setPlayerStart);

		// Named cast type access.
		usertypeDefinition["isAbility"] = sol::readonly_property(&TES3::Spell::isAbility);
		usertypeDefinition["isBlightDisease"] = sol::readonly_property(&TES3::Spell::isBlightDisease);
		usertypeDefinition["isCorprusDisease"] = sol::readonly_property(&TES3::Spell::isCorprusDisease);
		usertypeDefinition["isCommonDisease"] = sol::readonly_property(&TES3::Spell::isCommonDisease);
		usertypeDefinition["isCurse"] = sol::readonly_property(&TES3::Spell::isCurse);
		usertypeDefinition["isDisease"] = sol::readonly_property(&TES3::Spell::isDisease);
		usertypeDefinition["isPower"] = sol::readonly_property(&TES3::Spell::isPower);
		usertypeDefinition["isSpell"] = sol::readonly_property(&TES3::Spell::isSpell);
	}
}
