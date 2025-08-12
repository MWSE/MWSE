#pragma once

namespace se::cs {
	struct Actor;
	struct AIConfig;
	struct AnimatedObject;
	struct Apparatus;
	struct Armor;
	struct BaseObject;
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
	struct GameFile;
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

	struct BaseObject_VirtualTable;
	struct Object_VirtualTable;
	struct Actor_VirtualTable;

	namespace ObjectType {
		enum ObjectType {
			Invalid = 0,
			Activator = 'ITCA',
			Alchemy = 'HCLA',
			Ammo = 'OMMA',
			AnimationGroup = 'GINA',
			Apparatus = 'APPA',
			Armor = 'OMRA',
			Birthsign = 'NGSB',
			Bodypart = 'YDOB',
			Book = 'KOOB',
			Cell = 'LLEC',
			Class = 'SALC',
			Clothing = 'TOLC',
			Container = 'TNOC',
			Creature = 'AERC',
			CreatureClone = 'CERC',
			Dialogue = 'LAID',
			DialogueInfo = 'OFNI',
			Door = 'ROOD',
			Enchantment = 'HCNE',
			Faction = 'TCAF',
			GameSetting = 'TSMG',
			Global = 'BOLG',
			Ingredient = 'RGNI',
			Land = 'DNAL',
			LandTexture = 'XETL',
			LeveledCreature = 'CVEL',
			LeveledItem = 'IVEL',
			Light = 'HGIL',
			Lockpick = 'KCOL',
			MagicEffect = 'FEGM',
			MagicSourceInstance = 'LLPS',
			Misc = 'CSIM',
			MobileCreature = 'RCAM',
			MobileNPC = 'HCAM',
			MobileObject = 'TCAM',
			MobilePlayer = 'PCAM',
			MobileProjectile = 'JRPM',
			MobileSpellProjectile = 'PSPM',
			NPC = '_CPN',
			NPCClone = 'CCPN',
			PathGrid = 'DRGP',
			Probe = 'BORP',
			Quest = 'SEUQ',
			Race = 'ECAR',
			Reference = 'RFER',
			Region = 'NGER',
			Repair = 'APER',
			Script = 'TPCS',
			Skill = 'LIKS',
			Sound = 'NUOS',
			SoundGenerator = 'GDNS',
			Spell = 'LEPS',
			Static = 'TATS',
			TES3 = '3SET',
			Training = 'IART',
			Weapon = 'PAEW',
		};
	}
}
