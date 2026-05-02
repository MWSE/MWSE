#pragma once

#include "TES3Object.h"

#include "TES3IteratedList.h"

namespace TES3 {
	enum class DialogueType : unsigned char {
		Topic,
		Voice,
		Greeting,
		Persuasion,
		Journal
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

	struct DialogueName {
		Dialogue* dialogue; // 0x0
		const char* name; // 0x4

		static nonstd::span<DialogueName> getVoices();
		static nonstd::span<DialogueName> getGreetings();
		static nonstd::span<DialogueName> getResponses();
	};

	struct Dialogue : BaseObject {
		char * name; // 0x10
		DialogueType type; // 0x14
		IteratedList<DialogueInfo*> info; // 0x18
		int journalIndex; // 0x2C

		static constexpr auto OBJECT_TYPE = ObjectType::Dialogue;

		Dialogue() = delete;
		~Dialogue() = delete;

		//
		// Virtual table overrides.
		//

		const char * getObjectID() const;

		//
		// Other related this-call functions.
		//

		bool addToJournal(int index, MobileActor* actor);

		sol::optional<const char*> getQuestName() const;
		sol::optional<int> getJournalIndex() const;
		bool setJournalIndex(int index);
		bool setJournalIndexAndMarkModified(int index);

		enum class GetFilteredInfoContext : int {
			Unknown,
			ClickAnswer,
			ClickTopic,
			ClickTopicFallback,
			Greeting,
			HyperlinkParser,
			Persuasion,
			Script,
			ServiceBarter,
			ServiceEnchanting,
			ServiceRepair,
			ServiceSpellmaking,
			ServiceSpells,
			ServiceTraining,
			ServiceTravel,
			TopicPopulation,
			Voice,

			COUNT,
			Invalid = -1,
		};

		DialogueInfo* getJournalInfoForIndex(int index) const;
		DialogueInfo* getFilteredInfo(Actor* actor, Reference* reference, bool flag);
		DialogueInfo* getFilteredInfoWithContext(Actor* actor, Reference* reference, bool flag, GetFilteredInfoContext context);

		//
		// Custom functions.
		//

		std::string toJson();

		bool addToJournal_lua(sol::table params);
		DialogueInfo* getDeepFilteredInfo(Actor* actor, Reference* reference, bool flag, GetFilteredInfoContext context);
		DialogueInfo* getDeepFilteredInfo_lua(sol::table params);
		DialogueInfo* getJournalInfo(sol::optional<int> index) const;

		VoiceType getVoiceType() const;
		GreetingType getGreetingType() const;
		ResponseType getResponseType() const;

		//
		// Other related static functions.
		//

		static Dialogue* getDialogue(int type, int page);

		//
		// Cached values to help speed up dialogue filtering.
		//

		static std::optional<int> cachedActorDisposition;

	};
	static_assert(sizeof(Dialogue) == 0x30, "TES3::Dialogue failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::Dialogue)
