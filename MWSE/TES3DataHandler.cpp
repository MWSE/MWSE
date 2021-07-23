#include "TES3DataHandler.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "LuaAddSoundEvent.h"
#include "LuaAddTempSoundEvent.h"
#include "LuaLoadedGameEvent.h"
#include "LuaLoadGameEvent.h"
#include "LuaMeshLoadedEvent.h"
#include "LuaSavedGameEvent.h"
#include "LuaSaveGameEvent.h"

#include "TES3Util.h"

#include "TES3Alchemy.h"
#include "TES3Cell.h"
#include "TES3DialogueInfo.h"
#include "TES3GlobalVariable.h"
#include "TES3MagicEffectController.h"
#include "TES3MobilePlayer.h"
#include "TES3Reference.h"
#include "TES3Sound.h"
#include "TES3Spell.h"
#include "TES3WorldController.h"
#include "TES3UIManager.h"

#define TES3_NonDynamicData_saveGame 0x4C4250
#define TES3_NonDynamicData_loadGameInGame 0x4C4800
#define TES3_NonDynamicData_loadGameMainMenu 0x4C4EB0
#define TES3_NonDynamicData_resolveObject 0x4B8B60
#define TES3_NonDynamicData_findDialogue 0x4BA8D0
#define TES3_NonDynamicData_findFirstCloneOfActor 0x4B8F50
#define TES3_NonDynamicData_findScriptByName 0x4BA700
#define TES3_NonDynamicData_findGlobalVariable 0x4BA820
#define TES3_NonDynamicData_findSound 0x4BA7A0
#define TES3_NonDynamicData_addNewObject 0x4B8980
#define TES3_NonDynamicData_deleteObject 0x4B8B20

#define TES3_DataHandler_getSoundPlaying 0x48BBD0
#define TES3_DataHandler_removeSound 0x48C0D0
#define TES3_Audio_setBufferVolume 0x4029F0

namespace TES3 {

	Cell* DataHandler::previousVisitedCell = nullptr;
	bool DataHandler::dontThreadLoad = false;
	bool DataHandler::suppressThreadLoad = false;

	//
	// MeshData
	//

