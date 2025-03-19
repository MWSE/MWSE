#pragma once

#include "TES3Defines.h"

#include "TES3Object.h"


namespace TES3 {
	namespace ServiceFlag {
		enum ServiceFlag {
			BartersWeapons = 0x1,
			BartersArmor = 0x2,
			BartersClothing = 0x4,
			BartersBooks = 0x8,
			BartersIngredients = 0x10,
			BartersLockpicks = 0x20,
			BartersProbes = 0x40,
			BartersLights = 0x80,
			BartersApparatus = 0x100,
			BartersRepairTools = 0x200,
			BartersMiscItems = 0x400,
			OffersSpells = 0x800,
			BartersEnchantedItems = 0x1000,
			BartersAlchemy = 0x2000,
			OffersTraining = 0x4000,
			OffersSpellmaking = 0x8000,
			OffersEnchanting = 0x10000,
			OffersRepairs = 0x20000
		};

		constexpr auto OffersBarteringMask = BartersWeapons | BartersArmor | BartersClothing | BartersBooks | BartersIngredients | BartersLockpicks | BartersProbes | BartersLights | BartersApparatus | BartersRepairTools | BartersMiscItems | BartersAlchemy;
		static_assert(OffersBarteringMask == 0x27FF, "Bartering mask does not match game code. Was a new bartering type added?");
	}

	struct Class : BaseObject {
		char id[32]; // 0x10
		char name[32]; // 0x30
		int attributes[2]; // 0x50
		int specialization; // 0x58
		int skills[10]; // 0x5C
		int playable; // 0x84
		unsigned int services; // 0x88
		char * description; // 0x8C
		unsigned int descriptionOffset; // 0x90

		static constexpr auto OBJECT_TYPE = ObjectType::Class;

		Class() = delete;
		~Class() = delete;

		//
		// Virtual table overrides.
		//

		const char * getObjectID() const;
		const char * getName() const;

		//
		// Custom functions.
		//

		void setName(const char* name);

		char * loadDescription();
		sol::optional<std::string> getOrLoadDescription();
		void setDescription(const char*);
		void freeDescription();

		std::string getLevelUpImage() const;

		std::reference_wrapper<int[2]> getAttributes();
		std::reference_wrapper<int[10]> getSkills();

		bool getIsPlayable();
		void setIsPlayable(bool value);

		bool getServiceFlag(unsigned int) const;
		void setServiceFlag(unsigned int, bool);

		bool getBartersAlchemy() const;
		void setBartersAlchemy(bool value);
		bool getBartersApparatus() const;
		void setBartersApparatus(bool value);
		bool getBartersArmor() const;
		void setBartersArmor(bool value);
		bool getBartersBooks() const;
		void setBartersBooks(bool value);
		bool getBartersClothing() const;
		void setBartersClothing(bool value);
		bool getBartersEnchantedItems() const;
		void setBartersEnchantedItems(bool value);
		bool getBartersIngredients() const;
		void setBartersIngredients(bool value);
		bool getBartersLights() const;
		void setBartersLights(bool value);
		bool getBartersLockpicks() const;
		void setBartersLockpicks(bool value);
		bool getBartersMiscItems() const;
		void setBartersMiscItems(bool value);
		bool getBartersProbes() const;
		void setBartersProbes(bool value);
		bool getBartersRepairTools() const;
		void setBartersRepairTools(bool value);
		bool getBartersWeapons() const;
		void setBartersWeapons(bool value);
		bool getOffersBartering() const;
		bool getOffersEnchanting() const;
		void setOffersEnchanting(bool value);
		bool getOffersRepairs() const;
		void setOffersRepairs(bool value);
		bool getOffersSpellmaking() const;
		void setOffersSpellmaking(bool value);
		bool getOffersSpells() const;
		void setOffersSpells(bool value);
		bool getOffersTraining() const;
		void setOffersTraining(bool value);

		void setDescription_lua(const char* value);
		sol::table getMajorSkills_lua(sol::this_state ts);
		sol::table getMinorSkills_lua(sol::this_state ts);

	};
	static_assert(sizeof(Class) == 0x94, "TES3::Class failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::Class)
