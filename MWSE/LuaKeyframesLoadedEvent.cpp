#include "LuaKeyframesLoadedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3ActorAnimationController.h"

#include "NIKeyframeManager.h"

namespace mwse::lua::event {
	KeyframesLoadedEvent::KeyframesLoadedEvent(const char* path, const char* sequenceName, TES3::KeyframeDefinition* keyframeDefinition) :
		GenericEvent("keyframesLoaded"),
		m_Path(path),
		m_SequenceName(sequenceName),
		m_KeyframeDefinition(keyframeDefinition)
	{
	}

	sol::table KeyframesLoadedEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["path"] = m_Path;
		eventData["sequenceName"] = m_SequenceName;
		// eventData["keyframeDefinition"] = m_KeyframeDefinition;

		auto sequence = m_KeyframeDefinition->sequences[0];
		if (sequence) {
			eventData["textKeys"] = sequence->textKeys;
		}

		return eventData;
	}

	sol::object KeyframesLoadedEvent::getEventOptions() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto options = state.create_table();
		options["filter"] = m_Path;
		return options;
	}
}
