#include "LuaInfoFilterEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Actor.h"
#include "TES3DialogueInfo.h"
#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			InfoFilterEvent::InfoFilterEvent(TES3::DialogueInfo * info, TES3::Object * actor, TES3::Reference * reference, int source, TES3::Dialogue * dialogue, bool passes) :
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
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["info"] = makeLuaObject(m_DialogueInfo);
				eventData["actor"] = makeLuaObject(m_Actor);
				eventData["reference"] = makeLuaObject(m_Reference);
				eventData["source"] = m_Source;
				eventData["dialogue"] = makeLuaObject(m_Dialogue);
				eventData["passes"] = m_Passes;

				return eventData;
			}
		}
	}
}
