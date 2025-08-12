#include "TES3DialogueInfo.h"

#include "LuaManager.h"
#include "LuaInfoFilterEvent.h"

#include "TES3Actor.h"
#include "TES3Cell.h"
#include "TES3Class.h"
#include "TES3DataHandler.h"
#include "TES3DialogueFilterContext.h"
#include "TES3Faction.h"
#include "TES3MobileCreature.h"
#include "TES3MobilePlayer.h"
#include "TES3NPC.h"
#include "TES3Race.h"

#include "BitUtil.h"
#include "MemoryUtil.h"

#include "MWSEConfig.h"

namespace TES3 {
	const auto TES3_DialogueInfo_getText = reinterpret_cast<const char* (__thiscall*)(const DialogueInfo*)>(0x4B1B80);
	const auto TES3_DialogueInfo_loadId = reinterpret_cast<bool(__thiscall*)(DialogueInfo*)>(0x4B1A10);
	const auto TES3_DialogueInfo_unloadId = reinterpret_cast<void(__thiscall*)(DialogueInfo*)>(0x4AF3A0);

	const char* DialogueInfo::getText() const {
		return TES3_DialogueInfo_getText(this);
	}

	bool DialogueInfo::loadId() {
		return TES3_DialogueInfo_loadId(this);
	}

	void DialogueInfo::unloadId() {
		TES3_DialogueInfo_unloadId(this);
	}

	bool DialogueInfo::filterVanillaReplacer(Object * speaker, Reference * reference, FilterSource source, Dialogue * dialogue) const {
		if (getDeleted()) {
			return false;
		}

		DialogueFilterContext context(speaker, reference, source, dialogue, this);

		// Check for actor condition.
		if (context.filterActor && context.speakerBaseActor != context.filterActor) {
			return false;
		}

		// Creatures have no valid conditions other than their ID check.
		if (context.speakerBaseActor->objectType == ObjectType::Creature && context.filterActor != context.speakerBaseActor) {
			return false;
		}

		// Check for cell condition.
		if (context.filterCell) {
			auto playerCell = DataHandler::get()->currentCell;
			if (!playerCell) {
				return false;
			}

			std::string_view cellId = context.filterCell->getObjectID();
			if (_strnicmp(playerCell->getObjectID(), cellId.data(), cellId.size()) != 0) {
				return false;
			}
		}

		// Check for player faction membership/rank.
		if (context.filterPlayerFaction) {
			if (!context.filterPlayerFaction->getPlayerJoined()) {
				return false;
			}
			if (pcRank != -1 && pcRank > context.filterPlayerFaction->getEffectivePlayerRank()) {
				return false;
			}
		}

		// NPC only conditions.
		if (context.speakerBaseActor->objectType == ObjectType::NPC) {
			// Check for race condition.
			if (context.filterRace && context.speakerBaseActor->getRace() != context.filterRace) {
				return false;
			}

			// Check for sex condition.
			const auto npcIsFemale = (npcSex == 1);
			if (npcSex != -1 && context.speakerBaseActor->isFemale() != npcIsFemale) {
				return false;
			}

			// Check for class condition.
			if (context.filterClass && context.speakerBaseActor->getClass() != context.filterClass) {
				return false;
			}

			// Check for faction condition, with no-faction special case.
			const auto speakerFaction = context.speakerBaseActor->getFaction();
			if (context.filterFaction) {
				if (reinterpret_cast<int>(context.filterFaction) == -1) {
					if (speakerFaction) {
						return false;
					}
				}
				else if (speakerFaction != context.filterFaction) {
					return false;
				}
			}

			// Check for player faction membership/rank in speaker's faction.
			if (pcRank != -1 && !context.filterPlayerFaction) {
				if (speakerFaction == nullptr || pcRank > speakerFaction->getEffectivePlayerRank()) {
					return false;
				}
			}

			// Check for faction rank condition.
			if (npcRank != -1 && (speakerFaction == nullptr || context.speakerBaseActor->getFactionRank() < npcRank)) {
				return false;
			}

			// Check for disposition.
			if (context.speakerMobile) {
				const auto mobileNPC = static_cast<MobileNPC*>(context.speakerMobile);
				// Service refusal disposition checks are flipped for some reason?
				if (dialogue->getResponseType() == ResponseType::ServiceRefusal) {
					if (source != FilterSource::None && disposition && mobileNPC->getDisposition() >= disposition) {
						return false;
					}
				}
				else if (source != FilterSource::None) {
					if (mobileNPC->getDisposition() < disposition) {
						return false;
					}
				}
			}
		}

		// Check for conditionals.
		for (auto i = 0; i < DialogueInfoConditionalCount; ++i) {
			auto& conditionalContext = context.conditionalContexts[i];
			if (conditionalContext.conditional == nullptr) {
				continue;
			}

			// Load the necessary data to compare against.
			conditionalContext.load();

			// Were we given an explicit result?
			if (conditionalContext.resultOverride) {
				if (conditionalContext.resultOverride.value()) {
					continue;
				}
				else {
					return false;
				}
			}

			// Otherwise compare the value we got back.
			if (conditionalContext.compareValue) {
				const auto compareValue = conditionalContext.compareValue.value();
				const auto compareAgainst = conditionalContext.conditional->compareValue;

				bool conditionResult = false;
				switch (conditionalContext.compareOperator) {
				case DialogueConditionalComparator::Equal:
					conditionResult = compareValue == compareAgainst;
					break;
				case DialogueConditionalComparator::NotEqual:
					conditionResult = compareValue != compareAgainst;
					break;
				case DialogueConditionalComparator::Greater:
					conditionResult = compareValue > compareAgainst;
					break;
				case DialogueConditionalComparator::GreaterEqual:
					conditionResult = compareValue >= compareAgainst;
					break;
				case DialogueConditionalComparator::Less:
					conditionResult = compareValue < compareAgainst;
					break;
				case DialogueConditionalComparator::LessEqual:
					conditionResult = compareValue <= compareAgainst;
					break;
				default:
#if _DEBUG
					throw std::runtime_error("Invalid comparison operator specified.");
#else
					return false;
#endif
				}

				if (conditionResult) {
					continue;
				} else {
					return false;
				}
			}

			// If we've fallen to this point then we have no input and some error condition is reached. Filter out.
			return false;
		}

		// If nothing stopped us from getting to this point, congrats, we can use this info.
		return true;
	}

