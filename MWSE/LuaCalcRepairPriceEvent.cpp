#include "LuaCalcRepairPriceEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3ItemData.h"
#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			CalculateRepairPriceEvent::CalculateRepairPriceEvent(TES3::MobileActor * mobileActor, int basePrice, int price, TES3::ItemStack* stack) :
				ObjectFilteredEvent("calcRepairPrice", mobileActor->reference),
				m_MobileActor(mobileActor),
				m_BasePrice(basePrice),
				m_Price(price),
				m_Stack(stack)
			{

			}

			sol::table CalculateRepairPriceEvent::createEventTable() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table eventData = state.create_table();

				eventData["mobile"] = m_MobileActor;
				if (m_MobileActor) {
					eventData["reference"] = m_MobileActor->reference;
				}

				if (m_Stack) {
					eventData["item"] = m_Stack->object;
					if (m_Stack->variables->size() > 0) {
						eventData["itemData"] = m_Stack->variables->at(0);
					}
				}

				eventData["basePrice"] = m_BasePrice;
				eventData["price"] = m_Price;

				return eventData;
			}

			bool CalculateRepairPriceEvent::m_EventEnabled = false;
		}
	}
}
