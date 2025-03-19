#include "TES3DataHandler.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "LuaAddSoundEvent.h"
#include "LuaAddTempSoundEvent.h"
#include "LuaKeyframesLoadEvent.h"
#include "LuaKeyframesLoadedEvent.h"
#include "LuaLoadedGameEvent.h"
#include "LuaLoadGameEvent.h"
#include "LuaMeshLoadedEvent.h"
#include "LuaMeshLoadEvent.h"
#include "LuaSavedGameEvent.h"
#include "LuaSaveGameEvent.h"

#include "TES3Util.h"

#include "TES3Actor.h"
#include "TES3Alchemy.h"
#include "TES3Cell.h"
#include "TES3DialogueInfo.h"
#include "TES3GlobalVariable.h"
#include "TES3GameSetting.h"
#include "TES3MagicEffectController.h"
#include "TES3MobilePlayer.h"
#include "TES3Reference.h"
#include "TES3Sound.h"
#include "TES3Spell.h"
#include "TES3UIManager.h"
#include "TES3WorldController.h"

#include "MWSEConfig.h"

namespace TES3 {

	Cell* DataHandler::previousVisitedCell = nullptr;
	bool DataHandler::dontThreadLoad = false;
	bool DataHandler::suppressThreadLoad = false;
	std::unordered_map<DWORD, std::string_view> DataHandler::currentlyLoadingMeshes = {};
	std::recursive_mutex DataHandler::currentlyLoadingMeshesMutex = {};

	std::string_view pushLoadingMesh(const std::string_view path) {
		const auto threadId = GetCurrentThreadId();

		DataHandler::currentlyLoadingMeshesMutex.lock();

		std::string_view previousMesh;
		const auto existing = DataHandler::currentlyLoadingMeshes.find(threadId);
		if (existing != DataHandler::currentlyLoadingMeshes.end()) {
			previousMesh = existing->second;
			DataHandler::currentlyLoadingMeshes.erase(existing);
		}

		if (!path.empty()) {
			DataHandler::currentlyLoadingMeshes[threadId] = path;
		}

		DataHandler::currentlyLoadingMeshesMutex.unlock();

		return previousMesh;
	}

	//
	// MeshData
	//

	const auto TES3_MeshData_loadMesh = reinterpret_cast<NI::Node * (__thiscall*)(MeshData*, const char*)>(0x4EE0A0);
	NI::Node* MeshData::loadMesh(const char* rawPath) {
		// Allow changing the desired mesh path.
		std::string meshPath = rawPath;
		if (mwse::lua::event::MeshLoadEvent::getEventEnabled()) {
			auto handle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			sol::table response = handle.triggerEvent(new mwse::lua::event::MeshLoadEvent(rawPath));
			if (response.valid()) {
				meshPath = response.get_or("path", rawPath);
			}
		}

		// Store the loading path for debugging purposes.
		auto previouslyLoadingMesh = pushLoadingMesh(meshPath);

		// Check the loaded NIF count to see if anything new was loaded.
		auto countBefore = NIFs->count;

		// Actually load the mesh.
		auto mesh = TES3_MeshData_loadMesh(this, meshPath.c_str());

		// If the loaded mesh count increased, send off an event to 
		if (mesh && NIFs->count > countBefore && mwse::lua::event::MeshLoadedEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::MeshLoadedEvent(meshPath.c_str(), mesh));
		}

		// Clean up debug information.
		pushLoadingMesh(previouslyLoadingMesh);

