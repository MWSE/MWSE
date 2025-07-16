#include "LuaPotionBrewSkillCheckEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Alchemy.h"
#include "TES3Apparatus.h"
#include "TES3Ingredient.h"
#include "TES3MobilePlayer.h"
#include "TES3WorldController.h"

namespace mwse::lua::event {
	PotionBrewSkillCheckEvent::PotionBrewSkillCheckEvent(float potionStrength, const TES3::AlchemyBrewingItems& items) :
		GenericEvent("potionBrewSkillCheck"),
		m_PotionStrength(potionStrength),
		m_Items(items)
	{

	}

	sol::table PotionBrewSkillCheckEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["potionStrength"] = m_PotionStrength;
		eventData["success"] = m_PotionStrength >= 0;

		// Add the apparatus used.
		eventData["alembic"] = m_Items.alembic;
		eventData["calcinator"] = m_Items.calcinator;
		eventData["mortar"] = m_Items.mortar;
		eventData["retort"] = m_Items.retort;

		// Add the ingredients used.
		sol::table ingredients = state.create_table();
		if (m_Items.ingredient1) ingredients.add(m_Items.ingredient1);
		if (m_Items.ingredient2) ingredients.add(m_Items.ingredient2);
		if (m_Items.ingredient3) ingredients.add(m_Items.ingredient3);
		if (m_Items.ingredient4) ingredients.add(m_Items.ingredient4);
		eventData["ingredients"] = ingredients;

		return eventData;
	}
}
