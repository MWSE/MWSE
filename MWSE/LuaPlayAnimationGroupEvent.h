#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse {
	namespace lua {
		namespace event {
			class PlayAnimationGroupEvent : public ObjectFilteredEvent, public DisableableEvent<PlayAnimationGroupEvent> {
			public:
				PlayAnimationGroupEvent(TES3::AnimationData* animationData, int animationGroup, int triIndex, int startFlag, int loopCount);
				sol::table createEventTable();

			protected:
				TES3::AnimationData* m_AnimationData;
				int m_AnimationGroup;
				int m_TriIndex;
				int m_StartFlag;
				int m_LoopCount;
			};
		}
	}
}
