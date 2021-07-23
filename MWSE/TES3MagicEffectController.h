#pragma once

#include "TES3Defines.h"

#include "TES3MagicEffect.h"
#include "TES3MobileObject.h"

namespace TES3 {
	constexpr int MAX_EFFECT_COUNT = SHRT_MAX;
	constexpr int EFFECT_ID_INVALID = MAX_EFFECT_COUNT - 1;

	namespace MagicEffectAttribute {
		typedef unsigned int value_type;

		enum EffectAttribute : value_type {
			AttackBonus = 0,
			Sanctuary = 1,
			ResistMagicka = 2,
			ResistFire = 3,
			ResistFrost = 4,
			ResistShock = 5,
			ResistCommonDisease = 6,
			ResistBlightDisease = 7,
			ResistCorprus = 8,
			ResistPoison = 9,
			ResistParalysis = 10,
			Chameleon = 11,
			ResistNormalWeapons = 12,
			WaterBreathing = 13,
			WaterWalking = 14,
			SwiftSwim = 15,
			Jump = 16,
			Levitate = 17,
			Shield = 18,
			Sound = 19,
			Silence = 20,
			Blind = 21,
			Paralyze = 22,
			Invisibility = 23,
			Fight = 24,
			Flee = 25,
			Hello = 26,
			Alarm = 27,
			NonResistable = 28,
		};
	}

	class MagicEffectController {
	public:
		MagicEffectController();
		~MagicEffectController();

		// MagicEffect object functions.
		MagicEffect* getEffectObject(int id);
		void addEffectObject(MagicEffect* effect);
		bool getEffectExists(int id);

		const char * getEffectName(int id);

		// Flag manipulation.
		static unsigned int getEffectFlags(int id);
		static void setEffectFlags(int id, unsigned int flags);
		static bool getEffectFlag(int id, EffectFlag::FlagBit flag);
		static void setEffectFlag(int id, EffectFlag::FlagBit flag, bool value);

		static void InstallCustomMagicEffectController();
		void spellProjectileHit(MagicSourceInstance * instance, MobileObject::Collision * collision);

		// Implemented.
		std::unordered_map<int, MagicEffect*> effectObjects;
		std::unordered_map<int, sol::protected_function> effectLuaTickFunctions;
		std::unordered_map<int, sol::protected_function> effectLuaCollisionFunctions;
		std::unordered_map<int, std::string> effectCustomNames;
		static unsigned int effectNameGMSTs[MAX_EFFECT_COUNT];
		static unsigned int effectFlags[MAX_EFFECT_COUNT];
		static unsigned int effectCounters[MAX_EFFECT_COUNT][5];
	};
}
