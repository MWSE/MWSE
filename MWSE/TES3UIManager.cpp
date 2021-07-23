#include "MemoryUtil.h"

#include "TES3Util.h"
#include "Log.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3UIElement.h"
#include "TES3UIManager.h"
#include "TES3UIMenuController.h"

#include "TES3GameSetting.h"
#include "TES3ItemData.h"
#include "TES3NPC.h"
#include "TES3MobilePlayer.h"
#include "TES3Reference.h"
#include "TES3WorldController.h"

#include "LuaShowRestWaitMenuEvent.h"

namespace TES3 {
	namespace UI {
		static bool bSuppressHelpMenu = false;

		const DWORD TES3_hook_dispatchMousewheelUp = 0x58F19B;
		const DWORD TES3_hook_dispatchMousewheelDown = 0x58F1CA;

		const auto TES3_uiMainRoot = reinterpret_cast<Element* const*>(0x7D1C28);
		const auto TES3_uiHelpRoot = reinterpret_cast<Element* const*>(0x7D1C74);
		const auto TES3_ui_captureMouseDrag = reinterpret_cast<bool*>(0x7D207D);

		const auto TES3_ui_registerID = reinterpret_cast<UI_ID (__cdecl *)(const char *)>(0x58DF10);
		const auto TES3_ui_createChildElement = reinterpret_cast<Element* (__thiscall *)(Element*)>(0x582B50);
		const auto TES3_ui_createMenu = reinterpret_cast<Element* (__cdecl *)(UI_ID)>(0x595400);
		const auto TES3_ui_createTooltipMenu = reinterpret_cast<Element* (__cdecl *)(UI_ID)>(0x595A40);
		const auto TES3_ui_findMenu = reinterpret_cast<Element* (__cdecl*)(UI_ID)>(0x595370);
		const auto TES3_ui_findHelpLayerMenu = reinterpret_cast<Element* (__cdecl*)(UI_ID)>(0x595A10);
		const auto TES3_ui_getMenuOnTop = reinterpret_cast<Element* (__cdecl*)()>(0x595290);
		const auto TES3_ui_getPaletteColour = reinterpret_cast<Vector3& (__cdecl*)(Vector3&, Property)>(0x57F610);
		const auto TES3_ui_onMenuUnfocus = reinterpret_cast<EventCallback>(0x58F790);
		const auto TES3_ui_ScrollbarArrow_onClick = reinterpret_cast<EventCallback>(0x647A60);
		const auto TES3_ui_requestMenuModeOn = reinterpret_cast<bool (__cdecl*)(UI_ID)>(0x595230);
		const auto TES3_ui_requestMenuModeOff = reinterpret_cast<bool (__cdecl*)(UI_ID)>(0x595270);
		const auto TES3_ui_getServiceActor = reinterpret_cast<MobileActor* (__cdecl*)()>(0x5BFEA0);
		const auto TES3_ui_updateDialogDisposition = reinterpret_cast<void (__cdecl*)()>(0x5C0780);

		const UI_ID idNull = static_cast<UI_ID>(TES3::UI::Property::null);

		//
		// UI framework functions
		//

		UI_ID registerID(const char *name) {
			return TES3_ui_registerID(name);
		}

		const auto TES3_ui_lookupID = reinterpret_cast<const char*(__cdecl *)(UI_ID)>(0x57B220);
		const char* lookupID(UI_ID id) {
			return TES3_ui_lookupID(id);
		}

		Property registerProperty(const char *name) {
			return static_cast<Property>(TES3_ui_registerID(name));
		}

		Element* createMenu(UI_ID id) {
			return TES3_ui_createMenu(id);
		}

		Element* createMenu_lua(sol::table params) {
			auto id = mwse::lua::getOptionalUIID(params, "id");
			if (id == idNull) {
				mwse::log::getLog() << "createMenu: id argument is required." << std::endl;
				return nullptr;
			}

			Element* menu = createMenu(id);

			if (params.get_or("fixedFrame", false)) {
				menu->createFixedFrame(id, 1);
				// Standard behaviours
				preventInventoryMenuToggle(menu);
			}
			else if (params.get_or("dragFrame", false)) {
				menu->createDragFrame(id, 1);
			}

			return menu;
		}

		Element* createHelpLayerMenu(UI_ID id) {
			// Simple replacement implementation. The Morrowind implementation is convoluted.
			Element* menu = TES3_ui_createChildElement(*TES3_uiHelpRoot);
			menu->createFixedFrame(id, 1);
			return menu;
		}

