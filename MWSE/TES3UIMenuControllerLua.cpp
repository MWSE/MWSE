#pragma once

#include "TES3UIMenuControllerLua.h"

#include "LuaManager.h"

#include "TES3ScriptCompiler.h"

#include "TES3UIElement.h"
#include "TES3UIMenuController.h"

#include "NINode.h"

namespace mwse {
	namespace lua {
		void bindTES3UIMenuController() {
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Binding for TES3::UI::MenuInputController
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<TES3::UI::MenuInputController>("tes3uiMenuInputController");
				usertypeDefinition["new"] = sol::no_constructor;

				// Basic property binding.
				usertypeDefinition["lastInputTime"] = &TES3::UI::MenuInputController::lastInputTime;
				usertypeDefinition["menuController"] = sol::readonly_property(&TES3::UI::MenuInputController::menuController);
				usertypeDefinition["textInputFocus"] = sol::property(&TES3::UI::MenuInputController::getTextInputElement, &TES3::UI::MenuInputController::acquireTextInput);

				// Basic function binding.
				usertypeDefinition["flushBufferedTextEvents"] = &TES3::UI::MenuInputController::flushBufferedTextEvents;
			}

			// Binding for TES3::UI::FontColor
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<TES3::UI::FontColor>("tes3uiFontColor");
				usertypeDefinition["new"] = sol::no_constructor;

				// Basic property binding.
				usertypeDefinition["r"] = &TES3::UI::FontColor::r;
				usertypeDefinition["g"] = &TES3::UI::FontColor::g;
				usertypeDefinition["b"] = &TES3::UI::FontColor::b;
			}

			// Binding for TES3::UI::MenuController
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<TES3::UI::MenuController>("tes3uiMenuController");
				usertypeDefinition["new"] = sol::no_constructor;

				// Basic property binding.
				usertypeDefinition["fontColors"] = sol::readonly_property(&TES3::UI::MenuController::getFontColors);
				usertypeDefinition["helpDelay"] = sol::readonly_property(&TES3::UI::MenuController::helpDelay);
				usertypeDefinition["helpRoot"] = sol::readonly_property(&TES3::UI::MenuController::helpRoot);
				usertypeDefinition["inputController"] = sol::readonly_property(&TES3::UI::MenuController::menuInputController);
				usertypeDefinition["inventoryMenuEnabled"] = sol::property(&TES3::UI::MenuController::getInventoryMenuEnabled, &TES3::UI::MenuController::setInventoryMenuEnabled);
				usertypeDefinition["magicMenuEnabled"] = sol::property(&TES3::UI::MenuController::getMagicMenuEnabled, &TES3::UI::MenuController::setMagicMenuEnabled);
				usertypeDefinition["mainRoot"] = sol::readonly_property(&TES3::UI::MenuController::mainRoot);
				usertypeDefinition["mapMenuEnabled"] = sol::property(&TES3::UI::MenuController::getMapMenuEnabled, &TES3::UI::MenuController::setMapMenuEnabled);
				usertypeDefinition["scriptCompiler"] = sol::readonly_property(&TES3::UI::MenuController::scriptCompiler);
				usertypeDefinition["statsMenuEnabled"] = sol::property(&TES3::UI::MenuController::getStatsMenuEnabled, &TES3::UI::MenuController::setStatsMenuEnabled);
			}
		}
	}
}
