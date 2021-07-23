#include "TES3UtilLua.h"

#include "LuaManager.h"

#include "TES3GameFile.h"

#include "TES3Util.h"
#include "UIUtil.h"
#include "LuaUtil.h"
#include "Stack.h"
#include "Log.h"
#include "ScriptUtil.h"
#include "CodePatchUtil.h"
#include "LuaObject.h"
#include "MemoryUtil.h"

#include "NICamera.h"
#include "NINode.h"
#include "NIPick.h"
#include "NIRTTI.h"
#include "NIStream.h"
#include "NITriShape.h"

#include "TES3Actor.h"
#include "TES3ActorAnimationController.h"
#include "TES3AIData.h"
#include "TES3AIPackage.h"
#include "TES3Alchemy.h"
#include "TES3AnimationData.h"
#include "TES3Archive.h"
#include "TES3Armor.h"
#include "TES3AudioController.h"
#include "TES3Birthsign.h"
#include "TES3Cell.h"
#include "TES3Class.h"
#include "TES3Container.h"
#include "TES3CombatSession.h"
#include "TES3Creature.h"
#include "TES3CrimeEventList.h"
#include "TES3DataHandler.h"
#include "TES3Dialogue.h"
#include "TES3DialogueInfo.h"
#include "TES3Door.h"
#include "TES3Enchantment.h"
#include "TES3Faction.h"
#include "TES3Fader.h"
#include "TES3Game.h"
#include "TES3GameSetting.h"
#include "TES3GlobalVariable.h"
#include "TES3InputController.h"
#include "TES3ItemData.h"
#include "TES3LeveledList.h"
#include "TES3Light.h"
#include "TES3MagicEffectController.h"
#include "TES3MagicEffectInstance.h"
#include "TES3MagicInstanceController.h"
#include "TES3Misc.h"
#include "TES3MobController.h"
#include "TES3MobileCreature.h"
#include "TES3MobilePlayer.h"
#include "TES3NPC.h"
#include "TES3PlayerAnimationController.h" 
#include "TES3Race.h"
#include "TES3Reference.h"
#include "TES3Region.h"
#include "TES3Script.h"
#include "TES3ScriptCompiler.h"
#include "TES3Sound.h"
#include "TES3SoundGenerator.h"
#include "TES3Spell.h"
#include "TES3UIElement.h"
#include "TES3UIManager.h"
#include "TES3UIMenuController.h"
#include "TES3Weather.h"
#include "TES3WeatherController.h"
#include "TES3WorldController.h"

#include "BitUtil.h"
#include <sstream>

namespace mwse {
	namespace lua {
		//
		// Helper functions.
		//

		auto iterateObjectsFiltered(const std::unordered_set<unsigned int> desiredTypes) {
			auto dataHandler = TES3::DataHandler::get();
			if (dataHandler == nullptr) {
				throw std::runtime_error("Function called before game initialization.");
			}
			auto ndd = dataHandler->nonDynamicData;

			// Prepare the lists we care about.
			std::queue<TES3::Object*> objectListQueue;
			bool searchingSpells = desiredTypes.count(TES3::ObjectType::Spell);
			if (searchingSpells) {
				objectListQueue.push(ndd->spellsList->head);
			}
			if (desiredTypes.size() != 1 || !searchingSpells) {
				objectListQueue.push(ndd->list->head);
			}

			// Get the first reference we care about.
			TES3::Object* object = nullptr;
			if (!objectListQueue.empty()) {
				object = objectListQueue.front();
				objectListQueue.pop();
			}

			return [object, objectListQueue, desiredTypes]() mutable -> TES3::BaseObject* {
				while (object && !desiredTypes.empty() && !desiredTypes.count(object->objectType)) {
					object = reinterpret_cast<TES3::Reference*>(object->nextInCollection);

					// If we hit the end of the list, check for the next list.
					if (object == nullptr && !objectListQueue.empty()) {
						object = objectListQueue.front();
						objectListQueue.pop();
					}
				}

				if (object == nullptr) {
					return nullptr;
				}

				// Get the object we want to return.
				auto ret = object;

				// Get the next reference. If we're at the end of the list, go to the next one
				object = reinterpret_cast<TES3::Reference*>(object->nextInCollection);
				if (object == nullptr && !objectListQueue.empty()) {
					object = objectListQueue.front();
					objectListQueue.pop();
				}

				return ret;
			};
		}

		//
		// tes3.* functions.
		//

		TES3::Reference* getPlayerRef() {
			TES3::WorldController* worldController = TES3::WorldController::get();
			if (worldController) {
				TES3::MobilePlayer* mobilePlayer = worldController->getMobilePlayer();
				if (mobilePlayer) {
					return mobilePlayer->reference;
				}
			}
			return nullptr;
		}

		TES3::MobilePlayer* getMobilePlayer() {
			TES3::WorldController* worldController = TES3::WorldController::get();
			if (worldController) {
				return worldController->getMobilePlayer();
			}
			return nullptr;
		}

		TES3::Cell* getPlayerCell() {
			TES3::DataHandler* dataHandler = TES3::DataHandler::get();
			if (dataHandler) {
				return dataHandler->currentCell;
			}
			return nullptr;
		}

		TES3::Reference* getPlayerTarget() {
			TES3::Game* game = TES3::Game::get();
			if (game) {
				return game->playerTarget;
			}
			return nullptr;
		}

		TES3::Reference* getReference(sol::optional<const char*> id) {
			if (id) {
				return tes3::getReference(id.value());
			}
			else {
				return LuaManager::getInstance().getCurrentReference();
			}
		}

		TES3::BaseObject* getObject(const char* id) {
			TES3::DataHandler* dataHandler = TES3::DataHandler::get();
			if (dataHandler) {
				return dataHandler->nonDynamicData->resolveObject(id);
			}
			return nullptr;
		}

		bool deleteObject(sol::object maybe) {
			TES3::BaseObject* object = maybe.as<TES3::BaseObject*>();
			if (object) {
				TES3::DataHandler::get()->nonDynamicData->deleteObject(object);
				object->vTable.base->destructor(object, true);
				return true;
			}
			return false;
		}

		TES3::Script* getScript(const char* id) {
			TES3::DataHandler* dataHandler = TES3::DataHandler::get();
			if (dataHandler) {
				return dataHandler->nonDynamicData->findScriptByName(id);
			}
			return nullptr;
		};

		sol::optional<double> getGlobal(const char* id) {
			TES3::DataHandler* dataHandler = TES3::DataHandler::get();
			if (dataHandler) {
				TES3::GlobalVariable* global = dataHandler->nonDynamicData->findGlobalVariable(id);
				if (global) {
					return global->getValue_lua();
				}
			}
			return {};
		}

		bool setGlobal(std::string& id, double value) {
			TES3::DataHandler* dataHandler = TES3::DataHandler::get();
			if (dataHandler) {
				TES3::GlobalVariable* global = dataHandler->nonDynamicData->findGlobalVariable(id.c_str());
				if (global) {
					global->setValue_lua(value);
					return true;
				}
			}
			return false;
		}

		TES3::GlobalVariable* findGlobal(std::string& id) {
			TES3::DataHandler* dataHandler = TES3::DataHandler::get();
			if (dataHandler) {
				return dataHandler->nonDynamicData->findGlobalVariable(id.c_str());
			}
			return nullptr;
		}

		TES3::GameSetting* findGMST(sol::object key) {
			TES3::DataHandler* dataHandler = TES3::DataHandler::get();
			if (dataHandler == nullptr) {
				return nullptr;
			}

			int index = -1;
			if (key.is<int>()) {
				index = key.as<int>();
			}
			else if (key.is<const char*>()) {
				auto& luaManager = mwse::lua::LuaManager::getInstance();
				auto stateHandle = luaManager.getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::object asIndex = state["tes3"]["gmst"][key.as<const char*>()];
				if (asIndex.is<int>()) {
					index = asIndex.as<int>();
				}
			}

			if (index >= TES3::GMST::sMonthMorningstar && index <= TES3::GMST::sWitchhunter) {
				return dataHandler->nonDynamicData->GMSTs[index];
			}

			return nullptr;
		}

		TES3::GameSetting* getGMST(sol::object key) {
			auto& luaManager = mwse::lua::LuaManager::getInstance();
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Display deprecation warning and traceback.
			logStackTrace("WARNING: Use of deprecated function tes3.getGMST. Use tes3.findGMST instead.");

			TES3::DataHandler* dataHandler = TES3::DataHandler::get();
			if (dataHandler == nullptr) {
				return nullptr;
			}

			if (key.is<int>()) {
				int index = key.as<int>();
				if (index >= TES3::GMST::sMonthMorningstar && index <= TES3::GMST::sWitchhunter) {
					return dataHandler->nonDynamicData->GMSTs[index];
				}
			}
			else if (key.is<std::string>()) {
				int index = -1;
				std::string keyStr = key.as<std::string>();
				for (int i = 0; i <= TES3::GMST::sWitchhunter; i++) {
					TES3::GameSettingInfo* info = TES3::GameSettingInfo::get(i);
					if (strcmp(info->name, keyStr.c_str()) == 0) {
						index = i;
						break;
					}
				}

				if (index != -1) {
					return dataHandler->nonDynamicData->GMSTs[index];
				}
			}

			return nullptr;
		}

		bool playSound(sol::optional<sol::table> params) {
			// Get parameters.
			TES3::Sound* sound = getOptionalParamSound(params, "sound");
			const char* soundPath = getOptionalParam<const char*>(params, "soundPath", nullptr);
			TES3::Reference* reference = getOptionalParamReference(params, "reference");
			bool loop = getOptionalParam<bool>(params, "loop", false);
			int mix = getOptionalParam<int>(params, "mixChannel", int(TES3::AudioMixType::Effects));
			double volume = getOptionalParam<double>(params, "volume", 1.0);
			float pitch = getOptionalParam<float>(params, "pitch", 1.0f);

			if (sound == nullptr && soundPath == nullptr) {
				log::getLog() << "tes3.playSound: Could not locate sound." << std::endl;
				return false;
			}

			// Clamp volume. RIP no std::clamp.
			volume = std::max(0.0, volume);
			volume = std::min(volume, 1.0);

			// Apply mix and rescale to 0-250
			volume *= 250.0 * TES3::WorldController::get()->audioController->getMixVolume(TES3::AudioMixType(mix));

			if (soundPath) {
				bool isVoiceover = getOptionalParam<bool>(params, "isVoiceover", false);
				TES3::DataHandler::get()->addTemporySound(soundPath, reference, loop ? TES3::SoundPlayFlags::Loop : 0, int(volume), pitch, isVoiceover, sound);
				return true;
			}
			else if (sound) {
				TES3::DataHandler::get()->addSound(sound, reference, loop ? TES3::SoundPlayFlags::Loop : 0, int(volume), pitch);
				return true;
			}

			return false;
		}

		bool getSoundPlaying(sol::optional<sol::table> params) {
			// Get parameters.
			TES3::Sound* sound = getOptionalParamSound(params, "sound");
			TES3::Reference* reference = getOptionalParamReference(params, "reference");

			if (sound == nullptr) {
				log::getLog() << "tes3.getSoundPlaying: Could not locate sound." << std::endl;
				return false;
			}

			return bool(TES3::DataHandler::get()->getSoundPlaying(sound, reference));
		}

		void adjustSoundVolume(sol::optional<sol::table> params) {
			// Get parameters.
			TES3::Sound* sound = getOptionalParamSound(params, "sound");
			TES3::Reference* reference = getOptionalParamReference(params, "reference");
			auto mix = getOptionalParam<int>(params, "mixChannel", int(TES3::AudioMixType::Effects));
			auto volume = getOptionalParam<double>(params, "volume", 1.0);

			if (!sound || !reference) {
				log::getLog() << "tes3.adjustSoundVolume: Valid sound and reference required." << std::endl;
				return;
			}

			// Clamp volume.
			volume = std::max(0.0, volume);
			volume = std::min(volume, 1.0);

			// Apply mix and rescale to 0-250
			volume *= 250.0 * TES3::WorldController::get()->audioController->getMixVolume(TES3::AudioMixType(mix));

			TES3::DataHandler::get()->adjustSoundVolume(sound, reference, unsigned char(volume));
		}

		void removeSound(sol::optional<sol::table> params) {
			// Get parameters.
			TES3::Sound* sound = getOptionalParamSound(params, "sound");
			TES3::Reference* reference = getOptionalParamReference(params, "reference");

			TES3::DataHandler::get()->removeSound(sound, reference);
		}

		void streamMusic(sol::optional<sol::table> params) {
			auto worldController = TES3::WorldController::get();

			// Get parameters.
			const char* relativePath = getOptionalParam<const char*>(params, "path", nullptr);
			int situation = getOptionalParam<int>(params, "situation", int(TES3::MusicSituation::Uninterruptible));
			double crossfade = getOptionalParam<double>(params, "crossfade", 1.0);
			float volume = getOptionalParam<float>(params, "volume", worldController->audioController->getMusicVolume());

			if (relativePath) {
				char path[260];

				std::snprintf(path, sizeof(path), "Data Files/music/%s", relativePath);
				worldController->audioController->changeMusicTrack(path, int(1000.0f * crossfade), volume);
				worldController->musicSituation = TES3::MusicSituation(situation);
			}
		}

		TES3::UI::Element* messageBox(sol::object param, sol::optional<sol::variadic_args> va) {
			auto& luaManager = mwse::lua::LuaManager::getInstance();
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			if (param.is<std::string>()) {
				std::string message = state["string"]["format"](param, va);
				return TES3::UI::showMessageBox(message.c_str());
			}
			else if (param.get_type() == sol::type::table) {
				sol::table params = param;

				// We need to make sure the strings stay in memory, we can't just snag the c-string in passing.
				std::string buttonText[32];
				struct {
					const char* text[32] = {};
				} buttonTextStruct;

				//  Get the parameters out of the table and into the table.
				std::string message = params["message"];
				sol::optional<sol::table> maybeButtons = params["buttons"];
				if (maybeButtons && maybeButtons.value().size() > 0) {
					sol::table buttons = maybeButtons.value();
					size_t size = std::min(buttons.size(), size_t(32));
					for (size_t i = 0; i < size; i++) {
						std::string result = buttons[i + 1];
						if (result.empty()) {
							break;
						}

						buttonText[i] = result;
						buttonTextStruct.text[i] = buttonText[i].c_str();
					}
				}

				// No buttons, do a normal popup.
				else {
					auto showInDialog = getOptionalParam<bool>(params, "showInDialog", true);

					auto element = TES3::UI::showMessageBox(message.c_str(), nullptr, showInDialog);
					if (element) {
						// Allow overriding the duration.
						auto duration = getOptionalParam<float>(params, "duration");
						if (duration) {
							element->setProperty(TES3::UI::registerProperty("MenuNotify_timestamp"), duration.value());
						}
					}
					return element;
				}

				// Set up our event callback.
				LuaManager::getInstance().setButtonPressedCallback(params["callback"]);

				// Temporary hook into the function that creates message boxes. 
				reinterpret_cast<void(__cdecl*)(const char*, ...)>(0x5F1AA0)(message.c_str(), buttonTextStruct, NULL);
				return TES3::UI::findMenu(TES3::UI::registerID("MenuMessage"));
			}
			else {
				sol::protected_function_result result = state["tostring"](param);
				if (result.valid()) {
					sol::optional<const char*> asString = result;
					if (asString) {
						return TES3::UI::showMessageBox(asString.value());
					}
				}
				throw std::exception("tes3.messageBox: Unable to convert parameter to string.");
			}
			return nullptr;
		}

		bool saveGame(sol::optional<sol::table> params) {
			const char* fileName = getOptionalParam<const char*>(params, "file", "quiksave");
			const char* saveName = getOptionalParam<const char*>(params, "name", "Quicksave");

			// Prevent saves from happening if the player is dead.
			if (TES3::WorldController::get()->getMobilePlayer()->health.current <= 0) {
				return false;
			}

			return TES3::DataHandler::get()->nonDynamicData->saveGame(fileName, saveName);
		}

		void loadGame(const char* fileName) {
			// Char Gen State will equal 0 in the menu.
			if (TES3::WorldController::get()->gvarCharGenState->value == 0.0f) {
				TES3::DataHandler::get()->nonDynamicData->loadGameMainMenu(fileName);
			}
			else {
				TES3::DataHandler::get()->nonDynamicData->loadGame(fileName);
			}
		}

		bool isModActive(const char* modName) {
			TES3::DataHandler* dataHandler = TES3::DataHandler::get();
			if (dataHandler == nullptr) {
				return false;
			}

			auto ndd = dataHandler->nonDynamicData;
			for (int i = 0; i < ndd->activeModCount; i++) {
				TES3::GameFile* gameFile = ndd->activeMods[i];
				if (gameFile == nullptr) {
					return false;
				}

				// Compare mod name with this active mod.
				if (_stricmp(gameFile->filename, modName) == 0) {
					return true;
				}
			}

			return false;
		}

		sol::object getModList() {
			auto& luaManager = mwse::lua::LuaManager::getInstance();
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			TES3::DataHandler* dataHandler = TES3::DataHandler::get();
			if (dataHandler == nullptr) {
				return sol::nil;
			}

			auto ndd = dataHandler->nonDynamicData;
			sol::table mods = state.create_table();
			for (int i = 0; i < ndd->activeModCount; i++) {
				TES3::GameFile* gameFile = ndd->activeMods[i];
				if (gameFile == nullptr) {
					break;
				}
				mods[i + 1] = static_cast<const char*>(gameFile->filename);
			}

			return mods;
		}

		sol::table getArchiveList(sol::this_state s) {
			sol::state_view state = s;
			sol::table archives = state.create_table();

			TES3::Archive* archive = **reinterpret_cast<TES3::Archive***>(0x7C9F74);
			size_t i = 0;
			while (archive) {
				archives[++i] = std::string(archive->path, strnlen_s(archive->path, 128));
				archive = archive->nextArchive;
			}

			return archives;
		}

		void playItemPickupSound(sol::optional<sol::table> params) {
			TES3::Reference* reference = getOptionalParamExecutionReference(params);
			TES3::Item* item = getOptionalParamObject<TES3::Item>(params, "item");
			auto state = getOptionalParam<bool>(params, "pickup", true) ? TES3::ItemSoundState::Up : TES3::ItemSoundState::Down;
			if (item == NULL) {
				return;
			}

			TES3::WorldController::get()->playItemUpDownSound(item, state, reference);
		}

		auto iterateObjects(sol::optional<sol::object> filter) {
			std::unordered_set<unsigned int> filters;

			if (filter) {
				if (filter.value().is<unsigned int>()) {
					filters.insert(filter.value().as<unsigned int>());
				}
				else if (filter.value().is<sol::table>()) {
					sol::table filterTable = filter.value().as<sol::table>();
					for (const auto& kv : filterTable) {
						if (kv.second.is<unsigned int>()) {
							filters.insert(kv.second.as<unsigned int>());
						}
					}
				}
				else {
					throw std::invalid_argument("Iteration can only be filtered by object type, a table of object types, or must not have any filter.");
				}
			}

			return iterateObjectsFiltered(std::move(filters));
		}

		TES3::Sound* getSound(const char* id) {
			TES3::DataHandler* dataHandler = TES3::DataHandler::get();
			if (dataHandler) {
				return TES3::DataHandler::get()->nonDynamicData->findSound(id);
			}
			else {
				throw std::exception("Function called before Data Handler was initialized.");
			}
		}

		TES3::SoundGenerator* getSoundGenerator(std::string creatureId, unsigned int type) {
			auto nonDynamicData = TES3::DataHandler::get()->nonDynamicData;
			auto creature = nonDynamicData->resolveObjectByType<TES3::Creature>(creatureId, TES3::ObjectType::Creature);
			if (creature == nullptr) {
				return nullptr;
			}

			while (creature->soundGenerator) {
				creature = creature->soundGenerator;
			}

			auto soundGenerators = nonDynamicData->soundGenerators;
			const char* id = creature->getObjectID();
			size_t idLength = strnlen_s(id, 32);
			for (auto itt = soundGenerators->head; itt != nullptr; itt = itt->next) {
				if (itt->data->soundType != static_cast<TES3::SoundType>(type)) {
					continue;
				}

				if (_strnicmp(id, itt->data->name, idLength) == 0) {
					return itt->data;
				}
			}

			return nullptr;
		}

		TES3::Class* findClass(const char* id) {
			TES3::DataHandler* dataHandler = TES3::DataHandler::get();
			if (dataHandler) {
				return dataHandler->nonDynamicData->findClass(id);
			}
			return nullptr;
		}

		TES3::Faction* getFaction(const char* id) {
			TES3::DataHandler* dataHandler = TES3::DataHandler::get();
			if (dataHandler) {
				return dataHandler->nonDynamicData->findFaction(id);
			}
			return nullptr;
		}

		TES3::MagicEffect* getMagicEffect(int id) {
			TES3::DataHandler* dataHandler = TES3::DataHandler::get();
			if (dataHandler) {
				auto magicEffectsController = dataHandler->nonDynamicData->magicEffects;
				if (magicEffectsController->getEffectExists(id)) {
					return magicEffectsController->getEffectObject(id);
				}
			}
			return nullptr;
		}

		// This function currently calls out to MGE, which should be changed at some point.
		TES3::Vector3 getCameraVector() {
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEGetEyeVec);

			// Get the results from the MWSE stack.
			Stack& stack = Stack::getInstance();
			float x = stack.popFloat();
			float y = stack.popFloat();
			float z = stack.popFloat();

			return TES3::Vector3(x, y, z);
		}

		sol::optional<TES3::Vector3> getCameraPosition() {
			TES3::WorldController* worldController = TES3::WorldController::get();
			if (worldController) {
				return worldController->worldCamera.cameraData.camera->worldBoundOrigin;
			}
			return sol::optional<TES3::Vector3>();
		}

		sol::optional<TES3::Vector3> getPlayerEyePosition() {
			auto worldController = TES3::WorldController::get();
			if (worldController) {
				auto mobilePlayer = worldController->getMobilePlayer();
				if (mobilePlayer) {
					return mobilePlayer->animationController.asPlayer->firstPersonHeadCameraNode->worldTransform.translation;
				}
			}
			return sol::optional<TES3::Vector3>();
		}

