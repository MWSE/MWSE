#include "TES3UIWidgets.h"

#include "TES3UIManager.h"

namespace TES3 {
	namespace UI {
		static void initColourProperty(Property(&prop)[4], const char *prefix) {
			std::string name(prefix);
			std::string::size_type n = name.size();
			name.reserve(n + 6);

			name.replace(name.begin() + n, name.end(), "_red");
			prop[0] = registerProperty(name.c_str());
			name.replace(name.begin() + n, name.end(), "_green");
			prop[1] = registerProperty(name.c_str());
			name.replace(name.begin() + n, name.end(), "_blue");
			prop[2] = registerProperty(name.c_str());
			name.replace(name.begin() + n, name.end(), "_alpha");
			prop[3] = registerProperty(name.c_str());
		}

		static sol::table getColourProperty(Element& e, Property(&prop)[4], sol::state_view state) {
			return state.create_table_with(
				1, e.getProperty(PropertyType::Float, prop[0]).floatValue,
				2, e.getProperty(PropertyType::Float, prop[1]).floatValue,
				3, e.getProperty(PropertyType::Float, prop[2]).floatValue,
				4, e.getProperty(PropertyType::Float, prop[3]).floatValue
			);
		}

		static void setColourProperty(Element& e, Property(&prop)[4], const float(&c)[3]) {
			e.setProperty(prop[0], c[0]);
			e.setProperty(prop[1], c[1]);
			e.setProperty(prop[2], c[2]);
		}

		static void setColourProperty(Element& e, Property(&prop)[4], const sol::table c) {
			e.setProperty(prop[0], c.get_or(1, 1.0f));
			e.setProperty(prop[1], c.get_or(2, 1.0f));
			e.setProperty(prop[2], c.get_or(3, 1.0f));
			e.setProperty(prop[3], c.get_or(4, 1.0f));
		}

		//
		// WidgetButton
		//
		static Property propButtonState;
		static Property propButtonIdle[4], propButtonOver[4], propButtonPressed[4];
		static Property propButtonDisabled[4], propButtonDisabledOver[4], propButtonDisabledPressed[4];
		static Property propButtonActive[4], propButtonActiveOver[4], propButtonActivePressed[4];
		static UI_ID uiidButtonText;

		const auto TES3_WidgetButton_updateStateColor = reinterpret_cast<bool(__cdecl*)(WidgetButton*)>(0x639E60);

		bool WidgetButton::initProperties() {
			propButtonState = registerProperty("PartButton_state");
			initColourProperty(propButtonIdle, "PartButton_idle");
			initColourProperty(propButtonOver, "PartButton_over");
			initColourProperty(propButtonPressed, "PartButton_pressed");
			initColourProperty(propButtonDisabled, "PartButton_disabled");
			initColourProperty(propButtonDisabledOver, "PartButton_disabled_over");
			initColourProperty(propButtonDisabledPressed, "PartButton_disabled_pressed");
			initColourProperty(propButtonActive, "PartButton_active");
			initColourProperty(propButtonActiveOver, "PartButton_active_over");
			initColourProperty(propButtonActivePressed, "PartButton_active_pressed");
			uiidButtonText = registerID("PartButton_text_ptr");
			return true;
		}

		WidgetButton* WidgetButton::fromElement(Element* element) {
			static bool initialized = initProperties();
			return static_cast<WidgetButton*>(element);
		}

		const WidgetButton* WidgetButton::fromElement(const Element* element) {
			static bool initialized = initProperties();
			return static_cast<const WidgetButton*>(element);
		}

		int WidgetButton::getState() const {
			return getProperty(PropertyType::Integer, propButtonState).integerValue;
		}
		void WidgetButton::setState(int state) {
			setProperty(propButtonState, state);
			TES3_WidgetButton_updateStateColor(this);
		}

