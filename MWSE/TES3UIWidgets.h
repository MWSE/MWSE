#pragma once

#include "TES3UIElement.h"

namespace TES3::UI {
	struct Widget : Element {
		Widget() = delete;
		~Widget() = delete;

		Element* getElement();
	};

	struct WidgetButton : Widget {
		int getState() const;
		void setState(int state);

		sol::table getColourIdle_lua(sol::this_state);
		void setColourIdle(const float(&c)[3]);
		void setColourIdle_lua(sol::table);
		sol::table getColourOver_lua(sol::this_state);
		void setColourOver(const float(&c)[3]);
		void setColourOver_lua(sol::table);
		sol::table getColourPressed_lua(sol::this_state);
		void setColourPressed(const float(&c)[3]);
		void setColourPressed_lua(sol::table);
		sol::table getColourDisabled_lua(sol::this_state);
		void setColourDisabled(const float(&c)[3]);
		void setColourDisabled_lua(sol::table);
		sol::table getColourDisabledOver_lua(sol::this_state);
		void setColourDisabledOver(const float(&c)[3]);
		void setColourDisabledOver_lua(sol::table);
		sol::table getColourDisabledPressed_lua(sol::this_state);
		void setColourDisabledPressed(const float(&c)[3]);
		void setColourDisabledPressed_lua(sol::table);
		sol::table getColourActive_lua(sol::this_state);
		void setColourActive(const float(&c)[3]);
		void setColourActive_lua(sol::table);
		sol::table getColourActiveOver_lua(sol::this_state);
		void setColourActiveOver(const float(&c)[3]);
		void setColourActiveOver_lua(sol::table);
		sol::table getColourActivePressed_lua(sol::this_state);
		void setColourActivePressed(const float(&c)[3]);
		void setColourActivePressed_lua(sol::table);

		const char* getText() const;
		void setText(const char* text);

		Element* getTextElement() const;

		WidgetButton() = delete;
		~WidgetButton() = delete;
		static WidgetButton* fromElement(Element* e);
		static const WidgetButton* fromElement(const Element* e);
	private:
		static bool initProperties();
	};

	struct WidgetFillbar : Widget {
		double getCurrent() const;
		void setCurrent(double value);
		double getMax() const;
		void setMax(double value);
		double getNormalized() const;
		void setNormalized(double value);
		bool getShowText() const;
		void setShowText(bool flag);
		sol::table getFillColour_lua(sol::this_state ts);
		void setFillColour(const float(&c)[3]);
		void setFillColour_lua(sol::table c);
		float getFillAlpha();
		void setFillAlpha(float a);

		WidgetFillbar() = delete;
		~WidgetFillbar() = delete;
		static WidgetFillbar* fromElement(Element* e);
		static const WidgetFillbar* fromElement(const Element* e);
	private:
		static bool initProperties();
	};

	struct WidgetParagraphInput : Widget {
		int getLengthLimit() const;
		void setLengthLimit(int limit);
		void setLengthLimit_lua(sol::optional<int> limit = 1023);
		std::string getText() const;
		void setText(const char* text);

		WidgetParagraphInput() = delete;
		~WidgetParagraphInput() = delete;
		static WidgetParagraphInput* fromElement(Element* e);
		static const WidgetParagraphInput* fromElement(const Element* e);
	private:
		static bool initProperties();
	};

	struct WidgetScrollBar : Widget {
		int getCurrent() const;
		void setCurrent(int value);
		int getMax() const;
		void setMax(int value);
		int getStepX() const;
		void setStepX(int value);
		int getJumpX() const;
		void setJumpX(int value);

		WidgetScrollBar() = delete;
		~WidgetScrollBar() = delete;
		static WidgetScrollBar* fromElement(Element* e);
		static const WidgetScrollBar* fromElement(const Element* e);
	private:
		static bool initProperties();
	};

	struct WidgetScrollPane : Widget {
		int getHorizontalPos() const;
		void setHorizontalPos(int value);
		int getVerticalPos() const;
		void setVerticalPos(int value);
		bool getScrollbarVisible() const;
		void setScrollbarVisible(bool value);
		TES3::UI::Element * getContentPane();
		void contentPaneChanged();

		WidgetScrollPane() = delete;
		~WidgetScrollPane() = delete;
		static WidgetScrollPane* fromElement(Element* e);
		static const WidgetScrollPane* fromElement(const Element* e);
	private:
		static bool initProperties();
	};

	struct WidgetTextInput : Widget {
		int getLengthLimit() const;
		sol::optional<int> getLengthLimit_lua() const;
		void setLengthLimit(int limit);
		void setLengthLimit_lua(sol::optional<int> limit);
		bool getNoLimit() const;
		void setNoLimit(bool flag);
		bool getEraseOnFirstKey() const;
		void setEraseOnFirstKey(bool flag);
		std::string getText() const;
		void setText(const char* text);

		WidgetTextInput() = delete;
		~WidgetTextInput() = delete;
		static WidgetTextInput* fromElement(Element* e);
		static const WidgetTextInput* fromElement(const Element* e);
	private:
		static bool initProperties();
	};

	struct WidgetTextSelect : Widget {
		int getState() const;
		void setState(int state);

		sol::table getColourIdle_lua(sol::this_state);
		void setColourIdle(const float(&c)[3]);
		void setColourIdle_lua(sol::table);
		sol::table getColourOver_lua(sol::this_state);
		void setColourOver(const float(&c)[3]);
		void setColourOver_lua(sol::table);
		sol::table getColourPressed_lua(sol::this_state);
		void setColourPressed(const float(&c)[3]);
		void setColourPressed_lua(sol::table);
		sol::table getColourDisabled_lua(sol::this_state);
		void setColourDisabled(const float(&c)[3]);
		void setColourDisabled_lua(sol::table);
		sol::table getColourDisabledOver_lua(sol::this_state);
		void setColourDisabledOver(const float(&c)[3]);
		void setColourDisabledOver_lua(sol::table);
		sol::table getColourDisabledPressed_lua(sol::this_state);
		void setColourDisabledPressed(const float(&c)[3]);
		void setColourDisabledPressed_lua(sol::table);
		sol::table getColourActive_lua(sol::this_state);
		void setColourActive(const float(&c)[3]);
		void setColourActive_lua(sol::table);
		sol::table getColourActiveOver_lua(sol::this_state);
		void setColourActiveOver(const float(&c)[3]);
		void setColourActiveOver_lua(sol::table);
		sol::table getColourActivePressed_lua(sol::this_state);
		void setColourActivePressed(const float(&c)[3]);
		void setColourActivePressed_lua(sol::table);

		WidgetTextSelect() = delete;
		~WidgetTextSelect() = delete;
		static WidgetTextSelect* fromElement(Element* e);
		static const WidgetTextSelect* fromElement(const Element* e);
	private:
		static bool initProperties();
	};
}