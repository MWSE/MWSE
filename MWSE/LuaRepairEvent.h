#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse::lua::event {
	class RepairEvent : public ObjectFilteredEvent, public DisableableEvent<RepairEvent> {
	public:
		RepairEvent(TES3::MobileActor* repairer, TES3::Item* item, TES3::ItemData* itemData, TES3::RepairTool* tool, TES3::ItemData* toolData, int roll, float chance, int repairAmount);
		sol::table createEventTable();

	protected:
		TES3::MobileActor* m_Repairer;
		TES3::Item* m_Item;
		TES3::ItemData* m_ItemData;
		TES3::RepairTool* m_Tool;
		TES3::ItemData* m_ToolData;
		int m_Roll;
		float m_Chance;
		int m_RepairAmount;
	};
}
