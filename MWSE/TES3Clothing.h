#pragma once

#include "TES3Defines.h"

#include "TES3Item.h"
#include "TES3IteratedList.h"
#include "TES3WearablePart.h"

namespace TES3 {
	namespace ClothingSlot {
		typedef unsigned char value_type;

		enum ClothingSlot : value_type {
			Pants = 0x0,
			Shoes = 0x1,
			Shirt = 0x2,
			Belt = 0x3,
			Robe = 0x4,
			RightGlove = 0x5,
			LeftGlove = 0x6,
			Skirt = 0x7,
			Ring = 0x8,
			Amulet = 0x9,

			First = Pants,
			Last = Amulet,
		};
	}

	struct Clothing : Item {
		IteratedList<TES3::BaseObject*> stolenList; // 0x30
		char * name; // 0x44
		Script * script; // 0x48
		char * model; // 0x4C
		char * icon; // 0x50
		WearablePart parts[7];  // 0x54
		ClothingSlot::value_type slot; // 0xA8
		float weight; // 0xAC
		unsigned short value; // 0xB0
		unsigned short enchantCapacity; // 0xB2
		Enchantment * enchantment; // 0xB4

		static constexpr auto OBJECT_TYPE = ObjectType::Clothing;

		Clothing();
		~Clothing();

		//
		// Other related this-call functions.
		//

		void setupBodyParts(BodyPartManager* bodyPartManager, bool isFemale, bool isFirstPerson);

		//
		// Custom functions.
		//

		void addActiveBodyParts(BodyPartManager* bodyPartManager, bool isFemale, bool isFirstperson);
		void removeBodyPartsUnder(BodyPartManager* bodyPartManager) const;

		void setIconPath(const char* path);
		const char* getSlotName();

		std::reference_wrapper<WearablePart[7]> getParts();

		bool isUsableByBeasts() const;
	};
	static_assert(sizeof(Clothing) == 0xB8, "TES3::Clothing failed size validation");

	struct ClothingSlotData {
		int slot;
		std::string name;
	};
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::Clothing)
