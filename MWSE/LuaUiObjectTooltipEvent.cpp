#include "LuaUiObjectTooltipEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3ItemData.h"
#include "TES3Reference.h"
#include "TES3UIElement.h"

namespace mwse {
	namespace lua {
		namespace event {
			UiObjectTooltipEvent::UiObjectTooltipEvent(TES3::UI::Element* tooltip, TES3::Object* object, TES3::ItemData* itemData, int count) :
				GenericEvent("uiObjectTooltip"),
				m_Tooltip(tooltip),
				m_Object(object),
				m_ItemData(itemData),
				m_Count(count)
			{

			}

			sol::table UiObjectTooltipEvent::createEventTable() {
				sol::table eventData = LuaManager::getInstance().createTable();

				// If the object is a reference, expose its base object and the reference.
				if (m_Object->objectType == TES3::ObjectType::Reference) {
					eventData["object"] = makeLuaObject(static_cast<TES3::Reference*>(m_Object)->baseObject);
					eventData["reference"] = makeLuaObject(m_Object);
					eventData["itemData"] = static_cast<TES3::Reference*>(m_Object)->getAttachedItemData();
				}
				// Otherwise just expose the object.
				else {
					eventData["object"] = makeLuaObject(m_Object);
					eventData["itemData"] = m_ItemData;
				}

				eventData["tooltip"] = m_Tooltip;
				eventData["count"] = m_Count;

				return eventData;
			}
		}
	}
}
