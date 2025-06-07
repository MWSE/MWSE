#include "LuaUiSkillTooltipEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3Skill.h"
#include "TES3UIElement.h"
#include "TES3UIMenuController.h"

namespace mwse::lua::event {
	UiSkillTooltipEvent::UiSkillTooltipEvent(TES3::UI::Element* tooltip, int skill, int type) :
		GenericEvent("uiSkillTooltip"),
		m_Tooltip(tooltip),
		m_Skill(skill),
		m_Type(type)
	{

	}

	sol::table UiSkillTooltipEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["tooltip"] = m_Tooltip;
		eventData["skill"] = m_Skill;
		eventData["type"] = m_Type;
		eventData["creator"] = TES3::UI::MenuInputController::lastTooltipSource;

		return eventData;
	}
}
