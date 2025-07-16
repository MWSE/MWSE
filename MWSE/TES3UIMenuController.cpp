#include "TES3UIMenuController.h"

#include "TES3DataHandler.h"
#include "TES3Game.h"
#include "TES3MobManager.h"
#include "TES3UIManager.h"
#include "TES3UIElement.h"
#include "TES3WaterController.h"
#include "TES3WeatherController.h"
#include "TES3WorldController.h"
#include "TES3Cell.h"

#include "LuaManager.h"
#include "LuaUiObjectTooltipEvent.h"

#include "BitUtil.h"

namespace TES3::UI {
	// Storage of the last data used for displayObjectTooltip, for use with updateObjectTooltip.
	Object* MenuInputController::lastTooltipObject = nullptr;
	ItemData* MenuInputController::lastTooltipItemData = nullptr;
	int MenuInputController::lastTooltipCount = 0;
	Element* MenuInputController::lastTooltipSource = nullptr;
	int MenuInputController::lastKeyPressDIK = 0xFF;

	const auto TES3_MenuInputController_flushBufferedTextEvents = reinterpret_cast<void(__thiscall*)(MenuInputController*)>(0x58E9C0);
	void MenuInputController::flushBufferedTextEvents() {
		TES3_MenuInputController_flushBufferedTextEvents(this);
	}

	Element* MenuInputController::getTextInputElement() const {
		return textInputFocus;
	}

	void MenuInputController::acquireTextInput(Element* element) {
		if (element && !element->isValid()) {
			throw std::invalid_argument("Element passed is not valid.");
		}

		// Set target for buffered text input
		textInputFocus = element;

		// Reset text buffer to avoid previous input appearing immediately
		flushBufferedTextEvents();
	}

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

	Element* MenuInputController::previousTextInputFocus = nullptr;

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

		// Preserve the lifespan.
		auto PartHelpMenu_lifespan = *reinterpret_cast<TES3::UI::Property*>(0x7D7B8C);
		auto lifespan = helpMenu->getProperty(TES3::UI::PropertyType::Float, PartHelpMenu_lifespan).floatValue;

		// Rebuild the tooltip.
		displayObjectTooltip(lastTooltipObject, lastTooltipItemData, lastTooltipCount);

		// We have to refetch the help menu because something lua-side may have mucked with it.
		helpMenu = TES3::UI::findHelpLayerMenu(mainHelpLayerMenu);
		if (helpMenu == nullptr) {
			return;
		}

		// Restore lifespan, so that help delay isn't retriggered.
		helpMenu->setProperty(PartHelpMenu_lifespan, lifespan);
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

	const auto TES3_updateFogOfWarRenderState = reinterpret_cast<void(__cdecl*)()>(0x5EB340);
	void MenuController::updateFogOfWarRenderState() {
		TES3_updateFogOfWarRenderState();
	}

	bool MenuController::getInventoryMenuEnabled() const {
		return inventoryMenuEnabled;
	}

	bool MenuController::getMagicMenuEnabled() const {
		return magicMenuEnabled;
	}

	bool MenuController::getMapMenuEnabled() const {
		return mapMenuEnabled;
	}

	bool MenuController::getStatsMenuEnabled() const {
		return statsMenuEnabled;
	}

	bool MenuController::getShowCombatStats() const {
		return BITMASK_TEST(gameplayFlags, MenuControllerGameplayFlags::ShowCombatStats);
	}

	void MenuController::setShowCombatStats(bool state) {
		BITMASK_SET(gameplayFlags, MenuControllerGameplayFlags::ShowCombatStats, state);
	}

	bool MenuController::getGodModeEnabled() const {
		return BITMASK_TEST(gameplayFlags, MenuControllerGameplayFlags::GodModeEnabled);
	}

	void MenuController::setGodModeEnabled(bool state) {
		BITMASK_SET(gameplayFlags, MenuControllerGameplayFlags::GodModeEnabled, state);
	}

	bool MenuController::getLightingUpdatesDisabled() const {
		return BITMASK_TEST(gameplayFlags, MenuControllerGameplayFlags::LightingUpdateDisabled);
	}

	void MenuController::setLightingUpdatesDisabled(bool state) {
		BITMASK_SET(gameplayFlags, MenuControllerGameplayFlags::LightingUpdateDisabled, state);
	}

	bool MenuController::getAIDisabled() const {
		return BITMASK_TEST(gameplayFlags, MenuControllerGameplayFlags::AIDisabled);
	}

	void MenuController::setAIDisabled(bool state) {
		BITMASK_SET(gameplayFlags, MenuControllerGameplayFlags::AIDisabled, state);
		TES3::WorldController::get()->disableAI = state;
	}

	bool MenuController::getBordersEnabled() const {
		return BITMASK_TEST(gameplayFlags, MenuControllerGameplayFlags::BordersEnabled);
	}

	void MenuController::setBordersEnabled(bool state) {
		BITMASK_SET(gameplayFlags, MenuControllerGameplayFlags::BordersEnabled, state);

		const auto dataHandler = TES3::DataHandler::get();
		if (dataHandler->currentInteriorCell == nullptr) {
			dataHandler->setDisplayCellBorders(state);
		}
	}

	bool MenuController::getSkyDisabled() const {
		return BITMASK_TEST(gameplayFlags, MenuControllerGameplayFlags::SkyDisabled);
	}