		sol::optional<TES3::Vector3> getPlayerEyeVector() {
			auto worldController = TES3::WorldController::get();
			if (worldController) {
				auto rotation = worldController->armCamera.cameraRoot->localRotation;
				return TES3::Vector3(rotation->m0.y, rotation->m1.y, rotation->m2.y);
			}
			return sol::optional<TES3::Vector3>();
		}

		sol::optional<TES3::Vector3> get3rdPersonCameraOffset() {
			auto worldController = TES3::WorldController::get();
			if (!worldController) {
				return {};
			}

			auto macp = worldController->getMobilePlayer();
			if (!macp) {
				return {};
			}

			return macp->animationController.asPlayer->cameraOffset;
		}

		void set3rdPersonCameraOffset(sol::table params) {
			auto offset = getOptionalParamVector3(params, "offset");
			if (!offset) {
				throw std::exception("Invalid 'offset' param provided.");
			}
			auto worldController = TES3::WorldController::get();
			if (!worldController) {
				throw std::exception("WorldController is not yet initialized.");
			}

			auto macp = worldController->getMobilePlayer();
			if (!macp) {
				throw std::exception("MobilePlayer is not yet initialized.");
			}

			macp->animationController.asPlayer->cameraOffset = offset.value();
		}

		const auto TES3_rand = reinterpret_cast<int (__cdecl*)(unsigned int)>(0x47B3B0);
		int random(sol::optional<sol::object> maybe_object) {
			if (maybe_object) {
				sol::object object = maybe_object.value();
				if (object.is<void*>()) {
					return TES3_rand(unsigned int(object.as<void*>()));
				}
				else if (object.is<double>()) {
					return TES3_rand(unsigned int(object.as<double>()));
				}
				else {
					throw std::exception("Object must be reasonably convertable to a number.");
				}
			}
			else {
				return TES3_rand(0);
			}
		}

		static NI::Pick* rayTestCache = nullptr;
		static std::vector<NI::AVObject*> rayTestIgnoreRoots;
		sol::object rayTest(sol::table params) {
			auto& luaManager = mwse::lua::LuaManager::getInstance();
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Make sure we got our required position.
			sol::optional<TES3::Vector3> position = getOptionalParamVector3(params, "position");
			if (!position) {
				return false;
			}

			// Make sure we got our required direction.
			sol::optional<TES3::Vector3> direction = getOptionalParamVector3(params, "direction");
			if (!direction) {
				return false;
			}

			// Get optional maximum search distance.
			auto maxDistance = getOptionalParam<float>(params, "maxDistance", 0.0f);

			// Create our pick if it doesn't exist.
			if (rayTestCache == nullptr) {
				rayTestCache = NI::Pick::malloc();
			}

			// Or clean up results otherwise.
			else {
				rayTestCache->clearResults();
			}

			// Added ablity to use any node
			// In Lua Script use "root = tes3.mobilePlayer.firstPersonReference.sceneNode"
			// to have rayTest scan 1st person scene node
			rayTestCache->root = getOptionalParam<NI::Node*>(params, "root", TES3::Game::get()->worldRoot);

			// Are we finding all or the first?
			if (getOptionalParam<bool>(params, "findAll", false)) {
				rayTestCache->pickType = NI::PickType::FIND_ALL;
			}
			else {
				rayTestCache->pickType = NI::PickType::FIND_FIRST;
			}

			// Sort results by distance?
			if (getOptionalParam<bool>(params, "sort", true)) {
				rayTestCache->sortType = NI::PickSortType::SORT;
			}
			else {
				rayTestCache->sortType = NI::PickSortType::NO_SORT;
			}

			// Use triangle or model bounds for intersection?
			if (getOptionalParam<bool>(params, "useModelBounds", false)) {
				rayTestCache->intersectType = NI::PickIntersectType::BOUND_INTERSECT;
			}
			else {
				rayTestCache->intersectType = NI::PickIntersectType::TRIANGLE_INTERSECT;
			}

			// Use model coordinates or world coordinates?
			if (getOptionalParam<bool>(params, "useModelCoordinates", false)) {
				rayTestCache->coordinateType = NI::PickCoordinateType::MODEL_COORDINATES;
			}
			else {
				rayTestCache->coordinateType = NI::PickCoordinateType::WORLD_COORDINATES;
			}

			// Use the back side of a triangle? Note: Parameter name flipped!
			rayTestCache->frontOnly = !getOptionalParam<bool>(params, "useBackTriangles", false);

			// Observe app cull flag?
			rayTestCache->observeAppCullFlag = getOptionalParam<bool>(params, "observeAppCullFlag", true);

			// Determine what returned values we care about.
			rayTestCache->returnColor = getOptionalParam<bool>(params, "returnColor", false);
			rayTestCache->returnNormal = getOptionalParam<bool>(params, "returnNormal", true);
			rayTestCache->returnSmoothNormal = getOptionalParam<bool>(params, "returnSmoothNormal", false);
			rayTestCache->returnTexture = getOptionalParam<bool>(params, "returnTexture", false);

			// Default root nodes to ignore.
			std::vector<NI::AVObject*> ignoreRestoreList;
			if (rayTestIgnoreRoots.empty()) {
				auto weather = TES3::WorldController::get()->weatherController;
				auto world = TES3::Game::get()->worldRoot;
				rayTestIgnoreRoots.push_back(weather->sgRainRoot);
				rayTestIgnoreRoots.push_back(weather->sgSnowRoot);
				rayTestIgnoreRoots.push_back(weather->sgStormRoot);
				rayTestIgnoreRoots.push_back(world->getObjectByName("WorldProjectileRoot"));
				rayTestIgnoreRoots.push_back(world->getObjectByName("WorldSpellRoot"));
				rayTestIgnoreRoots.push_back(world->getObjectByName("WorldVFXRoot"));
			}
			for (const auto node : rayTestIgnoreRoots) {
				if (!node->getAppCulled()) {
					node->setAppCulled(true);
					ignoreRestoreList.push_back(node);
				}
			}

			// Allow defining references/nodes to ignore from the raytest.
			sol::optional<sol::table> ignoreTable = params["ignore"];
			if (ignoreTable) {
				for (const auto& kvPair : ignoreTable.value()) {
					sol::object value = kvPair.second;
					if (value.is<NI::Node>()) {
						auto node = value.as<NI::Node*>();
						if (!node->getAppCulled()) {
							// Cull the node, and add it to a list to set as unculled later.
							node->setAppCulled(true);
							ignoreRestoreList.push_back(node);
						}
					}
					else if (value.is<TES3::Reference>()) {
						auto reference = value.as<TES3::Reference*>();
						if (reference->sceneNode) {
							if (!reference->sceneNode->getAppCulled()) {
								// Cull the node, and add it to a list to set as unculled later.
								reference->sceneNode->setAppCulled(true);
								ignoreRestoreList.push_back(reference->sceneNode);
							}
						}
					}
					else {
						// Restore previous cull states.
						for (const auto node : ignoreRestoreList) {
							node->setAppCulled(false);
						}

						throw std::exception("tes3.rayTest: Invalid item in ignore list. Must contain only scene graph nodes or references.");
					}
				}
			}

			// Our pick is configured. Let's run it! (Use normalized direction for skinned mesh fix later.)
			auto pickSuccess = rayTestCache->pickObjects(&position.value(), &direction.value().normalized(), false, maxDistance);

			// Restore previous cull states.
			for (auto itt = ignoreRestoreList.begin(); itt != ignoreRestoreList.end(); itt++) {
				(*itt)->setAppCulled(false);
			}

			// Did we get any results?
			if (!pickSuccess || rayTestCache->results.empty()) {
				return sol::nil;
			}

			// Fix distances and missing intersection data for skinned nodes.
			auto distanceScale = 1.0 / direction.value().length();
			auto skinnedCorrection = (maxDistance != 0.0) ? maxDistance : 1.0;

			for (auto& r : rayTestCache->results) {
				if (r == nullptr) {
					continue;
				}

				// Adjust distance as if direction was not normalized.
				r->distance *= float(distanceScale);

				// Skinned nodes only have usable scaled distance data.
				if (r->object->isInstanceOfType(NI::RTTIStaticPtr::NiTriShape)) {
					auto node = static_cast<const NI::TriShape*>(r->object);
					if (node->skinInstance) {
						r->distance *= float(skinnedCorrection);
						r->intersection = position.value() + direction.value() * r->distance;
						r->normal = (r->intersection - node->worldBoundOrigin).normalized();
					}
				}
			}

			// Parameter: Ignore Skinned results.
			// Removes results of skinned objects
			if (getOptionalParam<bool>(params, "ignoreSkinned", false)) {
				// We're now in multi-result mode. We'll store these in a table.
				sol::table results = state.create_table();

				// Go through and clone the results in a way that will play nice.
				// Skip any results that have a skinInstance
				for (auto& r : rayTestCache->results) {
					if (r->object->isInstanceOfType(NI::RTTIStaticPtr::NiTriShape)) {
						auto node = static_cast<const NI::TriShape*>(r->object);
						if (!node->skinInstance) {
							results.add(r);
						}
					}
					else {
						results.add(r);
					}
				}

				// Return nothing if all results were skinned.
				if (results.empty()) {
					return sol::nil;
				}

				// Are we looking for a single result?
				if (rayTestCache->pickType == NI::PickType::FIND_FIRST) {
					return results[1];
				}

				return results;
			}
			else {
				// Treat results as normal
				// Are we looking for a single result?
				if (rayTestCache->pickType == NI::PickType::FIND_FIRST) {
					return sol::make_object(state, rayTestCache->results[0]);
				}

				// We're now in multi-result mode. We'll store these in a table.
				sol::table results = state.create_table();

				// Go through and clone the results in a way that will play nice.
				for (auto& r : rayTestCache->results) {
					results.add(r);
				}

				return results;
			}
		}

		bool isThirdPerson() {
			auto worldController = TES3::WorldController::get();
			if (worldController) {
				auto macp = worldController->getMobilePlayer();
				if (macp) {
					return macp->is3rdPerson();
				}
			}
			return false;
		}