		Element* createHelpLayerMenu_lua(sol::table params) {
			auto id = mwse::lua::getOptionalUIID(params, "id");
			if (id == idNull) {
				mwse::log::getLog() << "createHelpLayerMenu: id argument is required." << std::endl;
				return static_cast<Element*>(nullptr);
			}
			return createHelpLayerMenu(id);
		}

		Element* createTooltipMenu(UI_ID id) {
			return TES3_ui_createTooltipMenu(id);
		}

		Element* createTooltipMenu_lua() {
			return createTooltipMenu(TES3::UI::UI_ID(TES3::UI::Property::HelpMenu));
		}

		void refreshTooltip() {
			TES3::WorldController::get()->menuController->menuInputController->updateObjectTooltip();
		}

		Element* findMenu(UI_ID id) {
			return TES3_ui_findMenu(id);
		}

		Element* findMenu_lua(sol::object id) {
			return TES3_ui_findMenu(mwse::lua::getUIIDFromObject(id));
		}

		Element* findHelpLayerMenu(UI_ID id) {
			return TES3_ui_findHelpLayerMenu(id);
		}

		Element* findHelpLayerMenu_lua(sol::object id) {
			return TES3_ui_findHelpLayerMenu(mwse::lua::getUIIDFromObject(id));
		}

		Element* getMenuOnTop() {
			return TES3_ui_getMenuOnTop();
		}

		bool enterMenuMode(UI_ID id) {
			return TES3_ui_requestMenuModeOn(id);
		}

		bool enterMenuMode_lua(sol::object id) {
			return enterMenuMode(mwse::lua::getUIIDFromObject(id));
		}

		bool leaveMenuMode() {
			return TES3_ui_requestMenuModeOff(0);
		}

		const auto TES3_ui_closeJournal = reinterpret_cast<bool(__cdecl*)()>(0x5D6A10);
		bool closeJournal() {
			if (!TES3_ui_closeJournal()) return false;
			while (TES3_ui_closeJournal());
			return true;
		}

		void acquireTextInput(Element* element) {
			TES3::WorldController::get()->menuController->menuInputController->acquireTextInput(element);
		}

		void captureMouseDrag(bool capture) {
			*TES3_ui_captureMouseDrag = capture;
		}

		void preventInventoryMenuToggle(Element* menu) {
			menu->setProperty(Property::event_unfocus, TES3_ui_onMenuUnfocus);
		}

		Vector3 getPaletteColour(Property prop) {
			Vector3 colour;
			return TES3_ui_getPaletteColour(colour, prop);
		}

		//
		// Added functionality
		//

		bool __cdecl onScrollPaneMousewheel(Element* owningWidget, Property eventID, int data0, int data1, Element* source) {
			static auto propScrollPane = registerProperty("PartScrollPane");
			static auto propScrollPaneHor = registerProperty("PartScrollPaneHor");
			static auto propScrollPaneVert = registerProperty("PartScrollPaneVert");
			static auto propScrollbarHor = registerID("PartScrollPane_hor_scrollbar");
			static auto propScrollbarVert = registerID("PartScrollPane_vert_scrollbar");
			static auto propScrollLeftArrow = registerID("PartScrollBar_left_arrow");
			static auto propScrollRightArrow = registerID("PartScrollBar_right_arrow");

			Element* scrollbar = nullptr;
			Element* pane;
			for (pane = source; pane; pane = pane->parent) {
				Property part = pane->getProperty(PropertyType::Property, Property::is_part).propertyValue;
				if (part == propScrollPaneVert) {
					scrollbar = pane->findChild(propScrollbarVert);
					break;
				}
				else if (part == propScrollPane || part == propScrollPaneHor) {
					scrollbar = pane->findChild(propScrollbarHor);
					break;
				}
			}

			if (scrollbar) {
				// Forward event to left or right arrow depending on scroll direction in data0
				UI_ID arrowID = (data0 < 0) ? propScrollRightArrow : propScrollLeftArrow;
				Element* arrow = scrollbar->findChild(arrowID);
				return TES3_ui_ScrollbarArrow_onClick(scrollbar, eventID, data0, data1, arrow);
			}

			return true;
		}

		MobileActor* getServiceActor() {
			return TES3_ui_getServiceActor();
		}

		void updateDialogDisposition() {
			TES3_ui_updateDialogDisposition();
		}

