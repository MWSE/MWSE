#include "LuaRepairEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3ItemData.h"
#include "TES3Reference.h"
#include "TES3RepairTool.h"
#include "TES3MobileActor.h"

namespace mwse::lua::event {
	RepairEvent::RepairEvent(TES3::MobileActor* repairer, TES3::Item* item, TES3::ItemData* itemData, TES3::RepairTool* tool, TES3::ItemData* toolData, float chance, int repairAmount) :
		ObjectFilteredEvent("repair", item),
		m_Repairer(repairer),
		m_Item(item),
		m_ItemData(itemData),
		m_Tool(tool),
		m_ToolData(toolData),
		m_Chance(chance),
		m_RepairAmount(repairAmount)
	{

	}

	sol::table RepairEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["repairer"] = m_Repairer;

		eventData["item"] = m_Item;
		eventData["itemData"] = m_ItemData;

		eventData["tool"] = m_Tool;
		eventData["toolItemData"] = m_ToolData;

		eventData["chance"] = m_Chance;
		eventData["repairAmount"] = m_RepairAmount;

		return eventData;
	}
}
