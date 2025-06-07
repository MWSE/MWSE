#pragma once

#include "TES3UIDefines.h"
#include "TES3Vectors.h"

namespace TES3::UI {
	UI_ID registerID(const char* name);
	const char* lookupID(UI_ID id);
	const char* lookupID(Property id);
	const char* lookupID_lua(sol::object id);
	Property registerProperty(const char* name);
	Element* createMenu(UI_ID id);
	Element* createMenu_lua(sol::table params);
	Element* createHelpLayerMenu(UI_ID id);
	Element* createHelpLayerMenu_lua(sol::table params);
	Element* createTooltipMenu(UI_ID id);
	Element* createTooltipMenu_lua(sol::optional<sol::table> params);
	void refreshTooltip();
	Element* findMenu(UI_ID id);
	Element* findMenu(const char* id);
	Element* findMenu_lua(sol::object id);
	Element* findHelpLayerMenu(UI_ID id);
	Element* findHelpLayerMenu(const char* id);
	Element* findHelpLayerMenu_lua(sol::object id);
	Element* getMenuOnTop();
	bool enterMenuMode(UI_ID id);
	bool enterMenuMode(const char* id);
	bool enterMenuMode_lua(sol::object id);
	bool leaveMenuMode();
	void acquireTextInput(Element* element);
	void captureMouseDrag(bool capture);
	void preventInventoryMenuToggle(Element* menu);
	Vector3 getPaletteColour(Property prop);

	bool __cdecl onScrollPaneMousewheel(Element*, Property, int, int, Element*);

	MobileActor* getServiceActor();
	void updateDialogDisposition();

	Reference* getConsoleReference();
	void __cdecl setConsoleReference(Reference* reference);

	Element* getCursor();
	InventoryTile* getCursorTile();

	std::tuple<unsigned int, unsigned int> getViewportSize_lua();
	float getViewportScale();

	const char* getInventorySelectType();
	void stealHelpMenu();
	void suppressHelpMenu();
	bool isSuppressingHelpMenu();
	void setSuppressingHelpMenu(bool);

	void showDialogueMessage(const char* message, int style, int answerIndex);
	void showDialogueMessage_lua(sol::table params);
	void __cdecl updateTopicsList();

	Element* showMessageBox(const char* message, const char* image = nullptr, bool showInDialog = true);

	void logToConsole(const char* text, bool isCommand = false);
	void logToConsole_lua(const char* text, sol::optional<bool> isCommand = false);

	void hideCursor();
	void closeBookMenu();
	void closeDialogueMenu();
	void closeScrollMenu();
	bool showJournal();
	bool closeJournal();
	void showBookMenu(const char* text);
	void showScrollMenu(const char* text);
	void showRestMenu(bool resting = true, bool scripted = true);
	void showRestMenu_lua(sol::optional<bool> resting);

	void __cdecl showSpellmakingMenu();
	void showSpellmakingMenuWithOverride(MobileActor* serviceActorOverride);
	MobileActor* __cdecl getSpellmakingServiceActor();

	void updateFillBar(UI_ID id, float current, float max);
	void updateHealthFillBar(float current, float max);
	void updateMagickaFillBar(float current, float max);
	void updateFatigueFillBar(float current, float max);
	void updateCurrentMagicFillBar(float current, float max);
	void updateCurrentMagicFromSpell(const char* iconPath, const char* spellName, Spell* spell);
	void updateCurrentMagicFromEquipmentStack(const char* iconPath, const char* itemName, EquipmentStack* equipmentStack);
	void updateEncumbranceBar();
	void updatePlayerAttribute(float current, int attributeId);
	void updateStatsPane();
	void updateMagicMenuEnchantedItemSelection();
	void updateMagicMenuSelection();
	void updateSpellmakingMenu();
	void updateEnchantingMenu();
	void flagSkillUpdated(int unknown);

	void updateInventoryMenuTiles();
	void updateContentsMenuTiles();
	void updateContentsCompanionElements();
	void updateBarterMenuTiles();
	int updateSelectInventoryTiles();
	sol::optional<int> updateInventorySelectTiles_lua();
	void forcePlayerInventoryUpdate();
	void updateInventoryWindowTitle();
	void updateInventoryCharacterImage();

	bool isInMenuMode();

	void moveMenuToFront(Element* menu);
	void moveMenuToFront_lua(sol::object id);

	sol::table getPalette_lua(sol::this_state ts, const char* name);

	void showMagicSelectMenu_lua(sol::table params);
	void showInventorySelectMenu_lua(sol::table params);

	void __cdecl createResponseText(Element* parent, const char* text, int type, int answerIndex = -1);
	void createResponseText_lua(sol::table params);
	void choice(const char* text, int index);

	std::tuple<int, int> textLayoutGetFontHeight_lua(sol::table params);
	void textLayoutGetTextExtent(TES3::Font* font, const char* text, float* out_width, float* out_verticalAdvance, int maxCharsOrLineMode, bool useLineHeight);
	std::tuple<int, int, int> textLayoutGetTextExtent_lua(sol::table params);
	int textLayoutWrapTextInPlace(TES3::Font* font, char* textBuffer, unsigned int maxWidth, bool ignoreLinkDelimiters, char newlineReplacement);
	std::tuple<std::string, int> textLayoutWrapText_lua(sol::table params);

	void showLoadingMenu(const char* title, float progress);
	void updateLoadingMenu(float progress);
	void destroyLoadingMenu();

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