		const char* getInventorySelectType() {
			const char* callbackType = "unknown";
			auto callbackAddress = *reinterpret_cast<DWORD*>(0x7D3CA0);
			switch (callbackAddress) {
			case 0x59A1F0: callbackType = "alembic"; break;
			case 0x59A160: callbackType = "calcinator"; break;
			case 0x5C6B40: callbackType = "enchantedItem"; break;
			case 0x59A220: callbackType = "ingredient"; break;
			case 0x59A190: callbackType = "mortar"; break;
			case 0x608A90: callbackType = "quick"; break;
			case 0x59A1C0: callbackType = "retort"; break;
			case 0x5C6B00: callbackType = "soulGemFilled"; break;
			}
			return callbackType;
		}

		void stealHelpMenu() {
			// Move HelpMenu from help layer to main layer.
			Element *help = TES3_ui_findHelpLayerMenu(static_cast<UI_ID>(Property::HelpMenu));
			if (help) {
				// Remove menu from help layer child vector.
				Element **p = help->parent->vectorChildren.begin;
				while (*p != help) {
					++p;
				}
				for (size_t n = (help->parent->vectorChildren.end - p) + 1; n; --n) {
					*p = *(p + 1);
				}
				help->parent->vectorChildren.end--;
				*p = 0;

				// Place menu in main layer.
				help->reattachToParent(*TES3_uiMainRoot);

				// Add an empty dummy menu to staisfy game code that expects the help menu.
				createTooltipMenu(static_cast<UI_ID>(Property::HelpMenu));
			}
		}

		void suppressHelpMenu() {
			// Hide help menu without releasing the scene node, for code that expects its presence.
			Element *help = TES3_ui_findHelpLayerMenu(static_cast<UI_ID>(Property::HelpMenu));
			if (help) {
				help->maxWidth = 0;
				help->maxHeight = 0;
				help->performLayout(1);
			}
		}

		bool isSuppressingHelpMenu() {
			return bSuppressHelpMenu;
		}

		void setSuppressingHelpMenu(bool suppress) {
			bSuppressHelpMenu = suppress;
			if (suppress) {
				suppressHelpMenu();
			}
		}

		const auto TES3_showDialogueMessage = reinterpret_cast<void(__cdecl*)(const char*, int, int)>(0x5C00A0);
		void showDialogueMessage(const char* message, int style, int answerIndex) {
			TES3_showDialogueMessage(message, style, answerIndex);
		}

		void showDialogueMessage_lua(sol::table params) {
			const char* text = mwse::lua::getOptionalParam<const char*>(params, "text", nullptr);
			int style = mwse::lua::getOptionalParam<int>(params, "style", 0);
			int answerIndex = mwse::lua::getOptionalParam<int>(params, "answerIndex", 0);

			TES3_showDialogueMessage(text, style, answerIndex);
		}

		const auto TES3_showMessageBox = reinterpret_cast<Element*(__cdecl*)(const char*, const char*, bool)>(0x5F90C0);
		Element* showMessageBox(const char* message, const char* image, bool showInDialog) {
			return TES3_showMessageBox(message, image, showInDialog);
		}

		//
		// UI framework improvement hooks
		//

		__declspec(naked) void patchDispatchMousewheelUp() {
			__asm {
				mov eax, [esi + 4]
				mov edx, [esi + 8]
				push ecx
				push edx
				push eax
				push 0xFFFF8036
				jmp $ + 0x37C
			}
		}
		const size_t patchDispatchMousewheelUp_size = 0x14;

		__declspec(naked) void patchDispatchMousewheelDown() {
			__asm {
				mov eax, [esi + 4]
				mov edx, [esi + 8]
				push ecx
				push edx
				push eax
				push 0xFFFF8037
				jmp $ + 0x34D
			}
		}
		const size_t patchDispatchMousewheelDown_size = 0x14;

		const auto TES3_ConsoleLogResult = reinterpret_cast<void(__cdecl*)(const char*, bool)>(0x5B2C20);
		void logToConsole(const char* text, bool isCommand) {
			TES3_ConsoleLogResult(text, isCommand);
		}

		void logToConsole_lua(const char* text, sol::optional<bool> isCommand) {
			TES3_ConsoleLogResult(text, isCommand.value_or(false));
		}

		const auto TES3_ShowBookMenu = reinterpret_cast<void(__cdecl*)(const char*)>(0x5AC2A0);
		void showBookMenu(const char* text) {
			TES3_ShowBookMenu(text);
		}