		sol::table WidgetButton::getColourIdle_lua(sol::this_state ts) { return getColourProperty(*this, propButtonIdle, ts); }
		void WidgetButton::setColourIdle(const float(&c)[3]) { setColourProperty(*this, propButtonIdle, c); }
		void WidgetButton::setColourIdle_lua(sol::table c) { setColourProperty(*this, propButtonIdle, c); }
		sol::table WidgetButton::getColourOver_lua(sol::this_state ts) { return getColourProperty(*this, propButtonOver, ts); }
		void WidgetButton::setColourOver(const float(&c)[3]) { setColourProperty(*this, propButtonOver, c); }
		void WidgetButton::setColourOver_lua(sol::table c) { setColourProperty(*this, propButtonOver, c); }
		sol::table WidgetButton::getColourPressed_lua(sol::this_state ts) { return getColourProperty(*this, propButtonPressed, ts); }
		void WidgetButton::setColourPressed(const float(&c)[3]) { setColourProperty(*this, propButtonPressed, c); }
		void WidgetButton::setColourPressed_lua(sol::table c) { setColourProperty(*this, propButtonPressed, c); }
		sol::table WidgetButton::getColourDisabled_lua(sol::this_state ts) { return getColourProperty(*this, propButtonDisabled, ts); }
		void WidgetButton::setColourDisabled(const float(&c)[3]) { setColourProperty(*this, propButtonDisabled, c); }
		void WidgetButton::setColourDisabled_lua(sol::table c) { setColourProperty(*this, propButtonDisabled, c); }
		sol::table WidgetButton::getColourDisabledOver_lua(sol::this_state ts) { return getColourProperty(*this, propButtonDisabledOver, ts); }
		void WidgetButton::setColourDisabledOver(const float(&c)[3]) { setColourProperty(*this, propButtonDisabledOver, c); }
		void WidgetButton::setColourDisabledOver_lua(sol::table c) { setColourProperty(*this, propButtonDisabledOver, c); }
		sol::table WidgetButton::getColourDisabledPressed_lua(sol::this_state ts) { return getColourProperty(*this, propButtonDisabledPressed, ts); }
		void WidgetButton::setColourDisabledPressed(const float(&c)[3]) { setColourProperty(*this, propButtonDisabledPressed, c); }
		void WidgetButton::setColourDisabledPressed_lua(sol::table c) { setColourProperty(*this, propButtonDisabledPressed, c); }
		sol::table WidgetButton::getColourActive_lua(sol::this_state ts) { return getColourProperty(*this, propButtonActive, ts); }
		void WidgetButton::setColourActive(const float(&c)[3]) { setColourProperty(*this, propButtonActive, c); }
		void WidgetButton::setColourActive_lua(sol::table c) { setColourProperty(*this, propButtonActive, c); }
		sol::table WidgetButton::getColourActiveOver_lua(sol::this_state ts) { return getColourProperty(*this, propButtonActiveOver, ts); }
		void WidgetButton::setColourActiveOver(const float(&c)[3]) { setColourProperty(*this, propButtonActiveOver, c); }
		void WidgetButton::setColourActiveOver_lua(sol::table c) { setColourProperty(*this, propButtonActiveOver, c); }
		sol::table WidgetButton::getColourActivePressed_lua(sol::this_state ts) { return getColourProperty(*this, propButtonActivePressed, ts); }
		void WidgetButton::setColourActivePressed(const float(&c)[3]) { setColourProperty(*this, propButtonActivePressed, c); }
		void WidgetButton::setColourActivePressed_lua(sol::table c) { setColourProperty(*this, propButtonActivePressed, c); }

	
		const char* WidgetButton::getText() const {
			return findChild(uiidButtonText)->getText();
		}
		void WidgetButton::setText(const char* text) {
			findChild(uiidButtonText)->setText(text);
		}

		//
		// WidgetFillbar
		//
		static Property propFillbarCurrent, propFillbarMax;
		static Property propFillbarShowText;
		static Property propFillbarCol[4];

