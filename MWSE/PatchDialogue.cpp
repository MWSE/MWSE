#include "PatchDialogue.h"

#include "MemoryUtil.h"
#include "StringUtil.h"
#include "TES3Util.h"

#include "TES3DataHandler.h"
#include "TES3Dialogue.h"
#include "TES3DialogueInfo.h"
#include "TES3MobileNPC.h"
#include "TES3UIManager.h"
#include "TES3WorldController.h"

namespace mwse::patch::dialogue {
	//
	// Patch: Cache values between dialogue filters.
	// 
	// Current additional caching:
	//  * speaker's dialogue
	//

	int __fastcall PatchDialogueFilterCacheGetDisposition(TES3::MobileNPC* npc) {
		if (TES3::Dialogue::cachedActorDisposition) {
			return TES3::Dialogue::cachedActorDisposition.value();
		}

		// Always allow the default behavior if something we hit a weird context.
		return npc->getDisposition();
	}

	//
	// Patch: Improve performance of initial game load.
	//

	static void __cdecl PatchDialogueSorting() {
		const auto dataHandler = TES3::DataHandler::get();
		const auto dialogues = dataHandler->nonDynamicData->dialogues;
		if (dialogues->empty()) {
			return;
		}

		// Create a sorter structure that caches the length of the topic so we don't have to constantly calculate it.
		struct DialogueLengthCache {
			TES3::Dialogue* dialogue;
			size_t length;

			DialogueLengthCache(TES3::Dialogue* d) {
				dialogue = d;
				length = strlen(d->name ? d->name : "");
			}
		};
		struct {
			bool operator()(const DialogueLengthCache& a, const DialogueLengthCache& b) {
				return a.length > b.length;
			}
		} dialogueLengthSorter;

		// Clone the dialogues in an array, then sort the array.
		// TODO: We should improve our TList implementation to support sorting.
		std::vector<DialogueLengthCache> sortedDialogues;
		sortedDialogues.reserve(dialogues->size());
		for (const auto& dialogue : *dialogues) {
			sortedDialogues.push_back(dialogue);
		}
		std::ranges::sort(sortedDialogues, dialogueLengthSorter);

		// Clone the sorted data back into the TList, without any allocations.
		auto itt = dialogues->head;
		for (auto i = 0u; i < sortedDialogues.size(); ++i) {
			itt->data = sortedDialogues[i].dialogue;
			itt = itt->next;
		}

		// Update load progress all at once. This doesn't take a significant amount of time.
		dataHandler->incrementLoadedRecords(dialogues->size());
		const auto percentLoaded = dataHandler->getTotalLoadedRecordsFraction() * 100.0f;
		TES3::UI::updateLoadingMenu(percentLoaded);
	}

	static void __fastcall PatchDialogueInfoDestructorCleanup(TES3::DialogueInfo* info, DWORD _) {
		info->removeFromLoadIDCache();
		const auto TES3_DialogueInfo_dtor = reinterpret_cast<void(__thiscall*)(TES3::DialogueInfo*)>(0x4AE8A0);
		TES3_DialogueInfo_dtor(info);
	}

	static void __fastcall PatchMergeDialogueInfo(TES3::Dialogue* self, DWORD _, TES3::DialogueInfo* info, bool alwaysAddInfo) {
		const auto previousId = (info->loadLinkNode && info->loadLinkNode->previous) ? info->loadLinkNode->previous : "";
		if (previousId[0] == '\0') {
			self->info.push_front(info);
			self->cacheInfoByLoadID(info);
			return;
		}

		if (alwaysAddInfo || self->info.empty()) {
			self->info.push_back(info);
			self->cacheInfoByLoadID(info);
			return;
		}

		auto insertBefore = self->info.end();
		if (const auto previousInfo = self->findInfoByLoadID(previousId)) {
			for (auto it = self->info.begin(); it != self->info.end(); ++it) {
				if (*it == previousInfo) {
					insertBefore = it;
					++insertBefore;
					break;
				}
			}
		}
		else {
			for (auto it = self->info.rbegin(); it != self->info.rend(); ++it) {
				const auto currentLoadLinkNode = (*it)->loadLinkNode;
				const auto currentInfoName = currentLoadLinkNode ? currentLoadLinkNode->name : "";
				if (currentInfoName && se::string::iequal(previousId, currentInfoName)) {
					insertBefore = it.base();
					break;
				}
			}
		}

		self->info.insert(insertBefore, info);
		self->cacheInfoByLoadID(info);
	}

