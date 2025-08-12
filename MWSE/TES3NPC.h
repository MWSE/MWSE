#pragma once

#include "TES3Defines.h"

#include "TES3Actor.h"
#include "TES3AIConfig.h"
#include "TES3SpellList.h"

namespace TES3 {
	namespace ActorFlagNPC {
		typedef unsigned int value_type;

		enum Flag : value_type {
			Female = 0x1,
			Essential = 0x2,
			Respawn = 0x4,
			IsBase = 0x8,
			AutoCalc = 0x10,
			BloodSkeleton = 0x400,
			BloodMetal = 0x800,
		};

		enum FlagBit {
			FemaleBit = 0,
			EssentialBit = 1,
			RespawnBit = 2,
			IsBaseBit = 3,
			AutoCalcBit = 4,
			BloodSkeletonBit = 10,
			BloodMetalBit = 11,
		};
	}

	struct NPCBase : Actor {
		static constexpr auto OBJECT_TYPE = ObjectType::NPC;

		// No data, this is only used for shared functions.

		NPCBase() = delete;
		~NPCBase() = delete;

		//
		// Related this-call functions.
		//

		int getBaseDisposition(bool);
		bool isGuard();

		bool getIsFemale() const;
		void setIsFemale(bool value);
		bool getIsAutoCalc() const;
		void setIsAutoCalc(bool value);
		bool getIsEssential_legacy() const;
		void setIsEssential_legacy(bool value);
		bool getRespawns_legacy() const;
		void setRespawns_legacy(bool value);

		float getWeight() const;
		float getHeight() const;

	};

	struct NPC : NPCBase {
		struct LinkedObjectIds {
			const char* raceId; // 0x0
			const char* classId; // 0x4
			const char* birthsignId; // 0x8
			const char* headId; // 0xC
			const char* hairId; // 0x10
		};
		char * model; // 0x6C
		char * name; // 0x70
		Script * script; // 0x74
		LinkedObjectIds* linkedObjectIDs; // 0x78
		short level; // 0x7C
		unsigned char attributes[8]; // 0x7E
		unsigned char skills[27]; // 0x86
		unsigned char unknown_0xA1; // Padding.
		short health; // 0xA2
		short magicka; // 0xA4
		short fatigue; // 0xA6
		signed char baseDisposition; // 0xA8
		unsigned char initialReputation; // 0xA9
		unsigned char factionRank; // 0xAA
		char unknown_0xAB; // Padding.
		int barterGold; // 0xAC
		Race * race; // 0xB0
		Class * class_; // 0xB4
		Faction * faction; // 0xB8
		BodyPart * head; // 0xBC
		BodyPart * hair; // 0xC0
		SpellList spellList; // 0xC4
		void * aiPackageList; // 0xDC
		AIConfig aiConfig; // 0xE0

		NPC() = delete;
		~NPC() = delete;

		//
		// Custom functions.
		//

		Race* getRace() const;
		void setRace(Race* race);

		std::reference_wrapper<unsigned char[8]> getAttributes();
		std::reference_wrapper<unsigned char[27]> getSkills();

		sol::optional<int> getSoulValue();
	};
	static_assert(sizeof(NPC) == 0xF0, "TES3::NPC failed size validation");
	static_assert(sizeof(NPC::LinkedObjectIds) == 0x14, "TES3::NPC linked object IDs failed size validation");

	struct NPCInstance : NPCBase {
		NPC * baseNPC; // 0x6C
		short baseDisposition; // 0x70
		unsigned char reputation; // 0x72
		char unknown_0x73; // Padding.
		AIPackageConfig * aiPackageConfig; // 0x74

		NPCInstance() = delete;
		~NPCInstance() = delete;

		//
		// Related this-call functions.
		//

		int getDisposition(bool clamp = false);
		void reevaluateEquipment();

		//
		// Custom functions.
		//

		unsigned char getReputation();
		void setReputation(unsigned char);

		short getBaseDisposition();
		void setBaseDisposition(short);

		void setFactionRank(unsigned char);

		int getDisposition_lua();

		std::reference_wrapper<unsigned char[8]> getAttributes();
		std::reference_wrapper<unsigned char[27]> getSkills();

		sol::optional<int> getBaseSoulValue();

		Class* getBaseClass();
		Faction* getBaseFaction();
		unsigned char getBaseFactionRank();
		Race* getBaseRace();
		Script* getBaseScript();
		SpellList* getBaseSpellList();

	};
	static_assert(sizeof(NPCInstance) == 0x78, "TES3::NPCInstance failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::NPCBase)
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::NPC)
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::NPCInstance)
