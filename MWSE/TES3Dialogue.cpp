#include "TES3Dialogue.h"

#include "LuaManager.h"

#include "LuaJournalEvent.h"
#include "LuaDialogueFilteredEvent.h"

#include "LuaUtil.h"

#include "TES3Actor.h"
#include "TES3DialogueInfo.h"
#include "TES3MobilePlayer.h"
#include "TES3Reference.h"
#include "TES3WorldController.h"

#include "StringUtil.h"

namespace TES3 {
	namespace {
		using DialogueInfoLoadIdMap = std::unordered_map<Dialogue*, std::unordered_map<std::string, DialogueInfo*>>;
		using DialogueInfoLoadIdReverseMap = std::unordered_map<DialogueInfo*, std::pair<Dialogue*, std::string>>;

		static DialogueInfoLoadIdMap g_DialogueInfoLoadIdMap;
		static DialogueInfoLoadIdReverseMap g_DialogueInfoLoadIdReverseMap;
	}

	//
	// DialogueName
	//

	std::span<DialogueName> DialogueName::getVoices() {
		return std::span(reinterpret_cast<DialogueName*>(0x793248), (size_t)VoiceType::COUNT);
	}

	std::span<DialogueName> DialogueName::getGreetings() {
		return std::span(reinterpret_cast<DialogueName*>(0x793280), (size_t)GreetingType::COUNT);
	}

	std::span<DialogueName> DialogueName::getResponses() {
		return std::span(reinterpret_cast<DialogueName*>(0x7932D0), (size_t)ResponseType::COUNT);
	}

	//
	// Dialogue
	//

	const char* Dialogue::getObjectID() const {
		return name;
	}

	const auto TES3_Dialogue_journalAdd = reinterpret_cast<bool(__thiscall*)(Dialogue*, int, MobileActor*)>(0x4B2F80);
	bool Dialogue::addToJournal(int index, MobileActor* actor) {
		if (type != DialogueType::Journal) {
			return false;
		}

		// Store our old index so we can refer to it later.
		int oldIndex = journalIndex;

		// Call the original function.
		bool added = TES3_Dialogue_journalAdd(this, index, actor);

		// If the journal index changed, raise an event that it was modified.
		if (journalIndex > oldIndex && mwse::lua::event::JournalEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::JournalEvent(this, oldIndex, index));
		}

