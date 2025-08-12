#include "LuaActivationTargetChangedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Reference.h"

namespace mwse::lua::event {
	TES3::Reference* ActivationTargetChangedEvent::ms_PreviousReference = nullptr;

	ActivationTargetChangedEvent::ActivationTargetChangedEvent(TES3::Reference* current) :
		ObjectFilteredEvent("activationTargetChanged", current),
		m_CurrentReference(current)
	{

	}

	sol::table ActivationTargetChangedEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["previous"] = ms_PreviousReference;
		eventData["current"] = m_CurrentReference;

		return eventData;
	}
}
