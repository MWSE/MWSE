#include "LuaDispositionEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3NPC.h"
#include "TES3Reference.h"
#include "TES3MobileActor.h"

namespace mwse::lua::event {
	DispositionEvent::DispositionEvent(TES3::NPCInstance* npc, int disposition, bool clamp) :
		ObjectFilteredEvent("disposition", npc->baseNPC),
		m_NPC(npc),
		m_Disposition(disposition),
		m_Clamp(clamp)
	{

	}

	sol::table DispositionEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		// Make sure to handle the case where the mobile isn't available.
		auto mobile = m_NPC->getMobile();
		auto reference = m_NPC->getReference();

		eventData["mobile"] = mobile;
		eventData["reference"] = reference;

		eventData["disposition"] = m_Disposition;
		eventData["clamped"] = m_Clamp;

		return eventData;
	}
}