		const auto TES3_ShowScrollMenu = reinterpret_cast<void(__cdecl*)(const char*)>(0x6138A0);
		void showScrollMenu(const char* text) {
			TES3_ShowScrollMenu(text);
		}

		const auto TES3_ShowRestMenu = reinterpret_cast<void(__cdecl*)(bool)>(0x610170);
		void showRestMenu(bool resting, bool scripted) {
			// Execute event. If the event blocked the call, bail.
			if (mwse::lua::event::ShowRestWaitMenuEvent::getEventEnabled()) {
				mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
				auto stateHandle = luaManager.getThreadSafeStateHandle();
				sol::table eventData = stateHandle.triggerEvent(new mwse::lua::event::ShowRestWaitMenuEvent(resting, scripted));
				if (eventData.valid()) {
					if (eventData.get_or("block", false)) {
						return;
					}

					resting = eventData["allowRest"];
				}
			}

			TES3_ShowRestMenu(resting);
		}

		void showRestMenu_lua(sol::optional<bool> resting) {
			showRestMenu(resting.value_or(true));
		}

		const auto TES3_UpdateFillBar = reinterpret_cast<void(__cdecl*)(UI_ID, float, float)>(0x6262D0);
		void updateFillBar(UI_ID id, float current, float max) {
			TES3_UpdateFillBar(id, current, max);
		}

		const auto TES3_UpdateHealthFillBar = reinterpret_cast<void(__cdecl*)(float, float)>(0x50FBB0);
		void updateHealthFillBar(float current, float max) {
			TES3_UpdateHealthFillBar(current, max);
		}

		const auto TES3_UpdateMagickaFillBar = reinterpret_cast<void(__cdecl*)(float, float)>(0x50FBD0);
		void updateMagickaFillBar(float current, float max) {
			TES3_UpdateMagickaFillBar(current, max);
		}

		const auto TES3_UpdateFatigueFillBar = reinterpret_cast<void(__cdecl*)(float, float)>(0x50FBF0);
		void updateFatigueFillBar(float current, float max) {
			TES3_UpdateFatigueFillBar(current, max);
		}

		const auto TES3_UpdateEncumbrance = reinterpret_cast<void(__cdecl*)()>(0x5CD1B0);
		void updateEncumbranceBar() {
			TES3_UpdateEncumbrance();
		}

		const auto TES3_UpdatePlayerAttribute = reinterpret_cast<void(__cdecl*)(float, int)>(0x626410);
		void updatePlayerAttribute(float current, int attributeId) {
			TES3_UpdatePlayerAttribute(current, attributeId);
		}

		const auto TES3_UpdateStatsPane = reinterpret_cast<void(__cdecl*)()>(0x6266D0);
		void updateStatsPane() {
			TES3_UpdateStatsPane();
		}

		const auto TES3_UI_UpdateSpellmakingMenu = reinterpret_cast<void(__cdecl*)()>(0x622020);
		void updateSpellmakingMenu() {
			TES3_UI_UpdateSpellmakingMenu();
		}

		const auto TES3_UI_UpdateEnchantingMenu = reinterpret_cast<void(__cdecl*)()>(0x5C3840);
		void updateEnchantingMenu() {
			TES3_UI_UpdateEnchantingMenu();
		}

		const auto TES3_UpdateInventoryTiles = reinterpret_cast<void(__cdecl*)()>(0x5CC910);
		void updateInventoryMenuTiles() {
			TES3_UpdateInventoryTiles();
		}

		const auto TES3_UpdateContentsMenuTiles = reinterpret_cast<void(__cdecl*)()>(0x5B67E0);
		void updateContentsMenuTiles() {
			TES3_UpdateContentsMenuTiles();
		}

		const auto TES3_UpdateContentsCompanionElements = reinterpret_cast<void(__cdecl*)()>(0x5B7830);
		void updateContentsCompanionElements() {
			TES3_UpdateContentsCompanionElements();
		}

		const auto TES3_UpdateBarterMenuTiles = reinterpret_cast<void(__cdecl*)()>(0x5A5620);
		void updateBarterMenuTiles() {
			TES3_UpdateBarterMenuTiles();
		}

		const auto TES3_UpdateInventorySelectTiles = reinterpret_cast<int(__cdecl*)()>(0x5D3E70);
		int updateSelectInventoryTiles() {
			return TES3_UpdateInventorySelectTiles();
		}

