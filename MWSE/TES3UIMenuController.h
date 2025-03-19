#pragma once

#include "TES3UIDefines.h"
#include "TES3UIVector.h"
#include "NINode.h"
#include "NIPick.h"

namespace TES3::UI {
	struct MenuInputController {
		struct Event {
			int type;
			int data0;
			int data1;
			Element* element;

			Event() = delete;
			~Event() = delete;
		};

		NI::Pick pick; // 0x0
		MenuController* menuController; // 0x38
		Element* pointerMoveEventSource; // 0x3C
		Element* pointerMovePreviousEventSource; // 0x40
		Element* buttonPressEventSource; // 0x44
		Element* buttonPressPreviousEventSource; // 0x48
		Element* unknown_0x4C;
		int lastMouseX; // 0x50
		int lastMouseY; // 0x54
		bool flagUpdateOnMouseReleased; // 0x58
		bool isMouseButtonHeldDown; // 0x59
		bool transitionToMenuMode; // 0x5A
		int repeatKeyCode; // 0x5C
		float repeatKeyTimer; // 0x60
		Event events[2]; // 0x64
		bool shiftKeyDown; // 0x84
		Element* textInputFocus; // 0x88
		Vector<void*> repeatingEvents; // 0x8C
		unsigned int modifierKeyFlags; // 0x9C 
		bool chargenNameDone; // 0xA0
		bool unknown_0xA1;
		bool chargenClassDone; // 0xA2
		bool chargenRaceSexDone; // 0xA3
		bool chargenBirthsignDone; // 0xA4
		Reference* mouseoverWorldObject; // 0xA8

		static Object* lastTooltipObject;
		static ItemData* lastTooltipItemData;
		static int lastTooltipCount;
		static Element* lastTooltipSource;
		static int lastKeyPressDIK;

		MenuInputController() = delete;
		~MenuInputController() = delete;

		Element* getTextInputElement() const;
		void acquireTextInput(Element* element);
		void displayObjectTooltip(TES3::Object* object, TES3::ItemData* itemData, int count = 0);

		//
		// Other related this-call functions.
		//

		void flushBufferedTextEvents();

		//
		// Custom functions.
		//

		static Element* previousTextInputFocus;

		void updateObjectTooltip();
	};
	static_assert(sizeof(MenuInputController) == 0xAC, "TES3::UI::MenuInputController failed size validation");
	static_assert(sizeof(MenuInputController::Event) == 0x10, "TES3::UI::MenuInputController::Event failed size validation");
	static_assert(offsetof(MenuInputController, textInputFocus) == 0x88, "TES3::UI::MenuInputController::textInputFocus failed offset validation");

	struct FontColor {
		int r;
		int g;
		int b;

		FontColor() = delete;
		~FontColor() = delete;
	};
	static_assert(sizeof(FontColor) == 0xC, "TES3::UI::FontColor failed size validation");

	enum FontColorId : unsigned int {
		normal,
		normal_over,
		normal_pressed,
		active,
		active_over,
		active_pressed,
		disabled,
		disabled_over,
		disabled_pressed,
		link,
		link_over,
		link_pressed,
		journal,
		journal_over,
		journal_pressed,
		journal_topic,
		journal_topic_over,
		journal_topic_pressed,
		journal_finished_quest_topic,
		journal_finished_quest_topic_over,
		journal_finished_quest_topic_pressed,
		answer,
		answer_over,
		answer_pressed,
		header,
		notify,
		big_normal,
		big_normal_over,
		big_normal_pressed,
		big_link,
		big_link_over,
		big_link_pressed,
		big_answer,
		big_answer_over,
		big_answer_pressed,
		big_header,
		big_notify,
		background,
		focus,
		npc_health,
		health,
		magic,
		fatigue,
		misc,
		weapon_fill,
		magic_fill,
		positive,
		negative,
		count,

		COLOR_COUNT,

		MAX_ID = count,
	};

