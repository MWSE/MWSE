#include "TES3UIManagerLua.h"

#include "TES3GameSetting.h"
#include "TES3Item.h"
#include "TES3ItemData.h"
#include "TES3MobileActor.h"
#include "TES3MobilePlayer.h"
#include "TES3NPC.h"
#include "TES3Reference.h"
#include "TES3WorldController.h"

#include "TES3UIElement.h"
#include "TES3UIManager.h"
#include "TES3UIMenuController.h"

#include "LuaUtil.h"

#include "LuaManager.h"
#include "Log.h"

namespace mwse {
	namespace lua {
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
		static std::unordered_map<Element*, std::unordered_map<Property, std::vector<EventLuaCallback>>> eventCallbacksAfter;
		static std::unordered_map<Element*, std::unordered_map<Property, EventCallback>> originalCallbackMap;
		static std::unordered_map<Element*, void(__cdecl*)(Element*)> destroyMap;

		struct LegacyEventLuaCallback {
			Property id;
			sol::protected_function callback;
		};
		static std::unordered_map<Element*, std::vector<LegacyEventLuaCallback>> legacyEventMap;

		bool __cdecl eventDispatcher(Element* owningWidget, Property eventID, int data0, int data1, Element* source) {
			LuaManager& luaManager = LuaManager::getInstance();
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

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
					sol::table eventData = state.create_table();
					eventData["forwardSource"] = source;
					eventData["source"] = target;
					eventData["widget"] = owningWidget;
					eventData["id"] = eventID;
					eventData["data0"] = data0;
					eventData["data1"] = data1;

					// For mouse events, convert screen coordinates to element relative coordinates.
					if (eventID >= Property::event_mouse_leave && eventID <= Property::event_mouse_release) {
						eventData["relativeX"] = data0 - target->cached_screenX;
						eventData["relativeY"] = target->cached_screenY - data1;
					}

					for (const auto& eventLua : iterCallback->second) {
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
							const char* errorSource = target->name.cString ? target->name.cString : "(unnamed)";
							log::getLog() << "Lua error encountered during UI before-event from element " << errorSource << ":" << std::endl << error.what() << std::endl;
							return true;
						}
					}
				}
			}

			// Run our legacy event if applicable.
			bool legacyUsed = false;
			auto iterElements = legacyEventMap.find(target);
			if (iterElements != legacyEventMap.end()) {
				for (const auto& eventLua : iterElements->second) {
					if (eventLua.id == eventID) {
						legacyUsed = true;

						sol::table eventData = state.create_table();
						eventData["forwardSource"] = source;
						eventData["source"] = target;
						eventData["widget"] = owningWidget;
						eventData["id"] = eventID;
						eventData["data0"] = data0;
						eventData["data1"] = data1;

						// For mouse events, convert screen coordinates to element relative coordinates.
						if (eventID >= Property::event_mouse_leave && eventID <= Property::event_mouse_release) {
							eventData["relativeX"] = data0 - target->cached_screenX;
							eventData["relativeY"] = target->cached_screenY - data1;
						}

						// Note: sol::protected_function needs to be a local, as Lua functions can destroy it when modifying events.
						sol::protected_function callback = eventLua.callback;
						sol::protected_function_result result = callback(eventData);
						if (result.valid()) {
							// Return boolean results
							sol::optional<bool> bool_result = result;
							if (bool_result) {
								return bool_result.value();
							}
							break;
						}
						else {
							sol::error error = result;
							const char *errorSource = target->name.cString ? target->name.cString : "(unnamed)";
							log::getLog() << "Lua error encountered during UI event from element " << errorSource << ":" << std::endl << error.what() << std::endl;
							return true;
						}
					}
				}
			}

			// Run our original event if no legacy was used.
			if (!legacyUsed && eventID != Property::event_destroy) {
				auto original = originalCallbackMap[target][eventID];
				if (original) {
					original(target, eventID, data0, data1, source);
				}
			}

			// TODO: After events.
			auto iterAfterElements = eventCallbacksAfter.find(target);
			if (iterAfterElements != eventCallbacksAfter.end()) {
				auto iterCallback = iterAfterElements->second.find(eventID);
				if (iterCallback != iterAfterElements->second.end()) {
					sol::table eventData = state.create_table();
					eventData["forwardSource"] = source;
					eventData["source"] = target;
					eventData["widget"] = owningWidget;
					eventData["id"] = eventID;
					eventData["data0"] = data0;
					eventData["data1"] = data1;

					// For mouse events, convert screen coordinates to element relative coordinates.
					if (eventID >= Property::event_mouse_leave && eventID <= Property::event_mouse_release) {
						eventData["relativeX"] = data0 - target->cached_screenX;
						eventData["relativeY"] = target->cached_screenY - data1;
					}

					for (const auto& eventLua : iterCallback->second) {
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
							const char* errorSource = target->name.cString ? target->name.cString : "(unnamed)";
							log::getLog() << "Lua error encountered during UI after-event from element " << errorSource << ":" << std::endl << error.what() << std::endl;
							return true;
						}
					}
				}
			}

			return true;
		}

		void __cdecl eventDestroyDispatcher(Element* source) {
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// 
			eventDispatcher(source, Property::event_destroy, 0, 0, source);

			// Call original destroy callback
			auto callback = destroyMap[source];
			if (callback) {
				callback(source);
			}

			// Remove event mappings
			eventCallbacksBefore.erase(source);
			eventCallbacksAfter.erase(source);
			legacyEventMap.erase(source);
			originalCallbackMap.erase(source);
			destroyMap.erase(source);
		}

		void registerBeforeUIEvent(TES3::UI::Element* target, TES3::UI::Property eventID, sol::protected_function callback, double priority) {
			auto& properties = eventCallbacksBefore[target];
			auto& callbacks = properties[eventID];

			auto pos = std::find_if(callbacks.begin(), callbacks.end(), [priority](auto& s) { return s.priority < priority; });
			callbacks.insert(pos, { callback, priority });

			// Set a destroy hook the first time an event is added to an Element to allow cleanup
			// This callback uses PropertyType::Pointer instead of PropertyType::EventCallback
			if (destroyMap.find(target) == destroyMap.end()) {
				auto prevDestroy = target->getProperty(PropertyType::Pointer, Property::event_destroy).ptrValue;
				target->setProperty(Property::event_destroy, static_cast<void*>(&eventDestroyDispatcher));
				destroyMap[target] = static_cast<void(__cdecl*)(Element*)>(prevDestroy);
			}

			// Forward the event to our dispatcher.
			if (eventID != Property::event_destroy) {
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

			auto pos = std::find_if(callbacks.begin(), callbacks.end(), [priority](auto& s) { return s.priority < priority; });
			callbacks.insert(pos, { callback, priority });

			// Set a destroy hook the first time an event is added to an Element to allow cleanup
			// This callback uses PropertyType::Pointer instead of PropertyType::EventCallback
			if (destroyMap.find(target) == destroyMap.end()) {
				auto prevDestroy = target->getProperty(PropertyType::Pointer, Property::event_destroy).ptrValue;
				target->setProperty(Property::event_destroy, static_cast<void*>(&eventDestroyDispatcher));
				destroyMap[target] = static_cast<void(__cdecl*)(Element*)>(prevDestroy);
			}

			// Forward the event to our dispatcher.
			if (eventID != Property::event_destroy) {
				auto prevCallback = target->getProperty(PropertyType::EventCallback, eventID).eventCallback;
				if (prevCallback != &eventDispatcher) {
					originalCallbackMap[target][eventID] = prevCallback;
					target->setProperty(eventID, &eventDispatcher);
				}
			}
		}

		void registerUIEvent(Element* target, Property eventID, sol::protected_function callback) {
			// Save original callback, if present
			auto prevCallback = target->getProperty(PropertyType::EventCallback, eventID).eventCallback;
			if (prevCallback == eventDispatcher) {
				return;
			}

			// Set a destroy hook the first time an event is added to an Element to allow cleanup
			// This callback uses PropertyType::Pointer instead of PropertyType::EventCallback
			if (destroyMap.find(target) == destroyMap.end()) {
				auto prevDestroy = target->getProperty(PropertyType::Pointer, Property::event_destroy).ptrValue;
				target->setProperty(Property::event_destroy, static_cast<void*>(&eventDestroyDispatcher));
				destroyMap[target] = static_cast<void (__cdecl*)(Element*)>(prevDestroy);
			}

			// Forward the event to our dispatcher.
			if (eventID != Property::event_destroy) {
				originalCallbackMap[target][eventID] = prevCallback;
				target->setProperty(eventID, &eventDispatcher);
			}

			// Add the event.
			auto& elementEvents = legacyEventMap[target];
			elementEvents.push_back(LegacyEventLuaCallback{ eventID, callback });
		}

		bool unregisterBeforeUIEvent(Element* target, Property eventID, sol::protected_function callback) {
			const auto& targetItt = eventCallbacksBefore.find(target);
			if (targetItt != eventCallbacksBefore.end()) {
				const auto& propertyItt = targetItt->second.find(eventID);
				if (propertyItt != targetItt->second.end()) {
					// Search for our callback.
					for (auto& itt = propertyItt->second.begin(); itt != propertyItt->second.end(); itt++) {
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
					for (auto& itt = propertyItt->second.begin(); itt != propertyItt->second.end(); itt++) {
						if (itt->callback == callback) {
							propertyItt->second.erase(itt);
							return true;
						}
					}
				}
			}

			return false;
		}

		void unregisterUIEvent(Element* target, Property eventID) {
			auto& elementEvents = legacyEventMap[target];

			// Check for existing event
			for (auto it = elementEvents.begin(); it != elementEvents.end(); ++it) {
				if (it->id == eventID) {
					// Remove event
					elementEvents.erase(it);
					return;
				}
			}
		}

		bool eventForwarder(sol::table eventData) {
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

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

		void bindTES3UIManager() {
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;
			auto tes3ui = state.create_named_table("tes3ui");

			tes3ui["acquireTextInput"] = TES3::UI::acquireTextInput;
			tes3ui["captureMouseDrag"] = TES3::UI::captureMouseDrag;
			tes3ui["closeJournal"] = TES3::UI::closeJournal;
			tes3ui["createHelpLayerMenu"] = TES3::UI::createHelpLayerMenu_lua;
			tes3ui["createMenu"] = TES3::UI::createMenu_lua;
			tes3ui["createTooltipMenu"] = TES3::UI::createTooltipMenu_lua;
			tes3ui["enterMenuMode"] = TES3::UI::enterMenuMode_lua;
			tes3ui["findHelpLayerMenu"] = TES3::UI::findHelpLayerMenu_lua;
			tes3ui["findMenu"] = TES3::UI::findMenu_lua;
			tes3ui["forcePlayerInventoryUpdate"] = TES3::UI::forcePlayerInventoryUpdate;
			tes3ui["getInventorySelectType"] = TES3::UI::getInventorySelectType;
			tes3ui["getMenuOnTop"] = TES3::UI::getMenuOnTop;
			tes3ui["getPalette"] = TES3::UI::getPalette_lua;
			tes3ui["getServiceActor"] = TES3::UI::getServiceActor;
			tes3ui["leaveMenuMode"] = TES3::UI::leaveMenuMode;
			tes3ui["logToConsole"] = TES3::UI::logToConsole_lua;
			tes3ui["lookupID"] = TES3::UI::lookupID;
			tes3ui["menuMode"] = TES3::UI::isInMenuMode;
			tes3ui["refreshTooltip"] = TES3::UI::refreshTooltip;
			tes3ui["registerID"] = TES3::UI::registerID;
			tes3ui["registerProperty"] = TES3::UI::registerProperty;
			tes3ui["showBookMenu"] = TES3::UI::showBookMenu;
			tes3ui["showDialogueMessage"] = TES3::UI::showDialogueMessage_lua;
			tes3ui["showInventorySelectMenu"] = TES3::UI::showInventorySelectMenu_lua;
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
}
