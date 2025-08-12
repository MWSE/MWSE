#include "TES3UIManagerLua.h"

#include "TES3GameSetting.h"
#include "TES3Item.h"
#include "TES3ItemData.h"
#include "TES3MobileActor.h"
#include "TES3MobilePlayer.h"
#include "TES3NPC.h"
#include "TES3Reference.h"
#include "TES3WorldController.h"
#include "TES3InputController.h"

#include "TES3UIElement.h"
#include "TES3UIInventoryTile.h"
#include "TES3UIManager.h"
#include "TES3UIMenuController.h"

#include "LuaUtil.h"

#include "LuaManager.h"
#include "Log.h"

namespace mwse::lua {
	using TES3::UI::Element;
	using TES3::UI::UI_ID;
	using TES3::UI::Property;
	using TES3::UI::PropertyType;
	using TES3::UI::EventCallback;

	struct EventLuaCallback {
		sol::protected_function callback;
		double priority;
	};
	static std::unordered_map<Element*, std::unordered_map<Property, std::vector<EventLuaCallback>>> eventCallbacksBefore;
	static std::unordered_map<Element*, std::unordered_map<Property, sol::protected_function>> eventCallbacksLegacy;
	static std::unordered_map<Element*, std::unordered_map<Property, std::vector<EventLuaCallback>>> eventCallbacksAfter;
	static std::unordered_map<Element*, std::unordered_map<Property, EventCallback>> originalCallbackMap;
	static std::unordered_map<Element*, void(__cdecl*)(Element*)> destroyMap;

	static sol::table createKeyData(sol::state& state, int keyCode) {
		const auto inputController = TES3::WorldController::get()->inputController;
		auto keyData = state.create_table();
		keyData["keyCode"] = keyCode;
		keyData["isShiftDown"] = inputController->isShiftDown();
		keyData["isControlDown"] = inputController->isControlDown();
		keyData["isAltDown"] = inputController->isAltDown();
		keyData["isSuperDown"] = inputController->isSuperDown();
		return keyData;
	}

	static sol::table createEventTable(sol::state& state, Element* owningWidget, Property eventID, int data0, int data1, Element* source, Element* target) {
		auto eventData = state.create_table();
		eventData["forwardSource"] = source;
		eventData["source"] = target;
		eventData["widget"] = owningWidget;
		eventData["id"] = eventID;
		eventData["data0"] = data0;
		eventData["data1"] = data1;

		switch (eventID) {
		case Property::event_mouse_leave:
		case Property::event_mouse_over:
		case Property::event_mouse_down:
		case Property::event_mouse_click:
		case Property::event_mouse_scroll_up:
		case Property::event_mouse_scroll_down:
		case Property::event_mouse_double_click:
		case Property::event_mouse_still_idle:
		case Property::event_mouse_still_over:
		case Property::event_mouse_still_pressed_outside:
		case Property::event_mouse_still_pressed:
		case Property::event_mouse_release:
			// For mouse events, convert screen coordinates to element relative coordinates.
			eventData["relativeX"] = data0 - target->cached_screenX;
			eventData["relativeY"] = target->cached_screenY - data1;
			break;
		case Property::event_key_press:
			// Note that this is a DirectInput key code, not a scan code.
			const auto charMasked = data0 & 0x7FFFFFFF;
			eventData["character"] = charMasked > 0 ? state["string"]["char"](charMasked) : sol::object(sol::nil);
			eventData["keyData"] = createKeyData(state, TES3::UI::MenuInputController::lastKeyPressDIK);
			break;
		}

		return eventData;
	}