	const auto TES3_MeshData_loadMesh = reinterpret_cast<NI::AVObject *(__thiscall*)(MeshData*, const char *)>(0x4EE0A0);
	NI::AVObject * MeshData::loadMesh(const char* path) {
		auto countBefore = NIFs->count;
		auto mesh = TES3_MeshData_loadMesh(this, path);
		if (mesh && NIFs->count > countBefore && mwse::lua::event::MeshLoadedEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::MeshLoadedEvent(path, mesh));
		}
		return mesh;
	}

	const auto TES3_MeshData_loadKeyFrame = reinterpret_cast<KeyframeDefinition * (__thiscall*)(MeshData*, const char*, const char*)>(0x4EE200);
	KeyframeDefinition* MeshData::loadKeyFrame(const char* path, const char* animation) {
		return TES3_MeshData_loadKeyFrame(this, path, animation);
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

		bool saved = reinterpret_cast<signed char(__thiscall *)(NonDynamicData*, const char*, const char*)>(TES3_NonDynamicData_saveGame)(this, eventFileName.c_str(), eventSaveName.c_str());

		luaManager.clearPersistentTimers();

		// Pass a follow-up event if we successfully saved.
		if (saved && mwse::lua::event::SavedGameEvent::getEventEnabled()) {
			luaManager.getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::SavedGameEvent(eventSaveName.c_str(), eventFileName.c_str()));
		}

		return saved;
	}

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

		bool loaded = reinterpret_cast<bool(__thiscall *)(NonDynamicData*, const char*)>(TES3_NonDynamicData_loadGameInGame)(this, eventFileName.c_str());

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

		return loaded ? LoadGameResult::Success : LoadGameResult::Failure;
	}

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

		bool loaded = reinterpret_cast<bool(__thiscall *)(NonDynamicData*, const char*)>(TES3_NonDynamicData_loadGameMainMenu)(this, eventFileName.c_str());

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

	BaseObject* NonDynamicData::resolveObject(const char* id) {
		return reinterpret_cast<BaseObject*(__thiscall *)(NonDynamicData*, const char*)>(TES3_NonDynamicData_resolveObject)(this, id);
	}

	Reference* NonDynamicData::findFirstCloneOfActor(const char* baseId) {
		return reinterpret_cast<Reference*(__thiscall *)(NonDynamicData*, const char*)>(TES3_NonDynamicData_findFirstCloneOfActor)(this, baseId);
	}

	Spell* NonDynamicData::getSpellById(const char* id) {
		auto spell = spellsList->front();
		while (spell != NULL && _stricmp(id, spell->objectID) != 0) {
			spell = reinterpret_cast<TES3::Spell*>(spell->nextInCollection);
		}
		return spell;
	}

	Script* NonDynamicData::findScriptByName(const char* name) {
		return reinterpret_cast<Script*(__thiscall *)(NonDynamicData*, const char*)>(TES3_NonDynamicData_findScriptByName)(this, name);
	}

	GlobalVariable* NonDynamicData::findGlobalVariable(const char* name) {
#if MWSE_CUSTOM_GLOBALS
		return globals->getVariable(name);
#else
		return reinterpret_cast<GlobalVariable*(__thiscall *)(NonDynamicData*, const char*)>(TES3_NonDynamicData_findGlobalVariable)(this, name);
#endif
	}

	Dialogue* NonDynamicData::findDialogue(const char* name) {
		return reinterpret_cast<Dialogue*(__thiscall *)(NonDynamicData*, const char*)>(TES3_NonDynamicData_findDialogue)(this, name);
	}

	bool NonDynamicData::addSound(Sound* sound) {
		if (findSound(sound->getObjectID())) {
			return false;
		}

		sounds->push_back(sound);
		return true;
	}

	Sound* NonDynamicData::findSound(const char* id) {
		return reinterpret_cast<Sound*(__thiscall *)(NonDynamicData*, const char*)>(TES3_NonDynamicData_findSound)(this, id);
	}

	const auto TES3_NonDynamicData_findClass = reinterpret_cast<Class * (__thiscall*)(NonDynamicData*, const char*)>(0x4BA6B0);
	Class* NonDynamicData::findClass(const char* id) {
		return TES3_NonDynamicData_findClass(this, id);
	}

	const auto TES3_NonDynamicData_findFaction = reinterpret_cast<Faction *(__thiscall*)(NonDynamicData*, const char*)>(0x4BA750);
	Faction* NonDynamicData::findFaction(const char* id) {
		return TES3_NonDynamicData_findFaction(this, id);
	}

	const auto TES3_NonDynamicData_findClosestExteriorReferenceOfObject = reinterpret_cast<Reference * (__thiscall*)(NonDynamicData*, PhysicalObject*, Vector3*, bool, int)>(0x4B96F0);
	Reference* NonDynamicData::findClosestExteriorReferenceOfObject(PhysicalObject* object, Vector3* position, bool searchForExteriorDoorMarker, int ignored){
		return TES3_NonDynamicData_findClosestExteriorReferenceOfObject(this, object, position, searchForExteriorDoorMarker, ignored);
	}

	bool NonDynamicData::addNewObject(BaseObject* object) {
		return reinterpret_cast<signed char(__thiscall *)(NonDynamicData*, BaseObject*)>(TES3_NonDynamicData_addNewObject)(this, object);
	}

	void NonDynamicData::deleteObject(BaseObject* object) {
		reinterpret_cast<void(__thiscall *)(NonDynamicData*, BaseObject*)>(TES3_NonDynamicData_deleteObject)(this, object);
	}

	const auto TES3_NonDynamicData_respawnContainers = reinterpret_cast<void(__thiscall*)(NonDynamicData*)>(0x4C3B00);
	void NonDynamicData::respawnContainers() {
		TES3_NonDynamicData_respawnContainers(this);
	}

	const auto TES3_NonDynamicData_getCellByGrid = reinterpret_cast<Cell *(__thiscall*)(NonDynamicData*, int, int)>(0x4BAA10);
	Cell * NonDynamicData::getCellByGrid(int x, int y) {
		return TES3_NonDynamicData_getCellByGrid(this, x, y);
	}

	const auto TES3_NonDynamicData_getCellByName = reinterpret_cast<Cell *(__thiscall*)(NonDynamicData*, const char*)>(0x4BA9B0);
	Cell * NonDynamicData::getCellByName(const char* name) {
		return TES3_NonDynamicData_getCellByName(this, name);
	}

	const auto TES3_NonDynamicData_getRegionById = reinterpret_cast<Region * (__thiscall*)(NonDynamicData*, const char*)>(0x4BA610);
	Region* NonDynamicData::getRegion(const char* id) {
		return TES3_NonDynamicData_getRegionById(this, id);
	}

	MagicEffect * NonDynamicData::getMagicEffect(int id) {
		return magicEffects->getEffectObject(id);
	}

	const auto TES3_NonDynamicData_createReference = reinterpret_cast<float(__thiscall*)(NonDynamicData*, PhysicalObject*, Vector3*, Vector3*, bool&, Reference*, Cell*)>(0x4C0E80);
	float NonDynamicData::createReference(PhysicalObject * object, Vector3 * position, Vector3 * orientation, bool& cellWasCreated, Reference * existingReference, Cell * cell) {
		return TES3_NonDynamicData_createReference(this, object, position, orientation, cellWasCreated, existingReference, cell);
	}

	const auto TES3_NonDynamicData_showLocationOnMap = reinterpret_cast<void(__thiscall*)(TES3::NonDynamicData*, const char*)>(0x4C8480);
	void NonDynamicData::showLocationOnMap(const char* name) {
		auto idLength = strnlen_s(name, 32);
		for (auto cell : *cells) {
			if (cell->name && !cell->isInterior() && !(cell->cellFlags & 0x20)) {
				if (_strnicmp(cell->name, name, idLength) == 0) {
					drawCellMapMarker(cell);
				}
			}
		}
	}

	const auto TES3_NonDynamicData_drawCellMapMarker = reinterpret_cast<void(__thiscall*)(TES3::NonDynamicData*, TES3::Cell*, int)>(0x4C8540);
	void NonDynamicData::drawCellMapMarker(Cell* cell, int unused) {
		// Fix crash when trying to render cells too far out.
		if (cell->getGridY() < -27) {
			return;
		}

		TES3_NonDynamicData_drawCellMapMarker(this, cell, unused);
	}

	const auto TES3_NonDynamicData_getBaseAnimationFile = reinterpret_cast<const char*(__thiscall*)(const TES3::NonDynamicData*, int, int)>(0x4C2720);
	const char* NonDynamicData::getBaseAnimationFile(int isFemale, int firstPerson) const {
		return TES3_NonDynamicData_getBaseAnimationFile(this, isFemale, firstPerson);
	}

	Alchemy* NonDynamicData::getMatchingAlchemyItem(const Alchemy* testAgainst) const {
		if (testAgainst->objectType != ObjectType::Alchemy) {
			return nullptr;
		}

		for (auto item : *list) {
			auto alch = static_cast<Alchemy*>(item);

			// We only care about alchemy objects.
			if (alch->objectType != ObjectType::Alchemy) {
				continue;
			}

			// Check object flags.
			if (alch->objectFlags != testAgainst->objectFlags) {
				continue;
			}

			// Check basic values.
			if (alch->weight != testAgainst->weight || alch->value != testAgainst->value || alch->flags != alch->flags) {
				continue;
			}

			// Check effects.
			if (!alch->effectsMatchWith(testAgainst)) {
				continue;
			}

			// Check script.
			if (alch->script != testAgainst->script) {
				continue;
			}

			// Check name.
			if (_strnicmp(alch->name, testAgainst->name, 32) != 0) {
				continue;
			}

			// Check model.
			if (_strnicmp(alch->model, testAgainst->model, 32) != 0) {
				continue;
			}

			// Check icon.
			if (_strnicmp(alch->icon, testAgainst->icon, 32) != 0) {
				continue;
			}

			// Good enough. It's a match.
			return alch;
		}
		return nullptr;
	}

	std::reference_wrapper<Skill[27]> NonDynamicData::getSkills() {
		return std::ref(skills);
	}

	sol::table NonDynamicData::getMagicEffects_lua(sol::this_state ts) {
		sol::state_view state = ts;
		sol::table results = state.create_table();
		for (auto itt : magicEffects->effectObjects) {
			results[itt.second->id + 1] = itt.second;
		}
		return results;
	}

	//
	// DataHandler
	//

	DataHandler * DataHandler::get() {
		return *reinterpret_cast<TES3::DataHandler**>(0x7C67E0);
	}

	Vector3 DataHandler::getLastExteriorPosition() {
		if (currentInteriorCell && lastExteriorCellPositionX != INT_MAX && lastExteriorCellPositionX != INT_MAX) {
			auto x = float(lastExteriorCellPositionX * TES3::Cell::exteriorGridWidth);
			auto y = float(lastExteriorCellPositionY * TES3::Cell::exteriorGridWidth);
			return Vector3(x, y, 0.0f);
		}
		else {
			auto macp = TES3::WorldController::get()->getMobilePlayer();
			return macp->reference->position;
		}
	}

	const auto TES3_DataHandler_addSound = reinterpret_cast<void(__thiscall*)(DataHandler*, Sound*, Reference*, int, unsigned char, float, bool, int)>(0x48BD40);
	void DataHandler::addSound(Sound* sound, Reference* reference, int playbackFlags, unsigned char volume, float pitch, bool isVoiceover, int unknown) {
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

	const auto TES3_DataHandler_addSoundById = reinterpret_cast<Sound*(__thiscall*)(DataHandler*, const char*, Reference*, int, unsigned char, float, int)>(0x48BCB0);
	Sound* DataHandler::addSoundById(const char* soundId, Reference* reference, int playbackFlags, unsigned char volume, float pitch, int unknown) {
		return TES3_DataHandler_addSoundById(this, soundId, reference, playbackFlags, volume, pitch, unknown);
	}

	const auto TES3_DataHandler_addTemporySound = reinterpret_cast<void(__thiscall*)(DataHandler*, const char*, Reference*, int, int, float, bool, Sound*)>(0x48C2B0);
	void DataHandler::addTemporySound(const char* path, Reference * reference, int playbackFlags, int volume, float pitch, bool isVoiceover, Sound * sound) {
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

		TES3_DataHandler_addTemporySound(this, path, reference, playbackFlags, volume, pitch, isVoiceover, sound);
	}

	SoundEvent* DataHandler::getSoundPlaying(Sound* sound, Reference* reference) {
		return reinterpret_cast<SoundEvent*(__thiscall *)(DataHandler*, Sound*, Reference*)>(TES3_DataHandler_getSoundPlaying)(this, sound, reference);
	}

	void DataHandler::adjustSoundVolume(Sound* sound, Reference* reference, unsigned char volume) {
		SoundEvent* e = getSoundPlaying(sound, reference);
		if (e) {
			// Active buffer differs between sounds and temp sounds
			SoundBuffer* buffer = e->soundBuffer ? e->soundBuffer : e->sound->soundBuffer;
			reinterpret_cast<void(__stdcall*)(SoundBuffer*, unsigned char)>(TES3_Audio_setBufferVolume)(buffer, volume);
		}
	}

	void DataHandler::removeSound(Sound* sound, Reference* reference) {
		reinterpret_cast<void(__thiscall *)(DataHandler*, Sound*, Reference*)>(TES3_DataHandler_removeSound)(this, sound, reference);
	}

	const auto TES3_DataHandler_loadSourceTexture = reinterpret_cast<NI::SourceTexture*(__thiscall*)(TES3::DataHandler*, const char*)>(0x48DB60);
	NI::Pointer<NI::SourceTexture> DataHandler::loadSourceTexture(const char* path) {
		return TES3_DataHandler_loadSourceTexture(this, path);
	}

	const auto TES3_DataHandler_updateLightingForReference = reinterpret_cast<void(__thiscall*)(TES3::DataHandler *, TES3::Reference *)>(0x485E40);
	void DataHandler::updateLightingForReference(Reference * reference) {
		TES3_DataHandler_updateLightingForReference(this, reference);
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

	std::reference_wrapper<DataHandler::ExteriorCellData* [9]> DataHandler::getExteriorCellData_lua() {
		return std::ref(exteriorCellData);
	}

}
