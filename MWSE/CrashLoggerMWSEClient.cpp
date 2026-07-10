#include "CrashLogger.h"

#include "Log.h"

#include "TES3Defines.h"
#include "TES3Game.h"

#include "NIDX8Renderer.h"

namespace CrashLogger::Labels {
	void FillMWSELabels();
}

namespace CrashLogger::Client {
	std::ostream& GetLog() {
		return mwse::log::getLog();
	}

	std::string GetGPU() {
		const auto game = TES3::Game::get();
		if (!game || !game->renderer) return "<unknown>";

		const auto adapter = game->renderer->getCurrentAdapter();
		return adapter ? adapter->identifier.Description : "<unknown>";
	}

	static const char* GetKnownClassNameInternal(void* object) {
		__try {
			switch (*static_cast<UINT32*>(object)) {
			case TES3::VirtualTableAddress::Activator: return "Activator";
			case TES3::VirtualTableAddress::ActorAnimController: return "ActorAnimController";
			case TES3::VirtualTableAddress::ActorWearsObjects: return "ActorWearsObjects";
			case TES3::VirtualTableAddress::AIPackageActivate: return "AIPackageActivate";
			case TES3::VirtualTableAddress::AIPackageBase: return "AIPackageBase";
			case TES3::VirtualTableAddress::AIPackageEscort: return "AIPackageEscort";
			case TES3::VirtualTableAddress::AIPackageFollow: return "AIPackageFollow";
			case TES3::VirtualTableAddress::AIPackageTravel: return "AIPackageTravel";
			case TES3::VirtualTableAddress::AIPackageWander: return "AIPackageWander";
			case TES3::VirtualTableAddress::Alchemy: return "Alchemy";
			case TES3::VirtualTableAddress::AnimatedObject: return "AnimatedObject";
			case TES3::VirtualTableAddress::AnimationGroup: return "AnimationGroup";
			case TES3::VirtualTableAddress::Apparatus: return "Apparatus";
			case TES3::VirtualTableAddress::ArchiveFile: return "ArchiveFile";
			case TES3::VirtualTableAddress::Armor: return "Armor";
			case TES3::VirtualTableAddress::Attribute: return "Attribute";
			case TES3::VirtualTableAddress::AttributeFatigue: return "AttributeFatigue";
			case TES3::VirtualTableAddress::AttributeSkill: return "AttributeSkill";
			case TES3::VirtualTableAddress::BaseObject: return "BaseObject";
			case TES3::VirtualTableAddress::Birthsign: return "Birthsign";
			case TES3::VirtualTableAddress::BodyPart: return "BodyPart";
			case TES3::VirtualTableAddress::Book: return "Book";
			case TES3::VirtualTableAddress::Cell: return "Cell";
			case TES3::VirtualTableAddress::Class: return "Class";
			case TES3::VirtualTableAddress::Clothing: return "Clothing";
			case TES3::VirtualTableAddress::ContainerBase: return "ContainerBase";
			case TES3::VirtualTableAddress::ContainerInstance: return "ContainerInstance";
			case TES3::VirtualTableAddress::CreatureBase: return "CreatureBase";
			case TES3::VirtualTableAddress::CreatureInstance: return "CreatureInstance";
			case TES3::VirtualTableAddress::CutscenePlayer: return "CutscenePlayer";
			case TES3::VirtualTableAddress::Dialogue: return "Dialogue";
			case TES3::VirtualTableAddress::DialogueInfo: return "DialogueInfo";
			case TES3::VirtualTableAddress::Door: return "Door";
			case TES3::VirtualTableAddress::Enchantment: return "Enchantment";
			case TES3::VirtualTableAddress::Faction: return "Faction";
			case TES3::VirtualTableAddress::Game: return "Game";
			case TES3::VirtualTableAddress::GameBase: return "GameBase";
			case TES3::VirtualTableAddress::GameSetting: return "GameSetting";
			case TES3::VirtualTableAddress::GlobalVariable: return "GlobalVariable";
			case TES3::VirtualTableAddress::Ingredient: return "Ingredient";
			case TES3::VirtualTableAddress::Land: return "Land";
			case TES3::VirtualTableAddress::LandTexture: return "LandTexture";
			case TES3::VirtualTableAddress::LeveledCreature: return "LeveledCreature";
			case TES3::VirtualTableAddress::LeveledItem: return "LeveledItem";
			case TES3::VirtualTableAddress::Light: return "Light";
			case TES3::VirtualTableAddress::Lockpick: return "Lockpick";
			case TES3::VirtualTableAddress::MagicEffect: return "MagicEffect";
			case TES3::VirtualTableAddress::MagicSourceInstance: return "MagicSourceInstance";
			case TES3::VirtualTableAddress::Miscellaneous: return "Miscellaneous";
			case TES3::VirtualTableAddress::MobileActor: return "MobileActor";
			case TES3::VirtualTableAddress::MobileCreature: return "MobileCreature";
			case TES3::VirtualTableAddress::MobileNPC: return "MobileNPC";
			case TES3::VirtualTableAddress::MobileObject: return "MobileObject";
			case TES3::VirtualTableAddress::MobilePlayer: return "MobilePlayer";
			case TES3::VirtualTableAddress::MobileProjectile: return "MobileProjectile";
			case TES3::VirtualTableAddress::NPCBase: return "NPCBase";
			case TES3::VirtualTableAddress::NPCInstance: return "NPCInstance";
			case TES3::VirtualTableAddress::Object: return "Object";
			case TES3::VirtualTableAddress::PathGrid: return "PathGrid";
			case TES3::VirtualTableAddress::PhysicalObject: return "PhysicalObject";
			case TES3::VirtualTableAddress::PlayerAnimController: return "PlayerAnimController";
			case TES3::VirtualTableAddress::Probe: return "Probe";
			case TES3::VirtualTableAddress::Quest: return "Quest";
			case TES3::VirtualTableAddress::Race: return "Race";
			case TES3::VirtualTableAddress::Reference: return "Reference";
			case TES3::VirtualTableAddress::Region: return "Region";
			case TES3::VirtualTableAddress::RepairTool: return "RepairTool";
			case TES3::VirtualTableAddress::Script: return "Script";
			case TES3::VirtualTableAddress::Skill: return "Skill";
			case TES3::VirtualTableAddress::Sound: return "Sound";
			case TES3::VirtualTableAddress::SoundGenerator: return "SoundGenerator";
			case TES3::VirtualTableAddress::Spell: return "Spell";
			case TES3::VirtualTableAddress::SpellProjectile: return "SpellProjectile";
			case TES3::VirtualTableAddress::Static: return "Static";
			case TES3::VirtualTableAddress::TArray: return "TArray";
			case TES3::VirtualTableAddress::TES3Archive: return "TES3Archive";
			case TES3::VirtualTableAddress::Weapon: return "Weapon";
			case TES3::VirtualTableAddress::WeatherAshstorm: return "WeatherAshstorm";
			case TES3::VirtualTableAddress::WeatherBase: return "WeatherBase";
			case TES3::VirtualTableAddress::WeatherBlight: return "WeatherBlight";
			case TES3::VirtualTableAddress::WeatherBlizzard: return "WeatherBlizzard";
			case TES3::VirtualTableAddress::WeatherClear: return "WeatherClear";
			case TES3::VirtualTableAddress::WeatherCloudy: return "WeatherCloudy";
			case TES3::VirtualTableAddress::WeatherFog: return "WeatherFog";
			case TES3::VirtualTableAddress::WeatherOvercast: return "WeatherOvercast";
			case TES3::VirtualTableAddress::WeatherRain: return "WeatherRain";
			case TES3::VirtualTableAddress::WeatherSnow: return "WeatherSnow";
			case TES3::VirtualTableAddress::WeatherStorm: return "WeatherStorm";
			default: return "";
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			return "";
		}
	}

	std::string GetKnownClassName(void* object) {
		return GetKnownClassNameInternal(object);
	}

	bool DescribeObject(void* object, std::string& labelName, std::string& objectName, std::string& description) {
		return false;
	}

	bool IsValidVTable(UINT32 vtable) {
		return vtable > 0x740000 && vtable < 0x750000;
	}

	void FillLabels() {
		Labels::FillMWSELabels();
	}
}
