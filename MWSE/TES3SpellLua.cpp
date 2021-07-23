#include "TES3SpellLua.h"

#include "LuaManager.h"
#include "TES3ObjectLua.h"

#include "TES3MobileActor.h"
#include "TES3Reference.h"
#include "TES3Spell.h"

#include "MemoryUtil.h"

#include "LuaSpellCreatedEvent.h"

namespace mwse {
	namespace lua {
		TES3::Spell* createSpell(std::string id, sol::optional<std::string> name) {
			// Make sure a spell doesn't already exist with this id.
			if (TES3::DataHandler::get()->nonDynamicData->resolveObjectByType<TES3::Spell>(id, TES3::ObjectType::Spell) != nullptr) {
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
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Start our usertype. We must finish this with state.set_usertype.
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
			usertypeDefinition["magickaCost"] = &TES3::Spell::magickaCost;
			usertypeDefinition["name"] = sol::property(&TES3::Spell::getName, &TES3::Spell::setName);

			// Basic function binding.
			usertypeDefinition["create"] = &createSpell;
			usertypeDefinition["calculateCastChance"] = &TES3::Spell::calculateCastChance_lua;
			usertypeDefinition["getActiveEffectCount"] = &TES3::Spell::getActiveEffectCount;
			usertypeDefinition["getFirstIndexOfEffect"] = &TES3::Spell::getFirstIndexOfEffect;
			usertypeDefinition["getLeastProficientEffect"] = &TES3::Spell::getLeastProficientEffect_lua;
			usertypeDefinition["getLeastProficientSchool"] = &TES3::Spell::getLeastProficientSchool_lua;

			// Convenient flag access.
			usertypeDefinition["alwaysSucceeds"] = sol::property(&TES3::Spell::getAlwaysSucceeds, &TES3::Spell::setAlwaysSucceeds);
			usertypeDefinition["autoCalc"] = sol::property(&TES3::Spell::getAutoCalc, &TES3::Spell::setAutoCalc);
			usertypeDefinition["playerStart"] = sol::property(&TES3::Spell::getPlayerStart, &TES3::Spell::setPlayerStart);
		}
	}
}
