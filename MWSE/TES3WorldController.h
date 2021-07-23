#pragma once

#include "TES3Defines.h"
#include "TES3UIDefines.h"

#include "TES3IteratedList.h"
#include "TES3Vectors.h"

#include "TES3UIVector.h"

#include "NIDefines.h"
#include "NIDirectionalLight.h"
#include "NIRenderedTexture.h"
#include "NIProperty.h"
#include "NISourceTexture.h"
#include "NITextureEffect.h"

namespace TES3 {
	enum class MusicSituation : int {
		Explore = 0,
		Combat = 1,
		Uninterruptible = 2
	};

	enum class ItemSoundState : int {
		Up = 0,
		Down = 1,
		Direct = 2,
	};

	struct WorldControllerRenderCamera {
		struct CameraData {
			NI::Camera* camera; // 0x0
			NI::Node* unknown_0x4;
			float fovDegrees; // 0x8
			float nearPlaneDistance; // 0xC
			float farPlaneDistance; // 0x10
			float viewportWidth; // 0x14
			float viewportHeight; // 0x18

			//
			// Other related this-call functions.
			//

			float getFOV() const;
			void setFOV(float degrees);

		};
		void* vTable; // 0x0
		NI::Renderer* renderer; // 0x4
		NI::Node* root; // 0x8
		NI::Node* cameraRoot; // 0xC
		CameraData cameraData; // 0x10

		//
		// Custom functions.
		//

		NI::Camera* getCamera() const;

	};
	static_assert(sizeof(WorldControllerRenderCamera::CameraData) == 0x1C, "TES3::WorldControllerRenderCamera::CameraData failed size validation");
	static_assert(sizeof(WorldControllerRenderCamera) == 0x2C, "TES3::WorldControllerRenderCamera failed size validation");

	struct WorldControllerRenderTarget : WorldControllerRenderCamera {
		NI::Pointer<NI::RenderedTexture> renderedTexture; // 0x2C
		NI::Pointer<NI::Object> unknown_0x30;
		int unknown_0x34;
		int unknown_0x38;
		int unknown_0x3C;
		int unknown_0x40;
		int unknown_0x44;
		int unknown_0x48;
		NI::Pointer<NI::ZBufferProperty> zBufferProperty; // 0x4C
		int unknown_0x50;
		int unknown_0x54;
		NI::Pointer<NI::Object> unknown_0x58; // OffscreenSceneGraph::MasterPropertyAccumulator
		int unknown_0x5C;
		NI::Pointer<NI::DirectionalLight> directionalLight; // 0x60
		int unknown_0x64;
		NI::Pointer<NI::Object> unknown_0x68; // NiScreenPolygon
		int unknown_0x6C;
		int unknown_0x70;
		int unknown_0x74;
		int unknown_0x78;
		NI::Pointer<NI::AlphaProperty> alphaProperty; // 0x7C
		NI::Pointer<NI::VertexColorProperty> vertexColorProperty; // 0x80
	};
	static_assert(sizeof(WorldControllerRenderTarget) == 0x84, "TES3::WorldControllerRenderTarget failed size validation");

	struct MouseController {
		NI::Object* cursors[5];
		int unknown_0x14;
		Vector3 position; // 0x18
		Vector3 minimumPosition; // 0x24
		Vector3 maximumPosition; // 0x30
		NI::Node* cursorRoot; // 0x3C
		NI::Node* cursorChildRoot; // 0x40
		int unknown_0x44;
		int unknown_0x48;
		int unknown_0x4C;
		int unknown_0x50;
		int unknown_0x54;
		int unknown_0x58;
		int unknown_0x5C;
		int unknown_0x60;
		int unknown_0x64;
		int unknown_0x68;
		int unknown_0x6C;
		int unknown_0x70;
		int unknown_0x74;
		int unknown_0x78;
		int unknown_0x7C;
	};
	static_assert(sizeof(MouseController) == 0x80, "TES3::MouseController failed size validation");

#define MWSE_CUSTOM_KILLCOUNTER true
	struct KillCounter {
#if !MWSE_CUSTOM_KILLCOUNTER
		struct Node {
			int count; // 0x0
			Actor* actor; // 0x4
		};
#endif
		int werewolfKills; // 0x0
		int totalKills; // 0x4
#if MWSE_CUSTOM_KILLCOUNTER
		std::unordered_map<Actor*, int>* counter;
#else
		IteratedList<Node*>* killedActors; // 0x8
#endif

