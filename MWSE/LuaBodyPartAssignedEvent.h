#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

#include "TES3BodyPartManager.h"

namespace mwse {
	namespace lua {
		namespace event {
			class BodyPartAssignedEvent : public ObjectFilteredEvent, public DisableableEvent<BodyPartAssignedEvent> {
			public:
				BodyPartAssignedEvent(TES3::BodyPartManager* manager, TES3::Reference* reference, TES3::PhysicalObject* object, TES3::BodyPartManager::ActiveBodyPart::Index index, TES3::BodyPart* bodyPart, bool isFirstPerson);
				sol::table createEventTable();

			protected:
				TES3::BodyPartManager* m_BodyPartManager;
				TES3::Reference* m_Reference;
				TES3::PhysicalObject* m_Object;
				TES3::BodyPartManager::ActiveBodyPart::Index m_Index;
				TES3::BodyPart* m_BodyPart;
				bool m_IsFirstPerson;
			};
		}
	}
}
