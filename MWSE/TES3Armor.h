#pragma once

#include "TES3ArmorFlags.h"
#include "TES3Defines.h"

#include "TES3Item.h"
#include "TES3WearablePart.h"

#include "NIIteratedList.h"

namespace TES3 {
	struct Armor : Item {
		NI::IteratedList<TES3::BaseObject*> stolenList; // 0x30
		char * name; // 0x44
		char * model; // 0x48
		Script * script; // 0x4C
		char * icon; // 0x50
		WearablePart parts[7];  // 0x54
		ArmorSlot::value_type slot; // 0xA8
		float weight; // 0xAC
		int value; // 0xB0
		int maxCondition; // 0xB4
		int enchantCapacity; // 0xB8
		int armorRating; // 0xBC
		Enchantment * enchantment; // 0xC0

		static constexpr auto OBJECT_TYPE = ObjectType::Armor;

		Armor();
		~Armor();

		//
		// Other related this-call functions.
		//

		float calculateArmorRating(MobileActor * actor);
		float calculateArmorRatingForNPC(NPC * npc);
		const char * getSlotName();
		int getWeightClass();

		void setupBodyParts(BodyPartManager* bodyPartManager, bool isFemale, bool isFirstPerson);

		//
		// Custom functions.
		//

		void addActiveBodyParts(BodyPartManager* bodyPartManager, bool isFemale, bool isFirstperson);
		void removeBodyPartsUnder(BodyPartManager* bodyPartManager) const;

		float getArmorScalar() const;

		// Overwrite vtable call to actually do something.
		void setDurability(int value);

		void setIconPath(const char* path);

		std::reference_wrapper<WearablePart[7]> getParts();

		float calculateArmorRating_lua(sol::object actor);

		bool isClosedHelmet() const;
		bool isUsableByBeasts() const;
	};
	static_assert(sizeof(Armor) == 0xC4, "TES3::Armor failed size validation");

	struct ArmorSlotData {
		int slot;
		std::string name;
		float weight;
		float armorScalar;
	};
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::Armor)