	bool __cdecl eventDispatcher(Element* owningWidget, Property eventID, int data0, int data1, Element* source) {
		LuaManager& luaManager = LuaManager::getInstance();
		const auto stateHandle = luaManager.getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Find dispatch target. Almost always source, but is owningWidget for 'focus' and 'unfocus' events.
		Element* target = source;

		// Handle event bubbling.
		if (eventID != Property::event_destroy) {
			if (eventID == Property::event_focus || eventID == Property::event_unfocus) {
				target = owningWidget;
			}

			while (target) {
				if (target->getProperty(PropertyType::Property, eventID).propertyValue == Property::inherit
					|| target->getProperty(PropertyType::EventCallback, eventID).eventCallback == nullptr) {

					target = target->parent;
				}
				else {
					break;
				}
			}
		}

		// TODO: Before events.
		auto iterBeforeElements = eventCallbacksBefore.find(target);
		if (iterBeforeElements != eventCallbacksBefore.end()) {
			auto iterCallback = iterBeforeElements->second.find(eventID);
			if (iterCallback != iterBeforeElements->second.end()) {
				auto eventData = createEventTable(state, owningWidget, eventID, data0, data1, source, target);

				auto& callbacks = iterCallback->second;
				for (const auto& eventLua : callbacks) {
					// Note: sol::protected_function needs to be a local, as Lua functions can destroy it when modifying events.
					sol::protected_function callback = eventLua.callback;
					sol::protected_function_result result = callback(eventData);
					if (result.valid()) {
						// Return boolean results
						sol::optional<bool> bool_result = result;
						if (bool_result) {
							return bool_result.value();
						}
					}
					else {
						sol::error error = result;
						char errorSource[256];
						std::snprintf(errorSource, sizeof(errorSource), "uiElement %s", target->name.cString ? target->name.cString : "(unnamed)");
						log::getLog() << "Lua error encountered during UI before-event from " << errorSource << ":" << std::endl << error.what() << std::endl;
						reportErrorInGame(errorSource, error);
						return true;
					}
				}
			}
		}

		// Run our legacy event if applicable.
		bool legacyUsed = false;
		auto iterElements = eventCallbacksLegacy.find(target);
		if (iterElements != eventCallbacksLegacy.end()) {
			auto iterCallback = iterElements->second.find(eventID);
			if (iterCallback != iterElements->second.end()) {
				legacyUsed = true;

				auto eventData = createEventTable(state, owningWidget, eventID, data0, data1, source, target);

				// Note: sol::protected_function needs to be a local, as Lua functions can destroy it when modifying events.
				sol::protected_function callback = iterCallback->second;
				sol::protected_function_result result = callback(eventData);
				if (result.valid()) {
					// Return boolean results
					sol::optional<bool> bool_result = result;
					if (bool_result) {
						return bool_result.value();
					}
				}
				else {
					sol::error error = result;
					char errorSource[256];
					std::snprintf(errorSource, sizeof(errorSource), "uiElement %s", target->name.cString ? target->name.cString : "(unnamed)");
					log::getLog() << "Lua error encountered during UI event from " << errorSource << ":" << std::endl << error.what() << std::endl;
					reportErrorInGame(errorSource, error);
					return true;
				}
			}
		}

		// Run our original event if no legacy was used.
		if (!legacyUsed) {
			if (eventID == Property::event_destroy) {
				auto callback = destroyMap[source];
				if (callback) {
					callback(source);
				}
			}
			else {
				auto original = originalCallbackMap[target][eventID];
				if (original) {
					original(target, eventID, data0, data1, source);
				}
			}
		}

		// TODO: After events.
		auto iterAfterElements = eventCallbacksAfter.find(target);
		if (iterAfterElements != eventCallbacksAfter.end()) {
			auto iterCallback = iterAfterElements->second.find(eventID);
			if (iterCallback != iterAfterElements->second.end()) {
				auto eventData = createEventTable(state, owningWidget, eventID, data0, data1, source, target);

				auto& callbacks = iterCallback->second;
				for (const auto& eventLua : callbacks) {
					// Note: sol::protected_function needs to be a local, as Lua functions can destroy it when modifying events.
					sol::protected_function callback = eventLua.callback;
					sol::protected_function_result result = callback(eventData);
					if (result.valid()) {
						// Return boolean results
						sol::optional<bool> bool_result = result;
						if (bool_result) {
							return bool_result.value();
						}
					}
					else {
						sol::error error = result;
						char errorSource[256];
						std::snprintf(errorSource, sizeof(errorSource), "uiElement %s", target->name.cString ? target->name.cString : "(unnamed)");
						log::getLog() << "Lua error encountered during UI after-event from " << errorSource << ":" << std::endl << error.what() << std::endl;
						reportErrorInGame(errorSource, error);
						return true;
					}
				}
			}
		}

		return true;
	}

	void __cdecl eventDestroyDispatcher(Element* source) {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Send off our event.
		eventDispatcher(source, Property::event_destroy, 0, 0, source);
	}

