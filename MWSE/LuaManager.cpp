#include "LuaManager.h"

#include "mwOffsets.h"
#include "Log.h"
#include "TES3Util.h"
#include "MemoryUtil.h"
#include "ScriptUtil.h"
#include "UIUtil.h"
#include "MWSEDefs.h"
#include "BuildDate.h"

#include "sol.hpp"

#include "LuaTimer.h"

#include "LuaScript.h"

#include "TES3Defines.h"
#include "TES3Actor.h"
#include "TES3ActorAnimationData.h"
#include "TES3Alchemy.h"
#include "TES3Book.h"
#include "TES3CombatSession.h"
#include "TES3Creature.h"
#include "TES3DataHandler.h"
#include "TES3Dialogue.h"
#include "TES3DialogueInfo.h"
#include "TES3Fader.h"
#include "TES3Game.h"
#include "TES3GameFile.h"
#include "TES3GameSetting.h"
#include "TES3InputController.h"
#include "TES3ItemData.h"
#include "TES3LeveledList.h"
#include "TES3MagicEffect.h"
#include "TES3MagicEffectInstance.h"
#include "TES3Misc.h"
#include "TES3MobController.h"
#include "TES3MobileActor.h"
#include "TES3MobileCreature.h"
#include "TES3MobilePlayer.h"
#include "TES3MobileProjectile.h"
#include "TES3Reference.h"
#include "TES3SoulGemData.h"
#include "TES3Spell.h"
#include "TES3UIElement.h"
#include "TES3UIInventoryTile.h"
#include "TES3UIManager.h"
#include "TES3WorldController.h"

// Lua binding files. These are split out rather than kept here to help with compile times.
#include "StackLua.h"
#include "ScriptUtilLua.h"
#include "StringUtilLua.h"
#include "TES3UtilLua.h"
#include "TES3ActionDataLua.h"
#include "TES3ActivatorLua.h"
#include "TES3AILua.h"
#include "TES3AlchemyLua.h"
#include "TES3ApparatusLua.h"
#include "TES3ArmorLua.h"
#include "TES3AttachmentLua.h"
#include "TES3BodyPartLua.h"
#include "TES3BookLua.h"
#include "TES3CellLua.h"
#include "TES3ClassLua.h"
#include "TES3ClothingLua.h"
#include "TES3CollectionsLua.h"
#include "TES3CombatSessionLua.h"
#include "TES3ContainerLua.h"
#include "TES3CreatureLua.h"
#include "TES3DataHandlerLua.h"
#include "TES3DialogueLua.h"
#include "TES3DoorLua.h"
#include "TES3EnchantmentLua.h"
#include "TES3FactionLua.h"
#include "TES3FaderLua.h"
#include "TES3GameLua.h"
#include "TES3GameFileLua.h"
#include "TES3GameSettingLua.h"
#include "TES3GlobalVariableLua.h"
#include "TES3IngredientLua.h"
#include "TES3InputControllerLua.h"
#include "TES3InventoryLua.h"
#include "TES3LeveledListLua.h"
#include "TES3LightLua.h"
#include "TES3LockpickLua.h"
#include "TES3MagicEffectLua.h"
#include "TES3MiscLua.h"
#include "TES3MobileActorLua.h"
#include "TES3MobileCreatureLua.h"
#include "TES3MobileNPCLua.h"
#include "TES3MobilePlayerLua.h"
#include "TES3MobileProjectileLua.h"
#include "TES3MoonLua.h"
#include "TES3NPCLua.h"
#include "TES3ProbeLua.h"
#include "TES3RaceLua.h"
#include "TES3ReferenceLua.h"
#include "TES3ReferenceListLua.h"
#include "TES3RegionLua.h"
#include "TES3RepairToolLua.h"
#include "TES3ScriptLua.h"
#include "TES3SkillLua.h"
#include "TES3SoundLua.h"
#include "TES3SpellLua.h"
#include "TES3SpellListLua.h"
#include "TES3MagicEffectInstanceLua.h"
#include "TES3MagicSourceInstanceLua.h"
#include "TES3StaticLua.h"
#include "TES3StatisticLua.h"
#include "TES3UIElementLua.h"
#include "TES3UIInventoryTileLua.h"
#include "TES3UIManagerLua.h"
#include "TES3UIMenuController.h"
#include "TES3UIWidgetsLua.h"
#include "TES3VectorsLua.h"
#include "TES3WeaponLua.h"
#include "TES3WeatherControllerLua.h"
#include "TES3WeatherLua.h"
#include "TES3WorldControllerLua.h"

#include "NICameraLua.h"
#include "NINodeLua.h"
#include "NIObjectLua.h"
#include "NIPickLua.h"
#include "NIPixelDataLua.h"
#include "NISourceTextureLua.h"
#include "NISwitchNodeLua.h"
#include "NITimeControllerLua.h"
#include "NITriShapeLua.h"

#include "LuaActivationTargetChangedEvent.h"
#include "LuaAddTopicEvent.h"
#include "LuaAttackEvent.h"
#include "LuaCalcBarterPriceEvent.h"
#include "LuaCalcHitChanceEvent.h"
#include "LuaCalcRepairPriceEvent.h"
#include "LuaCalcRestInterruptEvent.h"
#include "LuaCalcSoulValueEvent.h"
#include "LuaCalcSpellPriceEvent.h"
#include "LuaCalcTrainingPriceEvent.h"
#include "LuaCalcTravelPriceEvent.h"
#include "LuaCellChangedEvent.h"
#include "LuaEquipEvent.h"
#include "LuaFilterBarterMenuEvent.h"
#include "LuaFilterContentsMenuEvent.h"
#include "LuaFilterInventoryEvent.h"
#include "LuaFilterInventorySelectEvent.h"
#include "LuaFilterSoulGemTargetEvent.h"
#include "LuaFrameEvent.h"
#include "LuaGenericUiActivatedEvent.h"
#include "LuaGenericUiPostEvent.h"
#include "LuaGenericUiPreEvent.h"
#include "LuaInfoGetTextEvent.h"
#include "LuaInfoResponseEvent.h"
#include "LuaItemDroppedEvent.h"
#include "LuaItemTileUpdatedEvent.h"
#include "LuaKeyDownEvent.h"
#include "LuaKeyUpEvent.h"
#include "LuaLevelUpEvent.h"
#include "LuaLoadedGameEvent.h"
#include "LuaMagicCastedEvent.h"
#include "LuaMenuStateEvent.h"
#include "LuaMouseAxisEvent.h"
#include "LuaMouseButtonDownEvent.h"
#include "LuaMouseButtonUpEvent.h"
#include "LuaMouseWheelEvent.h"
#include "LuaMusicSelectTrackEvent.h"
#include "LuaPotionBrewedEvent.h"
#include "LuaProjectileExpireEvent.h"
#include "LuaRestInterruptEvent.h"
#include "LuaSimulateEvent.h"
#include "LuaSkillRaisedEvent.h"
#include "LuaSpellCastedEvent.h"
#include "LuaSpellResistEvent.h"
#include "LuaSpellTickEvent.h"
#include "LuaUiObjectTooltipEvent.h"
#include "LuaUiRefreshedEvent.h"
#include "LuaUiSpellTooltipEvent.h"
#include "LuaWeaponReadiedEvent.h"
#include "LuaWeaponUnreadiedEvent.h"
#include "LuaWeatherChangedImmediateEvent.h"
#include "LuaWeatherCycledEvent.h"
#include "LuaWeatherTransitionFinishedEvent.h"
#include "LuaWeatherTransitionStartedEvent.h"

#include "windows.h"
#include "psapi.h"

#include <filesystem>
#include <unordered_map>

#define TES3_HOOK_RUNSCRIPT_LUACHECK 0x5029A4
#define TES3_HOOK_RUNSCRIPT_LUACHECK_SIZE 0x6
#define TES3_HOOK_RUNSCRIPT_LUACHECK_RETURN (TES3_HOOK_RUNSCRIPT_LUACHECK + TES3_HOOK_RUNSCRIPT_LUACHECK_SIZE)

#define TES3_HOOK_SAVE_REFERENCE 0x4E1C95
#define TES3_HOOK_SAVE_REFERENCE_SIZE 0x6
#define TES3_HOOK_SAVE_REFERENCE_RETURN (TES3_HOOK_SAVE_REFERENCE + TES3_HOOK_SAVE_REFERENCE_SIZE)

#define TES3_HOOK_LOAD_REFERENCE 0x4DE532
#define TES3_HOOK_LOAD_REFERENCE_SIZE 0x5
#define TES3_HOOK_LOAD_REFERENCE_RETURN (TES3_HOOK_LOAD_REFERENCE + TES3_HOOK_LOAD_REFERENCE_SIZE)
#define TES3_HOOK_LOAD_REFERENCE_RETURN_SUCCESS 0x4DE406

#define TES3_HOOK_FINISH_INITIALIZATION 0x4BBC0C
#define TES3_HOOK_FINISH_INITIALIZATION_SIZE 0x5
#define TES3_HOOK_FINISH_INITIALIZATION_RETURN (TES3_HOOK_FINISH_INITIALIZATION + TES3_HOOK_FINISH_INITIALIZATION_SIZE)

#define TES3_HOOK_UI_EVENT 0x58371A
#define TES3_HOOK_UI_EVENT_SIZE 0x5
#define TES3_HOOK_UI_EVENT_RETURN (TES3_HOOK_UI_EVENT + TES3_HOOK_UI_EVENT_SIZE)

#define TES3_HOOK_MAGIC_CAST_SUCCESS 0x5157E6
#define TES3_HOOK_MAGIC_CAST_SUCCESS_SIZE 0xA
#define TES3_HOOK_MAGIC_CAST_SUCCESS_RETURN (TES3_HOOK_MAGIC_CAST_SUCCESS + TES3_HOOK_MAGIC_CAST_SUCCESS_SIZE)

#define TES3_HOOK_SPELL_CAST_FAILURE 0x5157D5
#define TES3_HOOK_SPELL_CAST_FAILURE_SIZE 0xA
#define TES3_HOOK_SPELL_CAST_FAILURE_RETURN (TES3_HOOK_SPELL_CAST_FAILURE + TES3_HOOK_SPELL_CAST_FAILURE_SIZE)

#define TES3_ActorAnimData_attackCheckMeleeHit 0x541530

#define TES3_BaseObject_destructor 0x4F0CA0

namespace mwse {
	namespace lua {
		// Initialize singleton.
		LuaManager LuaManager::singleton;

		// Fast-access mapping from a TES3::Script* to its associated Lua module.
		static std::unordered_map<unsigned long, sol::object> scriptOverrides;

		// The currently executing overwritten script.
		static LuaScript * currentOverwrittenScript = NULL;

		// We still abort the program if an unprotected lua error happens. Here we at least
		// get it in the log so it can be debugged.
		int panic(lua_State* L) {
			const char* message = lua_tostring(L, -1);
			log::getLog() << (message ? message : "An unexpected error occurred and forced the lua state to call atpanic.") << std::endl;
			return 0;
		}

		int exceptionHandler(lua_State* L, sol::optional<const std::exception&> maybe_exception, sol::string_view description) {
			log::getLog() << "An unhandled exception occurred: ";
			if (maybe_exception) {
				const std::exception& ex = *maybe_exception;
				log::getLog() << ex.what() << std::endl;
			}
			else {
				log::getLog().write(description.data(), description.size());
				log::getLog() << std::endl;
			}

			logStackTrace();

			return sol::stack::push(L, description);
		}

		// LuaManager constructor. This is private, as a singleton.
		LuaManager::LuaManager() {
			// Open default lua libraries.
			luaState.open_libraries();

			// Override the default atpanic to print to the log.
			luaState.set_panic(&panic);
			luaState.set_exception_handler(&exceptionHandler);

			// Set up our timers.
			gameTimers = std::make_shared<TimerController>();
			simulateTimers = std::make_shared<TimerController>();
			realTimers = std::make_shared<TimerController>();


			// Overwrite the default print function to print to the MWSE log.
			luaState["print"] = [](sol::object message) {
				sol::state& state = LuaManager::getInstance().getState();
				std::string result = state["tostring"](message);
				log::getLog() << result << std::endl;
			};

			// Bind our data types.
			bindData();
		}

		void LuaManager::bindData() {
			// Bind our LuaScript type, which is used for holding script contexts.
			luaState.new_usertype<LuaScript>("LuaScript",
				sol::constructors<LuaScript()>(),

				// Implement dynamic object metafunctions.
				sol::meta_function::index, &DynamicLuaObject::dynamic_get,
				sol::meta_function::new_index, &DynamicLuaObject::dynamic_set,
				sol::meta_function::length, [](DynamicLuaObject& d) { return d.entries.size(); },

				// Set up read-only properties.
				"script", sol::readonly(&LuaScript::script),
				"reference", sol::readonly(&LuaScript::reference),
				"context", sol::readonly_property([](LuaScript& self) { return std::shared_ptr<ScriptContext>(new ScriptContext(self.script)); })

				);

			// Create the base of API tables.
			luaState["mwse"] = createTable();
			luaState["mwscript"] = createTable();
			luaState["mge"] = createTable();

			// Expose timers.
			bindLuaTimer();
			luaState["mwse"]["realTimers"] = realTimers;
			luaState["mwse"]["simulateTimers"] = simulateTimers;
			luaState["mwse"]["gameTimers"] = gameTimers;

			luaState["mwse"]["getVersion"] = []() {
				return MWSE_VERSION_INTEGER;
			};

			luaState["mwse"]["version"] = MWSE_VERSION_INTEGER;
			luaState["mwse"]["buildDate"] = MWSE_BUILD_DATE;

			// We want to take care of this here rather than in an external file so we have access to scriptOverrides.
			luaState["mwse"]["overrideScript"] = [](const char* scriptId, sol::object target) {
				auto dataHandler = TES3::DataHandler::get();
				if (dataHandler == nullptr) {
					mwse::log::getLog() << "WARNING: mwse.overrideScript called before game data is initialized." << std::endl;
					return false;
				}

				TES3::Script* script = dataHandler->nonDynamicData->findScriptByName(scriptId);
				if (script == nullptr) {
					return false;
				}

				if (target.is<sol::function>()) {
					scriptOverrides[(unsigned long)script] = target;
					script->dataLength = 0;
					return true;
				}
				else if (target.is<std::string>()) {
					sol::state& state = LuaManager::getInstance().getState();
					sol::object result = state.safe_script_file("./Data Files/MWSE/mods/" + target.as<std::string>() + ".lua");
					if (result.get_type() == sol::type::table) {
						scriptOverrides[(unsigned long)script] = result;
						script->dataLength = 0;
						return true;
					}
				}

				return false;
			};

			luaState["mwse"]["virtualKeyPressed"] = [](int VK_key) {
				return (GetAsyncKeyState(VK_key) & 0x8000) == 0x8000;
			};

			luaState["mwse"]["getVirtualMemoryUsage"] = []() {
				PROCESS_MEMORY_COUNTERS_EX memCounter;
				GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&memCounter, sizeof(memCounter));
				return memCounter.PrivateUsage;
			};

			// Bind TES3 data types.
			bindTES3ActionData();
			bindTES3Activator();
			bindTES3AI();
			bindTES3Alchemy();
			bindTES3Apparatus();
			bindTES3Armor();
			bindTES3Attachment();
			bindTES3BodyPart();
			bindTES3Book();
			bindTES3Cell();
			bindTES3Class();
			bindTES3Clothing();
			bindTES3Collections();
			bindTES3CombatSession();
			bindTES3Container();
			bindTES3Creature();
			bindTES3DataHandler();
			bindTES3Dialogue();
			bindTES3Door();
			bindTES3Enchantment();
			bindTES3Faction();
			bindTES3Fader();
			bindTES3Game();
			bindTES3GameFile();
			bindTES3GameSetting();
			bindTES3GlobalVariable();
			bindTES3Ingredient();
			bindTES3InputController();
			bindTES3Inventory();
			bindTES3LeveledList();
			bindTES3Light();
			bindTES3Lockpick();
			bindTES3MagicEffect();
			bindTES3MagicEffectInstance();
			bindTES3MagicSourceInstance();
			bindTES3Misc();
			bindTES3MobileActor();
			bindTES3MobileCreature();
			bindTES3MobileNPC();
			bindTES3MobilePlayer();
			bindTES3MobileProjectile();
			bindTES3Moon();
			bindTES3NPC();
			bindTES3Probe();
			bindTES3Race();
			bindTES3Reference();
			bindTES3ReferenceList();
			bindTES3Region();
			bindTES3RepairTool();
			bindTES3Script();
			bindTES3Skill();
			bindTES3Sound();
			bindTES3Spell();
			bindTES3SpellList();
			bindTES3Static();
			bindTES3Statistic();
			bindTES3Vectors();
			bindTES3Weapon();
			bindTES3Weather();
			bindTES3WeatherController();
			bindTES3WorldController();

			bindTES3UIElement();
			bindTES3UIInventoryTile();
			bindTES3UIManager();
			bindTES3UIWidgets();

			// Bind NI data types.
			bindNICamera();
			bindNINode();
			bindNIObject();
			bindNIPick();
			bindNIPixelData();
			bindNISourceTexture();
			bindNISwitchNode();
			bindNITimeController();
			bindNITriShape();
		}

		//
		// Hook: Run script.
		//

		// Determines if a script should be overriden, and executes the module::execute function if so.
		static void _stdcall RunScript(TES3::Script* script) {
			// Determine if we own this script.
			auto searchResult = scriptOverrides.find((unsigned long)script);
			if (searchResult == scriptOverrides.end()) {
				return;
			}

			// Update the LuaManager to reference our current context.
			lua::LuaManager& manager = lua::LuaManager::getInstance();
			TES3::Reference * reference = *reinterpret_cast<TES3::Reference**>(TES3_SCRIPTTARGETREF_IMAGE);
			manager.setCurrentReference(reference);
			manager.setCurrentScript(script);

			// Get and run the execute function.
			sol::state& state = manager.getState();
			sol::protected_function execute;
			if (searchResult->second.is<sol::function>()) {
				execute = searchResult->second.as<sol::function>();
			}
			else if (searchResult->second.get_type() == sol::type::table) {
				execute = searchResult->second.as<sol::table>()["execute"];
			}

			if (execute) {
				sol::table params = LuaManager::getInstance().createTable();
				params["script"] = makeLuaObject(script);
				params["reference"] = makeLuaObject(reference);
				params["scriptData"] = mwse::mwscript::getLocalScriptVariables();

				sol::protected_function_result result = execute(params);
				if (!result.valid()) {
					sol::error error = result;
					log::getLog() << "Lua error encountered when override of script '" << script->name << "':" << std::endl << error.what() << std::endl;
					mwscript::StopScript(script, script);
				}
			}
			else {
				log::getLog() << "No execute function found for script override of '" << script->name << "'. Script execution stopped." << std::endl;
				mwscript::StopScript(script, script);
			}
		}

		// Hook for HookRunScriptIndirect.
		static DWORD callbackRunScript = TES3_HOOK_RUNSCRIPT_LUACHECK_RETURN;
		static __declspec(naked) void HookRunScript() {
			_asm
			{
				// Save all registers.
				pushad

				// Actually use our hook.
				push ebx
				call RunScript

				// Restore all registers.
				popad

				// Overwritten code.
				mov ecx, dword ptr ds : [TES3_SCRIPTTARGETREF_IMAGE]

				// Resume normal execution.
				jmp callbackRunScript
			}
		}

		//
		// Hook: Player container (re)created.
		//

		void __fastcall OnPlayerRecreated(TES3::MobController_0x24 * self) {
			// Call overwritten function.
			reinterpret_cast<void(__thiscall *)(TES3::MobController_0x24*)>(0x56EAE0)(self);

			// Grab the new player pointers for lua.
			// Update tes3.player and tes3.mobilePlayer.
			sol::state& state = LuaManager::getInstance().getState();
			TES3::MobilePlayer * mobilePlayer = self->mobilePlayer;
			state["tes3"]["mobilePlayer"] = mwse::lua::makeLuaObject(mobilePlayer);
			state["tes3"]["player"] = mwse::lua::makeLuaObject(mobilePlayer->reference);
		}

		//
		// Hook: Finished initializing game code.
		//

		static void _stdcall FinishInitialization() {
			// Hook up shorthand access to data handler, world controller, and game.
			sol::state& state = LuaManager::getInstance().getState();
			state["tes3"]["dataHandler"] = TES3::DataHandler::get();
			state["tes3"]["worldController"] = TES3::WorldController::get();
			state["tes3"]["game"] = TES3::Game::get();

			LuaManager::getInstance().triggerEvent(new event::GenericEvent("initialized"));
		}

		static DWORD callbackFinishedInitialization = TES3_HOOK_FINISH_INITIALIZATION_RETURN;
		static __declspec(naked) void HookFinishInitialization() {
			_asm
			{
				// Save the registers.
				pushad

				// Actually use our hook.
				call FinishInitialization

				// Resume normal execution.
				popad
				mov eax, 1
				jmp callbackFinishedInitialization
			}
		}

		//
		// Hook: Enter Frame
		//

		TES3::Cell* lastCell = NULL;
		bool lastMenuMode = true;
		void __fastcall EnterFrame(TES3::WorldController* worldController, DWORD _UNUSED_) {
			// Run the function before raising our event.
			worldController->mainLoopBeforeInput();

			// Fire off any button pressed events if we had one queued.
			LuaManager& luaManager = LuaManager::getInstance();
			if (tes3::ui::getButtonPressedIndex() != -1) {
				luaManager.triggerButtonPressed();
			}
			
			// Update timer controllers.
			double highResolutionTimestamp = worldController->getHighPrecisionSimulationTimestamp();
			luaManager.updateTimers(worldController->deltaTime, highResolutionTimestamp, worldController->flagMenuMode == 0);

			// Has menu mode changed?
			if (worldController->flagMenuMode != lastMenuMode) {
				luaManager.triggerEvent(new event::MenuStateEvent(worldController->flagMenuMode));
				lastMenuMode = worldController->flagMenuMode;
			}

			// Has our cell changed?
			TES3::DataHandler * dataHandler = TES3::DataHandler::get();
			if (dataHandler->cellChanged) {
				LuaManager::getInstance().triggerEvent(new event::CellChangedEvent(dataHandler->currentCell, lastCell));
				lastCell = dataHandler->currentCell;
			}

			// Send off our enterFrame event always.
			luaManager.triggerEvent(new event::FrameEvent(worldController->deltaTime, worldController->flagMenuMode));

			// If we're not in menu mode, send off the simulate event.
			if (!worldController->flagMenuMode) {
				luaManager.triggerEvent(new event::SimulateEvent(worldController->deltaTime, highResolutionTimestamp));
			}
		}

