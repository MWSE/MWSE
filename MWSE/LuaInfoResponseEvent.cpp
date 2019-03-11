#include "LuaInfoResponseEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Dialogue.h"
#include "TES3DialogueInfo.h"
#include "TES3Reference.h"
#include "TES3Script.h"

namespace mwse {
	namespace lua {
		namespace event {
			InfoResponseEvent::InfoResponseEvent(const char* command, TES3::Reference * reference, TES3::ScriptVariables * variables, TES3::Dialogue * dialogue, TES3::DialogueInfo * info) :
				ObjectFilteredEvent("infoResponse", info),
				m_Command(command),
				m_Reference(reference),
				m_ScriptVariables(variables),
				m_Dialogue(dialogue),
				m_DialogueInfo(info)
			{

			}

			sol::table InfoResponseEvent::createEventTable() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["command"] = m_Command;
				eventData["reference"] = makeLuaObject(m_Reference);
				eventData["variables"] = m_ScriptVariables;
				eventData["dialogue"] = makeLuaObject(m_Dialogue);
				eventData["info"] = makeLuaObject(m_DialogueInfo);

				return eventData;
			}
		}
	}
}