		return mesh;
	}

	struct LoadTempMeshNode {
		const char* path;
		NI::Pointer<NI::Node> mesh;
		LoadTempMeshNode* nextNode;

		LoadTempMeshNode(const char* path) {
			const auto ctor = reinterpret_cast<void(__thiscall*)(LoadTempMeshNode*, const char*)>(0x4ED6C0);
			ctor(this, path);
		}

		~LoadTempMeshNode() {
			const auto dtor = reinterpret_cast<void(__thiscall*)(LoadTempMeshNode*)>(0x4EDB70);
			dtor(this);
		}
	};
	static_assert(sizeof(LoadTempMeshNode) == sizeof(TES3::HashMap<char*, NI::Pointer<NI::AVObject>>::Node), "Temp mesh load node size mismatch!");

	NI::Pointer<NI::Node> MeshData::loadMeshUncached(const char* rawPath) {
		// Allow changing the desired mesh path.
		std::string meshPath = rawPath;
		if (mwse::lua::event::MeshLoadEvent::getEventEnabled()) {
			auto handle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			sol::table response = handle.triggerEvent(new mwse::lua::event::MeshLoadEvent(rawPath));
			if (response.valid()) {
				meshPath = response.get_or("path", rawPath);
			}
		}

		// Store the loading path for debugging purposes.
		auto previouslyLoadingMesh = pushLoadingMesh(meshPath);

		// Actually load the mesh.
		auto mesh = LoadTempMeshNode(meshPath.c_str()).mesh;

		// If the loaded mesh count increased, send off an event to 
		if (mesh && mwse::lua::event::MeshLoadedEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::MeshLoadedEvent(meshPath.c_str(), mesh));
		}

		// Clean up debug information.
		pushLoadingMesh(previouslyLoadingMesh);

		return mesh;
	}

	const auto TES3_MeshData_loadKeyframes = reinterpret_cast<KeyframeDefinition * (__thiscall*)(MeshData*, const char*, const char*)>(0x4EE200);
	KeyframeDefinition* MeshData::loadKeyframes(const char* path, const char* sequenceName) {
		// Allow changing the desired mesh path.
		std::string keyframesPath = path;
		std::string sequenceString = sequenceName;

		if (mwse::lua::event::KeyframesLoadEvent::getEventEnabled()) {
			auto handle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			sol::table response = handle.triggerEvent(new mwse::lua::event::KeyframesLoadEvent(path, sequenceName));
			if (response.valid()) {
				keyframesPath = response.get_or("path", path);
				sequenceString = response.get_or("sequenceName", path);
			}
		}

		path = keyframesPath.c_str();
		sequenceName = sequenceString.c_str();

		auto keyframeDefinition = TES3_MeshData_loadKeyframes(this, path, sequenceName);
		if (keyframeDefinition && mwse::lua::event::KeyframesLoadedEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(
				new mwse::lua::event::KeyframesLoadedEvent(path, sequenceName, keyframeDefinition)
			);
		}

		return keyframeDefinition;
	}

	//
	// GlobalHashContainer
	//

#if MWSE_CUSTOM_GLOBALS
	GlobalVariable* GlobalHashContainer::getVariable(const char* id) const {
		auto itt = cache.find(id);
		if (itt == cache.end()) {
			return nullptr;
		}
		return itt->second;
	}

	void GlobalHashContainer::addVariable(GlobalVariable* value) {
		variables.push_back(value);
		cache[value->name] = value;
	}

	void GlobalHashContainer::addVariableCacheOnly(GlobalVariable* value) {
		cache[value->name] = value;
	}