		bool WidgetFillbar::initProperties() {
			propFillbarCurrent = registerProperty("PartFillbar_current");
			propFillbarMax = registerProperty("PartFillbar_max");
			propFillbarShowText = registerProperty("PartFillbar_show_text");
			initColourProperty(propFillbarCol, "PartFillbar");
			return true;
		}

		WidgetFillbar* WidgetFillbar::fromElement(Element* element) {
			static bool initialized = initProperties();
			return static_cast<WidgetFillbar*>(element);
		}

		const WidgetFillbar* WidgetFillbar::fromElement(const Element* element) {
			static bool initialized = initProperties();
			return static_cast<const WidgetFillbar*>(element);
		}

		double WidgetFillbar::getCurrent() const {
			const auto type = getPropertyType(propFillbarCurrent);
			if (type == PropertyType::Integer) {
				return getProperty(PropertyType::Integer, propFillbarCurrent).integerValue;
			}
			else if (type == PropertyType::Float) {
				return getProperty(PropertyType::Float, propFillbarCurrent).floatValue;
			}
			return 0.0;
		}
		void WidgetFillbar::setCurrent(double value) {
			const auto type = getPropertyType(propFillbarCurrent);
			if (type == PropertyType::Integer) {
				setProperty(propFillbarCurrent, (int)value);
			}
			else {
				setProperty(propFillbarCurrent, (float)value);
			}
		}

		double WidgetFillbar::getMax() const {
			const auto type = getPropertyType(propFillbarMax);
			if (type == PropertyType::Integer) {
				return getProperty(PropertyType::Integer, propFillbarMax).integerValue;
			}
			else if (type == PropertyType::Float) {
				return getProperty(PropertyType::Float, propFillbarMax).floatValue;
			}
			return 0.0;
		}
		void WidgetFillbar::setMax(double value) {
			const auto type = getPropertyType(propFillbarMax);
			if (type == PropertyType::Integer) {
				setProperty(propFillbarMax, (int)value);
			}
			else {
				setProperty(propFillbarMax, (float)value);
			}
		}

		double WidgetFillbar::getNormalized() const {
			const double max = getMax();
			if (max == 0.0) {
				return 0.0;
			}
			return getCurrent() / max;
		}
		void WidgetFillbar::setNormalized(double value) {
			setCurrent(value * getMax());
		}

		bool WidgetFillbar::getShowText() const {
			auto prop = getProperty(PropertyType::Property, propFillbarShowText).propertyValue;
			return (prop == Property::boolean_true);
		}
		void WidgetFillbar::setShowText(bool flag) {
			auto value = flag ? Property::boolean_true : Property::boolean_false;
			setProperty(propFillbarShowText, value);
		}

		sol::table WidgetFillbar::getFillColour_lua(sol::this_state ts) {
			return getColourProperty(*this, propFillbarCol, ts);
		}

		void WidgetFillbar::setFillColour(const float(&c)[3]) {
			setColourProperty(*this, propFillbarCol, c);
		}

		void WidgetFillbar::setFillColour_lua(sol::table c) {
			setColourProperty(*this, propFillbarCol, c);
		}

		void WidgetFillbar::setFillAlpha(float a) {
			setProperty(propFillbarCol[3], a);
		}

		//
		// WidgetParagraphInput
		//
		static Property propParagraphInputMaxLength;
		static UI_ID uiidParagraphInputText;

		bool WidgetParagraphInput::initProperties() {
			propParagraphInputMaxLength = registerProperty("PartParagraphInput_max_length");
			uiidParagraphInputText = registerID("PartParagraphInput_text_input");
			return true;
		}

		WidgetParagraphInput* WidgetParagraphInput::fromElement(Element* element) {
			static bool initialized = initProperties();
			return static_cast<WidgetParagraphInput*>(element);
		}

		const WidgetParagraphInput* WidgetParagraphInput::fromElement(const Element* element) {
			static bool initialized = initProperties();
			return static_cast<const WidgetParagraphInput*>(element);
		}

		int WidgetParagraphInput::getLengthLimit() const {
			return getProperty(PropertyType::Integer, propParagraphInputMaxLength).integerValue;
		}

