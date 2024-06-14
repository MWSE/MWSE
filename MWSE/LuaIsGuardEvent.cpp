#include "LuaIsGuardEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3NPC.h"

namespace mwse::lua::event {
	IsGuardEvent::IsGuardEvent(TES3::NPCBase* npc, bool isGuard) :
		GenericEvent("isGuard"),
		m_NpcBase(npc),
		m_IsGuard(isGuard)
	{

	}

	sol::table IsGuardEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["npc"] = m_NpcBase;
		eventData["isGuard"] = m_IsGuard;

		return eventData;
	}
}