		//
		// Custom functions.
		//

		KillCounter* ctor();
		void dtor();

		int getKillCount(Actor* actor) const;
		void decrementMobile(MobileActor* actor);
		void decrement(Actor* actor);
		void incrementMobile(MobileActor* actor);
		void increment(Actor* actor);
		void setKillCount(Actor* actor, int count);
		void clear();

		void load(GameFile* file);
		void save(GameFile* file) const;

	};
	static_assert(sizeof(KillCounter) == 0xC, "TES3::KillCounter failed size validation");

	struct InventoryData {
		UI::Vector<UI::InventoryTile*> tiles; // 0x0
		bool unknown_0x10;
		int unknown_0x14;
		int unknown_0x18;
		int unknown_0x1C;
		int unknown_0x20;

		//
		// Other related this-call functions.
		//

		_declspec(dllexport) void clearIcons(int type);
		_declspec(dllexport) void addInventoryItems(Inventory* inventory, int type);

	};
	static_assert(sizeof(InventoryData) == 0x24, "TES3::InventoryData failed size validation");

	struct Font {
		short propertyCount; // 0x0
		const char* filePath; // 0x4
		NI::Property* texturingProperties[8]; // 0x8 // Up to 8 NiTexturingProperty pointers, filled count is stored by propertyCount.
		NI::Property* vertexColorProperty; // 0x28 // NiVertexColorProperty.
		int unknown_0x2C;
		int unknown_0x30;
		void* rawFontData; // 0x34 // The raw .fnt file contents.
	};
	static_assert(sizeof(Font) == 0x38, "TES3::Font failed size validation");

	struct JournalHTML {
		bool changedSinceLastSync; // 0x0
		char* data; // 0x4
		unsigned int length; // 0x8

		//
		// Other related this-call functions.
		//

		void updateJournal(DialogueInfo* info, MobileActor* updatingActor = nullptr);

		//
		// Custom functions
		//

		void writeTimestampedEntry(const char* text);
		void writeText(const char* text);
		void showJournalUpdateNotification();

	};
	static_assert(sizeof(JournalHTML) == 0xC, "TES3::JournalHTML failed size validation");

	struct SplashController {
		struct ActiveSplash {
			NI::Pointer<NI::Node> node; // 0x0
			float age; // 0x4
			float maxAge; // 0x8
		};
		int bloodMeshCount; // 0x0
		int bloodTextureCount; // 0x4
		NI::Pointer<NI::Node> bloodMeshes[4]; // 0x8
		float bloodSplashDurations[6]; // 0x18 // ActiveSplash::maxAge for the used mesh index offset by 1. First and last entries seem unused garbage...
		NI::Pointer<NI::SourceTexture> bloodTextures[8]; // 0x30
		NI::Pointer<NI::TexturingProperty> bloodTextureProperties[8]; // 0x50
		IteratedList<ActiveSplash*>* activeSplashes; // 0x70
	};
	static_assert(sizeof(SplashController) == 0x74, "TES3::SplashController failed size validation");
	static_assert(sizeof(SplashController::ActiveSplash) == 0xC, "TES3::SplashController::ActiveSplash failed size validation");

