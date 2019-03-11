#include "LuaCalcArmorRatingEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Armor.h"
#include "TES3MobileNPC.h"
#include "TES3NPC.h"
#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			CalculateArmorRatingEvent::CalculateArmorRatingEvent(TES3::Armor * armor, TES3::MobileActor * actor) :
				ObjectFilteredEvent("calcArmorRating", armor),
				m_Armor(armor),
				m_Actor(actor),
				m_NPC(nullptr)
			{
				if (actor->actorType == TES3::MobileActorType::NPC) {
					m_NPC = reinterpret_cast<TES3::MobileNPC*>(actor)->npcInstance->baseNPC;
				}
			}

			CalculateArmorRatingEvent::CalculateArmorRatingEvent(TES3::Armor * armor, TES3::NPC * npc) :
				ObjectFilteredEvent("calcArmorRating", armor),
				m_Armor(armor),
				m_Actor(nullptr),
				m_NPC(npc)
			{

			}

			sol::table CalculateArmorRatingEvent::createEventTable() {
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["armor"] = makeLuaObject(m_Armor);

				if (m_Actor) {
					eventData["mobile"] = makeLuaObject(m_Actor);
					eventData["reference"] = makeLuaObject(m_Actor->reference);
				}
				
				if (m_NPC) {
					eventData["npc"] = makeLuaObject(m_NPC);
				}

				return eventData;
			}
		}
	}
}
