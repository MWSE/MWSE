#include "LuaEnchantChargeUseEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Enchantment.h"
#include "TES3ItemData.h"
#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	EnchantChargeUseEvent::EnchantChargeUseEvent(TES3::Enchantment* enchant, TES3::MobileActor* caster, TES3::MagicSourceInstance* magicSource, float chargeRequired) :
		GenericEvent("enchantChargeUse"),
		m_Enchant(enchant),
		m_Caster(caster),
		m_MagicSource(magicSource),
		m_ChargeRequired(chargeRequired)
	{

	}

	sol::table EnchantChargeUseEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["caster"] = m_Caster->reference;
		eventData["source"] = m_Enchant;
		eventData["charge"] = m_ChargeRequired;
		eventData["isCast"] = m_MagicSource != nullptr;

		if (m_MagicSource) {
			eventData["sourceInstance"] = m_MagicSource;
			eventData["item"] = m_MagicSource->castingItem;
			eventData["itemData"] = m_MagicSource->castingItemCondition;
		}

		return eventData;
	}

	sol::object EnchantChargeUseEvent::getEventOptions() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto options = state.create_table();

		options["filter"] = m_Caster->reference;

		return options;
	}
}