	namespace MenuControllerGameplayFlags {
		enum GameplayFlags : unsigned int {
			ShowCombatStats = 0x1,
			LightingUpdateDisabled = 0x2,
			GodModeEnabled = 0x4,
			AIDisabled = 0x8,
			BordersEnabled = 0x10,
			SkyDisabled = 0x20,
			WorldDisabled = 0x40,
			WireframeEnabled = 0x80,
			CollisionDisabled = 0x100,
			CollisionBoxesEnabled = 0x200,
			CollisionGridEnabled = 0x400,
			DebugTextEnabled = 0x800,
			TextureStringEnabled = 0x1000,
			GridEnabled = 0x2000,
			MenusDisabled = 0x4000,
			ScriptOutputEnabled = 0x8000,
			MoveOneToOneEnabled = 0x10000,
			WaterDisabled = 0x20000,
			MagicStats = 0x40000,
			DialogueStats = 0x80000,
			KillStats = 0x100000,
			ScriptsDisabled = 0x400000,
			ShowPathGrid = 0x800000,
			FullHelpEnabled = 0x1000000,
			FogOfWarDisabled = 0x2000000,
		};
	}

	struct MenuController {
		Element* mainRoot; // 0x0
		Element* helpRoot; // 0x4
		NI::Node* mainNode; // 0x8
		NI::Node* helpNode; // 0xC
		MenuInputController* menuInputController; // 0x10
		int unknown_0x14;
		void* unknown_0x18;
		int unknown_0x1C;
		ScriptCompiler* scriptCompiler; // 0x20
		unsigned int gameplayFlags; // 0x24
		int helpDelay; // 0x28
		bool flagClearHelpMenu; // 0x2C
		TES3::UI::Element* menuAtFront; // 0x30
		bool inventoryMenuEnabled; // 0x34
		bool statsMenuEnabled; // 0x35
		bool magicMenuEnabled; // 0x36
		bool mapMenuEnabled; // 0x37
		char unknown_0x38;
		char unknown_0x39;
		char padding_0x3A[2];
		UI_ID unknown_0x3C;
		FontColor fontColors[FontColorId::COLOR_COUNT];
		void* unknown_0x28C;
		void* unknown_0x290;
		int unknown_0x294;
		int unknown_0x298;
		float unknown_0x29C;
		char unknown_0x2A0;

		MenuController() = delete;
		~MenuController() = delete;

		//
		// Other related this-call functions.
		//

		void setInventoryMenuEnabled(bool enabled);
		void setMagicMenuEnabled(bool enabled);
		void setMapMenuEnabled(bool enabled);
		void setStatsMenuEnabled(bool enabled);

		static void __cdecl updateFogOfWarRenderState();

		//
		// Custom functions.
		//

		bool getInventoryMenuEnabled() const;
		bool getMagicMenuEnabled() const;
		bool getMapMenuEnabled() const;
		bool getStatsMenuEnabled() const;

		bool getGodModeEnabled() const;
		void setGodModeEnabled(bool state);
		bool getLightingUpdatesDisabled() const;
		void setLightingUpdatesDisabled(bool state);
		bool getAIDisabled() const;
		void setAIDisabled(bool state);
		bool getBordersEnabled() const;
		void setBordersEnabled(bool state);
		bool getSkyDisabled() const;
		void setSkyDisabled(bool state);
		bool getWorldDisabled() const;
		void setWorldDisabled(bool state);
		bool getWireframeEnabled() const;
		void setWireframeEnabled(bool state);
		bool getCollisionDisabled() const;
		void setCollisionDisabled(bool state);
		bool getCollisionBoxesEnabled() const;
		void setCollisionBoxesEnabled(bool state);
		bool getFogOfWarDisabled() const;
		void setFogOfWarDisabled(bool state);
		bool getMenusDisabled() const;
		void setMenusDisabled(bool state);
		bool getScriptsDisabled() const;
		void setScriptsDisabled(bool state);
		bool getShowPathGrid() const;
		void setShowPathGrid(bool state);

		std::reference_wrapper<FontColor[FontColorId::MAX_ID + 1]> getFontColors();

	};
	static_assert(sizeof(MenuController) == 0x2A4, "TES3::UI::MenuController failed size validation");
}