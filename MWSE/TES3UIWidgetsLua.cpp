#include "TES3UIManager.h"
#include "TES3UIWidgets.h"

#include "LuaManager.h"

namespace mwse::lua {
	using TES3::UI::Element;
	using TES3::UI::Property;
	using TES3::UI::PropertyType;
	using TES3::UI::UI_ID;
	using TES3::UI::WidgetButton;
	using TES3::UI::WidgetFillbar;
	using TES3::UI::WidgetParagraphInput;
	using TES3::UI::WidgetScrollBar;
	using TES3::UI::WidgetScrollPane;
	using TES3::UI::WidgetTextInput;
	using TES3::UI::WidgetTextSelect;
	using TES3::UI::registerID;
	using TES3::UI::registerProperty;

	void bindTES3UIWidgets() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;

		//
		// Button (PartButton)
		//
		{
			auto usertypeDefinition = state.new_usertype<WidgetButton>("tes3uiButton");
			usertypeDefinition["new"] = sol::no_constructor;

			usertypeDefinition["state"] = sol::property(&WidgetButton::getState, &WidgetButton::setState);
			usertypeDefinition["idle"] = sol::property(&WidgetButton::getColourIdle_lua, &WidgetButton::setColourIdle_lua);
			usertypeDefinition["over"] = sol::property(&WidgetButton::getColourOver_lua, &WidgetButton::setColourOver_lua);
			usertypeDefinition["pressed"] = sol::property(&WidgetButton::getColourPressed_lua, &WidgetButton::setColourPressed_lua);
			usertypeDefinition["idleDisabled"] = sol::property(&WidgetButton::getColourDisabled_lua, &WidgetButton::setColourDisabled_lua);
			usertypeDefinition["overDisabled"] = sol::property(&WidgetButton::getColourDisabledOver_lua, &WidgetButton::setColourDisabledOver_lua);
			usertypeDefinition["pressedDisabled"] = sol::property(&WidgetButton::getColourDisabledPressed_lua, &WidgetButton::setColourDisabledPressed_lua);
			usertypeDefinition["idleActive"] = sol::property(&WidgetButton::getColourActive_lua, &WidgetButton::setColourActive_lua);
			usertypeDefinition["overActive"] = sol::property(&WidgetButton::getColourActiveOver_lua, &WidgetButton::setColourActiveOver_lua);
			usertypeDefinition["pressedActive"] = sol::property(&WidgetButton::getColourActivePressed_lua, &WidgetButton::setColourActivePressed_lua);
			usertypeDefinition["element"] = sol::readonly_property(&WidgetButton::getElement);
			usertypeDefinition["textElement"] = sol::readonly_property(&WidgetButton::getTextElement);
		}

		//
		// FillBar (PartFillbar)
		//
		{
			auto usertypeDefinition = state.new_usertype<WidgetFillbar>("tes3uiFillBar");
			usertypeDefinition["new"] = sol::no_constructor;

			usertypeDefinition["current"] = sol::property(&WidgetFillbar::getCurrent, &WidgetFillbar::setCurrent);
			usertypeDefinition["max"] = sol::property(&WidgetFillbar::getMax, &WidgetFillbar::setMax);
			usertypeDefinition["showText"] = sol::property(&WidgetFillbar::getShowText, &WidgetFillbar::setShowText);
			usertypeDefinition["normalized"] = sol::property(&WidgetFillbar::getNormalized, &WidgetFillbar::setNormalized);
			usertypeDefinition["fillColor"] = sol::property(&WidgetFillbar::getFillColour_lua, &WidgetFillbar::setFillColour_lua);
			usertypeDefinition["fillAlpha"] = sol::property(&WidgetFillbar::getFillAlpha, &WidgetFillbar::setFillAlpha);
			usertypeDefinition["element"] = sol::readonly_property(&WidgetFillbar::getElement);
		}

		//
		// ParagraphInput (PartParagraphInput)
		//
		{
			auto usertypeDefinition = state.new_usertype<WidgetParagraphInput>("tes3uiParagraphInput");
			usertypeDefinition["new"] = sol::no_constructor;

			usertypeDefinition["lengthLimit"] = sol::property(&WidgetParagraphInput::getLengthLimit, &WidgetParagraphInput::setLengthLimit_lua);
			usertypeDefinition["element"] = sol::readonly_property(&WidgetParagraphInput::getElement);
		}

