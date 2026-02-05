#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse::lua::event {
	class PlayAnimationGroupEvent : public ObjectFilteredEvent, public DisableableEvent<PlayAnimationGroupEvent> {
	public:
		PlayAnimationGroupEvent(TES3::AnimationData* animationData, int animationGroup, int bodySection, int startFlag, int loopCount);
		sol::table createEventTable();

	protected:
		TES3::AnimationData* m_AnimationData;
		int m_AnimationGroup;
		int m_BodySection;
		int m_StartFlag;
		int m_LoopCount;
	};
}
