#pragma once

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
	struct BoundingBox;
	struct Cell;
	struct CellExteriorData;
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
	struct DataHandler;
	struct Dialogue;
	struct DialogueConditional;
	struct DialogueInfo;
	struct Door;
	struct Effect;
	struct Enchantment;
	struct EquipmentStack;
	struct Faction;
	struct Fader;
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
	struct LockAttachmentNode;
	struct Lockpick;
	struct MagicEffect;
	struct MagicEffectInstance;
	struct MagicSourceCombo;
	struct MagicSourceInstance;
	struct MapNote;
	struct Matrix33;
	struct Misc;
	struct MobController;
	struct MobileActor_vTable;
	struct MobileActor;
	struct MobileCreature;
	struct MobileNPC_vTable;
	struct MobileNPC;
	struct MobileObject_vTable;
	struct MobileObject;
	struct MobilePlayer;
	struct MobileProjectile;
	struct MobileSpellProjectile;
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
	struct MagicInstanceController;
	struct SpellList;
	struct StartScript;
	struct Static;
	struct Statistic;
	struct Transform;
	struct TravelDestination;
	struct Vector2;
	struct Vector3;
	struct Vector4;
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

	namespace VirtualTableAddress {
		enum VirtualTableAddress : unsigned int {
			Activator = 0x747FC4,
			ActorAnimController = 0x74ADE8,
			ActorWearsObjects = 0x7478E0,
			AIPackageActivate = 0x74ADC4,
			AIPackageBase = 0x74AD58,
			AIPackageEscort = 0x74AD98,
			AIPackageFollow = 0x74ADB0,
			AIPackageTravel = 0x74AD80,
			AIPackageWander = 0x74AD6C,
			Alchemy = 0x749684,
			AnimatedObject = 0x74A580,
			AnimationGroup = 0x747880,
			Apparatus = 0x748110,
			ArchiveFile = 0x7478BC,
			Armor = 0x748258,
			Attribute = 0x746464,
			AttributeFatigue = 0x746600,
			AttributeSkill = 0x746AD4,
			BaseObject = 0x74A2D8,
			Birthsign = 0x747644,
			BodyPart = 0x7483A4,
			Book = 0x7484EC,
			Cell = 0x74A0F8,
			Class = 0x74939C,
			Clothing = 0x748634,
			ContainerBase = 0x74877C,
			ContainerInstance = 0x7488FC,
			CreatureBase = 0x747A74,
			CreatureInstance = 0x747BF4,
			CutscenePlayer = 0x7467A0,
			Dialogue = 0x74983C,
			DialogueInfo = 0x749818,
			Door = 0x748A7C,
			Enchantment = 0x749548,
			Faction = 0x7497CC,
			Game = 0x74665C,
			GameBase = 0x74A920,
			GameSetting = 0x749868,
			GlobalVariable = 0x7497F0,
			Ingredient = 0x748BC8,
			Land = 0x7498D8,
			LandTexture = 0x7498B4,
			LeveledCreature = 0x749958,
			LeveledItem = 0x749AA8,
			Light = 0x749BF4,
			Lockpick = 0x748D10,
			MagicEffect = 0x7493E4,
			MagicSourceInstance = 0x74AA14,
			Miscellaneous = 0x748E58,
			MobileActor = 0x74AB4C,
			MobileCreature = 0x74AFA4,
			MobileNPC = 0x74AE6C,
			MobileObject = 0x74B0C0,
			MobilePlayer = 0x74B174,
			MobileProjectile = 0x74B2B4,
			NPCBase = 0x749DE8,
			NPCInstance = 0x749F68,
			Object = 0x74A444,
			PathGrid = 0x74A8F8,
			PhysicalObject = 0x74A2FC,
			PlayerAnimController = 0x74AE30,
			Probe = 0x748FA0,
			Quest = 0x7469E0,
			Race = 0x749378,
			Reference = 0x74A140,
			Region = 0x7477BC,
			RepairTool = 0x7490E8,
			Script = 0x74A990,
			Skill = 0x7493C0,
			Sound = 0x74A9C0,
			SoundGenerator = 0x74A9EC,
			Spell = 0x749408,
			SpellProjectile = 0x74B360,
			Static = 0x749230,
			TArray = 0x747A60,
			TES3Archive = 0x7478B0,
			Weapon = 0x74A7A4,
			WeatherAshstorm = 0x746C60,
			WeatherBase = 0x746BD4,
			WeatherBlight = 0x746C70,
			WeatherBlizzard = 0x746C90,
			WeatherClear = 0x746BE4,
			WeatherCloudy = 0x746BFC,
			WeatherFog = 0x746C0C,
			WeatherOvercast = 0x746C1C,
			WeatherRain = 0x746C2C,
			WeatherSnow = 0x746C80,
			WeatherStorm = 0x746C48,
		};
	}

	namespace UI {
		struct Element;
		struct InventoryTile;
		struct String;
		struct Tree;
	}

}
