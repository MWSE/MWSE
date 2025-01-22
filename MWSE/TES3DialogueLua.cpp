#include "TES3DialogueLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"
#include "TES3ObjectLua.h"

#include "TES3Actor.h"
#include "TES3Cell.h"
#include "TES3Class.h"
#include "TES3Dialogue.h"
#include "TES3DialogueConditional.h"
#include "TES3DialogueInfo.h"
#include "TES3Faction.h"
#include "TES3MobileActor.h"
#include "TES3Quest.h"
#include "TES3Race.h"
#include "TES3Reference.h"

namespace mwse::lua {
	void bindTES3Dialogue() {
		// Get our lua state.
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;

		// Binding for TES3::Dialogue
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::Dialogue>("tes3dialogue");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::BaseObject>();
			setUserdataForTES3BaseObject(usertypeDefinition);

			// Base class overrides.
			usertypeDefinition[sol::meta_function::to_string] = &TES3::Dialogue::getObjectID;
			usertypeDefinition["id"] = sol::readonly_property(&TES3::Dialogue::getObjectID);

			// Override tojson to use the name rather than the (non-existant) id.
			usertypeDefinition["__tojson"] = &TES3::Dialogue::toJson;

			// Basic property binding.
			usertypeDefinition["info"] = sol::readonly_property(&TES3::Dialogue::info);
			usertypeDefinition["journalIndex"] = sol::property(&TES3::Dialogue::getJournalIndex, &TES3::Dialogue::setJournalIndexAndMarkModified);
			usertypeDefinition["type"] = sol::readonly_property(&TES3::Dialogue::type);

			// Basic property binding.
			usertypeDefinition["addToJournal"] = &TES3::Dialogue::addToJournal_lua;
			usertypeDefinition["getInfo"] = &TES3::Dialogue::getDeepFilteredInfo_lua;
			usertypeDefinition["getJournalInfo"] = &TES3::Dialogue::getJournalInfo;
			usertypeDefinition["loadQuestName"] = &TES3::Dialogue::getQuestName;
		}

		// Binding for TES3::DialogueInfo
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::DialogueInfo>("tes3dialogueinfo");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::BaseObject>();
			setUserdataForTES3BaseObject(usertypeDefinition);

			// Allow dialogue to be serialized to json using its ID.
			usertypeDefinition["__tojson"] = &TES3::DialogueInfo::toJson;

			// Basic property binding.
			usertypeDefinition["disposition"] = sol::readonly_property(&TES3::DialogueInfo::disposition);
			usertypeDefinition["firstHeardFrom"] = &TES3::DialogueInfo::firstHeardFrom;
			usertypeDefinition["isQuestFinished"] = sol::property(&TES3::DialogueInfo::isQuestFinished);
			usertypeDefinition["isQuestName"] = sol::property(&TES3::DialogueInfo::isQuestName);
			usertypeDefinition["isQuestRestart"] = sol::property(&TES3::DialogueInfo::isQuestRestart);
			usertypeDefinition["journalIndex"] = sol::property(&TES3::DialogueInfo::getJournalIndex_lua, &TES3::DialogueInfo::setJournalIndex_lua);
			usertypeDefinition["npcRank"] = sol::readonly_property(&TES3::DialogueInfo::npcRank);
			usertypeDefinition["npcSex"] = sol::readonly_property(&TES3::DialogueInfo::npcSex);
			usertypeDefinition["pcRank"] = sol::readonly_property(&TES3::DialogueInfo::pcRank);
			usertypeDefinition["type"] = sol::readonly_property(&TES3::DialogueInfo::type);

			// Filter functions.
			usertypeDefinition["actor"] = sol::property(&TES3::DialogueInfo::getFilterActor);
			usertypeDefinition["npcRace"] = sol::property(&TES3::DialogueInfo::getFilterNPCRace);
			usertypeDefinition["npcClass"] = sol::property(&TES3::DialogueInfo::getFilterNPCClass);
			usertypeDefinition["npcFaction"] = sol::property(&TES3::DialogueInfo::getFilterNPCFaction);
			usertypeDefinition["cell"] = sol::property(&TES3::DialogueInfo::getFilterNPCCell);
			usertypeDefinition["pcFaction"] = sol::property(&TES3::DialogueInfo::getFilterPCFaction);

			// Load link access.
			usertypeDefinition["id"] = sol::readonly_property(&TES3::DialogueInfo::getID);

			// Functions exposed as properties.
			usertypeDefinition["text"] = sol::readonly_property(&TES3::DialogueInfo::getText);

			// Basic function binding.
			usertypeDefinition["runScript"] = &TES3::DialogueInfo::runScript;
			usertypeDefinition["filter"] = &TES3::DialogueInfo::filter;
			usertypeDefinition["findDialogue"] = &TES3::DialogueInfo::findDialogue;
			usertypeDefinition["getSoundPath"] = &TES3::DialogueInfo::getSoundPath;
		}

		// Binding for TES3::Quest
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::Quest>("tes3quest");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::BaseObject>();
			setUserdataForTES3BaseObject(usertypeDefinition);

			// Base class overrides.
			usertypeDefinition[sol::meta_function::to_string] = &TES3::Quest::getObjectID;
			usertypeDefinition["id"] = sol::readonly_property(&TES3::Quest::getObjectID);

			// Allow objects to be serialized to json using their ID.
			usertypeDefinition["__tojson"] = &TES3::Quest::toJson;

			// Basic property binding.
			usertypeDefinition["dialogue"] = sol::readonly_property(&TES3::Quest::dialogues);
			usertypeDefinition["info"] = sol::readonly_property(&TES3::Quest::activeInfos);
			usertypeDefinition["isActive"] = sol::readonly_property(&TES3::Quest::isActive);
			usertypeDefinition["isFinished"] = sol::readonly_property(&TES3::Quest::isFinished);
			usertypeDefinition["isStarted"] = sol::readonly_property(&TES3::Quest::isStarted);
		}
	}
}
