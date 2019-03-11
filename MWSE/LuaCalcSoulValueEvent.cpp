#include "LuaCalcSoulValueEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Creature.h"
#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			CalculateSoulValueEvent::CalculateSoulValueEvent(TES3::Actor * actor, int value) :
				ObjectFilteredEvent("calcSoulValue", actor),
				m_Actor(actor),
				m_Value(value)
			{

			}

			sol::table CalculateSoulValueEvent::createEventTable() {
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["actor"] = makeLuaObject(m_Actor);
				eventData["value"] = m_Value;

				return eventData;
			}
		}
	}
}
