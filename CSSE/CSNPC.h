#pragma once

#include "CSActor.h"
#include "CSAIConfig.h"
#include "CSSpellList.h"

namespace se::cs {
	struct NPC : Actor {
		struct substruct_0x90 {
			const char* raceId; // 0x0
			const char* classId; // 0x4
			const char* factionId; // 0x8
			const char* headBodyPartId; // 0xC
			const char* hairBodyPartId; // 0x10
		};
		int unknown_0x7C;
		int unknown_0x80;
		char* model; // 0x84
		char* name; // 0x88
		Script* script; // 0x8C
		substruct_0x90* unknown_0x90;
		short level; // 0x94
		byte attributes[8]; // 0x96
		byte skills[27]; // 0x9E
		short health; // 0xBA
		short magicka; // 0xBC
		short fatigue; // 0xBE
		byte baseDisposition; // 0xC0
		byte reputation; // 0xC1
		byte factionRank; // 0xC2
		int barterGold; // 0xC4
		Race* race; // 0xC8
		Class* class_; // 0xCC
		Faction* faction; // 0xD0
		BodyPart* head; // 0xD4
		BodyPart* hair; // 0xD8
		SpellList spellList; // 0xDC
		int unknown_0xF4;
		AIConfig aiConfig; // 0xF8

		const char* getFactionRankName() const;
	};
	static_assert(sizeof(NPC) == 0x108, "NPC failed size validation");
}