#endif

	//
	// NonDynamicData
	//

	const auto TES3_NonDynamicData_saveGame = reinterpret_cast<signed char(__thiscall*)(NonDynamicData*, const char*, const char*)>(0x4C4250);
	bool NonDynamicData::saveGame(const char* fileName, const char* saveName) {
		std::string eventFileName = fileName ? fileName : "";
		std::string eventSaveName = saveName;

		// Execute event. If the event blocked the call, bail.
		mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
		if (mwse::lua::event::SaveGameEvent::getEventEnabled()) {
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::table eventData = stateHandle.triggerEvent(new mwse::lua::event::SaveGameEvent(saveName, fileName));
			if (eventData.valid() && eventData.get_or("block", false)) {
				return true;
			}

			// Fetch the names back from the event data, in case the event changed them.
			eventFileName = eventData["filename"];
			eventSaveName = eventData["name"];
		}

		luaManager.savePersistentTimers();

		bool saved = TES3_NonDynamicData_saveGame(this, eventFileName.c_str(), eventSaveName.c_str());

		luaManager.clearPersistentTimers();

		// Pass a follow-up event if we successfully saved.
		if (saved && mwse::lua::event::SavedGameEvent::getEventEnabled()) {
			luaManager.getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::SavedGameEvent(eventSaveName.c_str(), eventFileName.c_str()));
		}

		return saved;
	}

	const auto TES3_NonDynamicData_loadGameInGame = reinterpret_cast<bool(__thiscall*)(NonDynamicData*, const char*)>(0x4C4800);
	LoadGameResult NonDynamicData::loadGame(const char* fileName) {
		// Execute event. If the event blocked the call, bail.
		mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
		std::string eventFileName = fileName ? fileName : "";

		if (mwse::lua::event::LoadGameEvent::getEventEnabled()) {
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::table eventData = stateHandle.triggerEvent(new mwse::lua::event::LoadGameEvent(fileName));
			if (eventData.valid() && eventData.get_or("block", false)) {
				return LoadGameResult::Block;
			}

			// Fetch the names back from the event data, in case the event changed them.
			// Also add back in the .ess extension.
			eventFileName = eventData["filename"];
			eventFileName += ".ess";
		}

		// Bugfix: Preserve lowestZInCurrentCell when reloading a save in an interior cell.
		// The engine resets it to -FLT_MAX instead of recalculating it if the player is in the same cell before and after loading.
		float* pLowestZInCurrentCell = reinterpret_cast<float*>(0x7B217C);
		float previousLowestZInCurrentCell = *pLowestZInCurrentCell;

		bool loaded = TES3_NonDynamicData_loadGameInGame(this, eventFileName.c_str());

		// Bugfix: In interior, restore previous lowestZInCurrentCell if has been reset.
		if (loaded && TES3::DataHandler::get()->currentInteriorCell != nullptr) {
			if (*pLowestZInCurrentCell == -FLT_MAX) {
				*pLowestZInCurrentCell = previousLowestZInCurrentCell;
			}
		}

		// Pass a follow-up event if we successfully loaded and clear timers.
		if (loaded) {
			DataHandler::previousVisitedCell = nullptr;
			luaManager.clearTimers();
			luaManager.restorePersistentTimers();

			if (mwse::lua::event::LoadedGameEvent::getEventEnabled()) {
				luaManager.getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::LoadedGameEvent(eventFileName.c_str(), fileName == NULL));
			}

			// Extra things we want to do if we're successfully loading.
			TES3::UI::setSuppressingHelpMenu(false);
		}

		// Update compatibility globals.
		const auto mwseBuildGlobal = TES3::DataHandler::get()->nonDynamicData->findGlobalVariable("MWSE_BUILD");
		if (mwseBuildGlobal) {
			mwseBuildGlobal->value = mwse::Configuration::BuildNumber;
		}

		return loaded ? LoadGameResult::Success : LoadGameResult::Failure;
	}

	const auto TES3_NonDynamicData_loadGameMainMenu = reinterpret_cast<bool(__thiscall*)(NonDynamicData*, const char*)>(0x4C4EB0);
	LoadGameResult NonDynamicData::loadGameMainMenu(const char* fileName) {
		// Execute event. If the event blocked the call, bail.
		mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
		std::string eventFileName = fileName ? fileName : "";

		if (mwse::lua::event::LoadGameEvent::getEventEnabled()) {
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::table eventData = stateHandle.triggerEvent(new mwse::lua::event::LoadGameEvent(fileName));
			if (eventData.valid() && eventData.get_or("block", false)) {
				return LoadGameResult::Block;
			}

			// Fetch the names back from the event data, in case the event changed them.
			// Also add back in the .ess extension.
			eventFileName = eventData["filename"];
			eventFileName += ".ess";
		}

		bool loaded = TES3_NonDynamicData_loadGameMainMenu(this, eventFileName.c_str());

		// Pass a follow-up event if we successfully loaded and clear timers.
		if (loaded) {
			DataHandler::previousVisitedCell = nullptr;
			luaManager.clearTimers();
			luaManager.restorePersistentTimers();

			if (mwse::lua::event::LoadedGameEvent::getEventEnabled()) {
				luaManager.getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::LoadedGameEvent(eventFileName.c_str()));
			}

			// Extra things we want to do if we're successfully loading.
			TES3::UI::setSuppressingHelpMenu(false);
		}

		return loaded ? LoadGameResult::Success : LoadGameResult::Failure;
	}

	const auto TES3_NonDynamicData_resolveObject = reinterpret_cast<BaseObject * (__thiscall*)(NonDynamicData*, const char*)>(0x4B8B60);
	BaseObject* NonDynamicData::resolveObject(const char* id) {
		return TES3_NonDynamicData_resolveObject(this, id);
	}

	const auto TES3_NonDynamicData_findFirstCloneOfActor = reinterpret_cast<Reference * (__thiscall*)(NonDynamicData*, const char*)>(0x4B8F50);
	Reference* NonDynamicData::findFirstCloneOfActor(const char* baseId) {
		return TES3_NonDynamicData_findFirstCloneOfActor(this, baseId);
	}

	const auto TES3_NonDynamicData_resolveReferenceBySourceID = reinterpret_cast<Reference * (__thiscall*)(NonDynamicData*, unsigned int)>(0x4B9180);
	Reference* NonDynamicData::resolveReferenceBySourceID(unsigned int id) {
		return TES3_NonDynamicData_resolveReferenceBySourceID(this, id);
	}

	Spell* NonDynamicData::getSpellById(const char* id) {
		for (const auto spell : *spellsList) {
			if (_stricmp(id, spell->objectID) == 0) {
				return spell;
			}
		}
		return nullptr;
	}

	const auto TES3_NonDynamicData_findScriptByName = reinterpret_cast<Script * (__thiscall*)(NonDynamicData*, const char*)>(0x4BA700);
	Script* NonDynamicData::findScriptByName(const char* name) {
		return TES3_NonDynamicData_findScriptByName(this, name);
	}

	const auto TES3_NonDynamicData_findGlobalVariable = reinterpret_cast<GlobalVariable * (__thiscall*)(NonDynamicData*, const char*)>(0x4BA820);
	GlobalVariable* NonDynamicData::findGlobalVariable(const char* name) {
#if MWSE_CUSTOM_GLOBALS
		return globals->getVariable(name);
#else
		return TES3_NonDynamicData_findGlobalVariable(this, name);
#endif
	}

	const auto TES3_NonDynamicData_findDialogue = reinterpret_cast<Dialogue * (__thiscall*)(NonDynamicData*, const char*)>(0x4BA8D0);
	Dialogue* NonDynamicData::findDialogue(const char* name) {
		return TES3_NonDynamicData_findDialogue(this, name);
	}

	bool NonDynamicData::addSound(Sound* sound) {
		if (findSound(sound->getObjectID())) {
			return false;
		}

		sounds->push_back(sound);
		return true;
	}

	const auto TES3_NonDynamicData_findSound = reinterpret_cast<Sound * (__thiscall*)(NonDynamicData*, const char*)>(0x4BA7A0);
	Sound* NonDynamicData::findSound(const char* id) {
		return TES3_NonDynamicData_findSound(this, id);
	}

	const auto TES3_NonDynamicData_findClass = reinterpret_cast<Class * (__thiscall*)(NonDynamicData*, const char*)>(0x4BA6B0);
	Class* NonDynamicData::findClass(const char* id) {
		return TES3_NonDynamicData_findClass(this, id);
	}

	const auto TES3_NonDynamicData_findRace = reinterpret_cast<Race * (__thiscall*)(NonDynamicData*, const char*)>(0x4BA5C0);
	Race* NonDynamicData::findRace(const char* id) {
		return TES3_NonDynamicData_findRace(this, id);
	}

	const auto TES3_NonDynamicData_findFaction = reinterpret_cast<Faction * (__thiscall*)(NonDynamicData*, const char*)>(0x4BA750);
	Faction* NonDynamicData::findFaction(const char* id) {
		return TES3_NonDynamicData_findFaction(this, id);
	}

	const auto TES3_NonDynamicData_findClosestExteriorReferenceOfObject = reinterpret_cast<Reference * (__thiscall*)(NonDynamicData*, PhysicalObject*, Vector3*, bool, int)>(0x4B96F0);
	Reference* NonDynamicData::findClosestExteriorReferenceOfObject(PhysicalObject* object, Vector3* position, bool searchForExteriorDoorMarker, int ignored) {
		return TES3_NonDynamicData_findClosestExteriorReferenceOfObject(this, object, position, searchForExteriorDoorMarker, ignored);
	}

	const auto TES3_NonDynamicData_addNewObject = reinterpret_cast<signed char(__thiscall*)(NonDynamicData*, BaseObject*)>(0x4B8980);
	bool NonDynamicData::addNewObject(BaseObject* object) {
		return TES3_NonDynamicData_addNewObject(this, object);
	}

	const auto TES3_NonDynamicData_deleteObject = reinterpret_cast<void(__thiscall*)(NonDynamicData*, BaseObject*)>(0x4B8B20);
	void NonDynamicData::deleteObject(BaseObject* object) {
		TES3_NonDynamicData_deleteObject(this, object);
	}

	const auto TES3_NonDynamicData_respawnContainers = reinterpret_cast<void(__thiscall*)(NonDynamicData*)>(0x4C3B00);
	void NonDynamicData::respawnContainers() {
		TES3_NonDynamicData_respawnContainers(this);
	}

	const auto TES3_NonDynamicData_getCellByGrid = reinterpret_cast<Cell * (__thiscall*)(NonDynamicData*, int, int)>(0x4BAA10);
	Cell* NonDynamicData::getCellByGrid(int x, int y) {
		return TES3_NonDynamicData_getCellByGrid(this, x, y);
	}

	Cell* NonDynamicData::getCellByPosition(float x, float y) {
		int cellX = Cell::toGridCoord(x);
		int cellY = Cell::toGridCoord(y);
		return getCellByGrid(cellX, cellY);
	}

	const auto TES3_NonDynamicData_getCellByName = reinterpret_cast<Cell * (__thiscall*)(NonDynamicData*, const char*)>(0x4BA9B0);
	Cell* NonDynamicData::getCellByName(const char* name) {
		return TES3_NonDynamicData_getCellByName(this, name);
	}

	const auto TES3_NonDynamicData_getRegionById = reinterpret_cast<Region * (__thiscall*)(NonDynamicData*, const char*)>(0x4BA610);
	Region* NonDynamicData::getRegion(const char* id) {
		return TES3_NonDynamicData_getRegionById(this, id);
	}

	MagicEffect* NonDynamicData::getMagicEffect(int id) {
		return magicEffects->getEffectObject(id);
	}

	const auto TES3_NonDynamicData_createReference = reinterpret_cast<float(__thiscall*)(NonDynamicData*, PhysicalObject*, Vector3*, Vector3*, bool&, Reference*, Cell*)>(0x4C0E80);
	float NonDynamicData::createReference(PhysicalObject* object, Vector3* position, Vector3* orientation, bool& cellWasCreated, Reference* existingReference, Cell* cell) {
		return TES3_NonDynamicData_createReference(this, object, position, orientation, cellWasCreated, existingReference, cell);
	}

	const auto TES3_NonDynamicData_showLocationOnMap = reinterpret_cast<void(__thiscall*)(TES3::NonDynamicData*, const char*)>(0x4C8480);
	void NonDynamicData::showLocationOnMap(const char* name) {
		auto idLength = strnlen_s(name, 32);
		for (auto cell : *cells) {
			if (cell->name && !cell->getIsInterior() && !cell->getIsMarkerDrawn()) {
				if (_strnicmp(cell->name, name, idLength) == 0) {
					drawCellMapMarker(cell);
				}
			}
		}
	}

	const auto TES3_NonDynamicData_drawCellMapMarker = reinterpret_cast<void(__thiscall*)(TES3::NonDynamicData*, TES3::Cell*, int)>(0x4C8540);
	void NonDynamicData::drawCellMapMarker(Cell* cell, int unused) {
		// Vanilla crash is patched in PatchUtil.cpp
		TES3_NonDynamicData_drawCellMapMarker(this, cell, unused);
	}

	const auto TES3_NonDynamicData_getBaseAnimationFile = reinterpret_cast<const char* (__thiscall*)(const TES3::NonDynamicData*, int, int)>(0x4C2720);
	const char* NonDynamicData::getBaseAnimationFile(int isFemale, int firstPerson) const {
		return TES3_NonDynamicData_getBaseAnimationFile(this, isFemale, firstPerson);
	}

	const auto TES3_NonDynamicData_getGameFile = reinterpret_cast<GameFile* (__thiscall*)(TES3::NonDynamicData*, const char*)>(0x4BAF80);
	GameFile* NonDynamicData::getGameFile(const char* name) {
		return TES3_NonDynamicData_getGameFile(this, name);
	}

	std::reference_wrapper<Skill[27]> NonDynamicData::getSkills() {
		return std::ref(skills);
	}

	nonstd::span<GameFile*> NonDynamicData::getActiveMods() {
		return nonstd::span(activeMods, activeModCount);
	}

	IteratedList<GlobalVariable*>* NonDynamicData::getGlobalsList() const {
		if (globals == nullptr) {
			return nullptr;
		}

		return &globals->variables;
	}

	sol::table NonDynamicData::getMagicEffects_lua(sol::this_state ts) {
		sol::state_view state = ts;
		sol::table results = state.create_table();
		for (const auto& effect : magicEffects->effectObjects) {
			if (effect) {
				results[effect->id + 1] = effect;
			}
		}
		return results;
	}

	bool NonDynamicData::objectExists(const std::string_view& id) {
		return resolveObject(id.data()) != nullptr;
	}

	//
	// DataHandler
	//

	DataHandler* DataHandler::get() {
		return *reinterpret_cast<TES3::DataHandler**>(0x7C67E0);
	}

	Vector3 DataHandler::getLastExteriorPosition() const {
		if (currentInteriorCell && lastExteriorCellPositionX != INT_MAX && lastExteriorCellPositionY != INT_MAX) {
			const auto x = float(lastExteriorCellPositionX * TES3::Cell::exteriorGridWidth);
			const auto y = float(lastExteriorCellPositionY * TES3::Cell::exteriorGridWidth);
			return Vector3(x, y, 0.0f);
		}
		else {
			auto macp = TES3::WorldController::get()->getMobilePlayer();
			return macp->reference->position;
		}
	}

	float DataHandler::getLowestZInCurrentCell() const {
		return *reinterpret_cast<float*>(0x7B217C);
	}

	const auto TES3_DataHandler_addSound = reinterpret_cast<void(__thiscall*)(DataHandler*, Sound*, Reference*, int, unsigned char, float, bool, int)>(0x48BD40);
	void DataHandler::addSound(Sound* sound, Reference* reference, int playbackFlags, unsigned char volume, float pitch, bool isVoiceover, int unknown) {
		if (sound == nullptr) {
			return;
		}

		if (mwse::lua::event::AddSoundEvent::getEventEnabled()) {
			auto& luaManager = mwse::lua::LuaManager::getInstance();
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::table eventData = stateHandle.triggerEvent(new mwse::lua::event::AddSoundEvent(sound, reference, playbackFlags, volume, pitch, isVoiceover));
			if (eventData.valid()) {
				if (eventData.get_or("block", false)) {
					return;
				}

				// Override event data.
				sound = eventData["sound"];
				reference = eventData["reference"];
				volume = eventData["volume"];
				pitch = eventData["pitch"];
				isVoiceover = eventData["isVoiceover"];
			}
		}

		TES3_DataHandler_addSound(this, sound, reference, playbackFlags, volume, pitch, isVoiceover, unknown);
	}

	const auto TES3_DataHandler_addSoundById = reinterpret_cast<Sound * (__thiscall*)(DataHandler*, const char*, Reference*, int, unsigned char, float, int)>(0x48BCB0);
	Sound* DataHandler::addSoundById(const char* soundId, Reference* reference, int playbackFlags, unsigned char volume, float pitch, int unknown) {
		return TES3_DataHandler_addSoundById(this, soundId, reference, playbackFlags, volume, pitch, unknown);
	}

	const auto TES3_DataHandler_addTemporarySound = reinterpret_cast<void(__thiscall*)(DataHandler*, const char*, Reference*, int, int, float, bool, Sound*)>(0x48C2B0);
	void DataHandler::addTemporarySound(const char* path, Reference* reference, int playbackFlags, int volume, float pitch, bool isVoiceover, Sound* sound) {
		if (mwse::lua::event::AddTempSoundEvent::getEventEnabled()) {
			auto& luaManager = mwse::lua::LuaManager::getInstance();
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::table eventData = stateHandle.triggerEvent(new mwse::lua::event::AddTempSoundEvent(path, reference, playbackFlags, volume, pitch, isVoiceover, sound));
			if (eventData.valid()) {
				if (eventData.get_or("block", false)) {
					return;
				}

				// Override event data.
				path = eventData.get_or<const char*>("path", path);
				reference = eventData["reference"];
				volume = eventData["volume"];
				pitch = eventData["pitch"];
				isVoiceover = eventData["isVoiceover"];
				sound = eventData["sound"];
			}
		}

		TES3_DataHandler_addTemporarySound(this, path, reference, playbackFlags, volume, pitch, isVoiceover, sound);
	}

	const auto TES3_DataHandler_getSoundPlaying = reinterpret_cast<SoundEvent * (__thiscall*)(DataHandler*, Sound*, Reference*)>(0x48BBD0);
	SoundEvent* DataHandler::getSoundPlaying(Sound* sound, Reference* reference) {
		return TES3_DataHandler_getSoundPlaying(this, sound, reference);
	}

	const auto TES3_Audio_setBufferVolume = reinterpret_cast<void(__stdcall*)(SoundBuffer*, unsigned char)>(0x4029F0);
	void DataHandler::adjustSoundVolume(Sound* sound, Reference* reference, unsigned char volume) {
		SoundEvent* e = getSoundPlaying(sound, reference);
		if (e) {
			// Active buffer differs between sounds and temp sounds
			SoundBuffer* buffer = e->soundBuffer ? e->soundBuffer : e->sound->soundBuffer;
			TES3_Audio_setBufferVolume(buffer, volume);
		}
	}

	const auto TES3_DataHandler_removeSound = reinterpret_cast<void(__thiscall*)(DataHandler*, Sound*, Reference*)>(0x48C0D0);
	void DataHandler::removeSound(Sound* sound, Reference* reference) {
		TES3_DataHandler_removeSound(this, sound, reference);
	}

	const auto TES3_DataHandler_loadSourceTexture = reinterpret_cast<NI::SourceTexture * (__thiscall*)(TES3::DataHandler*, const char*)>(0x48DB60);
	NI::Pointer<NI::SourceTexture> DataHandler::loadSourceTexture(const char* path) {
		return TES3_DataHandler_loadSourceTexture(this, path);
	}

	const auto TES3_DataHandler_setDisplayCellBorders = reinterpret_cast<void(__thiscall*)(TES3::DataHandler*, bool)>(0x48E0E0);
	void DataHandler::setDisplayCellBorders(bool display) {
		TES3_DataHandler_setDisplayCellBorders(this, display);
	}

	const auto TES3_DataHandler_setActorCollisionBoxesDisplay = reinterpret_cast<void(__thiscall*)(TES3::DataHandler*, bool, bool)>(0x48DCC0);
	void DataHandler::setActorCollisionBoxesDisplay(bool showActorDrawBounds, bool showWireframe) {
		TES3_DataHandler_setActorCollisionBoxesDisplay(this, showActorDrawBounds, showWireframe);
	}

	const auto TES3_DataHandler_updateLightingForReference = reinterpret_cast<void(__thiscall*)(TES3::DataHandler*, TES3::Reference*)>(0x485E40);
	void DataHandler::updateLightingForReference(Reference* reference) {
		TES3_DataHandler_updateLightingForReference(this, reference);
	}

	const auto TES3_DataHandler_updateLightingForExteriorCells = reinterpret_cast<void(__thiscall*)(TES3::DataHandler*)>(0x485C50);
	void DataHandler::updateLightingForExteriorCells() {
		TES3_DataHandler_updateLightingForExteriorCells(this);
	}

	const auto TES3_DataHandler_setDynamicLightingForReference = reinterpret_cast<void(__thiscall*)(DataHandler*, Reference*)>(0x485B00);
	void DataHandler::setDynamicLightingForReference(Reference* reference) {
		TES3_DataHandler_setDynamicLightingForReference(this, reference);
	}

	static bool& TES3_COLLISION_DATA_RESETTING = *reinterpret_cast<bool*>(0x7C9A78);
	static bool& COLLISION_DATA_RESET_COLLISION_GROUPS = *reinterpret_cast<bool*>(0x7C9BCC);
	void DataHandler::updateCollisionGroupsForActiveCells(bool force, bool isResettingData, bool resetCollisionGroups) {
		TES3_COLLISION_DATA_RESETTING = isResettingData;
		COLLISION_DATA_RESET_COLLISION_GROUPS = resetCollisionGroups;
		updateCollisionGroupsForActiveCells_raw(force);
	}

	const auto TES3_DataHandler_updateCollisionGroupsForActiveCells = reinterpret_cast<void(__thiscall*)(DataHandler*, bool)>(0x488950);
	void DataHandler::updateCollisionGroupsForActiveCells_raw(bool force) {
		TES3_DataHandler_updateCollisionGroupsForActiveCells(this, force);
	}

	const auto TES3_DataHandler_getClosestPrisonReferences = reinterpret_cast<void(__thiscall*)(DataHandler*, Reference**, Reference**)>(0x48EE30);
	void DataHandler::getClosestPrisonReferences(Reference** prisonMarker, Reference** stolenGoods) {
		TES3_DataHandler_getClosestPrisonReferences(this, prisonMarker, stolenGoods);
	}

	const auto TES3_DataHandler_isCellInMemory  = reinterpret_cast<bool(__thiscall*)(const DataHandler*, const Cell*, bool)>(0x484AF0);
	bool DataHandler::isCellInMemory(const Cell* cell, bool unknown) const {
		return TES3_DataHandler_isCellInMemory(this, cell, unknown);
	}

	std::reference_wrapper<DataHandler::ExteriorCellData* [9]> DataHandler::getExteriorCellData_lua() {
		return std::ref(exteriorCellData);
	}

	long DataHandler::getGameSettingLong(int id) const {
		return nonDynamicData->GMSTs[id]->value.asLong;
	}

	float DataHandler::getGameSettingFloat(int id) const {
		return nonDynamicData->GMSTs[id]->value.asFloat;
	}

	const char* DataHandler::getGameSettingString(int id) const {
		return nonDynamicData->GMSTs[id]->value.asString;
	}

}
