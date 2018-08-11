#include "TES3UtilLua.h"

#include <algorithm>
#include "sol.hpp"
#include "LuaManager.h"

#include "TES3GameFile.h"

#include "TES3Util.h"
#include "UIUtil.h"
#include "LuaUtil.h"
#include "Stack.h"
#include "Log.h"
#include "ScriptUtil.h"
#include "CodePatchUtil.h"

#include "NICamera.h"
#include "NINode.h"
#include "NIPick.h"
#include "NIRTTI.h"
#include "NIStream.h"

#include "TES3Actor.h"
#include "TES3AudioController.h"
#include "TES3Cell.h"
#include "TES3CrimeTree.h"
#include "TES3DataHandler.h"
#include "TES3Game.h"
#include "TES3GameSetting.h"
#include "TES3GlobalVariable.h"
#include "TES3InputController.h"
#include "TES3MobController.h"
#include "TES3MobilePlayer.h"
#include "TES3NPC.h"
#include "TES3Reference.h"
#include "TES3Region.h"
#include "TES3Script.h"
#include "TES3Sound.h"
#include "TES3SoundGenerator.h"
#include "TES3Spell.h"
#include "TES3UIElement.h"
#include "TES3Weather.h"
#include "TES3WeatherController.h"
#include "TES3WorldController.h"

namespace mwse {
	namespace lua {
		auto iterateObjectsFiltered(unsigned int desiredType) {
			TES3::Object* object = tes3::getDataHandler()->nonDynamicData->list->head;
			return [object, desiredType]() mutable -> sol::object {
				while (object && desiredType != 0 && object->objectType != desiredType) {
					object = object->nextInCollection;
				}

				if (object == NULL) {
					return sol::nil;
				}

				sol::object ret = makeLuaObject(object);
				object = object->nextInCollection;
				return ret;
			};
		}

		auto iterateObjects() {
			return iterateObjectsFiltered(0);
		}

