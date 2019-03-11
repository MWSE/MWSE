#include "LuaCalcBarterPriceEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3ItemData.h"
#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			CalculateBarterPriceEvent::CalculateBarterPriceEvent(TES3::MobileActor * mobileActor, int basePrice, int price, bool buying, int count, TES3::EquipmentStack* stack) :
				ObjectFilteredEvent("calcBarterPrice", mobileActor->reference),
				m_MobileActor(mobileActor),
				m_BasePrice(basePrice),
				m_Price(price),
				m_Buying(buying),
				m_Count(count),
				m_Stack(stack)
			{

			}

			sol::table CalculateBarterPriceEvent::createEventTable() {
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["mobile"] = makeLuaObject(m_MobileActor);
				if (m_MobileActor) {
					eventData["reference"] = makeLuaObject(m_MobileActor->reference);
				}

				if (m_Stack) {
					eventData["item"] = makeLuaObject(m_Stack->object);
					eventData["itemData"] = m_Stack->variables;
				}

				eventData["basePrice"] = m_BasePrice;
				eventData["count"] = m_Count;
				eventData["price"] = m_Price;
				eventData["buying"] = m_Buying;

				return eventData;
			}
		}
	}
}
