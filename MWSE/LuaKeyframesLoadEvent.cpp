#include "LuaKeyframesLoadEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

namespace mwse::lua::event {
	KeyframesLoadEvent::KeyframesLoadEvent(const char* path, const char* sequenceName) :
		GenericEvent("keyframesLoad"),
		m_Path(path),
		m_SequenceName(sequenceName)
	{

	}

	sol::table KeyframesLoadEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["path"] = m_Path;
		eventData["sequenceName"] = m_SequenceName;

		return eventData;
	}

	sol::object KeyframesLoadEvent::getEventOptions() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto options = state.create_table();

		options["filter"] = m_Path;

		return options;
	}
}