		void bindTES3Util() {
			sol::state& state = LuaManager::getInstance().getState();

			//
			// Extend tes3 library with extra functions.
			//

			// Bind function: tes3.getPlayerRef
			state["tes3"]["getPlayerRef"] = []() -> sol::object {
				TES3::WorldController* worldController = tes3::getWorldController();
				if (worldController) {
					TES3::MobilePlayer* mobilePlayer = worldController->getMobilePlayer();
					if (mobilePlayer) {
						return makeLuaObject(mobilePlayer->reference);
					}
				}
				return sol::nil;
			};

			// Bind function: tes3.getMobilePlayer
			state["tes3"]["getMobilePlayer"] = []() -> sol::object {
				TES3::WorldController* worldController = tes3::getWorldController();
				if (worldController) {
					return makeLuaObject(worldController->getMobilePlayer());
				}
				return sol::nil;
			};

			// Bind function: tes3.getPlayerCell()
			state["tes3"]["getPlayerCell"] = []() -> sol::object {
				TES3::DataHandler* dataHandler = tes3::getDataHandler();
				if (dataHandler) {
					return makeLuaObject(dataHandler->currentCell);
				}
				return sol::nil;
			};

			// Bind function: tes3.getGame
			state["tes3"]["getGame"] = []() {
				return tes3::getGame();
			};

			// Bind function: tes3.getDataHandler
			state["tes3"]["getDataHandler"] = []() {
				return tes3::getDataHandler();
			};

			// Bind function: tes3.getGame
			state["tes3"]["getWorldController"] = []() {
				return tes3::getWorldController();
			};

			// Bind function: tes3.getPlayerTarget
			state["tes3"]["getPlayerTarget"] = []() -> sol::object {
				TES3::Game * game = tes3::getGame();
				if (game) {
					return makeLuaObject(game->playerTarget);
				}
				return sol::nil;
			};

			// Bind function: tes3.getReference
			state["tes3"]["getReference"] = [](sol::optional<std::string> id) {
				if (id) {
					return makeLuaObject(tes3::getReference(id.value()));
				}
				else {
					return makeLuaObject(LuaManager::getInstance().getCurrentReference());
				}
			};

			// Bind function: tes3.getObject
			state["tes3"]["getObject"] = [](std::string& id) -> sol::object {
				TES3::DataHandler * dataHandler = tes3::getDataHandler();
				if (dataHandler) {
					return makeLuaObject(dataHandler->nonDynamicData->resolveObject(id.c_str()));
				}
				return sol::nil;
			};

			state["tes3"]["deleteObject"] = [](sol::object maybe) {
				TES3::BaseObject* object = maybe.as<TES3::BaseObject*>();
				if (object) {
					tes3::getDataHandler()->nonDynamicData->deleteObject(object);
					object->vTable.base->destructor(object, true);
					return true;
				}
				return false;
			};

			// Bind function: tes3.getScript
			state["tes3"]["getScript"] = [](std::string& id) -> sol::object {
				TES3::DataHandler * dataHandler = tes3::getDataHandler();
				if (dataHandler) {
					return makeLuaObject(dataHandler->nonDynamicData->findScriptByName(id.c_str()));
				}
				return sol::nil;
			};

			// Bind function: tes3.getGlobal
			state["tes3"]["getGlobal"] = [](std::string& id) -> sol::object {
				TES3::DataHandler * dataHandler = tes3::getDataHandler();
				if (dataHandler) {
					TES3::GlobalVariable * global = dataHandler->nonDynamicData->findGlobalVariable(id.c_str());
					return sol::make_object(LuaManager::getInstance().getState(), global->value);
				}
				return sol::nil;
			};

			// Bind function: tes3.setGlobal
			state["tes3"]["setGlobal"] = [](std::string& id, double value) {
				TES3::DataHandler * dataHandler = tes3::getDataHandler();
				if (dataHandler) {
					TES3::GlobalVariable * global = dataHandler->nonDynamicData->findGlobalVariable(id.c_str());
					if (global) {
						global->value = value;
						return true;
					}
				}
				return false;
			};

			// Bind function: tes3.findGlobal
			state["tes3"]["findGlobal"] = [](std::string& id) -> sol::object {
				TES3::DataHandler * dataHandler = tes3::getDataHandler();
				if (dataHandler) {
					return makeLuaObject(dataHandler->nonDynamicData->findGlobalVariable(id.c_str()));
				}
				return sol::nil;
			};

			// Bind function: tes3.getGMST
			state["tes3"]["getGMST"] = [](sol::object key) -> sol::object {
				TES3::DataHandler * dataHandler = tes3::getDataHandler();
				if (dataHandler == nullptr) {
					return sol::nil;
				}

				sol::state& state = LuaManager::getInstance().getState();
				if (key.is<double>()) {
					int index = key.as<double>();
					if (index >= TES3::GMST::sMonthMorningstar && index <= TES3::GMST::sWitchhunter) {
						return makeLuaObject(dataHandler->nonDynamicData->GMSTs[index]);
					}
				}
				else if (key.is<std::string>()) {
					int index = -1;
					std::string keyStr = key.as<std::string>();
					for (int i = 0; i <= TES3::GMST::sWitchhunter; i++) {
						TES3::GameSettingInfo* info = tes3::getGMSTInfo(i);
						if (strcmp(info->name, keyStr.c_str()) == 0) {
							index = i;
							break;
						}
					}

					if (index != -1) {
						return makeLuaObject(dataHandler->nonDynamicData->GMSTs[index]);
					}
				}

				return sol::nil;
			};

			// Bind function: tes3.playSound
			state["tes3"]["playSound"] = [](sol::optional<sol::table> params) {
				// Get parameters.
				TES3::Sound* sound = getOptionalParamSound(params, "sound");
				TES3::Reference* reference = getOptionalParamReference(params, "reference");
				bool loop = getOptionalParam<bool>(params, "loop", false);
				int mix = getOptionalParam<int>(params, "mixChannel", int(TES3::AudioMixType::Effects));
				double volume = getOptionalParam<double>(params, "volume", 1.0);
				float pitch = getOptionalParam<double>(params, "pitch", 1.0);

				if (sound == NULL) {
					log::getLog() << "tes3.playSound: Could not locate sound." << std::endl;
					return false;
				}

				// Clamp volume. RIP no std::clamp.
				volume = std::max(0.0, volume);
				volume = std::min(volume, 1.0);

				// Apply mix and rescale to 0-250
				volume *= 250.0 * tes3::getWorldController()->audioController->getMixVolume(TES3::AudioMixType(mix));

				tes3::getDataHandler()->addSound(sound, reference, loop ? TES3::SoundPlayFlags::Loop : 0, volume, pitch);
				return true;
			};

			// Bind function: tes3.getSoundPlaying
			state["tes3"]["getSoundPlaying"] = [](sol::optional<sol::table> params) {
				// Get parameters.
				TES3::Sound* sound = getOptionalParamSound(params, "sound");
				TES3::Reference* reference = getOptionalParamReference(params, "reference");

				if (sound == NULL) {
					log::getLog() << "tes3.getSoundPlaying: Could not locate sound." << std::endl;
					return false;
				}

				return bool(tes3::getDataHandler()->getSoundPlaying(sound, reference));
			};

			// Bind function: tes3.adjustSoundVolume
			state["tes3"]["adjustSoundVolume"] = [](sol::optional<sol::table> params) {
				// Get parameters.
				TES3::Sound* sound = getOptionalParamSound(params, "sound");
				TES3::Reference* reference = getOptionalParamReference(params, "reference");
				int mix = getOptionalParam<int>(params, "mixChannel", int(TES3::AudioMixType::Effects));
				double volume = getOptionalParam<double>(params, "volume", 1.0);

				if (!sound || !reference) {
					log::getLog() << "tes3.adjustSoundVolume: Valid sound and reference required." << std::endl;
					return;
				}

				// Clamp volume.
				volume = std::max(0.0, volume);
				volume = std::min(volume, 1.0);

				// Apply mix and rescale to 0-250
				volume *= 250.0 * tes3::getWorldController()->audioController->getMixVolume(TES3::AudioMixType(mix));

				tes3::getDataHandler()->adjustSoundVolume(sound, reference, volume);
			};
			// Bind function: tes3.removeSound
			state["tes3"]["removeSound"] = [](sol::optional<sol::table> params) {
				// Get parameters.
				TES3::Sound* sound = getOptionalParamSound(params, "sound");
				TES3::Reference* reference = getOptionalParamReference(params, "reference");

				tes3::getDataHandler()->removeSound(sound, reference);
			};

			// Bind function: tes3.streamMusic
			state["tes3"]["streamMusic"] = [](sol::optional<sol::table> params) {
				// Get parameters.
				const char* relativePath = getOptionalParam<const char*>(params, "path", nullptr);
				int situation = getOptionalParam<int>(params, "situation", int(TES3::MusicSituation::Uninterruptible));
				double crossfade = getOptionalParam<double>(params, "crossfade", 1.0);

				if (relativePath) {
					auto w = tes3::getWorldController();
					char path[260];

					std::snprintf(path, sizeof(path), "Data Files/music/%s", relativePath);
					w->audioController->changeMusicTrack(path, 1000 * crossfade, 1.0);
					w->musicSituation = TES3::MusicSituation(situation);
				}
			};

			// Bind function: tes3.messageBox
			state["tes3"]["messageBox"] = [](sol::object param, sol::optional<sol::variadic_args> va) {
				if (param.is<std::string>()) {
					sol::state& state = LuaManager::getInstance().getState();
					std::string message = state["string"]["format"](param, va);
					return tes3::ui::messagePlayer(message.c_str());
				}
				else if (param.is<sol::table>()) {
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
						return tes3::ui::messagePlayer(message.c_str());
					}

					// Set up our event callback.
					LuaManager::getInstance().setButtonPressedCallback(params["callback"]);

					// Temporary hook into the function that creates message boxes. 
					int result = reinterpret_cast<int(__cdecl *)(const char*, ...)>(0x5F1AA0)(message.c_str(), buttonTextStruct, NULL);
					return result;
				}

				return 0;
			};

