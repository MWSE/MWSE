#pragma once

#include "TES3Defines.h"

#include "TES3Dialogue.h"
#include "TES3DialogueConditional.h"
#include "TES3Object.h"

namespace TES3 {
	enum class DialogueInfoFilterType {
		Actor,
		Race,
		Class,
		NPCFaction,
		Cell,
		PCFaction,
		SoundPath,
		Conditional0,
		Conditional1,
		Conditional2,
		Conditional3,
		Conditional4,
		Conditional5,
	};

	constexpr auto DialogueInfoConditionalCount = 6u;

	namespace ObjectFlag {
		typedef unsigned int value_type;

		enum DialogueInfoFlag : value_type {
			QuestName = 0x40,
			QuestStarted = 0x80,
			QuestFinished = 0x100,
			QuestRestart = 0x200,
			HasResultText = 0x2000,
		};

		enum DialogueInfoFlagBit {
			QuestNameBit = 6,
			QuestStartedBit = 7,
			QuestFinishedBit = 8,
			QuestRestartBit = 9,
			HasResultTextBit = 13,
		};
	}

	struct DialogueInfoFilterNode {
		DialogueInfoFilterType tag;
		DialogueInfoFilterNode* next;
		union {
			BaseObject* object;
			Actor* actor;
			Race* npcRace;
			Class* npcClass;
			Faction* npcFaction;
			Cell* cell;
			Faction* pcFaction;
			const char* soundPath;
			DialogueConditional* conditional;
		};

		DialogueInfoFilterNode() = delete;
		~DialogueInfoFilterNode() = delete;
	};

	struct DialogueInfo : BaseObject {
		struct LoadLinkNode {
			char * name;
			char * previous;
			char * next;
		};
		LoadLinkNode * loadLinkNode; // 0x10
		DialogueType type; // 0x14
		union {
			int disposition; // 0x18
			int journalIndex; // 0x18
		};
		signed char npcRank; // 0x1C
		signed char npcSex; // 0x1D
		signed char pcRank; // 0x1E
		DialogueInfoFilterNode* conditions; // 0x20
		long espFileOffset; // 0x24
		Actor* firstHeardFrom; // 0x28

		static constexpr auto OBJECT_TYPE = ObjectType::DialogueInfo;

		DialogueInfo() = delete;
		~DialogueInfo() = delete;

		//
		// Other related this-call functions.
		//

		const char* getText() const;

		bool loadId();
		void unloadId();

		enum class FilterSource : int {
			None = 0,
			Unknown = 1,

			MCP_Barter = 17,
			MCP_Repair = 18,
			MCP_Spells = 19,
			MCP_Training = 20,
			MCP_Travel = 21,
			MCP_Spellmaking = 22,
			MCP_Enchanting = 23,

			MCP_Offset = 16,
		};

		bool filterVanillaReplacer(Object* speaker, Reference* reference, FilterSource source, Dialogue* dialogue) const;
		bool filter(Object* speaker, Reference* reference, FilterSource source, Dialogue* dialogue);
		void runScript(Reference * reference);

		//
		// Access to associated data.
		//

		static char* getLastLoadedText();
		static void setLastLoadedText(const char* text);

		static char* getLastLoadedScript();
		static void setLastLoadedScript(const char* text);

		const char* getSoundPath();

		//
		// Custom functions.
		//

		sol::optional<std::string> getID();

		sol::optional<int> getJournalIndex_lua() const;
		void setJournalIndex_lua(int value);

		BaseObject* getFilterObject(TES3::DialogueInfoFilterType type);

		Actor* getFilterActor();
		Race* getFilterNPCRace();
		Class* getFilterNPCClass();
		Faction* getFilterNPCFaction();
		Cell* getFilterNPCCell();
		Faction* getFilterPCFaction();

		sol::optional<bool> isQuestName() const;
		sol::optional<bool> isQuestFinished() const;
		sol::optional<bool> isQuestRestart() const;

		Dialogue* findDialogue() const;

		std::string toJson();

	};
	static_assert(sizeof(DialogueInfo) == 0x2C, "TES3::DialogueInfo failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::DialogueInfo)
