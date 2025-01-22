#include "LuaPickLockEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3ItemData.h"
#include "TES3Reference.h"
#include "TES3MobileNPC.h"

namespace mwse::lua::event {
	PickLockEvent::PickLockEvent(TES3::Reference* reference, TES3::LockAttachmentNode* lockData, TES3::MobileNPC* picker, TES3::Item* tool, TES3::ItemData* itemData, float chance, bool lockPresent) :
		ObjectFilteredEvent("lockPick", reference->baseObject),
		m_Reference(reference),
		m_LockData(lockData),
		m_Picker(picker),
		m_Tool(tool),
		m_ItemData(itemData),
		m_Chance(chance),
		m_LockPresent(lockPresent)
	{

	}

	sol::table PickLockEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["reference"] = m_Reference;
		eventData["lockData"] = m_LockData;

		eventData["picker"] = m_Picker;

		eventData["tool"] = m_Tool;
		eventData["toolItemData"] = m_ItemData;

		eventData["chance"] = m_Chance;
		eventData["lockPresent"] = m_LockPresent;

		return eventData;
	}
}