	static void __fastcall PatchMergeDialogues(TES3::Dialogue* self, DWORD _, TES3::Dialogue* other) {
		if (other->type > TES3::DialogueType::MAX_VALUE) {
			return;
		}

		// Changed behavior: Don't show warnings for empty, deleted dialogues
		if (self->name == nullptr && other->name == nullptr && self->type != other->type && self->getDeleted() && other->getDeleted()) {
			return;
		}

		const auto myName = self->name ? self->name : "";
		const auto otherName = other->name ? other->name : "";

		if (self->type != other->type) {
			tes3::logAndShowError("Dialogue \"%s\" type \"%s\" tried to become type \"%s\".\r\n",
				myName,
				self->getFilterTypeName(),
				other->getFilterTypeName());
			return;
		}

		if (other->getDeleted()) {
			self->setDeleted(true);
		}

		if (!se::string::equal(myName, otherName)) {
			tes3::setDataString(&self->name, otherName);
		}

		for (auto& info : other->info) {
			PatchMergeDialogueInfo(self, NULL, info, false);
		}
	}

	void install() {
		using se::memory::genCallEnforced;
		using se::memory::genCallUnprotected;

		// Patch: Optimize GetDeadCount and associated dialogue filtering/logic.
		auto killCounter_increment = &TES3::KillCounter::incrementMobile;
		genCallEnforced(0x523D73, 0x55D820, *reinterpret_cast<DWORD*>(&killCounter_increment));
		auto killCounter_getCount = &TES3::KillCounter::getKillCount;
		genCallEnforced(0x4B0B2E, 0x55D900, *reinterpret_cast<DWORD*>(&killCounter_getCount));
		genCallEnforced(0x50AC85, 0x55D900, *reinterpret_cast<DWORD*>(&killCounter_getCount));
		genCallEnforced(0x50ACAB, 0x55D900, *reinterpret_cast<DWORD*>(&killCounter_getCount));
		genCallEnforced(0x745FF0, 0x55D900, *reinterpret_cast<DWORD*>(&killCounter_getCount));
#if MWSE_CUSTOM_KILLCOUNTER
		auto killCounter_ctor = &TES3::KillCounter::ctor;
		genCallEnforced(0x40DE9B, 0x55D750, *reinterpret_cast<DWORD*>(&killCounter_ctor));
		auto killCounter_dtor = &TES3::KillCounter::dtor;
		genCallEnforced(0x40E049, 0x55D7D0, *reinterpret_cast<DWORD*>(&killCounter_dtor));
		auto killCounter_clear = &TES3::KillCounter::clear;
		genCallEnforced(0x4C6F76, 0x55DBD0, *reinterpret_cast<DWORD*>(&killCounter_clear));
		auto killCounter_load = &TES3::KillCounter::load;
		genCallEnforced(0x4C076C, 0x55DA90, *reinterpret_cast<DWORD*>(&killCounter_load));
		auto killCounter_save = &TES3::KillCounter::save;
		genCallEnforced(0x4BCB7E, 0x55D950, *reinterpret_cast<DWORD*>(&killCounter_save));
#endif

		// Patch: Cache values between dialogue filters. The actual override that makes use of this cache is in LuaManager for its hooks.
		genCallUnprotected(0x4B1646, reinterpret_cast<DWORD>(PatchDialogueFilterCacheGetDisposition), 0x6);
		genCallUnprotected(0x4B167B, reinterpret_cast<DWORD>(PatchDialogueFilterCacheGetDisposition), 0x6);

		// Patch: Improve performance of initial game load.
		genCallEnforced(0x4BB84E, 0x4B2C90, reinterpret_cast<DWORD>(PatchDialogueSorting));
		genCallEnforced(0x4BC85C, 0x4B2C90, reinterpret_cast<DWORD>(PatchDialogueSorting));
		genCallEnforced(0x4BE98C, 0x4B2790, reinterpret_cast<DWORD>(PatchMergeDialogues));
		genCallEnforced(0x4B2828, 0x4B2840, reinterpret_cast<DWORD>(PatchMergeDialogueInfo));
		genCallEnforced(0x4BFB6E, 0x4B2840, reinterpret_cast<DWORD>(PatchMergeDialogueInfo));
		auto dialogueFindInfoByLoadID = &TES3::Dialogue::findInfoByLoadID;
		genCallEnforced(0x431F77, 0x4B2920, *reinterpret_cast<DWORD*>(&dialogueFindInfoByLoadID));
		genCallEnforced(0x4BF8AE, 0x4B2920, *reinterpret_cast<DWORD*>(&dialogueFindInfoByLoadID));
		genCallEnforced(0x4BFA40, 0x4B2920, *reinterpret_cast<DWORD*>(&dialogueFindInfoByLoadID));
		genCallEnforced(0x4AE883, 0x4AE8A0, reinterpret_cast<DWORD>(PatchDialogueInfoDestructorCleanup));
	}
}
