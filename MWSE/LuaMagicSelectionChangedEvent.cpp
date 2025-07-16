#include "LuaMagicSelectionChangedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Enchantment.h"
#include "TES3Object.h"
#include "TES3Spell.h"

namespace mwse::lua::event {
	MagicSelectionChangedEvent::MagicSelectionChangedEvent(TES3::Spell* spell) :
		GenericEvent("magicSelectionChanged"),
		isSpell(true), m_Spell(spell), m_Enchantment(nullptr), m_Item(nullptr)
	{
	}

	MagicSelectionChangedEvent::MagicSelectionChangedEvent(TES3::Enchantment* enchantment, TES3::Object* item) :
		GenericEvent("magicSelectionChanged"),
		isSpell(false), m_Spell(nullptr), m_Enchantment(enchantment), m_Item(item)
	{
	}

	sol::table MagicSelectionChangedEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto eventData = stateHandle.getState().create_table();

		if (isSpell) {
			eventData["source"] = m_Spell;
		}
		else {
			eventData["source"] = m_Enchantment;
			eventData["item"] = m_Item;
		}
		return eventData;
	}
}
