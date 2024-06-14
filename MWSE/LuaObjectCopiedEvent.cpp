#include "LuaObjectCopiedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Object.h"

namespace mwse::lua::event {
	ObjectCopiedEvent::ObjectCopiedEvent(TES3::Object* copy, TES3::Object* original) :
		ObjectFilteredEvent("objectCopied", original),
		m_Copy(copy),
		m_Original(original)
	{

	}

	sol::table ObjectCopiedEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["copy"] = m_Copy;
		eventData["original"] = m_Original;

		return eventData;
	}
	TES3::Object* ObjectCopiedEvent::ms_LastCopied = nullptr;
	TES3::Object* ObjectCopiedEvent::ms_LastCopiedFrom = nullptr;
}
