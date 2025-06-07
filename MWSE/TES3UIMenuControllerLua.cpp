#pragma once

#include "TES3UIMenuControllerLua.h"

#include "LuaManager.h"

#include "TES3ScriptCompiler.h"

#include "TES3UIElement.h"
#include "TES3UIMenuController.h"

#include "NINode.h"

namespace mwse::lua {
	void bindTES3UIMenuController() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;

		// Binding for TES3::UI::MenuInputController
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::UI::MenuInputController>("tes3uiMenuInputController");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["lastInputTime"] = &TES3::UI::MenuInputController::repeatKeyTimer;
			usertypeDefinition["menuController"] = sol::readonly_property(&TES3::UI::MenuInputController::menuController);
			usertypeDefinition["pointerMoveEventSource"] = sol::readonly_property(&TES3::UI::MenuInputController::pointerMoveEventSource);
			usertypeDefinition["pointerMovePreviousEventSource"] = sol::readonly_property(&TES3::UI::MenuInputController::pointerMovePreviousEventSource);
			usertypeDefinition["textInputFocus"] = sol::property(&TES3::UI::MenuInputController::getTextInputElement, &TES3::UI::MenuInputController::acquireTextInput);

			// Basic function binding.
			usertypeDefinition["flushBufferedTextEvents"] = &TES3::UI::MenuInputController::flushBufferedTextEvents;
		}

		// Binding for TES3::UI::FontColor
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::UI::FontColor>("tes3uiFontColor");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["r"] = &TES3::UI::FontColor::r;
			usertypeDefinition["g"] = &TES3::UI::FontColor::g;
			usertypeDefinition["b"] = &TES3::UI::FontColor::b;
		}

		// Binding for TES3::UI::MenuController
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::UI::MenuController>("tes3uiMenuController");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["aiDisabled"] = sol::property(&TES3::UI::MenuController::getAIDisabled, &TES3::UI::MenuController::setAIDisabled);
			usertypeDefinition["bordersEnabled"] = sol::property(&TES3::UI::MenuController::getBordersEnabled, &TES3::UI::MenuController::setBordersEnabled);
			usertypeDefinition["collisionBoxesEnabled"] = sol::property(&TES3::UI::MenuController::getCollisionBoxesEnabled, &TES3::UI::MenuController::setCollisionBoxesEnabled);
			usertypeDefinition["collisionDisabled"] = sol::property(&TES3::UI::MenuController::getCollisionDisabled, &TES3::UI::MenuController::setCollisionDisabled);
			usertypeDefinition["fogOfWarDisabled"] = sol::property(&TES3::UI::MenuController::getFogOfWarDisabled, &TES3::UI::MenuController::setFogOfWarDisabled);
			usertypeDefinition["fontColors"] = sol::readonly_property(&TES3::UI::MenuController::getFontColors);
			usertypeDefinition["godModeEnabled"] = sol::property(&TES3::UI::MenuController::getGodModeEnabled, &TES3::UI::MenuController::setGodModeEnabled);
			usertypeDefinition["helpDelay"] = sol::readonly_property(&TES3::UI::MenuController::helpDelay);
			usertypeDefinition["helpRoot"] = sol::readonly_property(&TES3::UI::MenuController::helpRoot);
			usertypeDefinition["inputController"] = sol::readonly_property(&TES3::UI::MenuController::menuInputController);
			usertypeDefinition["inventoryMenuEnabled"] = sol::property(&TES3::UI::MenuController::getInventoryMenuEnabled, &TES3::UI::MenuController::setInventoryMenuEnabled);
			usertypeDefinition["magicMenuEnabled"] = sol::property(&TES3::UI::MenuController::getMagicMenuEnabled, &TES3::UI::MenuController::setMagicMenuEnabled);
			usertypeDefinition["mainRoot"] = sol::readonly_property(&TES3::UI::MenuController::mainRoot);
			usertypeDefinition["mapMenuEnabled"] = sol::property(&TES3::UI::MenuController::getMapMenuEnabled, &TES3::UI::MenuController::setMapMenuEnabled);
			usertypeDefinition["menusDisabled"] = sol::property(&TES3::UI::MenuController::getMenusDisabled, &TES3::UI::MenuController::setMenusDisabled);
			usertypeDefinition["pathGridShown"] = sol::property(&TES3::UI::MenuController::getShowPathGrid, &TES3::UI::MenuController::setShowPathGrid);
			usertypeDefinition["scriptCompiler"] = sol::readonly_property(&TES3::UI::MenuController::scriptCompiler);
			usertypeDefinition["scriptsDisabled"] = sol::property(&TES3::UI::MenuController::getScriptsDisabled, &TES3::UI::MenuController::setScriptsDisabled);
			usertypeDefinition["skyDisabled"] = sol::property(&TES3::UI::MenuController::getSkyDisabled, &TES3::UI::MenuController::setSkyDisabled);
			usertypeDefinition["statsMenuEnabled"] = sol::property(&TES3::UI::MenuController::getStatsMenuEnabled, &TES3::UI::MenuController::setStatsMenuEnabled);
			usertypeDefinition["wireframeEnabled"] = sol::property(&TES3::UI::MenuController::getWireframeEnabled, &TES3::UI::MenuController::setWireframeEnabled);
			usertypeDefinition["worldDisabled"] = sol::property(&TES3::UI::MenuController::getWorldDisabled, &TES3::UI::MenuController::setWorldDisabled);
		}
	}
}
