#pragma once

#include "CSBaseObject.h"

namespace se::cs {
	struct Race : BaseObject {
		struct SkillBonus {
			int skill;
			unsigned int bonus;
		};
		struct BaseAttribute {
			unsigned int male;
			unsigned int female;
		};
		struct HeightWeight {
			float male;
			float female;
		};
		struct BodyParts {
			BodyPart* head;
			BodyPart* hair;
			BodyPart* neck;
			BodyPart* chest;
			BodyPart* groin;
			BodyPart* hands;
			BodyPart* wrist;
			BodyPart* forearm;
			BodyPart* upperArm;
			BodyPart* foot;
			BodyPart* ankle;
			BodyPart* knee;
			BodyPart* upperLeg;
			BodyPart* clavicle;
			BodyPart* tail;
			BodyPart* vampireHead;
			BodyPart* vampireHair;
			BodyPart* vampireNeck;
			BodyPart* vampireChest;
			BodyPart* vampireGroin;
			BodyPart* vampireHands;
			BodyPart* vampireWrist;
			BodyPart* vampireForearm;
			BodyPart* vampireUpperArm;
			BodyPart* vampireFoot;
			BodyPart* vampireAnkle;
			BodyPart* vampireKnee;
			BodyPart* vampireUpperLeg;
			BodyPart* vampireClavicle;
			BodyPart* vampireTail;
		};
		enum class PartIndex : int {
			Head,
			Hair,
			Neck,
			Chest,
			Groin,
			Hands,
			Wrist,
			Forearm,
			UpperArm,
			Foot,
			Ankle,
			Knee,
			UpperLeg,
			Clavicle,
			Tail,
			COUNT,
		};
		enum Flag : unsigned int {
			Playable = 0x1,
			Beast = 0x2,
		};
		enum FlagBit : unsigned int {
			PlayableBit = 0,
			BeastBit = 1,
		};

		char id[32]; // 0x10
		char name[32]; // 0x30
		SkillBonus skillBonuses[7]; // 0x50
		BaseAttribute baseAttributes[8]; // 0x88 // Index corresponds to Attributes enum.
		HeightWeight height; // 0xC8
		HeightWeight weight; // 0xD0
		unsigned int flags; // 0xD8 // 1 = playable, 2 = beast
		SpellList* abilities; // 0xDC
		char* description; // 0xE0
		union {
			// Split struct out for legacy lua access.
			struct {
				BodyParts maleBody; // 0xE4
				BodyParts femaleBody; // 0x15C
			};
			BodyPart* bodyParts[int(PartIndex::COUNT) * 2 * 2]; // 0xE4 // Body parts for both sexes and each vampirism state.
		};

		bool search(const std::string_view& needle, const SearchSettings& settings, std::regex* regex = nullptr) const;
	};
	static_assert(sizeof(Race) == 0x1D4, "Race failed size validation");
}
