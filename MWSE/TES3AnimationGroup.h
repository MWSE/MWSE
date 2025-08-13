#pragma once

#include "TES3Object.h"

#include "NIDefines.h"

namespace TES3 {
	enum class AnimGroupID : unsigned char {
		Idle,
		Idle2,
		Idle3,
		Idle4,
		Idle5,
		Idle6,
		Idle7,
		Idle8,
		Idle9,
		Idlehh,
		Idle1h,
		Idle2c,
		Idle2w,
		IdleSwim,
		IdleSpell,
		IdleCrossbow,
		IdleSneak,
		IdleStorm,
		Torch,
		Hit1,
		Hit2,
		Hit3,
		Hit4,
		Hit5,
		SwimHit1,
		SwimHit2,
		SwimHit3,
		Death1,
		Death2,
		Death3,
		Death4,
		Death5,
		DeathKnockDown,
		DeathKnockOut,
		KnockDown,
		KnockOut,
		SwimDeath,
		SwimDeath2,
		SwimDeath3,
		SwimDeathKnockDown,
		SwimDeathKnockOut,
		SwimKnockOut,
		SwimKnockDown,
		SwimWalkForward,
		SwimWalkBack,
		SwimWalkLeft,
		SwimWalkRight,
		SwimRunForward,
		SwimRunBack,
		SwimRunLeft,
		SwimRunRight,
		SwimTurnLeft,
		SwimTurnRight,
		WalkForward,
		WalkBack,
		WalkLeft,
		WalkRight,
		TurnLeft,
		TurnRight,
		RunForward,
		RunBack,
		RunLeft,
		RunRight,
		SneakForward,
		SneakBack,
		SneakLeft,
		SneakRight,
		Jump,
		WalkForwardhh,
		WalkBackhh,
		WalkLefthh,
		WalkRighthh,
		TurnLefthh,
		TurnRighthh,
		RunForwardhh,
		RunBackhh,
		RunLefthh,
		RunRighthh,
		SneakForwardhh,
		SneakBackhh,
		SneakLefthh,
		SneakRighthh,
		Jumphh,
		WalkForward1h,
		WalkBack1h,
		WalkLeft1h,
		WalkRight1h,
		TurnLeft1h,
		TurnRight1h,
		RunForward1h,
		RunBack1h,
		RunLeft1h,
		RunRight1h,
		SneakForward1h,
		SneakBack1h,
		SneakLeft1h,
		SneakRight1h,
		Jump1h,
		WalkForward2c,
		WalkBack2c,
		WalkLeft2c,
		WalkRight2c,
		TurnLeft2c,
		TurnRight2c,
		RunForward2c,
		RunBack2c,
		RunLeft2c,
		RunRight2c,
		SneakForward2c,
		SneakBack2c,
		SneakLeft2c,
		SneakRight2c,
		Jump2c,
		WalkForward2w,
		WalkBack2w,
		WalkLeft2w,
		WalkRight2w,
		TurnLeft2w,
		TurnRight2w,
		RunForward2w,
		RunBack2w,
		RunLeft2w,
		RunRight2w,
		SneakForward2w,
		SneakBack2w,
		SneakLeft2w,
		SneakRight2w,
		Jump2w,
		SpellCast,
		SpellTurnLeft,
		SpellTurnRight,
		Attack1,
		Attack2,
		Attack3,
		SwimAttack1,
		SwimAttack2,
		SwimAttack3,
		HandToHand,
		Crossbow,
		BowAndArrow,
		ThrowWeapon,
		WeaponOneHand,
		WeaponTwoHand,
		WeaponTwoWide,
		Shield,
		PickProbe,
		InventoryHandToHand,
		InventoryWeaponOneHand,
		InventoryWeaponTwoHand,
		InventoryWeaponTwoWide,
		First = Idle,
		Last = InventoryWeaponTwoWide,
		NONE = 0xFF,
	};

