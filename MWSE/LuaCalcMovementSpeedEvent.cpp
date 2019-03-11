#include "LuaCalcMovementSpeedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			CalculateMovementSpeed::CalculateMovementSpeed(MovementType type, TES3::MobileActor * mobile, float speed) :
				ObjectFilteredEvent(NULL, mobile->reference),
				m_MobileActor(mobile),
				m_Speed(speed)
			{
				// Derive event name from its type.
				switch (type) {
				case Move:
					m_EventName = "calcMoveSpeed";
					break;
				case Walk:
					m_EventName = "calcWalkSpeed";
					break;
				case Run:
					m_EventName = "calcRunSpeed";
					break;
				case Swim:
					m_EventName = "calcSwimSpeed";
					break;
				case SwimRun:
					m_EventName = "calcSwimRunSpeed";
					break;
				case Fly:
					m_EventName = "calcFlySpeed";
					break;
				}
			}

			sol::table CalculateMovementSpeed::createEventTable() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["type"] = m_Type;
				eventData["speed"] = m_Speed;
				eventData["mobile"] = makeLuaObject(m_MobileActor);
				eventData["reference"] = makeLuaObject(m_MobileActor->reference);

				return eventData;
			}
		}
	}
}