		void WidgetParagraphInput::setLengthLimit(int limit) {
			setProperty(propParagraphInputMaxLength, limit);
		}

		void WidgetParagraphInput::setLengthLimit_lua(sol::optional<int> limit) {
			setLengthLimit(limit.value_or(1023));
		}

		std::string WidgetParagraphInput::getText() const {
			auto textInput = WidgetTextInput::fromElement(findChild(uiidParagraphInputText));
			return std::move(textInput->getText());
		}

		void WidgetParagraphInput::setText(const char* text) {
			std::string editText = text;
			editText.append("|");
			findChild(uiidParagraphInputText)->setText(editText.c_str());
		}

		//
		// WidgetScrollBar
		//
		static Property propScrollBarCurrent, propScrollBarMax;
		static Property propScrollStepX, propScrollJumpX;

		bool WidgetScrollBar::initProperties() {
			propScrollBarCurrent = registerProperty("PartScrollBar_current");
			propScrollBarMax = registerProperty("PartScrollBar_max");
			propScrollStepX = registerProperty("PartScrollBar_step_x");
			propScrollJumpX = registerProperty("PartScrollBar_jump_x");
			return true;
		}

		WidgetScrollBar* WidgetScrollBar::fromElement(Element* element) {
			static bool initialized = initProperties();
			return static_cast<WidgetScrollBar*>(element);
		}

		const WidgetScrollBar* WidgetScrollBar::fromElement(const Element* element) {
			static bool initialized = initProperties();
			return static_cast<const WidgetScrollBar*>(element);
		}

		int WidgetScrollBar::getCurrent() const {
			return getProperty(PropertyType::Integer, propScrollBarCurrent).integerValue;
		}
		void WidgetScrollBar::setCurrent(int value) {
			setProperty(propScrollBarCurrent, value);
		}

		int WidgetScrollBar::getMax() const {
			return getProperty(PropertyType::Integer, propScrollBarMax).integerValue;
		}
		void WidgetScrollBar::setMax(int value) {
			setProperty(propScrollBarMax, value);
		}

		int WidgetScrollBar::getStepX() const {
			return getProperty(PropertyType::Integer, propScrollStepX).integerValue;
		}
		void WidgetScrollBar::setStepX(int value) {
			setProperty(propScrollStepX, value);
		}

		int WidgetScrollBar::getJumpX() const {
			return getProperty(PropertyType::Integer, propScrollJumpX).integerValue;
		}
		void WidgetScrollBar::setJumpX(int value) {
			setProperty(propScrollJumpX, value);
		}

		//
		// WidgetScrollPane
		//
		static UI_ID uiidScrollPaneHScroll, uiidScrollPaneVScroll;

		const auto TES3_WidgetScrollPane_getContentPane = reinterpret_cast<TES3::UI::Element*(__cdecl*)(WidgetScrollPane*)>(0x649CA0);

		bool WidgetScrollPane::initProperties() {
			uiidScrollPaneHScroll = registerID("PartScrollPane_hor_scrollbar");
			uiidScrollPaneVScroll = registerID("PartScrollPane_vert_scrollbar");
			return true;
		}

		WidgetScrollPane* WidgetScrollPane::fromElement(Element* element) {
			static bool initialized = initProperties();
			return static_cast<WidgetScrollPane*>(element);
		}

		const WidgetScrollPane* WidgetScrollPane::fromElement(const Element* element) {
			static bool initialized = initProperties();
			return static_cast<const WidgetScrollPane*>(element);
		}

		int WidgetScrollPane::getHorizontalPos() const {
			auto scroll = WidgetScrollBar::fromElement(findChild(uiidScrollPaneHScroll));
			return scroll->getCurrent();
		}