	void MenuController::setSkyDisabled(bool disabled) {
		const auto weatherController = TES3::WorldController::get()->weatherController;
		if (disabled) {
			weatherController->disableSky();
		}
		else {
			weatherController->enableSky();
		}
	}

	bool MenuController::getWorldDisabled() const {
		return BITMASK_TEST(gameplayFlags, MenuControllerGameplayFlags::WorldDisabled);
	}

	void MenuController::setWorldDisabled(bool disabled) {
		BITMASK_SET(gameplayFlags, MenuControllerGameplayFlags::WorldDisabled, disabled);

		const auto dataHandler = TES3::DataHandler::get();
		dataHandler->worldObjectRoot->setAppCulled(disabled);
		dataHandler->worldPickObjectRoot->setAppCulled(disabled);
		dataHandler->worldLandscapeRoot->setAppCulled(disabled);
	}

	bool MenuController::getWireframeEnabled() const {
		return BITMASK_TEST(gameplayFlags, MenuControllerGameplayFlags::WireframeEnabled);
	}

	void MenuController::setWireframeEnabled(bool state) {
		BITMASK_SET(gameplayFlags, MenuControllerGameplayFlags::WireframeEnabled, state);
		TES3::Game::get()->wireframeProperty->setEnabled(state);
	}

	bool MenuController::getCollisionDisabled() const {
		return BITMASK_TEST(gameplayFlags, MenuControllerGameplayFlags::CollisionDisabled);
	}

	void MenuController::setCollisionDisabled(bool state) {
		BITMASK_SET(gameplayFlags, MenuControllerGameplayFlags::CollisionDisabled, state);

		const auto worldController = TES3::WorldController::get();
		if (state) {
			worldController->collisionEnabled = false;
			worldController->mobManager->resetConstantVelocities();
		}
		else {
			worldController->collisionEnabled = true;
			worldController->mobManager->clampAllActors();
		}
	}

	bool MenuController::getCollisionBoxesEnabled() const {
		return BITMASK_TEST(gameplayFlags, MenuControllerGameplayFlags::CollisionBoxesEnabled);
	}

	void MenuController::setCollisionBoxesEnabled(bool enabled) {
		BITMASK_SET(gameplayFlags, MenuControllerGameplayFlags::CollisionBoxesEnabled, enabled);

		TES3::DataHandler::get()->setActorCollisionBoxesDisplay(enabled, true);
		TES3::WorldController::get()->collisionEnabled = enabled;
	}

	bool MenuController::getFogOfWarDisabled() const {
		return BITMASK_TEST(gameplayFlags, MenuControllerGameplayFlags::FogOfWarDisabled);
	}

	void MenuController::setFogOfWarDisabled(bool state) {
		BITMASK_SET(gameplayFlags, MenuControllerGameplayFlags::FogOfWarDisabled, state);
		updateFogOfWarRenderState();
	}

	bool MenuController::getMenusDisabled() const {
		return BITMASK_TEST(gameplayFlags, MenuControllerGameplayFlags::MenusDisabled);
	}

	void MenuController::setMenusDisabled(bool disabled) {
		BITMASK_SET(gameplayFlags, MenuControllerGameplayFlags::MenusDisabled, disabled);

		if (disabled) {
			UI::hideCursor();
			UI::closeDialogueMenu();
		}
	}

	bool MenuController::getShowKillStats() const {
		return BITMASK_TEST(gameplayFlags, MenuControllerGameplayFlags::KillStats);
	}

	void MenuController::setShowKillStats(bool state) {
		BITMASK_SET(gameplayFlags, MenuControllerGameplayFlags::KillStats, state);
	}

	bool MenuController::getScriptsDisabled() const {
		return BITMASK_TEST(gameplayFlags, MenuControllerGameplayFlags::ScriptsDisabled);
	}

	void MenuController::setScriptsDisabled(bool state) {
		BITMASK_SET(gameplayFlags, MenuControllerGameplayFlags::ScriptsDisabled, state);
	}

	bool MenuController::getShowPathGrid() const {
		return BITMASK_TEST(gameplayFlags, MenuControllerGameplayFlags::ShowPathGrid);
	}

	void MenuController::setShowPathGrid(bool show) {
		BITMASK_SET(gameplayFlags, MenuControllerGameplayFlags::ShowPathGrid, show);

		const auto dataHandler = TES3::DataHandler::get();
		if (dataHandler->currentInteriorCell) {
			const auto pathGrid = dataHandler->currentInteriorCell->pathGrid;
			if (pathGrid) {
				if (show) {
					pathGrid->show();
				}
				else {
					pathGrid->hide();
				}
			}
		}
		else {
			for (size_t i = 0; i < 9; ++i) {
				auto cellDataPointer = dataHandler->exteriorCellData[i];
				if (cellDataPointer && cellDataPointer->loadingFlags >= 1 && cellDataPointer->cell->pathGrid) {
					const auto pathGrid = cellDataPointer->cell->pathGrid;
					if (pathGrid) {
						if (show) {
							pathGrid->show();
						}
						else {
							pathGrid->hide();
						}
					}
				}
			}
		}
	}

	std::reference_wrapper<FontColor[FontColorId::MAX_ID + 1]> MenuController::getFontColors() {
		return std::ref(fontColors);
	}
}