		sol::optional<int> updateInventorySelectTiles_lua() {
			auto menu = TES3::UI::findMenu(*reinterpret_cast<TES3::UI::UI_ID*>(0x7D3C14));
			if (menu == nullptr) {
				return {};
			}

			auto pane = menu->findChild(*reinterpret_cast<TES3::UI::UI_ID*>(0x7D821C));
			pane->destroyChildren();

			int count = TES3::UI::updateSelectInventoryTiles();

			menu->timingUpdate();

			return count;
		}

		void forcePlayerInventoryUpdate() {
			auto worldController = TES3::WorldController::get();
			auto playerMobile = worldController->getMobilePlayer();
			worldController->inventoryData->clearIcons(2);
			worldController->inventoryData->addInventoryItems(&playerMobile->npcInstance->inventory, 2);
			TES3::UI::updateInventoryMenuTiles();
		}

		void updateInventoryCharacterImage() {
			const auto TES3_updateInventoryCharacterImage = reinterpret_cast<void(__cdecl*)(bool)>(0x5CD2A0);
			TES3_updateInventoryCharacterImage(false);
		}

		bool isInMenuMode() {
			return TES3::WorldController::get()->flagMenuMode;
		}

		sol::table getPalette_lua(sol::this_state ts, const char* name) {
			sol::state_view state = ts;
			auto colour = TES3::UI::getPaletteColour(TES3::UI::registerProperty(name));
			return state.create_table_with(1, colour.x, 2, colour.y, 3, colour.z);
		}

		//
		// Custom InventorySelectMenu hook.
		//

		static bool inventorySelectLuaCallbackCloseAfter = true;
		static sol::protected_function inventorySelectLuaCallback = sol::nil;
		bool __cdecl luaDispatchedSelectCallback(Element* element, Property property, int data0, int data1, Element* elementAgain) {
			auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			if (inventorySelectLuaCallback.valid()) {
				sol::table params = stateHandle.state.create_table();

				auto MenuInventorySelect_object = reinterpret_cast<TES3::BaseObject*>(element->getProperty(PropertyType::Pointer, *reinterpret_cast<Property*>(0x7D3B7A)).ptrValue);
				auto MenuInventorySelect_extra = reinterpret_cast<TES3::ItemData*>(element->getProperty(PropertyType::Pointer, *reinterpret_cast<Property*>(0x7D3C48)).ptrValue);
				auto MenuInventorySelect_count = element->getProperty(PropertyType::Integer, *reinterpret_cast<Property*>(0x7D3BCE)).integerValue;
				auto MenuInventorySelect_container = reinterpret_cast<TES3::Inventory*>(element->getProperty(PropertyType::Pointer, *reinterpret_cast<Property*>(0x7D3C32)).ptrValue);
				auto MenuInventorySelect_container_object = reinterpret_cast<TES3::Actor*>(element->getProperty(PropertyType::Pointer, *reinterpret_cast<Property*>(0x7D3C3C)).ptrValue);

				params["item"] = MenuInventorySelect_object;
				params["itemData"] = MenuInventorySelect_extra;
				params["count"] = MenuInventorySelect_extra ? MenuInventorySelect_extra->count : MenuInventorySelect_count;
				params["inventory"] = MenuInventorySelect_container;
				params["actor"] = MenuInventorySelect_container_object;

				sol::protected_function_result result = inventorySelectLuaCallback(params);
				if (!result.valid()) {
					sol::error error = result;
					mwse::log::getLog() << "Lua error encountered during UI inventory select callback:" << std::endl << error.what() << std::endl;

					if (inventorySelectLuaCallbackCloseAfter) {
						TES3::UI::leaveMenuMode();
					}

					return false;
				}
			}

			if (inventorySelectLuaCallbackCloseAfter) {
				TES3::UI::leaveMenuMode();
			}

			return true;
		}

