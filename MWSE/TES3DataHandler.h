#pragma once

#include "TES3Defines.h"

#include "TES3CriticalSection.h"
#include "TES3HashMap.h"
#include "TES3MagicEffect.h"
#include "TES3Skill.h"
#include "TES3StlList.h"

#include "TES3AnimationData.h"

#include "NIAVObject.h"
#include "NISourceTexture.h"

#define MWSE_CUSTOM_EFFECTS true
#define MWSE_RAISED_FILE_LIMIT true
#define MWSE_CUSTOM_GLOBALS true

namespace TES3 {
	enum class LoadGameResult {
		Failure = 0x0,
		Success = 0x1,
		Block = 0x2
	};
	enum class BaseAnimationIndex : int {
		Male,
		MaleFirstPerson,
		Female,
		FemaleFirstPerson,
		COUNT
	};

	struct MeshData {
		HashMap<const char*, NI::Node*>* NIFs; // 0x0
		HashMap<const char*, KeyframeDefinition*>* KFs; // 0x4

		// Path is relative to Data Files.
		NI::AVObject* loadMesh(const char* path);
		KeyframeDefinition* loadKeyFrame(const char* path, const char* animation);
	};

	template <typename OT>
	struct ObjectMapContainer {
		HashMap<const char*, OT*>* map;
	};

#if MWSE_CUSTOM_GLOBALS
	struct GlobalHashContainer {
		struct icomp {
			bool operator() (const std::string_view& lhs, const std::string_view& rhs) const {
				return _strnicmp(lhs.data(), rhs.data(), 32) < 0;
			}
		};

		IteratedList<GlobalVariable*> variables;
		std::map<std::string_view, GlobalVariable*, icomp> cache;

		//
		// Custom functions.
		//

		GlobalVariable* getVariable(const char* id) const;
		void addVariable(GlobalVariable* value);
		void addVariableCacheOnly(GlobalVariable* value);

	};
#endif

	struct NonDynamicData {
		int activeModCount; // 0x0
		long unknown_0x04; // always 0?
		GameFile* unknown_0x08; // Points to info about the last loaded save?
		LinkedObjectList<Object> * list; // 0x0C
		LinkedObjectList<Spell> * spellsList; // 0x10
		MeshData * meshData; // 0x14
		GameSetting ** GMSTs; // 0x18 // Pointer to array of GMST pointers.
		IteratedList<Race*> * races; // 0x1C
		IteratedList<LandTexture*> * landTextures; // 0x20
		IteratedList<Class*> * classes; // 0x24
		IteratedList<Faction*> * factions; // 0x28
		IteratedList<Script*> * scripts; // 0x2C
		IteratedList<Sound*> * sounds; // 0x30
		IteratedList<SoundGenerator*> * soundGenerators; // 0x34
#if MWSE_CUSTOM_GLOBALS
		GlobalHashContainer* globals; // 0x38
#else
		IteratedList<GlobalVariable*>* globals; // 0x38
#endif
		IteratedList<Dialogue*> * dialogues; // 0x3C
		IteratedList<Region*> * regions; // 0x40
		IteratedList<Birthsign*> * birthsigns; // 0x44
		IteratedList<StartScript*> * startScripts; // 0x48
		Skill skills[27]; // 0x4C
#if MWSE_CUSTOM_EFFECTS
		MagicEffectController * magicEffects; // 0x5C8  
#if MWSE_RAISED_FILE_LIMIT
		GameFile* activeMods[1024]; // 0x5CC
		unsigned char freed_0x5CC[0x87EC]; // Unused space free for plundering.
#else 
		unsigned char freed_0x5CC[0x97EC]; // Unused space free for plundering.
#endif
#else
		MagicEffect magicEffects[143]; // 0x5C8
#endif
		StlList<Light*>* lights; // 0x9DB8
		AnimationGroup* baseAnimationGroups[4][150]; // 0x9DBC
		NI::Pointer<NI::Node> baseSkeletons[4]; // 0xA71C
		KeyframeDefinition* baseAnimations[4]; // 0xA72C
		AnimationGroup* baseBeastAnimationGroups[3][150]; // 0xA73C
		NI::Pointer<NI::Node> baseBeastSkeletons[3]; // 0x0xAE44
		KeyframeDefinition* baseBeastAnimations[3]; // 0xAE50
		NI::Pointer<NI::Property> collisionWireframeProperty; // 0xAE5C
		StlList<GameFile*>* TESFiles; // 0xAE60 
#ifdef MWSE_RAISED_FILE_LIMIT
		unsigned char freed_0xAE64[0x400]; // Unused space free for plundering.
#else
		GameFile* activeMods[256]; // 0xAE64 // Relocated and resized at 0x5CC.
#endif
		StlList<Cell*> * cells; // 0xB264
		ObjectMapContainer<BaseObject>* allObjectsById; // 0xB268
		ObjectMapContainer<Dialogue>* allDialoguesById; // 0xB26C
		char dataFilesPath[260]; // 0xB270
		char unknown_0xB374;
		bool isSaving; // 0xB375
		bool isModifyingMasters; // 0xB376
		char unknown_0xB377;
		char unknown_0xB378;
		char unknown_0xB379;
		char unknown_0xB37A;
		IteratedList<BaseObject*>* initiallyLoadedObjects; // 0xB37C
		NI::Pointer<NI::SourceTexture> mapTexture; // 0xB380
		Reference * playerSaveGame; // 0xB384
		CriticalSection criticalSection; // 0xB388