			// Bind function: tes3.saveGame
			state["tes3"]["saveGame"] = [](sol::optional<sol::table> params) {
				std::string fileName = getOptionalParam<std::string>(params, "file", "quiksave");
				std::string saveName = getOptionalParam<std::string>(params, "name", "Quicksave");

				tes3::getDataHandler()->nonDynamicData->saveGame(fileName.c_str(), saveName.c_str());
			};

			// Bind function: tes3.isModActive
			state["tes3"]["isModActive"] = [](std::string modName) {
				TES3::DataHandler* dataHandler = tes3::getDataHandler();
				if (dataHandler == nullptr) {
					return false;
				}

				for (int i = 0; i < 256; i++) {
					TES3::GameFile* gameFile = dataHandler->nonDynamicData->activeMods[i];
					if (gameFile == nullptr) {
						return false;
					}

					// Compare mod name with this active mod.
					if (_stricmp(gameFile->filename, modName.c_str()) == 0) {
						return true;
					}
				}

				return false;
			};

			// Bind function: tes3.getModList
			state["tes3"]["getModList"] = []() -> sol::object {
				sol::state& state = LuaManager::getInstance().getState();

				TES3::DataHandler* dataHandler = tes3::getDataHandler();
				if (dataHandler == nullptr) {
					return sol::nil;
				}

				sol::table mods = state.create_table();
				for (int i = 0; i < 256; i++) {
					TES3::GameFile* gameFile = dataHandler->nonDynamicData->activeMods[i];
					if (gameFile == nullptr) {
						break;
					}
					mods[i + 1] = static_cast<const char*>(gameFile->filename);
				}

				return mods;
			};

