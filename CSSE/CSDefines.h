#pragma once

#include "TES3Defines.h"

namespace se::cs {
	struct Actor;
	struct AIConfig;
	struct AnimatedObject;
	struct Apparatus;
	struct Armor;
	struct Birthsign;
	struct BodyPart;
	struct Book;
	struct Cell;
	struct Class;
	struct Creature;
	struct DataHandler;
	struct Dialogue;
	struct DialogueInfo;
	struct Door;
	struct Effect;
	struct Enchantment;
	struct Faction;
	struct GameSetting;
	struct GameSettingInitializer;
	struct GlobalVariable;
	struct ItemData;
	struct Land;
	struct LandTexture;
	struct LeveledCreature;
	struct LeveledItem;
	struct Light;
	struct Lockpick;
	struct MagicEffect;
	struct ModelLoader;
	struct NPC;
	struct Object;
	struct PhysicalObject;
	struct Probe;
	struct Race;
	struct RecordHandler;
	struct Reference;
	struct Region;
	struct RepairTool;
	struct Script;
	struct SecurityAttachmentNode;
	struct Skill;
	struct Sound;
	struct Spell;
	struct SpellList;
	struct Static;
	struct TravelDestination;
	struct Weapon;

	using BaseObject = TES3::BaseObject;
	using GameFile = TES3::GameFile;
	using ObjectType = TES3::ObjectType::ObjectType;
}