	void registerBeforeUIEvent(TES3::UI::Element* target, TES3::UI::Property eventID, sol::protected_function callback, double priority) {
		auto& properties = eventCallbacksBefore[target];
		auto& callbacks = properties[eventID];

		auto existing = std::find_if(callbacks.begin(), callbacks.end(), [&](auto& s) { return s.callback == callback && s.priority == priority; });
		if (existing != callbacks.end()) {
			return;
		}

		auto pos = std::find_if(callbacks.begin(), callbacks.end(), [priority](auto& s) { return s.priority < priority; });
		callbacks.insert(pos, { callback, priority });

		// Forward the event to our dispatcher.
		if (eventID == Property::event_destroy) {
			auto prevDestroy = static_cast<void(__cdecl*)(Element*)>(target->getProperty(PropertyType::Pointer, Property::event_destroy).ptrValue);
			if (prevDestroy && prevDestroy != &eventDestroyDispatcher) {
				destroyMap[target] = prevDestroy;
			}
			target->setProperty(Property::event_destroy, static_cast<void*>(&eventDestroyDispatcher));
		} else {
			auto prevCallback = target->getProperty(PropertyType::EventCallback, eventID).eventCallback;
			if (prevCallback != &eventDispatcher) {
				originalCallbackMap[target][eventID] = prevCallback;
				target->setProperty(eventID, &eventDispatcher);
			}
		}
	}

	void registerAfterUIEvent(TES3::UI::Element* target, TES3::UI::Property eventID, sol::protected_function callback, double priority) {
		auto& properties = eventCallbacksAfter[target];
		auto& callbacks = properties[eventID];

		auto existing = std::find_if(callbacks.begin(), callbacks.end(), [&](auto& s) { return s.callback == callback && s.priority == priority; });
		if (existing != callbacks.end()) {
			return;
		}

		auto pos = std::find_if(callbacks.begin(), callbacks.end(), [priority](auto& s) { return s.priority < priority; });
		callbacks.insert(pos, { callback, priority });

		// Forward the event to our dispatcher.
		if (eventID == Property::event_destroy) {
			auto prevDestroy = static_cast<void(__cdecl*)(Element*)>(target->getProperty(PropertyType::Pointer, Property::event_destroy).ptrValue);
			if (prevDestroy && prevDestroy != &eventDestroyDispatcher) {
				destroyMap[target] = prevDestroy;
			}
			target->setProperty(Property::event_destroy, static_cast<void*>(&eventDestroyDispatcher));
		} else {
			auto prevCallback = target->getProperty(PropertyType::EventCallback, eventID).eventCallback;
			if (prevCallback != &eventDispatcher) {
				originalCallbackMap[target][eventID] = prevCallback;
				target->setProperty(eventID, &eventDispatcher);
			}
		}
	}

	void registerUIEvent(Element* target, Property eventID, sol::protected_function callback) {
		// Forward the event to our dispatcher.
		if (eventID == Property::event_destroy) {
			auto prevDestroy = static_cast<void(__cdecl*)(Element*)>(target->getProperty(PropertyType::Pointer, Property::event_destroy).ptrValue);
			if (prevDestroy && prevDestroy != eventDestroyDispatcher) {
				destroyMap[target] = prevDestroy;
			}
			target->setProperty(Property::event_destroy, static_cast<void*>(&eventDestroyDispatcher));
		} else {
			auto prevCallback = target->getProperty(PropertyType::EventCallback, eventID).eventCallback;
			if (prevCallback != &eventDispatcher) {
				originalCallbackMap[target][eventID] = prevCallback;
				target->setProperty(eventID, &eventDispatcher);
			}
		}

		// Add the event.
		eventCallbacksLegacy[target][eventID] = callback;
	}

	bool unregisterBeforeUIEvent(Element* target, Property eventID, sol::protected_function callback) {
		const auto& targetItt = eventCallbacksBefore.find(target);
		if (targetItt != eventCallbacksBefore.end()) {
			const auto& propertyItt = targetItt->second.find(eventID);
			if (propertyItt != targetItt->second.end()) {
				// Search for our callback.
				for (auto itt = propertyItt->second.begin(); itt != propertyItt->second.end(); ++itt) {
					if (itt->callback == callback) {
						propertyItt->second.erase(itt);
						return true;
					}
				}
			}
		}

		return false;
	}

