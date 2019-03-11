#include "LuaMagicCastedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MagicSourceInstance.h"
#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			MagicCastedEvent::MagicCastedEvent(TES3::MagicSourceInstance* magicInstance) :
				GenericEvent("magicCasted"),
				m_MagicSourceInstance(magicInstance)
			{

			}

			sol::table MagicCastedEvent::createEventTable() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["caster"] = makeLuaObject(m_MagicSourceInstance->caster);
				eventData["target"] = makeLuaObject(m_MagicSourceInstance->target);
				eventData["source"] = makeLuaObject(m_MagicSourceInstance->sourceCombo.source.asGeneric);
				eventData["sourceInstance"] = makeLuaObject(m_MagicSourceInstance);

				return eventData;
			}

			sol::object MagicCastedEvent::getEventOptions() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table options = LuaManager::getInstance().createTable();

				options["filter"] = makeLuaObject(m_MagicSourceInstance->sourceCombo.source.asGeneric);
				return options;
			}
		}
	}
}