		//
		// Hook: On PC Equip
		//

		signed char __cdecl OnPCEquip(TES3::UI::InventoryTile* tile) {
			// Execute event. If the event blocked the call, bail.
			sol::object response = LuaManager::getInstance().triggerEvent(new event::EquipEvent(TES3::WorldController::get()->getMobilePlayer()->reference, tile->item, tile->itemData));
			if (response.get_type() == sol::type::table) {
				sol::table eventData = response;
				if (eventData["block"] == true) {
					// If we want to block it, we need to run some functions to clear the held item back to the inventory.
					TES3::UI::Element* inventoryMenu = tes3::ui::getMenuNode(tes3::ui::getInventoryMenuId());
					inventoryMenu->timingUpdate();
					tes3::ui::inventoryAddTile(1, tile);
					inventoryMenu->performLayout(1);
					tes3::ui::flagPaperDollUpdate();
					tes3::ui::inventoryUpdateIcons();
					return 0;
				}
			}

			// Call the original function.
			return tes3::ui::equipInventoryTile(tile);
		}

		//
		// Hook: On Equipped.
		//

		TES3::Object* __fastcall OnEquipped(TES3::Actor* actor, DWORD _UNUSED_, TES3::Object* item, TES3::ItemData* itemData, TES3::EquipmentStack** out_equipmentStack, TES3::MobileActor* mobileActor) {
			// Call our wrapper for the function so that events are triggered.
			return actor->equipItem(item, itemData, out_equipmentStack, mobileActor);
		}

		//
		// Hook: On Unequipped.
		//

		TES3::EquipmentStack* __fastcall OnUnequipped(TES3::Actor* actor, DWORD _UNUSED_, TES3::Object* item, bool deleteStack, TES3::MobileActor* mobileActor, bool updateGUI, TES3::ItemData* itemData) {
			// Call our wrapper for the function so that events are triggered.
			return actor->unequipItem(item, deleteStack, mobileActor, updateGUI, itemData);
		}

		//
		// Hook: On Activate
		//

		void __fastcall OnActivate(TES3::Reference* target, DWORD _UNUSED_, TES3::Reference* activator, int something) {
			// Call our wrapper for the function so that events are triggered.
			target->activate(activator, something);
		}

		//
		// Hook: On Save
		//

		signed char __fastcall OnSave(TES3::NonDynamicData* nonDynamicData, DWORD _UNUSED_, const char* fileName, const char* saveName) {
			// Call our wrapper for the function so that events are triggered.
			return nonDynamicData->saveGame(fileName, saveName);
		}

		//
		// Hook: On Load
		//

		bool __fastcall OnLoad(TES3::NonDynamicData* nonDynamicData, DWORD _UNUSED_, const char* fileName) {
			// Call our wrapper for the function so that events are triggered.
			TES3::LoadGameResult loaded = nonDynamicData->loadGame(fileName);

			// Extra things we want to do if we're successfully loading.
			if (loaded == TES3::LoadGameResult::Success) {
				TES3::DataHandler * dataHandler = TES3::DataHandler::get();
				LuaManager::getInstance().triggerEvent(new event::CellChangedEvent(dataHandler->currentCell, NULL));
				lastCell = dataHandler->currentCell;
			}

			return loaded != TES3::LoadGameResult::Failure;
		}

		bool __fastcall OnLoadMainMenu(TES3::NonDynamicData* nonDynamicData, DWORD _UNUSED_, const char* fileName) {
			// Call our wrapper for the function so that events are triggered.
			TES3::LoadGameResult loaded = nonDynamicData->loadGameMainMenu(fileName);

			// Fire off a cell changed event as well, and update the cached last cell.
			if (loaded == TES3::LoadGameResult::Success) {
				TES3::DataHandler * dataHandler = TES3::DataHandler::get();
				LuaManager::getInstance().triggerEvent(new event::CellChangedEvent(dataHandler->currentCell, NULL));
				lastCell = dataHandler->currentCell;
			}

			return loaded != TES3::LoadGameResult::Failure;
		}

		//
		// Hook: New Game. Points to the load event.
		//

		void OnNewGame() {
			tes3::startNewGame();
		}

		void __fastcall OnNewGameViaStartingCell(TES3::MobController * mobController) {
			// Call overwritten code.
			mobController->checkPlayerDistance();

			// Fire off the loaded/cellChanged events.
			LuaManager& luaManager = LuaManager::getInstance();
			lastCell = TES3::DataHandler::get()->currentCell;
			luaManager.triggerEvent(new event::LoadedGameEvent(nullptr, false, true));
			luaManager.triggerEvent(new event::CellChangedEvent(lastCell, nullptr));
		}

		//
		// Hook: Start Combat
		//

		void __fastcall OnStartCombat(TES3::MobileActor* mobileActor, DWORD _UNUSED_, TES3::MobileActor* target) {
			// Call our wrapper for the function so that events are triggered.
			mobileActor->startCombat(target);
		}

		//
		// Hook: Stop Combat
		//

		void __fastcall OnStopCombat(TES3::MobileActor* mobileActor, DWORD _UNUSED_, bool something) {
			// Call our wrapper for the function so that events are triggered.
			mobileActor->stopCombat(something);
		}

		//
		// Hook: Attack
		//

		void __fastcall OnAttack(TES3::ActorAnimationData* animData) {
			// Call original function.
			reinterpret_cast<void(__thiscall *)(TES3::ActorAnimationData*)>(TES3_ActorAnimData_attackCheckMeleeHit)(animData);

			// Prepare our event data.
			LuaManager::getInstance().triggerEvent(new event::AttackEvent(animData));
		}

		//
		// Collision events: Mobile Object
		//

		char __fastcall OnMobileObjectActorCollision(TES3::MobileObject* object, DWORD _UNUSED_, int referenceIndex) {
			return object->onActorCollision(referenceIndex);
		}

		char __fastcall OnMobileObjectObjectCollision(TES3::MobileObject* object, DWORD _UNUSED_, int referenceIndex, signed char flag) {
			return object->onObjectCollision(referenceIndex, flag);
		}

		char __fastcall OnMobileObjectTerrainCollision(TES3::MobileObject* object, DWORD _UNUSED_, int referenceIndex) {
			return object->onTerrainCollision(referenceIndex);
		}

		char __fastcall OnMobileObjectWaterCollision(TES3::MobileObject* object, DWORD _UNUSED_, int referenceIndex) {
			return object->onWaterCollision(referenceIndex);
		}

		char __fastcall OnMobileObjectActivatorCollision(TES3::MobileObject* object, DWORD _UNUSED_, int referenceIndex) {
			return object->onObjectCollision(referenceIndex, 0);
		}

		//
		// Collision events: Mobile Actor
		//

		char __fastcall OnMobileActorActorCollision(TES3::MobileActor* object, DWORD _UNUSED_, int referenceIndex) {
			return object->onActorCollision(referenceIndex);
		}

		char __fastcall OnMobileActorObjectCollision(TES3::MobileActor* object, DWORD _UNUSED_, int referenceIndex, signed char flag) {
			return object->onObjectCollision(referenceIndex, flag);
		}

		char __fastcall OnMobileActorTerrainCollision(TES3::MobileActor* object, DWORD _UNUSED_, int referenceIndex) {
			return object->onTerrainCollision(referenceIndex);
		}

		char __fastcall OnMobileActorActivatorCollision(TES3::MobileActor* object, DWORD _UNUSED_, int referenceIndex) {
			return object->onObjectCollision(referenceIndex, 0);
		}

		//
		// Collision events: Mobile Projectile
		//

		char __fastcall OnMobileProjectileActorCollision(TES3::MobileProjectile* object, DWORD _UNUSED_, int referenceIndex) {
			return object->onActorCollision(referenceIndex);
		}

		char __fastcall OnMobileProjectileObjectCollision(TES3::MobileProjectile* object, DWORD _UNUSED_, int referenceIndex, signed char flag) {
			return object->onObjectCollision(referenceIndex, flag);
		}

		char __fastcall OnMobileProjectileTerrainCollision(TES3::MobileProjectile* object, DWORD _UNUSED_, int referenceIndex) {
			return object->onTerrainCollision(referenceIndex);
		}

		char __fastcall OnMobileProjectileWaterCollision(TES3::MobileProjectile* object, DWORD _UNUSED_, int referenceIndex) {
			return object->onWaterCollision(referenceIndex);
		}

		//
		// UI event hooking.
		//

		signed char __cdecl OnUIEvent(DWORD function, TES3::UI::Element* parent, DWORD prop, DWORD b, DWORD c, TES3::UI::Element* source) {
			// Execute event. If the event blocked the call, bail.
			mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
			sol::table eventData = luaManager.triggerEvent(new event::GenericUiPreEvent(parent, source, prop, b, c));
			if (eventData.valid() && eventData["block"] == true) {
				return 0;
			}

			signed char result = reinterpret_cast<signed char (__cdecl *)(TES3::UI::Element*, DWORD, DWORD, DWORD, TES3::UI::Element*)>(function)(parent, prop, b, c, source);

			luaManager.triggerEvent(new event::GenericUiPostEvent(parent, source, prop, b, c));

			return result;
		}

		static DWORD callbackUIEvent = TES3_HOOK_UI_EVENT_RETURN;
		static __declspec(naked) void HookUIEvent() {
			_asm
			{
				push edi
				call OnUIEvent
				add esp, 0x18
				jmp callbackUIEvent
			}
		}

		//
		// Hook show rest attempt.
		//

		void __cdecl OnShowRestWaitMenu(signed char allowRest) {
			tes3::ui::showRestWaitMenu(allowRest, false);
		}

		void __cdecl OnShowRestWaitMenuScripted(signed char allowRest) {
			tes3::ui::showRestWaitMenu(allowRest, true);
		}

		//
		// Hook show rest attempt.
		//

		void __fastcall OnKeyReadState(TES3::InputController* inputController) {
			// Call the original function.
			inputController->readKeyState();

			// We only need to check the modifier key values once.
			bool controlDown = (inputController->keyboardState[DIK_LCONTROL] & 0x80) || (inputController->keyboardState[DIK_RCONTROL] & 0x80);
			bool shiftDown = (inputController->keyboardState[DIK_LSHIFT] & 0x80) || (inputController->keyboardState[DIK_RSHIFT] & 0x80);
			bool altDown = (inputController->keyboardState[DIK_LALT] & 0x80) || (inputController->keyboardState[DIK_RALT] & 0x80);
			bool superDown = (inputController->keyboardState[DIK_LWIN] & 0x80) || (inputController->keyboardState[DIK_RWIN] & 0x80);

			// Go through the keys to see if any of the states have changed, and launch an event based on that.
			LuaManager& luaManager = LuaManager::getInstance();
			for (size_t i = 0; i < 256; i++) {
				if (inputController->isKeyPressedThisFrame(i)) {
					luaManager.triggerEvent(new event::KeyDownEvent(i, controlDown, shiftDown, altDown, superDown));

					// TODO: Remove! Deprecated generic key event.
					luaManager.triggerEvent(new event::KeyEvent(i, true, controlDown, shiftDown, altDown, superDown));
				}
				else if (inputController->isKeyReleasedThisFrame(i)) {
					luaManager.triggerEvent(new event::KeyUpEvent(i, controlDown, shiftDown, altDown, superDown));

					// TODO: Remove! Deprecated generic key event.
					luaManager.triggerEvent(new event::KeyEvent(i, false, controlDown, shiftDown, altDown, superDown));
				}
			}

			// Do the same with mouse buttons.
			for (size_t i = 0; i < 8; i++) {
				if (inputController->isMouseButtonPressedThisFrame(i)) {
					luaManager.triggerEvent(new event::MouseButtonDownEvent(i, controlDown, shiftDown, altDown, superDown));
				}
				else if (inputController->isMouseButtonReleasedThisFrame(i)) {
					luaManager.triggerEvent(new event::MouseButtonUpEvent(i, controlDown, shiftDown, altDown, superDown));
				}
			}

			// Look at mouse axis events.
			LONG mouseDeltaX = inputController->mouseState.lX;
			LONG mouseDeltaY = inputController->mouseState.lY;
			if (mouseDeltaX || mouseDeltaY) {
				luaManager.triggerEvent(new event::MouseAxisEvent(mouseDeltaX, mouseDeltaY, controlDown, shiftDown, altDown, superDown));
			}

			// Finally, look at mouse scroll wheel.
			LONG mouseDeltaZ = inputController->mouseState.lZ;
			if (mouseDeltaZ != 0) {
				luaManager.triggerEvent(new event::MouseWheelEvent(mouseDeltaZ, controlDown, shiftDown, altDown, superDown));
			}
		}

		//
		// Mobile actor death event.
		//

		void __fastcall OnMobileActorDeath(TES3::MobileActor* mobileActor) {
			mobileActor->onDeath();
		}

		void __fastcall OnMobilePlayerDeath(TES3::MobilePlayer* mobilePlayer) {
			mobilePlayer->onDeath();
		}

		//
		// Mobile actor apply damage event.
		//

		bool __fastcall OnApplyDamage(TES3::MobileActor* mobileActor, DWORD _UNUSED_, float damage, char flag1, char flag2, char flag3) {
			return mobileActor->applyHealthDamage(damage, flag1, flag2, flag3);
		}

		//
		// Projectile expire event.
		//

		void __fastcall OnProjectileExpire(void* mobController, DWORD _UNUSED_, TES3::Reference* reference) {
			// Get the fired projectile, and trigger an event for it.
			
			TES3::MobileProjectile* projectile = reference->getAttachedMobileProjectile();
			LuaManager::getInstance().triggerEvent(new event::ProjectileExpireEvent(projectile));

			// Call overwritten function.
			reinterpret_cast<void(__thiscall *)(void*, TES3::Reference*)>(0x5637F0)(mobController, reference);
		}

		//
		// Spell resist event.
		//

		void __cdecl OnSpellResist(TES3::MagicSourceInstance * spellInstance, TES3::MagicEffectInstance * effectInstance, int effectIndex, int resistAttribute) {
			// Call original function.
			reinterpret_cast<void(__cdecl *)(TES3::MagicSourceInstance *, TES3::MagicEffectInstance *, int, int)>(0x517E40)(spellInstance, effectInstance, effectIndex, resistAttribute);

			// Trigger event, see if we want to overwrite the resisted percentage.
			sol::table eventData = LuaManager::getInstance().triggerEvent(new event::SpellResistEvent(spellInstance, effectInstance, effectIndex, resistAttribute));
			if (eventData.valid()) {
				effectInstance->resistedPercent = eventData["resistedPercent"];
			}
		}

		//
		// Spell tick event.
		//

		void __cdecl MagicEffectDispatch(TES3::EffectID::EffectID effectId, TES3::MagicSourceInstance * sourceInstance, float deltaTime, TES3::MagicEffectInstance * effectInstance, int effectIndex) {
			sol::table eventData = LuaManager::getInstance().triggerEvent(new event::SpellTickEvent(effectId, sourceInstance, deltaTime, effectInstance, effectIndex));
			if (eventData.valid()) {
				if (eventData["block"] == true) {
					// We still need the main effect event function to be called for visual effects and durations to be handled.
					int flags = (tes3::getBaseEffectFlags()[effectId] >> 12) & 0xFFFFFF01;
					int value = 0;
					reinterpret_cast<char(__cdecl *)(TES3::MagicSourceInstance *, float, TES3::MagicEffectInstance *, int, bool, int, int *, DWORD, int, bool(__cdecl *)(void *, void *, int))>(0x518460)(sourceInstance, deltaTime, effectInstance, effectIndex, true, flags, &value, 0x7886F0, 0x1C, nullptr);
					return;
				}
			}

			reinterpret_cast<void (__cdecl **)(TES3::MagicSourceInstance *, float, TES3::MagicEffectInstance *, int)>(0x7884B0)[effectId](sourceInstance, deltaTime, effectInstance, effectIndex);
		}

		void patchMagicEffectDispatch(DWORD address, BYTE pushInstruction) {
			// Push the switch index onto the stack.
			writeByteUnprotected(address, pushInstruction);

			// Call our dispatch handler.
			genCallUnprotected(address + 1, reinterpret_cast<DWORD>(MagicEffectDispatch));

			// Fix ESP by popping the value back. We throw the value into a register that won't be used.
			// This is done to try to keep the instructions all of the same size, so that stepping over the 
			// instructions in another debugger doesn't show as being ugly.
			writeByteUnprotected(address + 6, 0x59);
		}

		//
		// Exercise skill event.
		//

		void __fastcall OnExerciseSkill(TES3::MobilePlayer* player, DWORD _UNUSED_, int skill, float progress) {
			player->exerciseSkill(skill, progress);
		}

		//
		// Level up event.
		//

		void __stdcall OnLevelUp() {
			// Call the original function we overwrote to call this one.
			reinterpret_cast<void(__stdcall *)()>(0x626220)();

			// Launch our event.
			LuaManager::getInstance().triggerEvent(new event::LevelUpEvent());
		}

		//
		// Projectile expire event.
		//

		DWORD __fastcall OnBrewPotion(TES3::UI::InventoryTile* inventoryTile, DWORD _UNUSED_, TES3::Alchemy* object, TES3::ItemData* itemData, DWORD unk1, DWORD unk2, DWORD unk3, DWORD unk4) {
			// Call original function.
			DWORD result = reinterpret_cast<DWORD(__thiscall*)(TES3::UI::InventoryTile*, TES3::Object*, TES3::ItemData*, DWORD, DWORD, DWORD, DWORD)>(0x6313E0)(inventoryTile, object, itemData, unk1, unk2, unk3, unk4);

			// Pass a lua event.
			LuaManager::getInstance().triggerEvent(new event::PotionBrewedEvent(object));

			return result;
		}

		// Spell cast resolution hook

		float __fastcall OnSpellCastResolution(TES3::Spell* spell, DWORD _UNUSED_, TES3::MobileActor* caster, bool checkMagicka, int* weakestSchoolId) {
			return spell->castChanceOnCast(caster, checkMagicka, weakestSchoolId);
		}

		//
		// Magic cast success hook (includes spells, enchants and alchemy)
		//

		void __stdcall OnMagicCastSuccess(TES3::MagicSourceInstance* magicInstance, int* pExpGainSchool) {
			// Ignore ability spells, as they are automatically activated as NPCs enter simulation range.
			if (magicInstance->sourceCombo.sourceType == TES3::MagicSourceType::Spell && magicInstance->sourceCombo.source.asSpell->castType == TES3::SpellCastType::Ability) {
				return;
			}
			
			// Magic from any source event
			LuaManager& luaManager = LuaManager::getInstance();
			luaManager.triggerEvent(new event::MagicCastedEvent(magicInstance));

			if (magicInstance->sourceCombo.sourceType == TES3::MagicSourceType::Spell) {
				// Spell cast event, allows updating the school that gains experience
				sol::table eventData = luaManager.triggerEvent(new event::SpellCastedEvent(magicInstance, true, *pExpGainSchool));
				if (eventData.valid()) {
					*pExpGainSchool = eventData.get_or("expGainSchool", int(TES3::MagicSchool::None));
				}
			}
		}

		static DWORD postMagicCastSuccess = TES3_HOOK_MAGIC_CAST_SUCCESS_RETURN;
		static __declspec(naked) void HookMagicCastSuccess() {
			_asm
			{
				// Save all registers.
				pushad

				// Actually use our hook.
				lea eax, [ebp - 0x1C]
				push eax
				push esi
				call OnMagicCastSuccess

				// Restore all registers.
				popad

				// Overwritten code.
				mov dword ptr [esi + 0xB4], 1

				// Resume normal execution.
				jmp postMagicCastSuccess
			}
		}

		//
		// Spell cast failure hook (only when cast chance roll fails)
		//

		void __stdcall OnSpellCastFailure(TES3::MagicSourceInstance* magicInstance, int* pExpGainSchool) {
			LuaManager& luaManager = LuaManager::getInstance();
			luaManager.triggerEvent(new event::SpellCastedEvent(magicInstance, false, *pExpGainSchool));
		}

		static DWORD postSpellCastFailure = TES3_HOOK_SPELL_CAST_FAILURE_RETURN;
		static __declspec(naked) void HookSpellCastFailure() {
			_asm
			{
				// Save all registers.
				pushad

				// Actually use our hook.
				lea eax, [ebp - 0x1C]
				push eax
				push esi
				call OnSpellCastFailure

				// Restore all registers.
				popad

				// Overwritten code.
				mov dword ptr [esi + 0xB4], 7

				// Resume normal execution.
				jmp postSpellCastFailure
			}
		}

		//
		// When an object is deleted, we need to clear any lua mapping to it.
		//

		TES3::BaseObject* __fastcall OnEntityDelete(TES3::BaseObject* object) {
			// Clear the object from the userdata cache.
			LuaManager::getInstance().removeUserdataFromCache(object);

			// Let the object finally die.
			return reinterpret_cast<TES3::BaseObject*(__thiscall *)(TES3::BaseObject*)>(TES3_BaseObject_destructor)(object);
		}

		//
		// Event: Calculate Rest Interruption
		//