		static sol::protected_function inventorySelectLuaFilter = sol::nil;
		bool __cdecl luaDispatchedSelectFilter(Element* element, Property property, int data0, int data1, Element* elementAgain) {
			auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			if (inventorySelectLuaFilter.valid()) {
				sol::table params = stateHandle.state.create_table();

				auto MenuInventorySelect_filter_object = reinterpret_cast<TES3::BaseObject*>(element->getProperty(PropertyType::Pointer, *reinterpret_cast<Property*>(0x7D3C88)).ptrValue);
				auto MenuInventorySelect_filter_extra = reinterpret_cast<TES3::ItemData*>(element->getProperty(PropertyType::Pointer, *reinterpret_cast<Property*>(0x7D3C16)).ptrValue);

				params["item"] = MenuInventorySelect_filter_object;
				params["itemData"] = MenuInventorySelect_filter_extra;

				sol::protected_function_result result = inventorySelectLuaFilter(params);
				if (!result.valid()) {
					sol::error error = result;
					mwse::log::getLog() << "Lua error encountered during UI inventory select filtering for item '" << MenuInventorySelect_filter_object->getObjectID() << "':" << std::endl << error.what() << std::endl;

					return false;
				}
				else {
					sol::object resultObject = result;
					if (resultObject.is<bool>()) {
						return resultObject.as<bool>();
					}
				}
			}

			return true;
		}

		const auto TES3_UI_showSelectMenu = reinterpret_cast<void(__cdecl*)(TES3::Actor*, EventCallback*, const char*)>(0x5D35B0);
		void showSelectMenu(TES3::Actor* actor, EventCallback* callback, const char* titleText) {
			TES3_UI_showSelectMenu(actor, callback, titleText);
		}

		const std::unordered_map<std::string, EventCallback*> inventorySelectFilterIdMap = {
			{ "alembic", reinterpret_cast<EventCallback*>(0x59A1F0) },
			{ "calcinator", reinterpret_cast<EventCallback*>(0x59A160) },
			{ "enchanted", reinterpret_cast<EventCallback*>(0x5C6B40) },
			{ "ingredients", reinterpret_cast<EventCallback*>(0x59A220) },
			{ "mortar", reinterpret_cast<EventCallback*>(0x59A190) },
			{ "quickUse", reinterpret_cast<EventCallback*>(0x608A90) },
			{ "retort", reinterpret_cast<EventCallback*>(0x59A1C0) },
			{ "soulgemFilled", reinterpret_cast<EventCallback*>(0x5C6B00) },
		};

		void showInventorySelectMenu_lua(sol::table params) {
			// Get our actor.
			TES3::Reference* actorRef = mwse::lua::getOptionalParamExecutionReference(params);
			if (actorRef == nullptr) {
				actorRef = TES3::WorldController::get()->getMobilePlayer()->reference;
			}
			else if (!actorRef->baseObject->isActor()) {
				throw std::exception("Invalid reference passed to tes3ui.showInventorySelectMenu. Must be an actor.");
			}
			TES3::Actor* actor = static_cast<TES3::Actor*>(actorRef->baseObject);

			// Get our callback.
			EventCallback* callback = reinterpret_cast<EventCallback*>(&luaDispatchedSelectCallback);
			inventorySelectLuaCallback = mwse::lua::getOptionalParam<sol::protected_function>(params, "callback", sol::nil);
			if (!inventorySelectLuaCallback.valid()) {
				throw std::exception("Invalid callback argument provided.");
			}

			// Get our filter.
			EventCallback* filter = nullptr;
			inventorySelectLuaFilter = sol::nil;
			sol::object maybeFilter = params["filter"];
			if (maybeFilter.is<sol::protected_function>()) {
				filter = reinterpret_cast<EventCallback*>(&luaDispatchedSelectFilter);
				inventorySelectLuaFilter = maybeFilter;
			}
			else if (maybeFilter.is<std::string>()) {
				std::string callbackId = maybeFilter.as<std::string>();
				auto findResult = inventorySelectFilterIdMap.find(callbackId);
				if (findResult == inventorySelectFilterIdMap.end()) {
					throw std::exception("Invalid filter ID provided.");
				}
				filter = findResult->second;
			}
			else {
				throw std::exception("Invalid filter argument provided. Must be a function, or a string ID.");
			}

			// And our title text.
			const char* titleText = mwse::lua::getOptionalParam<const char*>(params, "title", nullptr);
			if (titleText == nullptr) {
				throw std::exception("Invalid title argument passed to tes3ui.showInventorySelectMenu.");
			}

			// Allow overwriting of our "no item found" text.
			auto sInventorySelectNoItems = TES3::DataHandler::get()->nonDynamicData->GMSTs[TES3::GMST::sInventorySelectNoItems];
			const char* oldNoResultsText = sInventorySelectNoItems->value.asString;
			const char* noResultsText = mwse::lua::getOptionalParam<const char*>(params, "noResultsText", nullptr);
			if (noResultsText != nullptr) {
				sInventorySelectNoItems->value.asString = (char*)noResultsText;
			}

			// Do we close the menu after?
			inventorySelectLuaCallbackCloseAfter = mwse::lua::getOptionalParam<bool>(params, "leaveMenuMode", !TES3::WorldController::get()->flagMenuMode);

			// Set our filter and show the menu.
			*reinterpret_cast<EventCallback**>(0x7D3CA0) = filter;
			showSelectMenu(actor, callback, titleText);

			// Restore the previous results text.
			if (noResultsText != nullptr) {
				sInventorySelectNoItems->value.asString = (char*)oldNoResultsText;
			}

			// If the menu was successfully shown, enter menu mode.
			auto MenuInventorySelect = *reinterpret_cast<UI_ID*>(0x7D3C14);
			if (TES3::UI::findMenu(MenuInventorySelect)) {
				TES3::UI::enterMenuMode(MenuInventorySelect);
			}
		}