		//
		// Slider (PartScrollBar)
		//
		{
			auto usertypeDefinition = state.new_usertype<WidgetScrollBar>("tes3uiSlider");
			usertypeDefinition["new"] = sol::no_constructor;

			usertypeDefinition["current"] = sol::property(&WidgetScrollBar::getCurrent, &WidgetScrollBar::setCurrent);
			usertypeDefinition["max"] = sol::property(&WidgetScrollBar::getMax, &WidgetScrollBar::setMax);
			usertypeDefinition["step"] = sol::property(&WidgetScrollBar::getStepX, &WidgetScrollBar::setStepX);
			usertypeDefinition["jump"] = sol::property(&WidgetScrollBar::getJumpX, &WidgetScrollBar::setJumpX);
			usertypeDefinition["element"] = sol::readonly_property(&WidgetScrollBar::getElement);
		}

		//
		// ScrollPane (PartScrollPane)
		//
		{
			auto usertypeDefinition = state.new_usertype<WidgetScrollPane>("tes3uiScrollPane");
			usertypeDefinition["new"] = sol::no_constructor;

			usertypeDefinition["contentsChanged"] = &WidgetScrollPane::contentPaneChanged;
			usertypeDefinition["positionX"] = sol::property(&WidgetScrollPane::getHorizontalPos, &WidgetScrollPane::setHorizontalPos);
			usertypeDefinition["positionY"] = sol::property(&WidgetScrollPane::getVerticalPos, &WidgetScrollPane::setVerticalPos);
			usertypeDefinition["scrollbarVisible"] = sol::property(&WidgetScrollPane::getScrollbarVisible, &WidgetScrollPane::setScrollbarVisible);
			usertypeDefinition["element"] = sol::readonly_property(&WidgetScrollPane::getElement);

			// Quick access to the pane contents.
			usertypeDefinition["contentPane"] = sol::readonly_property(&WidgetScrollPane::getContentPane);


		}

		//
		// TextInput (PartTextInput)
		//
		{
			auto usertypeDefinition = state.new_usertype<WidgetTextInput>("tes3uiTextInput");
			usertypeDefinition["new"] = sol::no_constructor;

			usertypeDefinition["lengthLimit"] = sol::property(&WidgetTextInput::getLengthLimit_lua, &WidgetTextInput::setLengthLimit_lua);
			usertypeDefinition["eraseOnFirstKey"] = sol::property(&WidgetTextInput::getEraseOnFirstKey, &WidgetTextInput::setEraseOnFirstKey);
			usertypeDefinition["element"] = sol::readonly_property(&WidgetTextInput::getElement);
		}

		//
		// TextSelect (PartTextSelect)
		//
		{
			auto usertypeDefinition = state.new_usertype<WidgetTextSelect>("tes3uiTextSelect");
			usertypeDefinition["new"] = sol::no_constructor;

			usertypeDefinition["state"] = sol::property(&WidgetTextSelect::getState, &WidgetTextSelect::setState);
			usertypeDefinition["idle"] = sol::property(&WidgetTextSelect::getColourIdle_lua, &WidgetTextSelect::setColourIdle_lua);
			usertypeDefinition["over"] = sol::property(&WidgetTextSelect::getColourOver_lua, &WidgetTextSelect::setColourOver_lua);
			usertypeDefinition["pressed"] = sol::property(&WidgetTextSelect::getColourPressed_lua, &WidgetTextSelect::setColourPressed_lua);
			usertypeDefinition["idleDisabled"] = sol::property(&WidgetTextSelect::getColourDisabled_lua, &WidgetTextSelect::setColourDisabled_lua);
			usertypeDefinition["overDisabled"] = sol::property(&WidgetTextSelect::getColourDisabledOver_lua, &WidgetTextSelect::setColourDisabledOver_lua);
			usertypeDefinition["pressedDisabled"] = sol::property(&WidgetTextSelect::getColourDisabledPressed_lua, &WidgetTextSelect::setColourDisabledPressed_lua);
			usertypeDefinition["idleActive"] = sol::property(&WidgetTextSelect::getColourActive_lua, &WidgetTextSelect::setColourActive_lua);
			usertypeDefinition["overActive"] = sol::property(&WidgetTextSelect::getColourActiveOver_lua, &WidgetTextSelect::setColourActiveOver_lua);
			usertypeDefinition["pressedActive"] = sol::property(&WidgetTextSelect::getColourActivePressed_lua, &WidgetTextSelect::setColourActivePressed_lua);
			usertypeDefinition["element"] = sol::readonly_property(&WidgetTextSelect::getElement);
		}
	}
}