		void __cdecl OnInterruptRestCheck(bool interruptRest) {
			// Call original function.
			reinterpret_cast<void(__cdecl *)(bool)>(0x634B90)(interruptRest);

			// Get the global values for rest interruption.
			int count = tes3::getRestInterruptCount();
			int hour = tes3::getRestHoursInterrupted();

			// Manually reset the count when there's no interruption.
			if (hour == -1) {
				count = 0;
				tes3::setRestInterruptCount(0);
			}

			// Fire off an event and change the globals accordingly.
			mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
			sol::table eventData = luaManager.triggerEvent(new event::CalcRestInterruptEvent(count, hour));
			if (eventData.valid()) {
				// Allow blocking any spawn.
				if (eventData["block"] == true) {
					tes3::setRestInterruptCount(0);
					tes3::setRestHoursInterrupted(-1);
					return;
				}

				// Get the values back from lua.
				count = eventData["count"];
				hour = eventData["hour"];

				// Validate values so if one is invalid then both represent no spawn.
				if (hour <= -1 || count == 0) {
					count = 0;
					hour = -1;
				}

				// Update the global values.
				tes3::setRestInterruptCount(count);
				tes3::setRestHoursInterrupted(hour);
			}
		}

		TES3::Object * __fastcall OnInterruptRest(TES3::LeveledCreature* leveledCreature, DWORD _UNUSED_) {
			// Creature that we return.
			TES3::Object * creature = NULL;

			// Fire off an event and change the determined creature.
			mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
			sol::table eventData = luaManager.triggerEvent(new event::RestInterruptEvent(leveledCreature));
			if (eventData.valid()) {
				// Allow blocking any spawn.
				if (eventData["block"] == true) {
					tes3::setRestInterruptCount(0);
					tes3::setRestHoursInterrupted(-1);
					return NULL;
				}

				// Allow overriding the spawn.
				sol::object maybeCreature = eventData["creature"];
				if (maybeCreature.is<TES3::MobileNPC*>() || maybeCreature.is<TES3::MobileCreature*>()) {
					return maybeCreature.as<TES3::Actor*>();
				}
				else if (maybeCreature.is<TES3::LeveledCreature*>()) {
					return maybeCreature.as<TES3::LeveledCreature*>()->resolve();
				}
			}

			// If we were given a leveled creature initially, fall back to that.
			if (leveledCreature) {
				return leveledCreature->resolve();
			}
			else {
				return NULL;
			}
		}

		//
		// Event: topicAdded
		//

		void __fastcall OnAddTopicAtIndex(TES3::Iterator<TES3::Dialogue> * topicList, DWORD _UNUSED_, TES3::Dialogue * topic, unsigned int index) {
			// Run overwritten function.
			topicList->addItemAtIndex(topic, index);

			// Raise event.
			mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
			luaManager.triggerEvent(new event::AddTopicEvent(topic));
		}

		void __fastcall OnAddTopic(TES3::Iterator<TES3::Dialogue> * topicList, DWORD _UNUSED_, TES3::Dialogue * topic) {
			// Run overwritten function.
			topicList->addItem(topic);

			// Raise event.
			mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
			luaManager.triggerEvent(new event::AddTopicEvent(topic));
		}

		//
		// Event: Journal
		//

		bool __fastcall OnJournalAdd(TES3::Dialogue * dialogue, DWORD _UNUSED_, int index, TES3::MobileActor * actor) {
			return dialogue->addToJournal(index, actor);
		}

		void __fastcall OnJournalSet(TES3::Dialogue * dialogue, DWORD _UNUSED_, int index) {
			dialogue->setJournalIndex(index);
		}

		//
		// Event: UI Created
		//

		void __fastcall OnUICreatedAfterSetProperty(TES3::UI::Element* element, DWORD _UNUSED_, TES3::UI::Property property, TES3::UI::PropertyValue value, TES3::UI::PropertyType type) {
			// Overwritten code.
			element->setProperty(property, value, type);

			// Fire off the event.
			LuaManager::getInstance().triggerEvent(new event::GenericUiActivatedEvent(element));

			// DEPRECATED. TODO: Remove before 2.1 final.
			LuaManager::getInstance().triggerEvent(new event::GenericUiCreatedEvent(element));
		}

		void __fastcall OnUICreatedAfterPerformLayout(TES3::UI::Element* element, DWORD _UNUSED_, bool unknown) {
			// Overwritten code.
			element->performLayout(unknown);

			// Fire off the event.
			LuaManager::getInstance().triggerEvent(new event::GenericUiActivatedEvent(element));

			// DEPRECATED. TODO: Remove before 2.1 final.
			LuaManager::getInstance().triggerEvent(new event::GenericUiCreatedEvent(element));
		}

		void __cdecl OnUICreatedAfterConsoleMenuFocus(TES3::UI::Element* widget, int event, int data0, int data1, TES3::UI::Element* element) {
			// Overwritten code.
			const auto ui_ConsoleMenu_onFocus = reinterpret_cast<void (__cdecl*)(TES3::UI::Element*,int,int,int, TES3::UI::Element*)>(0x5B23E0);
			ui_ConsoleMenu_onFocus(widget, event, data0, data1, element);

			// Fire off the event.
			LuaManager::getInstance().triggerEvent(new event::GenericUiActivatedEvent(element));

			// DEPRECATED. TODO: Remove before 2.1 final.
			LuaManager::getInstance().triggerEvent(new event::GenericUiCreatedEvent(element));
		}

		void __cdecl OnUICreatedAfterConsoleMenuShown(TES3::UI::Element* element) {
			// Overwritten code.
			reinterpret_cast<void(__cdecl*)(TES3::UI::Element*)>(0x595350)(element);

			// Fire off the event.
			LuaManager::getInstance().triggerEvent(new event::GenericUiActivatedEvent(element));

			// DEPRECATED. TODO: Remove before 2.1 final.
			LuaManager::getInstance().triggerEvent(new event::GenericUiCreatedEvent(element));
		}

		void __fastcall OnUICreatedAfterMenuVisibleEvent(TES3::UI::Element* element, DWORD _UNUSED_, bool unknown) {
			// Overwritten code.
			const auto ui_setMenuVisibleWithEvent = reinterpret_cast<void (__thiscall*)(TES3::UI::Element*, bool)>(0x582F20);
			ui_setMenuVisibleWithEvent(element, unknown);

			// Fire off the event.
			LuaManager::getInstance().triggerEvent(new event::GenericUiActivatedEvent(element));

			// DEPRECATED. TODO: Remove before 2.1 final.
			LuaManager::getInstance().triggerEvent(new event::GenericUiCreatedEvent(element));
		}

		void __cdecl OnUICreatedAfterScrollPaneUpdate(TES3::UI::Element* element) {
			// Overwritten code.
			const auto ui_ScrollPane_UpdateScrollBars = reinterpret_cast<void (__cdecl*)(TES3::UI::Element*)>(0x649E40);
			ui_ScrollPane_UpdateScrollBars(element);

			// Fire off the event.
			LuaManager::getInstance().triggerEvent(new event::GenericUiActivatedEvent(element));

			// DEPRECATED. TODO: Remove before 2.1 final.
			LuaManager::getInstance().triggerEvent(new event::GenericUiCreatedEvent(element));
		}

		//
		// Event: UI Object Tooltip post-creation.
		//

		void __stdcall OnUIObjectTooltip(TES3::Object* object, TES3::ItemData* itemData, int count) {
			// Call original function.
			reinterpret_cast<void(__stdcall *)(TES3::Object*, TES3::ItemData*, int)>(0x590D90)(object, itemData, count);

			// Fire off the event.
			TES3::UI::Element* tooltip = TES3::UI::findHelpLayerMenu(TES3::UI::UI_ID(TES3::UI::Property::HelpMenu));
			LuaManager::getInstance().triggerEvent(new event::UiObjectTooltipEvent(tooltip, object, itemData, count));
		}

		//
		// Event: UI Spell Tooltip post-creation.
		//

		bool __cdecl OnUISpellTooltip(TES3::UI::Element* widget, int event, int data0, int data1, TES3::UI::Element* element) {
			// Call original function.
			bool created = reinterpret_cast<bool(__cdecl *)(TES3::UI::Element*, int, int, int, TES3::UI::Element*)>(0x5E45C0)(widget, event, data0, data1, element);

			// Fire off the event.
			if (created) {
				TES3::UI::Element* tooltip = TES3::UI::findHelpLayerMenu(TES3::UI::UI_ID(TES3::UI::Property::HelpMenu));
				TES3::Spell * spell = reinterpret_cast<TES3::Spell *>(element->getProperty(TES3::UI::PropertyType::Pointer, *reinterpret_cast<TES3::UI::Property*>(0x7D431C)).ptrValue);
				LuaManager::getInstance().triggerEvent(new event::UiSpellTooltipEvent(tooltip, spell));
			}

			return created;
		}

		//
		// Event: Activation Target Changed
		//

		static const auto global_TES3_Game = reinterpret_cast<TES3::Game**const>(0x7C6CDC);
		static const uintptr_t MACP__getPlayerAnimData_fieldEC = 0x567990;
		static TES3::Reference* previousTarget;

		static __declspec(naked) void HookPreFindActivationTarget() {
			_asm {
				mov eax, ds:[0x7C6CDC]  // global_TES3_Game
				mov eax, [eax + 0xE8]	// game->playerTarget
				mov previousTarget, eax
				jmp MACP__getPlayerAnimData_fieldEC
			}
		}

		static void HookPostFindActivationTarget() {
			TES3::Reference *currentTarget = (*global_TES3_Game)->playerTarget;
			if (previousTarget != currentTarget) {
				LuaManager::getInstance().triggerEvent(new event::ActivationTargetChangedEvent(previousTarget, currentTarget));
			}
		}

		//
		// Event: Weather cycle and transition events
		//

		// Fix missing coverage of updates to lastActiveRegion
		__declspec(naked) void patchWeatherRegionCheck() {
			__asm {
				mov ecx, [esi + 0x58]	// ecx = WorldController->weatherController
				mov [ecx + 0x1D0], eax  // weatherController->lastActiveRegion = eax
				nop
			}
		}
		const size_t patchWeatherRegionCheck_size = 0xA;

		bool __fastcall OnWeatherCycle(TES3::Cell* cell, DWORD _UNUSED_) {
			// Fire off the event.
			LuaManager::getInstance().triggerEvent(new event::WeatherCycledEvent());

			// Call original function.
			return reinterpret_cast<bool (__thiscall *)(TES3::Cell*)>(0x4E22F0)(cell);
		}

		void __fastcall OnWeatherImmediateChange(TES3::WeatherController* controller, DWORD _UNUSED_, DWORD weatherId, DWORD transitionScalar) {
			// Call original function.
			reinterpret_cast<void (__thiscall *)(TES3::WeatherController*, DWORD, DWORD)>(0x441C40)(controller, weatherId, transitionScalar);

			// Fire off the event, after function completes.
			LuaManager::getInstance().triggerEvent(new event::WeatherChangedImmediateEvent());
		}

		void* __cdecl OnWeatherTransitionBegin(const char* texturePath, void* data) {
			// Fire off the event.
			LuaManager::getInstance().triggerEvent(new event::WeatherTransitionStartedEvent());

			// Call original function.
			return reinterpret_cast<void* (__cdecl *)(const char*, void*)>(0x6DE7F0)(texturePath, data);
		}

		void __fastcall OnWeatherTransitionEnd(void* modelData, DWORD _UNUSED_) {
			// Fire off the event.
			LuaManager::getInstance().triggerEvent(new event::WeatherTransitionFinishedEvent());

			// Call original function.
			reinterpret_cast<void (__thiscall *)(void*)>(0x414890)(modelData);
		}

		//
		// Event: Music, new track
		//

		bool __fastcall OnSelectMusicTrack(TES3::WorldController* controller, DWORD _UNUSED_, int situation) {
			// Fire off the event.
			sol::table eventData = LuaManager::getInstance().triggerEvent(new event::MusicSelectTrackEvent(situation));

			if (eventData.valid()) {
				sol::optional<std::string> musicPath = eventData["music"];
				if (musicPath) {
					const auto TES3_getThreadSafeStringBuffer = reinterpret_cast<char*(__thiscall*)(char*)>(0x4D51B0);
					char* buffer = TES3_getThreadSafeStringBuffer(reinterpret_cast<char*>(0x7CB478));
					snprintf(buffer, 512, "Data Files/music/%s", musicPath.value().c_str());
					return true;
				}
			}

			// Call original function.
			return reinterpret_cast<bool(__thiscall *)(TES3::WorldController*, int)>(0x410EA0)(controller, situation);
		}

		//
		// Event: Weapon Ready
		//

		const auto TES3_MobileActor_offhandAnimState = reinterpret_cast<void(__thiscall*)(const TES3::MobileActor*)>(0x52D5B0);

		void __fastcall OnReadyNoWeapon(TES3::MobileActor * actor) {
			TES3_MobileActor_offhandAnimState(actor);
			if (actor->reference) {
				LuaManager::getInstance().triggerEvent(new event::WeaponReadiedEvent(actor->reference));
			}
		}

		const auto TES3_MobileActor_unreadyWeapon = reinterpret_cast<void(__thiscall*)(const TES3::MobileActor*)>(0x528000);

		void __fastcall OnUnreadyWeapon(TES3::MobileActor * mobile) {
			bool wasDrawn = (mobile->actorFlags & TES3::MobileActorFlag::WeaponDrawn);

			TES3_MobileActor_unreadyWeapon(mobile);

			if (mobile && mobile->reference && wasDrawn) {
				LuaManager::getInstance().triggerEvent(new event::WeaponUnreadiedEvent(mobile->reference));
			}
		}

		//
		// Event: Calculate barter price.
		//

		// Store the last item stack sold/bought.
		static TES3::EquipmentStack * OnCalculateBarterPrice_stack = nullptr;

		// Store the last item stack's calculated base price.
		static int OnCalculateBarterPrice_value = 0;

		// Hook point for calculating an item's price, before our real event. We'll store the above values here.
		int __cdecl OnCalculateBarterPrice_CalcItemValue(TES3::EquipmentStack* stack) {
			OnCalculateBarterPrice_stack = stack;
			OnCalculateBarterPrice_value = stack->getAdjustedValue();
			return OnCalculateBarterPrice_value;
		}

		// Use the above stored values and finish our event.
		int __fastcall OnCalculateBarterPrice(TES3::MobileActor * mobile, DWORD _UNUSED_, int basePrice, bool buying) {
			int price = mobile->determineModifiedPrice(basePrice, buying);
			int count = 1;
			if (OnCalculateBarterPrice_value > 0) {
				count = std::abs(basePrice / OnCalculateBarterPrice_value);
			}

			sol::table result = LuaManager::getInstance().triggerEvent(new event::CalculateBarterPriceEvent(mobile, basePrice, price, buying, count, OnCalculateBarterPrice_stack));
			if (result.valid()) {
				price = result["price"];
			}

			return price;
		}

		//
		// Event: Calculate repair price.
		//

		// Cached value of the inventory iterator.
		static TES3::Iterator<TES3::ItemStack>* OnCalculateRepairPriceForList_CurrentInventoryList = nullptr;

		// Store the inventory list that we're looking at when generating the repair list.
		TES3::IteratorNode<TES3::ItemStack>* __fastcall OnCalculateRepairPriceForList_GetItemList(TES3::Iterator<TES3::ItemStack>* inventoryList) {
			OnCalculateRepairPriceForList_CurrentInventoryList = inventoryList;

			return inventoryList->getFirstNode();
		}

		// Get the price for each item on the list.
		int __fastcall OnCalculateRepairPriceForList(TES3::MobileActor * mobile, DWORD _UNUSED_, int basePrice, bool buying) {
			int price = mobile->determineModifiedPrice(basePrice, buying);

			if (basePrice <= 0) {
				basePrice = 1;
			}

			if (price <= 0) {
				price = 1;
			}

			sol::table result = LuaManager::getInstance().triggerEvent(new event::CalculateRepairPriceEvent(mobile, basePrice, price, OnCalculateRepairPriceForList_CurrentInventoryList->current->data));
			if (result.valid()) {
				price = result["price"];
			}

			return price;
		}

		// Cache value of the item stack that is going to be repaired.
		static TES3::ItemStack* OnCalculateRepairPrice_ItemStack = nullptr;

		// Set the above value for later reference.
		TES3::UI::PropertyValue* __fastcall OnCalculateRepairPrice_GetRepairStack(const TES3::UI::Element* element, DWORD _UNUSED_, TES3::UI::PropertyValue* propValue, TES3::UI::Property prop, TES3::UI::PropertyType propType, const TES3::UI::Element* element2, bool checkInherited) {
			OnCalculateRepairPrice_ItemStack = (TES3::ItemStack*)element->getProperty(propValue, prop, propType, element2, checkInherited)->ptrValue;
			return propValue;
		}

		// Get the price for an item when actually repairing.
		int __fastcall OnCalculateRepairPrice(TES3::MobileActor * mobile, DWORD _UNUSED_, int basePrice, bool buying) {
			int price = mobile->determineModifiedPrice(basePrice, buying);

			if (basePrice <= 0) {
				basePrice = 1;
			}

			if (price <= 0) {
				price = 1;
			}

			sol::table result = LuaManager::getInstance().triggerEvent(new event::CalculateRepairPriceEvent(mobile, basePrice, price, OnCalculateRepairPrice_ItemStack));
			if (result.valid()) {
				price = result["price"];
			}

			return price;
		}

		//
		// Event: Calculate spell price.
		//

		// Cached value of the inventory iterator.
		static TES3::Iterator<TES3::Spell>* OnCalculateSpellPrice_CurrentInventoryList = nullptr;

		// Store the inventory list that we're looking at when generating the repair list.
		TES3::IteratorNode<TES3::Spell>* __fastcall OnCalculateSpellPriceForList_GetSpellList(TES3::Iterator<TES3::Spell>* spellList) {
			OnCalculateSpellPrice_CurrentInventoryList = spellList;

			auto result = spellList->head;
			spellList->current = result;
			return result;
		}

		// Get the price for each item on the list.
		int __fastcall OnCalculateSpellPriceForList(TES3::MobileActor * mobile, DWORD _UNUSED_, int basePrice, bool buying) {
			int price = mobile->determineModifiedPrice(basePrice, buying);

			sol::table result = LuaManager::getInstance().triggerEvent(new event::CalculateSpellPriceEvent(mobile, basePrice, price, OnCalculateSpellPrice_CurrentInventoryList->current->data));
			if (result.valid()) {
				price = result["price"];
			}

			return price;
		}

		static TES3::Spell* OnCalculateSpellPrice_Spell = nullptr;

		TES3::UI::PropertyValue* __fastcall OnCalculateSpellPrice_GetSpell(const TES3::UI::Element* element, DWORD _UNUSED_, TES3::UI::PropertyValue* propValue, TES3::UI::Property prop, TES3::UI::PropertyType propType, const TES3::UI::Element* element2, bool checkInherited) {
			OnCalculateSpellPrice_Spell = (TES3::Spell*)element->getProperty(propValue, prop, propType, element2, checkInherited)->ptrValue;
			return propValue;
		}

		int __fastcall OnCalculateSpellPrice(TES3::MobileActor * mobile, DWORD _UNUSED_, int basePrice, bool buying) {
			int price = mobile->determineModifiedPrice(basePrice, buying);

			sol::table result = LuaManager::getInstance().triggerEvent(new event::CalculateSpellPriceEvent(mobile, basePrice, price, OnCalculateSpellPrice_Spell));
			if (result.valid()) {
				price = result["price"];
			}

			return price;
		}

		//
		// Event: Calculate training price.
		//

		static int OnCalculateTrainingPrice_SkillId = -1;

		// Version of getting the skill value for training for the MCP patched version of the game.
		float __fastcall OnCalculateTrainingPrice_GetSkillPatched(TES3::MobileActor* actor, DWORD _UNUSED_, int skillId) {
			OnCalculateTrainingPrice_SkillId = skillId;
			return actor->getSkillStatistic(skillId)->base;
		}

		// Version of getting the skill value for training for the vanilla version of the game.
		float __fastcall OnCalculateTrainingPrice_GetSkillUnpatched(TES3::MobileActor* actor, DWORD _UNUSED_, int skillId) {
			OnCalculateTrainingPrice_SkillId = skillId;
			return actor->getSkillStatistic(skillId)->current;
		}

		// Our actual hook for sending off the training price event.
		int __fastcall OnCalculateTrainingPrice(TES3::MobileActor * mobile, DWORD _UNUSED_, int basePrice, bool buying) {
			int price = mobile->determineModifiedPrice(basePrice, buying);

			sol::table result = LuaManager::getInstance().triggerEvent(new event::CalculateTrainingPriceEvent(mobile, basePrice, price, OnCalculateTrainingPrice_SkillId));
			if (result.valid()) {
				price = result["price"];
			}

			return price;
		}

		//
		// Event: Calculate travel price.
		//

		// The destination list for the merchant.
		static TES3::Iterator<TES3::TravelDestination> * OnCalculateTravelPrice_DestinationList;

		// The player's friendly actor list.
		static TES3::Iterator<TES3::MobileActor> * OnCalculateTravelPrice_CompanionList;

		// A custom list of followers close enough to travel with the player.
		static std::vector<TES3::MobileActor*> OnCalculateTravelPrice_TravelCompanionList;

		// Hook for ensuring that we have the right destination list.
		TES3::IteratorNode<TES3::TravelDestination>* __fastcall OnCalculateTravelPrice_GetDestinationList(TES3::Iterator<TES3::TravelDestination>* iterator) {
			OnCalculateTravelPrice_DestinationList = iterator;

			return iterator->getFirstNode();
		}

		// Hook for ensuring that we have the right companion list.
		TES3::IteratorNode<TES3::MobileActor>* __fastcall OnCalculateTravelPrice_GetCompanionList(TES3::Iterator<TES3::MobileActor>* iterator) {
			OnCalculateTravelPrice_CompanionList = iterator;

			OnCalculateTravelPrice_TravelCompanionList.clear();

			return iterator->getFirstNode();
		}

		// Hook for checking and adding companions for our custom list so we can report valid companions in the event.
		float OnCalculateTravelPrice_CheckCompanionDistance(TES3::Vector3* destinationPosition, TES3::Vector3* playerPosition) {
			float distance = destinationPosition->distance(playerPosition);
			if (OnCalculateTravelPrice_CompanionList->size * 128.0f + 512.0f > distance) {
				OnCalculateTravelPrice_TravelCompanionList.push_back(OnCalculateTravelPrice_CompanionList->current->data);
			}
			return distance;
		}

