#include "LuaUiSpellTooltipEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Spell.h"
#include "TES3UIElement.h"

namespace mwse {
	namespace lua {
		namespace event {
			UiSpellTooltipEvent::UiSpellTooltipEvent(TES3::UI::Element* tooltip, TES3::Spell* spell) :
				ObjectFilteredEvent("uiSpellTooltip", spell),
				m_Tooltip(tooltip),
				m_Spell(spell)
			{

			}

			sol::table UiSpellTooltipEvent::createEventTable() {
				sol::table eventData = LuaManager::getInstance().createTable();

				eventData["tooltip"] = m_Tooltip;
				eventData["spell"] = makeLuaObject(m_Spell);

				return eventData;
			}
		}
	}
}