		void WidgetScrollPane::setHorizontalPos(int value) {
			auto scroll = WidgetScrollBar::fromElement(findChild(uiidScrollPaneHScroll));
			scroll->setCurrent(value);

			const auto TES3_ui_ScrollPaneHorz_scrollBarChanged = reinterpret_cast<EventCallback>(0x649A20);
			TES3_ui_ScrollPaneHorz_scrollBarChanged(this, Property::null, 0, 0, scroll);
		}

		int WidgetScrollPane::getVerticalPos() const {
			auto scroll = WidgetScrollBar::fromElement(findChild(uiidScrollPaneVScroll));
			return scroll->getCurrent();
		}

		void WidgetScrollPane::setVerticalPos(int value) {
			auto scroll = WidgetScrollBar::fromElement(findChild(uiidScrollPaneVScroll));
			scroll->setCurrent(value);

			const auto TES3_ui_ScrollPaneVert_scrollBarChanged = reinterpret_cast<EventCallback>(0x649870);
			TES3_ui_ScrollPaneVert_scrollBarChanged(this, Property::null, 0, 0, scroll);
		}

		bool WidgetScrollPane::getScrollbarVisible() const {
			auto scrollH = findChild(uiidScrollPaneHScroll);
			auto scrollV = findChild(uiidScrollPaneVScroll);

			if (scrollH) {
				return scrollH->visible;
			}
			if (scrollV) {
				return scrollV->visible;
			}
			return false;
		}
		void WidgetScrollPane::setScrollbarVisible(bool value) {
			auto scrollH = findChild(uiidScrollPaneHScroll);
			auto scrollV = findChild(uiidScrollPaneVScroll);

			if (scrollH) {
				scrollH->setVisible(value);
			}
			if (scrollV) {
				scrollV->setVisible(value);
			}
		}

		TES3::UI::Element * WidgetScrollPane::getContentPane() {
			return TES3_WidgetScrollPane_getContentPane(this);
		}

		void WidgetScrollPane::contentPaneChanged() {
			const auto TES3_ui_ScrollPane_contentPaneChanged = reinterpret_cast<void (__cdecl*)(WidgetScrollPane*)>(0x649E40);
			TES3_ui_ScrollPane_contentPaneChanged(this);
		}

		//
		// WidgetTextInput
		//
		static Property propTextInputLengthLimit, propTextInputNoLimit;
		static Property propTextInputEraseOnFirstKey;

		bool WidgetTextInput::initProperties() {
			propTextInputLengthLimit = registerProperty("PartTextInput_length_limit");
			propTextInputNoLimit = registerProperty("PartTextInput_no_limit");
			propTextInputEraseOnFirstKey = registerProperty("PartTextInput_erase_on_first_key");
			return true;
		}

		WidgetTextInput* WidgetTextInput::fromElement(Element* element) {
			static bool initialized = initProperties();
			return static_cast<WidgetTextInput*>(element);
		}

		const WidgetTextInput* WidgetTextInput::fromElement(const Element* element) {
			static bool initialized = initProperties();
			return static_cast<const WidgetTextInput*>(element);
		}

		int WidgetTextInput::getLengthLimit() const {
			return getProperty(PropertyType::Integer, propTextInputLengthLimit).integerValue;
		}

		sol::optional<int> WidgetTextInput::getLengthLimit_lua() const {
			// Convert no limit to nil
			int n = getLengthLimit();
			if (getNoLimit() || n <= 0) {
				return {};
			}
			return n;
		}

		void WidgetTextInput::setLengthLimit(int limit) {
			setProperty(propTextInputLengthLimit, limit);
		}

		void WidgetTextInput::setLengthLimit_lua(sol::optional<int> limit) {
			// Convert nil to no limit
			if (limit) {
				setLengthLimit(limit.value());
				setNoLimit(false);
			}
			else {
				setNoLimit(true);
			}
		}

		bool WidgetTextInput::getNoLimit() const {
			auto prop = getProperty(PropertyType::Property, propTextInputNoLimit).propertyValue;
			return (prop == Property::boolean_true);
		}
		void WidgetTextInput::setNoLimit(bool flag) {
			auto value = flag ? Property::boolean_true : Property::boolean_false;
			setProperty(propTextInputNoLimit, value);
		}