		//
		// Other related this-call functions.
		//

		bool saveGame(const char* fileName, const char* saveName);
		LoadGameResult loadGame(const char* fileName);
		LoadGameResult loadGameMainMenu(const char* fileName);

		BaseObject* resolveObject(const char*);
		Reference* findFirstCloneOfActor(const char*);
		Spell* getSpellById(const char*);
		Script* findScriptByName(const char*);
		GlobalVariable* findGlobalVariable(const char*);
		Dialogue* findDialogue(const char*);
		bool addSound(Sound*);
		Sound* findSound(const char*);
		Class* findClass(const char*);
		Faction* findFaction(const char*);
		Reference* findClosestExteriorReferenceOfObject(PhysicalObject* object, Vector3* position, bool searchForExteriorDoorMarker = false, int ignored = -1);
		bool addNewObject(BaseObject*);
		void deleteObject(BaseObject*);
		void respawnContainers();

		Cell * getCellByGrid(int x, int y);
		Cell * getCellByName(const char* name);
		Region* getRegion(const char* id);

		MagicEffect * getMagicEffect(int id);

		float createReference(PhysicalObject * object, Vector3 * position, Vector3 * orientation, bool& cellWasCreated, Reference * existingReference = nullptr, Cell * cell = nullptr);

		void showLocationOnMap(const char* name);
		void drawCellMapMarker(Cell* cell, int unused = 0);

		const char* getBaseAnimationFile(int isFemale = 0, int firstPerson = 0) const;

		//
		// Custom functions.
		//

		Alchemy* getMatchingAlchemyItem(const Alchemy*) const;

		std::reference_wrapper<Skill[27]> getSkills();

		sol::table getMagicEffects_lua(sol::this_state ts);

		// Wrapper around resolveObject that enforces type.
		template <typename T>
		T * resolveObjectByType(const char* id, ObjectType::ObjectType type = ObjectType::Invalid) {
			TES3::BaseObject* potentialResult = resolveObject(id);
			if (!potentialResult) {
				return nullptr;
			}
			else if (type != 0 && potentialResult->objectType != type) {
				return nullptr;
			}

			return static_cast<T*>(potentialResult);
		}