			// Bind function: tes3.playItemPickupSound
			state["tes3"]["playItemPickupSound"] = [](sol::optional<sol::table> params) {
				TES3::Reference* reference = getOptionalParamExecutionReference(params);
				TES3::Item* item = getOptionalParamObject<TES3::Item>(params, "item");
				bool pickup = getOptionalParam<bool>(params, "pickup", true);
				TES3::Sound* sound = getOptionalParamSound(params, "sound");
				if (item == NULL) {
					return;
				}

				tes3::getWorldController()->playItemUpDownSound(item, pickup, reference);
			};

			// Bind function: tes3.iterateList
			state["tes3"]["iterateObjects"] = sol::overload(&iterateObjects, &iterateObjectsFiltered);

			// Bind function: tes3.getSound
			state["tes3"]["getSound"] = [](const char* id) -> TES3::Sound * {
				TES3::DataHandler * dataHandler = tes3::getDataHandler();
				if (dataHandler) {
					return tes3::getDataHandler()->nonDynamicData->findSound(id);
				}
				else {
					sol::state& state = LuaManager::getInstance().getState();
					state["error"]("Function called before Data Handler was initialized.");
					return nullptr;
				}
			};

			// Bind function: tes3.getSoundGenerator
			state["tes3"]["getSoundGenerator"] = [](std::string creatureId, unsigned int type) -> sol::object {
				auto soundGenerators = tes3::getDataHandler()->nonDynamicData->soundGenerators;
				const char* creatureIdCstr = creatureId.c_str();
				for (auto itt = soundGenerators->head; itt != NULL; itt = itt->next) {
					if (itt->data->soundType != static_cast<TES3::SoundType>(type)) {
						continue;
					}

					if (strncmp(creatureIdCstr, itt->data->name, creatureId.length()) == 0) {
						return sol::make_object(LuaManager::getInstance().getState(), itt->data);
					}
				}

				return sol::nil;
			};

			// Bind function: tes3.newGame
			state["tes3"]["newGame"] = []() {
				tes3::startNewGame();
			};

			// Bind function: tes3.getCameraVector
			// This function currently calls out to MGE, which should be changed at some point.
			state["tes3"]["getCameraVector"] = [](sol::optional<sol::table> params) {
				Stack& stack = Stack::getInstance();
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEGetEyeVec);

				// Get the results from the MWSE stack.
				float x = stack.popFloat();
				float y = stack.popFloat();
				float z = stack.popFloat();

				return std::make_shared<TES3::Vector3>(x, y, z);
			};

			// Bind function: tes3.getCameraPosition
			state["tes3"]["getCameraPosition"] = [](sol::optional<sol::table> params) -> TES3::Vector3 * {
				TES3::WorldController * worldController = tes3::getWorldController();
				if (worldController) {
					return &worldController->worldCamera.camera->worldBoundOrigin;
				}
				return nullptr;
			};