		// Send off our travel price event when building the destination list.
		int __fastcall OnCalculateTravelPriceForList(TES3::MobileActor * mobile, DWORD _UNUSED_, int basePrice, bool buying) {
			int price = mobile->determineModifiedPrice(basePrice, buying);

			sol::table result = LuaManager::getInstance().triggerEvent(new event::CalculateTravelPriceEvent(mobile, basePrice, price, OnCalculateTravelPrice_DestinationList->current->data, &OnCalculateTravelPrice_TravelCompanionList));
			if (result.valid()) {
				price = result["price"];
			}

			return price;
		}

		// Send off our travel price event when actually selecting a destination.
		int __fastcall OnCalculateTravelPriceForDestination(TES3::MobileActor * mobile, DWORD _UNUSED_, int basePrice, bool buying) {
			int price = mobile->determineModifiedPrice(basePrice, buying);

			// Destination is off by one, so we need the previous entry.
			auto destination = OnCalculateTravelPrice_DestinationList->tail->data;
			if (OnCalculateTravelPrice_DestinationList->current != nullptr) {
				destination = OnCalculateTravelPrice_DestinationList->current->previous->data;
			}

			sol::table result = LuaManager::getInstance().triggerEvent(new event::CalculateTravelPriceEvent(mobile, basePrice, price, destination, &OnCalculateTravelPrice_TravelCompanionList));
			if (result.valid()) {
				price = result["price"];
			}

			return price;
		}

		//
		// Event: Skill raised.
		//

		const auto TES3_ShowSkillRaisedNotification = reinterpret_cast<void(__cdecl*)(int, char*)>(0x629FC0);

		void __cdecl OnSkillRaised(int skillId, char * buffer) {
			TES3_ShowSkillRaisedNotification(skillId, buffer);
			LuaManager::getInstance().triggerEvent(new event::SkillRaisedEvent(skillId, TES3::WorldController::get()->getMobilePlayer()->skills[skillId].base));
		}

		void LuaManager::executeMainModScripts(const char* path, const char* filename) {
			for (auto & p : std::experimental::filesystem::recursive_directory_iterator(path)) {
				if (p.path().filename() == filename) {
					// If a parent directory is marked .disabled, ignore files in it.
					if (p.path().string().find(".disabled\\") != std::string::npos) {
						log::getLog() << "[LuaManager] Skipping mod initializer in disabled directory: " << p.path().string() << std::endl;
						continue;
					}

					sol::protected_function_result result = luaState.safe_script_file(p.path().string(), &sol::script_pass_on_error);
					if (!result.valid()) {
						sol::error error = result;
						log::getLog() << "[LuaManager] ERROR: Failed to run mod initialization script:" << std::endl << error.what() << std::endl;
					}
				}
			}
		}

		//
		// Event: UI Refreshed
		//

		const auto TES3_UpdateStatsScrollPane = reinterpret_cast<bool(__cdecl*)(TES3::UI::Element *)>(0x649870);

		bool __cdecl OnRefreshedStatsPane(TES3::UI::Element * element) {
			bool result = TES3_UpdateStatsScrollPane(element);
			LuaManager::getInstance().triggerEvent(new event::UiRefreshedEvent(element));
			return result;
		}

		//
		// Event: Filter Inventory/Barter Tile
		//

		const auto TES3_FilterInventoryTile = reinterpret_cast<bool(__cdecl*)(TES3::UI::InventoryTile *, TES3::Item *)>(0x5CC720);

		bool __cdecl OnFilterInventoryTile(TES3::UI::InventoryTile * tile, TES3::Item * item) {
			sol::table payload = LuaManager::getInstance().triggerEvent(new event::FilterInventoryEvent(tile, item));
			if (payload.valid()) {
				sol::object filter = payload["filter"];
				if (filter.is<bool>()) {
					return filter.as<bool>();
				}
			}

			return TES3_FilterInventoryTile(tile, item);
		}

		const auto TES3_FilterBarterTile = reinterpret_cast<bool(__cdecl*)(TES3::UI::InventoryTile *, TES3::Item *)>(0x5A5430);

		bool __cdecl OnFilterBarterTile(TES3::UI::InventoryTile * tile, TES3::Item * item) {
			sol::table payload = LuaManager::getInstance().triggerEvent(new event::FilterBarterMenuEvent(tile, item));
			if (payload.valid()) {
				sol::object filter = payload["filter"];
				if (filter.is<bool>()) {
					return filter.as<bool>();
				}
			}

			return TES3_FilterBarterTile(tile, item);
		}

		void __fastcall OnFilterContentsTile(TES3::Iterator<TES3::UI::InventoryTile> * list, DWORD _UNUSUED_, TES3::UI::InventoryTile * tile) {
			sol::table payload = LuaManager::getInstance().triggerEvent(new event::FilterContentsMenuEvent(tile, tile->item));
			if (payload.valid()) {
				sol::object filter = payload["filter"];
				if (filter.is<bool>()) {
					if (!filter.as<bool>()) {
						return;
					}
				}
			}

			list->addItem(tile);
		}

		void __fastcall OnFilterContentsTileForTakeAll(TES3::Iterator<TES3::UI::InventoryTile> * list, DWORD _UNUSUED_, TES3::UI::InventoryTile * tile) {
			sol::table payload = LuaManager::getInstance().triggerEvent(new event::FilterContentsMenuEvent(tile, tile->item));
			if (payload.valid()) {
				sol::object filter = payload["filter"];
				if (filter.is<bool>()) {
					if (!filter.as<bool>()) {
						return;
					}
				}
			}

			list->addItem(tile);
			if (list->size == 1) {
				TES3::WorldController::get()->playItemUpDownSound(tile->item);
			}
		}

		bool __fastcall OnFilterInventorySelect(TES3::UI::Element * element, TES3::UI::EventCallback callback) {
			TES3::Item * item = reinterpret_cast<TES3::Item*>(element->getProperty(TES3::UI::PropertyType::Pointer, *reinterpret_cast<TES3::UI::Property*>(0x7D3C88)).ptrValue);
			TES3::ItemData * itemData = reinterpret_cast<TES3::ItemData*>(element->getProperty(TES3::UI::PropertyType::Pointer, *reinterpret_cast<TES3::UI::Property*>(0x7D3C16)).ptrValue);

			const char* callbackType = TES3::UI::getInventorySelectType();
			sol::table payload = LuaManager::getInstance().triggerEvent(new event::FilterInventorySelectEvent(callbackType, item, itemData));
			if (payload.valid()) {
				sol::object filter = payload["filter"];
				if (filter.is<bool>()) {
					return filter.as<bool>();
				}
			}

			// Call original callback.
			return callback(element, TES3::UI::Property::null, 0, 0, element);
		}

		__declspec(naked) void patchFilterInventorySelect() {
			__asm {
				mov ecx, edi
				mov edx, eax
				nop // Replaced with a call generation. Can't do so here, because offsets aren't accurate.
				nop // ^
				nop // ^
				nop // ^
				nop // ^
				nop
				nop
				nop
				nop
				nop
				nop
				nop
			}
		}
		const size_t patchFilterInventorySelect_size = 0x10;

		//
		// Event: Dialogue Info Read Text
		//

		static TES3::DialogueInfo * lastReadDialogueInfo = nullptr;

		TES3::GameFile * __fastcall PatchGetDialogueInfoText_GetSourceMod(TES3::DialogueInfo * dialogueInfo) {
			lastReadDialogueInfo = dialogueInfo;
			return dialogueInfo->sourceMod;
		}

		bool __fastcall PatchGetDialogueInfoText_ReadFromFile(TES3::GameFile * gameFile, DWORD _UNUSUED_, char * dialogueTextBuffer, size_t size) {
			// Allow the event to override the text.
			sol::object eventResult = mwse::lua::LuaManager::getInstance().triggerEvent(new mwse::lua::event::InfoGetTextEvent(lastReadDialogueInfo));
			if (eventResult.valid()) {
				sol::table eventData = eventResult;
				sol::object text = eventData["text"];
				if (text.is<const char*>()) {
					// Create our new buffer.
					const char* newText = text.as<const char*>();
					auto length = strlen(newText);
					char * buffer = reinterpret_cast<char*>(tes3::_new(length + 1));

					// Delete the previous buffer and replace it with this one.
					tes3::_delete(*reinterpret_cast<char**>(0x7CA5AC));
					*reinterpret_cast<char**>(0x7CA5AC) = buffer;

					// Copy into the buffer and get out of here.
					buffer[length] = '\0';
					strcpy(buffer, newText);
					return true;
				}
			}

			return gameFile->readChunkData(dialogueTextBuffer, size);
		}

		//
		// Event: Dialogue Info response script
		//

		void __fastcall OnRunDialogueCommand(TES3::Script * script, DWORD _UNUSUED_, TES3::ScriptCompiler * compiler, const char* command, int source, TES3::Reference * reference, TES3::ScriptVariables * variables, TES3::DialogueInfo * info, TES3::Dialogue * dialogue) {
			// Allow the event to override the text.
			sol::object eventResult = mwse::lua::LuaManager::getInstance().triggerEvent(new mwse::lua::event::InfoResponseEvent(command, reference, variables, dialogue, info));
			if (eventResult.valid()) {
				sol::table eventData = eventResult;
				if (eventData["block"] == true) {
					return;
				}
			}

			script->doCommand(compiler, command, source, reference, variables, info, dialogue);
		}

		//
		// Event: Item Dropped.
		//

		const auto TES3_DataHandler_UpdateLightingForReference = reinterpret_cast<void(__thiscall*)(TES3::DataHandler *, TES3::Reference *)>(0x485E40);

		void __fastcall OnItemDropped(TES3::DataHandler * dataHandler, DWORD _UNUSED_, TES3::Reference* reference) {
			TES3_DataHandler_UpdateLightingForReference(dataHandler, reference);

			mwse::lua::LuaManager::getInstance().triggerEvent(new mwse::lua::event::ItemDroppedEvent(reference));
		}

		//
		// Event: Calculate hit chance.
		//

		int __fastcall OnCalculateHitChance(TES3::MobileActor * attacker, int hitChance) {
			// Allow the event to override the text.
			sol::object eventResult = mwse::lua::LuaManager::getInstance().triggerEvent(new mwse::lua::event::CalcHitChanceEvent(attacker, hitChance));
			if (eventResult.valid()) {
				sol::table eventData = eventResult;
				hitChance = getOptionalParam<int>(eventData, "hitChance", hitChance);
			}

			// Overwritten code for this hook.
			if (TES3::WorldController::get()->menuController->unknown_0x24 % 1) {
				const auto TES3_getThreadSafeStringBuffer = reinterpret_cast<char*(__thiscall*)(char*)>(0x4D51B0);
				char* buffer = TES3_getThreadSafeStringBuffer(reinterpret_cast<char*>(0x7CB478));
				sprintf(buffer, "Attack Chance %d%%, for %s to hit %s", hitChance, attacker->reference->baseObject->getObjectID(), attacker->actionData.hitTarget->reference->baseObject->getObjectID());
				const auto TES3_ConsoleLogResult = reinterpret_cast<void(__cdecl*)(const char*, bool)>(0x5B2C20);
				TES3_ConsoleLogResult(buffer, false);
			}

			return hitChance;
		}

		__declspec(naked) void patchCalculateHitChance() {
			__asm {
				mov ecx, esi		// Size: 0x2
				mov edx, [ebp+0x8]	// Size: 0x3
				nop // Replaced with a call generation. Can't do so here, because offsets aren't accurate.
				nop // ^
				nop // ^
				nop // ^
				nop // ^
				mov [ebp+0x8], eax	// Size: 0x3?
			}
		}
		const size_t patchCalculateHitChance_size = 0xD;

		//
		// Event: Filters created. 
		//

		const auto TES3_WorldController_CreateFilters = reinterpret_cast<void(__thiscall*)(TES3::WorldController *)>(0x411400);
		void __fastcall CreateFilters(TES3::WorldController * worldController) {
			TES3_WorldController_CreateFilters(worldController);
			mwse::lua::LuaManager::getInstance().triggerEvent(new mwse::lua::event::GenericEvent("fadersCreated"));
		}

		//
		// Events related to soul gems.
		//

		bool __fastcall PatchWriteEnchantSoulData(TES3::ItemStack * stack) {
			return tes3::isSoulGem(stack->object);
		}

		__declspec(naked) void patchSetupWriteEnchantSoulData() {
			__asm {
				mov ecx, [esp + 0x44 + 0x4]		// Size: 0x4
				nop // Replaced with a call generation. Can't do so here, because offsets aren't accurate.
				nop // ^
				nop // ^
				nop // ^
				nop // ^
				test al, al						// Size: 0x2
				jnz $ + 0x7B					// Size: 0x6
			}
		}
		const size_t patchSetupWriteEnchantSoulData_size = 0x11;

		bool __fastcall PatchCheckSoulTrapSoulGem(TES3::Misc * item, TES3::MobileActor * mact) {
			if (!tes3::isSoulGem(item)) {
				return false;
			}

			// Allow lua to dictate what souls are allowed in which soulgems.
			sol::table payload = lua::LuaManager::getInstance().triggerEvent(new lua::event::FilterSoulGemTargetEvent(item, mact));
			if (payload.valid()) {
				sol::object filter = payload["filter"];
				if (filter.is<bool>()) {
					return filter.as<bool>();
				}
			}

			return true;
		}

		__declspec(naked) void patchSetupCheckSoulTrapSoulGem() {
			__asm {
				mov ecx, esi				// Size: 0x2 // Get base item in ecx.
				mov edx, [esp + 0x34 + 0x4]	// Size: 0x4 // Get target mobile actor from stack.
				nop							// Replaced with a call generation. Can't do so here, because offsets aren't accurate.
				nop							// ^
				nop							// ^
				nop							// ^
				nop							// ^
				test al, al					// Size: 0x2
				jnz $ + 0xB					// Size: 0x6
			}
		}
		const size_t patchSetupCheckSoulTrapSoulGem_size = 0x13;

		int __stdcall PatchGetSoulValueForActor(TES3::Actor * actor) {
			int value = 0;

			if (actor->objectType == TES3::ObjectType::Creature) {
				value = static_cast<TES3::Creature*>(actor)->soul;
			}

			// Allow lua to determine the soul's value.
			sol::table payload = lua::LuaManager::getInstance().triggerEvent(new lua::event::CalculateSoulValueEvent(actor, value));
			if (payload.valid()) {
				sol::object eventValue = payload["value"];
				if (eventValue.is<int>()) {
					value = eventValue.as<int>();
				}
			}

			return value;
		}

		int __fastcall PatchGetSoulValueForMobileActor(TES3::MobileActor * mact) {
			return PatchGetSoulValueForActor(static_cast<TES3::CreatureInstance*>(mact->reference->baseObject)->baseCreature);
		}

		__declspec(naked) void patchEnforceSoulValueAboveZero() {
			__asm {
				test eax, eax	// Size: 0x2
				jle $ + 0x3E3	// Size: 0x6
				nop				// Size: 0x1
			}
		}
		const size_t patchEnforceSoulValueAboveZero_size = 0x9;

		int PatchSPrintFSoulValue(char* buffer, const char* format, TES3::Actor * actor) {
			return sprintf(buffer, format, PatchGetSoulValueForActor(actor));
		}

		void __fastcall PatchSetSoulValueProperty(TES3::UI::Element* element, DWORD _UNUSED_, TES3::UI::Property property, TES3::Actor * actor, TES3::UI::PropertyType type) {
			TES3::UI::PropertyValue value;
			value.integerValue = PatchGetSoulValueForActor(actor);
			element->setProperty(property, value, type);
		}

		//
		// Fire an event when item tiles are updated.
		//

		TES3::IteratorNode<TES3::UI::InventoryTile> * __fastcall GetNextInventoryTileToUpdate(TES3::Iterator<TES3::UI::InventoryTile> * iterator) {
			lua::LuaManager::getInstance().triggerEvent(new lua::event::ItemTileUpdatedEvent(iterator->current->data));
			return iterator->getNextNode();
		}

		void __inline TriggerItemTileUpdatedEventForElement(TES3::UI::Element * element, DWORD propertyAddress) {
			TES3::UI::InventoryTile * tile = static_cast<TES3::UI::InventoryTile*>(element->getProperty(TES3::UI::PropertyType::Pointer, *reinterpret_cast<TES3::UI::Property*>(propertyAddress)).ptrValue);
			if (tile) {
				lua::LuaManager::getInstance().triggerEvent(new lua::event::ItemTileUpdatedEvent(tile));
			}
		}

		void __fastcall OnSetItemTileIcon(TES3::UI::Element * element, DWORD _UNUSED_, const char* iconPath) {
			// Overwritten function.
			element->setIcon(iconPath);

			// Get the associated tile and fire off the update event.
			TriggerItemTileUpdatedEventForElement(element, 0x7D3A70);
		}

		void __fastcall OnInventoryTileChildPropertySet(TES3::UI::Element* element, DWORD _UNUSED_, TES3::UI::Property property, TES3::UI::PropertyValue value, TES3::UI::PropertyType type) {
			// Overwritten code.
			element->setProperty(property, value, type);

			// Get the associated tile and fire off the update event.
			TriggerItemTileUpdatedEventForElement(element, 0x7D308C);
		}

		//
		// Handle all the hacks needed to get ItemData transferred over to a bigger structure.
		//

		TES3::ItemData * __fastcall OnDeletingItemData(TES3::ItemData * itemData, DWORD _UNUSED_, bool deleting) {
			TES3::ItemData::dtor(itemData);
			
			if (deleting) {
				tes3::_delete(itemData);
			}

			return itemData;
		}

		__declspec(naked) void patchInlineItemDataCreation() {
			__asm {
				mov ecx, eax	// Size: 0x2
				nop				// Replaced with a call generation. Can't do so here, because offsets aren't accurate.
				nop				// ^
				nop				// ^
				nop				// ^
				nop				// ^
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
			}
		}
		const size_t patchInlineItemDataCreation_size = 0x1C;

		void __inline GeneratePatchForInlineItemDataCreation(DWORD address) {
			writePatchCodeUnprotected(address, (BYTE*)&patchInlineItemDataCreation, patchInlineItemDataCreation_size);
			genCallUnprotected(address + 0x2, reinterpret_cast<DWORD>(&TES3::ItemData::ctor));
		}

		// Requires that the pointer is in EBX
		__declspec(naked) void patchInlineItemDataDestruction() {
			__asm {
				push edx		// Size: 0x1
				push ecx		// Size: 0x1
				mov ecx, ebx	// Size: 0x2
				nop				// Replaced with a call generation. Can't do so here, because offsets aren't accurate.
				nop				// ^
				nop				// ^
				nop				// ^
				nop				// ^ Size: 0x5
				pop ecx			// Size: 0x1
				pop edx			// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
				nop				// Size: 0x1
			}
		}
		const size_t patchInlineItemDataDestruction_size = 0x27;

		void __inline GeneratePatchForInlineItemDataDestruction(DWORD address) {
			writePatchCodeUnprotected(address, (BYTE*)&patchInlineItemDataDestruction, patchInlineItemDataDestruction_size);
			genCallUnprotected(address + 0x4, reinterpret_cast<DWORD>(&TES3::ItemData::dtor));
		}

		//
		// Handle saving of our extra ItemData information.
		//

		// Data we use to keep track of the currently saving item data record.
		TES3::Iterator<TES3::ItemStack> * currentlySavingInventoryIterator = nullptr;
		unsigned int currentlySavingInventoyItemDataIndex = 0;

		// Get a hold of the inventory we're looking at.
		TES3::IteratorNode<TES3::ItemStack> * __fastcall GetFirstSavedItemStack(TES3::Iterator<TES3::ItemStack> * iterator) {
			currentlySavingInventoryIterator = iterator;
			return iterator->getFirstNode();
		}

		// Get a hold of the current index in ItemData storage we're looking at.
		int __fastcall WriteItemDataIndex(TES3::GameFile * gameFile, DWORD _UNUSED_, unsigned int tag, const unsigned int * data, unsigned int size) {
			currentlySavingInventoyItemDataIndex = *data;
			return gameFile->writeChunkData(tag, data, size);
		}

		// The last extra data written. We'll add the lua data here if needed.
		int __fastcall WriteItemDataCondition(TES3::GameFile * gameFile, DWORD _UNUSED_, unsigned int tag, const void * data, unsigned int size) {
			// Overwritten code.
			int result = gameFile->writeChunkData(tag, data, size);

			TES3::ItemData * itemData = currentlySavingInventoryIterator->current->data->variables->storage[currentlySavingInventoyItemDataIndex];
			if (itemData->luaData) {
				sol::table table = itemData->luaData->data;

				// If it is empty, don't bother saving it.
				if (!table.empty()) {
					// Convert the table to json for storage.
					sol::state& state = LuaManager::getInstance().getState();
					std::string json = state["json"]["encode"](table);

					// Call original writechunk function.
					gameFile->writeChunkData('TAUL', json.c_str(), json.length() + 1);
				}
			}

			return result;
		}

		// Get a handle on the last created ItemData when loading.
		std::unordered_map<DWORD, TES3::ItemData*> saveLoadItemDataMap;
		TES3::ItemData * __fastcall CreateItemDataFromLoading(TES3::ItemData * itemData) {
			saveLoadItemDataMap[GetCurrentThreadId()] = itemData;
			return TES3::ItemData::ctor(itemData);
		}

		// When we load a next record while loading an actor, check to see if it's a lua extension and save it to the last loaded ItemData.
		int __fastcall LoadNextRecordForActor(TES3::GameFile * gameFile) {
			int result = gameFile->getNextSubrecord();

			// Load the lua data into the last loaded ItemData.
			if (result == 'TAUL') {
				char * buffer = new char[gameFile->currentChunkHeader.size];
				bool success = gameFile->readChunkData(buffer, gameFile->currentChunkHeader.size);

				// If we for whatever reason failed to load this chunk, bail.
				if (success) {
					// Get our lua table, and replace it with our new table.
					sol::state& state = LuaManager::getInstance().getState();
					auto threadID = GetCurrentThreadId();
					auto saveLoadItemData = saveLoadItemDataMap[threadID];
					if (saveLoadItemData && saveLoadItemData->luaData == nullptr) {
						saveLoadItemData->setLuaDataTable(state["json"]["decode"](buffer));
						saveLoadItemDataMap.erase(threadID);
					}
				}

				// Clean up the buffer we made.
				delete[] buffer;
			}

			// It's safe to return LUAT here, it will just pass to the next load for us.
			return result;
		}

