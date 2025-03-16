#include "LuaUpdateBodyPartsForItemEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3BodyPartManager.h"
#include "TES3Item.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	UpdateBodyPartsForItemEvent::UpdateBodyPartsForItemEvent(TES3::Item* item, TES3::BodyPartManager* bodyPartManager, bool isFemale, bool isFirstPerson) :
		ObjectFilteredEvent("updateBodyPartsForItem", item),
		m_Item(item),
		m_BodyPartManager(bodyPartManager),
		m_IsFemale(isFemale),
		m_IsFirstPerson(isFirstPerson)
	{

	}

	sol::table UpdateBodyPartsForItemEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["item"] = m_Item;
		eventData["bodyPartManager"] = m_BodyPartManager;
		eventData["reference"] = m_BodyPartManager->reference;
		eventData["isFemale"] = m_IsFemale;
		eventData["isFirstPerson"] = m_IsFirstPerson;

		return eventData;
	}
}
