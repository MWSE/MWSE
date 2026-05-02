#include "LuaJumpEvent.h"

#include "LuaManager.h"

#include "TES3MobileActor.h"
#include "TES3Reference.h"
#include "TES3Vectors.h"

namespace mwse::lua::event {
	JumpEvent::JumpEvent(TES3::MobileActor* mobile, TES3::Vector3& velocity, bool applyFatigueCost, bool isDefaultJump) :
		ObjectFilteredEvent("jump", mobile->reference),
		m_MobileActor(mobile),
		m_Velocity(velocity),
		m_ApplyFatigueCost(applyFatigueCost),
		m_IsDefaultJump(isDefaultJump)
	{

	}

	sol::table JumpEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();
		
		eventData["mobile"] = m_MobileActor;
		eventData["reference"] = m_MobileActor->reference;
		eventData["velocity"] = m_Velocity;
		eventData["applyFatigueCost"] = m_ApplyFatigueCost;
		eventData["isDefaultJump"] = m_IsDefaultJump;

		return eventData;
	}
}