		// Get ItemData for reference being saved.
		TES3::ItemData * __fastcall GetItemDataForReferenceSaving(TES3::Reference * reference) {
			auto itemData = reference->getAttachedItemData();
			saveLoadItemDataMap[GetCurrentThreadId()] = itemData;
			return itemData;
		}

		// The last extra data written for references. We'll add the lua data here if needed.
		int __fastcall WriteReferenceItemDataCondition(TES3::GameFile * gameFile, DWORD _UNUSED_, unsigned int tag, const void * data, unsigned int size) {
			// Overwritten code.
			int result = gameFile->writeChunkData(tag, data, size);

			auto saveLoadItemData = saveLoadItemDataMap[GetCurrentThreadId()];
			if (saveLoadItemData->luaData) {
				sol::table table = saveLoadItemData->luaData->data;

				// If it is empty, don't bother saving it.
				if (!table.empty()) {
					// Convert the table to json for storage.
					sol::state& state = LuaManager::getInstance().getState();
					std::string json = state["json"]["encode"](table);

					// Call original writechunk function.
					gameFile->writeChunkData('TAUL', json.c_str(), json.length() + 1);
				}
			}

			saveLoadItemData = nullptr;
			return result;
		}

		// Get reference that is being loaded.
		std::unordered_map<DWORD, TES3::Reference*> saveLoadReferenceMap;
		TES3::MobileActor * __fastcall LoadReferenceGetMACT(TES3::Reference * reference) {
			saveLoadReferenceMap[GetCurrentThreadId()] = reference;
			return reference->getAttachedMobileActor();
		}

		// When we load a next record while loading an actor, check to see if it's a lua extension and save it to the last loaded ItemData.
		int __fastcall LoadNextRecordForReference(TES3::GameFile * gameFile) {
			int result = gameFile->getNextSubrecord();

			// Load the lua data into the last loaded ItemData.
			if (result == 'TAUL') {
				char * buffer = new char[gameFile->currentChunkHeader.size];
				bool success = gameFile->readChunkData(buffer, gameFile->currentChunkHeader.size);

				// If we for whatever reason failed to load this chunk, bail.
				if (success) {
					// Get our lua table, and replace it with our new table.
					sol::state& state = LuaManager::getInstance().getState();
					auto itemData = saveLoadReferenceMap[GetCurrentThreadId()]->getAttachedItemData();
					if (itemData) {
						if (itemData->luaData == nullptr) {
							itemData->setLuaDataTable(state["json"]["decode"](buffer));
						}
					}
#if _DEBUG
					else {
						mwse::log::getLog() << "WARNING: Loading reference didn't have a condition attachment!" << std::endl;
					}
#endif
				}

				// Clean up the buffer we made.
				delete[] buffer;

				if (gameFile->hasNextSubrecord()) {
					result = gameFile->getNextSubrecord();
				}
				else {
					result = 0;
				}
			}

			// It's safe to return LUAT here, it will just pass to the next load for us.
			return result;
		}

		//
		//
		//

		sol::state& LuaManager::getState() {
#if _DEBUG
			auto dataHandler = TES3::DataHandler::get();
			if (dataHandler != nullptr && dataHandler->mainThreadID != GetCurrentThreadId()) {
				throw std::exception("Cannot be called from outside the main thread.");
			}
#endif

			return luaState;
		}

		sol::table LuaManager::createTable() {
#if _DEBUG
			auto dataHandler = TES3::DataHandler::get();
			if (dataHandler != nullptr && dataHandler->mainThreadID != GetCurrentThreadId()) {
				throw std::exception("Cannot be called from outside the main thread.");
			}
#endif
			return luaState.create_table();
		}

