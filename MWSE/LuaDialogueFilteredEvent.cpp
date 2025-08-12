#include "LuaDialogueFilteredEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Actor.h"
#include "TES3DialogueInfo.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	DialogueFilteredEvent::DialogueFilteredEvent(TES3::Dialogue* dialogue, TES3::DialogueInfo* info, TES3::Object* actor, TES3::Reference* reference, TES3::Dialogue::GetFilteredInfoContext context) :
		ObjectFilteredEvent("dialogueFiltered", dialogue),
		m_Dialogue(dialogue),
		m_DialogueInfo(info),
		m_Actor(actor),
		m_Reference(reference),
		m_Context(context)
	{

	}

	sol::table DialogueFilteredEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["dialogue"] = m_Dialogue;
		eventData["info"] = m_DialogueInfo;
		eventData["actor"] = m_Actor;
		eventData["reference"] = m_Reference;
		eventData["context"] = m_Context;

		return eventData;
	}
}
