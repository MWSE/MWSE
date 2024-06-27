#include "LuaObjectCreatedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Object.h"

#include "LuaObjectCopiedEvent.h"

namespace mwse::lua::event {
	ObjectCreatedEvent::ObjectCreatedEvent(TES3::BaseObject* object) :
		ObjectFilteredEvent("objectCreated", object),
		m_Object(object)
	{

	}

	sol::table ObjectCreatedEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["object"] = m_Object;
		if (m_Object == ObjectCopiedEvent::ms_LastCopied) {
			eventData["copiedFrom"] = ObjectCopiedEvent::ms_LastCopiedFrom;
		}

		return eventData;
	}
}