		void LuaManager::hook() {
			// Execute mwse_init.lua
			sol::protected_function_result result = luaState.safe_script_file("Data Files/MWSE/core/mwse_init.lua");
			if (!result.valid()) {
				sol::error error = result;
				log::getLog() << "[LuaManager] ERROR: Failed to initialize MWSE Lua interface." << std::endl << error.what() << std::endl;
				return;
			}

			// Bind libraries.
			bindMWSEStack();
			bindScriptUtil();
			bindStringUtil();
			bindTES3Util();

			// Hook the RunScript function so we can intercept Lua scripts and invoke Lua code if needed.
			genJumpUnprotected(TES3_HOOK_RUNSCRIPT_LUACHECK, reinterpret_cast<DWORD>(HookRunScript), TES3_HOOK_RUNSCRIPT_LUACHECK_SIZE);

			// Hook the MACP creation functions to update lua variables that point to the player.
			genCallEnforced(0x5635D6, 0x56EAE0, reinterpret_cast<DWORD>(OnPlayerRecreated));

			// Event: initialized. Hook just before we return successfully from where game data is loaded.
			genJumpUnprotected(TES3_HOOK_FINISH_INITIALIZATION, reinterpret_cast<DWORD>(HookFinishInitialization), TES3_HOOK_FINISH_INITIALIZATION_SIZE);

			// Event: enterFrame. This hook can be in a couple of locations, because of MCP.
			genCallEnforced(0x41ABB0, 0x40F610, reinterpret_cast<DWORD>(EnterFrame));
			genCallEnforced(0x41ABA5, 0x40F610, reinterpret_cast<DWORD>(EnterFrame));

			// Event: equip.
			genCallEnforced(0x5CB8E7, 0x5CE130, reinterpret_cast<DWORD>(OnPCEquip));
			genCallEnforced(0x5D11D9, 0x5CE130, reinterpret_cast<DWORD>(OnPCEquip));
			genCallEnforced(0x60E70F, 0x5CE130, reinterpret_cast<DWORD>(OnPCEquip));
			genCallEnforced(0x60E9BE, 0x5CE130, reinterpret_cast<DWORD>(OnPCEquip));

			// Event: equipped.
			genCallEnforced(0x49F053, 0x4958B0, reinterpret_cast<DWORD>(OnEquipped));
			genCallEnforced(0x4D9C66, 0x4958B0, reinterpret_cast<DWORD>(OnEquipped));
			genCallEnforced(0x4D9D90, 0x4958B0, reinterpret_cast<DWORD>(OnEquipped));
			genCallEnforced(0x528412, 0x4958B0, reinterpret_cast<DWORD>(OnEquipped));
			genCallEnforced(0x52C7F5, 0x4958B0, reinterpret_cast<DWORD>(OnEquipped));
			genCallEnforced(0x52C813, 0x4958B0, reinterpret_cast<DWORD>(OnEquipped));
			genCallEnforced(0x52C83C, 0x4958B0, reinterpret_cast<DWORD>(OnEquipped));
			genCallEnforced(0x52CC85, 0x4958B0, reinterpret_cast<DWORD>(OnEquipped));
			genCallEnforced(0x52CE43, 0x4958B0, reinterpret_cast<DWORD>(OnEquipped));
			genCallEnforced(0x52D0B9, 0x4958B0, reinterpret_cast<DWORD>(OnEquipped));
			genCallEnforced(0x54DDE8, 0x4958B0, reinterpret_cast<DWORD>(OnEquipped));
			genCallEnforced(0x5CEEB9, 0x4958B0, reinterpret_cast<DWORD>(OnEquipped));
			genCallEnforced(0x5CF3DB, 0x4958B0, reinterpret_cast<DWORD>(OnEquipped));
			genCallEnforced(0x5CF788, 0x4958B0, reinterpret_cast<DWORD>(OnEquipped));
			genCallEnforced(0x5CF825, 0x4958B0, reinterpret_cast<DWORD>(OnEquipped));
			genCallEnforced(0x5CF89F, 0x4958B0, reinterpret_cast<DWORD>(OnEquipped));
			genCallEnforced(0x5D00D6, 0x4958B0, reinterpret_cast<DWORD>(OnEquipped));
			genCallEnforced(0x5D048E, 0x4958B0, reinterpret_cast<DWORD>(OnEquipped));
			genCallEnforced(0x5D1468, 0x4958B0, reinterpret_cast<DWORD>(OnEquipped));

			// Event: unequipped.
			genCallEnforced(0x46089D, 0x496710, reinterpret_cast<DWORD>(OnUnequipped));
			genCallEnforced(0x460B0F, 0x496710, reinterpret_cast<DWORD>(OnUnequipped));
			genCallEnforced(0x464D99, 0x496710, reinterpret_cast<DWORD>(OnUnequipped));
			genCallEnforced(0x465732, 0x496710, reinterpret_cast<DWORD>(OnUnequipped));
			genCallEnforced(0x495954, 0x496710, reinterpret_cast<DWORD>(OnUnequipped));
			genCallEnforced(0x495B32, 0x496710, reinterpret_cast<DWORD>(OnUnequipped));
			genCallEnforced(0x495BA8, 0x496710, reinterpret_cast<DWORD>(OnUnequipped));
			genCallEnforced(0x495C46, 0x496710, reinterpret_cast<DWORD>(OnUnequipped));
			genCallEnforced(0x495DA5, 0x496710, reinterpret_cast<DWORD>(OnUnequipped));
			genCallEnforced(0x495ED9, 0x496710, reinterpret_cast<DWORD>(OnUnequipped));
			genCallEnforced(0x495F7B, 0x496710, reinterpret_cast<DWORD>(OnUnequipped));
			genCallEnforced(0x496062, 0x496710, reinterpret_cast<DWORD>(OnUnequipped));
			genCallEnforced(0x4960E7, 0x496710, reinterpret_cast<DWORD>(OnUnequipped));
			genCallEnforced(0x49615E, 0x496710, reinterpret_cast<DWORD>(OnUnequipped));
			genCallEnforced(0x496254, 0x496710, reinterpret_cast<DWORD>(OnUnequipped));
			genCallEnforced(0x4962A7, 0x496710, reinterpret_cast<DWORD>(OnUnequipped));
			genCallEnforced(0x496350, 0x496710, reinterpret_cast<DWORD>(OnUnequipped));
			genCallEnforced(0x496620, 0x496710, reinterpret_cast<DWORD>(OnUnequipped));
			genCallEnforced(0x4966AF, 0x496710, reinterpret_cast<DWORD>(OnUnequipped));
			genCallEnforced(0x5150FD, 0x496710, reinterpret_cast<DWORD>(OnUnequipped));
			genCallEnforced(0x525042, 0x496710, reinterpret_cast<DWORD>(OnUnequipped));
			genCallEnforced(0x52518F, 0x496710, reinterpret_cast<DWORD>(OnUnequipped));
			genCallEnforced(0x5282F4, 0x496710, reinterpret_cast<DWORD>(OnUnequipped));
			genCallEnforced(0x5283C1, 0x496710, reinterpret_cast<DWORD>(OnUnequipped));
			genCallEnforced(0x52C6B9, 0x496710, reinterpret_cast<DWORD>(OnUnequipped));
			genCallEnforced(0x541087, 0x496710, reinterpret_cast<DWORD>(OnUnequipped));
			genCallEnforced(0x54DA4B, 0x496710, reinterpret_cast<DWORD>(OnUnequipped));
			genCallEnforced(0x558472, 0x496710, reinterpret_cast<DWORD>(OnUnequipped));
			genCallEnforced(0x5586F6, 0x496710, reinterpret_cast<DWORD>(OnUnequipped));
			genCallEnforced(0x569CFB, 0x496710, reinterpret_cast<DWORD>(OnUnequipped));
			genCallEnforced(0x56A914, 0x496710, reinterpret_cast<DWORD>(OnUnequipped));
			genCallEnforced(0x56AF4D, 0x496710, reinterpret_cast<DWORD>(OnUnequipped));
			genCallEnforced(0x5B521D, 0x496710, reinterpret_cast<DWORD>(OnUnequipped));
			genCallEnforced(0x5B5A1F, 0x496710, reinterpret_cast<DWORD>(OnUnequipped));
			genCallEnforced(0x5D09F0, 0x496710, reinterpret_cast<DWORD>(OnUnequipped));
			genCallEnforced(0x5D0B4B, 0x496710, reinterpret_cast<DWORD>(OnUnequipped));
			genCallEnforced(0x5D0C54, 0x496710, reinterpret_cast<DWORD>(OnUnequipped));

			// Event: activate.
			genCallEnforced(0x41CCC8, 0x4E9610, reinterpret_cast<DWORD>(OnActivate));
			genCallEnforced(0x507298, 0x4E9610, reinterpret_cast<DWORD>(OnActivate));
			genCallEnforced(0x5364CD, 0x4E9610, reinterpret_cast<DWORD>(OnActivate));
			genCallEnforced(0x551002, 0x4E9610, reinterpret_cast<DWORD>(OnActivate));
			genCallEnforced(0x59051F, 0x4E9610, reinterpret_cast<DWORD>(OnActivate));
			genCallEnforced(0x5ADB6E, 0x4E9610, reinterpret_cast<DWORD>(OnActivate));
			genCallEnforced(0x613CC9, 0x4E9610, reinterpret_cast<DWORD>(OnActivate));

			// Event: save/saved.
			genCallEnforced(0x41B100, 0x4C4250, reinterpret_cast<DWORD>(OnSave));
			genCallEnforced(0x476F58, 0x4C4250, reinterpret_cast<DWORD>(OnSave));
			genCallEnforced(0x5C8EDB, 0x4C4250, reinterpret_cast<DWORD>(OnSave));
			genCallEnforced(0x610578, 0x4C4250, reinterpret_cast<DWORD>(OnSave));
			genCallEnforced(0x6106BE, 0x4C4250, reinterpret_cast<DWORD>(OnSave));
			genCallEnforced(0x6108B8, 0x4C4250, reinterpret_cast<DWORD>(OnSave));
			genCallEnforced(0x611B69, 0x4C4250, reinterpret_cast<DWORD>(OnSave));

			// Event: load/loaded.
			genCallEnforced(0x41B262, 0x4C4800, reinterpret_cast<DWORD>(OnLoad));
			genCallEnforced(0x5DD3C9, 0x4C4800, reinterpret_cast<DWORD>(OnLoad));
			genCallEnforced(0x5DD59F, 0x4C4800, reinterpret_cast<DWORD>(OnLoad));
			genCallEnforced(0x5FB629, 0x4C4800, reinterpret_cast<DWORD>(OnLoad));

			// Additional load/loaded event for loading from the main menu.
			genCallEnforced(0x5DD3B6, 0x4C4EB0, reinterpret_cast<DWORD>(OnLoadMainMenu));

			// Additional load/loaded events for new game.
			genCallEnforced(0x5FCCF4, 0x5FAEA0, reinterpret_cast<DWORD>(OnNewGame));
			genCallEnforced(0x5FCDAA, 0x5FAEA0, reinterpret_cast<DWORD>(OnNewGame));
			genCallEnforced(0x41A6E4, 0x563CE0, reinterpret_cast<DWORD>(OnNewGameViaStartingCell));

			// Event: Start Combat
			genCallEnforced(0x5073BC, 0x530470, reinterpret_cast<DWORD>(OnStartCombat));
			genCallEnforced(0x52FC2F, 0x530470, reinterpret_cast<DWORD>(OnStartCombat));
			genCallEnforced(0x530443, 0x530470, reinterpret_cast<DWORD>(OnStartCombat));
			genCallEnforced(0x5563F5, 0x530470, reinterpret_cast<DWORD>(OnStartCombat));
			genCallEnforced(0x5564C2, 0x530470, reinterpret_cast<DWORD>(OnStartCombat));
			genCallEnforced(0x558FE7, 0x530470, reinterpret_cast<DWORD>(OnStartCombat));
			genCallEnforced(0x55905E, 0x530470, reinterpret_cast<DWORD>(OnStartCombat));

			// Event: Stop Combat
			genCallEnforced(0x523BBF, 0x558720, reinterpret_cast<DWORD>(OnStopCombat));
			genCallEnforced(0x54E04D, 0x558720, reinterpret_cast<DWORD>(OnStopCombat));
			genCallEnforced(0x556524, 0x558720, reinterpret_cast<DWORD>(OnStopCombat));
			genCallEnforced(0x556568, 0x558720, reinterpret_cast<DWORD>(OnStopCombat));
			genCallEnforced(0x557CBC, 0x558720, reinterpret_cast<DWORD>(OnStopCombat));
			genCallEnforced(0x55916F, 0x558720, reinterpret_cast<DWORD>(OnStopCombat));
			genCallEnforced(0x559265, 0x558720, reinterpret_cast<DWORD>(OnStopCombat));
			genCallEnforced(0x5650F2, 0x558720, reinterpret_cast<DWORD>(OnStopCombat));
			genCallEnforced(0x568794, 0x558720, reinterpret_cast<DWORD>(OnStopCombat));
			genCallEnforced(0x461881, 0x558720, reinterpret_cast<DWORD>(OnStopCombat));
			genCallEnforced(0x461A61, 0x558720, reinterpret_cast<DWORD>(OnStopCombat));
			genJumpEnforced(0x7365E9, 0x558720, reinterpret_cast<DWORD>(OnStopCombat));

			// Event: Melee Hit Check
			genCallEnforced(0x541489, TES3_ActorAnimData_attackCheckMeleeHit, reinterpret_cast<DWORD>(OnAttack));
			genCallEnforced(0x5414CD, TES3_ActorAnimData_attackCheckMeleeHit, reinterpret_cast<DWORD>(OnAttack));
			genCallEnforced(0x569E78, TES3_ActorAnimData_attackCheckMeleeHit, reinterpret_cast<DWORD>(OnAttack));

			// Collision events: Mobile Object
#if false
			overrideVirtualTableEnforced(0x74B0C0, 0x80, 0x5615A0, reinterpret_cast<DWORD>(OnMobileObjectActorCollision));
			overrideVirtualTableEnforced(0x74B0C0, 0x84, 0x5615C0, reinterpret_cast<DWORD>(OnMobileObjectObjectCollision));
			overrideVirtualTableEnforced(0x74B0C0, 0x88, 0x5615E0, reinterpret_cast<DWORD>(OnMobileObjectTerrainCollision));
			overrideVirtualTableEnforced(0x74B0C0, 0x8C, 0x5615E0, reinterpret_cast<DWORD>(OnMobileObjectWaterCollision));
			overrideVirtualTableEnforced(0x74B0C0, 0x90, 0x561600, reinterpret_cast<DWORD>(OnMobileObjectActivatorCollision));
#endif

			// Collision events: Mobile Actor
#if false
			overrideVirtualTableEnforced(0x74AB4C, 0x80, 0x5234A0, reinterpret_cast<DWORD>(OnMobileActorActorCollision));
			overrideVirtualTableEnforced(0x74AB4C, 0x84, 0x5233B0, reinterpret_cast<DWORD>(OnMobileActorObjectCollision));
			overrideVirtualTableEnforced(0x74AB4C, 0x88, 0x523310, reinterpret_cast<DWORD>(OnMobileActorTerrainCollision));
			overrideVirtualTableEnforced(0x74AB4C, 0x8C, 0x5615E0, reinterpret_cast<DWORD>(OnMobileObjectWaterCollision));
			overrideVirtualTableEnforced(0x74AB4C, 0x90, 0x523590, reinterpret_cast<DWORD>(OnMobileActorActivatorCollision));
#endif

			// Collision events: Mobile Creature
#if false
			overrideVirtualTableEnforced(0x74AFA4, 0x80, 0x5234A0, reinterpret_cast<DWORD>(OnMobileActorActorCollision));
			overrideVirtualTableEnforced(0x74AFA4, 0x84, 0x5233B0, reinterpret_cast<DWORD>(OnMobileActorObjectCollision));
			overrideVirtualTableEnforced(0x74AFA4, 0x88, 0x523310, reinterpret_cast<DWORD>(OnMobileActorTerrainCollision));
			overrideVirtualTableEnforced(0x74AFA4, 0x8C, 0x5615E0, reinterpret_cast<DWORD>(OnMobileObjectWaterCollision));
			overrideVirtualTableEnforced(0x74AFA4, 0x90, 0x523590, reinterpret_cast<DWORD>(OnMobileActorActivatorCollision));
#endif

			// Collision events: Mobile NPC
#if false
			overrideVirtualTableEnforced(0x74AE6C, 0x80, 0x5234A0, reinterpret_cast<DWORD>(OnMobileActorActorCollision));
			overrideVirtualTableEnforced(0x74AE6C, 0x84, 0x5233B0, reinterpret_cast<DWORD>(OnMobileActorObjectCollision));
			overrideVirtualTableEnforced(0x74AE6C, 0x88, 0x523310, reinterpret_cast<DWORD>(OnMobileActorTerrainCollision));
			overrideVirtualTableEnforced(0x74AE6C, 0x8C, 0x5615E0, reinterpret_cast<DWORD>(OnMobileObjectWaterCollision));
			overrideVirtualTableEnforced(0x74AE6C, 0x90, 0x523590, reinterpret_cast<DWORD>(OnMobileActorActivatorCollision));
#endif

			// Collision events: Mobile Player
#if false
			overrideVirtualTableEnforced(0x74B174, 0x80, 0x5234A0, reinterpret_cast<DWORD>(OnMobileActorActorCollision));
			overrideVirtualTableEnforced(0x74B174, 0x84, 0x5233B0, reinterpret_cast<DWORD>(OnMobileActorObjectCollision));
			overrideVirtualTableEnforced(0x74B174, 0x88, 0x523310, reinterpret_cast<DWORD>(OnMobileActorTerrainCollision));
			overrideVirtualTableEnforced(0x74B174, 0x8C, 0x5615E0, reinterpret_cast<DWORD>(OnMobileObjectWaterCollision));
			overrideVirtualTableEnforced(0x74B174, 0x90, 0x523590, reinterpret_cast<DWORD>(OnMobileActorActivatorCollision));
#endif

			// Collision events: Mobile Projectile
			overrideVirtualTableEnforced(0x74B2B4, 0x80, 0x573860, reinterpret_cast<DWORD>(OnMobileProjectileActorCollision));
#if false
			overrideVirtualTableEnforced(0x74B2B4, 0x84, 0x573820, reinterpret_cast<DWORD>(OnMobileProjectileObjectCollision));
			overrideVirtualTableEnforced(0x74B2B4, 0x88, 0x5737F0, reinterpret_cast<DWORD>(OnMobileProjectileTerrainCollision));
			overrideVirtualTableEnforced(0x74B2B4, 0x8C, 0x573790, reinterpret_cast<DWORD>(OnMobileProjectileWaterCollision));
#endif

			// Mobile Projectile Expire
			genCallEnforced(0x57548A, 0x5637F0, reinterpret_cast<DWORD>(OnProjectileExpire));

			// Event: UI Event
			genJumpUnprotected(TES3_HOOK_UI_EVENT, reinterpret_cast<DWORD>(HookUIEvent), TES3_HOOK_UI_EVENT_SIZE);

			// Event: Show Rest/Wait Menu
			genCallEnforced(0x41ADB6, 0x610170, reinterpret_cast<DWORD>(OnShowRestWaitMenu));
			genCallEnforced(0x5097BA, 0x610170, reinterpret_cast<DWORD>(OnShowRestWaitMenuScripted));
			genCallEnforced(0x5097DD, 0x610170, reinterpret_cast<DWORD>(OnShowRestWaitMenuScripted));

			// Event: Key
			genCallEnforced(0x40F633, 0x4065E0, reinterpret_cast<DWORD>(OnKeyReadState));
			genCallEnforced(0x736C04, 0x4065E0, reinterpret_cast<DWORD>(OnKeyReadState));

			// Event: Dying/death.
			overrideVirtualTable(0x74AB4C, 0xB8, reinterpret_cast<DWORD>(OnMobileActorDeath)); // MACT
			overrideVirtualTable(0x74AFA4, 0xB8, reinterpret_cast<DWORD>(OnMobileActorDeath)); // MACR
			overrideVirtualTable(0x74AE6C, 0xB8, reinterpret_cast<DWORD>(OnMobileActorDeath)); // MACH
			overrideVirtualTable(0x74B174, 0xB8, reinterpret_cast<DWORD>(OnMobilePlayerDeath)); // MACP

			// Event: Damage(d)
			genCallEnforced(0x50B72C, 0x557CF0, reinterpret_cast<DWORD>(OnApplyDamage));
			genCallEnforced(0x50B7AB, 0x557CF0, reinterpret_cast<DWORD>(OnApplyDamage));
			genCallEnforced(0x50D138, 0x557CF0, reinterpret_cast<DWORD>(OnApplyDamage));
			genCallEnforced(0x50D1B4, 0x557CF0, reinterpret_cast<DWORD>(OnApplyDamage));
			genCallEnforced(0x524884, 0x557CF0, reinterpret_cast<DWORD>(OnApplyDamage));
			genCallEnforced(0x52978F, 0x557CF0, reinterpret_cast<DWORD>(OnApplyDamage));
			genCallEnforced(0x5299CB, 0x557CF0, reinterpret_cast<DWORD>(OnApplyDamage));
			genCallEnforced(0x555789, 0x557CF0, reinterpret_cast<DWORD>(OnApplyDamage));
			genCallEnforced(0x556AE0, 0x557CF0, reinterpret_cast<DWORD>(OnApplyDamage));
			genCallEnforced(0x55782C, 0x557CF0, reinterpret_cast<DWORD>(OnApplyDamage));

			// Event: Spell cast resolution
			genCallEnforced(0x5156B2, 0x4AA950, reinterpret_cast<DWORD>(OnSpellCastResolution));

			// Event: Magic cast success
			genJumpUnprotected(TES3_HOOK_MAGIC_CAST_SUCCESS, reinterpret_cast<DWORD>(HookMagicCastSuccess), TES3_HOOK_MAGIC_CAST_SUCCESS_SIZE);

			// Event: Spell cast failure
			genJumpUnprotected(TES3_HOOK_SPELL_CAST_FAILURE, reinterpret_cast<DWORD>(HookSpellCastFailure), TES3_HOOK_SPELL_CAST_FAILURE_SIZE);

			// Event: Spell tick.
			patchMagicEffectDispatch(0x4647E9, 0x50);
			patchMagicEffectDispatch(0x464A56, 0x50);
			patchMagicEffectDispatch(0x515AB1, 0x51);

			// Event: Spell Resist
			genCallEnforced(0x518616, 0x517E40, reinterpret_cast<DWORD>(OnSpellResist));

			// Event: Player exercise skill.
			genCallEnforced(0x4EB387, 0x56A5D0, reinterpret_cast<DWORD>(OnExerciseSkill));
			genCallEnforced(0x4EB586, 0x56A5D0, reinterpret_cast<DWORD>(OnExerciseSkill));
			genCallEnforced(0x5154EE, 0x56A5D0, reinterpret_cast<DWORD>(OnExerciseSkill));
			genCallEnforced(0x5158E3, 0x56A5D0, reinterpret_cast<DWORD>(OnExerciseSkill));
			genCallEnforced(0x5259AD, 0x56A5D0, reinterpret_cast<DWORD>(OnExerciseSkill));
			genCallEnforced(0x52985A, 0x56A5D0, reinterpret_cast<DWORD>(OnExerciseSkill));
			genCallEnforced(0x52CFB9, 0x56A5D0, reinterpret_cast<DWORD>(OnExerciseSkill));
			genCallEnforced(0x54DB5A, 0x56A5D0, reinterpret_cast<DWORD>(OnExerciseSkill));
			genCallEnforced(0x5568BE, 0x56A5D0, reinterpret_cast<DWORD>(OnExerciseSkill));
			genCallEnforced(0x556CA8, 0x56A5D0, reinterpret_cast<DWORD>(OnExerciseSkill));
			genCallEnforced(0x55771B, 0x56A5D0, reinterpret_cast<DWORD>(OnExerciseSkill));
			genCallEnforced(0x59D187, 0x56A5D0, reinterpret_cast<DWORD>(OnExerciseSkill));
			genCallEnforced(0x5A6777, 0x56A5D0, reinterpret_cast<DWORD>(OnExerciseSkill));
			genCallEnforced(0x5B5971, 0x56A5D0, reinterpret_cast<DWORD>(OnExerciseSkill));
			genCallEnforced(0x5C4CB6, 0x56A5D0, reinterpret_cast<DWORD>(OnExerciseSkill));
			genCallEnforced(0x5D1EA8, 0x56A5D0, reinterpret_cast<DWORD>(OnExerciseSkill));
			genCallEnforced(0x6003B5, 0x56A5D0, reinterpret_cast<DWORD>(OnExerciseSkill));
			genCallEnforced(0x600427, 0x56A5D0, reinterpret_cast<DWORD>(OnExerciseSkill));
			genCallEnforced(0x60049C, 0x56A5D0, reinterpret_cast<DWORD>(OnExerciseSkill));
			genCallEnforced(0x6004CD, 0x56A5D0, reinterpret_cast<DWORD>(OnExerciseSkill));
			genCallEnforced(0x60E81C, 0x56A5D0, reinterpret_cast<DWORD>(OnExerciseSkill));
			genCallEnforced(0x60ECB2, 0x56A5D0, reinterpret_cast<DWORD>(OnExerciseSkill));

			// Event: Brew potion.
			genCallEnforced(0x59D2A9, 0x6313E0, reinterpret_cast<DWORD>(OnBrewPotion));

			// Event: Player leveled.
			genCallEnforced(0x5DA620, 0x626220, reinterpret_cast<DWORD>(OnLevelUp));

			// Event: Calculate movement speed. Called after the below speed events.
			auto calculateMoveSpeed = &TES3::ActorAnimationData::calculateMovementSpeed;
			genCallEnforced(0x53E2F2, 0x53E1A0, *reinterpret_cast<DWORD*>(&calculateMoveSpeed));
			genCallEnforced(0x53ED52, 0x53E1A0, *reinterpret_cast<DWORD*>(&calculateMoveSpeed));
			genCallEnforced(0x540C7D, 0x53E1A0, *reinterpret_cast<DWORD*>(&calculateMoveSpeed));
			genCallEnforced(0x55968B, 0x53E1A0, *reinterpret_cast<DWORD*>(&calculateMoveSpeed));

			// Event: Calculate walk speed.
			auto calculateCreatureWalkSpeed = &TES3::MobileCreature::calculateWalkSpeed;
			auto calculateNPCWalkSpeed = &TES3::MobileNPC::calculateWalkSpeed;
			overrideVirtualTableEnforced(0x74AFA4, 0xB4, 0x55AAF0, *reinterpret_cast<DWORD*>(&calculateCreatureWalkSpeed));
			overrideVirtualTableEnforced(0x74AB4C, 0xB4, 0x526F70, *reinterpret_cast<DWORD*>(&calculateNPCWalkSpeed));
			overrideVirtualTableEnforced(0x74AE6C, 0xB4, 0x526F70, *reinterpret_cast<DWORD*>(&calculateNPCWalkSpeed));
			overrideVirtualTableEnforced(0x74B174, 0xB4, 0x526F70, *reinterpret_cast<DWORD*>(&calculateNPCWalkSpeed));

			// Event: Calculate run speed.
			auto calculateRunSpeed = &TES3::MobileActor::calculateRunSpeed;
			genCallEnforced(0x53E23B, 0x527050, *reinterpret_cast<DWORD*>(&calculateRunSpeed));
			genCallEnforced(0x548D95, 0x527050, *reinterpret_cast<DWORD*>(&calculateRunSpeed));

			// Event: Calculate swim speed.
			auto calculateSwimSpeed = &TES3::MobileActor::calculateSwimSpeed;
			genCallEnforced(0x53E227, 0x5270B0, *reinterpret_cast<DWORD*>(&calculateSwimSpeed));
			genCallEnforced(0x540BB3, 0x5270B0, *reinterpret_cast<DWORD*>(&calculateSwimSpeed));
			genCallEnforced(0x548D87, 0x5270B0, *reinterpret_cast<DWORD*>(&calculateSwimSpeed));

			// Event: Calculate swim "run" speed.
			auto calculateSwimRunSpeed = &TES3::MobileActor::calculateSwimRunSpeed;
			genCallEnforced(0x53E21D, 0x527130, *reinterpret_cast<DWORD*>(&calculateSwimRunSpeed));
			genCallEnforced(0x548D7F, 0x527130, *reinterpret_cast<DWORD*>(&calculateSwimRunSpeed));

			// Event: Calculate fly speed.
			auto calculateFlySpeed = &TES3::MobileActor::calculateFlySpeed;
			genCallEnforced(0x53E202, 0x5271F0, *reinterpret_cast<DWORD*>(&calculateFlySpeed));
			genCallEnforced(0x548D6A, 0x5271F0, *reinterpret_cast<DWORD*>(&calculateFlySpeed));

			// Event: Interrupt Rest Check
			genCallEnforced(0x610630, 0x634B90, reinterpret_cast<DWORD>(OnInterruptRestCheck));
			genCallEnforced(0x610776, 0x634B90, reinterpret_cast<DWORD>(OnInterruptRestCheck));
			genCallEnforced(0x610970, 0x634B90, reinterpret_cast<DWORD>(OnInterruptRestCheck));

			// Event: Interrupt Rest
			genCallEnforced(0x635236, 0x4CF870, reinterpret_cast<DWORD>(OnInterruptRest));

			// Event: topicAdded
			genCallEnforced(0x56A4FA, 0x47E4D0, reinterpret_cast<DWORD>(OnAddTopicAtIndex));
			genCallEnforced(0x56A513, 0x47E4D0, reinterpret_cast<DWORD>(OnAddTopicAtIndex));
			genCallEnforced(0x56A4E1, 0x47E360, reinterpret_cast<DWORD>(OnAddTopic));

			// Event: Journal Update
			genCallEnforced(0x5052B1, 0x4B2F80, reinterpret_cast<DWORD>(OnJournalAdd));
			genCallEnforced(0x508894, 0x4B2F80, reinterpret_cast<DWORD>(OnJournalAdd));
			genCallEnforced(0x5088E9, 0x50F8B0, reinterpret_cast<DWORD>(OnJournalSet));

			// Event: UI Created
			genCallEnforced(0x59DF2C, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuAlchemy
			genCallEnforced(0x59F690, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuAttributes
			genCallEnforced(0x5A0604, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuAttributesList
			genCallEnforced(0x5A1AB7, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuAudio
			genCallEnforced(0x5A3DC4, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuBarter
			genCallEnforced(0x5AABBB, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuBirthSign
			genCallEnforced(0x5AC433, 0x581F30, reinterpret_cast<DWORD>(OnUICreatedAfterSetProperty)); // MenuBook
			genCallEnforced(0x5AFB92, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuChooseClass
			genCallEnforced(0x5B08E8, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuClassChoice
			genCallEnforced(0x5B15AB, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuClassMessage
			genCallEnforced(0x5B2358, 0x5B23E0, reinterpret_cast<DWORD>(OnUICreatedAfterConsoleMenuFocus)); // MenuConsole
			genCallEnforced(0x5B2E35, 0x595350, reinterpret_cast<DWORD>(OnUICreatedAfterConsoleMenuShown)); // MenuConsole
			genCallEnforced(0x5B4696, 0x582F20, reinterpret_cast<DWORD>(OnUICreatedAfterMenuVisibleEvent)); // MenuContents
			genCallEnforced(0x5BA6E7, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuCreateClass
			genCallEnforced(0x5BC460, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuCtrls
			genCallEnforced(0x5BC70F, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuCtrls
			genCallEnforced(0x5BE59E, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuDialog
			genCallEnforced(0x5C2A25, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuEnchantment
			genCallEnforced(0x5C7F10, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuInput
			genCallEnforced(0x5C8D3B, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuInputSave
			genCallEnforced(0x5CAA92, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuInventory
			genCallEnforced(0x5D37EE, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuInventorySelect
			genCallEnforced(0x5D65E5, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuJournal
			genCallEnforced(0x5DA181, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuLevelUp
			genCallEnforced(0x5DD0B3, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuLoad
			genCallEnforced(0x5DEE78, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuLoading
			genCallEnforced(0x5E138D, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuMagic
			genCallEnforced(0x5E1535, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuMagic
			genCallEnforced(0x5E1550, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuMagic
			genCallEnforced(0x5E75C6, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuMagicSelect
			genCallEnforced(0x5E9789, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuMap
			genCallEnforced(0x5F0A97, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuMapNoteEdit
			genCallEnforced(0x5F1ED6, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuMessage
			genCallEnforced(0x5F4823, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuMulti
			genCallEnforced(0x5F8634, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuName
			genCallEnforced(0x5FCA28, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuOptions
			genCallEnforced(0x5FF250, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuOptions
			genCallEnforced(0x600174, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuPersuasion
			genCallEnforced(0x601D3A, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuPrefs
			genCallEnforced(0x60352C, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuQuantity
			genCallEnforced(0x603777, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuQuantity
			genCallEnforced(0x6074E8, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuQuick
			genCallEnforced(0x60AD51, 0x649E40, reinterpret_cast<DWORD>(OnUICreatedAfterScrollPaneUpdate)); // MenuRaceSex
			genCallEnforced(0x60DCC6, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuRepair
			genCallEnforced(0x60E0FE, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuRepair
			genCallEnforced(0x61027D, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuRestWait
			genCallEnforced(0x6115F1, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuSave
			genCallEnforced(0x613990, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuScroll
			genCallEnforced(0x615479, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuServiceRepair
			genCallEnforced(0x61660B, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuServiceSpells
			genCallEnforced(0x6180DB, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuServiceTraining
			genCallEnforced(0x619580, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuServiceTravel
			genCallEnforced(0x61B862, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuSetValues
			genCallEnforced(0x61DBEE, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuSkills
			genCallEnforced(0x61EB12, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuSkillsList
			genCallEnforced(0x61F810, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuSpecialization
			genCallEnforced(0x62150B, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuSpellmaking
			genCallEnforced(0x626270, 0x649E40, reinterpret_cast<DWORD>(OnUICreatedAfterScrollPaneUpdate)); // MenuStat
			genCallEnforced(0x626698, 0x649E40, reinterpret_cast<DWORD>(OnUICreatedAfterScrollPaneUpdate)); // MenuStat
			genCallEnforced(0x6266B6, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuStat
			genCallEnforced(0x62CD76, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuStatReview
			genCallEnforced(0x62CF1A, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuStatReview
			genCallEnforced(0x630097, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuSwimFillBar
			genCallEnforced(0x634D2B, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuTimePass
			genCallEnforced(0x63610A, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuTopic
			genCallEnforced(0x637FE6, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuVideo

			// Event: UI Tooltip post-creation.
			genCallEnforced(0x41CC2E, 0x590D90, reinterpret_cast<DWORD>(OnUIObjectTooltip));
			genCallEnforced(0x58FF1D, 0x590D90, reinterpret_cast<DWORD>(OnUIObjectTooltip));
			genCallEnforced(0x59E10D, 0x590D90, reinterpret_cast<DWORD>(OnUIObjectTooltip));
			genCallEnforced(0x5A7633, 0x590D90, reinterpret_cast<DWORD>(OnUIObjectTooltip));
			genCallEnforced(0x5B6FD1, 0x590D90, reinterpret_cast<DWORD>(OnUIObjectTooltip));
			genCallEnforced(0x5C663C, 0x590D90, reinterpret_cast<DWORD>(OnUIObjectTooltip));
			genCallEnforced(0x5CE054, 0x590D90, reinterpret_cast<DWORD>(OnUIObjectTooltip));
			genCallEnforced(0x5CE071, 0x590D90, reinterpret_cast<DWORD>(OnUIObjectTooltip));
			genCallEnforced(0x5D24B2, 0x590D90, reinterpret_cast<DWORD>(OnUIObjectTooltip));
			genCallEnforced(0x5D4B5C, 0x590D90, reinterpret_cast<DWORD>(OnUIObjectTooltip));
			genCallEnforced(0x5E4FAD, 0x590D90, reinterpret_cast<DWORD>(OnUIObjectTooltip));
			genCallEnforced(0x5E802D, 0x590D90, reinterpret_cast<DWORD>(OnUIObjectTooltip));
			genCallEnforced(0x5F6F78, 0x590D90, reinterpret_cast<DWORD>(OnUIObjectTooltip));
			genCallEnforced(0x607C33, 0x590D90, reinterpret_cast<DWORD>(OnUIObjectTooltip));
			genCallEnforced(0x607C86, 0x590D90, reinterpret_cast<DWORD>(OnUIObjectTooltip));
			genCallEnforced(0x607CA7, 0x590D90, reinterpret_cast<DWORD>(OnUIObjectTooltip));
			genCallEnforced(0x60EE6B, 0x590D90, reinterpret_cast<DWORD>(OnUIObjectTooltip));
			genCallEnforced(0x61550D, 0x590D90, reinterpret_cast<DWORD>(OnUIObjectTooltip));

			// Event: UI Tooltip post-creation for spells.
			genPushEnforced(0x5E3E5D, reinterpret_cast<DWORD>(OnUISpellTooltip));
			genPushEnforced(0x5E3EDF, reinterpret_cast<DWORD>(OnUISpellTooltip));
			genPushEnforced(0x5E3F8E, reinterpret_cast<DWORD>(OnUISpellTooltip));
			genPushEnforced(0x5E4069, reinterpret_cast<DWORD>(OnUISpellTooltip));
			genPushEnforced(0x5E4165, reinterpret_cast<DWORD>(OnUISpellTooltip));

			/*
			// TODO: Figure out a good hook point for this.
			genCallEnforced(0x0, 0x595A40, reinterpret_cast<DWORD>(OnBuildUI_Tooltip));
			genCallEnforced(0x0, 0x590D90, reinterpret_cast<DWORD>(OnBuildUI_ItemTooltip));
			*/

			// Event: Activation Target Changed
			genCallEnforced(0x41CA64, 0x567990, reinterpret_cast<DWORD>(HookPreFindActivationTarget));
			genJumpUnprotected(0x41CCF5, reinterpret_cast<DWORD>(HookPostFindActivationTarget));

			// Event: Weather transitions
			genCallEnforced(0x410294, 0x4E22F0, reinterpret_cast<DWORD>(OnWeatherCycle));
			genCallEnforced(0x410368, 0x441C40, reinterpret_cast<DWORD>(OnWeatherImmediateChange));
			genCallEnforced(0x441084, 0x441C40, reinterpret_cast<DWORD>(OnWeatherImmediateChange));
			genCallEnforced(0x45CE2D, 0x441C40, reinterpret_cast<DWORD>(OnWeatherImmediateChange));
			genCallEnforced(0x45D211, 0x441C40, reinterpret_cast<DWORD>(OnWeatherImmediateChange));
			genCallEnforced(0x441B49, 0x6DE7F0, reinterpret_cast<DWORD>(OnWeatherTransitionBegin));
			genCallEnforced(0x440F07, 0x414890, reinterpret_cast<DWORD>(OnWeatherTransitionEnd));
			writePatchCodeUnprotected(0x410308, (BYTE*)&patchWeatherRegionCheck, patchWeatherRegionCheck_size);

			// Event: Select music track
			genCallEnforced(0x40F8CA, 0x410EA0, reinterpret_cast<DWORD>(OnSelectMusicTrack));
			genCallEnforced(0x40F901, 0x410EA0, reinterpret_cast<DWORD>(OnSelectMusicTrack));

			// Event: Weapon ready.
			genCallEnforced(0x527FEA, 0x52D5B0, reinterpret_cast<DWORD>(OnReadyNoWeapon));

			// Event: Weapon unready.
			genCallEnforced(0x495D73, 0x528000, reinterpret_cast<DWORD>(OnUnreadyWeapon));
			genCallEnforced(0x495E3A, 0x528000, reinterpret_cast<DWORD>(OnUnreadyWeapon));
			genCallEnforced(0x49601C, 0x528000, reinterpret_cast<DWORD>(OnUnreadyWeapon));
			genCallEnforced(0x53FEDB, 0x528000, reinterpret_cast<DWORD>(OnUnreadyWeapon));
			genCallEnforced(0x558479, 0x528000, reinterpret_cast<DWORD>(OnUnreadyWeapon));
			genCallEnforced(0x569D02, 0x528000, reinterpret_cast<DWORD>(OnUnreadyWeapon));
			genCallEnforced(0x56AA65, 0x528000, reinterpret_cast<DWORD>(OnUnreadyWeapon));
			genCallEnforced(0x56B0B5, 0x528000, reinterpret_cast<DWORD>(OnUnreadyWeapon));
			genCallEnforced(0x5D0C66, 0x528000, reinterpret_cast<DWORD>(OnUnreadyWeapon));

			// Event: Leveled item picked.
			auto leveledItemPick = &TES3::LeveledItem::resolve;
			genCallEnforced(0x49A20E, 0x4D0BD0, *reinterpret_cast<DWORD*>(&leveledItemPick));
			genCallEnforced(0x49A25B, 0x4D0BD0, *reinterpret_cast<DWORD*>(&leveledItemPick));
			genCallEnforced(0x4D0DD3, 0x4D0BD0, *reinterpret_cast<DWORD*>(&leveledItemPick));

			// Event: Leveled creature picked.
			auto leveledCreaturePick = &TES3::LeveledCreature::resolve;
			genCallEnforced(0x4B8C95, 0x4CF870, *reinterpret_cast<DWORD*>(&leveledCreaturePick));
			genCallEnforced(0x4B8E80, 0x4CF870, *reinterpret_cast<DWORD*>(&leveledCreaturePick));
			genCallEnforced(0x4CF9E7, 0x4CF870, *reinterpret_cast<DWORD*>(&leveledCreaturePick));
			genCallEnforced(0x4CFB43, 0x4CF870, *reinterpret_cast<DWORD*>(&leveledCreaturePick));
			genCallEnforced(0x635236, 0x4CF870, *reinterpret_cast<DWORD*>(&leveledCreaturePick));

			// Event: Mobile added to controller.
			auto mobControllerAddMob = &TES3::MobController::addMob;
			genCallEnforced(0x4665D5, 0x5636A0, *reinterpret_cast<DWORD*>(&mobControllerAddMob));
			genCallEnforced(0x484F3D, 0x5636A0, *reinterpret_cast<DWORD*>(&mobControllerAddMob));
			genCallEnforced(0x4C6954, 0x5636A0, *reinterpret_cast<DWORD*>(&mobControllerAddMob));
			genCallEnforced(0x4DC965, 0x5636A0, *reinterpret_cast<DWORD*>(&mobControllerAddMob));
			genCallEnforced(0x4EBCBF, 0x5636A0, *reinterpret_cast<DWORD*>(&mobControllerAddMob));
			genCallEnforced(0x5090BF, 0x5636A0, *reinterpret_cast<DWORD*>(&mobControllerAddMob));
			genCallEnforced(0x50990C, 0x5636A0, *reinterpret_cast<DWORD*>(&mobControllerAddMob));
			genCallEnforced(0x509A6E, 0x5636A0, *reinterpret_cast<DWORD*>(&mobControllerAddMob));
			genCallEnforced(0x50EFE3, 0x5636A0, *reinterpret_cast<DWORD*>(&mobControllerAddMob));
			genCallEnforced(0x529C3B, 0x5636A0, *reinterpret_cast<DWORD*>(&mobControllerAddMob));
			genCallEnforced(0x54DE92, 0x5636A0, *reinterpret_cast<DWORD*>(&mobControllerAddMob));
			genCallEnforced(0x57356C, 0x5636A0, *reinterpret_cast<DWORD*>(&mobControllerAddMob));
			genCallEnforced(0x5752C6, 0x5636A0, *reinterpret_cast<DWORD*>(&mobControllerAddMob));
			genCallEnforced(0x57595B, 0x5636A0, *reinterpret_cast<DWORD*>(&mobControllerAddMob));
			genCallEnforced(0x635390, 0x5636A0, *reinterpret_cast<DWORD*>(&mobControllerAddMob));

			// Event: Mobile added to controller.
			auto mobControllerRemoveMob = &TES3::MobController::removeMob;
			genCallEnforced(0x4668D8, 0x5637F0, *reinterpret_cast<DWORD*>(&mobControllerRemoveMob));
			genCallEnforced(0x484E24, 0x5637F0, *reinterpret_cast<DWORD*>(&mobControllerRemoveMob));
			genCallEnforced(0x4E47C1, 0x5637F0, *reinterpret_cast<DWORD*>(&mobControllerRemoveMob));
			genCallEnforced(0x4E8911, 0x5637F0, *reinterpret_cast<DWORD*>(&mobControllerRemoveMob));
			genCallEnforced(0x4EBD8C, 0x5637F0, *reinterpret_cast<DWORD*>(&mobControllerRemoveMob));
			genCallEnforced(0x50919F, 0x5637F0, *reinterpret_cast<DWORD*>(&mobControllerRemoveMob));
			genCallEnforced(0x523A1F, 0x5637F0, *reinterpret_cast<DWORD*>(&mobControllerRemoveMob));
			genCallEnforced(0x523AE5, 0x5637F0, *reinterpret_cast<DWORD*>(&mobControllerRemoveMob));
			genCallEnforced(0x52E980, 0x5637F0, *reinterpret_cast<DWORD*>(&mobControllerRemoveMob));
			genCallEnforced(0x52EA6D, 0x5637F0, *reinterpret_cast<DWORD*>(&mobControllerRemoveMob));
			genCallEnforced(0x52EDE5, 0x5637F0, *reinterpret_cast<DWORD*>(&mobControllerRemoveMob));
			genCallEnforced(0x574FDB, 0x5637F0, *reinterpret_cast<DWORD*>(&mobControllerRemoveMob));
			genCallEnforced(0x57509A, 0x5637F0, *reinterpret_cast<DWORD*>(&mobControllerRemoveMob));
			genCallEnforced(0x57548A, 0x5637F0, *reinterpret_cast<DWORD*>(&mobControllerRemoveMob));
			genCallEnforced(0x575647, 0x5637F0, *reinterpret_cast<DWORD*>(&mobControllerRemoveMob));

			// Event: Calculate barter price.
			if (genCallEnforced(0x5A447B, 0x5A46E0, reinterpret_cast<DWORD>(OnCalculateBarterPrice_CalcItemValue))) {
				// Merchant Window: Selling (Buy back)
				genCallEnforced(0x5A4490, 0x52AA50, reinterpret_cast<DWORD>(OnCalculateBarterPrice));
			}
			if (genCallEnforced(0x5A44BA, 0x5A46E0, reinterpret_cast<DWORD>(OnCalculateBarterPrice_CalcItemValue))) {
				// Merchant Window: Buying
				genCallEnforced(0x5A44CF, 0x52AA50, reinterpret_cast<DWORD>(OnCalculateBarterPrice));
			}
			if (genCallEnforced(0x5A490E, 0x5A46E0, reinterpret_cast<DWORD>(OnCalculateBarterPrice_CalcItemValue))) {
				// PC Window: Buying (Sell back)
				genCallEnforced(0x5A4926, 0x52AA50, reinterpret_cast<DWORD>(OnCalculateBarterPrice));
			}
			if (genCallEnforced(0x5A494B, 0x5A46E0, reinterpret_cast<DWORD>(OnCalculateBarterPrice_CalcItemValue))) {
				// PC Window: Selling
				genCallEnforced(0x5A4963, 0x52AA50, reinterpret_cast<DWORD>(OnCalculateBarterPrice));
			}

			// Event: Calculate repair price.
			if (genCallEnforced(0x615243, 0x47E710, reinterpret_cast<DWORD>(OnCalculateRepairPriceForList_GetItemList))) {
				// Generate repair list.
				genCallEnforced(0x615347, 0x52AA50, reinterpret_cast<DWORD>(OnCalculateRepairPriceForList));
			}
			if (genCallEnforced(0x615638, 0x581440, reinterpret_cast<DWORD>(OnCalculateRepairPrice_GetRepairStack))) {
				// Click to repair an item.
				genCallEnforced(0x6156AC, 0x52AA50, reinterpret_cast<DWORD>(OnCalculateRepairPrice)); // With MCP patch #33.
				genCallEnforced(0x6156B4, 0x52AA50, reinterpret_cast<DWORD>(OnCalculateRepairPrice)); // Without MCP patch #33.
			}

			// Event: Calculate spell price.
			if (genCallEnforced(0x61645A, 0x47E710, reinterpret_cast<DWORD>(OnCalculateSpellPriceForList_GetSpellList))) {
				// Generate spell list.
				genCallEnforced(0x6164A6, 0x52AA50, reinterpret_cast<DWORD>(OnCalculateSpellPriceForList));
			}
			if (genCallEnforced(0x6166E5, 0x581440, reinterpret_cast<DWORD>(OnCalculateSpellPrice_GetSpell))) {
				// Actually buying a spell.
				genCallEnforced(0x61671C, 0x52AA50, reinterpret_cast<DWORD>(OnCalculateSpellPrice));
			}

			// Event: Calculate training price. We have to make sure it works with and without MCP patch #33.
			if (!genCallEnforced(0x617D0D, 0x736760, reinterpret_cast<DWORD>(OnCalculateTrainingPrice_GetSkillPatched))) {
				genCallUnprotected(0x617D0D, reinterpret_cast<DWORD>(OnCalculateTrainingPrice_GetSkillUnpatched), 0x6);
			}
			if (!genCallEnforced(0x617E11, 0x736760, reinterpret_cast<DWORD>(OnCalculateTrainingPrice_GetSkillPatched))) {
				genCallUnprotected(0x617E11, reinterpret_cast<DWORD>(OnCalculateTrainingPrice_GetSkillUnpatched), 0x6);
			}
			if (!genCallEnforced(0x617F52, 0x736760, reinterpret_cast<DWORD>(OnCalculateTrainingPrice_GetSkillPatched))) {
				genCallUnprotected(0x617F52, reinterpret_cast<DWORD>(OnCalculateTrainingPrice_GetSkillUnpatched), 0x6);
			}
			if (!genCallEnforced(0x618253, 0x736760, reinterpret_cast<DWORD>(OnCalculateTrainingPrice_GetSkillPatched))) {
				genCallUnprotected(0x618253, reinterpret_cast<DWORD>(OnCalculateTrainingPrice_GetSkillUnpatched), 0x6);
			}
			genCallEnforced(0x617D3B, 0x52AA50, reinterpret_cast<DWORD>(OnCalculateTrainingPrice));
			genCallEnforced(0x617E3F, 0x52AA50, reinterpret_cast<DWORD>(OnCalculateTrainingPrice));
			genCallEnforced(0x617F80, 0x52AA50, reinterpret_cast<DWORD>(OnCalculateTrainingPrice));
			genCallEnforced(0x618281, 0x52AA50, reinterpret_cast<DWORD>(OnCalculateTrainingPrice));

			// Event: Calculate travel price.
			if (genCallEnforced(0x619294, 0x47E710, reinterpret_cast<DWORD>(OnCalculateTravelPrice_GetDestinationList)) &&
				genCallEnforced(0x6193B2, 0x47E710, reinterpret_cast<DWORD>(OnCalculateTravelPrice_GetCompanionList)) &&
				genCallEnforced(0x619430, 0x53AD70, reinterpret_cast<DWORD>(OnCalculateTravelPrice_CheckCompanionDistance))) {
				// Generating travel list.
				genCallEnforced(0x619488, 0x52AA50, reinterpret_cast<DWORD>(OnCalculateTravelPriceForList));
			}
			if (genCallEnforced(0x61966C, 0x47E710, reinterpret_cast<DWORD>(OnCalculateTravelPrice_GetDestinationList)) &&
				genCallEnforced(0x6198AE, 0x47E710, reinterpret_cast<DWORD>(OnCalculateTravelPrice_GetCompanionList)) &&
				genCallEnforced(0x61992C, 0x53AD70, reinterpret_cast<DWORD>(OnCalculateTravelPrice_CheckCompanionDistance))) {
				// Actually traveling.
				genCallEnforced(0x619770, 0x52AA50, reinterpret_cast<DWORD>(OnCalculateTravelPriceForDestination)); // Without MCP patch #33.
				genCallEnforced(0x73691D, 0x52AA50, reinterpret_cast<DWORD>(OnCalculateTravelPriceForDestination)); // With MCP patch #33.
			}

			// Event: Determine AI actions.
			auto combatSessionDetermineAction = &TES3::CombatSession::determineNextAction;
			genCallEnforced(0x5591D6, 0x538F00, *reinterpret_cast<DWORD*>(&combatSessionDetermineAction));

			// Event: Skill Raised.
			genCallEnforced(0x4A28C6, 0x629FC0, reinterpret_cast<DWORD>(OnSkillRaised));
			genCallEnforced(0x56BCF2, 0x629FC0, reinterpret_cast<DWORD>(OnSkillRaised));

			// Event: UI Refreshed.
			genCallEnforced(0x6272F9, 0x649870, reinterpret_cast<DWORD>(OnRefreshedStatsPane)); // MenuStat_scroll_pane

			// Event: Inventory Filter.
			genCallEnforced(0x5CBD5F, 0x5CC720, reinterpret_cast<DWORD>(OnFilterInventoryTile));
			genCallEnforced(0x5CCAC5, 0x5CC720, reinterpret_cast<DWORD>(OnFilterInventoryTile));

			// Event: Barter Menu Filter.
			genCallEnforced(0x5A4AAD, 0x5A5430, reinterpret_cast<DWORD>(OnFilterBarterTile));
			genCallEnforced(0x5A4C5B, 0x5A5430, reinterpret_cast<DWORD>(OnFilterBarterTile));
			genCallEnforced(0x5A576D, 0x5A5430, reinterpret_cast<DWORD>(OnFilterBarterTile));

			// Event: Contents Menu Filter.
			genCallEnforced(0x5B6995, 0x47E360, reinterpret_cast<DWORD>(OnFilterContentsTile));
			genCallEnforced(0x5B7116, 0x47E360, reinterpret_cast<DWORD>(OnFilterContentsTileForTakeAll));
			genNOPUnprotected(0x5B711B, 0x1B);

			// Event: Inventory Select Menu Filter.
			writePatchCodeUnprotected(0x5D3F9C, (BYTE*)&patchFilterInventorySelect, patchFilterInventorySelect_size);
			genCallUnprotected(0x5D3FA0, reinterpret_cast<DWORD>(OnFilterInventorySelect));

			// Event: Info override text.
			genCallEnforced(0x4B1BF2, 0x4EEE40, reinterpret_cast<DWORD>(PatchGetDialogueInfoText_GetSourceMod));
			genCallEnforced(0x4B1D70, 0x4B6880, reinterpret_cast<DWORD>(PatchGetDialogueInfoText_ReadFromFile));

			// Event: Info Filter.
			auto dialogueInfoFilter = &TES3::DialogueInfo::filter;
			genCallEnforced(0x4B2A77, 0x4B0190, *reinterpret_cast<DWORD*>(&dialogueInfoFilter));

			// Event: Execute lua from dialogue response.
			genCallEnforced(0x4B1FB2, 0x50E5A0, reinterpret_cast<DWORD>(OnRunDialogueCommand));
			
			// Hook overriding book text.
			auto bookGetText = &TES3::Book::getBookText;
			genCallEnforced(0x4A29FA, 0x4A2A90, *reinterpret_cast<DWORD*>(&bookGetText));
			genCallEnforced(0x4A2A0F, 0x4A2A90, *reinterpret_cast<DWORD*>(&bookGetText));

			// Event: Item Dropped.
			genCallEnforced(0x485FCA, 0x485E40, reinterpret_cast<DWORD>(OnItemDropped)); // MCP-added function.
			genCallEnforced(0x49B550, 0x485E40, reinterpret_cast<DWORD>(OnItemDropped)); // Vanilla function.

			// Event: Calculate hit chance.
			genNOPUnprotected(0x55549B, 0x5C);
			writePatchCodeUnprotected(0x55549B, (BYTE*)&patchCalculateHitChance, patchCalculateHitChance_size);
			genCallUnprotected(0x5554A0, reinterpret_cast<DWORD>(OnCalculateHitChance));

			// Event: Created filters.
			genCallEnforced(0x418A10, 0x411400, reinterpret_cast<DWORD>(CreateFilters));

			// Override standard isSoulGem tests.
			auto isSoulGem = &tes3::isSoulGem;
			genCallEnforced(0x4E174A, 0x49ABE0, *reinterpret_cast<DWORD*>(&isSoulGem));
			genCallEnforced(0x4E79F3, 0x49ABE0, *reinterpret_cast<DWORD*>(&isSoulGem));
			genCallEnforced(0x5910EA, 0x49ABE0, *reinterpret_cast<DWORD*>(&isSoulGem));
			genCallEnforced(0x59173C, 0x49ABE0, *reinterpret_cast<DWORD*>(&isSoulGem));
			genCallEnforced(0x5A4744, 0x49ABE0, *reinterpret_cast<DWORD*>(&isSoulGem));
			genCallEnforced(0x5A55FF, 0x49ABE0, *reinterpret_cast<DWORD*>(&isSoulGem));
			genCallEnforced(0x5C5D0D, 0x49ABE0, *reinterpret_cast<DWORD*>(&isSoulGem));
			genCallEnforced(0x5C6B11, 0x49ABE0, *reinterpret_cast<DWORD*>(&isSoulGem));
			genCallEnforced(0x5CC8EF, 0x49ABE0, *reinterpret_cast<DWORD*>(&isSoulGem));
			genCallEnforced(0x5CE81F, 0x49ABE0, *reinterpret_cast<DWORD*>(&isSoulGem));
			genCallEnforced(0x608AB9, 0x49ABE0, *reinterpret_cast<DWORD*>(&isSoulGem));

			// Change inventory saving to use the above isSoulGem check.
			genNOPUnprotected(0x499AEF, 0x13);
			writePatchCodeUnprotected(0x499AEF, (BYTE*)&patchSetupWriteEnchantSoulData, patchSetupWriteEnchantSoulData_size);
			genCallUnprotected(0x499AEF + 0x4, reinterpret_cast<DWORD>(PatchWriteEnchantSoulData));

			// Change soul trap to use the above isSoulGem check.
			genNOPUnprotected(0x49ACD1, 0x13);
			writePatchCodeUnprotected(0x49ACD1, (BYTE*)&patchSetupCheckSoulTrapSoulGem, patchSetupCheckSoulTrapSoulGem_size);
			genCallUnprotected(0x49ACD7, reinterpret_cast<DWORD>(PatchCheckSoulTrapSoulGem));

			// Change soul trap inventory count check.
			auto inventoryGetSoulGemCount = &TES3::Inventory::getSoulGemCount;
			genCallEnforced(0x463310, 0x49AA10, *reinterpret_cast<DWORD*>(&inventoryGetSoulGemCount));

			// Yet another soul gem check. Relates to reference persisting and object saving.
			genNOPUnprotected(0x4C1A13, 0x28);
			genCallUnprotected(0x4C1A13, reinterpret_cast<DWORD>(tes3::isSoulGem));

			// Override actor soul value check: capturing.
			genCallEnforced(0x49AC82, 0x521610, reinterpret_cast<DWORD>(PatchGetSoulValueForMobileActor));
			writePatchCodeUnprotected(0x49AC87, (BYTE*)&patchEnforceSoulValueAboveZero, patchEnforceSoulValueAboveZero_size);

			// Override actor soul value check: enchanting
			writeByteUnprotected(0x5C5E53, 0x56);
			genCallEnforced(0x5C5E57, 0x581F30, reinterpret_cast<DWORD>(PatchSetSoulValueProperty));
			writeByteUnprotected(0x5C5E62, 0x56);
			genCallUnprotected(0x5C5E69, reinterpret_cast<DWORD>(PatchSPrintFSoulValue), 0x6);
			genNOPUnprotected(0x5C5E6F, 0x6);
			genCallEnforced(0x5C5E87, 0x581F30, reinterpret_cast<DWORD>(PatchSetSoulValueProperty));

			// Make soul gem data writable.
			DWORD OldProtect;
			VirtualProtect((DWORD*)0x791C98, 6 * sizeof(TES3::SoulGemData), PAGE_READWRITE, &OldProtect);

			// Allow overriding of armor rating.
			auto armorGetRating = &TES3::Armor::calculateArmorRating;
			genCallEnforced(0x54D421, 0x4A1120, *reinterpret_cast<DWORD*>(&armorGetRating));
			genCallEnforced(0x54D47D, 0x4A1120, *reinterpret_cast<DWORD*>(&armorGetRating));
			genCallEnforced(0x54D4DE, 0x4A1120, *reinterpret_cast<DWORD*>(&armorGetRating));
			genCallEnforced(0x54D53F, 0x4A1120, *reinterpret_cast<DWORD*>(&armorGetRating));
			genCallEnforced(0x54D5A0, 0x4A1120, *reinterpret_cast<DWORD*>(&armorGetRating));
			genCallEnforced(0x54D601, 0x4A1120, *reinterpret_cast<DWORD*>(&armorGetRating));
			genCallEnforced(0x54D662, 0x4A1120, *reinterpret_cast<DWORD*>(&armorGetRating));
			genCallEnforced(0x54D6C3, 0x4A1120, *reinterpret_cast<DWORD*>(&armorGetRating));
			genCallEnforced(0x54D712, 0x4A1120, *reinterpret_cast<DWORD*>(&armorGetRating));
			genCallEnforced(0x54D773, 0x4A1120, *reinterpret_cast<DWORD*>(&armorGetRating));
			genCallEnforced(0x54D7BE, 0x4A1120, *reinterpret_cast<DWORD*>(&armorGetRating));
			genCallEnforced(0x5912EC, 0x4A1120, *reinterpret_cast<DWORD*>(&armorGetRating));

			// Allow overriding of armor rating for NPCs.
			auto armorGetRatingForNPC = &TES3::Armor::calculateArmorRatingForNPC;
			genCallEnforced(0x4D9B95, 0x4A0FD0, *reinterpret_cast<DWORD*>(&armorGetRatingForNPC));
			genCallEnforced(0x4D9C2E, 0x4A0FD0, *reinterpret_cast<DWORD*>(&armorGetRatingForNPC));
			genCallEnforced(0x537AF6, 0x4A0FD0, *reinterpret_cast<DWORD*>(&armorGetRatingForNPC));

			// Allow overriding of armor weight class.
			auto armorGetWeightClass = &TES3::Armor::getWeightClass;
			genCallEnforced(0x411256, 0x4A0F30, *reinterpret_cast<DWORD*>(&armorGetWeightClass));
			genCallEnforced(0x46F61B, 0x4A0F30, *reinterpret_cast<DWORD*>(&armorGetWeightClass));
			genCallEnforced(0x507BDD, 0x4A0F30, *reinterpret_cast<DWORD*>(&armorGetWeightClass));
			genCallEnforced(0x54DA0A, 0x4A0F30, *reinterpret_cast<DWORD*>(&armorGetWeightClass));
			genCallEnforced(0x557480, 0x4A0F30, *reinterpret_cast<DWORD*>(&armorGetWeightClass));
			genCallEnforced(0x5916D9, 0x4A0F30, *reinterpret_cast<DWORD*>(&armorGetWeightClass));

			// Allow overriding of armor slot name.
			auto armorGetSlotName = &TES3::Armor::getSlotName;
			overrideVirtualTableEnforced(0x748258, 0x98, 0x4A1270, *reinterpret_cast<DWORD*>(&armorGetSlotName));

			// Recognize when an inventory tile is updated.
			genCallEnforced(0x5A5DA4, 0x47E720, reinterpret_cast<DWORD>(GetNextInventoryTileToUpdate)); // General barter menu update.
			genCallEnforced(0x5B6655, 0x581F30, reinterpret_cast<DWORD>(OnInventoryTileChildPropertySet)); // During click event into contents menu.
			genCallEnforced(0x5B6F13, 0x47E720, reinterpret_cast<DWORD>(GetNextInventoryTileToUpdate)); // General contents menu update.
			genCallEnforced(0x5CC366, 0x58AE20, reinterpret_cast<DWORD>(OnSetItemTileIcon)); // On manual AddTile.
			genCallEnforced(0x5CD147, 0x47E720, reinterpret_cast<DWORD>(GetNextInventoryTileToUpdate)); // General inventory menu update.
			genCallEnforced(0x5D1943, 0x58AE20, reinterpret_cast<DWORD>(OnSetItemTileIcon)); // Root purpose unclear.

			// Override ItemData creation/deletion.
			genPushEnforced(0x498A50, (BYTE)sizeof(TES3::ItemData)) && genCallEnforced(0x498A6C, 0x4E44B0, reinterpret_cast<DWORD>(&TES3::ItemData::ctor));
			genPushEnforced(0x498DB4, (BYTE)sizeof(TES3::ItemData)) && genCallEnforced(0x498DD0, 0x4E44B0, reinterpret_cast<DWORD>(&TES3::ItemData::ctor));
			genPushEnforced(0x49AAF9, (BYTE)sizeof(TES3::ItemData)) && genCallEnforced(0x49AB17, 0x4E44B0, reinterpret_cast<DWORD>(&TES3::ItemData::ctor));
			genPushEnforced(0x49ADA9, (BYTE)sizeof(TES3::ItemData)) && genCallEnforced(0x49ADC1, 0x4E44B0, reinterpret_cast<DWORD>(&TES3::ItemData::ctor));
			genPushEnforced(0x49AEEC, (BYTE)sizeof(TES3::ItemData)) && genCallEnforced(0x49AF08, 0x4E44B0, reinterpret_cast<DWORD>(&TES3::ItemData::ctor));
			genPushEnforced(0x49E590, (BYTE)sizeof(TES3::ItemData)) && genCallEnforced(0x49E5AF, 0x4E44B0, reinterpret_cast<DWORD>(CreateItemDataFromLoading));
			genPushEnforced(0x49E630, (BYTE)sizeof(TES3::ItemData)) && genCallEnforced(0x49E64F, 0x4E44B0, reinterpret_cast<DWORD>(CreateItemDataFromLoading));
			genPushEnforced(0x4A4FFC, (BYTE)sizeof(TES3::ItemData)) && genCallEnforced(0x4A5018, 0x4E44B0, reinterpret_cast<DWORD>(CreateItemDataFromLoading));
			genPushEnforced(0x4A5099, (BYTE)sizeof(TES3::ItemData)) && genCallEnforced(0x4A50B5, 0x4E44B0, reinterpret_cast<DWORD>(CreateItemDataFromLoading));
			genPushEnforced(0x4D9497, (BYTE)sizeof(TES3::ItemData)) && genCallEnforced(0x4D94B6, 0x4E44B0, reinterpret_cast<DWORD>(CreateItemDataFromLoading));
			genPushEnforced(0x4D9537, (BYTE)sizeof(TES3::ItemData)) && genCallEnforced(0x4D9556, 0x4E44B0, reinterpret_cast<DWORD>(CreateItemDataFromLoading));
			genPushEnforced(0x5142D3, (BYTE)sizeof(TES3::ItemData)) && genCallEnforced(0x5142F2, 0x4E44B0, reinterpret_cast<DWORD>(&TES3::ItemData::ctor));
			genCallEnforced(0x45519C, 0x4E44E0, reinterpret_cast<DWORD>(&TES3::ItemData::dtor));
			genCallEnforced(0x46579B, 0x4E44E0, reinterpret_cast<DWORD>(&TES3::ItemData::dtor));
			genCallEnforced(0x496664, 0x4E44E0, reinterpret_cast<DWORD>(&TES3::ItemData::dtor));
			genCallEnforced(0x497C6E, 0x4E44E0, reinterpret_cast<DWORD>(&TES3::ItemData::dtor));
			genCallEnforced(0x4990A8, 0x4E44E0, reinterpret_cast<DWORD>(&TES3::ItemData::dtor));
			genCallEnforced(0x49912D, 0x4E44E0, reinterpret_cast<DWORD>(&TES3::ItemData::dtor));
			genCallEnforced(0x499299, 0x4E44E0, reinterpret_cast<DWORD>(&TES3::ItemData::dtor));
			genCallEnforced(0x49931B, 0x4E44E0, reinterpret_cast<DWORD>(&TES3::ItemData::dtor));
			genCallEnforced(0x49987E, 0x4E44E0, reinterpret_cast<DWORD>(&TES3::ItemData::dtor));
			genCallEnforced(0x49B859, 0x4E44E0, reinterpret_cast<DWORD>(&TES3::ItemData::dtor));
			genCallEnforced(0x49B8AD, 0x4E44E0, reinterpret_cast<DWORD>(&TES3::ItemData::dtor));
			genCallEnforced(0x4E4AA6, 0x4E44E0, reinterpret_cast<DWORD>(&TES3::ItemData::dtor));
			genCallEnforced(0x4E70D5, 0x4E44E0, reinterpret_cast<DWORD>(&TES3::ItemData::dtor));
			genCallEnforced(0x5124B6, 0x4E44E0, reinterpret_cast<DWORD>(&TES3::ItemData::dtor));
			genCallEnforced(0x52C3DE, 0x4E44E0, reinterpret_cast<DWORD>(&TES3::ItemData::dtor));
			genCallEnforced(0x52C41D, 0x4E44E0, reinterpret_cast<DWORD>(&TES3::ItemData::dtor));
			genCallEnforced(0x5A6492, 0x4E44E0, reinterpret_cast<DWORD>(&TES3::ItemData::dtor));
			genCallEnforced(0x5C47A3, 0x4E44E0, reinterpret_cast<DWORD>(&TES3::ItemData::dtor));
			genCallEnforced(0x5C4C36, 0x4E44E0, reinterpret_cast<DWORD>(&TES3::ItemData::dtor));
			genCallEnforced(0x4E48A7, 0x4E5410, reinterpret_cast<DWORD>(OnDeletingItemData));
			genPushEnforced(0x4E7761, (BYTE)sizeof(TES3::ItemData));
			genCallEnforced(0x46589C, 0x4E7750, reinterpret_cast<DWORD>(&TES3::ItemData::createForObject));
			genCallEnforced(0x465CFC, 0x4E7750, reinterpret_cast<DWORD>(&TES3::ItemData::createForObject));
			genCallEnforced(0x465D92, 0x4E7750, reinterpret_cast<DWORD>(&TES3::ItemData::createForObject));
			genCallEnforced(0x465F1A, 0x4E7750, reinterpret_cast<DWORD>(&TES3::ItemData::createForObject));
			genCallEnforced(0x495402, 0x4E7750, reinterpret_cast<DWORD>(&TES3::ItemData::createForObject));
			genCallEnforced(0x496383, 0x4E7750, reinterpret_cast<DWORD>(&TES3::ItemData::createForObject));
			genCallEnforced(0x49803E, 0x4E7750, reinterpret_cast<DWORD>(&TES3::ItemData::createForObject));
			genCallEnforced(0x49825F, 0x4E7750, reinterpret_cast<DWORD>(&TES3::ItemData::createForObject));
			genCallEnforced(0x49A38C, 0x4E7750, reinterpret_cast<DWORD>(&TES3::ItemData::createForObject));
			genCallEnforced(0x49B28F, 0x4E7750, reinterpret_cast<DWORD>(&TES3::ItemData::createForObject));
			genCallEnforced(0x4E70F3, 0x4E7750, reinterpret_cast<DWORD>(&TES3::ItemData::createForObject));
			genCallEnforced(0x514FFD, 0x4E7750, reinterpret_cast<DWORD>(&TES3::ItemData::createForObject));
			genCallEnforced(0x573705, 0x4E7750, reinterpret_cast<DWORD>(&TES3::ItemData::createForObject));
			genCallEnforced(0x573D24, 0x4E7750, reinterpret_cast<DWORD>(&TES3::ItemData::createForObject));
			genCallEnforced(0x5A62DC, 0x4E7750, reinterpret_cast<DWORD>(&TES3::ItemData::createForObject));
			genCallEnforced(0x5CE707, 0x4E7750, reinterpret_cast<DWORD>(&TES3::ItemData::createForObject));
			genCallEnforced(0x5CEAF9, 0x4E7750, reinterpret_cast<DWORD>(&TES3::ItemData::createForObject));
			genCallEnforced(0x5CEDC8, 0x4E7750, reinterpret_cast<DWORD>(&TES3::ItemData::createForObject));
			genCallEnforced(0x5CF2D5, 0x4E7750, reinterpret_cast<DWORD>(&TES3::ItemData::createForObject));
			genCallEnforced(0x5CF667, 0x4E7750, reinterpret_cast<DWORD>(&TES3::ItemData::createForObject));
			genCallEnforced(0x5CFEA4, 0x4E7750, reinterpret_cast<DWORD>(&TES3::ItemData::createForObject));
			genCallEnforced(0x5D030D, 0x4E7750, reinterpret_cast<DWORD>(&TES3::ItemData::createForObject));
			genPushEnforced(0x4E7665, (BYTE)sizeof(TES3::ItemData));
			GeneratePatchForInlineItemDataCreation(0x4E7673);
			GeneratePatchForInlineItemDataDestruction(0x4E53A7);
			GeneratePatchForInlineItemDataDestruction(0x4E76DA);
			GeneratePatchForInlineItemDataDestruction(0x4E78F8);

			// Override item data fully repaired comparison to ensure there are no lua variables.
			genCallEnforced(0x41089C, 0x4E7970, reinterpret_cast<DWORD>(&TES3::ItemData::isFullyRepaired));
			genCallEnforced(0x465643, 0x4E7970, reinterpret_cast<DWORD>(&TES3::ItemData::isFullyRepaired));
			genCallEnforced(0x496BF0, 0x4E7970, reinterpret_cast<DWORD>(&TES3::ItemData::isFullyRepaired));
			genCallEnforced(0x497C58, 0x4E7970, reinterpret_cast<DWORD>(&TES3::ItemData::isFullyRepaired));
			genCallEnforced(0x4E162B, 0x4E7970, reinterpret_cast<DWORD>(&TES3::ItemData::isFullyRepaired));
			genCallEnforced(0x52C400, 0x4E7970, reinterpret_cast<DWORD>(&TES3::ItemData::isFullyRepaired));
			genCallEnforced(0x52C6A2, 0x4E7970, reinterpret_cast<DWORD>(&TES3::ItemData::isFullyRepaired));
			genCallEnforced(0x5B4F07, 0x4E7970, reinterpret_cast<DWORD>(&TES3::ItemData::isFullyRepaired));
			genCallEnforced(0x5E1826, 0x4E7970, reinterpret_cast<DWORD>(&TES3::ItemData::isFullyRepaired));
			genCallEnforced(0x60E172, 0x4E7970, reinterpret_cast<DWORD>(&TES3::ItemData::isFullyRepaired));
			genCallEnforced(0x60E566, 0x4E7970, reinterpret_cast<DWORD>(&TES3::ItemData::isFullyRepaired));
			genCallEnforced(0x615789, 0x4E7970, reinterpret_cast<DWORD>(&TES3::ItemData::isFullyRepaired));
			genCallEnforced(0x633A6F, 0x4E7970, reinterpret_cast<DWORD>(&TES3::ItemData::isFullyRepaired));

			// File loading/saving hooks for extended ItemData structure.
			genCallEnforced(0x4998EA, 0x47E710, reinterpret_cast<DWORD>(GetFirstSavedItemStack));
			genCallEnforced(0x4999BB, 0x4B6BA0, reinterpret_cast<DWORD>(WriteItemDataIndex));
			genCallEnforced(0x499B40, 0x4B6BA0, reinterpret_cast<DWORD>(WriteItemDataCondition));
			genCallEnforced(0x49D747, 0x4B67C0, reinterpret_cast<DWORD>(LoadNextRecordForActor));
			genCallEnforced(0x49E6BF, 0x4B67C0, reinterpret_cast<DWORD>(LoadNextRecordForActor));
			genCallEnforced(0x4A4741, 0x4B67C0, reinterpret_cast<DWORD>(LoadNextRecordForActor));
			genCallEnforced(0x4A5140, 0x4B67C0, reinterpret_cast<DWORD>(LoadNextRecordForActor));
			genCallEnforced(0x4D849B, 0x4B67C0, reinterpret_cast<DWORD>(LoadNextRecordForActor));
			genCallEnforced(0x4D95C8, 0x4B67C0, reinterpret_cast<DWORD>(LoadNextRecordForActor));
			genCallEnforced(0x4E1615, 0x4E5460, reinterpret_cast<DWORD>(GetItemDataForReferenceSaving));
			genCallEnforced(0x4E1856, 0x4B6BA0, reinterpret_cast<DWORD>(WriteReferenceItemDataCondition));
			genCallEnforced(0x4DE3C6, 0x4E5750, reinterpret_cast<DWORD>(LoadReferenceGetMACT));
			genCallEnforced(0x4DE426, 0x4B67C0, reinterpret_cast<DWORD>(LoadNextRecordForReference));

			// UI framework hooks
			TES3::UI::hook();

			// Make magic effects writable.
			VirtualProtect((DWORD*)TES3_DATA_EFFECT_FLAGS, 4 * 143, PAGE_READWRITE, &OldProtect);

			// Hook generic entity deletion so that we can do any necessary cleanup.
			genCallEnforced(0x4AA15B, 0x4F0CA0, reinterpret_cast<DWORD>(OnEntityDelete));
			genCallEnforced(0x4AAF10, 0x4F0CA0, reinterpret_cast<DWORD>(OnEntityDelete));
			genCallEnforced(0x4E49EE, 0x4F0CA0, reinterpret_cast<DWORD>(OnEntityDelete));
			genCallEnforced(0x4EEFAA, 0x4F0CA0, reinterpret_cast<DWORD>(OnEntityDelete));
			genCallEnforced(0x4F026F, 0x4F0CA0, reinterpret_cast<DWORD>(OnEntityDelete));
			genCallEnforced(0x4F0C83, 0x4F0CA0, reinterpret_cast<DWORD>(OnEntityDelete));

			// Look for main.lua scripts in the usual directories.
			executeMainModScripts("Data Files/MWSE/core");
			executeMainModScripts("Data Files/MWSE/mods");

			// Temporary backwards compatibility for old-style MWSE mods.
			executeMainModScripts("Data Files/MWSE/lua", "mod_init.lua");
		}

		void LuaManager::cleanup() {
			// Clean up our handles to our override tables. Helps to prevent a crash when
			// closing mid-execution.
			scriptOverrides.clear();

			userdataMapMutex.lock();
			userdataCache.clear();
			userdataMapMutex.unlock();
		}

		TES3::Script* LuaManager::getCurrentScript() {
			return currentScript;
		}

		void LuaManager::setCurrentScript(TES3::Script* script) {
			currentScript = script;
		}

		TES3::Reference* LuaManager::getCurrentReference() {
			return currentReference;
		}

		void LuaManager::setCurrentReference(TES3::Reference* reference) {
			currentReference = reference;
		}

		sol::object LuaManager::triggerEvent(event::BaseEvent* baseEvent) {
			DWORD threadId = GetCurrentThreadId();
			TES3::DataHandler* dataHandler = TES3::DataHandler::get();

			// If we're on the main thread, immediately execute the event
			if (dataHandler == nullptr || threadId == dataHandler->mainThreadID) {
				// Use this opportunity to check for background events that need to run.
				triggerBackgroundThreadEvents();

				// Execute the original event.
				sol::object response = event::trigger(baseEvent->getEventName(), baseEvent->createEventTable(), baseEvent->getEventOptions());
				delete baseEvent;
				return response;
			}

			// If we're not on the main thread, queue the event to run once we are.
			else if (threadId == dataHandler->backgroundThreadID) {
				backgroundThreadEventsMutex.lock();
				backgroundThreadEvents.push(baseEvent);
				backgroundThreadEventsMutex.unlock();
			}

			// If we're not on the main thread or the background thread we don't know WTF is going on.
			else {
				throw new std::exception("Event triggered from unknown thread!");
			}

			return sol::nil;
		}

		void LuaManager::triggerBackgroundThreadEvents() {
			backgroundThreadEventsMutex.lock();

			while (backgroundThreadEvents.size() > 0) {
				// Pop the event off the stack.
				event::BaseEvent* baseEvent = backgroundThreadEvents.front();
				backgroundThreadEvents.pop();

				// Trigger it.
				event::trigger(baseEvent->getEventName(), baseEvent->createEventTable(), baseEvent->getEventOptions());
				delete baseEvent;
			}

			backgroundThreadEventsMutex.unlock();
		}

		void LuaManager::setButtonPressedCallback(sol::optional<sol::protected_function> callback) {
			buttonPressedCallback = callback.value_or(sol::nil);
		}

		void LuaManager::triggerButtonPressed() {
			if (buttonPressedCallback != sol::nil) {
				sol::protected_function callback = buttonPressedCallback;
				buttonPressedCallback = sol::nil;
				sol::table eventData = createTable();
				eventData["button"] = tes3::ui::getButtonPressedIndex();
				tes3::ui::resetButtonPressedIndex();

				sol::protected_function_result result = callback(eventData);
				if (!result.valid()) {
					sol::error error = result;
					log::getLog() << "Runtime error when running tes3.messageBox button callback:\n" << error.what() << std::endl;
				}
			}
		}

		sol::object LuaManager::getCachedUserdata(TES3::BaseObject* object) {
			userdataMapMutex.lock();

			sol::object result = sol::nil;

			UserdataMap::iterator searchResult = userdataCache.find((unsigned long)object);
			if (searchResult != userdataCache.end()) {
				result = searchResult->second;
			}

			userdataMapMutex.unlock();

			return result;
		}

		sol::object LuaManager::getCachedUserdata(TES3::MobileObject* object) {
			userdataMapMutex.lock();

			sol::object result = sol::nil;

			UserdataMap::iterator searchResult = userdataCache.find((unsigned long)object);
			if (searchResult != userdataCache.end()) {
				result = searchResult->second;
			}

			userdataMapMutex.unlock();

			return result;
		}

		void LuaManager::insertUserdataIntoCache(TES3::BaseObject* object, sol::object luaObject) {
			userdataMapMutex.lock();
			userdataCache[(unsigned long)object] = luaObject;
			userdataMapMutex.unlock();
		}

		void LuaManager::insertUserdataIntoCache(TES3::MobileObject* object, sol::object luaObject) {
			userdataMapMutex.lock();
			userdataCache[(unsigned long)object] = luaObject;
			userdataMapMutex.unlock();
		}

		void LuaManager::removeUserdataFromCache(TES3::BaseObject* object) {
			userdataMapMutex.lock();

			if (!userdataCache.empty()) {
				UserdataMap::iterator it = userdataCache.find((unsigned long)object);
				if (it != userdataCache.end()) {
					// Clear any events that make use of this object.
					event::clearObjectFilter(it->second);

					// Remove it from the cache.
					userdataCache.erase(it);
				}
			}

			userdataMapMutex.unlock();
		}

		void LuaManager::removeUserdataFromCache(TES3::MobileObject* object) {
			userdataMapMutex.lock();

			if (!userdataCache.empty()) {
				UserdataMap::iterator it = userdataCache.find((unsigned long)object);
				if (it != userdataCache.end()) {
					// Clear any events that make use of this object.
					event::clearObjectFilter(it->second);

					// Remove it from the cache.
					userdataCache.erase(it);
				}
			}

			userdataMapMutex.unlock();
		}

		void LuaManager::updateTimers(float deltaTime, double simulationTimestamp, bool simulating) {
			realTimers->incrementClock(deltaTime);
			gameTimers->setClock(simulationTimestamp);

			if (simulating) {
				simulateTimers->incrementClock(deltaTime);
			}
		}

		void LuaManager::clearTimers() {
			realTimers->clearTimers();
			simulateTimers->clearTimers();
			gameTimers->clearTimers();

			// Reset the clocks for each timer.
			realTimers->setClock(0.0);
			simulateTimers->setClock(0.0);
			gameTimers->setClock(TES3::WorldController::get()->getHighPrecisionSimulationTimestamp());
		}

		std::shared_ptr<TimerController> LuaManager::getTimerController(TimerType type) {
			switch (type) {
			case TimerType::RealTime: return realTimers;
			case TimerType::SimulationTime: return simulateTimers;
			case TimerType::GameTime: return gameTimers;
			}
			return nullptr;
		}
	}
}