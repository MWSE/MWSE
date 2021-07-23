#include "LuaPotionBrewedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Alchemy.h"
#include "TES3Apparatus.h"
#include "TES3Ingredient.h"
#include "TES3MobilePlayer.h"
#include "TES3WorldController.h"

namespace mwse {
	namespace lua {
		namespace event {
			PotionBrewedEvent::PotionBrewedEvent(TES3::Alchemy * object, TES3::Ingredient* ingredient1, TES3::Ingredient* ingredient2, TES3::Ingredient* ingredient3, TES3::Ingredient* ingredient4) :
				GenericEvent("potionBrewed"),
				m_Object(object),
				m_Ingredient1(ingredient1),
				m_Ingredient2(ingredient2),
				m_Ingredient3(ingredient3),
				m_Ingredient4(ingredient4)
			{

			}

			sol::table PotionBrewedEvent::createEventTable() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table eventData = state.create_table();

				eventData["object"] = m_Object;

				// Add the ingredients used.
				sol::table ingredients = state.create_table();
				if (m_Ingredient1) ingredients.add(m_Ingredient1);
				if (m_Ingredient2) ingredients.add(m_Ingredient2);
				if (m_Ingredient3) ingredients.add(m_Ingredient3);
				if (m_Ingredient4) ingredients.add(m_Ingredient4);
				eventData["ingredients"] = ingredients;

				// Also supply easy access to the apparatus used.
				auto macp = TES3::WorldController::get()->getMobilePlayer();
				eventData["alembic"] = macp->lastUsedAlembic;
				eventData["calcinator"] = macp->lastUsedCalcinator;
				eventData["mortar"] = macp->lastUsedMortar;
				eventData["retort"] = macp->lastUsedRetort;

				return eventData;
			}

			bool PotionBrewedEvent::m_EventEnabled = false;
		}
	}
}
