#pragma once

#include "TES3Defines.h"

#include "TES3Object.h"

namespace TES3 {
	namespace SkillID {
		enum SkillID {
			Invalid = -1,
			Block,
			Armorer,
			MediumArmor,
			HeavyArmor,
			BluntWeapon,
			LongBlade,
			Axe,
			Spear,
			Athletics,
			Enchant,
			Destruction,
			Alteration,
			Illusion,
			Conjuration,
			Mysticism,
			Restoration,
			Alchemy,
			Unarmored,
			Security,
			Sneak,
			Acrobatics,
			LightArmor,
			ShortBlade,
			Marksman,
			Mercantile,
			Speechcraft,
			HandToHand,
			FirstSkill = Block,
			LastSkill = HandToHand
		};
	}

	namespace SkillType {
		enum SkillType {
			Major,
			Minor,
			Misc
		};
	}

	namespace SkillSpecialization {
		enum SkillSpecialization {
			Invalid = -1,
			Combat,
			Magic,
			Stealth,
			FirstSpecialization = Combat,
			LastSpecialization = Stealth
		};
	}

	struct Skill : BaseObject {
		int skill;
		int governingAttribute;
		int specialization;
		float progressActions[4];
		char* description;
		unsigned int descriptionFileOffset;

		static constexpr auto OBJECT_TYPE = ObjectType::Skill;

		Skill() = delete;
		~Skill() = delete;

		char* loadDescription();
		void freeDescription();

		//
		// Custom functions
		//

		sol::optional<std::string> getAndLoadDescription();

		const char* getName() const;

		int getNameGMST() const;

		std::string getIconPath() const;

		std::reference_wrapper<float[4]> getProgressActions();

		//
		// Related static data.
		//

		static constexpr auto ICON_PATHS = reinterpret_cast<const char**>(0x7BB158);
		static constexpr auto NAME_GMSTS = reinterpret_cast<int*>(0x794430);

	};
	static_assert(sizeof(Skill) == 0x34, "TES3::Skill failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::Skill)
