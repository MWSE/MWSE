#pragma once

#include "CSBaseObject.h"

namespace se::cs {
	struct DialogueInfo : BaseObject {
		struct LoadLinkNode {
			const char* name; // 0x0
			const char* previous; // 0x4
			const char* next; // 0x8
		};
		struct Condition {
			union {
				int integer;
				const char* string;
				BaseObject* object;
				Dialogue* dialogue;
			} compareValue; // 0x0
			char type; // 0x4
			char unknown_0x5;
			int compareOp; //0x8
			float value; // 0xC

			enum CompareOp {
				Equal,
				NotEqual,
				GreaterThan,
				GreaterThanOrEqual,
				LessThan,
				LessThanOrEqual,
			};

			enum Type {
				TypeNone,
				TypeFunction,
				TypeGlobal,
				TypeLocal,
				TypeJournal,
				TypeItem,
				TypeDead,
				TypeNotID,
				TypeNotFaction,
				TypeNotClass,
				TypeNotRace,
				TypeNotCell,
				TypeNotLocal,
			};
		};
		LoadLinkNode* loadLinkNodes; // 0x10
		bool hasSound; // 0x14
		int filterDisposition; // 0x18
		byte filterRank; // 0x1C
		byte filterGender; // 0x1D
		byte filterPlayerRank; // 0x1E
		Condition conditions[6]; // 0x20
		const char* text; // 0x80
		const char* resultsText; // 0x84
		Actor* filterActor; // 0x88
		Race* filterRace; // 0x8C
		Class* filterClass; // 0x90
		Faction* filterFaction; // 0x94
		Cell* filterCell; // 0x98
		Faction* filterPlayerFaction; // 0x9C
		const char* soundId; // 0xA0

		Dialogue* getDialogue() const;

		bool filter(Object* actor, Reference* reference = nullptr, int source = 1, Dialogue* dialogue = nullptr) const;

		bool search(const std::string_view& needle, const SearchSettings& settings, std::regex* regex = nullptr) const;
	};
	static_assert(sizeof(DialogueInfo) == 0xA4, "TES3::DialogueInfo failed size validation");
	static_assert(sizeof(DialogueInfo::LoadLinkNode) == 0xC, "TES3::DialogueInfo::LoadLinkNode failed size validation");
	static_assert(sizeof(DialogueInfo::Condition) == 0x10, "TES3::DialogueInfo::Condition failed size validation");
}