	struct WorldController {
		int framesSinceLastFPSMeasure; // 0x0
		int framesPerFPSMeasure; // 0x4, defaults to 3
		float lightFlickerSimTickRate; // 0x8, defaults to 15.0
		float minFPS; // 0xC, defaults to 5.0
		float maxFPS; // 0x10
		float framesPerSecond; // 0x14
		float lightFlickerSimTicksToSimulate; // 0x18
		float lightFlickerSimTicksSmoothed; // 0x1C
		unsigned int systemTimeMillis; // 0x20
		unsigned int lastFrameTimeMillis; // 0x24
		unsigned int lastFPSMeasureTimeMillis; // 0x28
		float deltaTime; // 0x2C
		NI::Renderer * renderer; // 0x30
		AudioController * audioController; // 0x34
		int unknown_0x38;
		int unknown_0x3C;
		Font * fonts[3]; // 0x40
		InputController * inputController; // 0x4C
		MouseController * mouseController; // 0x50
		Script * scriptCompileAndRun; // 0x54
		WeatherController * weatherController; // 0x58
		MobController * mobController; // 0x5C
		KillCounter * playerKills; // 0x60
		JournalHTML * journalHTML; // 0x64
		SplashController * splashController; // 0x68
		IteratedList<Quest*> * journalController; // 0x6C
		MagicInstanceController * magicInstanceController; // 0x70
		void * vfxManager; // 0x74
		int viewWidth; // 0x78
		int viewHeight; // 0x7C
		int bitDepth; // 0x80
		int bShadows; // 0x84
		int helpDelay; // 0x88
		unsigned char hudStyle; // 0x8C
		float menuAlpha; // 0x90
		bool cursorOff; // 0x94
		float aiDistanceScale; // 0x98
		float difficulty; // 0x9C
		int werewolfFOV; // 0xA0
		float projectionDistance; // 0xA4
		GlobalVariable * gvarGameHour; // 0xA8
		GlobalVariable * gvarYear; // 0xAC
		GlobalVariable * gvarMonth; // 0xB0
		GlobalVariable * gvarDay; // 0xB4
		GlobalVariable * gvarDaysPassed; // 0xB8
		GlobalVariable * gvarTimescale; // 0xBC
		GlobalVariable * gvarCharGenState; // 0xC0
		GlobalVariable * gvarMonthsToRespawn; // 0xC4
		HWND Win32_hWndParent; // 0xC8
		HWND Win32_hWnd; // 0xCC
		HINSTANCE Win32_hInstance; // 0xD0
		bool flagEventMenuModeOn; // 0xD4
		bool flagEventMenuModeOff; // 0xD5
		bool flagMenuMode; // 0xD6
		char unknown_0xD7;
		bool collisionEnabled; // 0xD8
		char unknown_0xD9;
		char disableAI; // 0xDA
		bool stopGameLoop; // 0xDB
		char unknown_0xDC;
		char unknown_0xDD;
		char unknown_0xDE;
		char unknown_0xDF;
		bool flagTeleportingDisabled; // 0xE0
		bool flagLevitationDisabled; // 0xE1
		bool useBestAttack; // 0xE2
		bool quickSaveWhenResting; // 0xE3
		char unknown_0xE4;
		char unknown_0xE5;
		char unknown_0xE6;
		bool shaderWaterReflectsTerrain; // 0xE7
		float mouseSensitivity; // 0xE8
		float horzSensitivity; // 0xEC
		int shaderWaterReflectUpdate; // 0xF0
		NI::Node * nodeCursor; // 0xF4
		WorldControllerRenderCamera splashscreenCamera; // 0xF8
		WorldControllerRenderCamera worldCamera; // 0x124
		WorldControllerRenderCamera armCamera; // 0x150
		WorldControllerRenderCamera menuCamera; // 0x17C
		WorldControllerRenderTarget characterRenderTarget; // 0x1A8
		WorldControllerRenderTarget mapRenderTarget; // 0x22C
		WorldControllerRenderCamera shadowCamera; // 0x2B0
		void * shadowManager; // 0x2DC
		void * mapController; // 0x2E0
		UI::MenuController * menuController; // 0x2E4
		InventoryData * inventoryData; // 0x2E8
		Sound * soundWeaponSwish; // 0x2EC
		Sound * soundLightArmorHit; // 0x2F0
		Sound * soundMediumArmorHit; // 0x2F4
		Sound * soundHeavyArmorHit; // 0x2F8
		Sound * soundHealthDamage; // 0x2FC
		Sound * soundDrown; // 0x300
		Sound * soundDrowningDamage; // 0x304
		Sound * soundHandToHandHit; // 0x308
		Sound * soundHandToHandHit2; // 0x30C
		Sound * soundMiss; // 0x310
		Sound * soundCriticalDamage; // 0x314
		Sound * soundDefaultLand; // 0x318
		Sound * soundDefaultLandWater; // 0x31C
		Sound * soundMenuClick; // 0x320
		Sound * soundMenuSize; // 0x324
		Sound * soundItemRepair; // 0x328
		Cell * startingCell; // 0x32C
		float deadFloatScale; // 0x330
		int unknown_0x334;
		IteratedList<GlobalScript*> * globalScripts; // 0x338
		IteratedList<MobileActor*> * allActors; // 0x33C
		IteratedList<void*> * chargableItems; // 0x340
		bool showSubtitles; // 0x344
		int countMusicTracksBattle; // 0x348
		int countMusicTracksExplore; // 0x34C
		MusicSituation musicSituation; // 0x350
		Fader * transitionFader; // 0x354
		Fader * blindnessFader; // 0x358
		Fader * sunglareFader; // 0x35C
		Fader * hitFader; // 0x360
		Fader * werewolfFader; // 0x364
		bool enchantedItemEffectCreated; // 0x368
		NI::Pointer<NI::TextureEffect> enchantedItemEffect; // 0x36C
		NI::Pointer<NI::SourceTexture>* enchantedItemEffectTextures; // 0x370 // 32 elements in length.

