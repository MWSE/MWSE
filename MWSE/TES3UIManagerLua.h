#pragma once

#include "TES3UIElement.h"

namespace mwse {
	namespace lua {
		void registerBeforeUIEvent(TES3::UI::Element* target, TES3::UI::Property eventID, sol::protected_function callback, double priority);
		void registerAfterUIEvent(TES3::UI::Element* target, TES3::UI::Property eventID, sol::protected_function callback, double priority);
		void registerUIEvent(TES3::UI::Element* target, TES3::UI::Property eventID, sol::protected_function callback);
		bool unregisterBeforeUIEvent(TES3::UI::Element* target, TES3::UI::Property eventID, sol::protected_function callback);
		bool unregisterAfterUIEvent(TES3::UI::Element* target, TES3::UI::Property eventID, sol::protected_function callback);
		void unregisterUIEvent(TES3::UI::Element* target, TES3::UI::Property eventID);
		bool eventForwarder(sol::table eventData);
		void triggerEvent(TES3::UI::Element* target, TES3::UI::Property eventID, int data0, int data1);

		void bindTES3UIManager();
	}
}
