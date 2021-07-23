#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse {
	namespace lua {
		namespace event {
			class ReferenceSceneNodeCreatedEvent : public ObjectFilteredEvent, public DisableableEvent<ReferenceSceneNodeCreatedEvent> {
			public:
				ReferenceSceneNodeCreatedEvent(TES3::Reference* reference);
				sol::table createEventTable();

			protected:
				TES3::Reference* m_Reference;
			};
		}
	}
}