	const auto TES3_DialogueInfo_filter = reinterpret_cast<bool(__thiscall*)(const DialogueInfo*, Object*, Reference*, DialogueInfo::FilterSource, Dialogue*)>(0x4B0190);
	bool DialogueInfo::filter(Object* speaker, Reference* reference, FilterSource source, Dialogue* dialogue) {
		auto result = false;
		if (mwse::Configuration::ReplaceDialogueFiltering) {
			result = filterVanillaReplacer(speaker, reference, source, dialogue);
		}
		else {
			result = TES3_DialogueInfo_filter(this, speaker, reference, source, dialogue);
		}

		if (mwse::lua::event::InfoFilterEvent::getEventEnabled()) {
			auto& luaManager = mwse::lua::LuaManager::getInstance();
			const auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::table eventData = stateHandle.triggerEvent(new mwse::lua::event::InfoFilterEvent(this, speaker, reference, (int)source, dialogue, result));
			sol::object passes = eventData["passes"];
			if (passes.is<bool>()) {
				result = passes.as<bool>();
			}
		}

		return result;
	}

	const auto TES3_DialogueInfo_runScript = reinterpret_cast<void(__thiscall*)(DialogueInfo*, Reference*)>(0x4B1E40);
	void DialogueInfo::runScript(Reference * reference) {
		TES3_DialogueInfo_runScript(this, reference);
	}

	auto& TES3_DialogueInfo_lastLoadedText = *reinterpret_cast<char**>(0x7CA5AC);
	char* DialogueInfo::getLastLoadedText() {
		return TES3_DialogueInfo_lastLoadedText;
	}

	void DialogueInfo::setLastLoadedText(const char* text) {
		if (TES3_DialogueInfo_lastLoadedText) {
			mwse::tes3::_delete(TES3_DialogueInfo_lastLoadedText);
			TES3_DialogueInfo_lastLoadedText = nullptr;
		}

		if (text) {
			auto length = strlen(text);
			TES3_DialogueInfo_lastLoadedText = reinterpret_cast<char*>(mwse::tes3::_new(length + 1));
			TES3_DialogueInfo_lastLoadedText[length] = '\0';
			strcpy(TES3_DialogueInfo_lastLoadedText, text);
		}
	}

	auto& TES3_DialogueInfo_lastLoadedScript = *reinterpret_cast<char**>(0x7CA5A8);
	char* DialogueInfo::getLastLoadedScript() {
		return TES3_DialogueInfo_lastLoadedScript;
	}