	bool unregisterAfterUIEvent(Element* target, Property eventID, sol::protected_function callback) {
		const auto& targetItt = eventCallbacksAfter.find(target);
		if (targetItt != eventCallbacksAfter.end()) {
			const auto& propertyItt = targetItt->second.find(eventID);
			if (propertyItt != targetItt->second.end()) {
				// Search for our callback.
				for (auto itt = propertyItt->second.begin(); itt != propertyItt->second.end(); ++itt) {
					if (itt->callback == callback) {
						propertyItt->second.erase(itt);
						return true;
					}
				}
			}
		}

		return false;
	}

	bool unregisterUIEvent(Element* target, Property eventID) {
		auto iterCallbacks = eventCallbacksLegacy.find(target);
		if (iterCallbacks != eventCallbacksLegacy.end()) {
			return iterCallbacks->second.erase(eventID);
		}

		return false;
	}

	bool eventForwarder(sol::table eventData) {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		Element* source = eventData["forwardSource"];
		Element* owningWidget = eventData["widget"];
		Property eventID = eventData["id"];
		int data0 = eventData["data0"];
		int data1 = eventData["data1"];

		// Use dispatch target resolved by eventDispatcher().
		Element* target = eventData["source"];
		auto& original = originalCallbackMap[target][eventID];
		if (original) {
			original(owningWidget, eventID, data0, data1, source);
		}

		return true;
	}

	void triggerEvent(Element* target, Property eventID, int data0, int data1) {
		auto callback = target->getProperty(PropertyType::EventCallback, eventID).eventCallback;
		if (callback) {
			callback(target, eventID, data0, data1, target);
		}
	}

	void cleanupEventRegistrations(TES3::UI::Element* element) {
		eventCallbacksBefore.erase(element);
		eventCallbacksAfter.erase(element);
		eventCallbacksLegacy.erase(element);
		originalCallbackMap.erase(element);
		destroyMap.erase(element);
	}

	void copyLuaCallbacks(TES3::UI::Element* from, TES3::UI::Element* to) {
		if (auto itt = eventCallbacksBefore.find(from); itt != eventCallbacksBefore.end()) {
			eventCallbacksBefore[to] = itt->second;
		}
		if (auto itt = eventCallbacksLegacy.find(from); itt != eventCallbacksLegacy.end()) {
			eventCallbacksLegacy[to] = itt->second;
		}
		if (auto itt = eventCallbacksAfter.find(from); itt != eventCallbacksAfter.end()) {
			eventCallbacksAfter[to] = itt->second;
		}
		if (auto itt = originalCallbackMap.find(from); itt != originalCallbackMap.end()) {
			originalCallbackMap[to] = itt->second;
		}
		if (auto itt = destroyMap.find(from); itt != destroyMap.end()) {
			destroyMap[to] = itt->second;
		}
	}

	void bindTES3UIManager() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto tes3ui = state.create_named_table("tes3ui");

