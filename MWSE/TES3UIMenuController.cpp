#include "TES3UIMenuController.h"

#include "MemoryUtil.h"

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

	const auto TES3_MenuInputController_flushBufferedTextEvents = reinterpret_cast<void(__thiscall*)(MenuInputController*)>(0x58E9C0);
	void MenuInputController::flushBufferedTextEvents() {
		TES3_MenuInputController_flushBufferedTextEvents(this);
	}

	//
	// Re-entrancy-safe replacement for MenuInputController::dispatchEvents (0x58F060). The engine
	// version leaves the event it is dispatching in the queue until the end of the iteration, so a
	// nested event pump run from inside a callback (e.g. the modal script-error box) dispatches it
	// again -- for a dialogue result script that fails to compile, that recurses until the stack
	// overflows -- and a nested mouseInput can overwrite the slot mid-iteration. This version
	// copies the event and clears its slot before any callback runs. In-flight copies form a stack
	// that patchInvalidateElementReferences scrubs, preserving the engine's protection against
	// dispatching to elements destroyed during a callback.
	//

	using TES3_ui_captureMouseDrag = se::memory::ExternalGlobal<bool, 0x7D207D>;
	using TES3_uiMainRoot = se::memory::ExternalGlobal<Element*, 0x7D1C28>;
	using TES3_ui_id_MenuConsole = se::memory::ExternalGlobal<UI_ID, 0x7D2ECC>;

	MenuInputController::DispatchingEvent* MenuInputController::lastDispatchingEvent = nullptr;

	const auto TES3_MenuInputController_checkMouseEventInChildren = reinterpret_cast<Element* (__thiscall*)(MenuInputController*, Element*, int, int)>(0x58EDE0);
	Element* MenuInputController::checkMouseEventInChildren(Element* element, int mouseX, int mouseY) {
		return TES3_MenuInputController_checkMouseEventInChildren(this, element, mouseX, mouseY);
	}

	bool MenuInputController::isDispatchTargetVisible(const Element* element) {
		return element && element->visibleAtLastUpdate;
	}

	// The engine converts the drag-release cursor position with two truncations: ftol(v), then
	// ftol(ftol(v) + 0.5). For negative coordinates (UI space is center-origin) this is one higher
	// than a single truncation; replicate it exactly.
	int MenuInputController::truncateCursorCoordinate(float value) {
		const auto truncated = static_cast<int>(value);
		return static_cast<int>(truncated + 0.5f);
	}

	void MenuInputController::dispatchMouseReleaseEvent(DispatchingEvent& event, Element* element, bool visible, bool isSameSourceAsLastButtonPress) {
		if (!isMouseButtonHeldDown) {
			return;
		}

		// Commit the released state before any callback runs, for the same reason dispatchEvents
		// commits the queue slot: a nested event pump (e.g. the script error box) re-synthesizes a
		// mouse release from the still-latched input edge, and it must not count as another click.
		// The engine version clears this only after its callbacks return.
		isMouseButtonHeldDown = false;

		if (isSameSourceAsLastButtonPress && visible && !TES3_ui_captureMouseDrag::get()) {
			// Plain click on the pressed element.
			element->dispatchInputEvent(Property::event_mouse_click, event.data0, event.data1, element);
			if (isDispatchTargetVisible(event.element)) {
				event.element->dispatchInputEvent(Property::event_mouse_over, event.data0, event.data1, event.element);
			}
			return;
		}

		if (isSameSourceAsLastButtonPress) {
			if (visible && TES3_ui_captureMouseDrag::get()) {
				// Drag release: hit-test what the cursor is over to decide between click and release.
				const auto mouseController = TES3::WorldController::get()->mouseController;
				const int mouseX = truncateCursorCoordinate(mouseController->position.x);
				const int mouseY = truncateCursorCoordinate(mouseController->position.z);

				Element* elementUnderCursor = nullptr;
				const auto uiRoot = TES3_uiMainRoot::get();
				for (auto it = uiRoot->vectorChildren.end(); it != uiRoot->vectorChildren.begin(); --it) {
					const auto child = *(it - 1);
					if (!child->visible) {
						continue;
					}
					elementUnderCursor = checkMouseEventInChildren(child, mouseX, mouseY);
					if (elementUnderCursor) {
						break;
					}
					if (child->visible && child->flagConsumeMouseEvents && child->checkMouseEventInElement(mouseX, mouseY)) {
						elementUnderCursor = child;
						break;
					}
				}

				if (pointerMoveEventSource == elementUnderCursor) {
					event.element->dispatchInputEvent(Property::event_mouse_click, event.data0, event.data1, event.element);
					if (isDispatchTargetVisible(event.element)) {
						event.element->dispatchInputEvent(Property::event_mouse_over, event.data0, event.data1, event.element);
					}
				}
				else {
					pointerMovePreviousEventSource = pointerMoveEventSource;
					pointerMoveEventSource = elementUnderCursor;
					event.element->dispatchInputEvent(Property::event_mouse_release, event.data0, event.data1, event.element);
					if (isDispatchTargetVisible(event.element)) {
						event.element->dispatchInputEvent(Property::event_mouse_leave, event.data0, event.data1, event.element);
					}
				}
				TES3_ui_captureMouseDrag::set(false);
			}
			return;
		}

		// Released over a different element than the one pressed.
		const auto pressSource = buttonPressEventSource;
		if (!isDispatchTargetVisible(pressSource)) {
			return;
		}
		pressSource->dispatchInputEvent(Property::event_mouse_release, event.data0, event.data1, pressSource);
		const auto pressSourceAfterRelease = buttonPressEventSource;
		if (isDispatchTargetVisible(pressSourceAfterRelease)) {
			pressSourceAfterRelease->dispatchInputEvent(Property::event_mouse_leave, event.data0, event.data1, pressSourceAfterRelease);
		}
		if (isDispatchTargetVisible(event.element)) {
			event.element->dispatchInputEvent(Property::event_mouse_over, event.data0, event.data1, event.element);
		}
	}

	void MenuInputController::dispatchEvents() {
		for (int i = 0; i < 2; ++i) {
			auto& slot = events[i];
			if (slot.type == Event::Type::None) {
				break;
			}

			// Copy the event and clear its queue slot before any callback can run a nested pump.
			DispatchingEvent event(slot, lastDispatchingEvent);
			slot.type = Event::Type::None;
			slot.data0 = 0;
			slot.data1 = 0;
			slot.element = nullptr;
			lastDispatchingEvent = &event;

			const bool isSameSourceAsLastButtonPress = event.element == buttonPressEventSource;
			const auto menuOnTop = getMenuOnTop();

			// Let the menu on top know it is losing focus when another menu is clicked. The unfocus
			// callback can veto the event by returning false.
			if (isDispatchTargetVisible(event.element) && menuOnTop && event.element != menuOnTop && menuOnTop == buttonPressPreviousEventSource) {
				const auto topLevelMenu = event.element->getTopLevelParent();
				if (menuOnTop != topLevelMenu && topLevelMenu->id != TES3_ui_id_MenuConsole::get()) {
					PropertyValue propValue;
					const auto callback = menuOnTop->getProperty(&propValue, Property::event_unfocus, PropertyType::EventCallback, nullptr, false)->eventCallback;
					if (callback && !callback(menuOnTop, Property::event_unfocus, event.data0, event.data1, topLevelMenu)) {
						event.element = nullptr;
					}
				}
			}

			// The element and visibility the engine latches before dispatching. Post-callback logic
			// re-reads event.element instead, which invalidation may have cleared.
			const auto element = event.element;
			const bool visible = isDispatchTargetVisible(element);

			switch (event.type) {
			case Event::Type::KeyEnter:
			case Event::Type::KeyPress:
				if (visible) {
					const auto eventProperty = event.type == Event::Type::KeyEnter ? Property::event_key_enter : Property::event_key_press;
					element->dispatchInputEvent(eventProperty, event.data0, event.data1, element);
				}
				buttonPressEventSource = isDispatchTargetVisible(event.element) ? event.element : nullptr;
				break;
			case Event::Type::ScrollUp:
			case Event::Type::ScrollDown:
				if (visible) {
					const auto eventProperty = event.type == Event::Type::ScrollUp ? Property::event_mouse_scroll_up : Property::event_mouse_scroll_down;
					element->dispatchInputEvent(eventProperty, event.data0, event.data1, element);
				}
				break;
			case Event::Type::MousePress:
				if (!isMouseButtonHeldDown && visible) {
					element->dispatchInputEvent(Property::event_mouse_down, event.data0, event.data1, element);
				}
				isMouseButtonHeldDown = true;
				buttonPressEventSource = visible ? event.element : nullptr;
				break;
			case Event::Type::MouseRelease:
				dispatchMouseReleaseEvent(event, element, visible, isSameSourceAsLastButtonPress);
				break;
			case Event::Type::MouseEnter:
				if (!isMouseButtonHeldDown) {
					if (element) {
						element->dispatchInputEvent(Property::event_mouse_over, event.data0, event.data1, element);
					}
				}
				else if (isSameSourceAsLastButtonPress && element) {
					element->dispatchInputEvent(Property::event_mouse_down, event.data0, event.data1, element);
				}
				break;
			case Event::Type::MouseLeave:
				if (!isMouseButtonHeldDown) {
					if (element) {
						element->dispatchInputEvent(Property::event_mouse_leave, event.data0, event.data1, element);
					}
				}
				else if (isSameSourceAsLastButtonPress && element) {
					// While the button is held, leaving the pressed element reports "still over".
					element->dispatchInputEvent(Property::event_mouse_over, event.data0, event.data1, element);
				}
				break;
			case Event::Type::MouseDragMove:
				if (isMouseButtonHeldDown && isSameSourceAsLastButtonPress && element) {
					element->dispatchInputEvent(Property::event_mouse_still_pressed, event.data0, event.data1, element);
				}
				break;
			}

			buttonPressPreviousEventSource = menuOnTop;
			lastDispatchingEvent = event.previous;
		}

		if (TES3::WorldController::get()->menuController->flagClearHelpMenu) {
			clearAndHideHelpMenu();
		}
	}

	const auto TES3_MenuInputController_invalidateElementReferences = reinterpret_cast<void(__thiscall*)(MenuInputController*, Element*)>(0x58EE90);
	void MenuInputController::patchInvalidateElementReferences(Element* element) {
		// Call overwritten code: scrubs the element from the event queue and event source fields.
		TES3_MenuInputController_invalidateElementReferences(this, element);

		// Also scrub events dispatchEvents has copied out of the queue and is mid-dispatch on, so
		// their post-callback dispatches see the invalidation just like the engine's re-reads did.
		for (auto dispatching = lastDispatchingEvent; dispatching; dispatching = dispatching->previous) {
			if (dispatching->element == element) {
				dispatching->element = nullptr;
			}
		}
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
		constexpr UI_ID mainHelpLayerMenu = static_cast<UI_ID>(0x8105);
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
				if (cellDataPointer && cellDataPointer->isFullyLoaded() && cellDataPointer->cell->pathGrid) {
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