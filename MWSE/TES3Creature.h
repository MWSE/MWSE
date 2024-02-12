#pragma once

#include "TES3Defines.h"
#include "TES3Actor.h"

#include "NINode.h"

namespace TES3 {
	namespace ActorFlagCreature {
		typedef unsigned int value_type;

		enum Flag : value_type {
			Biped = 0x1,
			Respawn = 0x2,
			WeaponAndShield = 0x4,
			IsBase = 0x8,
			Swims = 0x10,
			Flies = 0x20,
			Walks = 0x40,
			Essential = 0x80,

			// Blood is stored 10 bits in, and stored in 3 bits for 8 total options.
			BloodMask = 0x1C00,

			DefaultFlags = Walks | IsBase,
		};

		enum FlagBit {
			BipedBit = 0,
			RespawnBit = 1,
			WeaponAndShieldBit = 2,
			IsBaseBit = 3,
			SwimsBit = 4,
			FliesBit = 5,
			WalksBit = 6,
			EssentialBit = 7,

			BloodBitsFirst = 10,
			BloodBitsLast = 12,
		};
	}

	namespace CreatureType {
		enum CreatureType {
			Normal,
			Daedra,
			Undead,
			Humanoid
		};
	}

	namespace CreatureSkill {
		enum CreatureSkill {
			Combat,
			Magic,
			Stealth
		};
	}

	struct CreatureBase : Actor {
		static constexpr auto OBJECT_TYPE = ObjectType::Creature;

		//
		// Custom functions.
		//

		bool getIsBiped() const;
		void setIsBiped(bool value);
		bool getIsEssential() const;
		void setIsEssential(bool value);
		bool getRespawns() const;
		void setRespawns(bool value);
		bool getUsesEquipment() const;
		void setUsesEquipment(bool value);
		bool getSwims() const;
		void setSwims(bool value);
		bool getFlies() const;
		void setFlies(bool value);
		bool getWalks() const;
		void setWalks(bool value);
	};

	struct Creature : CreatureBase {
		char * model; // 0x6C
		char * name; // 0x70
		Script * script; // 0x74
		Creature * soundGenerator; // 0x78
		int creatureType; // 0x7C
		int level; // 0x80
		int attributes[8]; // 0x84
		int health; // 0xA4
		int magicka; // 0xA8
		int fatigue; // 0xAC
		int soul; // 0xB0
		int skills[3]; // 0xB4
		Range<int> attacks[3]; // 0xC0
		int barterGold; // 0xD8
		SpellList * spellList; // 0xDC
		void * aiPackageList; // 0xE0
		AIConfig * aiConfig; // 0xE4

		Creature();
		~Creature();

		//
		// Custom functions.
		//

		std::reference_wrapper<int[8]> getAttributes();
		std::reference_wrapper<int[3]> getSkills();
		std::reference_wrapper<Range<int>[3]> getAttacks();

		int getSoulValue();
		void setSoulValue(int value);

	};
	static_assert(sizeof(Creature) == 0xE8, "TES3::Creature failed size validation");

	struct CreatureInstance : CreatureBase {
		Creature * baseCreature; // 0x6C
		Item * weapon; // 0x70
		NI::Pointer<NI::Node> sgNode_74;
		int field_78;
		NI::Pointer<NI::Node> sgNode_7C;
		int field_80;
		NI::Pointer<NI::Node> sgNode_84;
		AIPackageConfig * aiPackageConfig; // 0x88

		CreatureInstance() = delete;
		~CreatureInstance() = delete;

		//
		// Base creature access functions.
		//

		AIConfig* getBaseAIConfig() const;
		std::reference_wrapper<int[8]> getBaseAttributes() const;
		std::reference_wrapper<int[3]> getBaseSkills() const;
		std::reference_wrapper<Range<int>[3]> getBaseAttacks() const;
		Creature* getBaseSoundGenerator() const;
		int getBaseCreatureType() const;
		SpellList* getBaseSpells() const;
		void reevaluateEquipment();

		int getBaseSoulValue() const;
		void setBaseSoulValue(int value);

	};
	static_assert(sizeof(CreatureInstance) == 0x8C, "TES3::CreatureInstance failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::CreatureBase)
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::Creature)
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::CreatureInstance)
