#include "LuaMeshLoadedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

namespace mwse::lua::event {
	MeshLoadedEvent::MeshLoadedEvent(const char* path, NI::AVObject* mesh) :
		GenericEvent("meshLoaded"),
		m_Path(path),
		m_Mesh(mesh)
	{

	}

	sol::table MeshLoadedEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["path"] = m_Path;
		eventData["node"] = m_Mesh;

		return eventData;
	}

	sol::object MeshLoadedEvent::getEventOptions() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto options = state.create_table();
		options["filter"] = m_Path;
		return options;
	}
}
