#include "CSDialogueInfo.h"

#include "CSActor.h"
#include "CSCell.h"
#include "CSDataHandler.h"
#include "CSDialogue.h"
#include "CSRecordHandler.h"

#include "StringUtil.h"

namespace se::cs {
	Dialogue* DialogueInfo::getDialogue() const {
		const auto& dialogues = *DataHandler::get()->recordHandler->dialogues;
		for (const auto& dialogue : dialogues) {
			for (const auto& info : dialogue->infos) {
				if (info == this) {
					return dialogue;
				}
			}
		}
		return nullptr;
	}

	bool DialogueInfo::filter(Object* actor, Reference* reference, int source, Dialogue* dialogue) const {
		const auto TES3_DialogueInfo_filter = reinterpret_cast<bool(__thiscall*)(const DialogueInfo*, Object*, Reference*, int, Dialogue*)>(0x402BB7);
		return TES3_DialogueInfo_filter(this, actor, reference, source, dialogue);
	}

	bool DialogueInfo::search(const std::string_view& needle, const SearchSettings& settings, std::regex* regex) const {
		if (loadLinkNodes && loadLinkNodes->name && string::complex_contains(loadLinkNodes->name, needle, settings, regex)) {
			return true;
		}

		if (text && string::complex_contains(text, needle, settings, regex)) {
			return true;
		}

		if (resultsText && string::complex_contains(resultsText, needle, settings, regex)) {
			return true;
		}

		if (filterCell && string::complex_contains(filterCell->getObjectID(), needle, settings, regex)) {
			return true;
		}

		if (filterActor && string::complex_contains(filterActor->getObjectID(), needle, settings, regex)) {
			return true;
		}

		for (const auto& condition : conditions) {
			switch (condition.type) {
			case Condition::Type::TypeGlobal:
			case Condition::Type::TypeItem:
			case Condition::Type::TypeDead:
			case Condition::Type::TypeNotID:
			case Condition::Type::TypeNotFaction:
			case Condition::Type::TypeNotClass:
			case Condition::Type::TypeNotRace:
				if (string::complex_contains(condition.compareValue.object->getObjectID(), needle, settings, regex)) {
					return true;
				}
				break;
			case Condition::Type::TypeJournal:
				if (condition.compareValue.dialogue->id && string::complex_contains(condition.compareValue.dialogue->id, needle, settings, regex)) {
					return true;
				}
				break;
			}

		}

		return false;
	}
}