		bool WidgetTextInput::getEraseOnFirstKey() const {
			auto prop = getProperty(PropertyType::Property, propTextInputEraseOnFirstKey).propertyValue;
			return (prop == Property::boolean_true);
		}
		void WidgetTextInput::setEraseOnFirstKey(bool flag) {
			auto value = flag ? Property::boolean_true : Property::boolean_false;
			setProperty(propTextInputEraseOnFirstKey, value);
		}

		std::string WidgetTextInput::getText() const {
			// getText's return value includes the text caret '|'
			// The game's function for removing it creates extra allocations that are inconvenient to free.
			std::string editText = Element::getText();
			auto i = editText.find('|');
			if (i != std::string::npos) {
				editText.erase(i, 1);
			}
			return std::move(editText);
		}
		void WidgetTextInput::setText(const char* text) {
			// Use standard setText without adding a caret.
			// PartTextInput behaviour - First keypress will clear the line.
			// PartParagraphInput behaviour - Caret is positioned at the end of the text, first keypress will append.
			Element::setText(text);
		}

		//
		// WidgetTextSelect
		//

		const auto TES3_ui_updateTextSelectColorForState = reinterpret_cast<bool(__cdecl*)(Element*)>(0x64CB60);

		static Property propSelectState;
		static Property propSelectIdle[4], propSelectOver[4], propSelectPressed[4];
		static Property propSelectDisabled[4], propSelectDisabledOver[4], propSelectDisabledPressed[4];
		static Property propSelectActive[4], propSelectActiveOver[4], propSelectActivePressed[4];

		bool WidgetTextSelect::initProperties() {
			propSelectState = registerProperty("PartTextSelect_state");
			initColourProperty(propSelectIdle, "PartTextSelect_idle");
			initColourProperty(propSelectOver, "PartTextSelect_over");
			initColourProperty(propSelectPressed, "PartTextSelect_pressed");
			initColourProperty(propSelectDisabled, "PartTextSelect_disabled");
			initColourProperty(propSelectDisabledOver, "PartTextSelect_disabled_over");
			initColourProperty(propSelectDisabledPressed, "PartTextSelect_disabled_pressed");
			initColourProperty(propSelectActive, "PartTextSelect_active");
			initColourProperty(propSelectActiveOver, "PartTextSelect_active_over");
			initColourProperty(propSelectActivePressed, "PartTextSelect_active_pressed");
			return true;
		}

		WidgetTextSelect* WidgetTextSelect::fromElement(Element* element) {
			static bool initialized = initProperties();
			return static_cast<WidgetTextSelect*>(element);
		}

		const WidgetTextSelect* WidgetTextSelect::fromElement(const Element* element) {
			static bool initialized = initProperties();
			return static_cast<const WidgetTextSelect*>(element);
		}

		int WidgetTextSelect::getState() const {
			return getProperty(PropertyType::Integer, propSelectState).integerValue;
		}

		void WidgetTextSelect::setState(int state) {
			setProperty(propSelectState, state);
			TES3_ui_updateTextSelectColorForState(this);
		}

