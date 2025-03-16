#include "MemoryUtil.h"

#include "TES3Util.h"
#include "Log.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3UIElement.h"
#include "TES3UIInventoryTile.h"
#include "TES3UILuaData.h"
#include "TES3UIManager.h"
#include "TES3UIMenuController.h"

#include "TES3Game.h"
#include "TES3GameSetting.h"
#include "TES3ItemData.h"
#include "TES3NPC.h"
#include "TES3MobilePlayer.h"
#include "TES3Reference.h"
#include "TES3WorldController.h"

#include "LuaConsoleReferenceChangedEvent.h"
#include "LuaMagicSelectionChangedEvent.h"
#include "LuaShowRestWaitMenuEvent.h"
#include "LuaUiSkillTooltipEvent.h"
#include "LuaUiSpellTooltipEvent.h"

#include "TES3UIManagerLua.h"

namespace TES3::UI {
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
	const auto TES3_ui_requestMenuModeOff = reinterpret_cast<bool (__cdecl*)()>(0x595270);
	const auto TES3_ui_getServiceActor = reinterpret_cast<MobileActor* (__cdecl*)()>(0x5BFEA0);
	const auto TES3_ui_updateDialogDisposition = reinterpret_cast<void (__cdecl*)()>(0x5C0780);

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

	const auto TES3_ui_lookupPropertyID = reinterpret_cast<const char* (__cdecl*)(Property)>(0x57B220);
	const char* lookupID(Property id) {
		return TES3_ui_lookupPropertyID(id);
	}

	const char* lookupID_lua(sol::object id) {
		if (id.is<TES3::UI::Property>()) {
			return lookupID(id.as<TES3::UI::Property>());
		}
		else if (id.is<TES3::UI::UI_ID>()) {
			return lookupID(id.as<TES3::UI::UI_ID>());
		}
		else if (id.is<short>()) {
			return lookupID((UI_ID)id.as<short>());
		}
		throw std::invalid_argument("ID cannot be interpreted as a valid UI ID or property.");
	}

	Property registerProperty(const char *name) {
		return static_cast<Property>(TES3_ui_registerID(name));
	}

	Element* createMenu(UI_ID id) {
		return TES3_ui_createMenu(id);
	}

	Element* createMenu_lua(sol::table params) {
		auto id = mwse::lua::getOptionalUIID(params, "id");
		if (id == ID_NULL) {
			throw std::invalid_argument("createMenu: id argument is required.");
		}

		bool isDragFrame = params.get_or("dragFrame", false);
		bool isFixedFrame = params.get_or("fixedFrame", false);

		if (!(isDragFrame || isFixedFrame)) {
			throw std::invalid_argument("createMenu: Either dragFrame or fixedFrame must be selected.");
		}

		Element* menu = createMenu(id);

		if (isFixedFrame) {
			menu->createFixedFrame(id, 1);
			// Standard behaviours
			preventInventoryMenuToggle(menu);

			// Optionally turn off modal feature of fixedFrame menus.
			if (!params.get_or("modal", true)) {
				const auto returnTrueFunc = reinterpret_cast<EventCallback>(0x5A7655);
				menu->setProperty(TES3::UI::Property::event_unfocus, returnTrueFunc);
			}
		}
		else if (isDragFrame) {
			menu->createDragFrame(id, 1);

			// Optionally mark menu as auto-saving its positioning to morrowind.ini.
			if (params.get_or("loadable", true)) {
				menu->setProperty(Property::savable_menu, Property::boolean_true);
			}
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
		if (id == ID_NULL) {
			mwse::log::getLog() << "createHelpLayerMenu: id argument is required." << std::endl;
			return static_cast<Element*>(nullptr);
		}
		return createHelpLayerMenu(id);
	}

	Element* createTooltipMenu(UI_ID id) {
		return TES3_ui_createTooltipMenu(id);
	}

	Element* createTooltipMenu_lua(sol::optional<sol::table> params) {
		using mwse::lua::getOptionalParam;
		using mwse::lua::getOptionalParamObject;

		auto menu = createTooltipMenu(UI_ID(Property::HelpMenu));

		if (!params) {
			// Empty tooltip creation.
			return menu;
		}

		auto object = getOptionalParamObject<TES3::Object>(params, "object");
		if (object == nullptr) {
			// Backwards compatibility with the old item parameter.
			object = getOptionalParamObject<TES3::Object>(params, "item");
		}
		if (object) {
			auto itemData = getOptionalParam<TES3::ItemData*>(params, "itemData", nullptr);
			auto count = itemData ? itemData->count : 0;

			WorldController::get()->menuController->menuInputController->displayObjectTooltip(object, itemData, count);
			return menu;
		}

		auto spell = getOptionalParamObject<TES3::Spell>(params, "spell");
		if (spell) {
			// Set a property on a temp element, where the spell tooltip game function expects to find the spell.
			const auto ui_id_MagicMenu_Spell = *reinterpret_cast<const Property*>(0x7D431C);
			Element* tempElement = TES3_ui_createChildElement(*TES3_uiHelpRoot);
			tempElement->setProperty(ui_id_MagicMenu_Spell, spell);

			// Build the tooltip.
			const auto TES3_buildSpellTooltip = reinterpret_cast<EventCallback>(0x5E45C0);
			bool created = TES3_buildSpellTooltip(tempElement, Property(), 0, 0, tempElement);

			// Fire off the related event.
			if (created && mwse::lua::event::UiSpellTooltipEvent::getEventEnabled()) {
				mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
				luaManager.getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::UiSpellTooltipEvent(menu, spell));
			}

			// Clean up temp element.
			tempElement->destroy();
			return menu;
		}

		auto skill = getOptionalParamObject<TES3::Skill>(params, "skill");
		if (skill) {
			// Build the tooltip.
			const auto TES3_buildSkillTooltip = reinterpret_cast<void(__cdecl*)(char)>(0x6297F0);
			TES3_buildSkillTooltip(skill->skill);

			// Fire off the related event.
			if (mwse::lua::event::UiSkillTooltipEvent::getEventEnabled()) {
				mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
				luaManager.getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::UiSkillTooltipEvent(menu, skill->skill, 0));
			}

			return menu;
		}

