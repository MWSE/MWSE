#include "LuaRemovedEquipmentBodyPartsEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3BodyPartManager.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	RemovedEquipmentBodyPartsEvent::RemovedEquipmentBodyPartsEvent(TES3::BodyPartManager* bodyPartManager) :
		ObjectFilteredEvent("removedEquipmentBodyParts", bodyPartManager->reference),
		m_BodyPartManager(bodyPartManager)
	{

	}

	sol::table RemovedEquipmentBodyPartsEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["reference"] = m_BodyPartManager->reference;

		return eventData;
	}
}
