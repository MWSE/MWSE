#pragma once

#include "TES3Defines.h"
#include "TES3UIDefines.h"

#include "TES3IteratedList.h"
#include "TES3Vectors.h"

#include "TES3UIVector.h"

#include "NIDefines.h"
#include "NICamera.h"
#include "NIDirectionalLight.h"
#include "NIRenderedTexture.h"
#include "NIProperty.h"
#include "NIRenderer.h"
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
		Consume = 3
	};

	struct WorldControllerRenderCamera {
		struct VirtualTable {
			void* unknown_0x0;
			void* unknown_0x4;
			void* unknown_0x8;
			void* unknown_0xC;
			void* unknown_0x10;
			void* unknown_0x14;
			void* unknown_0x18;
		};
		struct CameraData {
			NI::Pointer<NI::Camera> camera; // 0x0
			NI::Pointer<NI::Node> unknown_0x4;
			float fovDegrees; // 0x8
			float nearPlaneDistance; // 0xC
			float farPlaneDistance; // 0x10
			unsigned int viewportWidth; // 0x14
			unsigned int viewportHeight; // 0x18

			CameraData() = delete;
			~CameraData() = delete;

			//
			// Other related this-call functions.
			//

			float getFOV() const;
			void setFOV(float degrees);

		};
		void* vTable; // 0x0
		NI::Pointer<NI::Renderer> renderer; // 0x4
		NI::Pointer<NI::Node> root; // 0x8
		NI::Pointer<NI::Node> cameraRoot; // 0xC
		CameraData cameraData; // 0x10

		WorldControllerRenderCamera() = delete;
		~WorldControllerRenderCamera() = delete;

		//
		// Custom functions.
		//

		NI::Camera* getCamera() const;

	};
	static_assert(sizeof(WorldControllerRenderCamera::VirtualTable) == 0x1C, "TES3::WorldControllerRenderCamera::VirtualTable failed size validation");
	static_assert(sizeof(WorldControllerRenderCamera::CameraData) == 0x1C, "TES3::WorldControllerRenderCamera::CameraData failed size validation");
	static_assert(sizeof(WorldControllerRenderCamera) == 0x2C, "TES3::WorldControllerRenderCamera failed size validation");

	struct WorldControllerRenderTarget : WorldControllerRenderCamera {
		NI::Pointer<NI::RenderedTexture> renderedTexture; // 0x2C
		NI::Pointer<NI::SourceTexture> readbackTexture; // 0x30
		int unknown_0x34;
		unsigned int rendererWidth; // 0x38
		unsigned int rendererHeight; // 0x3C
		unsigned int targetWidth; // 0x40
		unsigned int targetHeight; // 0x44
		int unknown_0x48;
		NI::Pointer<NI::ZBufferProperty> zBufferProperty; // 0x4C
		void* offscreenD3DSurface; // 0x50
		void* textureD3DSurface; // 0x54
		NI::Pointer<NI::Object> accumulator; // OffscreenSceneGraph::MasterPropertyAccumulator
		int unknown_0x5C;
		NI::Pointer<NI::DirectionalLight> directionalLight; // 0x60
		int unknown_0x64;
		NI::Pointer<NI::Object> unknown_0x68; // NiScreenPolygon
		bool readbackAlphaDataInitialized; // 0x6C
		char* readbackAlphaChannelData; // 0x70
		void* unknown_0x74;
		void* unknown_0x78;
		NI::Pointer<NI::AlphaProperty> alphaProperty; // 0x7C
		NI::Pointer<NI::VertexColorProperty> vertexColorProperty; // 0x80

		WorldControllerRenderTarget() = delete;
		~WorldControllerRenderTarget() = delete;
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

		MouseController() = delete;
		~MouseController() = delete;
	};
	static_assert(sizeof(MouseController) == 0x50, "TES3::MouseController failed size validation");

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

		KillCounter() = delete;
		~KillCounter() = delete;

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

		InventoryData() = delete;
		~InventoryData() = delete;

		//
		// Other related this-call functions.
		//

		_declspec(dllexport) void clearIcons(int type);
		_declspec(dllexport) void addInventoryItems(Inventory* inventory, int type);
		void refreshForReference(const Reference* reference, int type);
		_declspec(dllexport) UI::InventoryTile* findTile(const Item* item) const;
		_declspec(dllexport) UI::InventoryTile* findTile(Item* item, ItemData* itemData, int type) const;
		_declspec(dllexport) void mergeTile(UI::InventoryTile* tile);

	};
	static_assert(sizeof(InventoryData) == 0x24, "TES3::InventoryData failed size validation");

	struct Font {
		struct FontData {
			struct TextureData {
				int unknown_0;
				char filename[32];
			};
			struct GlyphData {
				int unknown_0;
				TES3::Vector2 topLeft, topRight, bottomLeft, bottomRight;
				float width, height, leftKerning, rightKerning, ascent;
			};

			float lineHeight; // 0x0
			int textureCount; // 0x4
			TextureData textures[8]; // 0x8
			GlyphData glyphs[256]; // 0x12C
		};

		short propertyCount; // 0x0
		const char* filePath; // 0x4
		NI::Property* texturingProperties[8]; // 0x8 // Up to 8 NiTexturingProperty pointers, filled count is stored by propertyCount.
		NI::Property* vertexColorProperty; // 0x28 // NiVertexColorProperty.
		float maxGlyphHeight; // 0x2C // Maximum possible height of a single line of text.
		float layoutCurrentYOffset; // 0x30 // Running Y offset from UI layout function.
		FontData* fontData; // 0x34 // The .fnt file contents.

		Font() = delete;
		~Font() = delete;

		void substituteTextMacros(const Actor* actor, const char* text) const;

		char* getSubstituteResult() const;
	};
	static_assert(sizeof(Font) == 0x38, "TES3::Font failed size validation");
	static_assert(sizeof(Font::FontData) == 0x3928, "TES3::Font::FontData failed size validation");

	struct JournalHTML {
		bool changedSinceLastSync; // 0x0
		char* data; // 0x4
		unsigned int dataBufferSize; // 0x8

		JournalHTML() = delete;
		~JournalHTML() = delete;

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

			ActiveSplash() = delete;
			~ActiveSplash() = delete;
		};
		int bloodMeshCount; // 0x0
		int bloodTextureCount; // 0x4
		NI::Pointer<NI::Node> bloodMeshes[4]; // 0x8
		float bloodSplashDurations[6]; // 0x18 // ActiveSplash::maxAge for the used mesh index offset by 1. First and last entries seem unused garbage...
		NI::Pointer<NI::SourceTexture> bloodTextures[8]; // 0x30
		NI::Pointer<NI::TexturingProperty> bloodTextureProperties[8]; // 0x50
		IteratedList<ActiveSplash*>* activeSplashes; // 0x70

		SplashController() = delete;
		~SplashController() = delete;

		std::reference_wrapper<NI::Pointer<NI::Node>[4]> getBloodMeshes();
		std::reference_wrapper<float[6]> getBloodSplashDurations();
		std::reference_wrapper<NI::Pointer<NI::SourceTexture>[8]> getBloodTextures();
		std::reference_wrapper<NI::Pointer<NI::TexturingProperty>[8]> getBloodTextureProperties();

	};
	static_assert(sizeof(SplashController) == 0x74, "TES3::SplashController failed size validation");
	static_assert(sizeof(SplashController::ActiveSplash) == 0xC, "TES3::SplashController::ActiveSplash failed size validation");

	struct RechargingItem {
		Object* item; // 0x0
		Enchantment* enchantment; // 0x4
		ItemData* itemData; // 0x8

		RechargingItem() = delete;
		~RechargingItem() = delete;
	};
	static_assert(sizeof(RechargingItem) == 0xC, "TES3::RechargingItem failed size validation");

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
		MobManager * mobManager; // 0x5C
		KillCounter * playerKills; // 0x60
		JournalHTML * journalHTML; // 0x64
		SplashController * splashController; // 0x68
		IteratedList<Quest*> * journalController; // 0x6C
		MagicInstanceController * magicInstanceController; // 0x70
		VFXManager * vfxManager; // 0x74
		int viewWidth; // 0x78
		int viewHeight; // 0x7C
		int bitDepth; // 0x80
		int bShadows; // 0x84
		float helpDelay; // 0x88
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
		bool showActorCollisionBoxes; // 0xD9
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
		IteratedList<MobileActor*> * allMobileActors; // 0x33C
		IteratedList<RechargingItem*> * rechargingItems; // 0x340
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


		WorldController() = delete;
		~WorldController() = delete;

		// Get singleton.
		_declspec (dllexport) static WorldController* get();

		//
		// Other related this-call functions.
		//

		void mainLoopBeforeInput();
		_declspec(dllexport) MobilePlayer* getMobilePlayer();
		_declspec(dllexport) void playItemUpDownSound(BaseObject* item, ItemSoundState state = ItemSoundState::Up, Reference* reference = nullptr);
		_declspec(dllexport) float getSimulationTimestamp();
		_declspec(dllexport) void processGlobalScripts();
		_declspec(dllexport) void startGlobalScript(Script* script, Reference* reference = nullptr);
		void startGlobalScriptBySourceID(Script* script, unsigned int sourceID);
		_declspec(dllexport) void stopGlobalScript(Script* script);
		_declspec(dllexport) bool isGlobalScriptRunning(const Script* script) const;

		_declspec(dllexport) unsigned short getDaysInMonth(int);
		_declspec(dllexport) unsigned short getCumulativeDaysForMonth(int month);
		_declspec(dllexport) const char* getNameForMonth(int month);
		_declspec(dllexport) double getHighPrecisionSimulationTimestamp();

		_declspec(dllexport) bool applyEnchantEffect(NI::Node* node, Enchantment* enchantment);

		_declspec(dllexport) void updateTiming();
		_declspec(dllexport) void updateEnvironmentLightingWeather();

		float getAIDistance() const;
		void setAIDistance(float value);
		float getAIDistanceScale() const;
		void setAIDistanceScale(float scale);

		void rechargerAddItem(Object* item, ItemData* itemData, Enchantment* enchantment);
		bool getLevitationDisabled() const;
		void setLevitationDisabled(bool disable);

		// This puts the next music track into the game's thread-safe string buffer.
		bool selectNextMusicTrack(MusicSituation situation) const;

		//
		// Custom functions.
		//

		int getShadowLevel() const;
		void setShadowLevel(int shadows);
		_declspec(dllexport) void tickClock();
		_declspec(dllexport) void checkForDayWrapping();
		bool isChargenStarted() const;
		bool isChargenRunning() const;
		bool isChargenFinished() const;

		//
		// Helpful static variables.
		//

		class ItemUpDownSoundBlocker {
		public:
			ItemUpDownSoundBlocker() {
				previousValue = blockItemUpDownSound;
			}
			ItemUpDownSoundBlocker(bool playSound) : ItemUpDownSoundBlocker() {
				blockItemUpDownSound = playSound;
			}
			~ItemUpDownSoundBlocker() {
				blockItemUpDownSound = previousValue;
			}
		private:
			bool previousValue;
		};

		static float realDeltaTime;
		static float simulationTimeScalar;
		static bool blockItemUpDownSound;
	};
	static_assert(sizeof(WorldController) == 0x374, "TES3::WorldController failed size validation");
	static_assert(offsetof(WorldController, inputController) == 0x4C, "TES3::WorldController failed offset validation");
	static_assert(offsetof(WorldController, mobManager) == 0x5C, "TES3::WorldController failed offset validation");
	static_assert(offsetof(WorldController, gvarGameHour) == 0xA8, "TES3::WorldController failed offset validation");
	static_assert(offsetof(WorldController, flagMenuMode) == 0xD6, "TES3::WorldController failed offset validation");
	static_assert(offsetof(WorldController, menuController) == 0x2E4, "TES3::WorldController failed offset validation");
	static_assert(offsetof(WorldController, globalScripts) == 0x338, "TES3::WorldController failed offset validation");
	static_assert(offsetof(WorldController, menuController) == 0x2E4, "TES3::WorldController failed offset validation");
}