		// Get singleton.
		_declspec (dllexport) static WorldController * get();

		//
		// Other related this-call functions.
		//

		void mainLoopBeforeInput();
		_declspec(dllexport) MobilePlayer* getMobilePlayer();
		_declspec(dllexport) void playItemUpDownSound(BaseObject* item, ItemSoundState state = ItemSoundState::Up, Reference* reference = nullptr);
		_declspec(dllexport) float getSimulationTimestamp();
		_declspec(dllexport) void processGlobalScripts();

		_declspec(dllexport) unsigned short getDaysInMonth(int);
		_declspec(dllexport) unsigned short getCumulativeDaysForMonth(int month);
		_declspec(dllexport) const char* getNameForMonth(int month);
		_declspec(dllexport) double getHighPrecisionSimulationTimestamp();

		_declspec(dllexport) bool applyEnchantEffect(NI::Node* node, Enchantment * enchantment);

		_declspec(dllexport) void updateTiming();
		_declspec(dllexport) void updateEnvironmentLightingWeather();

		float getAIDistance() const;
		void setAIDistance(float value);
		float getAIDistanceScale() const;
		void setAIDistanceScale(float scale);

		void rechargerAddItem(Object* item, ItemData* itemData, Enchantment* enchantment);

		//
		// Custom functions.
		//

		_declspec(dllexport) void tickClock();
		_declspec(dllexport) void checkForDayWrapping();

	};
	static_assert(sizeof(WorldController) == 0x374, "TES3::WorldController failed size validation");
	static_assert(offsetof(WorldController, inputController) == 0x4C, "TES3::WorldController failed offset validation");
	static_assert(offsetof(WorldController, mobController) == 0x5C, "TES3::WorldController failed offset validation");
	static_assert(offsetof(WorldController, gvarGameHour) == 0xA8, "TES3::WorldController failed offset validation");
	static_assert(offsetof(WorldController, flagMenuMode) == 0xD6, "TES3::WorldController failed offset validation");
	static_assert(offsetof(WorldController, menuController) == 0x2E4, "TES3::WorldController failed offset validation");
	static_assert(offsetof(WorldController, globalScripts) == 0x338, "TES3::WorldController failed offset validation");
	static_assert(offsetof(WorldController, menuController) == 0x2E4, "TES3::WorldController failed offset validation");
}
