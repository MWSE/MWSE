#include "LuaVFXCreatedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3VFXManager.h"

namespace mwse::lua::event {
	VFXCreatedEvent::VFXCreatedEvent(TES3::VFX* vfx, const char* context) :
		ObjectFilteredEvent("vfxCreated", vfx->effectObject),
		m_VFX(vfx),
		m_Context(context)
	{

	}

	sol::table VFXCreatedEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["vfx"] = m_VFX;
		eventData["context"] = m_Context;

		return eventData;
	}
}
