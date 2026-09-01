#pragma once

namespace TES3 {
	enum class DialogueType : unsigned char {
		Topic,
		Voice,
		Greeting,
		Persuasion,
		Journal,

		MAX_VALUE = Journal,
	};

	enum class VoiceType : int {
		Hello,
		Idle,
		Intruder,
		Thief,
		Hit,
		Attack,
		Flee,

		COUNT,
		Invalid = -1,
	};

	enum class GreetingType : int {
		Greeting0,
		Greeting1,
		Greeting2,
		Greeting3,
		Greeting4,
		Greeting5,
		Greeting6,
		Greeting7,
		Greeting8,
		Greeting9,

		COUNT,
		Invalid = -1,
	};

	enum class ResponseType : int {
		InfoRefusal,
		AdmireSuccess,
		AdmireFail,
		IntimidateSuccess,
		IntimidateFail,
		TauntSuccess,
		TauntFail,
		ServiceRefusal,
		BribeSuccess,
		BribeFail,

		COUNT,
		Invalid = -1,
	};

	enum class DialogueInfoFilterType {
		Actor,
		Race,
		Class,
		NPCFaction,
		Cell,
		PCFaction,
		SoundPath,
		Conditional0,
		Conditional1,
		Conditional2,
		Conditional3,
		Conditional4,
		Conditional5,
	};
	constexpr auto DialogueInfoConditionalCount = 6u;

	namespace ObjectFlag {
		typedef unsigned int value_type;

		enum DialogueInfoFlag : value_type {
			QuestName = 0x40,
			QuestStarted = 0x80,
			QuestFinished = 0x100,
			QuestRestart = 0x200,
			HasResultText = 0x2000,
		};

		enum DialogueInfoFlagBit {
			QuestNameBit = 6,
			QuestStartedBit = 7,
			QuestFinishedBit = 8,
			QuestRestartBit = 9,
			HasResultTextBit = 13,
		};
	}

	enum class DialogueConditionalType : unsigned char {
		NoCondition,
		Function,
		GlobalVar,
		LocalVar,
		JournalIndex,
		ItemCount,
		DeadActor,
		NotID,
		NotFaction,
		NotClass,
		NotRace,
		NotCell,
		NotLocal,
	};

	enum class DialogueConditionalConstantType : char {
		None = '\0',
		Class = 'C',
		DeadActor = 'D',
		Faction = 'F',
		Item = 'I',
		Journal = 'J',
		Cell = 'L',
		Race = 'R',
		NotID = 'X',
		VariableFloat = 'f',
		VariableInt = 'i',
		VariableShort = 's',
	};

	enum class DialogueConditionalComparator : unsigned char {
		Equal = 0,
		NotEqual,
		Greater,
		GreaterEqual,
		Less,
		LessEqual,
	};

	enum class DialogueConditionalFunction : int {
		ReactionLow = 0,
		ReactionHigh,
		RankRequirement,
		Reputation,
		HealthPercent,
		PCReputation,
		PCLevel,
		PCHealthPercent,
		PCMagicka,
		PCFatigue,
		PCStrength,
		PCBlock,
		PCArmorer,
		PCMediumArmor,
		PCHeavyArmor,
		PCBluntWeapon,
		PCLongBlade,
		PCAxe,
		PCSpear,
		PCAthletics,
		PCEnchant,
		PCDestruction,
		PCAlteration,
		PCIllusion,
		PCConjuration,
		PCMysticism,
		PCRestoration,
		PCAlchemy,
		PCUnarmored,
		PCSecurity,
		PCSneak,
		PCAcrobatics,
		PCLightArmor,
		PCShortBlade,
		PCMarksman,
		PCMercantile,
		PCSpeechcraft,
		PCHandToHand,
		PCSex,
		PCExpelled,
		PCCommonDisease,
		PCBlightDisease,
		PCClothingModifier,
		PCCrimeLevel,
		SameSex,
		SameRace,
		SameFaction,
		FactionRankDifference,
		Detected,
		Alarmed,
		Choice,
		PCIntelligence,
		PCWillpower,
		PCAgility,
		PCSpeed,
		PCEndurance,
		PCPersonality,
		PCLuck,
		PCCorprus,
		Weather,
		PCVampire,
		Level,
		Attacked,
		TalkedToPC,
		PCHealth,
		CreatureTarget,
		FriendHit,
		Fight,
		Hello,
		Alarm,
		Flee,
		ShouldAttack,
		Werewolf,
		PCWerewolfKills,
	};
}
