#pragma once

#if defined(SE_TARGETS_MW) && SE_TARGETS_MW == 1

#include "TES3VirtualTableDefines.Morrowind.h"

#elif defined(SE_TARGETS_CS) && SE_TARGETS_CS == 1

#endif

namespace TES3 {
	class MagicEffectController;
	struct ActionAttachment;
	struct ActionData;
	struct Activator;
	struct ActiveMagicEffect;
	struct Actor;
	struct ActorAnimationController;
	struct ActorVirtualTable;
	struct AIConfig;
	struct AIPackage;
	struct AIPackageActivate;
	struct AIPackageConfig;
	struct AIPackageEscort;
	struct AIPackageFollow;
	struct AIPackageTravel;
	struct AIPackageWander;
	struct AIPlanner;
	struct Alchemy;
	struct AnimationData;
	struct AnimationGroup;
	struct Apparatus;
	struct Archive;
	struct Armor;
	struct ArmorSlotData;
	struct Attachment;
	struct AudioController;
	struct BaseObject;
	struct BaseObjectVirtualTable;
	struct Birthsign;
	struct BodyPart;
	struct BodyPartManager;
	struct Book;
	struct Cell;
	struct Class;
	struct Clothing;
	struct ClothingSlotData;
	struct CombatSession;
	struct Container;
	struct ContainerInstance;
	struct Creature;
	struct CreatureInstance;
	struct CrimeController;
	struct CrimeEvent;
	struct CrimeEventList;
	struct CriticalSection;
	struct CutscenePlayer;
	struct DataHandler;
	struct Dialogue;
	struct DialogueConditional;
	struct DialogueFilterContext;
	struct DialogueInfo;
	struct Door;
	struct Effect;
	struct Enchantment;
	struct EquipmentStack;
	struct Faction;
	struct Fader;
	struct Font;
	struct Game;
	struct GameFile;
	struct GameSetting;
	struct GameSettingInfo;
	struct GlobalScript;
	struct GlobalVariable;
	struct Ingredient;
	struct InputConfig;
	struct InputController;
	struct Inventory;
	struct Item;
	struct ItemData;
	struct ItemStack;
	struct KeyframeDefinition;
	struct Land;
	struct LandTexture;
	struct LeveledCreature;
	struct LeveledItem;
	struct LeveledListNode;
	struct Light;
	struct LoadScreenManager;
	struct LockAttachmentNode;
	struct Lockpick;
	struct MagicEffect;
	struct MagicEffectInstance;
	struct MagicInstanceController;
	struct MagicSourceCombo;
	struct MagicSourceInstance;
	struct MapNote;
	struct Misc;
	struct MobManager;
	struct MobileActor;
	struct MobileActor_vTable;
	struct MobileCreature;
	struct MobileNPC;
	struct MobileNPC_vTable;
	struct MobileObject;
	struct MobileObject_vTable;
	struct MobilePlayer;
	struct MobileProjectile;
	struct MobileProjectile_vTable;
	struct MobileSpellProjectile;
	struct MobileSpellProjectile_vTable;
	struct Moon;
	struct NonDynamicData;
	struct NPC;
	struct NPCBase;
	struct NPCInstance;
	struct Object;
	struct ObjectVirtualTable;
	struct OwnershipAttachmentNode;
	struct PhysicalObject;
	struct PhysicalObjectVirtualTable;
	struct PlayerAnimationController;
	struct PlayerBounty;
	struct Probe;
	struct ProcessManager;
	struct ProjectileManager;
	struct Quest;
	struct Race;
	struct Reference;
	struct ReferenceList;
	struct Region;
	struct RegionSound;
	struct RepairTool;
	struct Script;
	struct ScriptCompiler;
	struct ScriptVariables;
	struct Skill;
	struct SkillStatistic;
	struct SoulGemData;
	struct Sound;
	struct SoundBuffer;
	struct SoundGenerator;
	struct Spell;
	struct SpellList;
	struct StartScript;
	struct Static;
	struct Statistic;
	struct TravelDestination;
	struct VFX;
	struct VFXManager;
	struct WaterController;
	struct Weapon;
	struct WearablePart;
	struct Weather;
	struct WeatherAsh;
	struct WeatherBlight;
	struct WeatherBlizzard;
	struct WeatherClear;
	struct WeatherCloudy;
	struct WeatherController;
	struct WeatherFoggy;
	struct WeatherOvercast;
	struct WeatherRain;
	struct WeatherSnow;
	struct WeatherThunder;
	struct WorldController;


	namespace UI {
		struct Element;
		struct InventoryTile;
		struct String;
		struct Tree;

		class LuaData;
	}

	//
	// Object types. They are char[4], or can be interpreted as a 32-bit integer.
	//

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
