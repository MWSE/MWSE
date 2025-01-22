#include "LuaCalcArmorRatingEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Armor.h"
#include "TES3MobileNPC.h"
#include "TES3NPC.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	CalculateArmorRatingEvent::CalculateArmorRatingEvent(TES3::Armor* armor, TES3::MobileActor* actor) :
		ObjectFilteredEvent("calcArmorRating", armor),
		m_Armor(armor),
		m_Actor(actor),
		m_NPC(nullptr)
	{
		if (actor->actorType == TES3::MobileActorType::NPC) {
			m_NPC = reinterpret_cast<TES3::MobileNPC*>(actor)->npcInstance->baseNPC;
		}
	}

	CalculateArmorRatingEvent::CalculateArmorRatingEvent(TES3::Armor* armor, TES3::NPC* npc) :
		ObjectFilteredEvent("calcArmorRating", armor),
		m_Armor(armor),
		m_Actor(nullptr),
		m_NPC(npc)
	{

	}

	sol::table CalculateArmorRatingEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["armor"] = m_Armor;

		if (m_Actor) {
			eventData["mobile"] = m_Actor;
			eventData["reference"] = m_Actor->reference;
		}

		if (m_NPC) {
			eventData["npc"] = m_NPC;
		}

		return eventData;
	}
}
