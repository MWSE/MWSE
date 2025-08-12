#include "LuaDisarmTrapEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3ItemData.h"
#include "TES3Reference.h"
#include "TES3MobileNPC.h"

namespace mwse::lua::event {
	DisarmTrapEvent::DisarmTrapEvent(TES3::Reference* reference, TES3::LockAttachmentNode* lockData, TES3::MobileNPC* disarmer, TES3::Item* tool, TES3::ItemData* itemData, float chance, bool trapPresent) :
		ObjectFilteredEvent("trapDisarm", reference->baseObject),
		m_Reference(reference),
		m_LockData(lockData),
		m_Disarmer(disarmer),
		m_Tool(tool),
		m_ItemData(itemData),
		m_Chance(chance),
		m_TrapPresent(trapPresent)
	{

	}

	sol::table DisarmTrapEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["reference"] = m_Reference;
		eventData["lockData"] = m_LockData;

		eventData["disarmer"] = m_Disarmer;

		eventData["tool"] = m_Tool;
		eventData["toolItemData"] = m_ItemData;

		eventData["chance"] = m_Chance;
		eventData["trapPresent"] = m_TrapPresent;

		return eventData;
	}
}
