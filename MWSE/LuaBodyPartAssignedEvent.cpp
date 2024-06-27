#include "LuaBodyPartAssignedEvent.h"

#include "LuaManager.h"

#include "TES3BodyPart.h"
#include "TES3Item.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	BodyPartAssignedEvent::BodyPartAssignedEvent(TES3::BodyPartManager* manager, TES3::Reference* reference, TES3::PhysicalObject* object, TES3::BodyPartManager::ActiveBodyPart::Index index, TES3::BodyPart* bodyPart, bool isFirstPerson) :
		ObjectFilteredEvent("bodyPartAssigned", reference),
		m_BodyPartManager(manager),
		m_Reference(reference),
		m_Object(object),
		m_Index(index),
		m_BodyPart(bodyPart),
		m_IsFirstPerson(isFirstPerson)
	{

	}

	sol::table BodyPartAssignedEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["manager"] = m_BodyPartManager;
		eventData["reference"] = m_Reference;
		eventData["object"] = m_Object;
		eventData["index"] = m_Index;
		eventData["bodyPart"] = m_BodyPart;
		eventData["isFirstPerson"] = m_IsFirstPerson;

		return eventData;
	}
}