		return added;
	}

	sol::optional<const char*> Dialogue::getQuestName() const {
		if (type == DialogueType::Journal) {
			for (auto i : info) {
				if (i->isQuestName().value_or(false)) {
					return i->getText();
				}
			}
		}
		return {};
	}

	sol::optional<int> Dialogue::getJournalIndex() const {
		if (type != DialogueType::Journal) {
			return {};
		}

		return journalIndex;
	}

	bool Dialogue::setJournalIndex(int index) {
		// Only valid for journal entries.
		if (type != DialogueType::Journal) {
			return false;
		}

		// Only run if the index changes.
		if (journalIndex == index) {
			return false;
		}

		// Store our old index so we can refer to it later.
		int oldIndex = journalIndex;

		// Update the journal index, as the original function would do.
		journalIndex = index;

		// Raise a modified event.
		if (journalIndex > oldIndex && mwse::lua::event::JournalEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::JournalEvent(this, oldIndex, index));
		}

		return true;
	}

	bool Dialogue::setJournalIndexAndMarkModified(int index) {
		if (setJournalIndex(index)) {
			setObjectModified(true);
			return true;
		}
		return false;
	}

	DialogueInfo* Dialogue::getJournalInfoForIndex(int index) const {
		if (type == DialogueType::Journal) {
			for (auto i : info) {
				if (i->journalIndex == index) {
					return i;
				}
			}
		}

		return nullptr;
	}

	void Dialogue::clearInfoLoadIDCache() {
		const auto dialogue = const_cast<Dialogue*>(this);
		const auto dialogueIt = g_DialogueInfoLoadIdMap.find(dialogue);
		if (dialogueIt == g_DialogueInfoLoadIdMap.end()) {
			return;
		}

		for (const auto& [loadID, info] : dialogueIt->second) {
			const auto reverseIt = g_DialogueInfoLoadIdReverseMap.find(info);
			if (reverseIt != g_DialogueInfoLoadIdReverseMap.end() && reverseIt->second.first == dialogue && reverseIt->second.second == loadID) {
				g_DialogueInfoLoadIdReverseMap.erase(reverseIt);
			}
		}

		g_DialogueInfoLoadIdMap.erase(dialogueIt);
	}

	DialogueInfo* Dialogue::findInfoByLoadID(const char* infoID) const {
		if (infoID == nullptr || infoID[0] == '\0') {
			return nullptr;
		}

		auto normalizedInfoID = std::string(infoID);

		const auto dialogueIt = g_DialogueInfoLoadIdMap.find(const_cast<Dialogue*>(this));
		if (dialogueIt != g_DialogueInfoLoadIdMap.end()) {
			const auto infoIt = dialogueIt->second.find(normalizedInfoID);
			if (infoIt != dialogueIt->second.end()) {
				return infoIt->second;
			}
		}

		return nullptr;
	}

	void Dialogue::cacheInfoByLoadID(DialogueInfo* info) {
		if (info == nullptr) {
			return;
		}

		if (info->loadLinkNode == nullptr || info->loadLinkNode->name == nullptr) {
			return;
		}

		auto loadID = std::string(info->loadLinkNode->name);
		const auto dialogue = this;

		const auto reverseIt = g_DialogueInfoLoadIdReverseMap.find(info);
		if (reverseIt != g_DialogueInfoLoadIdReverseMap.end()) {
			const auto previousDialogue = reverseIt->second.first;
			const auto previousLoadID = reverseIt->second.second;

			const auto previousDialogueIt = g_DialogueInfoLoadIdMap.find(previousDialogue);
			if (previousDialogueIt != g_DialogueInfoLoadIdMap.end()) {
				previousDialogueIt->second.erase(previousLoadID);
				if (previousDialogueIt->second.empty()) {
					g_DialogueInfoLoadIdMap.erase(previousDialogueIt);
				}
			}
		}

		g_DialogueInfoLoadIdMap[dialogue][loadID] = info;
		g_DialogueInfoLoadIdReverseMap[info] = { dialogue, std::move(loadID) };
	}

	void DialogueInfo::removeFromLoadIDCache() {
		const auto reverseIt = g_DialogueInfoLoadIdReverseMap.find(this);
		if (reverseIt == g_DialogueInfoLoadIdReverseMap.end()) {
			return;
		}

		const auto dialogue = reverseIt->second.first;
		const auto loadID = reverseIt->second.second;
		const auto dialogueIt = g_DialogueInfoLoadIdMap.find(dialogue);
		if (dialogueIt != g_DialogueInfoLoadIdMap.end()) {
			dialogueIt->second.erase(loadID);
			if (dialogueIt->second.empty()) {
				g_DialogueInfoLoadIdMap.erase(dialogueIt);
			}
		}

		g_DialogueInfoLoadIdReverseMap.erase(reverseIt);
	}

	const auto TES3_Dialogue_getFilteredInfo = reinterpret_cast<DialogueInfo * (__thiscall*)(Dialogue*, Actor*, Reference*, bool)>(0x4B29E0);
	DialogueInfo* Dialogue::getFilteredInfo(Actor* actor, Reference* reference, bool flag) {
		// Cache some heavier values.
		if (actor->objectType == ObjectType::NPC) {
			auto mobile = static_cast<MobileNPC*>(reference->getAttachedMobileActor());
			if (mobile) {
				cachedActorDisposition = mobile->getDisposition();
			}
		}

		// Call original code.
		auto result = TES3_Dialogue_getFilteredInfo(this, actor, reference, flag);

		// Clean any cached values.
		cachedActorDisposition.reset();

		return result;
	}

	DialogueInfo* Dialogue::getFilteredInfoWithContext(Actor* actor, Reference* reference, bool flag, GetFilteredInfoContext context) {
		auto info = getFilteredInfo(actor, reference, flag);
		if (info == nullptr) {
			return nullptr;
		}

		if (mwse::lua::event::DialogueFilteredEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::DialogueFilteredEvent(this, info, actor, reference, context));
		}

		return info;
	}

	std::string Dialogue::toJson() {
		std::ostringstream ss;
		ss << "\"tes3dialogue:" << name << "\"";
		return std::move(ss.str());
	}

	bool Dialogue::addToJournal_lua(sol::table params) {
		int index = mwse::lua::getOptionalParam<int>(params, "index", 0);
		TES3::MobileActor* actor = mwse::lua::getOptionalParamMobileActor(params, "actor");
		if (actor == nullptr) {
			actor = TES3::WorldController::get()->getMobilePlayer();
		}
		return addToJournal(index, actor);
	}

	DialogueInfo* Dialogue::getDeepFilteredInfo(Actor* actor, Reference* reference, bool flag, GetFilteredInfoContext context) {
		auto info = getFilteredInfoWithContext(actor, reference, flag, context);
		if (info == nullptr) {
			auto dialogue = getDialogue(3, 0);
			if (dialogue) {
				info = dialogue->getFilteredInfoWithContext(actor, reference, flag, context);
			}
		}
		return info;
	}

	DialogueInfo* Dialogue::getDeepFilteredInfo_lua(sol::table params) {
		using namespace mwse::lua;
		using namespace mwse::lua::event;
		const auto mobile = getOptionalParamMobileActor(params, "actor");
		const auto context = (GetFilteredInfoContext)getOptionalParam(params, "context", (int)GetFilteredInfoContext::Script);
		return getDeepFilteredInfo(reinterpret_cast<TES3::Actor*>(mobile->reference->baseObject), mobile->reference, true, context);
	}

	DialogueInfo* Dialogue::getJournalInfo(sol::optional<int> index) const {
		return getJournalInfoForIndex(index.value_or(journalIndex));
	}

	VoiceType Dialogue::getVoiceType() const {
		const auto voices = DialogueName::getVoices();
		for (size_t i = 0; i < voices.size(); ++i) {
			if (voices[i].dialogue == this) {
				return (VoiceType)i;
			}
		}
		return VoiceType::Invalid;
	}

	GreetingType Dialogue::getGreetingType() const {
		const auto greetings = DialogueName::getGreetings();
		for (size_t i = 0; i < greetings.size(); ++i) {
			if (greetings[i].dialogue == this) {
				return (GreetingType)i;
			}
		}
		return GreetingType::Invalid;
	}

	ResponseType Dialogue::getResponseType() const {
		const auto responses = DialogueName::getResponses();
		for (size_t i = 0; i < responses.size(); ++i) {
			if (responses[i].dialogue == this) {
				return (ResponseType)i;
			}
		}
		return ResponseType::Invalid;
	}

	const char* Dialogue::getFilterTypeName() const {
		if (type > DialogueType::MAX_VALUE) {
			return nullptr;
		}
		const auto TES3_DialogueFilterTypes = reinterpret_cast<const char**>(0x793200);
		return TES3_DialogueFilterTypes[DWORD(type)];
	}

	const auto TES3_getDialogue = reinterpret_cast<Dialogue* (__cdecl*)(int, int)>(0x4B2C00);
	Dialogue* Dialogue::getDialogue(int type, int page) {
		return TES3_getDialogue(type, page);
	}

	std::optional<int> Dialogue::cachedActorDisposition = {};
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::Dialogue)
