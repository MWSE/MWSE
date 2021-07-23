#include "LuaMeshLoadedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

namespace mwse {
	namespace lua {
		namespace event {
			MeshLoadedEvent::MeshLoadedEvent(const char* path, NI::AVObject * mesh) :
				GenericEvent("meshLoaded"),
				m_Path(path),
				m_Mesh(mesh)
			{

			}

			sol::table MeshLoadedEvent::createEventTable() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table eventData = state.create_table();

				eventData["path"] = m_Path;
				eventData["node"] = m_Mesh;

				return eventData;
			}

			sol::object MeshLoadedEvent::getEventOptions() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table options = state.create_table();
				options["filter"] = m_Path;
				return options;
			}

			bool MeshLoadedEvent::m_EventEnabled = false;
		}
	}
}
