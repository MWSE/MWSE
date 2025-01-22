#include "LuaInfoFilterEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Actor.h"
#include "TES3DialogueInfo.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	InfoFilterEvent::InfoFilterEvent(TES3::DialogueInfo* info, TES3::Object* actor, TES3::Reference* reference, int source, TES3::Dialogue* dialogue, bool passes) :
		ObjectFilteredEvent("infoFilter", info),
		m_DialogueInfo(info),
		m_Actor(actor),
		m_Reference(reference),
		m_Source(source),
		m_Dialogue(dialogue),
		m_Passes(passes)
	{

	}

	sol::table InfoFilterEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["info"] = m_DialogueInfo;
		eventData["actor"] = m_Actor;
		eventData["reference"] = m_Reference;
		eventData["source"] = m_Source;
		eventData["dialogue"] = m_Dialogue;
		eventData["passes"] = m_Passes;

		return eventData;
	}
}
