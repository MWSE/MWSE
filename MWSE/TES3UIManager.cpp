#include "MemoryUtil.h"
#include "TES3UIElement.h"
#include "TES3UIManager.h"
#include "TES3UIMenuController.h"
#include "TES3Util.h"
#include "TES3WorldController.h"

namespace TES3 {
	namespace UI {
		const DWORD TES3_hook_dispatchMousewheelUp = 0x58F19B;
		const DWORD TES3_hook_dispatchMousewheelDown = 0x58F1CA;

		const auto TES3_uiHelpRoot = reinterpret_cast<Element* const*>(0x7D1C74);

		const auto TES3_ui_registerID = reinterpret_cast<UI_ID (__cdecl *)(const char *)>(0x58DF10);
		const auto TES3_ui_createChildElement = reinterpret_cast<Element* (__thiscall *)(Element*)>(0x582B50);
		const auto TES3_ui_createMenu = reinterpret_cast<Element* (__cdecl *)(UI_ID)>(0x595400);
		const auto TES3_ui_createTooltipMenu = reinterpret_cast<Element* (__cdecl *)(UI_ID)>(0x595A40);
		const auto TES3_ui_findMenu = reinterpret_cast<Element* (__cdecl*)(UI_ID)>(0x595370);
		const auto TES3_ui_findHelpLayerMenu = reinterpret_cast<Element* (__cdecl*)(UI_ID)>(0x595A10);
		const auto TES3_ui_getPaletteColour = reinterpret_cast<Vector3& (__cdecl*)(Vector3&, Property)>(0x57F610);
		const auto TES3_ui_onMenuUnfocus = reinterpret_cast<EventCallback>(0x58F790);
		const auto TES3_ui_ScrollbarArrow_onClick = reinterpret_cast<EventCallback>(0x647A60);
		const auto TES3_ui_requestMenuModeOn = reinterpret_cast<Boolean (__cdecl*)(UI_ID)>(0x595230);
		const auto TES3_ui_requestMenuModeOff = reinterpret_cast<Boolean (__cdecl*)(UI_ID)>(0x595270);
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

		Property registerProperty(const char *name) {
			return static_cast<Property>(TES3_ui_registerID(name));
		}

		Element* createMenu(UI_ID id) {
			return TES3_ui_createMenu(id);
		}

		Element* createHelpLayerMenu(UI_ID id) {
			// Simple replacement implementation. The Morrowind implementation is convoluted.
			Element* menu = TES3_ui_createChildElement(*TES3_uiHelpRoot);
			menu->createFixedFrame(id, 1);
			return menu;
		}

		Element* createTooltipMenu(UI_ID id) {
			return TES3_ui_createTooltipMenu(id);
		}

		Element* findMenu(UI_ID id) {
			return TES3_ui_findMenu(id);
		}

		Element* findHelpLayerMenu(UI_ID id) {
			return TES3_ui_findHelpLayerMenu(id);
		}

		Boolean enterMenuMode(UI_ID id) {
			return TES3_ui_requestMenuModeOn(id);
		}

		Boolean leaveMenuMode() {
			return TES3_ui_requestMenuModeOff(0);
		}

		void acquireTextInput(Element* element) {
			TES3::WorldController::get()->menuController->menuInputController->acquireTextInput(element);
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

		Boolean __cdecl onScrollPaneMousewheel(Element* owningWidget, Property eventID, int data0, int data1, Element* source) {
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
			return 1;
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

		const auto TES3_ShowBookMenu = reinterpret_cast<void(__cdecl*)(const char*)>(0x5AC2A0);
		void showBookMenu(const char* text) {
			TES3_ShowBookMenu(text);
		}

		const auto TES3_ShowScrollMenu = reinterpret_cast<void(__cdecl*)(const char*)>(0x6138A0);
		void showScrollMenu(const char* text) {
			TES3_ShowScrollMenu(text);
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

		void pushNewUIID(DWORD address, const char* name) {
			DWORD id = registerID(name);
			mwse::genPushEnforced(address, id);
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
