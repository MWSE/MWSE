#include "LuaEnchantedItemCreateFailedEvent.h"

#include "LuaManager.h"
#include "TES3UIManager.h"
#include "LuaUtil.h"

#include "TES3Actor.h"
#include "TES3Item.h"
#include "TES3Misc.h"
#include "TES3MobilePlayer.h"
#include "TES3Reference.h"
#include "TES3WorldController.h"

namespace mwse::lua::event {
	EnchantedItemCreateFailedEvent::EnchantedItemCreateFailedEvent(TES3::Item* baseItem, TES3::Misc* soulGem, TES3::Actor* soul) :
		GenericEvent("enchantedItemCreateFailed"),
		m_EnchantedFrom(baseItem),
		m_SoulGemUsed(soulGem),
		m_SoulUsed(soul)
	{

	}

	sol::table EnchantedItemCreateFailedEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		sol::state& state = stateHandle.state;
		sol::table eventData = state.create_table();

		eventData["baseObject"] = m_EnchantedFrom;
		eventData["soul"] = m_SoulUsed;
		eventData["soulGem"] = m_SoulGemUsed;

		// Get the enchanter.
		TES3::MobileActor* enchanter = nullptr;
		if (*reinterpret_cast<bool*>(0x7D36F0)) {
			enchanter = TES3::UI::getServiceActor();
		}
		else {
			enchanter = TES3::WorldController::get()->getMobilePlayer();
		}
		eventData["enchanter"] = enchanter;
		eventData["enchanterReference"] = enchanter->reference;

		return eventData;
	}

	bool EnchantedItemCreateFailedEvent::m_EventEnabled = false;
}