	void DialogueInfo::setLastLoadedScript(const char* text) {
		if (TES3_DialogueInfo_lastLoadedScript) {
			mwse::tes3::_delete(TES3_DialogueInfo_lastLoadedScript);
			TES3_DialogueInfo_lastLoadedScript = nullptr;
		}

		if (text) {
			auto length = strlen(text);
			TES3_DialogueInfo_lastLoadedScript = reinterpret_cast<char*>(mwse::tes3::_new(length + 1));
			TES3_DialogueInfo_lastLoadedScript[length] = '\0';
			strcpy(TES3_DialogueInfo_lastLoadedScript, text);
		}
	}

	const char* DialogueInfo::getSoundPath() {
		for (auto node = conditions; node; node = node->next) {
			if (node->tag == DialogueInfoFilterType::SoundPath) {
				return node->soundPath;
			}
		}
		return nullptr;
	}

	sol::optional<std::string> DialogueInfo::getID() {
		// If we're already loaded for some reason, don't reload.
		if (loadLinkNode) {
			return { loadLinkNode->name };
		}

		// Hit the IO to find it...
		if (loadId()) {
			std::string id = loadLinkNode->name;
			unloadId();
			return std::move(id);
		}

		return {};
	}

	sol::optional<int> DialogueInfo::getJournalIndex_lua() const {
		if (type == DialogueType::Journal) {
			return journalIndex;
		}
		return {};
	}

	void DialogueInfo::setJournalIndex_lua(int value) {
		if (type == DialogueType::Journal) {
			journalIndex = value;
		}
	}

	BaseObject* DialogueInfo::getFilterObject(TES3::DialogueInfoFilterType type) {
		TES3::DialogueInfoFilterNode* node;

		for (node = conditions; node; node = node->next) {
			if (node->tag == type) {
				break;
			}
		}
		if (!node) {
			return nullptr;
		}

		switch (type) {
		case TES3::DialogueInfoFilterType::Actor:
		case TES3::DialogueInfoFilterType::Race:
		case TES3::DialogueInfoFilterType::Class:
		case TES3::DialogueInfoFilterType::NPCFaction:
		case TES3::DialogueInfoFilterType::Cell:
		case TES3::DialogueInfoFilterType::PCFaction:
			return node->object;
		default:
			return nullptr;
		}
	}

	Actor* DialogueInfo::getFilterActor() {
		return static_cast<Actor*>(getFilterObject(TES3::DialogueInfoFilterType::Actor));
	}

	Race* DialogueInfo::getFilterNPCRace() {
		return static_cast<Race*>(getFilterObject(TES3::DialogueInfoFilterType::Race));
	}

	Class* DialogueInfo::getFilterNPCClass() {
		return static_cast<Class*>(getFilterObject(TES3::DialogueInfoFilterType::Class));
	}

	Faction* DialogueInfo::getFilterNPCFaction() {
		return static_cast<Faction*>(getFilterObject(TES3::DialogueInfoFilterType::NPCFaction));
	}

	Cell* DialogueInfo::getFilterNPCCell() {
		return static_cast<Cell*>(getFilterObject(TES3::DialogueInfoFilterType::Cell));
	}

	Faction* DialogueInfo::getFilterPCFaction() {
		return static_cast<Faction*>(getFilterObject(TES3::DialogueInfoFilterType::PCFaction));
	}

	sol::optional<bool> DialogueInfo::isQuestName() const {
		if (type != DialogueType::Journal) {
			return {};
		}

		return BIT_TEST(objectFlags, TES3::ObjectFlag::QuestNameBit);
	}

	sol::optional<bool> DialogueInfo::isQuestFinished() const {
		if (type != DialogueType::Journal) {
			return {};
		}

		return BIT_TEST(objectFlags, TES3::ObjectFlag::QuestFinishedBit);
	}

	sol::optional<bool> DialogueInfo::isQuestRestart() const {
		if (type != DialogueType::Journal) {
			return {};
		}

		return BIT_TEST(objectFlags, TES3::ObjectFlag::QuestRestartBit);
	}

	Dialogue* DialogueInfo::findDialogue() const {
		for (const auto& dialogue : *TES3::DataHandler::get()->nonDynamicData->dialogues) {
			if (dialogue->type == type) {
				for (const auto& info : dialogue->info) {
					if (info == this) {
						return dialogue;
					}
				}
			}
		}
		return nullptr;
	}

	std::string DialogueInfo::toJson() {
		std::ostringstream ss;
		ss << "\"tes3dialogueInfo:" << getID().value_or("<invalid>") << "\"";
		return std::move(ss.str());
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::DialogueInfo)