		void tapKey(int key) {
			Stack::getInstance().pushLong(key);
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGETapKey);
		}

		void pushKey(int key) {
			Stack::getInstance().pushLong(key);
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEPushKey);
		}

		void releaseKey(int key) {
			Stack::getInstance().pushLong(key);
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEReleaseKey);
		}

		void hammerKey(int key) {
			Stack::getInstance().pushLong(key);
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEHammerKey);
		}

		void unhammerKey(int key) {
			Stack::getInstance().pushLong(key);
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEUnhammerKey);
		}

		void enableKey(int key) {
			Stack::getInstance().pushLong(key);
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEAllowKey);
		}

		void disableKey(int key) {
			Stack::getInstance().pushLong(key);
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEDisallowKey);
		}

		sol::optional<bool> hasCodePatchFeature(int id) {
			if (!mcp::hasFeaturesFound()) {
				return sol::optional<bool>();
			}
			return mcp::getFeatureEnabled(id);
		}

		sol::optional<int> getDaysInMonth(int month) {
			TES3::WorldController* worldController = TES3::WorldController::get();
			if (worldController) {
				return worldController->getDaysInMonth(month);
			}
			return sol::optional<int>();
		}

		sol::optional<int> getCumulativeDaysForMonth(int month) {
			TES3::WorldController* worldController = TES3::WorldController::get();
			if (worldController) {
				return worldController->getCumulativeDaysForMonth(month);
			}
			return sol::optional<int>();
		}

		double getSimulationTimestamp(sol::optional<bool> highPrecision) {
			if (highPrecision.value_or(true)) {
				TES3::WorldController* worldController = TES3::WorldController::get();
				if (worldController) {
					return worldController->getHighPrecisionSimulationTimestamp();
				}
			}
			else {
				return mwse::tes3::getSimulationTimestamp();
			}
			return 0.0;
		}

		TES3::EquipmentStack* getEquippedItem(sol::table params) {
			// Find our equipment based on the object given.
			TES3::IteratedList<TES3::EquipmentStack*>* equipment = NULL;
			sol::object actor = params["actor"];
			if (actor.valid()) {
				if (actor.is<TES3::Reference*>()) {
					equipment = actor.as<TES3::Reference*>()->getEquipment();
				}
				else if (actor.is<TES3::MobileActor*>()) {
					equipment = actor.as<TES3::MobileActor*>()->reference->getEquipment();
				}
				else if (actor.is<TES3::Actor*>()) {
					equipment = &actor.as<TES3::Actor*>()->equipment;
				}
			}
			else {
				throw std::exception("No actor provided.");
			}

			// Make sure we got the equipment.
			if (equipment == nullptr) {
				throw std::exception("The provided actor's equipment could not be resolved.");
			}

			// Get filter: Item Type
			sol::optional<int> filterObjectType;
			sol::optional<int> filterSlot;
			if (params["objectType"].valid()) {
				filterObjectType = (int)params["objectType"];

				// Get filter: Item Slot/Type
				if (params["slot"].valid()) {
					filterSlot = (int)params["slot"];
				}
				else if (params["type"].valid()) {
					filterSlot = (int)params["type"];
				}
			}

			// Get filter: Item Enchanted
			sol::optional<bool> filterEnchanted;
			if (params["enchanted"].valid()) {
				filterEnchanted = (bool)params["enchanted"];
			}

			// Loop through the items and pick the first one that matches our filters.
			for (auto itt = equipment->head; itt != nullptr; itt = itt->next) {
				TES3::Object* object = itt->data->object;

				// Filter object type.
				if (filterObjectType && filterObjectType.value() != object->objectType) {
					continue;
				}

				// Filter slot/type.
				if (filterSlot && filterSlot.value() != object->getType()) {
					continue;
				}

				// Filter enchanted.
				if (filterEnchanted) {
					TES3::Enchantment* enchantment = object->getEnchantment();
					if (filterEnchanted.value() == true && enchantment == nullptr) {
						continue;
					}
					else if (filterEnchanted.value() == false && enchantment != nullptr) {
						continue;
					}
				}

				// If we got this far we match all filters. Return the object.
				return itt->data;
			}

			return nullptr;
		}

		TES3::InputConfig* getInputBinding(int code) {
			if (code < TES3::KeyBind::FirstKey || code > TES3::KeyBind::LastKey) {
				return nullptr;
			}

			TES3::WorldController* worldController = TES3::WorldController::get();
			if (worldController) {
				TES3::InputController* inputController = worldController->inputController;
				if (inputController) {
					return &inputController->inputMaps[code];
				}
			}
			return nullptr;
		}

		TES3::Region* getRegion(sol::optional<sol::table> maybeParams) {
			TES3::DataHandler* dataHandler = TES3::DataHandler::get();
			if (dataHandler) {
				// Try to get the current cell's region first.
				if (dataHandler->currentCell) {
					TES3::Region* region = dataHandler->currentCell->getRegion();
					if (region) {
						return region;
					}

					// Try to get a cell region.
					if (dataHandler->currentCell->isInterior() && getOptionalParam(maybeParams, "useDoors", false)) {
						for (auto ref : dataHandler->currentCell->persistentRefs) {
							if (ref->baseObject->objectType == TES3::ObjectType::Door) {
								auto destination = ref->getAttachedTravelDestination();
								if (destination && destination->cell) {
									auto region = destination->cell->getRegion();
									if (region) {
										return region;
									}
								}
							}
						}
					}
				}

				// Otherwise fall back to the last exterior cell's region.
				if (dataHandler->lastExteriorCell) {
					return dataHandler->lastExteriorCell->getRegion();
				}
			}

			return nullptr;
		}

		TES3::Region* findRegion(const char* id) {
			TES3::DataHandler* dataHandler = TES3::DataHandler::get();
			if (dataHandler) {
				return dataHandler->nonDynamicData->getRegion(id);
			}
			return nullptr;
		}

		TES3::Weather* getCurrentWeather() {
			TES3::WorldController* worldController = TES3::WorldController::get();
			if (worldController) {
				return worldController->weatherController->currentWeather;
			}
			return nullptr;
		}

		sol::optional<TES3::Vector2> getCursorPosition() {
			TES3::WorldController* worldController = TES3::WorldController::get();
			if (worldController) {
				return TES3::Vector2(worldController->mouseController->position.x, worldController->mouseController->position.z);
			}
			return sol::optional<TES3::Vector2>();
		}

		TES3::Skill* getSkill(int skillID) {
			TES3::DataHandler* dataHandler = TES3::DataHandler::get();
			if (dataHandler) {
				return &dataHandler->nonDynamicData->skills[skillID];
			}
			return nullptr;
		}

		void removeEffects(sol::table params) {
			auto& luaManager = mwse::lua::LuaManager::getInstance();
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			TES3::Reference* reference = getOptionalParamExecutionReference(params);
			if (reference == nullptr) {
				throw std::exception("tes3.removeEffects: No reference parameter provided.");
			}

			int effect = getOptionalParam<int>(params, "effect", -1);
			int castType = getOptionalParam<int>(params, "castType", -1);
			int chance = getOptionalParam<int>(params, "chance", 100);
			if (chance > 100) {
				chance = 100;
			}
			else if (chance < 0) {
				chance = 0;
			}

			if (effect != -1) {
				TES3::WorldController::get()->magicInstanceController->removeSpellsByEffect(reference, effect, chance);
			}
			else if (castType != -1) {
				bool removeSpell = getOptionalParam<bool>(params, "removeSpell", castType != int(TES3::SpellCastType::Spell));
				TES3::WorldController::get()->magicInstanceController->clearSpellEffect(reference, castType, chance, removeSpell);
			}
			else {
				throw std::exception("tes3.removeEffects: Must pass either 'effect' or 'castType' parameter!");
			}
		}

		int getPlayerGold() {
			auto worldController = TES3::WorldController::get();
			if (worldController) {
				auto macp = TES3::WorldController::get()->getMobilePlayer();
				if (macp) {
					return macp->getGold();
				}
			}
			return 0;
		}

		bool triggerCrime(sol::table params) {
			auto& luaManager = mwse::lua::LuaManager::getInstance();
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			TES3::CrimeEvent crimeEvent;

			// Look at the given type.
			int crimeType = getOptionalParam<int>(params, "type", 3);
			if (crimeType < 1 || crimeType > 7) {
				throw std::exception("Invalid type given. Value must be between 1 and 7.");
			}
			crimeEvent.type = crimeType;

			// Also set the type string based on the crime committed.
			switch (crimeType) {
			case 1:
				crimeEvent.typeString = "attack";
				break;
			case 2:
				crimeEvent.typeString = "killing";
				break;
			case 3:
				crimeEvent.typeString = "stealing";
				break;
			case 4:
				crimeEvent.typeString = "pickpocket";
				crimeEvent.penalty = 25;
				break;
			case 5:
				crimeEvent.typeString = "theft";
				break;
			case 6:
				crimeEvent.typeString = "trespass";
				break;
			case 7:
				crimeEvent.typeString = "werewolf";
				break;
			}

			// Criminal is assumed to be the player if no value is supplied.
			TES3::MobileActor* criminal = getOptionalParamMobileActor(params, "criminal");
			if (criminal == nullptr) {
				criminal = TES3::WorldController::get()->getMobilePlayer();
			}
			crimeEvent.criminal = criminal;

			// Set some basic crime event data.
			crimeEvent.timestamp = float(timeGetTime());
			crimeEvent.position = criminal->reference->position;
			crimeEvent.penalty = getOptionalParam<int>(params, "value", crimeEvent.penalty);

			// Victim can be more complicated.
			sol::object victim = params["victim"];
			crimeEvent.victim = TES3::WorldController::get()->getMobilePlayer();
			if (victim.is<TES3::Faction>()) {
				crimeEvent.victimFaction = victim.as<TES3::Faction*>();
			}
			else if (victim.is<TES3::Actor>()) {
				crimeEvent.victim = TES3::WorldController::get()->getMobilePlayer();
				crimeEvent.victimFaction = victim.as<TES3::Actor*>()->getFaction();
				if (victim.is<TES3::NPC>()) {
					crimeEvent.victimBaseActor = victim.as<TES3::NPC*>();
				}
				else if (victim.is<TES3::NPCInstance>()) {
					crimeEvent.victimBaseActor = victim.as<TES3::NPCInstance*>()->baseNPC;
				}
			}
			else if (victim.is<TES3::MobileNPC>()) {
				TES3::MobileNPC* mach = victim.as<TES3::MobileNPC*>();
				crimeEvent.victim = mach;
				crimeEvent.victimFaction = mach->npcInstance->getFaction();
				crimeEvent.victimBaseActor = mach->npcInstance;
			}

			// Do detection and the like.
			bool forceDetection = getOptionalParam<bool>(params, "forceDetection", false);
			auto processManager = TES3::WorldController::get()->mobController->processManager;
			if (!forceDetection && processManager->detectPresence(crimeEvent.criminal)) {
				processManager->checkAlarmRadius(crimeEvent.victim, crimeEvent.witnesses);
			}

			// If we were detected, add it to the list.
			if (forceDetection || crimeEvent.witnesses->size() > 0) {
				auto crimeController = &crimeEvent.victim->crimesA;
				crimeController->insertCrime(&crimeEvent);
			}

			return true;
		}

		NI::Pointer<NI::Object> loadMesh(const char* relativePath, sol::optional<bool> useCached) {
			std::string path = "Meshes\\";
			path += relativePath;

			if (useCached.value_or(true)) {
				return TES3::DataHandler::get()->nonDynamicData->meshData->loadMesh(path.c_str());
			}
			else {
				// Create a non-cached version by calling the constructors manually without the usual checking.
				using NodeType = TES3::HashMap<char*, NI::Object*>::Node;
				const auto node_ctor = reinterpret_cast<void(__thiscall*)(NodeType*, const char*)>(0x4ED6C0);
				const auto node_dtor = reinterpret_cast<void(__thiscall*)(NodeType*)>(0x4EDB70);

				auto node = new NodeType();
				node_ctor(node, path.c_str());

				NI::Pointer<NI::Object> result = node->value;

				node_dtor(node);
				delete node;

				return result;
			}

			return nullptr;
		};

		NI::Pointer<NI::SourceTexture> loadSourceTexture(const char* relativePath, sol::optional<bool> useCached) {
			std::string path = "Textures\\";
			path += relativePath;

			if (useCached.value_or(true)) {
				auto dataHandler = TES3::DataHandler::get();
				if (dataHandler == nullptr) {
					throw std::runtime_error("tes3dataHandler is not yet initialized.");
				}
				return dataHandler->loadSourceTexture(path.c_str());
			}
			else {
				return NI::SourceTexture::createFromPath(path.c_str());
			}
		}

		bool playVoiceover(sol::table params) {
			auto& luaManager = mwse::lua::LuaManager::getInstance();
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Get the actor that we're going to make say something.
			auto actor = getOptionalParamMobileActor(params, "actor");
			if (actor == nullptr) {
				return false;
			}

			// Accept either a string or a number as the voiceover id.
			int voiceover = -1;
			sol::object voiceoverObject = params["voiceover"];
			if (voiceoverObject.is<const char*>()) {
				sol::object result = state["tes3"]["voiceover"][voiceoverObject.as<const char*>()];
				if (result.is<int>()) {
					voiceover = result.as<int>();
				}
			}
			else if (voiceoverObject.is<int>()) {
				voiceover = voiceoverObject.as<int>();
			}

			// Validate the input.
			if (voiceover < TES3::Voiceover::First || voiceover > TES3::Voiceover::Last) {
				return false;
			}

			actor->playVoiceover(voiceover);
			return true;
		}

		bool getLocked(sol::table params) {
			TES3::Reference* reference = getOptionalParamExecutionReference(params);
			if (reference == nullptr) {
				return false;
			}

			auto node = reference->getAttachedLockNode();
			if (node == nullptr) {
				return false;
			}

			return node->locked;
		}

		bool setLockLevel(sol::table params) {
			TES3::Reference* reference = getOptionalParamExecutionReference(params);
			if (reference == nullptr) {
				return false;
			}

			auto node = reference->getOrCreateLockNode();
			if (node == nullptr) {
				return false;
			}

			int level = getOptionalParam<int>(params, "level", -1);
			if (level >= 0) {
				node->lockLevel = level;
				reference->setObjectModified(true);
				return true;
			}

			return false;
		}

		sol::optional<int> getLockLevel(sol::table params) {
			TES3::Reference* reference = getOptionalParamExecutionReference(params);
			if (reference == nullptr) {
				return sol::optional<int>();
			}

			auto node = reference->getAttachedLockNode();
			if (node == nullptr) {
				return sol::optional<int>();
			}

			return node->lockLevel;
		}

		bool lock(sol::table params) {
			auto reference = getOptionalParamExecutionReference(params);
			if (reference == nullptr) {
				return false;
			}

			// Manage state for doors.
			if (reference->baseObject->objectType == TES3::ObjectType::Door) {
				// Clear door opening/closing flags.
				reference->clearActionFlag(TES3::ActionFlags::DoorOpening | TES3::ActionFlags::DoorClosing | TES3::ActionFlags::DoorJammedOpening | TES3::ActionFlags::DoorJammedClosing);

				// Reset orientation.
				auto orientationAttachment = static_cast<TES3::NewOrientationAttachment*>(reference->getAttachment(TES3::AttachmentType::NewOrientation));
				if (orientationAttachment) {
					orientationAttachment->orientation.z = reference->orientation.z;
				}

				// Update the scene graph with the new orientation.
				if (reference->sceneNode) {
					TES3::Matrix33 tempOutArg;
					reference->sceneNode->setLocalRotationMatrix(reference->updateSceneMatrix(&tempOutArg));
					reference->sceneNode->update();
				}
			}

			// Get or create our lock node.
			auto lockNode = reference->getOrCreateLockNode();
			if (lockNode == nullptr) {
				return false;
			}

			// Set the lock level if one was provided.
			int lockLevel = getOptionalParam<int>(params, "level", -1);
			if (lockLevel >= 0) {
				lockNode->lockLevel = lockLevel;
			}

			// Set the locked state, and return true if it changed.
			reference->setObjectModified(true);
			if (!lockNode->locked) {
				lockNode->locked = true;
				return true;
			}
			else {
				return false;
			}
		}

		bool unlock(sol::table params) {
			TES3::Reference* reference = getOptionalParamExecutionReference(params);
			if (reference == nullptr) {
				return false;
			}

			auto node = reference->getAttachedLockNode();
			if (node == nullptr) {
				return false;
			}

			if (node->locked) {
				node->locked = false;
				reference->setObjectModified(true);
				return true;
			}
			else {
				return false;
			}
		}

		TES3::Spell* getTrap(sol::table params) {
			TES3::Reference* reference = getOptionalParamExecutionReference(params);
			if (reference == nullptr) {
				return nullptr;
			}

			auto node = reference->getAttachedLockNode();
			if (node == nullptr) {
				return nullptr;
			}

			return node->trap;
		}

		bool setTrap(sol::table params) {
			auto reference = getOptionalParamExecutionReference(params);
			if (reference == nullptr) {
				return false;
			}

			// Only accept containers or doors.
			auto objectType = reference->baseObject->objectType;
			if (objectType != TES3::ObjectType::Container && objectType != TES3::ObjectType::Door) {
				return false;
			}

			// Get or create our lock node.
			auto lockNode = reference->getOrCreateLockNode();
			if (lockNode == nullptr) {
				return false;
			}

			lockNode->trap = getOptionalParamSpell(params, "spell");
			reference->setObjectModified(true);
			return true;
		}

		bool checkMerchantTradesItem(sol::table params) {
			auto reference = getOptionalParamExecutionReference(params);
			if (reference == nullptr) {
				throw std::invalid_argument("Invalid reference parameter provided: Can't be nil.");
			}

			TES3::Item* item = getOptionalParamObject<TES3::Item>(params, "item");
			if (item == nullptr) {
				throw std::invalid_argument("Invalid item parameter provided: Can't be nil.");
			}

			auto actor = reinterpret_cast<TES3::Actor*>(reference->baseObject);
			if (!actor->isActor()) {
				throw std::invalid_argument("Invalid reference parameter provided: Base object must be an actor.");
			}

			return actor->tradesItemType(item->objectType);
		}

		std::pair<bool, TES3::DialogueInfo*> checkMerchantOffersService(sol::table params) {
			auto reference = getOptionalParamExecutionReference(params);
			if (reference == nullptr) {
				throw std::invalid_argument("Invalid reference parameter provided: Can't be nil.");
			}

			auto actor = reinterpret_cast<TES3::Actor*>(reference->baseObject);
			if (!actor->isActor()) {
				throw std::invalid_argument("Invalid reference parameter provided: Base object must be an actor.");
			}

			// Check if the actor is offering the specific service.
			sol::optional<int> service = params["service"];
			if (service && !actor->offersService(service.value())) {
				return { false, nullptr };
			}

			// Check for service refusal response.
			const int serviceRefusalPage = 7;
			auto dialogue = TES3::Dialogue::getDialogue((int)TES3::DialogueType::Persuasion, serviceRefusalPage);
			auto serviceRefusal = dialogue->getFilteredInfo(actor, reference, true);

			return { serviceRefusal == nullptr, serviceRefusal };
		}

		sol::optional<int> getJournalIndex(sol::table params) {
			TES3::Dialogue* journal = getOptionalParamDialogue(params, "id");
			if (journal == nullptr || journal->type != TES3::DialogueType::Journal) {
				return sol::optional<int>();
			}

			return journal->journalIndex;
		}

		bool setJournalIndex(sol::table params) {
			TES3::Dialogue* journal = getOptionalParamDialogue(params, "id");
			if (journal == nullptr || journal->type != TES3::DialogueType::Journal) {
				return false;
			}

			sol::optional<int> index = params["index"];
			if (!index) {
				return false;
			}

			if (!journal->setJournalIndex(index.value())) {
				return false;
			}

			sol::optional<bool> showMessage = params["showMessage"];
			if (showMessage.value_or(false) && tes3::ui::getMenuNode(*reinterpret_cast<short*>(0x7D3442)) == nullptr) {
				TES3::UI::showMessageBox(TES3::DataHandler::get()->nonDynamicData->GMSTs[TES3::GMST::sJournalEntry]->value.asString);
			}

			return true;
		}

		bool updateJournal(sol::table params) {
			TES3::Dialogue* journal = getOptionalParamDialogue(params, "id");
			if (journal == nullptr || journal->type != TES3::DialogueType::Journal) {
				return false;
			}

			sol::optional<int> index = params["index"];
			if (!index) {
				return false;
			}

			TES3::MobileActor* actor = getOptionalParamMobileActor(params, "speaker");
			if (actor == nullptr) {
				actor = TES3::WorldController::get()->getMobilePlayer();
			}

			if (!journal->addToJournal(index.value(), actor)) {
				return false;
			}

			sol::optional<bool> showMessage = params["showMessage"];
			if (showMessage.value_or(true) && tes3::ui::getMenuNode(*reinterpret_cast<short*>(0x7D3442)) == nullptr) {
				TES3::UI::showMessageBox(TES3::DataHandler::get()->nonDynamicData->GMSTs[TES3::GMST::sJournalEntry]->value.asString);
			}

			return true;
		}

		void addJournalEntry(sol::table params) {
			auto worldController = TES3::WorldController::get();
			if (worldController == nullptr) {
				throw std::runtime_error("Function called before world controller is loaded.");
			}

			auto journalHTML = worldController->journalHTML;
			if (journalHTML == nullptr || journalHTML->data == nullptr) {
				throw std::runtime_error("No current journal data exists.");
			}

			const char* text = getOptionalParam<const char*>(params, "text", nullptr);
			if (text == nullptr) {
				throw std::invalid_argument("Invalid 'text' parameter provided.");
			}

			journalHTML->writeTimestampedEntry(text);

			sol::optional<bool> showMessage = params["showMessage"];
			if (showMessage.value_or(true)) {
				journalHTML->showJournalUpdateNotification();
			}
		}

		bool getFileExists(const char* path) {
			return tes3::resolveAssetPath(path) != 0;
		}

		sol::optional<std::tuple<std::string, std::string>> getFileSource(const char* path) {
			char buffer[512];
			int result = tes3::resolveAssetPath(path, buffer);

			if (result == 1) {
				return std::make_tuple("file", buffer);
			}
			else if (result == 2) {
				return std::make_tuple("bsa", buffer);
			}

			return sol::optional<std::tuple<std::string, std::string>>();
		}

		// Very slow method to get an INFO record by its ID.
		TES3::DialogueInfo* getDialogueInfo(sol::table params) {
			TES3::Dialogue* dialogue = getOptionalParamDialogue(params, "dialogue");
			const char* id = getOptionalParam<const char*>(params, "id", nullptr);
			if (dialogue == nullptr || id == nullptr) {
				return nullptr;
			}

			for (auto itt = dialogue->info.head; itt; itt = itt->next) {
				auto dialogueInfo = itt->data;
				if (!dialogueInfo->loadId()) {
					continue;
				}

				if (_strcmpi(id, dialogueInfo->loadLinkNode->name) == 0) {
					dialogueInfo->unloadId();
					return dialogueInfo;
				}

				dialogueInfo->unloadId();
			}

			return nullptr;
		}

		TES3::Cell* getCell(sol::table params) {
			// If we were given a name, try that.
			sol::optional<const char*> cellId = params["id"];
			if (cellId) {
				return TES3::DataHandler::get()->nonDynamicData->getCellByName(cellId.value());
			}

			// Otherwise try to use X/Y.
			return TES3::DataHandler::get()->nonDynamicData->getCellByGrid(params["x"], params["y"]);
		}

		void fadeIn(sol::optional<sol::table> params) {
			TES3::Fader* fader = getOptionalParam(params, "fader", TES3::WorldController::get()->transitionFader);
			if (fader == nullptr) {
				return;
			}

			float duration = getOptionalParam(params, "duration", 1.0f);
			fader->fadeTo(0.0f, duration);
		}

		void fadeOut(sol::optional<sol::table> params) {
			TES3::Fader* fader = getOptionalParam(params, "fader", TES3::WorldController::get()->transitionFader);
			if (fader == nullptr) {
				return;
			}

			float duration = getOptionalParam(params, "duration", 1.0f);
			fader->fadeTo(1.0f, duration);
		}

		void fadeTo(sol::optional<sol::table> params) {
			TES3::Fader* fader = getOptionalParam(params, "fader", TES3::WorldController::get()->transitionFader);
			if (fader == nullptr) {
				return;
			}

			float value = getOptionalParam(params, "value", 1.0f);
			float duration = getOptionalParam(params, "duration", 1.0f);
			fader->fadeTo(value, duration);
		}

		void setStatistic(sol::table params) {
			auto& luaManager = mwse::lua::LuaManager::getInstance();
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Figure out our mobile object, in case someone gives us a reference instead.
			sol::userdata maybeMobile = params["reference"];
			if (maybeMobile.is<TES3::Reference>()) {
				maybeMobile = maybeMobile["mobile"];
			}

			// Make sure our object is of the right type.
			if (!maybeMobile.is<TES3::MobileActor>()) {
				throw std::invalid_argument("tes3.setStatistic: Could not resolve parameter 'reference'.");
			}

			// Try to get our statistic.
			TES3::MobileActor* mobile = maybeMobile.as<TES3::MobileActor*>();
			TES3::Statistic* statistic = nullptr;
			sol::optional<const char*> statisticName = params["name"];
			sol::optional<int> statisticSkill = params["skill"];
			sol::optional<int> statisticAttribute = params["attribute"];
			if (statisticSkill) {
				if (mobile->actorType == TES3::MobileActorType::Creature) {
					if (statisticSkill.value() >= TES3::CreatureSkillID::FirstSkill && statisticSkill.value() <= TES3::CreatureSkillID::LastSkill) {
						statistic = &static_cast<TES3::MobileCreature*>(mobile)->skills[statisticSkill.value()];
					}
					else {
						std::stringstream ss;
						ss << "tes3.setStatistic: Invalid skill index " << std::dec << statisticSkill.value() << " for creature.";
						throw std::invalid_argument(ss.str());
					}
				}
				else {
					if (statisticSkill.value() >= TES3::SkillID::FirstSkill && statisticSkill.value() <= TES3::SkillID::LastSkill) {
						statistic = &static_cast<TES3::MobileNPC*>(mobile)->skills[statisticSkill.value()];
					}
					else {
						std::stringstream ss;
						ss << "tes3.setStatistic: Invalid skill index " << std::dec << statisticSkill.value() << " for NPC.";
						throw std::invalid_argument(ss.str());
					}
				}
			}
			else if (statisticAttribute) {
				if (statisticAttribute.value() >= TES3::Attribute::FirstAttribute && statisticAttribute.value() <= TES3::Attribute::LastAttribute) {
					statistic = &mobile->attributes[statisticAttribute.value()];
				}
				else {
					std::stringstream ss;
					ss << "tes3.setStatistic: Invalid attribute index " << std::dec << statisticSkill.value() << ".";
					throw std::invalid_argument(ss.str());
				}
			}
			else if (statisticName) {
				sol::optional<TES3::Statistic*> maybeStatistic = maybeMobile[statisticName.value()];
				if (maybeStatistic) {
					// Further check is required as sol may convert nil to nullptr.
					statistic = maybeStatistic.value();
				}
				if (statistic == nullptr) {
					std::stringstream ss;
					ss << "tes3.setStatistic: No statistic named \"" << statisticName.value() << "\" could be found.";
					throw std::invalid_argument(ss.str());
				}
			}

			// This case shouldn't be hit.
			if (statistic == nullptr) {
				throw std::invalid_argument("tes3.setStatistic: No statistic resolved.");
			}

			// Retype our variables to something more friendly, and get additional params.
			sol::optional<bool> limit = params["limit"];

			sol::optional<float> current = params["current"];
			sol::optional<float> base = params["base"];
			sol::optional<float> value = params["value"];

			// Edit both.
			if (value) {
				statistic->setBaseAndCurrent(value.value());
			}
			// If we're given a current value, modify it.
			else if (current) {
				statistic->setCurrentCapped(current.value(), limit.value_or(false));
			}
			// If we're given a base value, modify it.
			else if (base) {
				statistic->setBase(base.value());
			}
			else {
				throw std::invalid_argument("tes3.setStatistic: No edit mode provided, missing parameter 'current' or 'base' or 'value'.");
			}

			// Update any derived statistics.
			mobile->updateDerivedStatistics(statistic);

			// Ensure the reference is flagged as modified.
			if (mobile->reference) {
				mobile->reference->setObjectModified(true);
			}

			// If this was on the player update any associated GUI widgets.
			if (mobile->actorType == TES3::MobileActorType::Player) {
				// Manually update health/magicka/fatigue UI elements.
				if (statistic == &mobile->health) {
					TES3::UI::updateHealthFillBar(statistic->current, statistic->base);
				}
				else if (statistic == &mobile->magicka) {
					TES3::UI::updateMagickaFillBar(statistic->current, statistic->base);
				}
				else if (statistic == &mobile->fatigue) {
					TES3::UI::updateFatigueFillBar(statistic->current, statistic->base);
				}
				else {
					// Check to see if an attribute was edited.
					for (size_t i = TES3::Attribute::FirstAttribute; i <= TES3::Attribute::LastAttribute; i++) {
						if (statistic == &mobile->attributes[i]) {
							TES3::UI::updateEncumbranceBar();
							TES3::UI::updatePlayerAttribute(statistic->getCurrent(), i);
							break;
						}
					}
				}

				TES3::UI::updateStatsPane();
			}
		}

		void modStatistic(sol::table params) {
			auto& luaManager = mwse::lua::LuaManager::getInstance();
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Figure out our mobile object, in case someone gives us a reference instead.
			sol::userdata maybeMobile = params["reference"];
			if (maybeMobile.is<TES3::Reference>()) {
				maybeMobile = maybeMobile["mobile"];
			}

			// Make sure our object is of the right type.
			if (!maybeMobile.is<TES3::MobileActor>()) {
				throw std::invalid_argument("tes3.modStatistic: Could not resolve parameter 'reference'.");
			}

			// Try to get our statistic.
			TES3::MobileActor* mobile = maybeMobile.as<TES3::MobileActor*>();
			TES3::Statistic* statistic = nullptr;
			sol::optional<const char*> statisticName = params["name"];
			sol::optional<int> statisticSkill = params["skill"];
			sol::optional<int> statisticAttribute = params["attribute"];
			if (statisticSkill) {
				if (mobile->actorType == TES3::MobileActorType::Creature) {
					if (statisticSkill.value() >= TES3::CreatureSkillID::FirstSkill && statisticSkill.value() <= TES3::CreatureSkillID::LastSkill) {
						statistic = &static_cast<TES3::MobileCreature*>(mobile)->skills[statisticSkill.value()];
					}
					else {
						std::stringstream ss;
						ss << "tes3.modStatistic: Invalid skill index " << std::dec << statisticSkill.value() << " for creature.";
						throw std::invalid_argument(ss.str());
					}
				}
				else {
					if (statisticSkill.value() >= TES3::SkillID::FirstSkill && statisticSkill.value() <= TES3::SkillID::LastSkill) {
						statistic = &static_cast<TES3::MobileNPC*>(mobile)->skills[statisticSkill.value()];
					}
					else {
						std::stringstream ss;
						ss << "tes3.modStatistic: Invalid skill index " << std::dec << statisticSkill.value() << " for NPC.";
						throw std::invalid_argument(ss.str());
					}
				}
			}
			else if (statisticAttribute) {
				if (statisticAttribute.value() >= TES3::Attribute::FirstAttribute && statisticAttribute.value() <= TES3::Attribute::LastAttribute) {
					statistic = &mobile->attributes[statisticAttribute.value()];
				}
				else {
					std::stringstream ss;
					ss << "tes3.modStatistic: Invalid attribute index " << std::dec << statisticSkill.value() << ".";
					throw std::invalid_argument(ss.str());
				}
			}
			else if (statisticName) {
				sol::optional<TES3::Statistic*> maybeStatistic = maybeMobile[statisticName.value()];
				if (maybeStatistic) {
					// Further check is required as sol may convert nil to nullptr.
					statistic = maybeStatistic.value();
				}
				if (statistic == nullptr) {
					std::stringstream ss;
					ss << "tes3.modStatistic: No statistic named \"" << statisticName.value() << "\" could be found.";
					throw std::invalid_argument(ss.str());
				}
			}

			// This case shouldn't be hit.
			if (statistic == nullptr) {
				throw std::invalid_argument("tes3.modStatistic: No statistic resolved.");
			}

			// Retype our variables to something more friendly, and get additional params.
			sol::optional<bool> limit = params["limit"];

			sol::optional<float> current = params["current"];
			sol::optional<float> base = params["base"];
			sol::optional<float> value = params["value"];

			// Edit both.
			if (value) {
				statistic->modBaseCapped(value.value(), limit.value_or(false), limit.value_or(false));
				statistic->modCurrentCapped(value.value(), limit.value_or(false), limit.value_or(false), limit.value_or(false));
			}
			// If we're given a current value, modify it.
			else if (current) {
				statistic->modCurrentCapped(current.value(), limit.value_or(false), limit.value_or(false), limit.value_or(false));
			}
			// If we're given a base value, modify it.
			else if (base) {
				statistic->modBaseCapped(base.value(), limit.value_or(false), limit.value_or(false));
			}
			else {
				throw std::invalid_argument("tes3.modStatistic: No edit mode provided, missing parameter 'current' or 'base' or 'value'.");
				return;
			}

			// Update any derived statistics.
			mobile->updateDerivedStatistics(statistic);

			// Ensure the reference is flagged as modified.
			if (mobile->reference) {
				mobile->reference->setObjectModified(true);
			}

			// If this was on the player update any associated GUI widgets.
			if (mobile->actorType == TES3::MobileActorType::Player) {
				// Manually update health/magicka/fatigue UI elements.
				if (statistic == &mobile->health) {
					TES3::UI::updateHealthFillBar(statistic->current, statistic->base);
				}
				else if (statistic == &mobile->magicka) {
					TES3::UI::updateMagickaFillBar(statistic->current, statistic->base);
				}
				else if (statistic == &mobile->fatigue) {
					TES3::UI::updateFatigueFillBar(statistic->current, statistic->base);
				}
				else {
					// Check to see if an attribute was edited.
					for (size_t i = TES3::Attribute::FirstAttribute; i <= TES3::Attribute::LastAttribute; i++) {
						if (statistic == &mobile->attributes[i]) {
							TES3::UI::updateEncumbranceBar();
							TES3::UI::updatePlayerAttribute(statistic->getCurrent(), i);
							break;
						}
					}
				}

				TES3::UI::updateStatsPane();
			}
		}

		bool runLegacyScript(sol::table params) {
			TES3::Script* script = getOptionalParamScript(params, "script");
			if (script == nullptr) {
				script = TES3::WorldController::get()->scriptCompileAndRun;
			}

			TES3::ScriptCompiler* compiler = TES3::WorldController::get()->menuController->scriptCompiler;
			int source = getOptionalParam<int>(params, "source", TES3::CompilerSource::Default);
			if (source < TES3::CompilerSource::FirstSource || source > TES3::CompilerSource::LastSource) {
				return false;
			}

			const char* command = getOptionalParam<const char*>(params, "command", nullptr);
			if (command == nullptr) {
				return false;
			}

			TES3::ScriptVariables* variables = getOptionalParam<TES3::ScriptVariables*>(params, "variables", nullptr);

			TES3::Reference* reference = getOptionalParamExecutionReference(params);
			if (reference && variables == nullptr) {
				variables = reference->getScriptVariables();
			}

			TES3::Dialogue* dialogue = getOptionalParamDialogue(params, "dialogue");
			TES3::DialogueInfo* dialogueInfo = getOptionalParam< TES3::DialogueInfo*>(params, "info", nullptr);
			if (dialogue == nullptr || dialogueInfo == nullptr) {
				dialogue = nullptr;
				dialogueInfo = nullptr;
				source = TES3::CompilerSource::Default;
			}
			else {
				source = TES3::CompilerSource::Dialogue;
			}

			TES3::DataHandler::suppressThreadLoad = true;
			script->doCommand(compiler, command, source, reference, variables, dialogueInfo, dialogue);
			TES3::DataHandler::suppressThreadLoad = false;

			return true;
		}

		bool force1stPerson() {
			auto worldController = TES3::WorldController::get();
			if (worldController == nullptr) {
				throw std::runtime_error("Function called before world controller is initialized.");
			}

			auto macp = worldController->getMobilePlayer();
			if (macp == nullptr) {
				throw std::runtime_error("Function called while mobile player is unavailable.");
			}

			auto animController = macp->animationController.asPlayer;
			if (animController == nullptr) {
				throw std::runtime_error("Function called while mobile player animation controller is unavailable.");
			}

			return animController->force1stPerson();
		}

		bool force3rdPerson() {
			auto worldController = TES3::WorldController::get();
			if (worldController == nullptr) {
				throw std::runtime_error("Function called before world controller is initialized.");
			}

			auto macp = worldController->getMobilePlayer();
			if (macp == nullptr) {
				throw std::runtime_error("Function called while mobile player is unavailable.");
			}

			auto animController = macp->animationController.asPlayer;
			if (animController == nullptr) {
				throw std::runtime_error("Function called while mobile player animation controller is unavailable.");
			}

			return animController->force3rdPerson();
		}

		sol::object getActiveCells() {
			auto dataHandler = TES3::DataHandler::get();
			if (dataHandler == nullptr) {
				return sol::nil;
			}

			auto& luaManager = mwse::lua::LuaManager::getInstance();
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;
			sol::table result = state.create_table();

			if (dataHandler->currentInteriorCell) {
				result[1] = dataHandler->currentInteriorCell;
			}
			else {
				int exteriorCount = 0;
				for (size_t i = 0; i < 9; i++) {
					auto cellDataPointer = dataHandler->exteriorCellData[i];
					if (cellDataPointer && cellDataPointer->loadingFlags >= 1) {
						exteriorCount++;
						result[exteriorCount] = cellDataPointer->cell;
					}
				}
			}

			return result;
		}

		bool positionCell(sol::table params) {
			auto worldController = TES3::WorldController::get();
			auto macp = worldController->getMobilePlayer();
			auto playerRef = macp->reference;

			// Get the target that we're working with.
			auto reference = getOptionalParamExecutionReference(params);
			if (reference == nullptr) {
				reference = playerRef;
			}

			// Get the position.
			sol::optional<TES3::Vector3> position = getOptionalParamVector3(params, "position");
			if (!position) {
				return false;
			}

			// Get the orientation.
			sol::optional<TES3::Vector3> orientation = getOptionalParamVector3(params, "orientation");
			if (!orientation) {
				orientation = reference->orientation;
			}

			// Get the cell.
			TES3::Cell* cell = getOptionalParamCell(params, "cell");
			if (cell == nullptr) {
				// Try to find an exterior cell from the position.
				int gridX = TES3::Cell::toGridCoord(position.value().x);
				int gridY = TES3::Cell::toGridCoord(position.value().y);
				cell = TES3::DataHandler::get()->nonDynamicData->getCellByGrid(gridX, gridY);

				if (cell == nullptr) {
					return false;
				}
			}

			// Are we doing a simple reposition?
			if (cell == reference->getCell() && !getOptionalParam<bool>(params, "forceCellChange", false)) {
				reference->setPosition(&position.value());
				reference->setOrientation(&orientation.value());
				return true;
			}

			// Are we dealing with the player? If so, use the special functions.
			if (reference == playerRef) {
				sol::optional<bool> suppressFaderOpt = params["suppressFader"];
				bool suppressFader = suppressFaderOpt.value_or(false);
				bool faderInitialState = TES3::DataHandler::get()->useCellTransitionFader;

				if (suppressFader) {
					TES3::DataHandler::get()->useCellTransitionFader = false;
				}

				sol::optional<bool> teleportCompanions = params["teleportCompanions"];
				if (teleportCompanions.value_or(true) && macp->listFriendlyActors.size() > 0) {
					TES3::DataHandler::suppressThreadLoad = true;
					const auto TES3_cellChangeWithCompanions = reinterpret_cast<void(__cdecl*)(TES3::Vector3, TES3::Vector3, TES3::Cell*)>(0x45C9B0);
					TES3_cellChangeWithCompanions(position.value(), orientation.value(), cell);
					TES3::DataHandler::suppressThreadLoad = false;
				}
				else {
					TES3::DataHandler::suppressThreadLoad = true;
					const auto TES3_cellChange = reinterpret_cast<void(__cdecl*)(TES3::Vector3, TES3::Vector3, TES3::Cell*, int)>(0x45CEF0);
					sol::optional<bool> flag = params["flag"];
					TES3_cellChange(position.value(), orientation.value(), cell, flag.value_or(true));
					TES3::DataHandler::suppressThreadLoad = false;
				}

				if (suppressFader) {
					TES3::DataHandler::get()->useCellTransitionFader = faderInitialState;
				}
			}
			else {
				TES3::DataHandler::suppressThreadLoad = true;
				reference->relocate(cell, &position.value(), orientation.value().z);
				TES3::DataHandler::suppressThreadLoad = false;
			}

			// Ensure the reference and cell is flagged as modified.
			reference->setObjectModified(true);
			cell->setObjectModified(true);

			return true;
		}

		int getLanguageCode() {
			return reinterpret_cast<int(__stdcall*)()>(0x4678F0)();
		}

		TES3::Vector3 getLastExteriorPosition() {
			return TES3::DataHandler::get()->getLastExteriorPosition();
		}

		sol::optional<std::string> getLanguage() {
			switch (getLanguageCode()) {
			case 0:
				return "eng";
			case 1:
				return "fra";
			case 2:
				return "deu";
			case 3:
				return "rus";
			}

			return sol::optional<std::string>();
		}

		bool addSoulGem(sol::table params) {
			TES3::Misc* item = getOptionalParamObject<TES3::Misc>(params, "item");
			if (item == nullptr) {
				return false;
			}

			auto data = mwse::tes3::addCustomSoulGem(item);
			if (data == nullptr) {
				return false;
			}

			return true;
		}

		void addArmorSlot(sol::table params) {
			sol::optional<int> slot = params["slot"];
			if (!slot || (slot.value() >= TES3::ArmorSlot::First && slot.value() <= TES3::ArmorSlot::Last) || mwse::tes3::getArmorSlotData(slot.value())) {
				throw std::exception("tes3.addArmorSlot: Invalid slot. An unusued slot must be provided.");
			}

			sol::optional<const char*> name = params["name"];
			if (!name || name.value() == nullptr) {
				throw std::exception("tes3.addArmorSlot: No name provided for slot.");
			}

			sol::optional<float> weight = params["weight"];
			sol::optional<float> armorScalar = params["scalar"];

			auto slotData = std::make_shared<TES3::ArmorSlotData>();
			slotData->slot = slot.value();
			slotData->name = name.value();
			slotData->weight = weight.value_or(0.0f);
			slotData->armorScalar = armorScalar.value_or(0.1f);

			mwse::tes3::setArmorSlotData(slotData);
		}

		void addClothingSlot(sol::table params) {
			sol::optional<int> slot = params["slot"];
			if (!slot || (slot.value() >= TES3::ClothingSlot::First && slot.value() <= TES3::ClothingSlot::Last) || mwse::tes3::getClothingSlotData(slot.value())) {
				throw std::exception("tes3.addClothingSlot: Invalid slot. An unusued slot must be provided.");
			}

			if (slot.value() < 0 || slot.value() > UINT8_MAX) {
				throw std::exception("tes3.addClothingSlot: Invalid slot. Value must be between 10 and 255.");
			}

			sol::optional<const char*> name = params["name"];
			if (!name || name.value() == nullptr) {
				throw std::exception("tes3.addClothingSlot: No name provided for slot.");
			}

			auto slotData = std::make_shared<TES3::ClothingSlotData>();
			slotData->slot = slot.value();
			slotData->name = name.value();

			mwse::tes3::setClothingSlotData(slotData);
		}

		// WARNING: This function doesn't persist through saves correctly and shouldn't be used.
		TES3::Cell* createCell(sol::table params) {
			auto nonDynamicData = TES3::DataHandler::get()->nonDynamicData;

			TES3::Cell* cell = nullptr;

			sol::optional<int> gridX = params["gridX"];
			sol::optional<int> gridY = params["gridY"];
			if (gridX && gridY) {
				auto existingCell = nonDynamicData->getCellByGrid(gridX.value(), gridY.value());
				if (existingCell) {
					mwse::log::getLog() << "Could not create cell at coordinates <" << gridX.value() << ", " << gridY.value() << ">. Cell already exists at that location." << std::endl;
					return nullptr;
				}

				cell = TES3::Cell::create();
				cell->setCellFlag(TES3::CellFlag::Interior, false);
				cell->setGridX(gridX.value());
				cell->setGridY(gridY.value());
				cell->setName("");
			}
			else {
				sol::optional<const char*> name = params["name"];
				if (!name) {
					mwse::log::getLog() << "Could not create cell. Interior cells must have a name." << std::endl;
					return nullptr;
				}

				auto existingCell = nonDynamicData->getCellByName(name.value());
				if (existingCell) {
					mwse::log::getLog() << "Could not create cell \"" << name.value() << "\". Cell already exists with the given name." << std::endl;
					return nullptr;
				}

				cell = TES3::Cell::create();
				cell->setCellFlag(TES3::CellFlag::Interior, true);
				cell->setName(name.value());
			}

			cell->setObjectModified(true);

			nonDynamicData->cells->push_back(cell);

			return cell;
		}

		TES3::Reference* createReference(sol::table params) {
			auto dataHandler = TES3::DataHandler::get();

			// Get the object we are going to create a reference for.
			auto object = getOptionalParamObject<TES3::PhysicalObject>(params, "object");
			if (object == nullptr) {
				throw std::invalid_argument("Invalid 'object' parameter provided.");
			}

			// Make sure we have a base object.
			object = static_cast<TES3::PhysicalObject*>(object->getBaseObject());

			// Get the position.
			auto maybePosition = getOptionalParamVector3(params, "position");
			if (!maybePosition) {
				throw std::invalid_argument("Invalid 'position' parameter provided.");
			}

			// Get the orientation.
			TES3::Vector3 orientation(0.0f, 0.0f, 0.0f);
			auto maybeOrientation = getOptionalParamVector3(params, "orientation");
			if (maybeOrientation) {
				orientation = maybeOrientation.value();
			}

			// Try to resolve the sell, either by what we were given, or a valid cell based on the given position.
			TES3::Cell* cell = getOptionalParamCell(params, "cell");
			if (cell == nullptr || (!cell->isInterior() && !cell->isPointInCell(maybePosition.value().x, maybePosition.value().y))) {
				int cellX = TES3::Cell::toGridCoord(maybePosition.value().x);
				int cellY = TES3::Cell::toGridCoord(maybePosition.value().y);
				cell = dataHandler->nonDynamicData->getCellByGrid(cellX, cellY);
			}

			// Make sure we actually got a cell.
			if (cell == nullptr) {
				throw std::invalid_argument("Invalid 'cell' parameter provided. Please provide a cell, or give a valid exterior position.");
			}

			// Create reference.
			TES3::Reference* reference = new TES3::Reference();
			reference->baseObject = object;

			// Scale as needed.
			float scale = getOptionalParam<float>(params, "scale", 1.0f);
			if (scale != 1.0f) {
				reference->setScale(scale);
			}

			// Add it to the cell lists/data handler.
			bool cellWasCreated = false;
			dataHandler->nonDynamicData->createReference(object, &maybePosition.value(), &orientation, cellWasCreated, reference, cell);
			reference->ensureScriptDataIsInstanced();
			cell->insertReference(reference);

			// Did we just make an actor? If so we need to add it to the mob manager.
			if (object->objectType == TES3::ObjectType::Creature || object->objectType == TES3::ObjectType::NPC) {
				TES3::WorldController::get()->mobController->addMob(reference);
				auto mact = reference->getAttachedMobileActor();
				if (mact && mact->isActor()) {
					mact->enterLeaveSimulation(true);
				}
			}
			// Activators, containers, and statics need collision.
			else if (object->objectType == TES3::ObjectType::Activator || object->objectType == TES3::ObjectType::Container || object->objectType == TES3::ObjectType::Static) {
				dataHandler->updateCollisionGroupsForActiveCells();
			}
			// Lights need to be configured.
			else if (object->objectType == TES3::ObjectType::Light) {
				dataHandler->setDynamicLightingForReference(reference);

				// Non-carryable lights also need collision.
				if (!static_cast<TES3::Light*>(object)->getCanCarry()) {
					dataHandler->updateCollisionGroupsForActiveCells();
				}
			}

			// Ensure the reference receives scene lighting.
			dataHandler->updateLightingForReference(reference);

			// Make sure everything is set as modified.
			reference->setObjectModified(true);
			cell->setObjectModified(true);

			return reference;
		}

		TES3::BaseObject* createObject(sol::table params) {
			auto objectType = getOptionalParam(params, "objectType", TES3::ObjectType::Invalid);
			auto getIfExists = getOptionalParam(params, "getIfExists", true);
			return makeObjectCreator(objectType)->create(params, getIfExists);
		}

		bool setDestination(sol::table params) {
			TES3::Reference* reference = getOptionalParamExecutionReference(params);
			if (reference == nullptr) {
				throw std::invalid_argument("Invalid reference parameter provided.");
			}

			if (reference->baseObject->objectType != TES3::ObjectType::Door) {
				throw std::invalid_argument("Provided reference is not a door.");
			}

			// Get the position.
			sol::optional<TES3::Vector3> position = getOptionalParamVector3(params, "position");
			if (!position) {
				throw std::invalid_argument("Invalid position parameter provided.");
			}

			// Get the orientation.
			sol::optional<TES3::Vector3> orientation = getOptionalParamVector3(params, "orientation");
			if (!orientation) {
				throw std::invalid_argument("Invalid orientation parameter provided.");
			}

			// Get the cell.
			TES3::Cell* cell = getOptionalParamCell(params, "cell");

			reference->setTravelDestination(&position.value(), &orientation.value(), cell);
			reference->setObjectModified(true);
			return true;
		}

		bool cast(sol::table params) {
			TES3::Reference* reference = getOptionalParamExecutionReference(params);
			if (reference == nullptr) {
				throw std::invalid_argument("Invalid reference parameter provided.");
			}

			TES3::Spell* spell = getOptionalParamSpell(params, "spell");
			if (spell == nullptr) {
				throw std::invalid_argument("Invalid spell parameter provided.");
			}

			TES3::MobileActor* casterMobile = reference->getAttachedMobileActor();
			TES3::Reference* target = getOptionalParamReference(params, "target");
			if (target == nullptr) {
				// Only allow player to cast without a target.
				if (!(casterMobile && casterMobile->actorType == TES3::MobileActorType::Player)) {
					throw std::invalid_argument("Invalid target parameter provided.");
				}
			}

			bool instant = getOptionalParam<bool>(params, "instant", false);
			if (casterMobile && !instant) {
				// Request AI to cast chosen spell.
				if (casterMobile->isActive() && target != nullptr) {
					casterMobile->setCurrentMagicSourceFiltered(spell);
					casterMobile->forceSpellCast(target->getAttachedMobileActor());
					return true;
				}
			}
			else {
				// Instant cast from both actors and non-actors.
				TES3::MagicSourceCombo sourceCombo(spell);
				auto magicInstanceController = TES3::WorldController::get()->magicInstanceController;
				auto serial = magicInstanceController->activateSpell(reference, nullptr, &sourceCombo);
				auto spellInstance = magicInstanceController->getInstanceFromSerial(serial);

				if (getOptionalParam<bool>(params, "alwaysSucceeds", true)) {
					spellInstance->overrideCastChance = 100.0f;
				}
				spellInstance->target = target;
				spellInstance->bypassResistances = getOptionalParam<bool>(params, "bypassResistances", false);

				// Trigger spells to progress from pre-cast to targetting state. This state is automatically reset by active AI.
				if (casterMobile) {
					casterMobile->actionData.animStateAttack = TES3::AttackAnimationState::CastingFollow;
				}

				return true;
			}

			return false;
		}

		static TES3::EquipmentStack tempApplyMagicSourceStack = { nullptr, nullptr };
		TES3::MagicSourceInstance* applyMagicSource(sol::table params) {
			// Get who we're adding the magic effect to.
			auto reference = getOptionalParamExecutionReference(params);
			if (reference == nullptr) {
				throw std::invalid_argument("Invalid 'reference' parameter provided.");
			}

			// Get the source for the effect.
			auto source = getOptionalParamObject<TES3::Object>(params, "source");

			// Are we given custom effects? If so make a potion.
			bool createCopy = true;
			sol::optional<sol::table> effects = params["effects"];
			if (source == nullptr && effects) {
				const char* name = getOptionalParam<const char*>(params, "name", nullptr);
				if (!name) {
					throw std::invalid_argument("Invalid 'name' parameter provided when creating dynamic effect.");
				}

				// Dynamically create our potion.
				auto dynamicPotion = new TES3::Alchemy();
				dynamicPotion->setName(name);
				dynamicPotion->flags |= 0x2;
				TES3::DataHandler::get()->nonDynamicData->addNewObject(dynamicPotion);

				// Assign effects.
				if (effects) {
					for (int i = 1; i <= 8; i++) {
						sol::optional<sol::table> effectParams = effects.value()[i];
						if (!effectParams) {
							break;
						}

						TES3::Effect* effect = &dynamicPotion->effects[i - 1];
						effect->effectID = getOptionalParam<int>(effectParams, "id", -1);
						effect->skillID = getOptionalParam<int>(effectParams, "skill", -1);
						effect->attributeID = getOptionalParam<int>(effectParams, "attribute", -1);
						effect->rangeType = static_cast<TES3::EffectRange>(getOptionalParam<int>(effectParams, "range", int(TES3::EffectRange::Self)));
						effect->radius = getOptionalParam<int>(effectParams, "radius", 0);
						effect->duration = getOptionalParam<int>(effectParams, "duration", 0);
						effect->magnitudeMin = getOptionalParam<int>(effectParams, "min", 0);
						effect->magnitudeMax = getOptionalParam<int>(effectParams, "max", 0);
					}
				}

				// If we don't have an effect, fill in the first one.
				if (dynamicPotion->effects[0].effectID == TES3::EffectID::None) {
					dynamicPotion->effects[0].effectID = TES3::EffectID::WaterBreathing;
				}

				source = dynamicPotion;
				createCopy = false;
			}

			// Did we end up figuring out a source?
			if (source == nullptr) {
				throw std::invalid_argument("Invalid 'source' parameter provided.");
			}

			// Try to make a source for this object.
			TES3::MagicSourceCombo sourceCombo(source);
			if (sourceCombo.sourceType == TES3::MagicSourceType::Invalid) {
				throw std::invalid_argument("Invalid 'source' parameter provided: Not a valid source type.");
			}

			// Do we have a piece of equipment this is coming from?
			auto from = getOptionalParam<TES3::EquipmentStack*>(params, "fromStack");

			// Handle some alchemy patching.
			if (sourceCombo.sourceType == TES3::MagicSourceType::Alchemy) {
				// Do we need to make a copy of this object?
				if (getOptionalParam<bool>(params, "createCopy", createCopy)) {
					auto copy = new TES3::Alchemy();
					copy->copy(source);
					copy->setID("");
					TES3::DataHandler::get()->nonDynamicData->addNewObject(copy);

					source = copy;
					sourceCombo.source.asAlchemy = copy;
				}

				if (!from) {
					tempApplyMagicSourceStack.object = source;
					from = &tempApplyMagicSourceStack;
				}
			}

			// Activate the source on our target.
			auto magicInstanceController = TES3::WorldController::get()->magicInstanceController;
			auto serial = magicInstanceController->activateSpell(reference, from.value_or(nullptr), &sourceCombo);
			auto instance = magicInstanceController->getInstanceFromSerial(serial);

			// Check if magic activation succeeded before setting more data.
			if (instance) {
				// Specify target.
				instance->target = getOptionalParamReference(params, "target");

				// Bypass resistances?
				instance->bypassResistances = getOptionalParam<bool>(params, "bypassResistances", false);

				if (sourceCombo.sourceType == TES3::MagicSourceType::Spell) {
					// Force spells to apply all effects to the target immediately. Use caster if no target is specified.
					// This method bypasses cast chance and animations.
					const auto& effects = sourceCombo.source.asSpell->effects;
					auto spellTarget = instance->target ? instance->target : reference;

					for (int i = 0; i < 8; ++i) {
						if (effects[i].effectID != -1) {
							instance->spellHit(spellTarget, i);
						}
					}
					instance->state = TES3::SpellEffectState::Working;
				}
				else if (sourceCombo.sourceType == TES3::MagicSourceType::Enchantment) {
					// Add enchantment source item to recharger.
					auto stack = from.value();
					TES3::WorldController::get()->rechargerAddItem(stack->object, stack->variables, sourceCombo.source.asEnchantment);
				}
			}

			return instance;
		}

		TES3::MagicSourceInstance* getMagicSourceInstanceBySerial(sol::table params) {
			auto serialNumber = getOptionalParam<unsigned int>(params, "serialNumber", UINT32_MAX);
			if (serialNumber == UINT32_MAX) {
				throw std::invalid_argument("Invalid 'serialNumber' parameter provided.");
			}

			return TES3::WorldController::get()->magicInstanceController->getInstanceFromSerial(serialNumber);
		}

		const auto TES3_UI_showAlchemyMenu = reinterpret_cast<void(__cdecl*)()>(0x599A30);
		void showAlchemyMenu() {
			TES3_UI_showAlchemyMenu();
		}

		const auto TES3_UI_showRepairServiceMenu = reinterpret_cast<void(__cdecl*)(TES3::MobileActor*)>(0x615160);
		void showRepairServiceMenu() {
			TES3_UI_showRepairServiceMenu(TES3::WorldController::get()->getMobilePlayer());
		}

		void updateInventoryGUI_internal(TES3::Reference* reference, std::optional<float> containerWeight = {}) {
			auto worldController = TES3::WorldController::get();
			auto macp = worldController->getMobilePlayer();

			// Update inventory menu if necessary.
			if (macp && reference == macp->reference) {
				worldController->inventoryData->clearIcons(2);
				worldController->inventoryData->addInventoryItems(&macp->npcInstance->inventory, 2);
				mwse::tes3::ui::inventoryUpdateIcons();
			}

			// Update contents menu if necessary.
			auto contentsMenu = TES3::UI::findMenu(*reinterpret_cast<TES3::UI::UI_ID*>(0x7D3098));
			if (contentsMenu) {
				TES3::Reference* contentsReference = static_cast<TES3::Reference*>(contentsMenu->getProperty(TES3::UI::PropertyType::Pointer, *reinterpret_cast<TES3::UI::Property*>(0x7D3048)).ptrValue);
				if (reference == contentsReference) {
					float isCompanion = *reinterpret_cast<float*>(0x7D3184);
					if (isCompanion != 0.0f) {
						TES3::UI::updateContentsCompanionElements();
					}

					TES3::UI::updateContentsMenuTiles();

					// Update container weight variable.
					auto MenuContents_totalweight = *reinterpret_cast<TES3::UI::Property*>(0x7D30B8);
					if (!containerWeight.has_value()) {
						containerWeight = static_cast<TES3::Actor*>(contentsReference->baseObject)->inventory.calculateContainedWeight();
					}
					contentsMenu->setProperty(MenuContents_totalweight, containerWeight.value());
				}
			}
		}

		void updateInventoryGUI(sol::table params) {
			TES3::Reference* reference = getOptionalParamReference(params, "reference");
			if (reference) {
				updateInventoryGUI_internal(reference);
			}
		}

		int addItem(sol::table params) {
			// Get the reference we are manipulating.
			TES3::Reference* reference = getOptionalParamReference(params, "reference");
			if (reference == nullptr) {
				throw std::invalid_argument("Invalid 'reference' parameter provided.");
			}

			// Get the item we are going to add.
			TES3::Item* item = getOptionalParamObject<TES3::Item>(params, "item");
			if (item == nullptr) {
				throw std::invalid_argument("Invalid 'item' parameter provided.");
			}

			TES3::ItemData* itemData = getOptionalParam<TES3::ItemData*>(params, "itemData", nullptr);

			// Make sure we're dealing with actors.
			TES3::Actor* actor = static_cast<TES3::Actor*>(reference->baseObject);
			if (!actor->isActor()) {
				throw std::invalid_argument("The 'reference' reference does not point to an actor.");
			}

			// Enable defining a soul to add.
			TES3::Actor* soul = getOptionalParamObject<TES3::Actor>(params, "soul");
			bool createdItemData = false;
			if (soul) {
				if (!tes3::isSoulGem(item)) {
					throw std::invalid_argument("Cannot add a soul to item, as it is not a soul gem.");
				}

				if (!itemData) {
					itemData = TES3::ItemData::createForObject(item);
					createdItemData = true;
				}

				itemData->soul = soul;
			}

			// Clone the object if needed.
			if (reference->clone()) {
				actor = static_cast<TES3::Actor*>(reference->baseObject);
			}

			// Get how many items we are adding.
			int fulfilledCount = 0;
			int desiredCount = std::max(std::abs(getOptionalParam(params, "count", 1)), 1);
			if (itemData != nullptr) {
				desiredCount = 1;
			}

			if (getOptionalParam<bool>(params, "limit", false)) {
				// Prevent placing items into organic containers.
				if (BIT_TEST(actor->actorFlags, TES3::ActorFlagContainer::OrganicBit)) {
					if (createdItemData) {
						delete itemData;
						itemData = nullptr;
					}
					return 0;
				}

				// Figure out how many more of the item can fit in the container.
				auto maxCapacity = static_cast<TES3::Container*>(reference->getBaseObject())->capacity;
				auto currentWeight = actor->inventory.calculateContainedWeight();
				int fulfilledCount = std::min((int)((maxCapacity - currentWeight) / item->getWeight()), desiredCount);
			}
			else {
				fulfilledCount = desiredCount;
			}

			// No items to add? Great, let's get out of here.
			if (fulfilledCount == 0) {
				if (createdItemData) {
					delete itemData;
					itemData = nullptr;
				}
				return 0;
			}

			if (itemData) {
				// Clear the owner, if any.
				itemData->owner = nullptr;

				// Delete the item data if it's fully repaired.
				if (TES3::ItemData::isFullyRepaired(itemData, item)) {
					delete itemData;
					itemData = nullptr;
				}
			}

			// Add the item and return the added count, since we do no inventory checking.
			auto mobile = reference->getAttachedMobileActor();
			if (itemData) {
				fulfilledCount = actor->inventory.addItem(mobile, item, fulfilledCount, false, &itemData);
			}
			else {
				fulfilledCount = actor->inventory.addItemWithoutData(mobile, item, fulfilledCount, false);
			}

			// Play the relevant sound.
			auto worldController = TES3::WorldController::get();
			auto playerMobile = worldController->getMobilePlayer();
			if (playerMobile && getOptionalParam<bool>(params, "playSound", true)) {
				if (mobile == playerMobile) {
					worldController->playItemUpDownSound(item, TES3::ItemSoundState::Down);
				}
			}

			// Update body parts for creatures/NPCs that may have items unequipped.
			if (mobile) {
				reference->updateBipedParts();

				if (mobile == playerMobile) {
					playerMobile->firstPersonReference->updateBipedParts();
				}

				mobile->updateOpacity();
			}

			// If either of them are the player, we need to update the GUI.
			if (playerMobile && getOptionalParam<bool>(params, "updateGUI", true)) {
				// Update contents menu if necessary.
				std::optional<float> newContainerWeight;
				auto contentsMenu = TES3::UI::findMenu(*reinterpret_cast<TES3::UI::UI_ID*>(0x7D3098));
				if (contentsMenu) {
					TES3::Reference* contentsReference = static_cast<TES3::Reference*>(contentsMenu->getProperty(TES3::UI::PropertyType::Pointer, *reinterpret_cast<TES3::UI::Property*>(0x7D3048)).ptrValue);
					if (reference == contentsReference) {
						TES3::UI::updateContentsMenuTiles();

						// Update container weight variable.
						auto MenuContents_totalweight = *reinterpret_cast<TES3::UI::Property*>(0x7D30B8);
						auto currentWeight = contentsMenu->getProperty(TES3::UI::PropertyType::Float, MenuContents_totalweight).floatValue;
						newContainerWeight = currentWeight + item->getWeight() * fulfilledCount;
					}
				}
				updateInventoryGUI_internal(reference, newContainerWeight);
			}

			reference->setObjectModified(true);
			return fulfilledCount;
		}

		int removeItem(sol::table params) {
			// Get the reference we are manipulating.
			TES3::Reference* reference = getOptionalParamReference(params, "reference");
			if (reference == nullptr) {
				throw std::invalid_argument("Invalid 'reference' parameter provided.");
			}

			// Get the item we are going to remove.
			TES3::Item* item = getOptionalParamObject<TES3::Item>(params, "item");
			if (item == nullptr) {
				throw std::invalid_argument("Invalid 'item' parameter provided.");
			}

			TES3::ItemData* itemData = getOptionalParam<TES3::ItemData*>(params, "itemData", nullptr);
			auto deleteItemData = getOptionalParam<bool>(params, "deleteItemData", itemData != nullptr);

			// Make sure we're dealing with actors.
			TES3::Actor* actor = static_cast<TES3::Actor*>(reference->baseObject);
			if (!actor->isActor()) {
				throw std::invalid_argument("The 'reference' reference does not point to an actor.");
			}

			// Clone the object if needed.
			if (reference->clone()) {
				actor = static_cast<TES3::Actor*>(reference->baseObject);
			}

			// Get how many items we are removing. Force to 1 if we supply an itemData.
			int desiredCount = std::max(std::abs(getOptionalParam(params, "count", 1)), 1);
			if (itemData != nullptr) {
				desiredCount = 1;
			}

			// Make sure that the inventory contains the item.
			TES3::ItemStack* stack = actor->inventory.findItemStack(item);
			if (stack == nullptr) {
				return 0;
			}

			// If we were given an itemData, make sure that it's here.
			if (itemData != nullptr && stack->variables != nullptr && !stack->variables->contains(itemData)) {
				return 0;
			}

			// Limit removal by stack count.
			int fulfilledCount = std::min(desiredCount, stack->count);

			// No items to remove? Great, let's get out of here.
			if (fulfilledCount == 0) {
				return 0;
			}

			// Try to unequip the item if it's equipped.
			auto mobile = reference->getAttachedMobileActor();
			if (itemData != nullptr) {
				actor->unequipItem(item, true, mobile, false, itemData);
			}

			// Add the item and return the added count, since we do no inventory checking.
			actor->inventory.removeItemWithData(mobile, item, itemData, fulfilledCount, deleteItemData);

			// Play the relevant sound.
			auto worldController = TES3::WorldController::get();
			auto playerMobile = worldController->getMobilePlayer();
			if (playerMobile && getOptionalParam<bool>(params, "playSound", true)) {
				if (mobile == playerMobile) {
					worldController->playItemUpDownSound(item, TES3::ItemSoundState::Up);
				}
			}

			// Update body parts for creatures/NPCs that may have items unequipped.
			if (mobile) {
				reference->updateBipedParts();

				if (mobile == playerMobile) {
					playerMobile->firstPersonReference->updateBipedParts();
				}

				mobile->updateOpacity();
			}

			// If either of them are the player, we need to update the GUI.
			if (playerMobile && getOptionalParam<bool>(params, "updateGUI", true)) {
				// Update contents menu if necessary.
				std::optional<float> newContainerWeight;
				auto contentsMenu = TES3::UI::findMenu(*reinterpret_cast<TES3::UI::UI_ID*>(0x7D3098));
				if (contentsMenu) {
					TES3::Reference* contentsReference = static_cast<TES3::Reference*>(contentsMenu->getProperty(TES3::UI::PropertyType::Pointer, *reinterpret_cast<TES3::UI::Property*>(0x7D3048)).ptrValue);
					if (reference == contentsReference) {
						TES3::UI::updateContentsMenuTiles();
					}

					// Update container weight variable.
					auto MenuContents_totalweight = *reinterpret_cast<TES3::UI::Property*>(0x7D30B8);
					auto currentWeight = contentsMenu->getProperty(TES3::UI::PropertyType::Float, MenuContents_totalweight).floatValue;
					newContainerWeight = currentWeight - item->getWeight() * fulfilledCount;
				}
				updateInventoryGUI_internal(reference, newContainerWeight);
			}

			reference->setObjectModified(true);
			return fulfilledCount;
		}

		bool togglePOV() {
			auto worldController = TES3::WorldController::get();
			if (worldController == nullptr) {
				throw std::runtime_error("Function called before world controller is initialized.");
			}

			auto macp = worldController->getMobilePlayer();
			if (macp == nullptr) {
				throw std::runtime_error("Function called while mobile player is unavailable.");
			}

			auto animController = macp->animationController.asPlayer;
			if (animController == nullptr) {
				throw std::runtime_error("Function called while mobile player animation controller is unavailable.");
			}

			animController->togglePOV = true;

			return animController->is3rdPerson;
		}

		int transferItem(sol::table params) {
			// Get the reference we are transferring from.
			TES3::Reference* fromReference = getOptionalParamReference(params, "from");
			if (fromReference == nullptr) {
				throw std::invalid_argument("Invalid 'from' parameter provided.");
			}

			// Get the reference we are transferring to.
			TES3::Reference* toReference = getOptionalParamReference(params, "to");
			if (toReference == nullptr) {
				throw std::invalid_argument("Invalid 'to' parameter provided.");
			}

			// Get the item we are going to transfer.
			TES3::Item* item = getOptionalParamObject<TES3::Item>(params, "item");
			if (item == nullptr) {
				throw std::invalid_argument("Invalid 'item' parameter provided.");
			}

			// Make sure we're dealing with actors.
			TES3::Actor* fromActor = static_cast<TES3::Actor*>(fromReference->baseObject);
			if (!fromActor->isActor()) {
				throw std::invalid_argument("The 'from' reference does not point to an actor.");
			}
			TES3::Actor* toActor = static_cast<TES3::Actor*>(toReference->baseObject);
			if (!toActor->isActor()) {
				throw std::invalid_argument("The 'to' reference does not point to an actor.");
			}

			// Do either of the references need to be cloned?
			if (fromReference->clone()) {
				fromActor = static_cast<TES3::Actor*>(fromReference->baseObject);
			}
			if (toReference->clone()) {
				toActor = static_cast<TES3::Actor*>(toReference->baseObject);
			}

			// Get any associated item data.
			TES3::ItemData* itemData = getOptionalParam<TES3::ItemData*>(params, "itemData", nullptr);

			// Get how many items we are transferring.
			int desiredCount = std::max(std::abs(getOptionalParam(params, "count", 1)), 1);
			int fulfilledCount = 0;

			// Get the mobile objects for the references, if applicable.
			auto toMobile = toReference->getAttachedMobileActor();
			auto fromMobile = fromReference->getAttachedMobileActor();

			// Are we looking at a non-container?
			auto fromIsContainer = (fromActor->objectType == TES3::ObjectType::Container);

			// Manage anything we need to regarding containers.
			float maxCapacity = -1.0f;
			float currentWeight = 0.0f;
			float itemWeight = item->getWeight();
			if (toActor->objectType == TES3::ObjectType::Container && getOptionalParam<bool>(params, "limitCapacity", true)) {
				// Prevent placing items into organic containers.
				if (BIT_TEST(toActor->actorFlags, TES3::ActorFlagContainer::OrganicBit)) {
					return 0;
				}

				// Figure out the max capacity and currently stored weight of the container.
				maxCapacity = static_cast<TES3::Container*>(toReference->getBaseObject())->capacity;
				currentWeight = toActor->inventory.calculateContainedWeight();
				if (currentWeight > maxCapacity) {
					return 0;
				}
			}

			// Were we given an ItemData? If so, we only need to transfer one item.
			if (itemData) {
				if ((maxCapacity == -1.0f || currentWeight + itemWeight <= maxCapacity) && fromActor->inventory.containsItem(item, itemData)) {
					toActor->inventory.addItem(toMobile, item, 1, false, &itemData);
					fromActor->inventory.removeItemWithData(fromMobile, item, itemData, 1, false);

					if (!fromIsContainer) {
						fromActor->unequipItem(item, true, fromMobile, false, itemData);
					}

					fulfilledCount = 1;
					currentWeight += itemWeight;
				}
			}
			// No ItemData? We have to go through and transfer items over one by one.
			else {
				TES3::ItemStack* fromStack = fromActor->inventory.findItemStack(item);
				if (fromStack) {
					int stackCount = std::abs(fromStack->count);
					int itemsLeftToTransfer = std::min(desiredCount, stackCount);

					// If we're limited by capacity, find out how many items we really want to transfer.
					if (maxCapacity != -1.0f && itemWeight != 0.0f) {
						itemsLeftToTransfer = std::min(itemsLeftToTransfer, (int)std::floorf((maxCapacity - currentWeight) / itemWeight));
					}

					if (itemsLeftToTransfer <= 0) {
						return 0;
					}

					// Remove transfer items without data first.
					int countWithoutVariables = stackCount - (fromStack->variables ? fromStack->variables->getEndIndex() : 0);
					if (countWithoutVariables > 0) {
						int amountToTransfer = std::min(countWithoutVariables, itemsLeftToTransfer);
						toActor->inventory.addItem(toMobile, item, amountToTransfer, false, nullptr);
						fromActor->inventory.removeItemWithData(fromMobile, item, nullptr, amountToTransfer, false);

						// Check for ammunition and thrown weapons, as unlike other equipment, they do not generate itemData when equipped.
						if (!fromIsContainer && (item->objectType == TES3::ObjectType::Ammo || item->objectType == TES3::ObjectType::Weapon)) {
							fromActor->unequipItem(item, true, fromMobile, false, nullptr);
						}

						fulfilledCount += amountToTransfer;
						itemsLeftToTransfer -= amountToTransfer;
					}

					// Then transfer over items with data.
					while (itemsLeftToTransfer > 0) {
						TES3::ItemData** itemDataRef = nullptr;

						if (fromStack->variables) {
							// We need to unequip the item first.
							auto removedEquipStack = fromActor->unequipItem(item, false, fromMobile, false, fromStack->variables->at(0));
							if (removedEquipStack == nullptr) {
								// If nothing was returned then the item wasn't equipped. So transfer using the first data.
								itemDataRef = &fromStack->variables->at(0);
							}
							else {
								// Item was unequipped, but remains the first item? Preserve the item data.
								if (fromStack->variables && fromStack->variables->at(0) == removedEquipStack->variables) {
									itemDataRef = &fromStack->variables->at(0);
								}

								// Clean up after our check and manually delete.
								removedEquipStack->object = nullptr;
								removedEquipStack->variables = nullptr;
								mwse::tes3::_delete(removedEquipStack);
								removedEquipStack = nullptr;
							}
						}

						toActor->inventory.addItem(toMobile, item, 1, false, itemDataRef);
						fromActor->inventory.removeItemWithData(fromMobile, item, itemDataRef ? *itemDataRef : nullptr, 1, false);

						fulfilledCount++;
						itemsLeftToTransfer--;
					}
				}
			}

			// No items to transfer? Great, let's get out of here.
			if (fulfilledCount == 0) {
				return 0;
			}

			// Play the relevant sound.
			auto worldController = TES3::WorldController::get();
			auto playerMobile = worldController->getMobilePlayer();
			if (playerMobile && getOptionalParam<bool>(params, "playSound", true)) {
				if (toMobile == playerMobile) {
					worldController->playItemUpDownSound(item, TES3::ItemSoundState::Down);
				}
				else if (fromMobile == playerMobile) {
					worldController->playItemUpDownSound(item, TES3::ItemSoundState::Up);
				}
			}

			// Update body parts for creatures/NPCs that may have items unequipped.
			if (fromMobile) {
				fromReference->updateBipedParts();

				if (fromMobile == playerMobile) {
					playerMobile->firstPersonReference->updateBipedParts();
				}

				fromMobile->updateOpacity();
			}

			// If we're looking at a companion, we need to update the profit value and trigger the GUI updates.
			float isCompanion = *reinterpret_cast<float*>(0x7D3184);
			if (isCompanion != 0.0f) {
				auto contentsMenu = TES3::UI::findMenu(*reinterpret_cast<TES3::UI::UI_ID*>(0x7D3098));
				if (contentsMenu) {
					TES3::Reference* contentsReference = static_cast<TES3::Reference*>(contentsMenu->getProperty(TES3::UI::PropertyType::Pointer, *reinterpret_cast<TES3::UI::Property*>(0x7D3048)).ptrValue);
					float& companionProfit = *reinterpret_cast<float*>(0x7D3188);
					if (toReference == contentsReference) {
						companionProfit += fulfilledCount * item->getValue();
					}
					else {
						companionProfit -= fulfilledCount * item->getValue();
					}
				}
			}

			// If either of them are the player, we need to update the GUI.
			if (playerMobile && getOptionalParam<bool>(params, "updateGUI", true)) {
				// Update inventory menu if necessary.
				if (fromMobile == playerMobile || toMobile == playerMobile) {
					worldController->inventoryData->clearIcons(2);
					worldController->inventoryData->addInventoryItems(&playerMobile->npcInstance->inventory, 2);
					mwse::tes3::ui::inventoryUpdateIcons();
				}

				// Update contents menu if necessary.
				std::optional<float> newContainerWeight;
				auto contentsMenu = TES3::UI::findMenu(*reinterpret_cast<TES3::UI::UI_ID*>(0x7D3098));
				if (contentsMenu) {
					// Make sure that the contents reference is one of the ones we care about.
					TES3::Reference* contentsReference = static_cast<TES3::Reference*>(contentsMenu->getProperty(TES3::UI::PropertyType::Pointer, *reinterpret_cast<TES3::UI::Property*>(0x7D3048)).ptrValue);

					// Update container weight variable.
					auto MenuContents_totalweight = *reinterpret_cast<TES3::UI::Property*>(0x7D30B8);
					auto currentWeight = contentsMenu->getProperty(TES3::UI::PropertyType::Float, MenuContents_totalweight).floatValue;
					if (toReference == contentsReference) {
						newContainerWeight = currentWeight + item->getWeight() * fulfilledCount;
					}
					else {
						newContainerWeight = currentWeight - item->getWeight() * fulfilledCount;
					}
				}

				updateInventoryGUI_internal(fromReference, newContainerWeight);
				updateInventoryGUI_internal(toReference, newContainerWeight);
			}

			fromReference->setObjectModified(true);
			toReference->setObjectModified(true);
			return fulfilledCount;
		}

		TES3::ItemData* addItemData(sol::table params) {
			// Get the reference of the item or item container.
			TES3::Reference* toReference = getOptionalParamReference(params, "to");
			if (toReference == nullptr) {
				throw std::invalid_argument("Invalid 'to' parameter provided.");
			}
			// Check if it's setting a world reference or an item in a container.
			TES3::Actor* toActor = static_cast<TES3::Actor*>(toReference->baseObject);
			if (!toActor->isActor()) {
				// It's a reference. This is the easy part.
				// Return nil if there is already itemData, or return the newly-created itemData.
				if (toReference->getAttachedItemData()) {
					return nullptr;
				}
				toReference->setObjectModified(true);
				return toReference->getOrCreateAttachedItemData();
			}

			// Get the item we are going to transfer.
			TES3::Item* item = getOptionalParamObject<TES3::Item>(params, "item");
			if (item == nullptr) {
				throw std::invalid_argument("Invalid 'item' parameter provided.");
			}

			// Does the reference need to be cloned?
			if (toReference->clone()) {
				toActor = static_cast<TES3::Actor*>(toReference->baseObject);
			}

			// Try to find an ItemData-less item and add ItemData for it.
			TES3::ItemStack* stack = toActor->inventory.findItemStack(item);
			if (!stack || stack->count < 1) {
				throw std::runtime_error("The actor does not possess any of 'item'.");
			}

			if (!stack->variables) {
				// Create array required to hold ItemData.
				stack->variables = new NI::TArray<TES3::ItemData*>();
			}
			else if (stack->count <= int(stack->variables->getFilledCount())) {
				// All items already have ItemData.
				return nullptr;
			}

			// Finally add ItemData to stack.
			TES3::ItemData* itemData = TES3::ItemData::createForObject(item);
			stack->variables->add(itemData);

			// If either of them are the player, we need to update the GUI.
			auto worldController = TES3::WorldController::get();
			auto toMobile = toReference->getAttachedMobileActor();
			auto playerMobile = worldController->getMobilePlayer();

			if (getOptionalParam<bool>(params, "updateGUI", true)) {
				updateInventoryGUI_internal(toReference);
			}

			toReference->setObjectModified(true);
			return itemData;
		}

		int getCurrentAIPackageId(sol::table params) {
			TES3::Reference* refr = getOptionalParamReference(params, "reference");
			TES3::MobileActor* mobileActor = getOptionalParamMobileActor(params, "reference");
			if (mobileActor) {
				if (mobileActor->aiPlanner != nullptr) {
					auto currentPackage = mobileActor->aiPlanner->getActivePackage();
					if (currentPackage != nullptr) {
						return int(currentPackage->packageType);
					}
				}
			}
			else if (refr && refr->baseObject->isActor()) {
				TES3::Actor* actor = static_cast<TES3::Actor*>(refr->baseObject);
				auto currentConfig = actor->getAIPackageConfig();
				if (currentConfig != nullptr) {
					// Convert config type to AIPackageType
					return int(currentConfig->toPackageType());
				}
			}
			else {
				throw std::invalid_argument("Invalid reference parameter provided.");
			}

			return -1;
		}

		void setAIActivate(sol::table params) {
			TES3::MobileActor* mobileActor = getOptionalParamMobileActor(params, "reference");
			if (mobileActor == nullptr) {
				throw std::invalid_argument("Invalid reference parameter provided.");
			}

			TES3::Reference* target = getOptionalParamReference(params, "target");
			if (target == nullptr) {
				throw std::invalid_argument("Invalid target parameter provided.");
			}

			auto config = tes3::_new<TES3::AIPackageActivate::Config>();
			config->type = TES3::AIPackageConfigType::Activate;
			config->target = target;
			config->reset = getOptionalParam<bool>(params, "reset", true);

			auto actor = static_cast<TES3::Actor*>(mobileActor->reference->baseObject);
			actor->setAIPackage(config, mobileActor->reference);
		}

		void setAIFollow(sol::table params) {
			TES3::MobileActor* mobileActor = getOptionalParamMobileActor(params, "reference");
			if (mobileActor == nullptr) {
				throw std::invalid_argument("Invalid reference parameter provided.");
			}

			TES3::Reference* target = getOptionalParamReference(params, "target");
			if (target == nullptr || !target->baseObject->isActor()) {
				throw std::invalid_argument("Invalid target parameter provided.");
			}

			auto destination = getOptionalParamVector3(params, "destination");

			auto config = tes3::_new<TES3::AIPackageFollow::Config>();
			config->type = TES3::AIPackageConfigType::Follow;
			if (destination) {
				config->destination = destination.value();
			}
			else {
				config->destination = TES3::Vector3(FLT_MAX, FLT_MAX, 0.0f);
			}
			config->duration = getOptionalParam<unsigned char>(params, "duration", 0);
			config->actor = static_cast<TES3::Actor*>(target->baseObject);
			config->cell = getOptionalParamCell(params, "cell");
			config->reset = getOptionalParam<bool>(params, "reset", true);

			auto actor = static_cast<TES3::Actor*>(mobileActor->reference->baseObject);
			actor->setAIPackage(config, mobileActor->reference);
		}

		void setAIEscort(sol::table params) {
			TES3::MobileActor* mobileActor = getOptionalParamMobileActor(params, "reference");
			if (mobileActor == nullptr) {
				throw std::invalid_argument("Invalid reference parameter provided.");
			}

			TES3::Reference* target = getOptionalParamReference(params, "target");
			if (target == nullptr || !target->baseObject->isActor()) {
				throw std::invalid_argument("Invalid target parameter provided.");
			}

			auto destination = getOptionalParamVector3(params, "destination");
			if (!destination) {
				throw std::invalid_argument("Destination parameter is missing.");
			}

			auto config = tes3::_new<TES3::AIPackageEscort::Config>();
			config->type = TES3::AIPackageConfigType::Escort;
			config->destination = destination.value();
			config->duration = getOptionalParam<unsigned char>(params, "duration", 0);
			config->actor = static_cast<TES3::Actor*>(target->getBaseObject());
			config->cell = getOptionalParamCell(params, "cell");
			config->reset = getOptionalParam<bool>(params, "reset", true);

			auto actor = static_cast<TES3::Actor*>(mobileActor->reference->baseObject);
			actor->setAIPackage(config, mobileActor->reference);
		}

		void setAITravel(sol::table params) {
			TES3::MobileActor* mobileActor = getOptionalParamMobileActor(params, "reference");
			if (mobileActor == nullptr) {
				throw std::invalid_argument("Invalid reference parameter provided.");
			}

			auto destination = getOptionalParamVector3(params, "destination");
			if (!destination) {
				throw std::invalid_argument("Invalid destination parameter provided.");
			}

			auto config = tes3::_new<TES3::AIPackageTravel::Config>();
			config->type = TES3::AIPackageConfigType::Travel;
			config->position = destination.value();
			config->reset = getOptionalParam<bool>(params, "reset", true);

			auto actor = static_cast<TES3::Actor*>(mobileActor->reference->baseObject);
			actor->setAIPackage(config, mobileActor->reference);
		}

		void setAIWander(sol::table params) {
			TES3::MobileActor* mobileActor = getOptionalParamMobileActor(params, "reference");
			if (mobileActor == nullptr) {
				throw std::invalid_argument("Invalid reference parameter provided.");
			}

			sol::optional<sol::table> maybeIdles = params["idles"];
			if (!maybeIdles || maybeIdles.value().get_type() != sol::type::table) {
				throw std::invalid_argument("Invalid idles table provided.");
			}

			auto config = tes3::_new<TES3::AIPackageWander::Config>();
			config->type = TES3::AIPackageConfigType::Wander;
			config->range = getOptionalParam<unsigned short>(params, "range", 0);
			config->duration = getOptionalParam<unsigned char>(params, "duration", 0);
			config->time = getOptionalParam<unsigned char>(params, "time", 0);
			config->reset = getOptionalParam<bool>(params, "reset", true);

			sol::table idles = maybeIdles.value();
			for (size_t i = 0; i < 8; i++) {
				config->idles[i] = idles.get_or(i, 0);
			}

			auto actor = static_cast<TES3::Actor*>(mobileActor->reference->baseObject);
			actor->setAIPackage(config, mobileActor->reference);
		}

		void say(sol::table params) {
			auto dataHandler = TES3::DataHandler::get();
			auto worldController = TES3::WorldController::get();
			if (worldController == nullptr || dataHandler == nullptr) {
				throw std::invalid_argument("Cannoy be called before tes3worldController and tes3dataHandler are initialized.");
			}

			TES3::Reference* reference = getOptionalParamExecutionReference(params);
			if (reference == nullptr) {
				throw std::invalid_argument("Invalid reference parameter provided.");
			}

			const char* path = getOptionalParam<const char*>(params, "soundPath", nullptr);
			if (path == nullptr) {
				throw std::invalid_argument("Invalid soundPath parameter provided.");
			}

			float pitch = getOptionalParam(params, "pitch", 1.0f);

			// Apply volume, using mix channel and rescale to 0-250.
			double volume = std::clamp(getOptionalParam(params, "volume", 1.0), 0.0, 1.0);
			volume *= 250.0 * worldController->audioController->getMixVolume(TES3::AudioMixType::Voice);

			// Show a messagebox.
			if (worldController->showSubtitles || getOptionalParam(params, "forceSubtitle", false)) {
				const char* subtitle = getOptionalParam<const char*>(params, "subtitle", nullptr);
				if (subtitle != nullptr) {
					TES3::UI::showMessageBox(subtitle);
				}
			}

			// Play the related sound.
			dataHandler->addTemporySound(path, reference, 0, int(volume), pitch, true);
		}

		bool hasOwnershipAccess(sol::table params) {
			// Who are we checking ownership for? The player by default.
			TES3::Reference* reference = getOptionalParamExecutionReference(params);
			auto playerReference = TES3::WorldController::get()->getMobilePlayer()->reference;
			if (reference == nullptr) {
				reference = playerReference;
			}
			auto referenceBaseObject = reference->getBaseObject();

			// What are we checking ownership of?
			TES3::Reference* target = getOptionalParamReference(params, "target");
			if (target == nullptr) {
				throw std::invalid_argument("Invalid target parameter provided.");
			}

			// You always have access to yourself.
			if (reference == target) {
				return true;
			}

			// The player never has access to living actors.
			auto targetBaseObject = target->getBaseObject();
			if (target->isActor() && !target->isDead().value_or(true)) {
				return false;
			}

			// Do we have an owner?
			auto targetData = target->getAttachedItemData();
			if (targetData == nullptr || targetData->owner == nullptr) {
				return true;
			}

			// Are we looking at an NPC owner?
			if (targetData->owner->objectType == TES3::ObjectType::NPC) {
				// We own our own things.
				if (referenceBaseObject == targetData->owner) {
					return true;
				}

				// Is the target dead? We have to check the kill counter here.
				else if (reference == playerReference && TES3::WorldController::get()->playerKills->getKillCount(static_cast<TES3::NPC*>(targetData->owner)) > 0) {
					return true;
				}

				// No variable? No chance to own.
				else if (targetData->requiredVariable == nullptr) {
					return false;
				}

				// Players get access if the variable is set.
				else if (reference == playerReference) {
					return targetData->requiredVariable->value > 0.0f;
				}
			}
			// How about a faction?
			else if (targetData->owner->objectType == TES3::ObjectType::Faction) {
				auto ownerAsFaction = reinterpret_cast<TES3::Faction*>(targetData->owner);

				// Players require the right rank.
				if (reference == playerReference) {
					return ownerAsFaction->getEffectivePlayerRank() >= targetData->requiredRank;
				}

				// As do NPCs, but their faction info is stored elsewhere.
				auto referenceObjectAsNPC = reinterpret_cast<TES3::NPC*>(reference->getBaseObject());
				if (referenceObjectAsNPC->objectType == TES3::ObjectType::NPC) {
					return (referenceObjectAsNPC->getFaction() == targetData->owner && referenceObjectAsNPC->factionRank >= targetData->requiredRank);
				}
			}

			return false;
		}

		TES3::Reference* dropItem(sol::table params) {
			// Who is dropping?
			TES3::MobileActor* mobile = getOptionalParamMobileActor(params, "reference");
			if (mobile == nullptr) {
				throw std::invalid_argument("Invalid reference parameter provided.");
			}

			// What are they dropping?
			TES3::Item* item = getOptionalParamObject<TES3::Item>(params, "item");
			if (item == nullptr) {
				throw std::invalid_argument("Invalid item parameter provided.");
			}

			// Get data about what is being dropped.
			auto itemData = getOptionalParam<TES3::ItemData*>(params, "itemData");
			int count = getOptionalParam<int>(params, "count", 1);
			bool matchNoItemData = getOptionalParam<bool>(params, "matchNoItemData", false);

			// Check if the item to drop exists, as the game functions don't handle it.
			auto actor = static_cast<TES3::Actor*>(mobile->reference->baseObject);
			auto stack = actor->inventory.findItemStack(item);
			if (!stack) {
				return nullptr;
			}
			if (matchNoItemData) {
				if (stack->variables && stack->count == stack->variables->filledCount) {
					// Match failed: All items have itemData.
					return nullptr;
				}
			}
			else if (itemData.has_value()) {
				if (stack->variables == nullptr || !stack->variables->contains(itemData.value())) {
					// Match failed: itemData is not found.
					return nullptr;
				}
			}

			// Drop the item.
			bool matchExact = itemData.has_value() || matchNoItemData;
			mobile->dropItem(item, itemData.value_or(nullptr), count, !matchExact);
			auto droppedReference = mobile->getCell()->temporaryRefs.tail;

			// Update inventory tiles if needed.
			if (getOptionalParam<bool>(params, "updateGUI", true)) {
				updateInventoryGUI_internal(mobile->reference);
			}

			return droppedReference;
		}

		bool persuade(sol::table params) {
			auto actor = getOptionalParamMobileActor(params, "actor");
			if (actor == nullptr) {
				throw std::invalid_argument("Invalid actor parameter provided.");
			}

			auto rng = rand() % 100;

			int index = getOptionalParam<int>(params, "index", -1);

			if (index >= 0 && index <= 6) {
				actor->reference->setObjectModified(true);
				return actor->persuade(rng, index);
			}
			else {
				auto fBribe100Mod = TES3::DataHandler::get()->nonDynamicData->GMSTs[TES3::GMST::fBribe100Mod];
				float modifier = getOptionalParam<float>(params, "modifier", 0.0f);

				float oldModifier = fBribe100Mod->value.asFloat;
				fBribe100Mod->value.asFloat = modifier;

				bool result = actor->persuade(rng, 4);
				fBribe100Mod->value.asFloat = oldModifier;

				actor->reference->setObjectModified(true);

				return result;
			}
		}

		TES3::Reference* findClosestExteriorReferenceOfObject(sol::table params) {
			TES3::DataHandler* dataHandler = TES3::DataHandler::get();

			TES3::PhysicalObject* object = getOptionalParamObject<TES3::PhysicalObject>(params, "object");
			if (object == nullptr) {
				throw std::invalid_argument("Invalid 'object' parameter provided.");
			}

			sol::optional<TES3::Vector3> position = getOptionalParamVector3(params, "position");
			if (!position) {
				position = dataHandler->getLastExteriorPosition();
			}

			if (dataHandler) {
				return dataHandler->nonDynamicData->findClosestExteriorReferenceOfObject(object, &position.value());
			}
			return nullptr;
		}

		TES3::Dialogue* findDialogue(sol::table params) {
			int type = getOptionalParam<int>(params, "type", -1);
			int page = getOptionalParam<int>(params, "page", -1);
			return TES3::Dialogue::getDialogue(type, page);
		}

		bool setEnabled(sol::table params) {
			TES3::Reference* reference = getOptionalParamExecutionReference(params);
			if (reference == nullptr) {
				throw std::invalid_argument("Invalid 'reference' parameter provided.");
			}

			reference->setObjectModified(true);

			// Allow toggling.
			if (getOptionalParam<bool>(params, "toggle", false)) {
				if (reference->getDisabled()) {
					return reference->enable();
				}
				else {
					return reference->disable();
				}
			}
			// Otherwise base it on enabled (default true).
			else {
				if (getOptionalParam<bool>(params, "enabled", true)) {
					return reference->enable();
				}
				else {
					return reference->disable();
				}
			}
		}

		sol::optional<std::tuple<unsigned char, unsigned char, unsigned char>> getCurrentAnimationGroups(sol::table params) {
			TES3::Reference* reference = getOptionalParamExecutionReference(params);
			if (reference == nullptr) {
				throw std::invalid_argument("Invalid 'reference' parameter provided.");
			}

			auto animData = reference->getAttachedAnimationData();
			if (animData == nullptr) {
				return {};
			}

			return std::make_tuple(animData->currentAnimGroup[0], animData->currentAnimGroup[1], animData->currentAnimGroup[2]);
		}

		void loadAnimation(sol::table params) {
			TES3::Reference* reference = getOptionalParamExecutionReference(params);
			if (reference == nullptr) {
				throw std::invalid_argument("Invalid 'reference' parameter provided.");
			}

			auto animData = reference->getAttachedAnimationData();
			if (reference->sceneNode == nullptr || animData == nullptr) {
				return;
			}

			// Change the animation temporarily. Passing nullptr resets the animation to base.
			const char* modelFile = getOptionalParam<const char*>(params, "file", nullptr);
			reference->setModelPath(modelFile, true);

			if (modelFile == nullptr) {
				// Reset animation control.
				auto mact = reference->getAttachedMobileActor();
				if (mact) {
					mact->setMobileActorFlag(TES3::MobileActorFlag::IdleAnim, false);
				}
			}
			else {
				animData = reference->getAttachedAnimationData();
				if (!animData->hasOverrideAnimations()) {
					throw std::logic_error("Animation file failed to load.");
				}
			}
		}

		void playAnimation(sol::table params) {
			TES3::Reference* reference = getOptionalParamExecutionReference(params);
			if (reference == nullptr) {
				throw std::invalid_argument("Invalid 'reference' parameter provided.");
			}

			// Ensure there is target data to override.
			auto animData = reference->getAttachedAnimationData();
			if (reference->sceneNode == nullptr || animData == nullptr) {
				return;
			}

			// Allow loading an animation temporarily.
			const char* modelFile = getOptionalParam<const char*>(params, "mesh", nullptr);
			if (modelFile != nullptr) {
				reference->setModelPath(modelFile, true);
				animData = reference->getAttachedAnimationData();
			}

			int group = getOptionalParam<int>(params, "group", -1);
			if (group < -1 || group > 149) {
				throw std::invalid_argument("Invalid 'group' parameter provided: must be between 0 and 149.");
			}

			int lowerGroup = getOptionalParam<int>(params, "lower", group);
			if (lowerGroup < -1 || lowerGroup > 149) {
				throw std::invalid_argument("Invalid 'lowerGroup' parameter provided: must be between 0 and 149.");
			}

			int upperGroup = getOptionalParam<int>(params, "upper", lowerGroup);
			if (upperGroup < -1 || upperGroup > 149) {
				throw std::invalid_argument("Invalid 'upperGroup' parameter provided: must be between 0 and 149.");
			}

			int shieldGroup = getOptionalParam<int>(params, "shield", upperGroup);
			if (shieldGroup < -1 || shieldGroup > 149) {
				throw std::invalid_argument("Invalid 'shieldGroup' parameter provided: must be between 0 and 149.");
			}

			// Play anim group 0 (returns control to AI) if no groups are specified.
			if (lowerGroup == -1 && upperGroup == -1 && shieldGroup == -1) {
				group = lowerGroup = upperGroup = shieldGroup = 0;
			}

			// Default to immediate start and infinite looping.
			int startFlag = getOptionalParam<int>(params, "startFlag", 1);
			int loopCount = getOptionalParam<int>(params, "loopCount", -1);

			// Start animations.
			if (lowerGroup != -1) {
				animData->playAnimationGroupForIndex(lowerGroup, 0, startFlag, loopCount);
			}
			if (upperGroup != -1) {
				animData->playAnimationGroupForIndex(upperGroup, 1, startFlag, loopCount);
			}
			if (shieldGroup != -1) {
				animData->playAnimationGroupForIndex(shieldGroup, 2, startFlag, loopCount);
			}

			auto mact = reference->getAttachedMobileActor();
			if (mact) {
				// If no overall group is specified, do not idle AI and only override specified body part groups.
				if (group == -1) {
					if (mact->animationController.asActor) {
						unsigned char targetBones = 0xFF;
						if (lowerGroup != -1) {
							targetBones = 0;
						}
						else if (upperGroup != -1) {
							targetBones = 1;
						}
						else if (shieldGroup != -1) {
							targetBones = 2;
						}
						mact->animationController.asActor->patchedOverrideState = targetBones;
					}
				}
				else {
					// Idle anim flag pauses all AI animation control.
					bool idleAnim = getOptionalParam<bool>(params, "idleAnim", lowerGroup > 0 || upperGroup > 0 || shieldGroup > 0);
					mact->setMobileActorFlag(TES3::MobileActorFlag::IdleAnim, idleAnim);
					if (mact->animationController.asActor) {
						mact->animationController.asActor->patchedOverrideState = 0xFF;
					}
				}
			}
		}

		void cancelAnimationLoop(sol::table params) {
			TES3::Reference* reference = getOptionalParamExecutionReference(params);
			if (reference == nullptr) {
				throw std::invalid_argument("Invalid 'reference' parameter provided.");
			}

			auto animData = reference->getAttachedAnimationData();
			if (reference->sceneNode == nullptr || animData == nullptr) {
				return;
			}

			if (animData->currentAnimGroup[0] != 0) {
				animData->loopCounts[0] = 0;
			}
			if (animData->currentAnimGroup[1] != 0) {
				animData->loopCounts[1] = 0;
			}
			if (animData->currentAnimGroup[2] != 0) {
				animData->loopCounts[2] = 0;
			}
		}

		void skipAnimationFrame(sol::table params) {
			TES3::Reference* reference = getOptionalParamExecutionReference(params);
			if (reference == nullptr) {
				throw std::invalid_argument("Invalid 'reference' parameter provided.");
			}

			auto animData = reference->getAttachedAnimationData();
			if (animData == nullptr) {
				return;
			}

			animData->flags |= 0xFFFF;
		}

		sol::optional<sol::table> getAnimationTiming(sol::table params, sol::this_state thisState) {
			TES3::Reference* reference = getOptionalParamExecutionReference(params);
			if (reference == nullptr) {
				throw std::invalid_argument("Invalid 'reference' parameter provided.");
			}

			auto animData = reference->getAttachedAnimationData();
			if (animData == nullptr) {
				return {};
			}

			sol::state_view state = thisState;
			sol::table result = state.create_table();
			for (size_t i = 0; i < 3; i++) {
				result[i + 1] = animData->timing[i];
			}
			return result;
		}

		void setAnimationTiming(sol::table params) {
			TES3::Reference* reference = getOptionalParamExecutionReference(params);
			if (reference == nullptr) {
				throw std::invalid_argument("Invalid 'reference' parameter provided.");
			}

			auto animData = reference->getAttachedAnimationData();
			if (animData == nullptr) {
				return;
			}

			sol::object timing = params["timing"];
			if (timing.valid() && timing != sol::nil) {
				if (timing.is<float>()) {
					const float fTiming = timing.as<float>();
					animData->timing[0] = fTiming;
					animData->timing[1] = fTiming;
					animData->timing[2] = fTiming;
				}
				else if (timing.is<sol::table>() && timing.as<sol::table>().size() == 3) {
					auto timings = timing.as<sol::table>();
					animData->timing[0] = timings[1];
					animData->timing[1] = timings[2];
					animData->timing[2] = timings[3];
				}
			}
		}

		bool isAffectedBy(sol::table params) {
			TES3::Reference* reference = getOptionalParamExecutionReference(params);
			if (reference == nullptr) {
				throw std::invalid_argument("Invalid 'reference' parameter provided.");
			}

			auto mact = reference->getAttachedMobileActor();
			if (mact == nullptr) {
				throw std::invalid_argument("Invalid 'reference' parameter provided. No mobile actor found.");
			}

			// Are we checking for being affected by an object?
			int effectId = getOptionalParam<int>(params, "effect", -1);
			TES3::BaseObject* object = getOptionalParamObject<TES3::BaseObject>(params, "object");
			if (object != nullptr) {
				if (object->objectType == TES3::ObjectType::Alchemy) {
					return mact->isAffectedByAlchemy(static_cast<TES3::Alchemy*>(object));
				}
				else if (object->objectType == TES3::ObjectType::Enchantment) {
					return mact->isAffectedByEnchantment(static_cast<TES3::Enchantment*>(object));
				}
				else if (object->objectType == TES3::ObjectType::Spell) {
					return mact->isAffectedBySpell(static_cast<TES3::Spell*>(object));
				}
				else if (object->objectType == TES3::ObjectType::MagicEffect) {
					effectId = static_cast<TES3::MagicEffect*>(object)->id;
				}
				else {
					throw std::invalid_argument("Invalid 'object' parameter provided.");
				}
			}

			// Check based on effect ID.
			if (effectId > -1) {
				auto effectController = TES3::DataHandler::get()->nonDynamicData->magicEffects;
				sol::optional<int> skillOrAttributeID;
				if (effectController->getEffectFlag(effectId, TES3::EffectFlag::TargetAttributeBit)) {
					skillOrAttributeID = getOptionalParam<int>(params, "attribute");
					if (!skillOrAttributeID) {
						throw std::invalid_argument("Invalid 'attribute' parameter provided. The given effect requires an associated attribute.");
					}
				}
				else if (effectController->getEffectFlag(effectId, TES3::EffectFlag::TargetSkillBit)) {
					skillOrAttributeID = getOptionalParam<int>(params, "skill");
					if (!skillOrAttributeID) {
						throw std::invalid_argument("Invalid 'skill' parameter provided. The given effect requires an associated skill.");
					}
				}

				for (auto& activeEffect : mact->activeMagicEffects) {
					if (activeEffect.magicEffectID == effectId && (!skillOrAttributeID || activeEffect.skillOrAttributeID == skillOrAttributeID)) {
						return true;
					}
				}
			}
			else {
				throw std::invalid_argument("Invalid 'effect' parameter provided.");
			}

			return false;
		}

		std::pair<float, unsigned int> getEffectMagnitude(sol::table params) {
			TES3::Reference* reference = getOptionalParamExecutionReference(params);
			if (reference == nullptr) {
				throw std::invalid_argument("Invalid 'reference' parameter provided.");
			}

			auto mact = reference->getAttachedMobileActor();
			if (mact == nullptr) {
				throw std::invalid_argument("Invalid 'reference' parameter provided. No mobile actor found.");
			}

			int effectId = getOptionalParam<int>(params, "effect", -1);
			auto effectController = TES3::DataHandler::get()->nonDynamicData->magicEffects;
			if (!effectController->getEffectExists(effectId)) {
				throw std::invalid_argument("Invalid 'effect' parameter provided. No effect exists with the given id.");
			}

			if (effectController->getEffectFlag(effectId, TES3::EffectFlag::NoMagnitudeBit)) {
				return std::make_pair(0.0f, 0);
			}

			// Get our attribute or skill, if appropriate.
			sol::optional<int> skillOrAttributeID;
			if (effectController->getEffectFlag(effectId, TES3::EffectFlag::TargetAttributeBit)) {
				skillOrAttributeID = getOptionalParam<int>(params, "attribute");
				if (!skillOrAttributeID) {
					return std::make_pair(0.0f, 0);
				}
			}
			else if (effectController->getEffectFlag(effectId, TES3::EffectFlag::TargetSkillBit)) {
				skillOrAttributeID = getOptionalParam<int>(params, "skill");
				if (!skillOrAttributeID) {
					return std::make_pair(0.0f, 0);
				}
			}

			auto magicInstanceController = TES3::WorldController::get()->magicInstanceController;
			auto referenceID = reference->getObjectID();
			unsigned int unresistedMagnitude = 0;
			float magnitude = 0;

			for (auto& activeEffect : mact->activeMagicEffects) {
				if (activeEffect.magicEffectID == effectId && (!skillOrAttributeID || activeEffect.skillOrAttributeID == skillOrAttributeID)) {
					auto instance = magicInstanceController->getInstanceFromSerial(activeEffect.magicInstanceSerial);
					if (instance) {
						auto effectInstance = instance->effects[activeEffect.magicInstanceEffectIndex].getNode(referenceID);
						if (effectInstance) {
							unresistedMagnitude += effectInstance->value.magnitude;
							magnitude += effectInstance->value.magnitude * (1.0f - effectInstance->value.resistedPercent / 100.0f);
						}
					}
				}
			}
			return std::make_pair(magnitude, unresistedMagnitude);
		}

		TES3::MagicEffect* addMagicEffect(sol::table params) {
			auto magicEffectController = TES3::DataHandler::get()->nonDynamicData->magicEffects;

			// Get the new ID.
			int id = getOptionalParam<int>(params, "id", -1);;
			if (id <= TES3::EffectID::LastEffect || id >= TES3::EFFECT_ID_INVALID) {
				throw std::invalid_argument("Invalid 'id' parameter provided. It must be an integer greater than 142 and less than 32766.");
			}
			else if (magicEffectController->getEffectExists(id)) {
				throw std::invalid_argument("Invalid 'id' parameter provided. An effect with this id already exists.");
			}

			// Create the effect and assign basic properties.
			auto effect = new TES3::MagicEffect(id);
			effect->baseMagickaCost = getOptionalParam<float>(params, "baseCost", 1.0f);
			effect->school = getOptionalParam<int>(params, "school", 0);
			effect->size = getOptionalParam<float>(params, "size", 1.0f);
			effect->sizeCap = getOptionalParam<float>(params, "sizeCap", 1.0f);
			effect->speed = getOptionalParam<float>(params, "speed", 1.0f);

			// Description.
			sol::optional<std::string> description = params["description"];
			if (description) {
				effect->setDescription(description.value().c_str());
			}
			else {
				effect->setDescription("No description available.");
			}

			// Set color.
			auto lighting = getOptionalParamVector3(params, "lighting");
			if (lighting) {
				effect->lightingRed = int(std::clamp(lighting.value().x, 0.0f, 1.0f) * 255);
				effect->lightingGreen = int(std::clamp(lighting.value().y, 0.0f, 1.0f) * 255);
				effect->lightingBlue = int(std::clamp(lighting.value().z, 0.0f, 1.0f) * 255);
			}
			else {
				effect->lightingRed = 255;
				effect->lightingGreen = 255;
				effect->lightingBlue = 255;
			}

			sol::optional<std::string> icon = params["icon"];
			if (!icon || icon.value().length() >= 31) {
				delete effect;
				throw std::invalid_argument("Invalid 'icon' parameter provided. Must be a string no longer than 31 characters long.");
			}
			else {
				strcpy_s(effect->icon, 32, icon.value().c_str());
			}

			sol::optional<std::string> particleTexture = params["particleTexture"];
			if (!particleTexture || particleTexture.value().length() >= 31) {
				delete effect;
				throw std::invalid_argument("Invalid 'particleTexture' parameter provided. Must be a string no longer than 31 characters long.");
			}
			else {
				strcpy_s(effect->particleTexture, 32, particleTexture.value().c_str());
			}

			sol::optional<std::string> castSound = params["castSound"];
			if (!castSound || castSound.value().length() >= 31) {
				delete effect;
				throw std::invalid_argument("Invalid 'castSound' parameter provided. Must be a string no longer than 31 characters long.");
			}
			else {
				strcpy_s(effect->castSoundEffect, 32, castSound.value().c_str());
			}

			sol::optional<std::string> boltSound = params["boltSound"];
			if (!boltSound || boltSound.value().length() >= 31) {
				delete effect;
				throw std::invalid_argument("Invalid 'boltSound' parameter provided. Must be a string no longer than 31 characters long.");
			}
			else {
				strcpy_s(effect->boltSoundEffect, 32, boltSound.value().c_str());
			}

			sol::optional<std::string> hitSound = params["hitSound"];
			if (!hitSound || hitSound.value().length() >= 31) {
				delete effect;
				throw std::invalid_argument("Invalid 'hitSound' parameter provided. Must be a string no longer than 31 characters long.");
			}
			else {
				strcpy_s(effect->hitSoundEffect, 32, hitSound.value().c_str());
			}

			sol::optional<std::string> areaSound = params["areaSound"];
			if (!areaSound || areaSound.value().length() >= 31) {
				delete effect;
				throw std::invalid_argument("Invalid 'areaSound' parameter provided. Must be a string no longer than 31 characters long.");
			}
			else {
				strcpy_s(effect->areaSoundEffect, 32, areaSound.value().c_str());
			}

			effect->castEffect = getOptionalParamObject<TES3::PhysicalObject>(params, "castVFX");
			effect->boltEffect = getOptionalParamObject<TES3::PhysicalObject>(params, "boltVFX");
			effect->hitEffect = getOptionalParamObject<TES3::PhysicalObject>(params, "hitVFX");
			effect->areaEffect = getOptionalParamObject<TES3::PhysicalObject>(params, "areaVFX");

			sol::optional<std::string> name = params["name"];
			if (name) {
				magicEffectController->effectCustomNames[id] = name.value();
			}
			else {
				magicEffectController->effectCustomNames[id] = "Unnamed Effect";
			}

			// Actually add the effect.
			magicEffectController->addEffectObject(effect);

			// Set individual flags.
			magicEffectController->setEffectFlags(id, 0U);
			magicEffectController->setEffectFlag(id, TES3::EffectFlag::AllowEnchantingBit,
				getOptionalParam<bool>(params, "allowEnchanting", true));
			magicEffectController->setEffectFlag(id, TES3::EffectFlag::AllowSpellmakingBit,
				getOptionalParam<bool>(params, "allowSpellmaking", true));
			magicEffectController->setEffectFlag(id, TES3::EffectFlag::AppliedOnceBit,
				getOptionalParam<bool>(params, "appliesOnce", true));
			magicEffectController->setEffectFlag(id, TES3::EffectFlag::CanCastSelfBit,
				getOptionalParam<bool>(params, "canCastSelf", true));
			magicEffectController->setEffectFlag(id, TES3::EffectFlag::CanCastTargetBit,
				getOptionalParam<bool>(params, "canCastTarget", true));
			magicEffectController->setEffectFlag(id, TES3::EffectFlag::CanCastTouchBit,
				getOptionalParam<bool>(params, "canCastTouch", true));
			magicEffectController->setEffectFlag(id, TES3::EffectFlag::CasterLinkedBit,
				getOptionalParam<bool>(params, "casterLinked", true));
			magicEffectController->setEffectFlag(id, TES3::EffectFlag::ContinuousVFXBit,
				getOptionalParam<bool>(params, "hasContinuousVFX", true));
			magicEffectController->setEffectFlag(id, TES3::EffectFlag::NoDurationBit,
				getOptionalParam<bool>(params, "hasNoDuration", true));
			magicEffectController->setEffectFlag(id, TES3::EffectFlag::NoMagnitudeBit,
				getOptionalParam<bool>(params, "hasNoMagnitude", true));
			magicEffectController->setEffectFlag(id, TES3::EffectFlag::IllegalDaedraBit,
				getOptionalParam<bool>(params, "illegalDaedra", true));
			magicEffectController->setEffectFlag(id, TES3::EffectFlag::HarmfulBit,
				getOptionalParam<bool>(params, "isHarmful", true));
			magicEffectController->setEffectFlag(id, TES3::EffectFlag::NonRecastableBit,
				getOptionalParam<bool>(params, "nonRecastable", true));
			magicEffectController->setEffectFlag(id, TES3::EffectFlag::TargetAttributeBit,
				getOptionalParam<bool>(params, "targetsAttributes", true));
			magicEffectController->setEffectFlag(id, TES3::EffectFlag::TargetSkillBit,
				getOptionalParam<bool>(params, "targetsSkills", true));
			magicEffectController->setEffectFlag(id, TES3::EffectFlag::UnreflectableBit,
				getOptionalParam<bool>(params, "unreflectable", true));
			magicEffectController->setEffectFlag(id, TES3::EffectFlag::NegativeLightingBit,
				getOptionalParam<bool>(params, "usesNegativeLighting", true));
			effect->flags = magicEffectController->getEffectFlags(id);

			// Get the tick function.
			sol::optional<sol::protected_function> onTick = params["onTick"];
			if (onTick) {
				magicEffectController->effectLuaTickFunctions[id] = onTick.value();
			}
			sol::optional<sol::protected_function> onCollision = params["onCollision"];
			if (onCollision) {
				magicEffectController->effectLuaCollisionFunctions[id] = onCollision.value();
			}

			// Set the GMST as the negative effect ID for custom hooks later.
			magicEffectController->effectNameGMSTs[id] = -id;

			return effect;
		}

		double advanceTime(sol::table params) {
			float rawHours = getOptionalParam(params, "hours", 0.0f);
			float hoursPassed = 0.0f;

			// Leverage the resting/waiting system if we're advancing by an hour or more.
			if (rawHours >= 1.0) {
				int hoursToAdvance = int(std::floor(rawHours));

				auto worldController = TES3::WorldController::get();
				worldController->gvarGameHour->value += rawHours - hoursToAdvance;
				hoursPassed += rawHours - hoursToAdvance;

				bool resting = getOptionalParam<bool>(params, "resting", false);
				bool updateEnvironment = getOptionalParam<bool>(params, "updateEnvironment", true);

				auto macp = worldController->getMobilePlayer();
				macp->restHoursRemaining = hoursToAdvance;
				macp->sleeping = resting;
				macp->waiting = !resting;

				while (macp->restHoursRemaining > 0) {
					reinterpret_cast<void(__cdecl*)(bool)>(0x6350B0)(updateEnvironment);
					hoursPassed += 1;
				}
			}
			// Otherwise, just do the bare minimum.
			else {
				auto worldController = TES3::WorldController::get();
				worldController->gvarGameHour->value += rawHours;
				worldController->checkForDayWrapping();
				worldController->updateEnvironmentLightingWeather();
				worldController->processGlobalScripts();
				hoursPassed += rawHours;
			}

			return hoursPassed;
		}

		// WARNING: This function is not yet complete.
		bool beginTransform(sol::table params) {
			TES3::Reference* reference = getOptionalParamExecutionReference(params);
			if (reference == nullptr) {
				throw std::invalid_argument("Invalid 'reference' parameter provided.");
			}

			auto mact = reference->getAttachedMobileActor();
			if (mact == nullptr) {
				throw std::invalid_argument("Invalid 'reference' parameter provided. No mobile actor found.");
			}

			if (mact->getIsWerewolf()) {
				return false;
			}

			if (mact->isDead()) {
				return false;
			}

			mact->changeWerewolfState(true);
			mact->setMobileActorFlag(TES3::MobileActorFlag::BodypartsChanged, false);

			auto macp = TES3::WorldController::get()->getMobilePlayer();
			if (mact == macp) {
				auto worldController = TES3::WorldController::get();

				// Change FOV.
				worldController->shadowCamera.cameraData.setFOV(float(worldController->werewolfFOV));
				worldController->worldCamera.cameraData.setFOV(float(worldController->werewolfFOV));

				// Update faders.
				worldController->werewolfFader->updateMaterialProperty(1.0f);
				worldController->sunglareFader->deactivate();
				worldController->werewolfFader->activate();
			}

			return true;
		}

		bool undoTransform(sol::table params) {
			TES3::Reference* reference = getOptionalParamExecutionReference(params);
			if (reference == nullptr) {
				throw std::invalid_argument("Invalid 'reference' parameter provided.");
			}

			auto mact = reference->getAttachedMobileActor();
			if (mact == nullptr) {
				throw std::invalid_argument("Invalid 'reference' parameter provided. No mobile actor found.");
			}

			if (!mact->getIsWerewolf()) {
				return false;
			}

			if (mact->isDead()) {
				return false;
			}

			mact->changeWerewolfState(false);
			mact->setMobileActorFlag(TES3::MobileActorFlag::BodypartsChanged, true);

			auto macp = TES3::WorldController::get()->getMobilePlayer();
			if (mact == macp) {
				auto worldController = TES3::WorldController::get();

				// Change FOV.
				worldController->shadowCamera.cameraData.setFOV(75.0f);
				worldController->worldCamera.cameraData.setFOV(75.0f);

				// Update faders.
				worldController->sunglareFader->activate();
				worldController->werewolfFader->deactivate();
			}

			return true;
		}

		void setMarkLocation(sol::table params) {
			auto macp = TES3::WorldController::get()->getMobilePlayer();

			// Get the position.
			sol::optional<TES3::Vector3> position = getOptionalParamVector3(params, "position");
			if (!position) {
				throw std::invalid_argument("Invalid 'position' parameter provided.");
			}

			// Get the orientation.
			float rotation = getOptionalParam<float>(params, "rotation", macp->reference->orientation.z);

			// Get the cell.
			TES3::Cell* cell = getOptionalParamCell(params, "cell");
			if (cell == nullptr) {
				// Try to find an exterior cell from the position.
				int gridX = TES3::Cell::toGridCoord(position.value().x);
				int gridY = TES3::Cell::toGridCoord(position.value().y);
				cell = TES3::DataHandler::get()->nonDynamicData->getCellByGrid(gridX, gridY);
				if (cell == nullptr) {
					throw std::invalid_argument("Invalid 'cell' parameter provided. Could not resolve cell by exterior position.");
				}
			}

			if (macp->markLocation == nullptr) {
				macp->markLocation = tes3::_new<TES3::MarkData>();
			}

			macp->markLocation->position = position.value();
			macp->markLocation->rotation = rotation;
			macp->markLocation->cell = cell;
		}

		void clearMarkLocation() {
			auto macp = TES3::WorldController::get()->getMobilePlayer();
			if (macp->markLocation) {
				tes3::_delete(macp->markLocation);
				macp->markLocation = nullptr;
			}
		}

		bool getItemIsStolen(sol::table params) {
			auto item = getOptionalParamObject<TES3::Item>(params, "item");
			auto from = getOptionalParamObject<TES3::BaseObject>(params, "from");

			if (item == nullptr) {
				throw std::invalid_argument("Invalid 'item' parameter provided.");
			}
			else if (from == nullptr) {
				throw std::invalid_argument("Invalid 'from' parameter provided.");
			}
			else if (item->getStolenList() == nullptr) {
				throw std::invalid_argument("Invalid 'item' parameter provided. Does not support a stolen list.");
			}

			return item->getStolenFlag(from);
		}

		void setItemIsStolen(sol::table params) {
			auto item = getOptionalParamObject<TES3::Item>(params, "item");
			auto from = getOptionalParamObject<TES3::BaseObject>(params, "from");

			if (item == nullptr) {
				throw std::invalid_argument("Invalid 'item' parameter provided.");
			}
			else if (from == nullptr) {
				throw std::invalid_argument("Invalid 'from' parameter provided.");
			}
			else if (item->getStolenList() == nullptr) {
				throw std::invalid_argument("Invalid 'item' parameter provided. Does not support a stolen list.");
			}

			if (getOptionalParam<bool>(params, "stolen", true)) {
				item->addStolenFlag(from);
			}
			else {
				item->removeStolenFlag(from);
			}
		}

		// Legacy tes3.getOwner.
		TES3::BaseObject* getOwnerLegacy(TES3::Reference* reference) {
			auto itemData = reference->getAttachedItemData();
			if (itemData == nullptr) {
				return nullptr;
			}

			return itemData->owner;
		}

		std::tuple<sol::object, sol::object> getOwner(sol::table params) {
			// Get the reference to get ownership for.
			TES3::Reference* reference = getOptionalParamReference(params, "reference");
			if (reference == nullptr) {
				throw std::invalid_argument("Invalid 'reference' parameter provided.");
			}

			auto itemData = reference->getAttachedItemData();
			if (itemData == nullptr || itemData->owner == nullptr) {
				return std::make_tuple<sol::object, sol::object>(sol::nil, sol::nil);
			}

			if (itemData->owner->objectType == TES3::ObjectType::NPC) {
				return std::make_tuple<sol::object, sol::object>(itemData->owner->getOrCreateLuaObject(params.lua_state()), itemData->requiredVariable->getOrCreateLuaObject(params.lua_state()));
			}
			else if (itemData->owner->objectType == TES3::ObjectType::Faction) {
				return std::make_tuple<sol::object, sol::object>(itemData->owner->getOrCreateLuaObject(params.lua_state()), sol::make_object(params.lua_state(), itemData->requiredRank));
			}
			else {
				throw std::runtime_error("Owner was not of a valid type. Report this issue to MWSE developers!");
			}
		}

		void setOwner(sol::table params) {
			// Get the reference to set ownership for.
			TES3::Reference* reference = getOptionalParamReference(params, "reference");
			if (reference == nullptr) {
				throw std::invalid_argument("Invalid 'reference' parameter provided.");
			}

			// Are we removing an actor?
			if (getOptionalParam<bool>(params, "remove", false)) {
				auto itemData = reference->getAttachedItemData();
				if (itemData) {
					itemData->owner = nullptr;
					itemData->requiredVariable = nullptr;
				}
				return;
			}

			// Are we getting an actor?
			TES3::BaseObject* owner = getOptionalParamBaseObject(params, "owner");
			if (owner == nullptr) {
				throw std::invalid_argument("Invalid 'owner' parameter provided.");
			}

			// Setting an NPC?
			else if (owner->objectType == TES3::ObjectType::NPC) {
				auto itemData = reference->getOrCreateAttachedItemData();
				itemData->owner = owner;
				itemData->requiredVariable = getOptionalParamObject<TES3::GlobalVariable>(params, "requiredGlobal");
			}

			// Setting a faction?
			else if (owner->objectType == TES3::ObjectType::Faction) {
				auto itemData = reference->getOrCreateAttachedItemData();
				itemData->owner = owner;
				itemData->requiredRank = getOptionalParam(params, "requiredRank", 0);
			}

			// Given a bogus object type?
			else {
				throw std::invalid_argument("Invalid 'owner' parameter provided: must be of type NPC or Faction.");
			}

			// Finish up.
			reference->setObjectModified(true);
		}

		int getKillCount(sol::optional<sol::table> params) {
			if (params) {
				auto actor = getOptionalParamObject<TES3::Actor>(params, "actor");
				if (actor == nullptr) {
					throw std::invalid_argument("Invalid 'actor' parameter provided: must be an actor id.");
				}
				return TES3::WorldController::get()->playerKills->getKillCount(actor);
			}
			else {
				return TES3::WorldController::get()->playerKills->totalKills;
			}
		}

		void setKillCount(sol::table params) {
			auto actor = getOptionalParamObject<TES3::Actor>(params, "actor");
			if (actor == nullptr) {
				throw std::invalid_argument("Invalid 'actor' parameter provided: must be an actor id.");
			}
			
			sol::optional<int> count = params["count"];
			if (!count) {
				throw std::invalid_argument("Invalid 'count' parameter provided: must be a number.");
			}

			TES3::WorldController::get()->playerKills->setKillCount(actor, count.value());
		}

		void decrementKillCount(sol::table params) {
			auto actor = getOptionalParamObject<TES3::Actor>(params, "actor");
			if (actor == nullptr) {
				throw std::invalid_argument("Invalid 'actor' parameter provided: must be an actor id.");
			}
			TES3::WorldController::get()->playerKills->decrement(actor);
		}

		void incrementKillCount(sol::table params) {
			auto actor = getOptionalParamObject<TES3::Actor>(params, "actor");
			if (actor == nullptr) {
				throw std::invalid_argument("Invalid 'actor' parameter provided: must be an actor id.");
			}
			TES3::WorldController::get()->playerKills->increment(actor);
		}

		sol::table getKillCounts(sol::this_state ts) {
			sol::state_view state = ts;
			sol::table killMap = state.create_table();
			for (auto& itt : *TES3::WorldController::get()->playerKills->counter) {
				killMap[itt.first] = itt.second;
			}
			return killMap;
		}

		int getWerewolfKillCount() {
			return TES3::WorldController::get()->playerKills->werewolfKills;
		}

		void setWerewolfKillCount(int count) {
			TES3::WorldController::get()->playerKills->werewolfKills = count;
		}

		TES3::Birthsign* findBirthsign(const char* id) {
			for (auto birthsign : *TES3::DataHandler::get()->nonDynamicData->birthsigns) {
				if (mwse::string::iequal(id, birthsign->id)) {
					return birthsign;
				}
			}
			return nullptr;
		}

		void setSourceless(TES3::BaseObject* object, sol::optional<bool> sourceless) {
			if (object) {
				object->setSourceless(sourceless.value_or(true));
			}
		}

		sol::optional<bool> canRest() {
			auto worldController = TES3::WorldController::get();
			if (worldController) {
				return worldController->mobController->processManager->canRest();
			}
			return {};
		}

		bool showRestMenu(sol::optional<sol::table> params) {
			auto worldController = TES3::WorldController::get();
			auto dataHandler = TES3::DataHandler::get();
			if (!worldController || !dataHandler) return false;

			// Check to see if there are enemies nearby.
			if (getOptionalParam<bool>(params, "checkForEnemies", true) && !worldController->mobController->processManager->canRest()) {
				if (getOptionalParam<bool>(params, "showMessage", true)) {
					TES3::UI::showMessageBox(dataHandler->nonDynamicData->GMSTs[TES3::GMST::sNotifyMessage2]->value.asString, nullptr, true);
				}
				return false;
			}

			// Check to make sure the player isn't underwater, jumping, or flying.
			auto macp = worldController->getMobilePlayer();
			if (getOptionalParam<bool>(params, "checkForSolidGround", true)) {
				const auto excludedMovement = TES3::ActorMovement::Flying | TES3::ActorMovement::Jumping;
				if ((macp->movementFlags & excludedMovement) || macp->getBasePositionIsUnderwater()) {
					if (getOptionalParam<bool>(params, "showMessage", true)) {
						TES3::UI::showMessageBox(dataHandler->nonDynamicData->GMSTs[TES3::GMST::sNotifyMessage1]->value.asString, nullptr, true);
					}
					return false;
				}
			}

			// Allow using resting/waiting param. Defaults to resting.
			bool resting = getOptionalParam(params, "resting", !getOptionalParam(params, "waiting", false));

			// Is sleeping illegal here?
			if (resting && getOptionalParam<bool>(params, "checkSleepingIllegal", true) && dataHandler->currentCell->getSleepingIsIllegal()) {
				resting = false;
			}

			// Werewolves can't sleep.
			if (resting && getOptionalParam<bool>(params, "checkIsWerewolf", true) && macp->getIsWerewolf()) {
				resting = false;
			}

			// Finally show the message.
			TES3::UI::showRestMenu(resting);
			return true;
		}

		bool setPlayerControlState(sol::optional<sol::table> params) {
			auto worldController = TES3::WorldController::get();
			if (!worldController) {
				return false;
			}

			auto macp = worldController->getMobilePlayer();
			if (!macp) {
				return false;
			}

			// Set individual control states.
			auto enabled = getOptionalParam<bool>(params, "enabled", false);
			macp->controlsDisabled = !enabled;
			macp->attackDisabled = !getOptionalParam(params, "attack", enabled);
			macp->jumpingDisabled = !getOptionalParam(params, "jumping", enabled);
			macp->magicDisabled = !getOptionalParam(params, "magic", enabled);
			macp->vanityDisabled = !getOptionalParam(params, "vanity", enabled);
			macp->viewSwitchDisabled = !getOptionalParam(params, "viewSwitch", enabled);
			return true;
		}

		bool wakeUp() {
			auto worldController = TES3::WorldController::get();
			if (!worldController) {
				return false;
			}

			auto macp = worldController->getMobilePlayer();
			if (!macp) {
				return false;
			}

			// Make sure we're currently sleeping/waiting.
			if (!macp->sleeping && !macp->waiting) {
				return false;
			}

			macp->wakeUp();
			return true;
		}

		bool setVanityMode(sol::optional<sol::table> params) {
			auto worldController = TES3::WorldController::get();
			if (!worldController) {
				return false;
			}

			auto macp = worldController->getMobilePlayer();
			if (!macp) {
				return false;
			}

			// Do nothing if vanity is disabled.
			if (macp->vanityDisabled && getOptionalParam(params, "checkVanityDisabled", true)) {
				return false;
			}

			// Allow toggling.
			bool enabled = getOptionalParam(params, "enabled", true);
			if (getOptionalParam(params, "toggle", false)) {
				enabled = macp->getVanityState() == 0;
			}

			macp->setVanityState(enabled ? 2 : 0);
			return true;
		}

		bool getVanityMode() {
			auto worldController = TES3::WorldController::get();
			if (!worldController) {
				return false;
			}

			auto macp = worldController->getMobilePlayer();
			if (!macp) {
				return false;
			}

			return macp->getVanityState() != 0;
		}

		bool testLineOfSight(sol::table params) {
			// Were we given position data directly?
			auto position1 = getOptionalParamVector3(params, "position1");
			if (position1) {
				auto position2 = getOptionalParamVector3(params, "position2");
				auto height1 = getOptionalParam<float>(params, "height1", 0.0f);
				auto height2 = getOptionalParam<float>(params, "height2", 0.0f);

				if (!position2) {
					throw std::invalid_argument("Invalid positional params provided. Must provided two references or two positions.");
				}

				return tes3::testLineOfSight(&position1.value(), height1, &position2.value(), height2);
			}

			// Were we given two actors?
			auto reference1 = getOptionalParamReference(params, "reference1");
			auto reference2 = getOptionalParamReference(params, "reference2");
			if (!reference1 || !reference2) {
				throw std::invalid_argument("Invalid positional params provided. Must provided two references or two positions/heights.");
			}

			// 
			sol::optional<TES3::Vector3> position2;
			float height1, height2;

			// Try to get the first reference's data.
			auto mobile1 = reference1->getAttachedMobileActor();
			position1 = reference1->position;
			if (mobile1) {
				height1 = mobile1->height;
			}
			else if (reference1->baseObject->boundingBox) {
				// Use distance from centre to top of bounding box as height.
				height1 = reference1->baseObject->boundingBox->maximum.z;
			}
			else {
				throw std::invalid_argument("Could not determine first reference's height data.");
			}

			// Try to get the second reference's data.
			auto mobile2 = reference2->getAttachedMobileActor();
			position2 = reference2->position;
			if (mobile2) {
				height2 = mobile2->height;
			}
			else if (reference2->baseObject->boundingBox) {
				// Use distance from centre to top of bounding box as height.
				height2 = reference2->baseObject->boundingBox->maximum.z;
			}
			else {
				throw std::invalid_argument("Could not determine second reference's height data.");
			}

			return tes3::testLineOfSight(&position1.value(), height1, &position2.value(), height2);
		}

		sol::object findActorsInProximity(sol::table params) {
			TES3::Reference* reference = getOptionalParamReference(params, "reference");
			auto position = getOptionalParamVector3(params, "position");
			auto range = getOptionalParam<float>(params, "range");

			if (reference) {
				position = reference->position;

				// Use centre of body location to match how the findActorsInProximity treats mobiles.
				auto mobile = reference->getAttachedMobileActor();
				if (mobile) {
					position.value().z += 0.5f * mobile->height;
				}
			}

			if (!position) {
				throw std::invalid_argument("A valid 'reference' or 'position' parameter is required.");
			}
			if (!range) {
				throw std::invalid_argument("Invalid 'range' parameter provided.");
			}

			TES3::IteratedList<TES3::MobileActor*> actors;
			auto processManager = TES3::WorldController::get()->mobController->processManager;
			processManager->findActorsInProximity(&position.value(), range.value(), &actors);

			// Convert list to lua array.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::table result = stateHandle.state.create_table();
			for (auto& i : actors) {
				result.add(i);
			}

			return result;
		}

		void bindTES3Util() {
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			//
			// Extend tes3 library with extra functions.
			//

			sol::table tes3 = state["tes3"];

			tes3["addArmorSlot"] = addArmorSlot;
			tes3["addItem"] = addItem;
			tes3["addItemData"] = addItemData;
			tes3["addJournalEntry"] = addJournalEntry;
			tes3["addMagicEffect"] = addMagicEffect;
			tes3["addSoulGem"] = addSoulGem;
			tes3["adjustSoundVolume"] = adjustSoundVolume;
			tes3["advanceTime"] = advanceTime;
			tes3["applyMagicSource"] = applyMagicSource;
			tes3["beginTransform"] = beginTransform;
			tes3["cancelAnimationLoop"] = cancelAnimationLoop;
			tes3["canRest"] = canRest;
			tes3["cast"] = cast;
			tes3["checkMerchantOffersService"] = checkMerchantOffersService;
			tes3["checkMerchantTradesItem"] = checkMerchantTradesItem;
			tes3["clearMarkLocation"] = clearMarkLocation;
			tes3["createCell"] = createCell;
			tes3["createObject"] = createObject;
			tes3["createReference"] = createReference;
			tes3["decrementKillCount"] = decrementKillCount;
			tes3["deleteObject"] = deleteObject;
			tes3["disableKey"] = disableKey;
			tes3["dropItem"] = dropItem;
			tes3["enableKey"] = enableKey;
			tes3["fadeIn"] = fadeIn;
			tes3["fadeOut"] = fadeOut;
			tes3["fadeTo"] = fadeTo;
			tes3["findActorsInProximity"] = findActorsInProximity;
			tes3["findBirthsign"] = findBirthsign;
			tes3["findClass"] = findClass;
			tes3["findClosestExteriorReferenceOfObject"] = findClosestExteriorReferenceOfObject;
			tes3["findDialogue"] = findDialogue;
			tes3["findGlobal"] = findGlobal;
			tes3["findGMST"] = findGMST;
			tes3["findRegion"] = findRegion;
			tes3["force1stPerson"] = force1stPerson;
			tes3["force3rdPerson"] = force3rdPerson;
			tes3["get3rdPersonCameraOffset"] = get3rdPersonCameraOffset;
			tes3["getActiveCells"] = getActiveCells;
			tes3["getAnimationGroups"] = getCurrentAnimationGroups;
			tes3["getAnimationTiming"] = getAnimationTiming;
			tes3["getArchiveList"] = getArchiveList;
			tes3["getCameraPosition"] = getCameraPosition;
			tes3["getCameraVector"] = getCameraVector;
			tes3["getCell"] = getCell;
			tes3["getCumulativeDaysForMonth"] = getCumulativeDaysForMonth;
			tes3["getCurrentAIPackageId"] = getCurrentAIPackageId;
			tes3["getCurrentWeather"] = getCurrentWeather;
			tes3["getCursorPosition"] = getCursorPosition;
			tes3["getDataHandler"] = TES3::DataHandler::get;
			tes3["getDaysInMonth"] = getDaysInMonth;
			tes3["getDialogueInfo"] = getDialogueInfo;
			tes3["getEffectMagnitude"] = getEffectMagnitude;
			tes3["getEquippedItem"] = getEquippedItem;
			tes3["getFaction"] = getFaction;
			tes3["getFileExists"] = getFileExists;
			tes3["getFileSource"] = getFileSource;
			tes3["getGame"] = TES3::Game::get;
			tes3["getGlobal"] = getGlobal;
			tes3["getGMST"] = getGMST;
			tes3["getInputBinding"] = getInputBinding;
			tes3["getItemIsStolen"] = getItemIsStolen;
			tes3["getJournalIndex"] = getJournalIndex;
			tes3["getKillCount"] = getKillCount;
			tes3["getKillCounts"] = getKillCounts;
			tes3["getLanguage"] = getLanguage;
			tes3["getLanguageCode"] = getLanguageCode;
			tes3["getLastExteriorPosition"] = getLastExteriorPosition;
			tes3["getLocked"] = getLocked;
			tes3["getLockLevel"] = getLockLevel;
			tes3["getMagicEffect"] = getMagicEffect;
			tes3["getMagicSourceInstanceBySerial"] = getMagicSourceInstanceBySerial;
			tes3["getMobilePlayer"] = getMobilePlayer;
			tes3["getModList"] = getModList;
			tes3["getObject"] = getObject;
			tes3["getOwner"] = sol::overload(getOwnerLegacy, getOwner);
			tes3["getPlayerCell"] = getPlayerCell;
			tes3["getPlayerEyePosition"] = getPlayerEyePosition;
			tes3["getPlayerEyeVector"] = getPlayerEyeVector;
			tes3["getPlayerGold"] = getPlayerGold;
			tes3["getPlayerRef"] = getPlayerRef;
			tes3["getPlayerTarget"] = getPlayerTarget;
			tes3["getReference"] = getReference;
			tes3["getRegion"] = getRegion;
			tes3["getScript"] = getScript;
			tes3["getSimulationTimestamp"] = getSimulationTimestamp;
			tes3["getSkill"] = getSkill;
			tes3["getSound"] = getSound;
			tes3["getSoundGenerator"] = getSoundGenerator;
			tes3["getSoundPlaying"] = getSoundPlaying;
			tes3["getTopMenu"] = TES3::UI::getMenuOnTop;
			tes3["getTrap"] = getTrap;
			tes3["getWerewolfKillCount"] = getWerewolfKillCount;
			tes3["getWorldController"] = TES3::WorldController::get;
			tes3["hammerKey"] = hammerKey;
			tes3["hasCodePatchFeature"] = hasCodePatchFeature;
			tes3["hasOwnershipAccess"] = hasOwnershipAccess;
			tes3["incrementKillCount"] = incrementKillCount;
			tes3["is3rdPerson"] = isThirdPerson;
			tes3["isAffectedBy"] = isAffectedBy;
			tes3["isModActive"] = isModActive;
			tes3["iterateObjects"] = iterateObjects;
			tes3["loadAnimation"] = loadAnimation;
			tes3["loadGame"] = loadGame;
			tes3["loadMesh"] = loadMesh;
			tes3["loadSourceTexture"] = loadSourceTexture;
			tes3["lock"] = lock;
			tes3["messageBox"] = messageBox;
			tes3["modStatistic"] = modStatistic;
			tes3["newGame"] = tes3::startNewGame;
			tes3["persuade"] = persuade;
			tes3["playAnimation"] = playAnimation;
			tes3["playItemPickupSound"] = playItemPickupSound;
			tes3["playSound"] = playSound;
			tes3["playVoiceover"] = playVoiceover;
			tes3["positionCell"] = positionCell;
			tes3["pushKey"] = pushKey;
			tes3["random"] = random;
			tes3["rayTest"] = rayTest;
			tes3["releaseKey"] = releaseKey;
			tes3["removeEffects"] = removeEffects;
			tes3["removeItem"] = removeItem;
			tes3["removeSound"] = removeSound;
			tes3["runLegacyScript"] = runLegacyScript;
			tes3["saveGame"] = saveGame;
			tes3["say"] = say;
			tes3["set3rdPersonCameraOffset"] = set3rdPersonCameraOffset;
			tes3["setAIActivate"] = setAIActivate;
			tes3["setAIEscort"] = setAIEscort;
			tes3["setAIFollow"] = setAIFollow;
			tes3["setAITravel"] = setAITravel;
			tes3["setAIWander"] = setAIWander;
			tes3["setAnimationTiming"] = setAnimationTiming;
			tes3["setDestination"] = setDestination;
			tes3["setEnabled"] = setEnabled;
			tes3["setGlobal"] = setGlobal;
			tes3["setItemIsStolen"] = setItemIsStolen;
			tes3["setJournalIndex"] = setJournalIndex;
			tes3["setKillCount"] = setKillCount;
			tes3["setLockLevel"] = setLockLevel;
			tes3["setMarkLocation"] = setMarkLocation;
			tes3["setOwner"] = setOwner;
			tes3["setPlayerControlState"] = setPlayerControlState;
			tes3["setSourceless"] = setSourceless;
			tes3["setStatistic"] = setStatistic;
			tes3["setTrap"] = setTrap;
			tes3["setVanityMode"] = setVanityMode;
			tes3["setWerewolfKillCount"] = setWerewolfKillCount;
			tes3["showAlchemyMenu"] = showAlchemyMenu;
			tes3["showRepairServiceMenu"] = showRepairServiceMenu;
			tes3["showRestMenu"] = showRestMenu;
			tes3["skipAnimationFrame"] = skipAnimationFrame;
			tes3["streamMusic"] = streamMusic;
			tes3["tapKey"] = tapKey;
			tes3["testLineOfSight"] = testLineOfSight;
			tes3["togglePOV"] = togglePOV;
			tes3["transferItem"] = transferItem;
			tes3["triggerCrime"] = triggerCrime;
			tes3["undoTransform"] = undoTransform;
			tes3["unhammerKey"] = unhammerKey;
			tes3["unlock"] = unlock;
			tes3["updateInventoryGUI"] = updateInventoryGUI;
			tes3["updateJournal"] = updateJournal;
			tes3["wakeUp"] = wakeUp;
		}
	}
}
