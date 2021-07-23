#include "LuaConvertReferenceToItemEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			ConvertReferenceToItemEvent::ConvertReferenceToItemEvent(TES3::Reference * reference) :
				ObjectFilteredEvent("convertReferenceToItem", reference->baseObject),
				m_Reference(reference)
			{

			}

			sol::table ConvertReferenceToItemEvent::createEventTable() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table eventData = state.create_table();

				eventData["reference"] = m_Reference;

				return eventData;
			}

			bool ConvertReferenceToItemEvent::m_EventEnabled = false;
		}
	}
}