		template <typename T>
		T* resolveObjectByType(const std::string& id, ObjectType::ObjectType type = ObjectType::Invalid) {
			return resolveObjectByType<T>(id.c_str(), type);
		}
	};
	static_assert(sizeof(NonDynamicData) == 0xB3AC, "TES3::NonDynamicData failed size validation");
	static_assert(offsetof(NonDynamicData, spellsList) == 0x10, "TES3::NonDynamicData failed offset validation");
	static_assert(offsetof(NonDynamicData, GMSTs) == 0x18, "TES3::NonDynamicData failed offset validation");
	static_assert(offsetof(NonDynamicData, skills) == 0x4C, "TES3::NonDynamicData failed offset validation");
	static_assert(offsetof(NonDynamicData, magicEffects) == 0x5C8, "TES3::NonDynamicData failed offset validation");
	static_assert(offsetof(NonDynamicData, TESFiles) == 0xAE60, "TES3::NonDynamicData failed offset validation");

	struct SoundEvent {
		Reference* reference;
		Sound* sound;
		SoundBuffer* soundBuffer;
		unsigned char unknown_0xC;
	};
	static_assert(sizeof(SoundEvent) == 0x10, "TES3::SoundEvent failed size validation");

	namespace CellGrid {
		enum CellGrid {
			NorthWest = 0,
			North = 1,
			NorthEast = 2,
			West = 3,
			Center = 4,
			East = 5,
			SouthWest = 6,
			South = 7,
			SouthEast = 8
		};
	}

	struct DataHandler {
		struct ExteriorCellData {
			unsigned char loadingFlags;
			Cell * cell;
			void * landRenderData;
		};

		NonDynamicData * nonDynamicData; // 0x0
		ExteriorCellData* exteriorCellData[9]; // 0x4
		ExteriorCellData* backgroundLoadExteriorCellData[25]; // 0x28
		NI::Node * worldObjectRoot; // 0x8C
		NI::Node * worldPickObjectRoot; // 0x90
		NI::Node * worldLandscapeRoot; // 0x94
		NI::DirectionalLight * sgSunlight;
		NI::FogProperty * sgFogProperty;
		int centralGridX; // 0xA0
		int centralGridY; // 0xA4
		bool cellChanged; // 0xA8
		char padding_0xA9[3];
		Cell * currentInteriorCell; // 0xAC
		Cell ** interiorCellBuffer; // 0xB0
		Cell ** exteriorCellBuffer; // 0xB4
		int lastExteriorCellPositionX; // 0xB8
		int lastExteriorCellPositionY; // 0xBC
		IteratedList<Reference*> collisionReferenceGrid[48][48]; // 0xC0
		int collision_0xB4C0;
		int collision_0xB4C4;
		int collision_0xB4C8;
		int collision_0xB4CC;
		IteratedList<SoundEvent*>* soundEvents; // 0xB4D0
		IteratedList<SoundEvent*>* tempSoundEvents; // 0xB4D4
		IteratedList<SoundEvent*>* lightSoundEvents; // 0xB4D8
		bool showActorDrawBounds; // 0xB4DC
		char unknown_0xB4DD;
		char unknown_0xB4DE;
		char unknown_0xB4DF;
		char unknown_0xB4E0;
		char unknown_0xB4E1;
		char unknown_0xB4E2;
		char unknown_0xB4E3;
		char unknown_0xB4E4;
		char unknown_0xB4E5;
		char unknown_0xB4E6;
		char unknown_0xB4E7;
		HashMap<const char*, NI::Pointer<NI::SourceTexture>>* textures; // 0xB4E8
		void * waterController;
		int unknown_0xB4F0;
		int unknown_0xB4F4;
		char unknown_0xB4F8;
		char unknown_0xB4F9;
		char unknown_0xB4FA;
		char unknown_0xB4FB;
		int backgroundThreadID; // 0xB4FC
		int mainThreadID; // 0xB500
		int backgroundThread; // 0xB504
		int mainThread; // 0xB508
		char unknown_0xB50C;
		char unknown_0xB50D;
		char unknown_0xB50E;
		char unknown_0xB50F;
		char unknown_0xB510;
		bool backgroundThreadRunning; // 0xB511
		char unknown_0xB512;
		char unknown_0xB513;
		int unknown_0xB514;
		char unknown_0xB518;
		char unknown_0xB519;
		char unknown_0xB51A;
		char unknown_0xB51B;
		int threadSleepTime; // 0xB51C
		int unknown_0xB520; // Time interval?
		char unknown_0xB524;
		char unknown_0xB525;
		char unknown_0xB526;
		char unknown_0xB527;
		float unknown_0xB528;
		float unknown_0xB52C;
		char unknown_0xB530;
		char unknown_0xB531;
		char unknown_0xB532;
		char unknown_0xB533;
		CriticalSection* criticalSectionAudioEvents; // 0xB534
		CriticalSection* criticalSection; // 0xB538
		bool useCellTransitionFader;
		char unknown_0xB53D;
		char unknown_0xB53E;
		char unknown_0xB53F;
		Cell * currentCell; // 0xB540
		Cell * lastExteriorCell; // 0xB544
		Sound * soundRegionalRandomSfx; // 0xB548
		Sound * currentAmbientWaterSound; // 0xB54C
		int exteriorCellDataBufferSize; // 0xB550
		void * exteriorCellDataBuffer; // 0xB554

