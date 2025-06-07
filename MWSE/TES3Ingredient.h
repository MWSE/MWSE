#pragma once

#include "TES3Defines.h"

#include "TES3Item.h"
#include "TES3IteratedList.h"

namespace TES3 {
	struct Ingredient : Item {
		IteratedList<TES3::BaseObject*> stolenList; // 0x30
		char name[32]; // 0x44
		char model[32]; // 0x64
		Script * script; // 0x84
		char texture[32]; // 0x88
		float weight; // 0xA8
		int value; // 0xAC
		int effects[4]; // 0xB0
		int effectSkillIds[4]; // 0xC0
		int effectAttributeIds[4]; // 0xD0

		static constexpr auto OBJECT_TYPE = ObjectType::Ingredient;

		Ingredient() = delete;
		~Ingredient() = delete;

		//
		// Custom functions.
		//

		// Isn't defined by the vtable, have to overwrite here.
		void setName(const char* n);

		void setIconPath(const char* path);

		std::reference_wrapper<int[4]> getEffects();
		std::reference_wrapper<int[4]> getEffectSkillIds();
		std::reference_wrapper<int[4]> getEffectAttributeIds();

		int getFirstIndexOfEffect(int effectId) const;
		bool hasEffect(int effectId) const;

	};
	static_assert(sizeof(Ingredient) == 0xE0, "TES3::Ingredient failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::Ingredient)
