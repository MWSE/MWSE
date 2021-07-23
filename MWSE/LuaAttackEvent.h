#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse {
	namespace lua {
		namespace event {
			class AttackEvent : public ObjectFilteredEvent, public DisableableEvent<AttackEvent> {
			public:
				AttackEvent(TES3::ActorAnimationController* animController);
				sol::table createEventTable();

			protected:
				TES3::ActorAnimationController* m_AnimationController;
			};
		}
	}
}
