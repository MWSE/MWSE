#include "LuaMeshLoadEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

namespace mwse::lua::event {
	MeshLoadEvent::MeshLoadEvent(const char* path) :
		GenericEvent("meshLoad"),
		m_Path(path)
	{

	}

	sol::table MeshLoadEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["path"] = m_Path;

		return eventData;
	}

	sol::object MeshLoadEvent::getEventOptions() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto options = state.create_table();
		options["filter"] = m_Path;
		return options;
	}
}
