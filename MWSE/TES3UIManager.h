#pragma once

#include "TES3UIDefines.h"
#include "TES3Vectors.h"

namespace TES3 {
	namespace UI {
		UI_ID registerID(const char* name);
		const char* lookupID(UI_ID id);
		Property registerProperty(const char* name);
		Element* createMenu(UI_ID id);
		Element* createMenu_lua(sol::table params);
		Element* createHelpLayerMenu(UI_ID id);
		Element* createHelpLayerMenu_lua(sol::table params);
		Element* createTooltipMenu(UI_ID id);
		Element* createTooltipMenu_lua();
		void refreshTooltip();
		Element* findMenu(UI_ID id);
		Element* findMenu_lua(sol::object id);
		Element* findHelpLayerMenu(UI_ID id);
		Element* findHelpLayerMenu_lua(sol::object id);
		Element* getMenuOnTop();
		bool enterMenuMode(UI_ID id);
		bool enterMenuMode_lua(sol::object id);
		bool leaveMenuMode();
		bool closeJournal();
		void acquireTextInput(Element* element);
		void captureMouseDrag(bool capture);
		void preventInventoryMenuToggle(Element* menu);
		Vector3 getPaletteColour(Property prop);

		bool __cdecl onScrollPaneMousewheel(Element*, Property, int, int, Element*);

		MobileActor* getServiceActor();
		void updateDialogDisposition();

		const char* getInventorySelectType();
		void stealHelpMenu();
		void suppressHelpMenu();
		bool isSuppressingHelpMenu();
		void setSuppressingHelpMenu(bool);

		void showDialogueMessage(const char* message, int style, int answerIndex);
		void showDialogueMessage_lua(sol::table params);

		Element* showMessageBox(const char* message, const char* image = nullptr, bool showInDialog = true);

		void logToConsole(const char* text, bool isCommand = false);
		void logToConsole_lua(const char* text, sol::optional<bool> isCommand = false);

		void showBookMenu(const char* text);
		void showScrollMenu(const char* text);

		void showRestMenu(bool resting = true, bool scripted = true);
		void showRestMenu_lua(sol::optional<bool> resting);

		void updateFillBar(UI_ID id, float current, float max);
		void updateHealthFillBar(float current, float max);
		void updateMagickaFillBar(float current, float max);
		void updateFatigueFillBar(float current, float max);
		void updateEncumbranceBar();
		void updatePlayerAttribute(float current, int attributeId);
		void updateStatsPane();
		void updateSpellmakingMenu();
		void updateEnchantingMenu();

		void updateInventoryMenuTiles();
		void updateContentsMenuTiles();
		void updateContentsCompanionElements();
		void updateBarterMenuTiles();
		int updateSelectInventoryTiles();
		sol::optional<int> updateInventorySelectTiles_lua();
		void forcePlayerInventoryUpdate();
		void updateInventoryCharacterImage();

		bool isInMenuMode();

		sol::table getPalette_lua(sol::this_state ts, const char* name);

		void showInventorySelectMenu_lua(sol::table params);

		//
		// Helper functions.
		//

		Property toBooleanProperty(bool b);
		bool toBool(Property prop);

		std::optional<Property> getStandardEventFromName(const std::string& name);

		//
		// Perform our hook.
		//

		void hook();
	}
}