			// Bind function: tes3.getCameraPosition
			static NI::Pick* rayTestCache = NULL;
			state["tes3"]["rayTest"] = [](sol::optional<sol::table> params) -> sol::object {
				// Make sure we got our required position.
				TES3::Vector3* position = getOptionalParamVector3(params, "position");
				if (position == NULL) {
					return false;
				}

				// Make sure we got our required direction.
				TES3::Vector3* direction = getOptionalParamVector3(params, "direction");
				if (direction == NULL) {
					return false;
				}

				// Create our pick if it doesn't exist.
				if (rayTestCache == NULL) {
					rayTestCache = NI::Pick::malloc();
				}

				// Or clean up results otherwise.
				else {
					rayTestCache->clearResults();
				}

				// TODO: Allow specifying the root?
				rayTestCache->root = tes3::getGame()->worldRoot;

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

				// Our pick is configured. Let's run it!
				rayTestCache->pickObjects(position, direction);

				// Did we get any results?
				if (rayTestCache->results.filledCount == 0) {
					return sol::nil;
				}

				// Are we looking for a single result?
				else if (rayTestCache->pickType == NI::PickType::FIND_FIRST) {
					return sol::make_object(LuaManager::getInstance().getState(), rayTestCache->results.storage[0]);
				}

				// We're now in multi-result mode. We'll store these in a table.
				sol::state& state = LuaManager::getInstance().getState();
				sol::table results = state.create_table();
				
				// Go through and clone the results in a way that will play nice.
				for (int i = 0; i < rayTestCache->results.filledCount; i++) {
					results[i + 1] = rayTestCache->results.storage[i];
				}

				return results;
			};

			// Bind function: tes3.is3rdPerson
			state["tes3"]["is3rdPerson"] = []() {
				return tes3::getWorldController()->getMobilePlayer()->is3rdPerson();
			};