		throw std::invalid_argument("createTooltipMenu: Could not find object matching arguments.");
	}

	void refreshTooltip() {
		TES3::WorldController::get()->menuController->menuInputController->updateObjectTooltip();
	}

	Element* findMenu(UI_ID id) {
		return TES3_ui_findMenu(id);
	}

	Element* findMenu(const char* id) {
		return findMenu(registerID(id));
	}

	Element* findMenu_lua(sol::object id) {
		return TES3_ui_findMenu(mwse::lua::getUIIDFromObject(id));
	}

	Element* findHelpLayerMenu(UI_ID id) {
		return TES3_ui_findHelpLayerMenu(id);
	}

	Element* findHelpLayerMenu(const char* id) {
		return findHelpLayerMenu(registerID(id));
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

	bool enterMenuMode(const char* id) {
		return TES3_ui_requestMenuModeOn(registerID(id));
	}

	bool enterMenuMode_lua(sol::object id) {
		return enterMenuMode(mwse::lua::getUIIDFromObject(id));
	}

	bool leaveMenuMode() {
		return TES3_ui_requestMenuModeOff();
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

	bool __cdecl onInventoryTileTooltip(Element* owningWidget, Property eventID, int data0, int data1, Element* source) {
		const auto p_propMenuInventory_Object = reinterpret_cast<Property*>(0x7D39C8);
		const auto p_propMenuInventory_extra = reinterpret_cast<Property*>(0x7D396C);
		const auto p_propMenuInventory_Thing = reinterpret_cast<Property*>(0x7D3A70);

		// This call is required because owningWidget is resolved differently for tooltips compared to regular events.
		const auto TES3_ui_findPartRootOrComponent = reinterpret_cast<Element* (__thiscall*)(Element*, Property)>(0x582B80);
		auto tileElement = TES3_ui_findPartRootOrComponent(source, Property::null);

		auto item = reinterpret_cast<Item*>(tileElement->getProperty(PropertyType::Pointer, *p_propMenuInventory_Object).ptrValue);
		auto itemData = reinterpret_cast<ItemData*>(tileElement->getProperty(PropertyType::Pointer, *p_propMenuInventory_extra).ptrValue);

		if (item && !findHelpLayerMenu(static_cast<UI_ID>(Property::CursorIcon))) {
			// The original code provided an incorrect count for tiles that were both from a split stack and had no item data.
			auto tile = reinterpret_cast<InventoryTile*>(tileElement->getProperty(PropertyType::Pointer, *p_propMenuInventory_Thing).ptrValue);
			TES3::WorldController::get()->menuController->menuInputController->displayObjectTooltip(item, itemData, tile->count);
		}

		return true;
	}
	
	MobileActor* getServiceActor() {
		return TES3_ui_getServiceActor();
	}

	void updateDialogDisposition() {
		TES3_ui_updateDialogDisposition();
	}


	Reference* getConsoleReference() {
		auto console = findMenu("MenuConsole");
		if (!console) {
			return nullptr;
		}

		return static_cast<Reference*>(console->getProperty(PropertyType::Pointer, registerProperty("MenuConsole_current_ref")).ptrValue);
	}

	const auto TES3_ui_setConsoleReference = reinterpret_cast<void(__cdecl*)(Reference*)>(0x5B2F00);
	void __cdecl setConsoleReference(TES3::Reference* reference) {
		// Store the previous value so we can know if it changed.
		auto referenceBefore = getConsoleReference();

		// Call the original function.
		TES3_ui_setConsoleReference(reference);

		// If it changed, fire off an event.
		auto referenceAfter = getConsoleReference();
		if (mwse::lua::event::ConsoleReferenceChangedEvent::getEventEnabled() && referenceBefore != referenceAfter) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::ConsoleReferenceChangedEvent(referenceAfter));
		}
	}

	Element* getCursor() {
		return TES3::UI::findHelpLayerMenu("CursorIcon");
	}

	InventoryTile* getCursorTile() {
		const auto cursor = getCursor();
		if (cursor == nullptr) {
			return nullptr;
		}

		return cursor->getPropertyPointer<TES3::UI::InventoryTile>("MenuInventory_Thing");
	}

	std::tuple<unsigned int, unsigned int> getViewportSize_lua() {
		auto& viewportCameraData = TES3::WorldController::get()->menuCamera.cameraData;
		return { viewportCameraData.viewportWidth, viewportCameraData.viewportHeight };
	}

	float getViewportScale() {
		return float(TES3::Game::get()->windowWidth) / float(TES3::WorldController::get()->worldCamera.cameraData.viewportWidth);
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
			Element **p = help->parent->vectorChildren.first, **end = help->parent->vectorChildren.nextEmpty;
			while (*p != help) {
				++p;
			}
			for (; p < end - 1; ++p) {
				*p = *(p + 1);
			}
			*p = 0;
			help->parent->vectorChildren.nextEmpty--;

			// Place menu in main layer.
			help->reattachToParent(*TES3_uiMainRoot);

			// Add an empty dummy menu to satisfy game code that expects the help menu.
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

	const auto TES3_UI_updateTopicsList = reinterpret_cast<void(__cdecl*)()>(0x5BE6C0);
	void __cdecl updateTopicsList() {
		TES3_UI_updateTopicsList();

		// Fire off an event to let mods know the list was updated.
		mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::GenericEvent("topicsListUpdated"));
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
		if (text == nullptr) {
			return;
		}
		logToConsole(text, isCommand.value_or(false));
	}

	const auto TES3_HideCursor = reinterpret_cast<void(__cdecl*)()>(0x5966F0);
	void hideCursor() {
		TES3_HideCursor();
	}

	const auto TES3_CloseBookMenu = reinterpret_cast<void(__cdecl*)()>(0x5AC7F0);
	void closeBookMenu() {
		Element* menuBook = TES3::UI::findMenu("MenuBook");
		if (menuBook) {
			TES3_CloseBookMenu();
		}
	}

	const auto TES3_CloseDialogueMenu = reinterpret_cast<void(__cdecl*)()>(0x5BE5B0);
	void closeDialogueMenu() {
		TES3_CloseDialogueMenu();
	}

	const auto TES3_CloseScrollMenu = reinterpret_cast<void(__cdecl*)()>(0x613A60);
	void closeScrollMenu() {
		Element* menuScroll = TES3::UI::findMenu("MenuScroll");
		if (menuScroll) {
			TES3_CloseScrollMenu();
		}
	}

	const auto TES3_UI_ShowJournal = reinterpret_cast<void(__cdecl*)()>(0x5D62B0);
	//const auto TES3_UI_ShowJournal = reinterpret_cast<void(__cdecl*)()>(0x5D62B0);
	bool showJournal() {
		if (findMenu("MenuJournal")) {
			return false;
		}

		TES3_UI_ShowJournal();
		TES3::WorldController::get()->menuController->flagClearHelpMenu = 1;

		return findMenu("MenuJournal") != nullptr;
	}

	const auto TES3_ui_closeJournal = reinterpret_cast<bool(__cdecl*)()>(0x5D6A10);
	bool closeJournal() {
		if (!TES3_ui_closeJournal()) {
			return false;
		}
		// Loop to exit out of all sub-sections of the journal.
		while (TES3_ui_closeJournal()) {}
		return true;
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

	const auto TES3_ShowSpellmakingMenu = reinterpret_cast<void(__cdecl*)()>(0x621450);
	auto& TES3_HasServiceActor = *reinterpret_cast<bool*>(0x7D69FC);
	static std::optional<MobileActor*> TES3_SpellmakingMenu_ServiceActorOverride = {};
	void __cdecl showSpellmakingMenu() {
		// Clear our override.
		TES3_SpellmakingMenu_ServiceActorOverride = {};

		TES3_ShowSpellmakingMenu();
	}

	void showSpellmakingMenuWithOverride(MobileActor* serviceActorOverride) {
		// Set our override and change the service actor flag.
		TES3_SpellmakingMenu_ServiceActorOverride = serviceActorOverride;
		TES3_HasServiceActor = serviceActorOverride != nullptr;

		TES3_ShowSpellmakingMenu();
	}

	MobileActor* __cdecl getSpellmakingServiceActor() {
		// If we have an override, make use of it.
		if (TES3_SpellmakingMenu_ServiceActorOverride) {
			return TES3_SpellmakingMenu_ServiceActorOverride.value();
		}

		// Otherwise use the existing logic.
		return getServiceActor();
	}

	void __fastcall patchSpellmakingMenuRemoveNoCost(MobilePlayer* self, DWORD _EDX_, int goldDelta) {
		if (goldDelta == 0) {
			return;
		}
		self->modGold(goldDelta);
	}

	Element* __cdecl patchSpellmakingMenuExitMenuModeIfNoDialogMenu(TES3::UI::UI_ID dialogMenuId) {
		auto dialogMenu = findMenu(dialogMenuId);

		// If no dialog menu exists, close menu mode.
		if (!dialogMenu) {
			leaveMenuMode();
		}

		return dialogMenu;
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

	const auto TES3_UI_UpdateCurrentMagicFillBar = reinterpret_cast<void(__cdecl*)(float, float)>(0x5F5500);
	void updateCurrentMagicFillBar(float current, float max) {
		TES3_UI_UpdateCurrentMagicFillBar(current, max);
	}

	const auto TES3_UI_UpdateCurrentMagicFromSpell = reinterpret_cast<void(__cdecl*)(const char*, const char*, Spell*)>(0x5F4E70);
	void updateCurrentMagicFromSpell(const char* iconPath, const char* spellName, Spell* spell) {
		// Call original code.
		TES3_UI_UpdateCurrentMagicFromSpell(iconPath, spellName, spell);

		// Fire event. Deselects when spell is nullptr.
		if (mwse::lua::event::MagicSelectionChangedEvent::getEventEnabled()) {
			auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			sol::object response = stateHandle.triggerEvent(new mwse::lua::event::MagicSelectionChangedEvent(spell));
		}
	}

	const auto TES3_UI_UpdateCurrentMagicFromEquipmentStack = reinterpret_cast<void(__cdecl*)(const char*, const char*, EquipmentStack*)>(0x5F4DB0);
	void updateCurrentMagicFromEquipmentStack(const char* iconPath, const char* itemName, EquipmentStack* equipmentStack) {
		// Call original code.
		TES3_UI_UpdateCurrentMagicFromEquipmentStack(iconPath, itemName, equipmentStack);

		// Fire event.
		if (equipmentStack) {
			auto enchantment = equipmentStack->object->getEnchantment();

			if (enchantment && mwse::lua::event::MagicSelectionChangedEvent::getEventEnabled()) {
				auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
				sol::object response = stateHandle.triggerEvent(new mwse::lua::event::MagicSelectionChangedEvent(enchantment, equipmentStack->object));
			}
		}
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

	const auto TES3_UI_MagicMenu_UpdateEnchantedItemSelection = reinterpret_cast<void(__cdecl*)()>(0x5E2E80);
	void updateMagicMenuEnchantedItemSelection() {
		TES3_UI_MagicMenu_UpdateEnchantedItemSelection();
	}

	const auto& TES3_UI_ID_MenuMagic = *reinterpret_cast<TES3::UI::UI_ID*>(0x7D431E);
	const auto& TES3_UI_ID_MenuMagic_ActiveItem = *reinterpret_cast<TES3::UI::Property*>(0x7D4448);
	const auto TES3_UI_MagicMenu_UpdateSpellSelection = reinterpret_cast<void(__cdecl*)(Element*)>(0x5E1AB0);
	const auto TES3_UI_MenuMagic_GetItemFromCurrentMagic = reinterpret_cast<Element*(__cdecl*)(PropertyValue*)>(0x5E16A0);
	void updateMagicMenuSelection() {
		UI::Element* magicMenu = UI::findMenu(TES3_UI_ID_MenuMagic);
		if (magicMenu) {
			Element* item = TES3_UI_MenuMagic_GetItemFromCurrentMagic(0);
			TES3_UI_MagicMenu_UpdateSpellSelection(item);
			updateMagicMenuEnchantedItemSelection();
			if (item) {
				magicMenu->setText(item->getText());
			}
			else {
				magicMenu->setText(TES3::DataHandler::get()->nonDynamicData->GMSTs[TES3::GMST::sNone]->value.asString);
			}
			magicMenu->setProperty(TES3_UI_ID_MenuMagic_ActiveItem, item);
		}
	}

	const auto TES3_UI_UpdateSpellmakingMenu = reinterpret_cast<void(__cdecl*)()>(0x622020);
	void updateSpellmakingMenu() {
		TES3_UI_UpdateSpellmakingMenu();
	}

	const auto TES3_UI_UpdateEnchantingMenu = reinterpret_cast<void(__cdecl*)()>(0x5C3840);
	void updateEnchantingMenu() {
		TES3_UI_UpdateEnchantingMenu();
	}

	const auto TES3_UI_FlagSkillUpdated = reinterpret_cast<void(__cdecl*)(int)>(0x627320);
	void flagSkillUpdated(int unknown) {
		TES3_UI_FlagSkillUpdated(unknown);
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

	const auto TES3_UI_UpdateInventoryWindowTitle = reinterpret_cast<void(__cdecl*)()>(0x5CE080);
	void updateInventoryWindowTitle() {
		TES3_UI_UpdateInventoryWindowTitle();
	}

	void updateInventoryCharacterImage() {
		const auto TES3_updateInventoryCharacterImage = reinterpret_cast<void(__cdecl*)(bool)>(0x5CD2A0);
		TES3_updateInventoryCharacterImage(false);
	}

	bool isInMenuMode() {
		return TES3::WorldController::get()->flagMenuMode;
	}

	const auto TES3_ui_moveMenuToFront = reinterpret_cast<void(__cdecl*)(Element*)>(0x595870);
	void moveMenuToFront(Element* menu) {
		TES3_ui_moveMenuToFront(menu);
	}

	void moveMenuToFront_lua(sol::object id) {
		auto menu = findMenu_lua(id);
		if (menu == nullptr) {
			throw std::invalid_argument("Parameter must be an element or a top level menu ID.");
		}
		moveMenuToFront(menu);
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

	const auto TES3_UI_showInventorySelectMenu = reinterpret_cast<void(__cdecl*)(TES3::Actor*, EventCallback*, const char*)>(0x5D35B0);
	void showInventorySelectMenu(TES3::Actor* actor, EventCallback* callback, const char* titleText) {
		TES3_UI_showInventorySelectMenu(actor, callback, titleText);
	}

	const std::unordered_map<std::string, EventCallback*> inventorySelectFilterIdMap = {
		{ "alembic", reinterpret_cast<EventCallback*>(0x59A1F0) },
		{ "calcinator", reinterpret_cast<EventCallback*>(0x59A160) },
		{ "enchanted", reinterpret_cast<EventCallback*>(0x5C6B40) },
		{ "ingredients", reinterpret_cast<EventCallback*>(0x59A220) },
		{ "mortar", reinterpret_cast<EventCallback*>(0x59A190) },
		{ "quickUse", reinterpret_cast<EventCallback*>(0x608A90) },
		{ "retort", reinterpret_cast<EventCallback*>(0x59A1C0) },
		{ "soulGemFilled", reinterpret_cast<EventCallback*>(0x5C6B00) },

		// Deprecated
		{ "soulgemFilled", reinterpret_cast<EventCallback*>(0x5C6B00) },
	};

	static sol::protected_function noValidItemsCallback = sol::nil;
	static const char* noValidItemsTextOverride = nullptr;
	Element* __cdecl messagePlayerForNoValidItems(const char* message, const char* image = nullptr, bool showInDialog = true) {
		if (noValidItemsCallback.valid()) {
			noValidItemsCallback();
		}
		return showMessageBox(noValidItemsTextOverride ? noValidItemsTextOverride : message, image, showInDialog);
	}

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
		noValidItemsTextOverride = mwse::lua::getOptionalParam<const char*>(params, "noResultsText", nullptr);
		noValidItemsCallback = mwse::lua::getOptionalParam<sol::protected_function>(params, "noResultsCallback", sol::nil);

		// Do we close the menu after?
		inventorySelectLuaCallbackCloseAfter = mwse::lua::getOptionalParam<bool>(params, "leaveMenuMode", !TES3::WorldController::get()->flagMenuMode);

		// Set our filter and show the menu.
		*reinterpret_cast<EventCallback**>(0x7D3CA0) = filter;
		showInventorySelectMenu(actor, callback, titleText);

		// Reset our overrides.
		noValidItemsTextOverride = nullptr;
		noValidItemsCallback = sol::nil;

		// If the menu was successfully shown, enter menu mode.
		auto MenuInventorySelect = *reinterpret_cast<UI_ID*>(0x7D3C14);
		if (TES3::UI::findMenu(MenuInventorySelect)) {
			TES3::UI::enterMenuMode(MenuInventorySelect);
		}
	}

	static sol::protected_function magicSelectLuaCallback = sol::nil;

	const auto TES3_UI_MenuQuick_setSlot = reinterpret_cast<bool(__cdecl*)(int, TES3::Item*, TES3::ItemData*, TES3::Spell*)>(0x6084E0);
	bool onMagicSelect(int quickType, TES3::Item* item, TES3::ItemData* itemData, TES3::Spell* spell) {
		// This function is shared with the quick keys menu.

		if (magicSelectLuaCallback.valid()) {
			// The menu was opened from a lua script. Run the callback.
			auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			sol::table params = stateHandle.state.create_table();
			params["item"] = item;
			params["itemData"] = itemData;
			params["spell"] = spell;

			sol::protected_function_result result = magicSelectLuaCallback(params);
			if (!result.valid()) {
				sol::error error = result;
				mwse::log::getLog() << "Lua error encountered during UI magic select callback:" << std::endl << error.what() << std::endl;
			}

			// Reset callback after use.
			magicSelectLuaCallback = sol::nil;
			return true;
		}
		else {
			// The menu was opened from the quick keys menu. Call the original function.
			return TES3_UI_MenuQuick_setSlot(quickType, item, itemData, spell);
		}
	}

	const auto TES3_UI_MenuMagicSelect_onCancel = reinterpret_cast<bool(__cdecl*)(Element*, Property, int, int, Element*)>(0x5E8040);
	bool __cdecl onMagicSelectCancel(Element* widget, Property property, int data0, int data1, Element* source) {
		// This function is shared with the quick keys menu.

		if (magicSelectLuaCallback.valid()) {
			// The menu was opened from a lua script. Run the callback.
			auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			sol::table params = stateHandle.state.create_table();

			sol::protected_function_result result = magicSelectLuaCallback(params);
			if (!result.valid()) {
				sol::error error = result;
				mwse::log::getLog() << "Lua error encountered during UI magic select callback:" << std::endl << error.what() << std::endl;
			}

			// Reset callback after use.
			magicSelectLuaCallback = sol::nil;
		}

		return TES3_UI_MenuMagicSelect_onCancel(widget, property, data0, data1, source);
	}

	const auto TES3_UI_showMagicSelectMenu = reinterpret_cast<void(__cdecl*)()>(0x5E7580);
	void showMagicSelectMenu() {
		TES3_UI_showMagicSelectMenu();
	}

	void showMagicSelectMenu_lua(sol::table params) {
		// Get our callback and place it in a global.
		EventCallback* callback = reinterpret_cast<EventCallback*>(&luaDispatchedSelectCallback);
		magicSelectLuaCallback = mwse::lua::getOptionalParam<sol::protected_function>(params, "callback", sol::nil);
		if (!magicSelectLuaCallback.valid()) {
			throw std::exception("Invalid callback argument provided.");
		}

		// Get our filters.
		bool selectSpells = mwse::lua::getOptionalParam<bool>(params, "selectSpells", true);
		bool selectPowers = mwse::lua::getOptionalParam<bool>(params, "selectPowers", true);
		bool selectEnchanted = mwse::lua::getOptionalParam<bool>(params, "selectEnchanted", true);

		// And our title text.
		auto titleText = mwse::lua::getOptionalParam<const char*>(params, "title", nullptr);
		if (titleText == nullptr) {
			throw std::exception("Invalid title argument.");
		}

		// Show the menu.
		showMagicSelectMenu();

		auto MenuMagicSelectID = *reinterpret_cast<UI_ID*>(0x7D4570);
		auto menuMagicSelect = findMenu(MenuMagicSelectID);
		if (menuMagicSelect) {
			// Set title, add a little whitespace.
			auto title = *menuMagicSelect->getContentElement()->vectorChildren.begin();
			title->setText(titleText);
			title->borderBottom = 6;

			// Filter by hiding sections that aren't enabled.
			auto MenuMagicSpellsListID = *reinterpret_cast<UI_ID*>(0x7D431A);
			auto magicList = menuMagicSelect->findChild(MenuMagicSpellsListID)->getContentElement();
			auto children = magicList->vectorChildren.first;

			if (!selectPowers) {
				// Hide title and list.
				children[0]->setVisible(false);
				children[1]->setVisible(false);
			}
			if (!(selectPowers && selectSpells)) {
				// Hide divider.
				children[2]->setVisible(false);
			}
			if (!selectSpells) {
				// Hide title and list.
				children[3]->setVisible(false);
				children[4]->setVisible(false);
			}
			if (!(selectSpells && selectEnchanted)) {
				// Hide divider.
				children[5]->setVisible(false);
			}
			if (!selectEnchanted) {
				// Hide title and list.
				children[6]->setVisible(false);
				children[7]->setVisible(false);
			}
		}
	}

	const auto TES3_UI_createResponseText = reinterpret_cast<void(__cdecl*)(Element*, const char*, int, int)>(0x5C00D0);
	void createResponseText(Element* parent, const char* text, int type, int answerIndex) {
		TES3_UI_createResponseText(parent, text, type, answerIndex);
	}

	void createResponseText_lua(sol::table params) {
		sol::optional<std::string> text = params["text"];
		auto type = mwse::lua::getOptionalParam(params, "type", 2);
		auto index = mwse::lua::getOptionalParam(params, "index", -1);

		if (!text) {
			throw std::invalid_argument("Invalid 'text' parameter provided.");
		}

		createResponseText(nullptr, text.value().c_str(), type, index);
	}

	void choice(const char* text, int index) {
		createResponseText(nullptr, text, 2, index);
	}

	std::tuple<int, int> textLayoutGetFontHeight_lua(sol::table params) {
		int fontIndex = mwse::lua::getOptionalParam<int>(params, "font", 0);

		if (fontIndex < 0 || fontIndex >= 3) {
			throw std::invalid_argument("Invalid 'font' parameter provided.");
		}

		auto font = TES3::WorldController::get()->fonts[fontIndex];
		return { font->maxGlyphHeight, font->fontData->lineHeight };
	}

	const auto TES3_UI_Font_getTextExtent = reinterpret_cast<void(__thiscall*)(TES3::Font*, const char*, float*, float*, int, int, bool)>(0x40B190);
	void textLayoutGetTextExtent(TES3::Font* font, const char* text, float* out_width, float* out_verticalAdvance, int maxCharsOrLineMode, bool useLineHeight) {
		TES3_UI_Font_getTextExtent(font, text, out_width, out_verticalAdvance, maxCharsOrLineMode, 0, useLineHeight);
	}

	std::tuple<int, int, int> textLayoutGetTextExtent_lua(sol::table params) {
		sol::optional<const char*> text = params["text"];
		int fontIndex = mwse::lua::getOptionalParam<int>(params, "font", 0);
		bool firstLineOnly = mwse::lua::getOptionalParam<bool>(params, "firstLineOnly", false);

		if (fontIndex < 0 || fontIndex >= 3) {
			throw std::invalid_argument("Invalid 'font' parameter provided.");
		}
		if (!text) {
			throw std::invalid_argument("Invalid 'text' parameter provided.");
		}
		
		auto font = TES3::WorldController::get()->fonts[fontIndex];
		float width, verticalAdvance;
		textLayoutGetTextExtent(font, text.value(), &width, &verticalAdvance, firstLineOnly ? -2 : -1, false);

		// Calculated label height from text layout system.
		float height = std::floor(verticalAdvance + font->maxGlyphHeight + 1.5f);

		return { width, height, verticalAdvance };
	}

	const auto TES3_UI_Font_wrapTextInPlace = reinterpret_cast<int(__thiscall*)(TES3::Font*, char*, unsigned int, bool, char)>(0x40BBC0);
	int textLayoutWrapTextInPlace(TES3::Font* font, char* textBuffer, unsigned int maxWidth, bool ignoreLinkDelimiters, char newlineReplacement) {
		return TES3_UI_Font_wrapTextInPlace(font, textBuffer, maxWidth, ignoreLinkDelimiters, newlineReplacement);
	}

	std::tuple<std::string, int> textLayoutWrapText_lua(sol::table params) {
		sol::optional<const char*> text = params["text"];
		int fontIndex = mwse::lua::getOptionalParam<int>(params, "font", 0);
		int maxWidth = mwse::lua::getOptionalParam<int>(params, "maxWidth", -1);
		bool ignoreLinkDelimiters = mwse::lua::getOptionalParam<bool>(params, "ignoreLinkDelimiters", false);

		if (fontIndex < 0 || fontIndex >= 3) {
			throw std::invalid_argument("Invalid 'font' parameter provided.");
		}
		if (!text) {
			throw std::invalid_argument("Invalid 'text' parameter provided.");
		}

		auto font = TES3::WorldController::get()->fonts[fontIndex];
		std::string textBuffer{ text.value() };

		int lineCount = textLayoutWrapTextInPlace(font, textBuffer.data(), maxWidth, ignoreLinkDelimiters, '\n');
		return { textBuffer, lineCount };
	}

	void showLoadingMenu(const char* title, float progress) {
		const auto TES3_UI_showLoadingMenu = reinterpret_cast<void(__cdecl*)(const char*, float)>(0x5DED20);
		TES3_UI_showLoadingMenu(title, progress);
	}

	void updateLoadingMenu(float progress) {
		const auto TES3_UI_updateLoadingMenu = reinterpret_cast<void(__cdecl*)(float)>(0x4C7D90);
		TES3_UI_updateLoadingMenu(progress);
	}

	void destroyLoadingMenu() {
		const auto TES3_UI_destroyLoadingMenu = reinterpret_cast<void(__cdecl*)()>(0x5DEEA0);
		TES3_UI_destroyLoadingMenu();
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

	void __fastcall patchElementDeletion(TES3::UI::Element* self, DWORD _UNUSED_, TES3::UI::PropertyValue* propValue, TES3::UI::Property prop, TES3::UI::PropertyType propType, const TES3::UI::Element* element, bool checkInherited) {
		// Call overwritten code.
		auto destroyEvent = reinterpret_cast<void(__cdecl*)(TES3::UI::Element*)>(self->getProperty(propValue, prop, propType, element, checkInherited)->ptrValue);
		if (destroyEvent) {
			destroyEvent(self);
		}

		// Clean up any lua event registration.
		mwse::lua::cleanupEventRegistrations(self);

		// Clean up stale pointers.
		if (MenuInputController::previousTextInputFocus == self) {
			MenuInputController::previousTextInputFocus = nullptr;
		}

		// Clean up any lua data.
		auto luaData = self->getLuaDataContainer();
		if (luaData) {
			delete luaData;
		}
	}

	bool __fastcall patchKeyboardInput(MenuInputController* controller) {
		// Let elements know that their focus has changed.
		if (controller->textInputFocus != MenuInputController::previousTextInputFocus) {
			if (MenuInputController::previousTextInputFocus) {
				mwse::lua::triggerEvent(MenuInputController::previousTextInputFocus, registerProperty("inputUnfocus"), 0, 0);
			}
			if (controller->textInputFocus) {
				mwse::lua::triggerEvent(controller->textInputFocus, registerProperty("inputFocus"), 0, 0);
			}
			MenuInputController::previousTextInputFocus = controller->textInputFocus;
		}

		// Call overwritten code.
		const auto TES3_UI_MenuInputController_keyboardInput = reinterpret_cast<bool(__thiscall*)(MenuInputController*)>(0x58E8A0);
		return TES3_UI_MenuInputController_keyboardInput(controller);
	}

	void hook() {
		// Patch mousewheel event dispatch to not redirect to the top-level element,
		// allowing mousewheel to apply to more than the first scrollpane in a menu.
		mwse::writePatchCodeUnprotected(TES3_hook_dispatchMousewheelUp, (BYTE*)&patchDispatchMousewheelUp, patchDispatchMousewheelUp_size);
		mwse::writePatchCodeUnprotected(TES3_hook_dispatchMousewheelDown, (BYTE*)&patchDispatchMousewheelDown, patchDispatchMousewheelDown_size);

		// Patch UI layout engine to reflow wrapped text content on size changes.
		auto patch = &Element::patchUpdateLayout_propagateFlow;
		mwse::genCallEnforced(0x585E1E, 0x584850, *reinterpret_cast<DWORD*>(&patch));
		mwse::genCallEnforced(0x5863AE, 0x584850, *reinterpret_cast<DWORD*>(&patch));

		// Patch item selection no items message to allow callbacks and changed text.
		mwse::genCallEnforced(0x5D37CF, 0x5F90C0, reinterpret_cast<DWORD>(messagePlayerForNoValidItems));

		// Patch quick keys magic select menu to allow general purpose use.
		mwse::genCallEnforced(0x5E7648, 0x6084E0, reinterpret_cast<DWORD>(onMagicSelect));
		mwse::genCallEnforced(0x5E76DC, 0x6084E0, reinterpret_cast<DWORD>(onMagicSelect));
		mwse::writeValueEnforced<DWORD>(0x5E7103+1, 0x5E8040, reinterpret_cast<DWORD>(onMagicSelectCancel));

		// Patch element resetting to clear up any custom event handlers.
		mwse::genCallEnforced(0x578517, 0x581440, reinterpret_cast<DWORD>(patchElementDeletion), 0x578528 - 0x578517);

		// Patch SpellmakingMenu to allow service actor overrides.
		mwse::genCallEnforced(0x5BF5AD, 0x621450, reinterpret_cast<DWORD>(showSpellmakingMenu));
		mwse::genCallEnforced(0x622337, 0x5BFEA0, reinterpret_cast<DWORD>(getSpellmakingServiceActor));
		mwse::genCallEnforced(0x6229C5, 0x5BFEA0, reinterpret_cast<DWORD>(getSpellmakingServiceActor));
		mwse::genCallEnforced(0x6229D4, 0x5BFEA0, reinterpret_cast<DWORD>(getSpellmakingServiceActor));
		mwse::genCallEnforced(0x6229C0, 0x52B480, reinterpret_cast<DWORD>(patchSpellmakingMenuRemoveNoCost));
		mwse::genCallEnforced(0x621CBB, 0x595370, reinterpret_cast<DWORD>(patchSpellmakingMenuExitMenuModeIfNoDialogMenu));
		mwse::genCallEnforced(0x622DAA, 0x595370, reinterpret_cast<DWORD>(patchSpellmakingMenuExitMenuModeIfNoDialogMenu));
		mwse::writeValueEnforced<BYTE>(0x62295A, 0x75, 0x7D);
		
		// Patch inventory item tooltip to get accurate item count from the tile, which was failing on split item stacks.
		mwse::writeValueEnforced<DWORD>(0x5CC0A9+1, 0x5CDF90, reinterpret_cast<DWORD>(onInventoryTileTooltip));
		mwse::writeValueEnforced<DWORD>(0x5CCC6F+1, 0x5CDF90, reinterpret_cast<DWORD>(onInventoryTileTooltip));

		// Patch input elements to allow focus/unfocus events.
		mwse::genCallEnforced(0x58E1E2, 0x58E8A0, reinterpret_cast<DWORD>(patchKeyboardInput));

		// Provide some UI IDs for elements that don't have them:
		// Tooltips (HelpMenu)
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

		// Spellmaking Menu (MenuSpellmaking)
		pushNewUIID(0x620C0A, "MenuSpellmaking_TopLayout");
		pushNewUIID(0x620C50, "MenuSpellmaking_TopLeftLayout");
		pushNewUIID(0x620C90, "MenuSpellmaking_SpellNameLayout");
		pushNewUIID(0x620CDD, "MenuSpellmaking_SpellNameLabel");
		pushNewUIID(0x620D2F, "MenuSpellmaking_SpellNameBorder");
		pushNewUIID(0x620DFC, "MenuSpellmaking_TopRightLayout");
		pushNewUIID(0x620E44, "MenuSpellmaking_SpellPointCostLayout");
		pushNewUIID(0x620E9C, "MenuSpellmaking_SpellPointCostLabel");
		pushNewUIID(0x620F04, "MenuSpellmaking_SpellChanceLayout");
		pushNewUIID(0x620F5C, "MenuSpellmaking_SpellChanceLabel");
		pushNewUIID(0x620FC2, "MenuSpellmaking_CenterLayout");
		pushNewUIID(0x620FD0, "MenuSpellmaking_EffectsLayout");
		pushNewUIID(0x621037, "MenuSpellmaking_EffectsLabel");
		pushNewUIID(0x6210BB, "MenuSpellmaking_SpellEffectsLayout");
		pushNewUIID(0x62110F, "MenuSpellmaking_SpellEffectsLabel");
		pushNewUIID(0x6211B1, "MenuSpellmaking_BottomLayout");
		pushNewUIID(0x6211F3, "MenuSpellmaking_BottomSpacer");
		pushNewUIID(0x621233, "MenuSpellmaking_PriceLayout");
		pushNewUIID(0x621280, "MenuSpellmaking_PriceLabel");
		pushNewUIID(0x6212D9, "MenuSpellmaking_PriceValueLabel");
		pushNewUIID(0x621329, "MenuSpellmaking_ButtonLayout");

		// Enchantment Menu (MenuEnchantment)
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

		// Inventory Menu (MenuInventory)
		pushNewUIID(0x5CA225, "MenuInventory_top_layout");
		pushNewUIID(0x5CA67B, "MenuInventory_bottom_layout");
		pushNewUIID(0x5CA68B, "MenuInventory_character_layout");
		pushNewUIID(0x5CA818, "MenuInventory_items_layout");
		pushNewUIID(0x5CCAEF, "itemTile_column");
		pushNewUIID(0x5CB155, "itemTile_column");
		pushNewUIID(0x5CCBD2, "itemTile_tile");
		pushNewUIID(0x5CCCB5, "itemTile_shadow");
		pushNewUIID(0x5CCDC0, "itemTile_icon");
		//On drop
		pushNewUIID(0x5CC021, "itemTile_tile");
		pushNewUIID(0x5CC102, "itemTile_shadow");
		pushNewUIID(0x5CC20D, "itemTile_icon");

		// Container Menu (MenuContents)
		pushNewUIID(0x5B69B8, "itemTile_column");
		pushNewUIID(0x5B49BE, "itemTile_column");
		pushNewUIID(0x5B6A92, "itemTile_tile");
		pushNewUIID(0x5B6B4A, "itemTile_shadow");
		pushNewUIID(0x5B6C69, "itemTile_icon");
		//On drop
		pushNewUIID(0x5B63A1, "itemTile_tile");
		pushNewUIID(0x5B6455, "itemTile_shadow");
		pushNewUIID(0x5B6573, "itemTile_icon");

		// Barter Menu (MenuBarter)
		pushNewUIID(0x5A579B, "itemTile_column");
		pushNewUIID(0x5A40AE, "itemTile_column");
		pushNewUIID(0x5A5883, "itemTile_tile");
		pushNewUIID(0x5A5959, "itemTile_shadow");
		pushNewUIID(0x5A5A8D, "itemTile_icon");
		//On drop
		pushNewUIID(0x5A4D49, "itemTile_tile");
		pushNewUIID(0x5A4E1C, "itemTile_shadow");
		pushNewUIID(0x5A4F27, "itemTile_icon");

		// Cursor Icon
		pushNewUIID(0x5D1708, "itemTile_shadow");
		pushNewUIID(0x5D1813, "itemTile_icon");

		// Status Menu (MenuStat)
		pushNewUIID(0x624F7F, "MenuStat_layout");
		pushNewUIID(0x62562F, "MenuStat_general_frame");
		pushNewUIID(0x6256FF, "MenuStat_level_name");
		pushNewUIID(0x625818, "MenuStat_race_name");
		pushNewUIID(0x625934, "MenuStat_class_name");
		pushNewUIID(0x6259E8, "MenuStat_attributes_frame");
		pushNewUIID(0x625AC6, "MenuStat_attribute_strength_name");
		pushNewUIID(0x625B9B, "MenuStat_attribute_intellegence_name"); // Spelling mistake for consistency with other ids
		pushNewUIID(0x625C8C, "MenuStat_attribute_willpower_name");
		pushNewUIID(0x625D7E, "MenuStat_attribute_agility_name");
		pushNewUIID(0x625E71, "MenuStat_attribute_speed_name");
		pushNewUIID(0x625F64, "MenuStat_attribute_endurance_name");
		pushNewUIID(0x626056, "MenuStat_attribute_personality_name");
		pushNewUIID(0x626149, "MenuStat_attribute_luck_name");
		pushNewUIID(0x626757, "MenuStat_major_title");
		pushNewUIID(0x626817, "MenuStat_major_name");
		pushNewUIID(0x6268AC, "MenuStat_major_value");
		pushNewUIID(0x626994, "MenuStat_divider");
		pushNewUIID(0x6269D4, "MenuStat_minor_title");
		pushNewUIID(0x626A95, "MenuStat_minor_name");
		pushNewUIID(0x626B2B, "MenuStat_minor_value");
		pushNewUIID(0x626C13, "MenuStat_divider");
		pushNewUIID(0x626C53, "MenuStat_misc_title");
		pushNewUIID(0x626D3E, "MenuStat_misc_name");
		pushNewUIID(0x626DC1, "MenuStat_misc_value");
		pushNewUIID(0x626EB3, "MenuStat_divider");
		pushNewUIID(0x626EF6, "MenuStat_faction_title");
		pushNewUIID(0x626FFB, "MenuStat_divider");
		pushNewUIID(0x6270D2, "MenuStat_divider");
		pushNewUIID(0x627171, "MenuStat_reputation_name");
		pushNewUIID(0x6271DC, "MenuStat_reputation_value");
		pushNewUIID(0x627249, "MenuStat_Bounty_name");

		// Magic Menu (MenuMagic)
		pushNewUIID(0x5E0DD3, "MagicMenu_spells_layout");
		pushNewUIID(0x5E0E45, "MagicMenu_power_title");
		pushNewUIID(0x5E0E5F, "MagicMenu_power_layout");
		pushNewUIID(0x5E0F28, "MagicMenu_divider");
		pushNewUIID(0x5E0FB5, "MagicMenu_spell_title");
		pushNewUIID(0x5E0FE2, "MagicMenu_spell_cost_title");
		pushNewUIID(0x5E0FFC, "MagicMenu_spell_layout");
		pushNewUIID(0x5E1127, "MagicMenu_divider");
		pushNewUIID(0x5E11B4, "MagicMenu_item_title");
		pushNewUIID(0x5E11E1, "MagicMenu_item_cost_title");
		pushNewUIID(0x5E11FB, "MagicMenu_item_layout");

		// HUD (MenuMulti)
		pushNewUIID(0x5F333F, "MenuMulti_bottom_row_left");
		pushNewUIID(0x5F339A, "MenuMulti_npc");
		pushNewUIID(0x5F34DC, "MenuMulti_fillbars");
		pushNewUIID(0x5F3552, "MenuMulti_health");
		pushNewUIID(0x5F3680, "MenuMulti_magic");
		pushNewUIID(0x5F37B9, "MenuMulti_fatigue");
		pushNewUIID(0x5F38EA, "MenuMulti_icons");
		pushNewUIID(0x5F391D, "MenuMulti_icons_spacer");
		pushNewUIID(0x5F393E, "MenuMulti_icons_layout");
		pushNewUIID(0x5F39DF, "MenuMulti_weapon_border");
		pushNewUIID(0x5F3C70, "MenuMulti_magic_border");
		pushNewUIID(0x5F3F83, "MenuMulti_bottom_row_right");
		pushNewUIID(0x5F4168, "MenuMulti_map");
		pushNewUIID(0x5F4226, "MenuMap_layout");

		// MenuPersuasion
		pushNewUIID(0x5FFB23, "MenuPersuasion_Header");
		pushNewUIID(0x5FFD6A, "MenuPersuasion_Admire");
		pushNewUIID(0x5FFDF3, "MenuPersuasion_Intimidate");
		pushNewUIID(0x5FFE7E, "MenuPersuasion_Taunt");
		pushNewUIID(0x5FFF09, "MenuPersuasion_Bribe10");
		pushNewUIID(0x5FFFCB, "MenuPersuasion_Bribe100");
		pushNewUIID(0x60008D, "MenuPersuasion_Bribe1000");
		pushNewUIID(0x5FFBE0, "MenuPersuasion_Footer");
		pushNewUIID(0x5FFC88, "MenuPersuasion_Gold");
	}
}
