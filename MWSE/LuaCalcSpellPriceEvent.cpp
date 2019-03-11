#include "LuaCalcSpellPriceEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileActor.h"
#include "TES3Reference.h"
#include "TES3Spell.h"

namespace mwse {
	namespace lua {
		namespace event {
			CalculateSpellPriceEvent::CalculateSpellPriceEvent(TES3::MobileActor * mobileActor, int basePrice, int price, TES3::Spell* spell) :
				ObjectFilteredEvent("calcSpellPrice", mobileActor->reference),
				m_MobileActor(mobileActor),
				m_BasePrice(basePrice),
				m_Price(price),
				m_Spell(spell)
			{

			}

			sol::table CalculateSpellPriceEvent::createEventTable() {
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["mobile"] = makeLuaObject(m_MobileActor);
				if (m_MobileActor) {
					eventData["reference"] = makeLuaObject(m_MobileActor->reference);
				}

				eventData["spell"] = makeLuaObject(m_Spell);

				eventData["basePrice"] = m_BasePrice;
				eventData["price"] = m_Price;

				return eventData;
			}
		}
	}
}