		void pushNewUIID(DWORD address, const char* name) {
			DWORD id = registerID(name);
			mwse::genPushEnforced(address, id);
		}


		Property toBooleanProperty(bool b) {
			return b ? Property::boolean_true : Property::boolean_false;
		}

		bool toBool(Property prop) {
			return prop == Property::boolean_true;
		}

		const std::unordered_map<std::string, TES3::UI::Property> standardNamedEvents = {
			{ "mouseLeave", TES3::UI::Property::event_mouse_leave },
			{ "mouseOver", TES3::UI::Property::event_mouse_over },
			{ "mouseDown", TES3::UI::Property::event_mouse_down },
			{ "mouseClick", TES3::UI::Property::event_mouse_click },
			{ "mouseScrollUp", TES3::UI::Property::event_mouse_scroll_up },
			{ "mouseScrollDown", TES3::UI::Property::event_mouse_scroll_down },
			{ "mouseDoubleClick", TES3::UI::Property::event_mouse_double_click },
			{ "mouseStillIdle", TES3::UI::Property::event_mouse_still_idle },
			{ "mouseStillOver", TES3::UI::Property::event_mouse_still_over },
			{ "mouseStillPressedOutside", TES3::UI::Property::event_mouse_still_pressed_outside },
			{ "mouseStillPressed", TES3::UI::Property::event_mouse_still_pressed },
			{ "mouseRelease", TES3::UI::Property::event_mouse_release },
			{ "keyPress", TES3::UI::Property::event_key_press },
			{ "keyEnter", TES3::UI::Property::event_key_enter },
			{ "focus", TES3::UI::Property::event_focus },
			{ "unfocus", TES3::UI::Property::event_unfocus },
			{ "update", TES3::UI::Property::event_update },
			{ "preUpdate", TES3::UI::Property::event_pre_update },
			{ "destroy", TES3::UI::Property::event_destroy }
		};

		std::optional<Property> getStandardEventFromName(const std::string& name) {
			auto it = standardNamedEvents.find(name);
			if (it != standardNamedEvents.end()) {
				return it->second;
			}
			return {};
		}

