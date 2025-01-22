#include "LuaBarterOfferEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileActor.h"
#include "TES3Reference.h"
#include "TES3UIElement.h"
#include "TES3UIInventoryTile.h"
#include "TES3UIManager.h"
#include "TES3UIWidgets.h"

namespace mwse::lua::event {
	BarterOfferEvent::BarterOfferEvent(TES3::MobileActor* mobileActor, bool success, int value, int offer) :
		GenericEvent("barterOffer"),
		m_MobileActor(mobileActor),
		m_Success(success),
		m_Value(value),
		m_Offer(offer)
	{

	}

	sol::table getBuySellTable(const char* baseId) {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		sol::table results = state.create_table();

		std::string scrollpaneId = baseId;
		scrollpaneId += "_scrollpane";

		std::string thingId = baseId;
		thingId += "_Thing";

		auto menu = TES3::UI::findMenu(baseId);
		if (menu) {
			auto thingyProperty = TES3::UI::registerProperty(thingId.c_str());
			auto contents = static_cast<TES3::UI::WidgetScrollPane*>(menu->findChild(scrollpaneId.c_str()))->getContentPane();
			for (auto columnItt = contents->vectorChildren.first; columnItt != contents->vectorChildren.nextEmpty; ++columnItt) {
				for (auto tileItt = (*columnItt)->vectorChildren.first; tileItt != (*columnItt)->vectorChildren.nextEmpty; ++tileItt) {
					auto tile = reinterpret_cast<TES3::UI::InventoryTile*>((*tileItt)->getProperty(TES3::UI::PropertyType::Pointer, thingyProperty).ptrValue);
					if (!tile) {
						continue;
					}

					if (tile->getIsBartered()) {
						results.add(tile);
					}
				}
			}
		}

		return results;
	}

	sol::table BarterOfferEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["mobile"] = m_MobileActor;
		eventData["success"] = m_Success;
		eventData["value"] = m_Value;
		eventData["offer"] = m_Offer;
		eventData["selling"] = getBuySellTable("MenuBarter");
		eventData["buying"] = getBuySellTable("MenuInventory");

		return eventData;
	}
}
