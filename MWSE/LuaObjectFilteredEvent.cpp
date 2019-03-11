#include "LuaObjectFilteredEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Actor.h"
#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			ObjectFilteredEvent::ObjectFilteredEvent(const char* name, TES3::BaseObject* filter) :
				GenericEvent(name),
				m_EventFilter(filter)
			{
				// We always filter on the base-most object.
				if (m_EventFilter != nullptr) {
					// Don't register based on reference, use the base object.
					if (m_EventFilter->objectType == TES3::ObjectType::Reference) {
						m_EventFilter = static_cast<TES3::Reference*>(m_EventFilter)->baseObject;
					}

					// If we were given an actor, get the base actor.
					if (m_EventFilter->objectType == TES3::ObjectType::Container || m_EventFilter->objectType == TES3::ObjectType::Creature || m_EventFilter->objectType == TES3::ObjectType::NPC) {
						auto asActor = static_cast<TES3::Actor*>(m_EventFilter);
						if (!asActor->getActorFlag(TES3::ActorFlag::IsBase)) {
							m_EventFilter = asActor->getBaseActor();
						}
					}
				}
			}

			sol::object ObjectFilteredEvent::getEventOptions() {
				sol::table options = LuaManager::getInstance().createTable();
				options["filter"] = makeLuaObject(m_EventFilter);
				return options;
			}
		}
	}
}