		void hook() {
			// Patch mousewheel event dispatch to not redirect to the top-level element,
			// allowing mousewheel to apply to more than the first scrollpane in a menu
			mwse::writePatchCodeUnprotected(TES3_hook_dispatchMousewheelUp, (BYTE*)&patchDispatchMousewheelUp, patchDispatchMousewheelUp_size);
			mwse::writePatchCodeUnprotected(TES3_hook_dispatchMousewheelDown, (BYTE*)&patchDispatchMousewheelDown, patchDispatchMousewheelDown_size);

			// Patch UI layout engine to reflow wrapped text content on size changes
			auto patch = &Element::patchUpdateLayout_propagateFlow;
			mwse::genCallEnforced(0x585E1E, 0x584850, *reinterpret_cast<DWORD*>(&patch));
			mwse::genCallEnforced(0x5863AE, 0x584850, *reinterpret_cast<DWORD*>(&patch));

			// Provide some UI IDs for elements that don't have them, like tooltips:
			pushNewUIID(0x590F59, "HelpMenu_titleBlock");
			pushNewUIID(0x590FBD, "HelpMenu_icon");
			pushNewUIID(0x5911DC, "HelpMenu_name");
			pushNewUIID(0x591293, "HelpMenu_uses");
			pushNewUIID(0x5912CD, "HelpMenu_qualityCondition");
			pushNewUIID(0x591315, "HelpMenu_armorRating");
			pushNewUIID(0x59136B, "HelpMenu_destinationTo");
			pushNewUIID(0x591383, "HelpMenu_destinationCell");
			pushNewUIID(0x5914AB, "HelpMenu_weaponType");
			pushNewUIID(0x5914E7, "HelpMenu_chop");
			pushNewUIID(0x59151D, "HelpMenu_slash");
			pushNewUIID(0x591553, "HelpMenu_thrust");	// Attack for ammo
			pushNewUIID(0x59166A, "HelpMenu_weight");
			pushNewUIID(0x59172F, "HelpMenu_weight");
			pushNewUIID(0x591790, "HelpMenu_value");
			pushNewUIID(0x5917EF, "HelpMenu_value");
			pushNewUIID(0x59195F, "HelpMenu_effectBlock");	// Potions
			pushNewUIID(0x5919BE, "HelpMenu_effectIcon");	// Potions
			pushNewUIID(0x591E8F, "HelpMenu_effectLabel");	// Potions
			pushNewUIID(0x59231C, "HelpMenu_castType");
			pushNewUIID(0x5929E0, "HelpMenu_chargeBlock");
			pushNewUIID(0x592A5E, "HelpMenu_chargeLabel");
			pushNewUIID(0x592B98, "HelpMenu_owner");
			pushNewUIID(0x592014, "HelpMenu_effectBlock");	// Ingredients
			pushNewUIID(0x5922C1, "HelpMenu_effectIcon");	// Ingredients
			pushNewUIID(0x59206D, "HelpMenu_effectLabel");	// Ingredients
			pushNewUIID(0x59232A, "HelpMenu_enchantmentContainer");
			pushNewUIID(0x5923B5, "HelpMenu_enchantEffectBlock");
			pushNewUIID(0x592425, "icon_container");	//enchant effect
			pushNewUIID(0x59294B, "label_container");	//enchant effect
			pushNewUIID(0x5929A9, "HelpMenu_enchantEffectLabel");
			pushNewUIID(0x59145F, "HelpMenu_weaponType");	//patched ammo and marksman weapons
			pushNewUIID(0x5F0DD0, "HelpMenu_mapNoteBlock");
			pushNewUIID(0x5F0E13, "HelpMenu_mapNoteIcon");
			pushNewUIID(0x5F0EE5, "HelpMenu_mapNoteLabel");
			pushNewUIID(0x5915E6, "HelpMenu_locked");
			pushNewUIID(0x591614, "HelpMenu_trapped");

			// Enchantment Menu
			pushNewUIID(0x5C1A89, "MenuEnchantment_topRow");
			pushNewUIID(0x5C1AC5, "MenuEnchantment_selectablesContainer");
			pushNewUIID(0x5C1B99, "MenuEnchantment_nameLabel");
			pushNewUIID(0x5C1C5C, "MenuEnchantment_itemLabel");
			pushNewUIID(0x5C1D18, "MenuEnchantment_nameFieldBorder");
			pushNewUIID(0x5C1FBA, "MenuEnchantment_soulLabel");
			pushNewUIID(0x5C20A0, "MenuEnchantment_infoContainer");
			pushNewUIID(0x5C211E, "MenuEnchantment_enchantmentContainer");
			pushNewUIID(0x5C2188, "MenuEnchantment_enchantmentLabel");
			pushNewUIID(0x5C2226, "MenuEnchantment_enchantmentBackslash");
			pushNewUIID(0x5C224D, "MenuEnchantment_castCostContainer");
			pushNewUIID(0x5C22B5, "MenuEnchantment_castCostLabel");
			pushNewUIID(0x5C232F, "MenuEnchantment_chargeContainer");
			pushNewUIID(0x5C2394, "MenuEnchantment_chargeLabel");
			pushNewUIID(0x5C23F9, "MenuEnchantment_scrollPanelsContainer");
			pushNewUIID(0x5C240D, "MenuEnchantment_magicEffectsContainer");
			pushNewUIID(0x5C2478, "MenuEnchantment_magicEffectsLabel");
			pushNewUIID(0x5C24F7, "MenuEnchantment_effectsContainer");
			pushNewUIID(0x5C2549, "MenuEnchantment_effectsLabel");
			pushNewUIID(0x5C25EB, "MenuEnchantment_bottomButtonRow");
			pushNewUIID(0x5C26F5, "MenuEnchantment_priceContainer");
			pushNewUIID(0x5C2752, "MenuEnchantment_priceLabel");
			pushNewUIID(0x5C2852, "MenuEnchantment_buttonContainer");
		}
	}
}
