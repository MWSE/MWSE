#include "TES3DialogueInfo.h"

#include "LuaManager.h"
#include "LuaInfoFilterEvent.h"

#include "TES3Actor.h"
#include "TES3Class.h"
#include "TES3Faction.h"
#include "TES3Race.h"
#include "TES3Cell.h"

namespace TES3 {
	const auto TES3_DialogueInfo_getText = reinterpret_cast<const char* (__thiscall*)(DialogueInfo*)>(0x4B1B80);
	const auto TES3_DialogueInfo_loadId = reinterpret_cast<bool(__thiscall*)(DialogueInfo*)>(0x4B1A10);
	const auto TES3_DialogueInfo_unloadId = reinterpret_cast<void(__thiscall*)(DialogueInfo*)>(0x4AF3A0);
	const auto TES3_DialogueInfo_filter = reinterpret_cast<bool(__thiscall*)(DialogueInfo*, Object*, Reference*, int, Dialogue*)>(0x4B0190);

	const char* DialogueInfo::getText() {
		return TES3_DialogueInfo_getText(this);
	}

	bool DialogueInfo::loadId() {
		return TES3_DialogueInfo_loadId(this);
	}

	void DialogueInfo::unloadId() {
		TES3_DialogueInfo_unloadId(this);
	}

	bool DialogueInfo::filter(Object * actor, Reference * reference, int source, Dialogue * dialogue) {
		bool result = TES3_DialogueInfo_filter(this, actor, reference, source, dialogue);

		if (mwse::lua::event::InfoFilterEvent::getEventEnabled()) {
			auto& luaManager = mwse::lua::LuaManager::getInstance();
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::table eventData = stateHandle.triggerEvent(new mwse::lua::event::InfoFilterEvent(this, actor, reference, source, dialogue, result));
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

	sol::optional<std::string> DialogueInfo::getID() {
		if (loadId()) {
			std::string id = loadLinkNode->name;
			unloadId();
			return std::move(id);
		}

		return sol::optional<std::string>();
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

	std::string DialogueInfo::getLongIDFromFile() {
		if (loadId()) {
			std::string id = loadLinkNode->name;
			unloadId();
			return id;
		}
		return "";
	}

	std::string DialogueInfo::toJson() {
		std::ostringstream ss;
		ss << "\"tes3dialogueInfo:" << getLongIDFromFile() << "\"";
		return std::move(ss.str());
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::DialogueInfo)
