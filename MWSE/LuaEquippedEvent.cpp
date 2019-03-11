#include "LuaEquippedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Actor.h"
#include "TES3ItemData.h"
#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			EquippedEvent::EquippedEvent(TES3::Actor* a, TES3::MobileActor* ma, TES3::BaseObject* i, TES3::ItemData* id) :
				ObjectFilteredEvent("equipped", i),
				m_Actor(a),
				m_MobileActor(ma),
				m_Item(i),
				m_ItemData(id)
			{

			}

			sol::table EquippedEvent::createEventTable() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["actor"] = makeLuaObject(m_Actor);
				eventData["mobile"] = makeLuaObject(m_MobileActor);
				if (m_MobileActor) {
					eventData["reference"] = makeLuaObject(m_MobileActor->reference);
				}
				eventData["item"] = makeLuaObject(m_Item);
				eventData["itemData"] = m_ItemData;

				return eventData;
			}
		}
	}
}
