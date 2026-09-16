#pragma once

#include "CSActor.h"

#include "NIRange.h"
#include "TES3CreatureFlags.h"

namespace se::cs {
	struct Creature : Actor {
		int unknown_0x7C;
		int unknown_0x80;
		char* model;
		char* name;
		Script* script;
		Creature* soundGenerator;
		int creatureType;
		int level;
		int attributes[8];
		int health;
		int magicka;
		int fatigue;
		int soul;
		int skills[3];
		NI::Range<int> attacks[3];
		int barterGold;
		SpellList* spellList;
		void* aiPackageList;
		AIConfig* aiConfig;

		const char* getMovementType() const;

		bool getIsBipedal() const;
		bool getUsesWeaponAndShield() const;
	};
	static_assert(sizeof(Creature) == 0x100, "CS::Creature failed size validation");
}
