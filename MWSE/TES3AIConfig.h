#pragma once

#include "TES3Attachment.h"
#include "TES3IteratedList.h"

namespace TES3 {
	struct AIConfig {
		short hello; // 0x0
		unsigned char fight; // 0x2
		unsigned char flee; // 0x3
		unsigned char alarm; // 0x4
		char unknown_0x5[3];
		unsigned int merchantFlags; // 0x8
		IteratedList<TravelDestination*> * travelDestinations; // 0xC

		AIConfig();
		~AIConfig();

		//
		// Custom functions.
		//

		bool tradesItemType(ObjectType::ObjectType objectType) const;

		bool getServiceFlag(unsigned int) const;
		void setServiceFlag(unsigned int, bool);

		bool getBartersAlchemy() const;
		void setBartersAlchemy(bool set);

		bool getBartersApparatus() const;
		void setBartersApparatus(bool set);

		bool getBartersArmor() const;
		void setBartersArmor(bool set);

		bool getBartersBooks() const;
		void setBartersBooks(bool set);

		bool getBartersClothing() const;
		void setBartersClothing(bool set);

		bool getBartersEnchantedItems() const;
		void setBartersEnchantedItems(bool set);

		bool getBartersIngredients() const;
		void setBartersIngredients(bool set);

		bool getBartersLights() const;
		void setBartersLights(bool set);

		bool getBartersLockpicks() const;
		void setBartersLockpicks(bool set);

		bool getBartersMiscItems() const;
		void setBartersMiscItems(bool set);

		bool getBartersProbes() const;
		void setBartersProbes(bool set);

		bool getBartersRepairTools() const;
		void setBartersRepairTools(bool set);

		bool getBartersWeapons() const;
		void setBartersWeapons(bool set);

		bool getOffersBartering() const;

		bool getOffersEnchanting() const;
		void setOffersEnchanting(bool set);

		bool getOffersRepairs() const;
		void setOffersRepairs(bool set);

		bool getOffersSpellmaking() const;
		void setOffersSpellmaking(bool set);

		bool getOffersSpells() const;
		void setOffersSpells(bool set);

		bool getOffersTraining() const;
		void setOffersTraining(bool set);

	};
	static_assert(sizeof(AIConfig) == 0x10, "TES3::AIConfig failed size validation");
}