		tes3ui["acquireTextInput"] = TES3::UI::acquireTextInput;
		tes3ui["captureMouseDrag"] = TES3::UI::captureMouseDrag;
		tes3ui["choice"] = TES3::UI::choice;
		tes3ui["closeBookMenu"] = TES3::UI::closeBookMenu;
		tes3ui["closeJournal"] = TES3::UI::closeJournal;
		tes3ui["closeScrollMenu"] = TES3::UI::closeScrollMenu;
		tes3ui["createHelpLayerMenu"] = TES3::UI::createHelpLayerMenu_lua;
		tes3ui["createMenu"] = TES3::UI::createMenu_lua;
		tes3ui["createResponseText"] = TES3::UI::createResponseText_lua;
		tes3ui["createTooltipMenu"] = TES3::UI::createTooltipMenu_lua;
		tes3ui["enterMenuMode"] = TES3::UI::enterMenuMode_lua;
		tes3ui["findHelpLayerMenu"] = TES3::UI::findHelpLayerMenu_lua;
		tes3ui["findMenu"] = TES3::UI::findMenu_lua;
		tes3ui["forcePlayerInventoryUpdate"] = TES3::UI::forcePlayerInventoryUpdate;
		tes3ui["getConsoleReference"] = TES3::UI::getConsoleReference;
		tes3ui["getCursor"] = TES3::UI::getCursor;
		tes3ui["getCursorTile"] = TES3::UI::getCursorTile;
		tes3ui["getInventorySelectType"] = TES3::UI::getInventorySelectType;
		tes3ui["getMenuOnTop"] = TES3::UI::getMenuOnTop;
		tes3ui["getPalette"] = TES3::UI::getPalette_lua;
		tes3ui["getServiceActor"] = TES3::UI::getServiceActor;
		tes3ui["getViewportScale"] = TES3::UI::getViewportScale;
		tes3ui["getViewportSize"] = TES3::UI::getViewportSize_lua;
		tes3ui["leaveMenuMode"] = TES3::UI::leaveMenuMode;
		tes3ui["logToConsole"] = TES3::UI::logToConsole_lua;
		tes3ui["lookupID"] = TES3::UI::lookupID_lua;
		tes3ui["menuMode"] = TES3::UI::isInMenuMode;
		tes3ui["moveMenuToFront"] = TES3::UI::moveMenuToFront_lua;
		tes3ui["refreshTooltip"] = TES3::UI::refreshTooltip;
		tes3ui["registerID"] = TES3::UI::registerID;
		tes3ui["registerProperty"] = TES3::UI::registerProperty;
		tes3ui["setConsoleReference"] = TES3::UI::setConsoleReference;
		tes3ui["showBookMenu"] = TES3::UI::showBookMenu;
		tes3ui["showDialogueMessage"] = TES3::UI::showDialogueMessage_lua;
		tes3ui["showInventorySelectMenu"] = TES3::UI::showInventorySelectMenu_lua;
		tes3ui["showJournal"] = TES3::UI::showJournal;
		tes3ui["showMagicSelectMenu"] = TES3::UI::showMagicSelectMenu_lua;
		tes3ui["showScrollMenu"] = TES3::UI::showScrollMenu;
		tes3ui["stealHelpMenu"] = TES3::UI::stealHelpMenu;
		tes3ui["suppressTooltip"] = TES3::UI::setSuppressingHelpMenu;
		tes3ui["updateBarterMenuTiles"] = TES3::UI::updateBarterMenuTiles;
		tes3ui["updateContentsMenuTiles"] = TES3::UI::updateContentsMenuTiles;
		tes3ui["updateDialogDisposition"] = TES3::UI::updateDialogDisposition;
		tes3ui["updateEnchantingMenu"] = TES3::UI::updateEnchantingMenu;
		tes3ui["updateInventoryCharacterImage"] = TES3::UI::updateInventoryCharacterImage;
		tes3ui["updateInventorySelectTiles"] = TES3::UI::updateInventorySelectTiles_lua;
		tes3ui["updateInventoryTiles"] = TES3::UI::updateInventoryMenuTiles;
		tes3ui["updateSpellmakingMenu"] = TES3::UI::updateSpellmakingMenu;
		tes3ui["updateStatsPane"] = TES3::UI::updateStatsPane;

		auto textLayout = state.create_table();
		tes3ui["textLayout"] = textLayout;
		textLayout["getFontHeight"] = TES3::UI::textLayoutGetFontHeight_lua;
		textLayout["getTextExtent"] = TES3::UI::textLayoutGetTextExtent_lua;
		textLayout["wrapText"] = TES3::UI::textLayoutWrapText_lua;

		// Internal mwse functions.
		state["mwse"]["copyLuaCallbacks"] = copyLuaCallbacks;

		// Add binding for TES3::UI::TreeItem type.
		// TODO: Move this to its own file after TES3::UI::Tree has been made a template.
		{
			auto usertypeDefinition = state.new_usertype<TES3::UI::TreeItem>("tes3uiProperty");

			usertypeDefinition["id"] = &TES3::UI::TreeItem::key;
			usertypeDefinition["name"] = sol::readonly_property(&TES3::UI::TreeItem::getName);
			usertypeDefinition["type"] = sol::readonly_property(&TES3::UI::TreeItem::getType);
			usertypeDefinition["value"] = sol::readonly_property(&TES3::UI::TreeItem::getValue_lua);
		}
	}
}
