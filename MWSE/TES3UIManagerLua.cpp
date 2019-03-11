#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "TES3MobileActor.h"
#include "TES3MobilePlayer.h"
#include "TES3NPC.h"
#include "TES3WorldController.h"

#include "TES3UIElement.h"
#include "TES3UIManager.h"
#include "TES3UIMenuController.h"

#include "LuaUtil.h"

#include "sol.hpp"
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
			Property id;
			sol::protected_function callback;
			EventCallback original;
		};
		static std::unordered_map<Element*, std::vector<EventLuaCallback>> eventMap;
		static std::unordered_map<Element*, void(__cdecl*)(Element*)> destroyMap;

		TES3::UI::Boolean __cdecl eventDispatcher(Element* owningWidget, Property eventID, int data0, int data1, Element* source) {
			LuaManager& luaManager = LuaManager::getInstance();
			sol::state& state = luaManager.getState();

			// Handle inheritance
			Element* target = source;
			while (target && target->getProperty(PropertyType::Property, eventID).propertyValue == Property::inherit) {
				target = target->parent;
			}

			auto iterElements = eventMap.find(target);
			if (iterElements != eventMap.end()) {
				for (const auto& eventLua : iterElements->second) {
					if (eventLua.id == eventID) {
						sol::table eventData = luaManager.createTable();
						eventData["source"] = source;
						eventData["widget"] = owningWidget;
						eventData["id"] = eventID;
						eventData["data0"] = data0;
						eventData["data1"] = data1;

						// Note: sol::protected_function needs to be a local, as Lua functions can destroy it when modifying events.
						sol::protected_function callback = eventLua.callback;
						sol::protected_function_result result = callback(eventData);
						if (result.valid()) {
							sol::optional<TES3::UI::Boolean> value = result;
							return value.value_or(1);
						}
						else {
							sol::error error = result;
							const char *errorSource = source->name.cString ? source->name.cString : "(unnamed)";
							log::getLog() << "Lua error encountered during UI event from element " << errorSource << ":" << std::endl << error.what() << std::endl;
							return 1;
						}
					}
				}
			}

			return 1;
		}

		void __cdecl eventDestroyDispatcher(Element* source) {
			sol::state& state = LuaManager::getInstance().getState();

			// Dispatch Lua callback
			auto iterElements = eventMap.find(source);
			if (iterElements != eventMap.end()) {
				for (const auto& eventLua : iterElements->second) {
					if (eventLua.id == Property::event_destroy) {
						sol::table eventData = LuaManager::getInstance().createTable();
						eventData["source"] = source;
						eventData["id"] = Property::event_destroy;

						// Note: sol::protected_function needs to be a local, as Lua functions can destroy it when modifying events.
						sol::protected_function callback = eventLua.callback;
						sol::protected_function_result result = callback();
						if (!result.valid()) {
							sol::error error = result;
							const char *errorSource = source->name.cString ? source->name.cString : "(unnamed)";
							log::getLog() << "Lua error encountered during UI event from element " << errorSource << ":" << std::endl << error.what() << std::endl;
						}

						break;
					}
				}
			}

			// Call original destroy callback
			auto callback = destroyMap[source];
			if (callback) {
				callback(source);
			}

			// Remove event mappings
			eventMap.erase(source);
			destroyMap.erase(source);
		}

		void registerUIEvent(Element& target, Property eventID, sol::protected_function callback) {
			// Save original callback, if present
			auto prevCallback = target.getProperty(PropertyType::EventCallback, eventID).eventCallback;
			if (prevCallback == eventDispatcher) {
				prevCallback = nullptr;
			}

			if (eventMap.find(&target) == eventMap.end()) {
				// Set a destroy hook the first time an event is added to an Element to allow cleanup
				// This callback uses PropertyType::Pointer instead of PropertyType::EventCallback
				auto prevDestroy = target.getProperty(PropertyType::Pointer, Property::event_destroy).ptrValue;
				target.setProperty(Property::event_destroy, static_cast<void*>(&eventDestroyDispatcher));
				destroyMap[&target] = static_cast<void (__cdecl*)(Element*)>(prevDestroy);
			}

			// Forward the event to our dispatcher
			if (eventID != Property::event_destroy) {
				target.setProperty(eventID, &eventDispatcher);
			}

			// Check for existing event to replace
			auto& elementEvents = eventMap[&target];
			for (auto& eventPair : elementEvents) {
				if (eventPair.id == eventID) {
					eventPair.callback = callback;
					return;
				}
			}

			// Add new event to list
			elementEvents.push_back(EventLuaCallback{ eventID, callback, prevCallback });
		}

		void unregisterUIEvent(Element& target, Property eventID) {
			auto& elementEvents = eventMap[&target];

			// Check for existing event
			for (auto it = elementEvents.begin(); it != elementEvents.end(); ++it) {
				if (it->id == eventID) {
					// Restore callback
					if (eventID != Property::event_destroy) {
						target.setProperty(eventID, it->original);
					}

					// Remove event
					elementEvents.erase(it);
					return;
				}
			}
		}

		TES3::UI::Boolean eventForwarder(sol::table eventData) {
			sol::state& state = LuaManager::getInstance().getState();

			Element* source = eventData["source"];
			Element* owningWidget = eventData["widget"];
			Property eventID = eventData["id"];
			int data0 = eventData["data0"];
			int data1 = eventData["data1"];

			// Handle inheritance
			Element* target = source;
			while (target && target->getProperty(PropertyType::Property, eventID).propertyValue == Property::inherit) {
				target = target->parent;
			}

			auto iterElements = eventMap.find(target);
			if (iterElements != eventMap.end()) {
				for (const auto& eventLua : iterElements->second) {
					if (eventLua.id == eventID && eventLua.original) {
						// Call original callback
						return eventLua.original(owningWidget, eventID, data0, data1, source);
					}
				}
			}
			return 1;
		}

		void triggerEvent(Element& target, Property eventID, int data0, int data1) {
			auto callback = target.getProperty(PropertyType::EventCallback, eventID).eventCallback;
			if (callback) {
				callback(&target, eventID, data0, data1, &target);
			}
		}

		void bindTES3UIManager() {
			sol::state& state = LuaManager::getInstance().getState();
			auto tes3ui = state.create_named_table("tes3ui");

			tes3ui["registerID"] = TES3::UI::registerID;
			tes3ui["lookupID"] = TES3::UI::lookupID;
			tes3ui["registerProperty"] = TES3::UI::registerProperty;
			tes3ui.set_function("createMenu", [](sol::table args) {
				auto id = args.get<sol::optional<UI_ID>>("id");
				if (!id) {
					log::getLog() << "createMenu: id argument is required." << std::endl;
					return static_cast<Element*>(nullptr);
				}

				Element* menu = TES3::UI::createMenu(id.value());

				if (args.get<bool>("fixedFrame")) {
					menu->createFixedFrame(id.value(), 1);
					// Standard behaviours
					TES3::UI::preventInventoryMenuToggle(menu);
				}
				else if (args.get<bool>("dragFrame")) {
					menu->createDragFrame(id.value(), 1);
				}

				return menu;
			});
			tes3ui.set_function("createHelpLayerMenu", [](sol::table args) {
				auto id = args.get<sol::optional<UI_ID>>("id");
				if (!id) {
					log::getLog() << "createHelpLayerMenu: id argument is required." << std::endl;
					return static_cast<Element*>(nullptr);
				}
				return TES3::UI::createHelpLayerMenu(id.value());
			});
			tes3ui.set_function("createTooltipMenu", []() {
				return TES3::UI::createTooltipMenu(TES3::UI::UI_ID(TES3::UI::Property::HelpMenu));
			});
			tes3ui["findMenu"] = TES3::UI::findMenu;
			tes3ui["findHelpLayerMenu"] = TES3::UI::findHelpLayerMenu;
			tes3ui["forcePlayerInventoryUpdate"] = []() {
				auto worldController = TES3::WorldController::get();
				auto playerMobile = worldController->getMobilePlayer();
				worldController->inventoryData->clearIcons(2);
				worldController->inventoryData->addInventoryItems(&playerMobile->npcInstance->inventory, 2);
				TES3::UI::updateInventoryMenuTiles();
			};
			tes3ui["enterMenuMode"] = TES3::UI::enterMenuMode;
			tes3ui["leaveMenuMode"] = TES3::UI::leaveMenuMode;
			tes3ui["acquireTextInput"] = TES3::UI::acquireTextInput;
			tes3ui.set_function("getPalette", [](const char* name) {
				sol::state& state = LuaManager::getInstance().getState();
				auto colour = TES3::UI::getPaletteColour(TES3::UI::registerProperty(name));
				return state.create_table_with(1, colour.x, 2, colour.y, 3, colour.z);
			});
			tes3ui["updateInventoryTiles"] = &TES3::UI::updateInventoryMenuTiles;
			tes3ui["updateBarterMenuTiles"] = &TES3::UI::updateBarterMenuTiles;
			tes3ui["updateContentsMenuTiles"] = &TES3::UI::updateContentsMenuTiles;
			tes3ui["updateInventorySelectTiles"] = []() -> sol::optional<int> {
				auto menu = TES3::UI::findMenu(*reinterpret_cast<TES3::UI::UI_ID*>(0x7D3C14));
				if (menu == nullptr) {
					return sol::optional<int>();
				}

				auto pane = menu->findChild(*reinterpret_cast<TES3::UI::UI_ID*>(0x7D821C));
				pane->destroyChildren();

				int count = TES3::UI::updateSelectInventoryTiles();

				menu->timingUpdate();

				return count;
			};
			tes3ui["getInventorySelectType"] = TES3::UI::getInventorySelectType;
			tes3ui["getServiceActor"] = []() {
				return mwse::lua::makeLuaObject(TES3::UI::getServiceActor());
			};
			tes3ui["showScrollMenu"] = &TES3::UI::showScrollMenu;
			tes3ui["showBookMenu"] = &TES3::UI::showBookMenu;

			tes3ui["logToConsole"] = [](const char* text, sol::optional<bool> isCommand) {
				TES3::UI::logToConsole(text, isCommand.value_or(false));
			};

			// Add binding for TES3::UI::TreeItem type.
			// TODO: Move this to its own file after TES3::UI::Tree has been made a template.
			{
				auto usertypeDefinition = state.create_simple_usertype<TES3::UI::TreeItem>();

				usertypeDefinition.set("id", &TES3::UI::TreeItem::key);
				usertypeDefinition.set("name", sol::property([](TES3::UI::TreeItem& self) {
					return TES3::UI::lookupID(self.key);
				}));
				usertypeDefinition.set("type", sol::property([](TES3::UI::TreeItem& self) {
					return unsigned int(self.valueType) & 0x7F;
				}));
				usertypeDefinition.set("value", sol::property([](TES3::UI::TreeItem& self) -> sol::object {
					sol::state& state = LuaManager::getInstance().getState();

					unsigned int type = unsigned int(self.valueType) & 0x7F;

					switch (type) {
					case TES3::UI::PropertyType::Integer:
						return sol::make_object(state, self.value.integerValue);
					case TES3::UI::PropertyType::Float:
						return sol::make_object(state, self.value.floatValue);
					case TES3::UI::PropertyType::Property:
						if (self.value.propertyValue == TES3::UI::Property::boolean_true) {
							return sol::make_object(state, true);
						}
						else if (self.value.propertyValue == TES3::UI::Property::boolean_false) {
							return sol::make_object(state, false);
						}
						else {
							return sol::make_object(state, self.value.propertyValue);
						}
					}
					return sol::nil;
				}));

				state.set_usertype("tes3uiProperty", usertypeDefinition);
			}
		}

	}
}