			// Bind function: tes3.tapKey
			state["tes3"]["tapKey"] = [](double key) {
				Stack::getInstance().pushLong(key);
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGETapKey);
			};

			// Bind function: tes3.pushKey
			state["tes3"]["pushKey"] = [](double key) {
				Stack::getInstance().pushLong(key);
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEPushKey);
			};

			// Bind function: tes3.releaseKey
			state["tes3"]["releaseKey"] = [](double key) {
				Stack::getInstance().pushLong(key);
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEReleaseKey);
			};

			// Bind function: tes3.hammerKey
			state["tes3"]["hammerKey"] = [](double key) {
				Stack::getInstance().pushLong(key);
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEHammerKey);
			};

			// Bind function: tes3.unhammerKey
			state["tes3"]["unhammerKey"] = [](double key) {
				Stack::getInstance().pushLong(key);
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEUnhammerKey);
			};

			// Bind function: tes3.enableKey
			state["tes3"]["enableKey"] = [](double key) {
				Stack::getInstance().pushLong(key);
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEAllowKey);
			};

			// Bind function: tes3.disableKey
			state["tes3"]["disableKey"] = [](double key) {
				Stack::getInstance().pushLong(key);
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEDisallowKey);
			};

			// Bind function: tes3.disableKey
			state["tes3"]["getTopMenu"] = []() {
				return tes3::ui::getTopMenu();
			};

			state["tes3"]["hasCodePatchFeature"] = [](int id) -> sol::object {
				if (!mcp::hasFeaturesFound()) {
					return sol::nil;
				}

				return sol::make_object(LuaManager::getInstance().getState(), mcp::getFeatureEnabled(id));
			};

			state["tes3"]["getDaysInMonth"] = [](int month) -> sol::object {
				TES3::WorldController * worldController = tes3::getWorldController();
				if (worldController) {
					return sol::make_object(LuaManager::getInstance().getState(), worldController->getDaysInMonth(month));
				}
				return sol::nil;
			};

			state["tes3"]["getSimulationTimestamp"] = []() -> sol::object {
				TES3::WorldController * worldController = tes3::getWorldController();
				if (worldController) {
					return sol::make_object(LuaManager::getInstance().getState(), worldController->getHighPrecisionSimulationTimestamp());
				}
				return sol::nil;
			};

			state["tes3"]["getEquippedItem"] = [](sol::table params) -> sol::object {
				sol::state& state = LuaManager::getInstance().getState();

				// Find our equipment based on the object given.
				TES3::Iterator<TES3::EquipmentStack> * equipment = NULL;
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
					state["error"]("No actor provided.");
					return sol::nil;
				}

				// Make sure we got the equipment.
				if (equipment == NULL) {
					state["error"]("The provided actor's equipment could not be resolved.");
					return sol::nil;
				}

				// Get filter: Item Type
				std::tuple<bool, int> filterObjectType(false, INT_MAX);
				std::tuple<bool, int> filterSlot(false, INT_MAX);
				if (params["objectType"].valid()) {
					std::get<0>(filterObjectType) = true;
					std::get<1>(filterObjectType) = (int)params["objectType"];

					// Get filter: Item Slot/Type
					if (params["slot"].valid()) {
						std::get<0>(filterSlot) = true;
						std::get<1>(filterSlot) = (int)params["slot"];
					}
					else if (params["type"].valid()) {
						std::get<0>(filterSlot) = true;
						std::get<1>(filterSlot) = (int)params["type"];
					}
				}

				// Get filter: Item Enchanted
				std::tuple<bool, bool> filterEnchanted(false, false);
				if (params["enchanted"].valid()) {
					std::get<0>(filterEnchanted) = true;
					std::get<1>(filterEnchanted) = params["enchanted"];
				}

				// Loop through the items and pick the first one that matches our filters.
				for (auto itt = equipment->head; itt != NULL; itt = itt->next) {
					TES3::Object * object = itt->data->object;

					// Filter object type.
					if (std::get<0>(filterObjectType) && std::get<1>(filterObjectType) != object->objectType) {
						continue;
					}

					// Filter slot/type.
					if (std::get<0>(filterSlot) && std::get<1>(filterSlot) != object->getType()) {
						continue;
					}

					// Filter enchanted.
					if (std::get<0>(filterEnchanted)) {
						TES3::Enchantment * enchantment = object->getEnchantment();
						if (std::get<1>(filterEnchanted) == true && enchantment == NULL) {
							continue;
						}
						else if (std::get<1>(filterEnchanted) == false && enchantment != NULL) {
							continue;
						}
					}

					// If we got this far we match all filters. Return the object.
					return sol::make_object(state, itt->data);
				}

				return sol::nil;
			};

			state["tes3"]["getInputBinding"] = [](int code) -> TES3::InputConfig* {
				if (code < TES3::KeyBind::FirstKey || code > TES3::KeyBind::LastKey) {
					return NULL;
				}

				TES3::WorldController * worldController = tes3::getWorldController();
				if (worldController) {
					TES3::InputController * inputController = worldController->inputController;
					if (inputController) {
						return &inputController->inputMaps[code];
					}
				}
				return NULL;
			};

			state["tes3"]["getRegion"] = []() -> sol::object {
				TES3::DataHandler * dataHandler = tes3::getDataHandler();
				if (dataHandler && dataHandler->lastExteriorCell) {
					return makeLuaObject(dataHandler->lastExteriorCell->getRegion());
				}

				return sol::nil;
			};

			state["tes3"]["getCurrentWeather"] = []() -> sol::object {
				TES3::WorldController * worldController = tes3::getWorldController();
				if (worldController) {
					return makeLuaObject(worldController->weatherController->currentWeather);
				}

				return sol::nil;
			};

			state["tes3"]["getCursorPosition"] = []() -> sol::object {
				TES3::WorldController * worldController = tes3::getWorldController();
				if (worldController) {
					sol::table results = LuaManager::getInstance().getState().create_table();
					results["x"] = worldController->mouseController->position.x;
					results["y"] = worldController->mouseController->position.z;
					return results;
				}

				return sol::nil;
			};

			state["tes3"]["getSkill"] = [](int skillID) -> sol::object {
				TES3::DataHandler * dataHandler = tes3::getDataHandler();
				if (dataHandler) {
					return makeLuaObject(&dataHandler->nonDynamicData->skills[skillID]);
				}

				return sol::nil;
			};

			state["tes3"]["removeEffects"] = [](sol::table params) {
				sol::state& state = LuaManager::getInstance().getState();
				
				TES3::Reference * reference = getOptionalParamExecutionReference(params);
				if (reference == nullptr) {
					state["error"]("tes3.removeEffects: No reference parameter provided.");
					return;
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
					tes3::getWorldController()->removeSpellsByEffect(reference, effect, chance);
				}
				else if (castType != -1) {
					bool removeSpell = getOptionalParam<bool>(params, "removeSpell", castType != int(TES3::SpellCastType::Spell));
					tes3::getWorldController()->clearSpellEffect(reference, castType, chance, removeSpell);
				}
				else {
					state["error"]("tes3.removeEffects: Must pass either 'effect' or 'castType' parameter!");
				}
			};

			state["tes3"]["getPlayerGold"] = []() -> int {
				auto player = tes3::getWorldController()->getMobilePlayer();
				return player->getGold();
			};

			state["tes3"]["triggerCrime"] = [](sol::table params) {
				sol::state& state = LuaManager::getInstance().getState();

				TES3::CrimeEvent crimeEvent;

				// Look at the given type.
				int crimeType = getOptionalParam<int>(params, "type", 3);
				if (crimeType < 1 || crimeType > 7) {
					state["error"]("Invalid type given. Value must be between 1 and 7.");
					return false;
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
				TES3::MobileActor * criminal = getOptionalParamMobileActor(params, "criminal");
				if (criminal == nullptr) {
					criminal = tes3::getWorldController()->getMobilePlayer();
				}
				crimeEvent.criminal = criminal;

				// Set some basic crime event data.
				crimeEvent.timestamp = timeGetTime();
				crimeEvent.position = criminal->position;
				crimeEvent.penalty = getOptionalParam<int>(params, "value", crimeEvent.penalty);

				// Victim can be more complicated.
				sol::object victim = params["victim"];
				crimeEvent.victim = tes3::getWorldController()->getMobilePlayer();
				if (victim.is<TES3::Faction>()) {
					crimeEvent.victimFaction = victim.as<TES3::Faction*>();
				}
				else if (victim.is<TES3::Actor>()) {
					crimeEvent.victim = tes3::getWorldController()->getMobilePlayer();
					crimeEvent.victimFaction = victim.as<TES3::Actor*>()->getFaction();
					if (victim.is<TES3::NPC>()) {
						crimeEvent.victimBaseActor = victim.as<TES3::NPC*>();
					}
					else if (victim.is<TES3::NPCInstance>()) {
						crimeEvent.victimBaseActor = victim.as<TES3::NPCInstance*>()->baseNPC;
					}
				}
				else if (victim.is<TES3::MobileNPC>()) {
					TES3::MobileNPC * mach = victim.as<TES3::MobileNPC*>();
					crimeEvent.victim = mach;
					crimeEvent.victimFaction = mach->npcInstance->getFaction();
					crimeEvent.victimBaseActor = mach->npcInstance;
				}

				// Do detection and the like.
				bool forceDetection = getOptionalParam<bool>(params, "forceDetection", false);
				auto controller = tes3::getWorldController()->mobController->unknown_0x24;
				if (!forceDetection && controller->detectPresence(crimeEvent.criminal)) {
					controller->checkRadius(crimeEvent.victim, crimeEvent.unknown_0x34);
				}

				// If we were detected, add it to the list.
				if (forceDetection || crimeEvent.unknown_0x34->size) {
					auto v167 = crimeEvent.victim->unknown_0x1B0;
					auto v168 = v167->b;
					auto v169 = tes3::_new<TES3::CrimeTree>();
					auto v170 = v167;
					if (!v167) {
						v170 = v169;
					}
					v169->a = v170;
					auto v171 = v168;
					if (!v168) {
						v171 = v169;
					}
					v169->b = v171;
					v167->b = v169;
					v169->b->a = v169;
					auto v172 = &v169->data;
					if (v172) {
						v172->copy(&crimeEvent);
					}
					crimeEvent.victim->unknown_0x1B4++;
				}

				return true;
			};

			state["tes3"]["loadMesh"] = [](const char* relativePath) -> sol::object {
				std::string path = "Data Files\\Meshes\\";
				path += relativePath;

				NI::Stream stream;
				if (!stream.load(path.c_str())) {
					return sol::nil;
				}

				return makeLuaObject(*stream.loadedObject);
			};
		}
	}
}
