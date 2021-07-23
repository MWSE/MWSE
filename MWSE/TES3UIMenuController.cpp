#include "TES3UIMenuController.h"

#include "TES3UIManager.h"
#include "TES3UIElement.h"

#include "LuaManager.h"
#include "LuaUiObjectTooltipEvent.h"

namespace TES3 {
	namespace UI {
		const auto TES3_MenuInputController_flushBufferedTextEvents = reinterpret_cast<void(__thiscall*)(MenuInputController*)>(0x58E9C0);
		void MenuInputController::flushBufferedTextEvents() {
			TES3_MenuInputController_flushBufferedTextEvents(this);
		}

		Element* MenuInputController::getTextInputElement() {
			return textInputFocus;
		}

		void MenuInputController::acquireTextInput(Element* element) {
			// Set target for buffered text input
			textInputFocus = element;

			// Reset text buffer to avoid previous input appearing immediately
			flushBufferedTextEvents();
		}

		// Storage of the last data used for displayObjectTooltip, for use with updateObjectTooltip.
		static TES3::Object * lastTooltipObject = nullptr;
		static TES3::ItemData * lastTooltipItemData = nullptr;
		static int lastTooltipCount = 0;

		const auto TES3_UI_displayObjectTooltip = reinterpret_cast<void(__thiscall*)(MenuInputController*, TES3::Object*, TES3::ItemData*, int)>(0x590D90);
		void MenuInputController::displayObjectTooltip(TES3::Object * object, TES3::ItemData * itemData, int count) {
			// Keep track of the last tooltip information shown for updateObjectTooltip.
			lastTooltipObject = object;
			lastTooltipItemData = itemData;
			lastTooltipCount = count;

			// Call native function.
			TES3_UI_displayObjectTooltip(this, object, itemData, count);

			// Check for suppression of world object tooltips.
			if (TES3::UI::isSuppressingHelpMenu() && object->objectType == TES3::ObjectType::Reference) {
				TES3::UI::suppressHelpMenu();
			}
			else if (mwse::lua::event::UiObjectTooltipEvent::getEventEnabled()) {
				// Fire off the event.
				TES3::UI::Element* tooltip = TES3::UI::findHelpLayerMenu(TES3::UI::UI_ID(TES3::UI::Property::HelpMenu));
				mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::UiObjectTooltipEvent(tooltip, object, itemData, count));
			}
		}

		void MenuInputController::updateObjectTooltip() {
			// Do we have a valid tooltip object?
			if (lastTooltipObject == nullptr) {
				return;
			}

			// Are tooltips suppressed?
			if (TES3::UI::isSuppressingHelpMenu()) {
				return;
			}

			// This only matters if the menu already exists and is showing.
			static const TES3::UI::UI_ID mainHelpLayerMenu = 0x8105;
			auto helpMenu = TES3::UI::findHelpLayerMenu(mainHelpLayerMenu);
			if (helpMenu == nullptr || helpMenu->visible == false) {
				return;
			}

			// Is this the same tooltip that we cared about?
			auto object = reinterpret_cast<TES3::Object*>(helpMenu->getProperty(TES3::UI::PropertyType::Pointer, *reinterpret_cast<TES3::UI::Property*>(0x7D7C50)).ptrValue);
			if (object != lastTooltipObject) {
				return;
			}

			// Rebuild the tooltip.
			displayObjectTooltip(lastTooltipObject, lastTooltipItemData, lastTooltipCount);
		}

		const auto TES3_MenuController_setInventoryMenuEnabled = reinterpret_cast<void(__thiscall*)(MenuController *, bool)>(0x5968D0);
		void MenuController::setInventoryMenuEnabled(bool enabled) {
			TES3_MenuController_setInventoryMenuEnabled(this, enabled);
		}

		const auto TES3_MenuController_setMagicMenuEnabled = reinterpret_cast<void(__thiscall*)(MenuController *, bool)>(0x596A90);
		void MenuController::setMagicMenuEnabled(bool enabled) {
			TES3_MenuController_setMagicMenuEnabled(this, enabled);
		}

		const auto TES3_MenuController_setMapMenuEnabled = reinterpret_cast<void(__thiscall*)(MenuController *, bool)>(0x596B70);
		void MenuController::setMapMenuEnabled(bool enabled) {
			TES3_MenuController_setMapMenuEnabled(this, enabled);
		}

		const auto TES3_MenuController_setStatsMenuEnabled = reinterpret_cast<void(__thiscall*)(MenuController *, bool)>(0x5969B0);
		void MenuController::setStatsMenuEnabled(bool enabled) {
			TES3_MenuController_setStatsMenuEnabled(this, enabled);
		}

		bool MenuController::getInventoryMenuEnabled() {
			return inventoryMenuEnabled;
		}

		bool MenuController::getMagicMenuEnabled() {
			return magicMenuEnabled;
		}

		bool MenuController::getMapMenuEnabled() {
			return mapMenuEnabled;
		}

		bool MenuController::getStatsMenuEnabled() {
			return statsMenuEnabled;
		}

		std::reference_wrapper<FontColor[FontColorId::MAX_ID + 1]> MenuController::getFontColors() {
			return std::ref(fontColors);
		}
	}
}