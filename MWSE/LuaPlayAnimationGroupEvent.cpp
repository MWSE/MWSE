#include "LuaPlayAnimationGroupEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3AnimationData.h"
#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			PlayAnimationGroupEvent::PlayAnimationGroupEvent(TES3::AnimationData* animationData, int animationGroup, int triIndex, int startFlag, int loopCount) :
				ObjectFilteredEvent("playGroup", animationData->getReference()),
				m_AnimationData(animationData),
				m_AnimationGroup(animationGroup),
				m_TriIndex(triIndex),
				m_StartFlag(startFlag),
				m_LoopCount(loopCount)
			{

			}

			sol::table PlayAnimationGroupEvent::createEventTable() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table eventData = state.create_table();

				eventData["animationData"] = m_AnimationData;
				eventData["reference"] = m_AnimationData->getReference();
				eventData["group"] = m_AnimationGroup;
				eventData["index"] = m_TriIndex;
				eventData["flags"] = m_StartFlag;
				eventData["loopCount"] = m_LoopCount;

				return eventData;
			}

			bool PlayAnimationGroupEvent::m_EventEnabled = false;
		}
	}
}