		//
		// Custom static data.
		//

		static Cell* previousVisitedCell;
		static bool dontThreadLoad;
		static bool suppressThreadLoad;

		// Get singleton.
		_declspec (dllexport) static DataHandler * get();

		//
		// Other related this-call functions.
		//

		Vector3 getLastExteriorPosition();

		void addSound(Sound* sound, Reference* reference = nullptr, int playbackFlags = 0, unsigned char volume = 250, float pitch = 1.0f, bool isVoiceover = false, int unknown = 0);
		Sound* addSoundById(const char* soundId, Reference* reference = 0, int playbackFlags = 0, unsigned char volume = 250, float pitch = 1.0f, int unknown = 0);
		void addTemporySound(const char* path, Reference * reference = nullptr, int playbackFlags = 0, int volume = 250, float pitch = 1.0f, bool isVoiceover = false, Sound * sound = nullptr);
		SoundEvent* getSoundPlaying(Sound*, Reference*);
		void adjustSoundVolume(Sound*, Reference*, unsigned char volume);
		void removeSound(Sound*, Reference*);

		NI::Pointer<NI::SourceTexture> loadSourceTexture(const char* path);

		void updateLightingForReference(Reference * reference);
		void setDynamicLightingForReference(Reference* reference);

		void updateCollisionGroupsForActiveCells(bool force = true, bool isResettingData = false, bool resetCollisionGroups = true);
		void updateCollisionGroupsForActiveCells_raw(bool force = true);

		//
		// Custom functions.
		//

		std::reference_wrapper<ExteriorCellData* [9]> getExteriorCellData_lua();

	};
	static_assert(sizeof(DataHandler) == 0xB558, "TES3::DataHandler failed size validation");
	static_assert(offsetof(DataHandler, worldObjectRoot) == 0x8C, "TES3::DataHandler failed offset validation");
	static_assert(offsetof(DataHandler, cellChanged) == 0xA8, "TES3::DataHandler failed offset validation");
	static_assert(offsetof(DataHandler, currentInteriorCell) == 0xAC, "TES3::DataHandler failed offset validation");
	static_assert(offsetof(DataHandler, soundEvents) == 0xB4D0, "TES3::DataHandler failed offset validation");
	static_assert(offsetof(DataHandler, backgroundThreadID) == 0xB4FC, "TES3::DataHandler failed offset validation");
	static_assert(offsetof(DataHandler, currentCell) == 0xB540, "TES3::DataHandler failed offset validation");
	static_assert(sizeof(DataHandler::ExteriorCellData) == 0xC, "TES3::DataHandler::ExteriorCellData failed size validation");
}