	enum struct AnimGroupActionClass {
		NonLooping,
		Looping,
		CreatureAttack,
		ProjectileWeapon,
		Blocking,
		PickProbe,
		Casting,
		MeleeWeapon,
	};

	static constexpr auto DEBUG_ANIM_PARSER = false;

	struct AnimationGroup : BaseObject {
		struct LuaEvent {
			static constexpr unsigned int eventTag = 0x4541554C; // "LUAE"

			unsigned int tag;
			int refCount;
			std::string id;
			std::string param;

			LuaEvent(const std::string_view& _id, const std::string_view& _param);
			~LuaEvent();
			std::string toString() const;
			static LuaEvent* toEvent(Sound* sound);
		};
		struct SoundGenKey {
			int startFrame; // 0x0
			float startTime; // 0x4
			unsigned char volume; // 0x8
			float pitch; // 0xC
			union { // 0x10
				Sound* sound;
				LuaEvent* event;
			};

			SoundGenKey() = delete;
			~SoundGenKey() = delete;

			bool hasLuaEvent() const;
			void freeLuaEvent();

			Sound* getSound() const;
			void setSound(Sound* sound);
			LuaEvent* getLuaEvent() const;
			void setLuaEvent(LuaEvent* event);
		};

		AnimGroupID groupId; // 0x10
		short patchedRootTravelSpeed; // 0x12
		unsigned int actionCount; // 0x14
		int* actionFrames; // 0x18
		float* actionTimings; // 0x1C
		AnimationGroup* nextGroup; // 0x20
		unsigned int soundGenCount; // 0x24
		SoundGenKey* soundGenKeys; // 0x28

		AnimationGroup() = delete;
		~AnimationGroup() = delete;

		//
		// Other related this-call functions.
		//

		AnimationGroup* ctor(int animGroupId);
		void dtor();

		void calcNoteTimes();
		void setSoundGenCount(unsigned int newCount);
		void setSoundGenVolume(unsigned int index, float volume);
		void setSoundGenPitch(unsigned int index, float volume);

		//
		// Custom functions.
		//

		AnimationGroup* findGroup(AnimGroupID groupId);
		static AnimGroupActionClass getActionClass(AnimGroupID groupId);

		nonstd::span<int> getActionFrames();
		nonstd::span<float> getActionTimings();
		nonstd::span<SoundGenKey> getSoundGenKeys();
	};
	static_assert(sizeof(AnimationGroup) == 0x2C, "TES3::AnimationGroup failed size validation");
	static_assert(sizeof(AnimationGroup::SoundGenKey) == 0x14, "TES3::AnimationGroup::SoundGenKey failed size validation");

	struct KeyframeDefinitionVanilla {
		const char* filename; // 0x0
		NI::Sequence* sequences[3]; // 0x4
		AnimationGroup* animationGroups; // 0x10
		unsigned short groupCount; // 0x14
		unsigned short refCount; // 0x16

		KeyframeDefinitionVanilla() = delete;
		~KeyframeDefinitionVanilla() = delete;
	};
	static_assert(sizeof(KeyframeDefinitionVanilla) == 0x18, "TES3::KeyframeDefinition failed size validation");

	struct KeyframeDefinition : KeyframeDefinitionVanilla {
		// These members need to be explicitly constructed in ctor().
		std::unordered_map<std::string, AnimationGroup*> namedGroups;

		KeyframeDefinition() = delete;
		~KeyframeDefinition() = delete;

		KeyframeDefinition* ctor(const char* nifPath, const char* name);
		void dtor();

		static std::string toCanonicalName(std::string_view name);
		static int __cdecl parseSeqTextKeysToAnimGroups(NI::Sequence* sequence, const char* meshPath, KeyframeDefinition* kfData);
	};

	struct ParsedTextKeyEntry {
		float time;
		std::string key;
		std::string value;
	};
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::AnimationGroup)