		sol::table WidgetTextSelect::getColourIdle_lua(sol::this_state ts) { return getColourProperty(*this, propSelectIdle, ts); }
		void WidgetTextSelect::setColourIdle(const float(&c)[3]) { setColourProperty(*this, propSelectIdle, c); TES3_ui_updateTextSelectColorForState(this); }
		void WidgetTextSelect::setColourIdle_lua(sol::table c) { setColourProperty(*this, propSelectIdle, c); TES3_ui_updateTextSelectColorForState(this); }
		sol::table WidgetTextSelect::getColourOver_lua(sol::this_state ts) { return getColourProperty(*this, propSelectOver, ts); }
		void WidgetTextSelect::setColourOver(const float(&c)[3]) { setColourProperty(*this, propSelectOver, c); TES3_ui_updateTextSelectColorForState(this); }
		void WidgetTextSelect::setColourOver_lua(sol::table c) { setColourProperty(*this, propSelectOver, c); TES3_ui_updateTextSelectColorForState(this); }
		sol::table WidgetTextSelect::getColourPressed_lua(sol::this_state ts) { return getColourProperty(*this, propSelectPressed, ts); }
		void WidgetTextSelect::setColourPressed(const float(&c)[3]) { setColourProperty(*this, propSelectPressed, c); TES3_ui_updateTextSelectColorForState(this); }
		void WidgetTextSelect::setColourPressed_lua(sol::table c) { setColourProperty(*this, propSelectPressed, c); TES3_ui_updateTextSelectColorForState(this); }
		sol::table WidgetTextSelect::getColourDisabled_lua(sol::this_state ts) { return getColourProperty(*this, propSelectDisabled, ts); }
		void WidgetTextSelect::setColourDisabled(const float(&c)[3]) { setColourProperty(*this, propSelectDisabled, c); TES3_ui_updateTextSelectColorForState(this); }
		void WidgetTextSelect::setColourDisabled_lua(sol::table c) { setColourProperty(*this, propSelectDisabled, c); TES3_ui_updateTextSelectColorForState(this); }
		sol::table WidgetTextSelect::getColourDisabledOver_lua(sol::this_state ts) { return getColourProperty(*this, propSelectDisabledOver, ts); }
		void WidgetTextSelect::setColourDisabledOver(const float(&c)[3]) { setColourProperty(*this, propSelectDisabledOver, c); TES3_ui_updateTextSelectColorForState(this); }
		void WidgetTextSelect::setColourDisabledOver_lua(sol::table c) { setColourProperty(*this, propSelectDisabledOver, c); TES3_ui_updateTextSelectColorForState(this); }
		sol::table WidgetTextSelect::getColourDisabledPressed_lua(sol::this_state ts) { return getColourProperty(*this, propSelectDisabledPressed, ts); }
		void WidgetTextSelect::setColourDisabledPressed(const float(&c)[3]) { setColourProperty(*this, propSelectDisabledPressed, c); TES3_ui_updateTextSelectColorForState(this); }
		void WidgetTextSelect::setColourDisabledPressed_lua(sol::table c) { setColourProperty(*this, propSelectDisabledPressed, c); TES3_ui_updateTextSelectColorForState(this); }
		sol::table WidgetTextSelect::getColourActive_lua(sol::this_state ts) { return getColourProperty(*this, propSelectActive, ts); }
		void WidgetTextSelect::setColourActive(const float(&c)[3]) { setColourProperty(*this, propSelectActive, c); TES3_ui_updateTextSelectColorForState(this); }
		void WidgetTextSelect::setColourActive_lua(sol::table c) { setColourProperty(*this, propSelectActive, c); TES3_ui_updateTextSelectColorForState(this); }
		sol::table WidgetTextSelect::getColourActiveOver_lua(sol::this_state ts) { return getColourProperty(*this, propSelectActiveOver, ts); }
		void WidgetTextSelect::setColourActiveOver(const float(&c)[3]) { setColourProperty(*this, propSelectActiveOver, c); TES3_ui_updateTextSelectColorForState(this); }
		void WidgetTextSelect::setColourActiveOver_lua(sol::table c) { setColourProperty(*this, propSelectActiveOver, c); TES3_ui_updateTextSelectColorForState(this); }
		sol::table WidgetTextSelect::getColourActivePressed_lua(sol::this_state ts) { return getColourProperty(*this, propSelectActivePressed, ts); }
		void WidgetTextSelect::setColourActivePressed(const float(&c)[3]) { setColourProperty(*this, propSelectActivePressed, c); TES3_ui_updateTextSelectColorForState(this); }
		void WidgetTextSelect::setColourActivePressed_lua(sol::table c) { setColourProperty(*this, propSelectActivePressed, c); TES3_ui_updateTextSelectColorForState(this); }
	}
}