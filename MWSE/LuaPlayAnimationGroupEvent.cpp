#include "LuaPlayAnimationGroupEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3AnimationData.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	PlayAnimationGroupEvent::PlayAnimationGroupEvent(TES3::AnimationData* animationData, int animationGroup, int bodySection, int startFlag, int loopCount) :
		ObjectFilteredEvent("playGroup", animationData->getReference()),
		m_AnimationData(animationData),
		m_AnimationGroup(animationGroup),
		m_BodySection(bodySection),
		m_StartFlag(startFlag),
		m_LoopCount(loopCount)
	{

	}

	sol::table PlayAnimationGroupEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["animationData"] = m_AnimationData;
		eventData["reference"] = m_AnimationData->getReference();
		eventData["currentGroup"] = m_AnimationData->currentAnimGroups[m_BodySection];
		eventData["group"] = m_AnimationGroup;
		eventData["index"] = m_BodySection;
		eventData["flags"] = m_StartFlag;
		eventData["loopCount"] = m_LoopCount;

		return eventData;
	}
}
