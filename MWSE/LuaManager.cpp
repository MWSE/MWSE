#include "LuaManager.h"

#include "mwOffsets.h"
#include "Log.h"
#include "TES3Util.h"
#include "MemoryUtil.h"
#include "ScriptUtil.h"
#include "StringUtil.h"
#include "UIUtil.h"
#include "MWSEDefs.h"
#include "BuildDate.h"
#include "MWSEUtilLua.h"
#include "WindowsUtil.h"
#include "MWSEConfig.h"
#include "BitUtil.h"

#include "LuaTimer.h"

#include "LuaScript.h"

#include "TES3Defines.h"
#include "TES3Actor.h"
#include "TES3ActorAnimationController.h"
#include "TES3Alchemy.h"
#include "TES3Apparatus.h"
#include "TES3Book.h"
#include "TES3BodyPartManager.h"
#include "TES3CombatSession.h"
#include "TES3Container.h"
#include "TES3Creature.h"
#include "TES3CrimeEvent.h"
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
#include "TES3Lockpick.h"
#include "TES3MagicEffect.h"
#include "TES3MagicEffectController.h"
#include "TES3MagicEffectInstance.h"
#include "TES3Misc.h"
#include "TES3MobController.h"
#include "TES3MobileActor.h"
#include "TES3MobileCreature.h"
#include "TES3MobilePlayer.h"
#include "TES3MobileProjectile.h"
#include "TES3NPC.h"
#include "TES3PlayerAnimationController.h"
#include "TES3Probe.h"
#include "TES3Reference.h"
#include "TES3RepairTool.h"
#include "TES3Script.h"
#include "TES3SoulGemData.h"
#include "TES3Sound.h"
#include "TES3Spell.h"
#include "TES3UIElement.h"
#include "TES3UIInventoryTile.h"
#include "TES3UIManager.h"
#include "TES3UIMenuController.h"
#include "TES3Weather.h"
#include "TES3WeatherController.h"
#include "TES3WorldController.h"

// Lua binding files. These are split out rather than kept here to help with compile times.
#include "MemoryUtilLua.h"
#include "StackLua.h"
#include "ScriptUtilLua.h"
#include "StringUtilLua.h"
#include "TES3UtilLua.h"
#include "TES3ActionDataLua.h"
#include "TES3ActivatorLua.h"
#include "TES3ActorAnimationControllerLua.h"
#include "TES3AILua.h"
#include "TES3AlchemyLua.h"
#include "TES3ApparatusLua.h"
#include "TES3ArmorLua.h"
#include "TES3AttachmentLua.h"
#include "TES3AudioControllerLua.h"
#include "TES3BirthsignLua.h"
#include "TES3BodyPartLua.h"
#include "TES3BookLua.h"
#include "TES3CellLua.h"
#include "TES3ClassLua.h"
#include "TES3ClothingLua.h"
#include "TES3CollisionLua.h"
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
#include "TES3PlayerAnimationControllerLua.h"
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
#include "TES3MobControllerLua.h"
#include "TES3StaticLua.h"
#include "TES3StatisticLua.h"
#include "TES3UIElementLua.h"
#include "TES3UIInventoryTileLua.h"
#include "TES3UIManagerLua.h"
#include "TES3UIMenuControllerLua.h"
#include "TES3UIWidgetsLua.h"
#include "TES3VectorsLua.h"
#include "TES3WeaponLua.h"
#include "TES3WeatherControllerLua.h"
#include "TES3WeatherLua.h"
#include "TES3WorldControllerLua.h"

#include "NICameraLua.h"
#include "NICollisionSwitchLua.h"
#include "NIColorLua.h"
#include "NIExtraDataLua.h"
#include "NIGeometryDataLua.h"
#include "NINodeLua.h"
#include "NIObjectLua.h"
#include "NILightLua.h"
#include "NIPickLua.h"
#include "NIPixelDataLua.h"
#include "NIPropertyLua.h"
#include "NISkinInstanceLua.h"
#include "NISourceTextureLua.h"
#include "NISwitchNodeLua.h"
#include "NITextureLua.h"
#include "NITimeControllerLua.h"
#include "NITriShapeLua.h"

#include "LuaDisableableEventManager.h"

#include "LuaActivationTargetChangedEvent.h"
#include "LuaAddTopicEvent.h"
#include "LuaAttackEvent.h"
#include "LuaBarterOfferEvent.h"
#include "LuaCalcBarterPriceEvent.h"
#include "LuaCalcHitArmorPieceEvent.h"
#include "LuaCalcHitChanceEvent.h"
#include "LuaCalcRepairPriceEvent.h"
#include "LuaCalcRestInterruptEvent.h"
#include "LuaCalcSoulValueEvent.h"
#include "LuaCalcSpellPriceEvent.h"
#include "LuaCalcTrainingPriceEvent.h"
#include "LuaCalcTravelPriceEvent.h"
#include "LuaCellChangedEvent.h"
#include "LuaCrimeWitnessedEvent.h"
#include "LuaDamageEvent.h"
#include "LuaDamageHandToHandEvent.h"
#include "LuaEnchantedItemCreatedEvent.h"
#include "LuaEnchantedItemCreateFailedEvent.h"
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
#include "LuaInfoLinkResolveEvent.h"
#include "LuaInfoResponseEvent.h"
#include "LuaItemDroppedEvent.h"
#include "LuaItemTileUpdatedEvent.h"
#include "LuaKeyDownEvent.h"
#include "LuaKeyUpEvent.h"
#include "LuaLeveledCreaturePickedEvent.h"
#include "LuaLevelUpEvent.h"
#include "LuaLoadedGameEvent.h"
#include "LuaMagicCastedEvent.h"
#include "LuaMenuStateEvent.h"
#include "LuaMouseAxisEvent.h"
#include "LuaMouseButtonDownEvent.h"
#include "LuaMouseButtonUpEvent.h"
#include "LuaMouseWheelEvent.h"
#include "LuaMusicSelectTrackEvent.h"
#include "LuaObjectInvalidatedEvent.h"
#include "LuaPostInfoResponseEvent.h"
#include "LuaPotionBrewedEvent.h"
#include "LuaPotionBrewFailedEvent.h"
#include "LuaPowerRechargedEvent.h"
#include "LuaPreLevelUpEvent.h"
#include "LuaPreventRestEvent.h"
#include "LuaProjectileExpireEvent.h"
#include "LuaRestInterruptEvent.h"
#include "LuaSimulateEvent.h"
#include "LuaSkillRaisedEvent.h"
#include "LuaSpellCastedEvent.h"
#include "LuaSpellCreatedEvent.h"
#include "LuaSpellResistEvent.h"
#include "LuaSpellTickEvent.h"
#include "LuaUiRefreshedEvent.h"
#include "LuaUiSpellTooltipEvent.h"
#include "LuaWeaponReadiedEvent.h"
#include "LuaWeaponUnreadiedEvent.h"
#include "LuaWeatherChangedImmediateEvent.h"
#include "LuaWeatherCycledEvent.h"
#include "LuaWeatherTransitionFinishedEvent.h"
#include "LuaWeatherTransitionStartedEvent.h"
#include "LuaLeveledItemPickedEvent.h"

#include "NITextureEffectLua.h"

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

#define TES3_HOOK_UI_EVENT 0x58371A
#define TES3_HOOK_UI_EVENT_SIZE 0x5
#define TES3_HOOK_UI_EVENT_RETURN (TES3_HOOK_UI_EVENT + TES3_HOOK_UI_EVENT_SIZE)

#define TES3_HOOK_MAGIC_CAST_SUCCESS 0x5157E6
#define TES3_HOOK_MAGIC_CAST_SUCCESS_SIZE 0xA
#define TES3_HOOK_MAGIC_CAST_SUCCESS_RETURN (TES3_HOOK_MAGIC_CAST_SUCCESS + TES3_HOOK_MAGIC_CAST_SUCCESS_SIZE)

#define TES3_HOOK_SPELL_CAST_FAILURE 0x5157D5
#define TES3_HOOK_SPELL_CAST_FAILURE_SIZE 0xA
#define TES3_HOOK_SPELL_CAST_FAILURE_RETURN (TES3_HOOK_SPELL_CAST_FAILURE + TES3_HOOK_SPELL_CAST_FAILURE_SIZE)

#define TES3_PATCH_MAGIC_SAVE_LOAD 0x51391F
#define TES3_PATCH_MAGIC_SAVE_LOAD_SIZE 0x8
#define TES3_PATCH_MAGIC_SAVE_LOAD_RETURN (TES3_PATCH_MAGIC_SAVE_LOAD + TES3_PATCH_MAGIC_SAVE_LOAD_SIZE)

#define TES3_ActorAnimController_attackCheckMeleeHit 0x541530

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

		bool LuaManager::overrideScript(const char* scriptId, sol::object target) {
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
				auto& luaManager = mwse::lua::LuaManager::getInstance();
				auto stateHandle = luaManager.getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::object result = state.safe_script_file("./Data Files/MWSE/mods/" + target.as<std::string>() + ".lua");
				if (result.get_type() == sol::type::table) {
					scriptOverrides[(unsigned long)script] = result;
					script->dataLength = 0;
					return true;
				}
			}

			return false;
		}

		void LuaManager::lua_print(sol::object message) {
			auto& luaManager = mwse::lua::LuaManager::getInstance();
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;
			// TODO: Optimize by caching function here.
			std::string result = state["tostring"](message);
			log::getLog() << result << std::endl;
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
			luaState["print"] = lua_print;

			// Bind our data types.
			bindData();
		}

		void lua_os_getClipboardText() {

		}

		void LuaManager::bindData() {
			// Bind our LuaScript type, which is used for holding script contexts.
			auto usertypeDefinition = luaState.new_usertype<LuaScript>("LuaScript");
			usertypeDefinition[sol::meta_function::index] = &LuaScript::dynamic_get;
			usertypeDefinition[sol::meta_function::new_index] = &LuaScript::dynamic_set;
			usertypeDefinition[sol::meta_function::length] = &LuaScript::size;
			usertypeDefinition["script"] = sol::readonly_property(&LuaScript::m_Script);
			usertypeDefinition["reference"] = sol::readonly_property(&LuaScript::m_Reference);
			usertypeDefinition["context"] = sol::readonly_property(&LuaScript::getContext);

			// Create the base of API tables.
			luaState["mwse"] = luaState.create_table();
			luaState["mwscript"] = luaState.create_table();
			luaState["mge"] = luaState.create_table();

			// Bind config.
			Configuration::bindToLua();

			// Expose timers.
			bindLuaTimer();
			bindMWSEUtil();

			// Extend OS library.
			luaState["os"]["getClipboardText"] = getClipboardText;
			luaState["os"]["setClipboardText"] = setClipboardText;

			// Bind TES3 data types.
			bindTES3ActionData();
			bindTES3Activator();
			bindTES3ActorAnimationController();
			bindTES3AI();
			bindTES3Alchemy();
			bindTES3Apparatus();
			bindTES3Armor();
			bindTES3Attachment();
			bindTES3AudioController();
			bindTES3Birthsign();
			bindTES3BodyPart();
			bindTES3Book();
			bindTES3Cell();
			bindTES3Class();
			bindTES3Clothing();
			bindTES3Collision();
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
			bindTES3MobController();
			bindTES3MobileCreature();
			bindTES3MobileNPC();
			bindTES3MobilePlayer();
			bindTES3MobileProjectile();
			bindTES3Moon();
			bindTES3NPC();
			bindTES3PlayerAnimationController();
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
			bindTES3UIMenuController();
			bindTES3UIWidgets();

			// Bind NI data types.
			bindNICamera();
			bindNICollisionSwitch();
			bindNIColor();
			bindNIDynamicEffect();
			bindNIExtraData();
			bindNIGeometryData();
			bindNINode();
			bindNIObject();
			bindNILight();
			bindNIPick();
			bindNIPixelData();
			bindNIProperties();
			bindNISkinInstance();
			bindNISourceTexture();
			bindNISwitchNode();
			bindNITextureEffect();
			bindNITexture();
			bindNITimeController();
			bindNITriShape();

			// Bind our disable event manager.
			mwse::lua::event::DisableableEventManager::bindToLua();
			luaState["mwse"]["disableableEvents"] = &m_DisableableEventManager;
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
			auto stateHandle = manager.getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;
			sol::protected_function execute;
			if (searchResult->second.is<sol::function>()) {
				execute = searchResult->second.as<sol::function>();
			}
			else if (searchResult->second.get_type() == sol::type::table) {
				execute = searchResult->second.as<sol::table>()["execute"];
			}

			if (execute) {
				sol::table params = state.create_table();
				params["script"] = script;
				params["reference"] = reference;
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

			manager.setCurrentReference(nullptr);
			manager.setCurrentScript(nullptr);
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

		const auto TES3_Reference_SetMobile = reinterpret_cast<void(__thiscall*)(TES3::Reference*, TES3::MobileObject*)>(0x4E5770);
		void __fastcall OnPlayerReferenceAssigned(TES3::Reference* player, DWORD _EDX_, TES3::MobilePlayer* macp) {
			// Call overwritten function.
			TES3_Reference_SetMobile(player, macp);

			// Initialize mobile data that is never updated for the player.
			macp->simulationDistance = 0;

			// Grab the new player pointers for lua.
			// Update tes3.player and tes3.mobilePlayer.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;
			state["tes3"]["mobilePlayer"] = macp;
			state["tes3"]["player"] = player;
			state["tes3"]["player1stPerson"] = macp->firstPersonReference;
		}

		TES3::Reference* __fastcall OnPlayerReferenceCreated(TES3::Reference* player) {
			// Call overwritten function.
			player->ctor();

			// Grab the new player pointers for lua.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;
			state["tes3"]["player"] = player;

			return player;
		}

		//
		// Hook: Finished initializing game code.
		//

		void __fastcall FinishInitialization(TES3::IteratedList<void*>* itt) {
			// Call overwritten code.
			itt->clear();

			// Hook up shorthand access to data handler, world controller, and game.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state &state = stateHandle.state;
			state["tes3"]["dataHandler"] = TES3::DataHandler::get();
			state["tes3"]["worldController"] = TES3::WorldController::get();
			state["tes3"]["game"] = TES3::Game::get();

			stateHandle.triggerEvent(new event::GenericEvent("initialized"));
		}

		//
		// Hook: Enter Frame
		//

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
				lastMenuMode = worldController->flagMenuMode;
				if (event::MenuStateEvent::getEventEnabled()) {
					luaManager.getThreadSafeStateHandle().triggerEvent(new event::MenuStateEvent(worldController->flagMenuMode));
				}
			}

			// Has our cell changed?
			TES3::DataHandler * dataHandler = TES3::DataHandler::get();
			if (dataHandler->currentCell != TES3::DataHandler::previousVisitedCell) {
				if (event::CellChangedEvent::getEventEnabled()) {
					luaManager.getThreadSafeStateHandle().triggerEvent(new event::CellChangedEvent(dataHandler->currentCell, TES3::DataHandler::previousVisitedCell));
				}
				TES3::DataHandler::previousVisitedCell = dataHandler->currentCell;
			}

			// Send off our enterFrame event always.
			if (event::FrameEvent::getEventEnabled()) {
				luaManager.getThreadSafeStateHandle().triggerEvent(new event::FrameEvent(worldController->deltaTime, worldController->flagMenuMode, highResolutionTimestamp));
			}

			// If we're not in menu mode, send off the simulate event.
			if (!worldController->flagMenuMode && event::SimulateEvent::getEventEnabled()) {
				luaManager.getThreadSafeStateHandle().triggerEvent(new event::SimulateEvent(worldController->deltaTime, highResolutionTimestamp));
			}
		}

		//
		// Hook: On PC Equip
		//

		signed char __cdecl OnPCEquip(TES3::UI::InventoryTile* tile) {
			// Execute event. If the event blocked the call, bail.
			if (event::EquipEvent::getEventEnabled()) {
				auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
				sol::object response = stateHandle.triggerEvent(new event::EquipEvent(TES3::WorldController::get()->getMobilePlayer()->reference, tile->item, tile->itemData));
				if (response.get_type() == sol::type::table) {
					sol::table eventData = response;
					if (eventData.get_or("block", false)) {
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
			}

			// Call the original function.
			return tes3::ui::equipInventoryTile(tile);
		}

		// Hook: On PC Equip (quickslots, etc)

		signed char __cdecl OnPCEquipItem(TES3::PhysicalObject* object, TES3::ItemData* data) {
			// Execute event. If the event blocked the call, bail.
			if (event::EquipEvent::getEventEnabled()) {
				auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
				sol::object response = stateHandle.triggerEvent(new event::EquipEvent(TES3::WorldController::get()->getMobilePlayer()->reference, object, data));
				if (response.get_type() == sol::type::table) {
					sol::table eventData = response;
					if (eventData.get_or("block", false)) {
						return 0;
					}
				}
			}

			// Call the original function.
			return tes3::ui::equipInventoryItem(object, data);
		}

		static bool OnPCEquipItemDoubled_blocked = false;
		signed char __cdecl OnPCEquipItemDoubled(TES3::PhysicalObject* object, TES3::ItemData* data) {
			OnPCEquipItemDoubled_blocked = false;

			// Execute event. If the event blocked the call, bail.
			if (event::EquipEvent::getEventEnabled()) {
				auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
				sol::object response = stateHandle.triggerEvent(new event::EquipEvent(TES3::WorldController::get()->getMobilePlayer()->reference, object, data));
				if (response.get_type() == sol::type::table) {
					sol::table eventData = response;
					if (eventData.get_or("block", false)) {
						OnPCEquipItemDoubled_blocked = true;
						return 0;
					}
				}
			}

			// Call the original function.
			return tes3::ui::equipInventoryItem(object, data);
		}

		signed char __cdecl OnPCEquipItemDoubledFollowUp(TES3::PhysicalObject* object, TES3::ItemData* data) {
			if (OnPCEquipItemDoubled_blocked) {
				return 0;
			}

			// Call the original function.
			return tes3::ui::equipInventoryItem(object, data);
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
		// Hook: On Biped parts update.
		//

		bool __fastcall OnUpdateBipedParts(TES3::Reference* reference) {
			// Call our wrapper for the function so that events are triggered.
			return reference->updateBipedParts();
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
			return loaded != TES3::LoadGameResult::Failure;
		}

		bool __fastcall OnLoadMainMenu(TES3::NonDynamicData* nonDynamicData, DWORD _UNUSED_, const char* fileName) {
			// Call our wrapper for the function so that events are triggered.
			TES3::LoadGameResult loaded = nonDynamicData->loadGameMainMenu(fileName);
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
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			if (event::LoadedGameEvent::getEventEnabled()) {
				stateHandle.triggerEvent(new event::LoadedGameEvent(nullptr, false, true));
			}
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

		void __fastcall OnAttack(TES3::ActorAnimationController* animController) {
			// Call original function.
			reinterpret_cast<void(__thiscall *)(TES3::ActorAnimationController*)>(TES3_ActorAnimController_attackCheckMeleeHit)(animController);

			// Prepare our event data.
			if (event::AttackEvent::getEventEnabled()) {
				LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::AttackEvent(animController));
			}
		}

		void __fastcall OnAttackStart(TES3::ActorAnimationController* animController, DWORD _UNUSED_, float swing) {
			// Call our wrapper for the function so that events are triggered.
			animController->startAttackAnimation(swing);
		}

		//
		// Collision events: Mobile Actor
		//

		bool __fastcall OnMobileActorActorCollision(TES3::MobileActor* object, DWORD _UNUSED_, int collisionIndex) {
			return object->onActorCollision(collisionIndex);
		}

		bool __fastcall OnMobileActorObjectCollision(TES3::MobileActor* object, DWORD _UNUSED_, int collisionIndex, bool flag) {
			return object->onObjectCollision(collisionIndex, flag);
		}

		bool __fastcall OnMobileActorTerrainCollision(TES3::MobileActor* object, DWORD _UNUSED_, int collisionIndex) {
			return object->onTerrainCollision(collisionIndex);
		}

		bool __fastcall OnMobileActorActivatorCollision(TES3::MobileActor* object, DWORD _UNUSED_, int collisionIndex) {
			return object->onObjectCollision(collisionIndex, 0);
		}

		//
		// Collision events: Mobile Projectile
		//

		bool __fastcall OnMobileProjectileActorCollision(TES3::MobileProjectile* object, DWORD _UNUSED_, int collisionIndex) {
			return object->onActorCollision(collisionIndex);
		}

		bool __fastcall OnMobileProjectileObjectCollision(TES3::MobileProjectile* object, DWORD _UNUSED_, int collisionIndex, bool flag) {
			return object->onObjectCollision(collisionIndex, flag);
		}

		bool __fastcall OnMobileProjectileTerrainCollision(TES3::MobileProjectile* object, DWORD _UNUSED_, int collisionIndex) {
			return object->onTerrainCollision(collisionIndex);
		}

		bool __fastcall OnMobileProjectileWaterCollision(TES3::MobileProjectile* object, DWORD _UNUSED_, int collisionIndex) {
			return object->onWaterCollision(collisionIndex);
		}

		//
		// UI event hooking.
		//

		signed char __cdecl OnUIEvent(DWORD function, TES3::UI::Element* parent, DWORD prop, DWORD b, DWORD c, TES3::UI::Element* source) {
			// Execute event. If the event blocked the call, bail.
			mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
			if (event::GenericUiPreEvent::getEventEnabled()) {
				auto stateHandle = luaManager.getThreadSafeStateHandle();
				sol::table eventData = stateHandle.triggerEvent(new event::GenericUiPreEvent(parent, source, prop, b, c));
				if (eventData.valid() && eventData.get_or("block", false)) {
					return 0;
				}
			}

			signed char result = reinterpret_cast<signed char (__cdecl *)(TES3::UI::Element*, DWORD, DWORD, DWORD, TES3::UI::Element*)>(function)(parent, prop, b, c, source);

			if (event::GenericUiPostEvent::getEventEnabled()) {
				luaManager.getThreadSafeStateHandle().triggerEvent(new event::GenericUiPostEvent(parent, source, prop, b, c));
			}

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
			// If we're using the run in background patch, add a check here.
			if (Configuration::RunInBackground && GetActiveWindow() != TES3::WorldController::get()->Win32_hWndParent) {
				return;
			}

			// Call the original function.
			inputController->readKeyState();

			// We only need to check the modifier key values once.
			bool controlDown = inputController->isControlDown();
			bool shiftDown = inputController->isShiftDown();
			bool altDown = inputController->isAltDown();
			bool superDown = inputController->isSuperDown();

			// Go through the keys to see if any of the states have changed, and launch an event based on that.
			LuaManager& luaManager = LuaManager::getInstance();
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			for (BYTE i = 0; i < UINT8_MAX; i++) {
				if (event::KeyDownEvent::getEventEnabled() && inputController->isKeyPressedThisFrame(i)) {
					stateHandle.triggerEvent(new event::KeyDownEvent(i, controlDown, shiftDown, altDown, superDown));

					// TODO: Remove! Deprecated generic key event.
					stateHandle.triggerEvent(new event::KeyEvent(i, true, controlDown, shiftDown, altDown, superDown));
				}
				else if (event::KeyUpEvent::getEventEnabled() && inputController->isKeyReleasedThisFrame(i)) {
					stateHandle.triggerEvent(new event::KeyUpEvent(i, controlDown, shiftDown, altDown, superDown));

					// TODO: Remove! Deprecated generic key event.
					stateHandle.triggerEvent(new event::KeyEvent(i, false, controlDown, shiftDown, altDown, superDown));
				}

				if (i == UINT8_MAX) {
					break;
				}
			}

			// Do the same with mouse buttons.
			for (BYTE i = 0; i < 8; i++) {
				if (event::MouseButtonDownEvent::getEventEnabled() && inputController->isMouseButtonPressedThisFrame(i)) {
					stateHandle.triggerEvent(new event::MouseButtonDownEvent(i, controlDown, shiftDown, altDown, superDown));
				}
				else if (event::MouseButtonUpEvent::getEventEnabled() && inputController->isMouseButtonReleasedThisFrame(i)) {
					stateHandle.triggerEvent(new event::MouseButtonUpEvent(i, controlDown, shiftDown, altDown, superDown));
				}
			}

			// Look at mouse axis events.
			LONG mouseDeltaX = inputController->mouseState.lX;
			LONG mouseDeltaY = inputController->mouseState.lY;
			if (event::MouseAxisEvent::getEventEnabled() && (mouseDeltaX || mouseDeltaY)) {
				stateHandle.triggerEvent(new event::MouseAxisEvent(mouseDeltaX, mouseDeltaY, controlDown, shiftDown, altDown, superDown));
			}

			// Finally, look at mouse scroll wheel.
			LONG mouseDeltaZ = inputController->mouseState.lZ;
			if (event::MouseWheelEvent::getEventEnabled() && mouseDeltaZ != 0) {
				stateHandle.triggerEvent(new event::MouseWheelEvent(mouseDeltaZ, controlDown, shiftDown, altDown, superDown));
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

		bool __fastcall OnApplyDamageFromScript(TES3::MobileActor* mobileActor, DWORD _UNUSED_, float damage, bool flipDifficultyScale, bool scaleWithDifficulty, bool doNotChangeHealth) {
			mwse::lua::event::DamageEvent::m_Source = "script";
			auto result = mobileActor->applyHealthDamage(damage, flipDifficultyScale, scaleWithDifficulty, doNotChangeHealth);
			mwse::lua::event::DamageEvent::m_Source = nullptr;
			return result;
		}

		bool __fastcall OnApplyDamageFromFalling(TES3::MobileActor* mobileActor, DWORD _UNUSED_, float damage, bool flipDifficultyScale, bool scaleWithDifficulty, bool doNotChangeHealth) {
			mwse::lua::event::DamageEvent::m_Source = "fall";
			auto result = mobileActor->applyHealthDamage(damage, flipDifficultyScale, scaleWithDifficulty, doNotChangeHealth);
			mwse::lua::event::DamageEvent::m_Source = nullptr;
			return result;
		}

		bool __fastcall OnApplyDamageFromSuffocation(TES3::MobileActor* mobileActor, DWORD _UNUSED_, float damage, bool flipDifficultyScale, bool scaleWithDifficulty, bool doNotChangeHealth) {
			mwse::lua::event::DamageEvent::m_Source = "suffocation";
			auto result = mobileActor->applyHealthDamage(damage, flipDifficultyScale, scaleWithDifficulty, doNotChangeHealth);
			mwse::lua::event::DamageEvent::m_Source = nullptr;
			return result;
		}

		bool __fastcall OnApplyDamageFromAttack(TES3::MobileActor* mobileActor, DWORD _UNUSED_, float damage, bool flipDifficultyScale, bool scaleWithDifficulty, bool doNotChangeHealth) {
			mwse::lua::event::DamageEvent::m_Source = "attack";
			auto result = mobileActor->applyHealthDamage(damage, flipDifficultyScale, scaleWithDifficulty, doNotChangeHealth);
			mwse::lua::event::DamageEvent::m_Source = nullptr;
			return result;
		}

		float __fastcall OnApplyFatigueDamageFromAttack(TES3::MobileActor* mobileActor, TES3::MobileActor* attacker, float damage, float swing, bool alwaysPlayHitVoice) {
			mwse::lua::event::DamageHandToHandEvent::m_Attacker = attacker;
			auto result = mobileActor->applyFatigueDamage(damage, swing, alwaysPlayHitVoice);
			mwse::lua::event::DamageHandToHandEvent::m_Attacker = nullptr;
			return result;
		}

		static __declspec(naked) float OnApplyFatigueDamageFromAttack_Wrapper() {
			_asm {
				mov edx, esi
				jmp OnApplyFatigueDamageFromAttack
			}
		}

		bool __fastcall OnApplyDamageFromMagic(TES3::MobileActor* mobileActor, DWORD _UNUSED_, float damage, bool flipDifficultyScale, bool scaleWithDifficulty, bool doNotChangeHealth) {
			mwse::lua::event::DamageEvent::m_Source = "magic";
			auto result = mobileActor->applyHealthDamage(damage, flipDifficultyScale, scaleWithDifficulty, doNotChangeHealth);
			mwse::lua::event::DamageEvent::m_Source = nullptr;
			return result;
		}

		bool __fastcall OnApplyDamageFromMagicShield(TES3::MobileActor* mobileActor, TES3::Deque<TES3::ActiveMagicEffect>::Node* effectNode, float damage, bool flipDifficultyScale, bool scaleWithDifficulty, bool doNotChangeHealth) {
			mwse::lua::event::DamageEvent::m_Source = "shield";
			mwse::lua::event::DamageEvent::m_ActiveMagicEffect = &effectNode->data;
			auto result = mobileActor->applyHealthDamage(damage, flipDifficultyScale, scaleWithDifficulty, doNotChangeHealth);
			mwse::lua::event::DamageEvent::m_Source = nullptr;
			mwse::lua::event::DamageEvent::m_ActiveMagicEffect = nullptr;
			return result;
		}

		static __declspec(naked) bool OnApplyDamageFromMagicShield_Wrapper() {
			_asm {
				mov edx, ebx
				jmp OnApplyDamageFromMagicShield
			}
		}

		const auto TES3_AttributeSpellEffect = reinterpret_cast<bool(__cdecl*)(TES3::MagicSourceInstance*, bool, TES3::Statistic*, void*, TES3::MagicEffectInstance*, float, int)>(0x519110);
		bool __cdecl AttributeSpellEffect(TES3::MagicSourceInstance * sourceInstance, bool positiveModifier, TES3::Statistic * statistic, void * attributeTypeInfo, TES3::MagicEffectInstance * effectInstance, float delta, int effectIndex) {
			mwse::lua::event::DamageEvent::m_MagicSourceInstance = sourceInstance;
			mwse::lua::event::DamageEvent::m_MagicEffectInstance = effectInstance;
			auto result = TES3_AttributeSpellEffect(sourceInstance, positiveModifier, statistic, attributeTypeInfo, effectInstance, delta, effectIndex);
			mwse::lua::event::DamageEvent::m_MagicSourceInstance = nullptr;
			mwse::lua::event::DamageEvent::m_MagicEffectInstance = nullptr;
			return result;
		}

		//
		// Projectile expire event.
		//

		void __fastcall OnProjectileExpire(void* mobController, DWORD _UNUSED_, TES3::Reference* reference) {
			// Get the fired projectile, and trigger an event for it.
			if (event::ProjectileExpireEvent::getEventEnabled()) {
				TES3::MobileProjectile* projectile = reference->getAttachedMobileProjectile();
				LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::ProjectileExpireEvent(projectile));
			}

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
			if (event::SpellResistEvent::getEventEnabled()) {
				auto& luaManager = mwse::lua::LuaManager::getInstance();
				auto stateHandle = luaManager.getThreadSafeStateHandle();
				sol::table eventData = stateHandle.triggerEvent(new event::SpellResistEvent(spellInstance, effectInstance, effectIndex, resistAttribute));
				if (eventData.valid()) {
					effectInstance->resistedPercent = eventData["resistedPercent"];
				}
			}
		}

		//
		// Patch ui_reenableMenuDialogue function to exit menumode if there is no dialogue menu.
		//

		void __cdecl ReenableMenuDialogue() {
			TES3::UI::Element* dialog = TES3::UI::findMenu(TES3::UI::registerID("MenuDialog"));
			if (dialog != nullptr) {
				// Call the original function
				reinterpret_cast<void(__cdecl*)()>(0x5C0B60)();
			}
			else {
				TES3::UI::leaveMenuMode();
			}
		}

		//
		// Exercise skill event.
		//

		void __fastcall OnExerciseSkill(TES3::MobilePlayer* player, DWORD _UNUSED_, int skill, float progress) {
			player->exerciseSkill(skill, progress);
		}

		//
		// Pre level up event.
		//

		void __cdecl OnPreLevelUp(TES3::UI::Element* e) {
			if (event::PreLevelUpEvent::getEventEnabled()) {
				LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::PreLevelUpEvent());
			}
			reinterpret_cast<void(__cdecl*)(TES3::UI::Element*)>(0x5D90A0)(e);
		}
		
		//
		// Level up event.
		//

		void __stdcall OnLevelUp() {
			// Call the original function we overwrote to call this one.
			reinterpret_cast<void(__stdcall *)()>(0x626220)();

			// Launch our event.
			if (event::LevelUpEvent::getEventEnabled()) {
				LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::LevelUpEvent());
			}
		}

		//
		// Potion brewed event.
		//

		static TES3::Alchemy* lastBrewedPotion = nullptr;
		DWORD __fastcall CacheLastBrewedPotion(TES3::UI::InventoryTile* inventoryTile, DWORD _UNUSED_, TES3::Alchemy* object, TES3::ItemData* itemData, DWORD unk1, DWORD unk2, DWORD unk3, DWORD unk4) {
			lastBrewedPotion = object;

			// Call original function.
			return reinterpret_cast<DWORD(__thiscall*)(TES3::UI::InventoryTile*, TES3::Object*, TES3::ItemData*, DWORD, DWORD, DWORD, DWORD)>(0x6313E0)(inventoryTile, object, itemData, unk1, unk2, unk3, unk4);
		}

		TES3::Ingredient* getBrewingIngredient(DWORD parentIdAddress) {
			auto menuAlchemy = TES3::UI::findMenu(*reinterpret_cast<TES3::UI::UI_ID*>(0x7D225C));
			if (!menuAlchemy) {
				return nullptr;
			}

			auto child = menuAlchemy->findChild(*reinterpret_cast<TES3::UI::UI_ID*>(parentIdAddress));
			if (!child) {
				return nullptr;
			}

			return reinterpret_cast<TES3::Ingredient*>(child->getProperty(TES3::UI::PropertyType::Pointer, *reinterpret_cast<TES3::UI::Property*>(0x7D2344)).ptrValue);
		}

		const auto TES3_AttemptPotionBrew = reinterpret_cast<bool(__cdecl*)()>(0x59C030);
		bool __cdecl OnBrewPotionAttempt() {
			// Reset success state.
			lastBrewedPotion = nullptr;

			// Store the last used ingredients, since they get nuked.
			auto ingredient1 = getBrewingIngredient(0x7D22F0);
			auto ingredient2 = getBrewingIngredient(0x7D22F2);
			auto ingredient3 = getBrewingIngredient(0x7D22C0);
			auto ingredient4 = getBrewingIngredient(0x7D2292);

			// Call original function.
			if (!TES3_AttemptPotionBrew()) {
				return false;
			}

			// Perform patches and fire off events.
			if (lastBrewedPotion) {
				// Set unused target attribute/skill ids to -1.
				lastBrewedPotion->cleanUnusedAttributeSkillIds();

				// Pass a lua event.
				if (event::PotionBrewedEvent::getEventEnabled()) {
					LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::PotionBrewedEvent(lastBrewedPotion, ingredient1, ingredient2, ingredient3, ingredient4));
				}
			}
			else {
				if (event::PotionBrewFailedEvent::getEventEnabled()) {
					LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::PotionBrewFailedEvent(ingredient1, ingredient2, ingredient3, ingredient4));
				}
			}

			return true;
		}

		//
		// Enchantment created event.
		//

		template <typename T>
		T* getValueFromEnchantingMenu(DWORD parentIdAddress, DWORD propertyIdAddress) {
			auto menuEnchanting = TES3::UI::findMenu(*reinterpret_cast<TES3::UI::UI_ID*>(0x7D36BC));
			if (!menuEnchanting) {
				return nullptr;
			}

			auto child = menuEnchanting->findChild(*reinterpret_cast<TES3::UI::UI_ID*>(parentIdAddress));
			if (!child) {
				return nullptr;
			}

			return reinterpret_cast<T*>(child->getProperty(TES3::UI::PropertyType::Pointer, *reinterpret_cast<TES3::UI::Property*>(propertyIdAddress)).ptrValue);
		}

		static TES3::Item* lastCreatedEnchantedItem = nullptr;
		void __fastcall CacheLastEnchantedItem(TES3::Item* item, DWORD _EDX_, TES3::Enchantment* enchantment) {
			lastCreatedEnchantedItem = item;
			item->setEnchantment(enchantment);
		}

		const auto TES3_AttemptCreateEnchantedItem = reinterpret_cast<bool(__cdecl*)()>(0x5C3D90);
		bool __cdecl OnCreateEnchantedItemAttempt() {
			// Reset success state.
			lastCreatedEnchantedItem = nullptr;

			// Store the original item, the soul gem, and the contained soul since they get nuked.
			// None of these names make sense. Blame Todd.
			const auto ui_id_ptr_MenuEnchantment_Item = 0x7D358C;
			const auto ui_id_ptr_MenuEnchantment_SoulGem = 0x7D35A6;
			auto enchantedFrom = getValueFromEnchantingMenu<TES3::Item>(ui_id_ptr_MenuEnchantment_Item, ui_id_ptr_MenuEnchantment_SoulGem);
			auto soulGemUsed = getValueFromEnchantingMenu<TES3::Misc>(ui_id_ptr_MenuEnchantment_SoulGem, ui_id_ptr_MenuEnchantment_SoulGem);
			auto soulGemItemData = getValueFromEnchantingMenu<TES3::ItemData>(ui_id_ptr_MenuEnchantment_SoulGem, ui_id_ptr_MenuEnchantment_Item);
			auto soulUsed = soulGemItemData ? soulGemItemData->soul : nullptr;

			// TODO: Allow item data to be accessed or transferred to the newly enchanted item.
			//auto enchantedFromItemData = getValueFromEnchantingMenu<TES3::ItemData>(ui_id_ptr_MenuEnchantment_Item, ui_id_ptr_MenuEnchantment_Item);

			// Call original function.
			if (!TES3_AttemptCreateEnchantedItem()) {
				return false;
			}

			// Fire off any events.
			if (lastCreatedEnchantedItem) {
				if (event::EnchantedItemCreatedEvent::getEventEnabled()) {
					LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::EnchantedItemCreatedEvent(lastCreatedEnchantedItem, enchantedFrom, soulGemUsed, soulUsed));
				}
			}
			else {
				if (event::EnchantedItemCreateFailedEvent::getEventEnabled()) {
					LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::EnchantedItemCreateFailedEvent(enchantedFrom, soulGemUsed, soulUsed));
				}
			}

			return true;
		}

		//
		// Spell created event.
		//

		void __fastcall OnAddNewlyCreatedSpell(TES3::NonDynamicData* ndd, DWORD _EDX_, TES3::Spell* spell) {
			// Call overwritten code.
			ndd->addNewObject(spell);

			// Pass a lua event.
			if (event::SpellCreatedEvent::getEventEnabled()) {
				LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::SpellCreatedEvent(spell, "service"));
			}
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
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			if (event::MagicCastedEvent::getEventEnabled()) {
				stateHandle.triggerEvent(new event::MagicCastedEvent(magicInstance));
			}

			if (event::SpellCastedEvent::getEventEnabled() && magicInstance->sourceCombo.sourceType == TES3::MagicSourceType::Spell) {
				// Spell cast event, allows updating the school that gains experience
				sol::table eventData = stateHandle.triggerEvent(new event::SpellCastedEvent(magicInstance, true, *pExpGainSchool));
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
			if (event::SpellCastedEvent::getEventEnabled()) {
				LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::SpellCastedEvent(magicInstance, false, *pExpGainSchool));
			}
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
			if (event::CalcRestInterruptEvent::getEventEnabled()) {
				mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
				auto stateHandle = luaManager.getThreadSafeStateHandle();
				sol::table eventData = stateHandle.triggerEvent(new event::CalcRestInterruptEvent(count, hour));
				if (eventData.valid()) {
					// Allow blocking any spawn.
					if (eventData.get_or("block", false)) {
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
		}

		TES3::Object * __fastcall OnInterruptRest(TES3::LeveledCreature* leveledCreature, DWORD _UNUSED_) {
			// Creature that we return.
			TES3::Object * creature = NULL;

			// Fire off an event and change the determined creature.
			if (event::RestInterruptEvent::getEventEnabled()) {
				mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
				auto stateHandle = luaManager.getThreadSafeStateHandle();
				sol::table eventData = stateHandle.triggerEvent(new event::RestInterruptEvent(leveledCreature));
				if (eventData.valid()) {
					// Allow blocking any spawn.
					if (eventData.get_or("block", false)) {
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
			}

			// If we were given a leveled creature initially, fall back to that.
			if (leveledCreature) {
				return leveledCreature->resolve();
			}
			else {
				return nullptr;
			}
		}

		//
		// Event: topicAdded
		//

		void __fastcall OnAddTopicAtIndex(TES3::IteratedList<TES3::Dialogue*> * topicList, DWORD _UNUSED_, TES3::Dialogue * topic, unsigned int index) {
			// Run overwritten function.
			topicList->insert(index, topic);

			// Raise event.
			if (event::AddTopicEvent::getEventEnabled()) {
				mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::AddTopicEvent(topic));
			}
		}

		void __fastcall OnAddTopic(TES3::IteratedList<TES3::Dialogue*> * topicList, DWORD _UNUSED_, TES3::Dialogue * topic) {
			// Run overwritten function.
			topicList->push_back(topic);

			// Raise event.
			if (event::AddTopicEvent::getEventEnabled()) {
				mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::AddTopicEvent(topic));
			}
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
			if (event::GenericUiActivatedEvent::getEventEnabled()) {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				stateHandle.triggerEvent(new event::GenericUiActivatedEvent(element));

				// DEPRECATED. TODO: Remove before 2.1 final.
				stateHandle.triggerEvent(new event::GenericUiCreatedEvent(element));
			}
		}

		void __fastcall OnUICreatedAfterPerformLayout(TES3::UI::Element* element, DWORD _UNUSED_, bool unknown) {
			// Overwritten code.
			element->performLayout(unknown);

			// Fire off the event.
			if (event::GenericUiActivatedEvent::getEventEnabled()) {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				stateHandle.triggerEvent(new event::GenericUiActivatedEvent(element));

				// DEPRECATED. TODO: Remove before 2.1 final.
				stateHandle.triggerEvent(new event::GenericUiCreatedEvent(element));
			}
		}

		void __cdecl OnUICreatedAfterConsoleMenuFocus(TES3::UI::Element* widget, int event, int data0, int data1, TES3::UI::Element* element) {
			// Overwritten code.
			const auto ui_ConsoleMenu_onFocus = reinterpret_cast<void (__cdecl*)(TES3::UI::Element*,int,int,int, TES3::UI::Element*)>(0x5B23E0);
			ui_ConsoleMenu_onFocus(widget, event, data0, data1, element);

			// Fire off the event.
			if (event::GenericUiActivatedEvent::getEventEnabled()) {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				stateHandle.triggerEvent(new event::GenericUiActivatedEvent(element));

				// DEPRECATED. TODO: Remove before 2.1 final.
				stateHandle.triggerEvent(new event::GenericUiCreatedEvent(element));
			}
		}

		void __cdecl OnUICreatedAfterConsoleMenuShown(TES3::UI::Element* element) {
			// Overwritten code.
			reinterpret_cast<void(__cdecl*)(TES3::UI::Element*)>(0x595350)(element);

			// Fire off the event.
			if (event::GenericUiActivatedEvent::getEventEnabled()) {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				stateHandle.triggerEvent(new event::GenericUiActivatedEvent(element));

				// DEPRECATED. TODO: Remove before 2.1 final.
				stateHandle.triggerEvent(new event::GenericUiCreatedEvent(element));
			}
		}

		void __fastcall OnUICreatedAfterMenuVisibleEvent(TES3::UI::Element* element, DWORD _UNUSED_, bool unknown) {
			// Overwritten code.
			const auto ui_setMenuVisibleWithEvent = reinterpret_cast<void (__thiscall*)(TES3::UI::Element*, bool)>(0x582F20);
			ui_setMenuVisibleWithEvent(element, unknown);

			// Fire off the event.
			if (event::GenericUiActivatedEvent::getEventEnabled()) {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				stateHandle.triggerEvent(new event::GenericUiActivatedEvent(element));

				// DEPRECATED. TODO: Remove before 2.1 final.
				stateHandle.triggerEvent(new event::GenericUiCreatedEvent(element));
			}
		}

		void __cdecl OnUICreatedAfterScrollPaneUpdate(TES3::UI::Element* element) {
			// Overwritten code.
			const auto ui_ScrollPane_UpdateScrollBars = reinterpret_cast<void (__cdecl*)(TES3::UI::Element*)>(0x649E40);
			ui_ScrollPane_UpdateScrollBars(element);

			// Fire off the event.
			if (event::GenericUiActivatedEvent::getEventEnabled()) {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				stateHandle.triggerEvent(new event::GenericUiActivatedEvent(element));

				// DEPRECATED. TODO: Remove before 2.1 final.
				stateHandle.triggerEvent(new event::GenericUiCreatedEvent(element));
			}
		}

		//
		// Event: UI Spell Tooltip post-creation.
		//

		bool __cdecl OnUISpellTooltip(TES3::UI::Element* widget, int event, int data0, int data1, TES3::UI::Element* element) {
			// Call original function.
			bool created = reinterpret_cast<bool(__cdecl *)(TES3::UI::Element*, int, int, int, TES3::UI::Element*)>(0x5E45C0)(widget, event, data0, data1, element);

			// Fire off the event.
			if (created && event::UiSpellTooltipEvent::getEventEnabled()) {
				TES3::UI::Element* tooltip = TES3::UI::findHelpLayerMenu(TES3::UI::UI_ID(TES3::UI::Property::HelpMenu));
				TES3::Spell * spell = reinterpret_cast<TES3::Spell *>(element->getProperty(TES3::UI::PropertyType::Pointer, *reinterpret_cast<TES3::UI::Property*>(0x7D431C)).ptrValue);
				LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::UiSpellTooltipEvent(tooltip, spell));
			}

			return created;
		}

		//
		// Event: Activation Target Changed
		//

		static const uintptr_t MACP__getPlayerAnimData_fieldEC = 0x567990;

		static __declspec(naked) void HookPreFindActivationTarget() {
			_asm {
				mov eax, ds:[0x7C6CDC]  // global_TES3_Game
				mov eax, [eax + 0xE8]	// game->playerTarget
				mov TES3::Game::previousPlayerTarget, eax
				jmp MACP__getPlayerAnimData_fieldEC
			}
		}

		static void HookPostFindActivationTarget() {
			TES3::Reference *currentTarget = TES3::Game::get()->playerTarget;
			if (TES3::Game::previousPlayerTarget != currentTarget && event::ActivationTargetChangedEvent::getEventEnabled()) {
				LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::ActivationTargetChangedEvent(TES3::Game::previousPlayerTarget, currentTarget));
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
			if (event::WeatherCycledEvent::getEventEnabled()) {
				LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::WeatherCycledEvent());
			}

			// Call original function.
			return reinterpret_cast<bool (__thiscall *)(TES3::Cell*)>(0x4E22F0)(cell);
		}

		void __fastcall OnWeatherImmediateChange(TES3::WeatherController* controller, DWORD _UNUSED_, DWORD weatherId, DWORD transitionScalar) {
			// Call original function.
			reinterpret_cast<void (__thiscall *)(TES3::WeatherController*, DWORD, DWORD)>(0x441C40)(controller, weatherId, transitionScalar);

			// Fire off the event, after function completes.
			if (event::WeatherChangedImmediateEvent::getEventEnabled()) {
				LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::WeatherChangedImmediateEvent());
			}
		}

		void* __cdecl OnWeatherTransitionBegin(const char* texturePath, void* data) {
			// Fire off the event.
			if (event::WeatherTransitionStartedEvent::getEventEnabled()) {
				LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::WeatherTransitionStartedEvent());
			}

			// Call original function.
			return reinterpret_cast<void* (__cdecl *)(const char*, void*)>(0x6DE7F0)(texturePath, data);
		}

		void __fastcall OnWeatherTransitionEnd(void* modelData, DWORD _UNUSED_) {
			// Fire off the event.
			if (event::WeatherTransitionFinishedEvent::getEventEnabled()) {
				LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::WeatherTransitionFinishedEvent());
			}

			// Call original function.
			reinterpret_cast<void (__thiscall *)(void*)>(0x414890)(modelData);
		}

		//
		// Event: Music, new track
		//

		bool __fastcall OnSelectMusicTrack(TES3::WorldController* controller, DWORD _UNUSED_, int situation) {
			// Fire off the event.
			if (event::MusicSelectTrackEvent::getEventEnabled()) {
				auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
				sol::table eventData = stateHandle.triggerEvent(new event::MusicSelectTrackEvent(situation));
				if (eventData.valid()) {
					sol::optional<std::string> musicPath = eventData["music"];
					if (musicPath) {
						char* buffer = mwse::tes3::getThreadSafeStringBuffer();
						snprintf(buffer, 512, "Data Files/music/%s", musicPath.value().c_str());
						return true;
					}
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
			if (actor->reference && event::WeaponReadiedEvent::getEventEnabled()) {
				LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::WeaponReadiedEvent(actor->reference));
			}
		}

		const auto TES3_MobileActor_unreadyWeapon = reinterpret_cast<void(__thiscall*)(const TES3::MobileActor*)>(0x528000);

		void __fastcall OnUnreadyWeapon(TES3::MobileActor * mobile) {
			bool wasDrawn = (mobile->actorFlags & TES3::MobileActorFlag::WeaponDrawn);

			TES3_MobileActor_unreadyWeapon(mobile);

			if (mobile && mobile->reference && wasDrawn && event::WeaponUnreadiedEvent::getEventEnabled()) {
				LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::WeaponUnreadiedEvent(mobile->reference));
			}
		}

		//
		// Events: Leveled list resolving
		//

		TES3::IteratedList<TES3::ItemStack*>* __fastcall CacheContainerCloseReference(TES3::ContainerInstance* self) {
			mwse::lua::event::LeveledItemPickedEvent::m_Reference = self->getReference();
			return &self->inventory.itemStacks;
		}

		// We can't use the mapped function in TES3Inventory.h because of the custom inventory pointer made. That breaks getActor.
		// So we need to get a cached reference above.
		const auto TES3_Inventory_resolveLeveledLists = reinterpret_cast<void(__thiscall*)(TES3::Inventory*, TES3::MobileActor*)>(0x49A190);
		void __fastcall ContainerCloseResolveLevelledLists(TES3::Inventory* inventory, DWORD _EDX_, TES3::MobileActor* mobile) {
			TES3_Inventory_resolveLeveledLists(inventory, mobile);
			mwse::lua::event::LeveledItemPickedEvent::m_Reference = nullptr;
		}

		TES3::Reference* __fastcall CacheLeveledCreatureSpawner(TES3::Reference* reference) {
			// Overwritten code.
			auto result = reference->getLeveledBaseReference();

			if (result == nullptr) {
				event::LeveledCreaturePickedEvent::m_LastLeveledSourceReference = reference;
			}

			return result;
		}

		TES3::Object* __fastcall PickLeveledCreatureWithCachedSpawner(TES3::LeveledCreature* leveledList) {
			auto result = leveledList->resolve();

			// Clear cached spawning reference so it doesn't pollute other calls.
			event::LeveledCreaturePickedEvent::m_LastLeveledSourceReference = nullptr;

			return result;
		}

		TES3::Object* __fastcall PickLeveledCreatureForEmptyCell(TES3::LeveledCreature* leveledList) {
			event::LeveledCreaturePickedEvent::m_IsForEmptyCell = true;
			auto result = leveledList->resolve();
			event::LeveledCreaturePickedEvent::m_IsForEmptyCell = false;
			return result;
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

			if (event::CalculateBarterPriceEvent::getEventEnabled()) {
				auto& luaManager = mwse::lua::LuaManager::getInstance();
				auto stateHandle = luaManager.getThreadSafeStateHandle();
				sol::table result = stateHandle.triggerEvent(new event::CalculateBarterPriceEvent(mobile, basePrice, price, buying, count, OnCalculateBarterPrice_stack));
				if (result.valid()) {
					price = result["price"];
				}
			}

			return price;
		}

		//
		// Event: Calculate repair price.
		//

		// Cached value of the inventory iterator.
		static TES3::IteratedList<TES3::ItemStack*>* OnCalculateRepairPriceForList_CurrentInventoryList = nullptr;

		// Store the inventory list that we're looking at when generating the repair list.
		TES3::IteratedList<TES3::ItemStack*>::Node* __fastcall OnCalculateRepairPriceForList_GetItemList(TES3::IteratedList<TES3::ItemStack*>* inventoryList) {
			OnCalculateRepairPriceForList_CurrentInventoryList = inventoryList;

			return inventoryList->cached_begin();
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

			if (event::CalculateRepairPriceEvent::getEventEnabled()) {
				auto& luaManager = mwse::lua::LuaManager::getInstance();
				auto stateHandle = luaManager.getThreadSafeStateHandle();
				sol::table result = stateHandle.triggerEvent(new event::CalculateRepairPriceEvent(mobile, basePrice, price, OnCalculateRepairPriceForList_CurrentInventoryList->current->data));
				if (result.valid()) {
					price = result["price"];
				}
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

			if (event::CalculateRepairPriceEvent::getEventEnabled()) {
				auto& luaManager = mwse::lua::LuaManager::getInstance();
				auto stateHandle = luaManager.getThreadSafeStateHandle();
				sol::table result = stateHandle.triggerEvent(new event::CalculateRepairPriceEvent(mobile, basePrice, price, OnCalculateRepairPrice_ItemStack));
				if (result.valid()) {
					price = result["price"];
				}
			}

			return price;
		}

		//
		// Event: Calculate spell price.
		//

		// Cached value of the inventory iterator.
		static TES3::IteratedList<TES3::Spell*>* OnCalculateSpellPrice_CurrentInventoryList = nullptr;

		// Store the inventory list that we're looking at when generating the repair list.
		TES3::IteratedList<TES3::Spell*>::Node* __fastcall OnCalculateSpellPriceForList_GetSpellList(TES3::IteratedList<TES3::Spell*>* spellList) {
			OnCalculateSpellPrice_CurrentInventoryList = spellList;

			return spellList->cached_begin();
		}

		// Get the price for each item on the list.
		int __fastcall OnCalculateSpellPriceForList(TES3::MobileActor * mobile, DWORD _UNUSED_, int basePrice, bool buying) {
			int price = mobile->determineModifiedPrice(basePrice, buying);

			if (event::CalculateSpellPriceEvent::getEventEnabled()) {
				auto& luaManager = mwse::lua::LuaManager::getInstance();
				auto stateHandle = luaManager.getThreadSafeStateHandle();
				sol::table result = stateHandle.triggerEvent(new event::CalculateSpellPriceEvent(mobile, basePrice, price, OnCalculateSpellPrice_CurrentInventoryList->current->data));
				if (result.valid()) {
					price = result["price"];
				}
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

			if (event::CalculateSpellPriceEvent::getEventEnabled()) {
				auto& luaManager = mwse::lua::LuaManager::getInstance();
				auto stateHandle = luaManager.getThreadSafeStateHandle();
				sol::table result = stateHandle.triggerEvent(new event::CalculateSpellPriceEvent(mobile, basePrice, price, OnCalculateSpellPrice_Spell));
				if (result.valid()) {
					price = result["price"];
				}
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

			if (event::CalculateTrainingPriceEvent::getEventEnabled()) {
				auto& luaManager = mwse::lua::LuaManager::getInstance();
				auto stateHandle = luaManager.getThreadSafeStateHandle();
				sol::table result = stateHandle.triggerEvent(new event::CalculateTrainingPriceEvent(mobile, basePrice, price, OnCalculateTrainingPrice_SkillId));
				if (result.valid()) {
					price = result["price"];
				}
			}

			return price;
		}

		//
		// Event: Calculate travel price.
		//

		// The destination list for the merchant.
		static TES3::IteratedList<TES3::TravelDestination*> * OnCalculateTravelPrice_DestinationList;

		// The player's friendly actor list.
		static TES3::IteratedList<TES3::MobileActor*> * OnCalculateTravelPrice_CompanionList;

		// A custom list of followers close enough to travel with the player.
		static std::vector<TES3::MobileActor*> OnCalculateTravelPrice_TravelCompanionList;

		// Hook for ensuring that we have the right destination list.
		TES3::IteratedList<TES3::TravelDestination*>::Node* __fastcall OnCalculateTravelPrice_GetDestinationList(TES3::IteratedList<TES3::TravelDestination*>* iterator) {
			OnCalculateTravelPrice_DestinationList = iterator;

			return iterator->cached_begin();
		}

		// Hook for ensuring that we have the right companion list.
		TES3::IteratedList<TES3::MobileActor*>::Node* __fastcall OnCalculateTravelPrice_GetCompanionList(TES3::IteratedList<TES3::MobileActor*>* iterator) {
			OnCalculateTravelPrice_CompanionList = iterator;

			OnCalculateTravelPrice_TravelCompanionList.clear();

			return iterator->cached_begin();
		}

		// Hook for checking and adding companions for our custom list so we can report valid companions in the event.
		float OnCalculateTravelPrice_CheckCompanionDistance(TES3::Vector3* destinationPosition, TES3::Vector3* playerPosition) {
			float distance = destinationPosition->distance(playerPosition);
			if (OnCalculateTravelPrice_CompanionList->size() * 128.0f + 512.0f > distance) {
				OnCalculateTravelPrice_TravelCompanionList.push_back(OnCalculateTravelPrice_CompanionList->current->data);
			}
			return distance;
		}

		// Send off our travel price event when building the destination list.
		int __fastcall OnCalculateTravelPriceForList(TES3::MobileActor * mobile, DWORD _UNUSED_, int basePrice, bool buying) {
			int price = mobile->determineModifiedPrice(basePrice, buying);

			if (event::CalculateTravelPriceEvent::getEventEnabled()) {
				auto& luaManager = mwse::lua::LuaManager::getInstance();
				auto stateHandle = luaManager.getThreadSafeStateHandle();
				sol::table result = stateHandle.triggerEvent(new event::CalculateTravelPriceEvent(mobile, basePrice, price, OnCalculateTravelPrice_DestinationList->current->data, &OnCalculateTravelPrice_TravelCompanionList));
				if (result.valid()) {
					price = result["price"];
				}
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

			if (event::CalculateTravelPriceEvent::getEventEnabled()) {
				auto& luaManager = mwse::lua::LuaManager::getInstance();
				auto stateHandle = luaManager.getThreadSafeStateHandle();
				sol::table result = stateHandle.triggerEvent(new event::CalculateTravelPriceEvent(mobile, basePrice, price, destination, &OnCalculateTravelPrice_TravelCompanionList));
				if (result.valid()) {
					price = result["price"];
				}
			}

			return price;
		}

		//
		// Event: Skill raised.
		//

		const auto TES3_ShowSkillRaisedNotification = reinterpret_cast<void(__cdecl*)(int, char*)>(0x629FC0);

		void __cdecl OnSkillRaisedBook(int skillId, char* buffer) {
			TES3_ShowSkillRaisedNotification(skillId, buffer);

			if (event::SkillRaisedEvent::getEventEnabled()) {
				LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::SkillRaisedEvent(skillId, TES3::WorldController::get()->getMobilePlayer()->skills[skillId].base, "book"));
			}
		}

		void __cdecl OnSkillRaisedProgress(int skillId, char* buffer) {
			TES3_ShowSkillRaisedNotification(skillId, buffer);

			if (event::SkillRaisedEvent::getEventEnabled()) {
				LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::SkillRaisedEvent(skillId, TES3::WorldController::get()->getMobilePlayer()->skills[skillId].base, "progress"));
			}
		}

		// Skill and Id of the skill trained.
		static TES3::SkillStatistic * OnSkillTrained_Skill;
		static int OnSkillTrained_SkillId = -1;

		void __fastcall OnSkillTrained_GetSkill(TES3::SkillStatistic * skill, DWORD _UNDEFINED_, float delta, bool capAt0, bool capAt100) {
			skill->modSkillCapped(delta, capAt0, capAt100);

			OnSkillTrained_SkillId = -1;
			auto macp = TES3::WorldController::get()->getMobilePlayer();
			for (int i = TES3::SkillID::FirstSkill; i <= TES3::SkillID::LastSkill; i++) {
				if (&macp->skills[i] == skill) {
					OnSkillTrained_SkillId = i;
					break;
				}
			}
			OnSkillTrained_Skill = skill;
		}
		
		// OnSkillTrained_GetSkill is too soon for our event, because some relevant data structures haven't been updated yet.
		// Specifically, levelUpProgress, and levelupsPerAttribute are not updated until later.
		// Trigger the event when the UI updates instead, which happens immediately after levelupsPerAttribute is updated.

		void __cdecl OnSkillTrained_UpdateStatsPane() {
			TES3::UI::updateStatsPane();

			if (OnSkillTrained_SkillId == -1) {
				return;
			}

			if (event::SkillRaisedEvent::getEventEnabled()) {
				LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::SkillRaisedEvent(OnSkillTrained_SkillId, OnSkillTrained_Skill->base, "training"));
			}
			
			// This should be unnecessary, but let's be extra careful.
			OnSkillTrained_SkillId = -1;
		}

		void LuaManager::executeMainModScripts(const char* path, const char* filename) {
			if (!std::filesystem::exists(path)) {
				return;
			}

			for (auto & p : std::filesystem::recursive_directory_iterator(path, std::filesystem::directory_options::follow_directory_symlink)) {
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

			if (event::UiRefreshedEvent::getEventEnabled()) {
				LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::UiRefreshedEvent(element));
			}

			return result;
		}

		//
		// Event: Filter Inventory/Barter Tile
		//

		const auto TES3_FilterInventoryTile = reinterpret_cast<bool(__cdecl*)(TES3::UI::InventoryTile *, TES3::Item *)>(0x5CC720);

		bool __cdecl OnFilterInventoryTile(TES3::UI::InventoryTile * tile, TES3::Item * item) {
			if (event::FilterInventoryEvent::getEventEnabled()) {
				auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
				sol::table payload = stateHandle.triggerEvent(new event::FilterInventoryEvent(tile, item));
				if (payload.valid()) {
					sol::object filter = payload["filter"];
					if (filter.is<bool>()) {
						return filter.as<bool>();
					}
				}
			}

			return TES3_FilterInventoryTile(tile, item);
		}

		const auto TES3_FilterBarterTile = reinterpret_cast<bool(__cdecl*)(TES3::UI::InventoryTile *, TES3::Item *)>(0x5A5430);

		bool __cdecl OnFilterBarterTile(TES3::UI::InventoryTile * tile, TES3::Item * item) {
			if (event::FilterBarterMenuEvent::getEventEnabled()) {
				auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
				sol::table payload = stateHandle.triggerEvent(new event::FilterBarterMenuEvent(tile, item));
				if (payload.valid()) {
					sol::object filter = payload["filter"];
					if (filter.is<bool>()) {
						return filter.as<bool>();
					}
				}
			}

			return TES3_FilterBarterTile(tile, item);
		}

		void __fastcall OnFilterContentsTile(TES3::IteratedList<TES3::UI::InventoryTile*> * list, DWORD _UNUSUED_, TES3::UI::InventoryTile * tile) {
			if (event::FilterContentsMenuEvent::getEventEnabled()) {
				auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
				sol::table payload = stateHandle.triggerEvent(new event::FilterContentsMenuEvent(tile, tile->item));
				if (payload.valid()) {
					sol::object filter = payload["filter"];
					if (filter.is<bool>()) {
						if (!filter.as<bool>()) {
							return;
						}
					}
				}
			}

			list->push_back(tile);
		}

		void __fastcall OnFilterContentsTileForTakeAll(TES3::IteratedList<TES3::UI::InventoryTile*> * list, DWORD _UNUSUED_, TES3::UI::InventoryTile * tile) {
			if (event::FilterContentsMenuEvent::getEventEnabled()) {
				auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
				sol::table payload = stateHandle.triggerEvent(new event::FilterContentsMenuEvent(tile, tile->item));
				if (payload.valid()) {
					sol::object filter = payload["filter"];
					if (filter.is<bool>()) {
						if (!filter.as<bool>()) {
							return;
						}
					}
				}
			}

			list->push_back(tile);
			if (list->size() == 1) {
				TES3::WorldController::get()->playItemUpDownSound(tile->item);
			}
		}

		bool __fastcall OnFilterInventorySelect(TES3::UI::Element * element, TES3::UI::EventCallback callback) {
			TES3::Item * item = reinterpret_cast<TES3::Item*>(element->getProperty(TES3::UI::PropertyType::Pointer, *reinterpret_cast<TES3::UI::Property*>(0x7D3C88)).ptrValue);
			TES3::ItemData * itemData = reinterpret_cast<TES3::ItemData*>(element->getProperty(TES3::UI::PropertyType::Pointer, *reinterpret_cast<TES3::UI::Property*>(0x7D3C16)).ptrValue);

			if (event::FilterInventorySelectEvent::getEventEnabled()) {
				auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
				const char* callbackType = TES3::UI::getInventorySelectType();
				sol::table payload = stateHandle.triggerEvent(new event::FilterInventorySelectEvent(callbackType, item, itemData));
				if (payload.valid()) {
					sol::object filter = payload["filter"];
					if (filter.is<bool>()) {
						return filter.as<bool>();
					}
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
			if (mwse::lua::event::InfoGetTextEvent::getEventEnabled()) {
				auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
				sol::object eventResult = stateHandle.triggerEvent(new mwse::lua::event::InfoGetTextEvent(lastReadDialogueInfo));
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
			}

			return gameFile->readChunkData(dialogueTextBuffer, size);
		}

		//
		// Event: Dialogue Info response script
		//

		void __fastcall OnRunDialogueCommand(TES3::Script * script, DWORD _UNUSUED_, TES3::ScriptCompiler * compiler, const char* command, int source, TES3::Reference * reference, TES3::ScriptVariables * variables, TES3::DialogueInfo * info, TES3::Dialogue * dialogue) {
			// Allow the event to override the text.
			if (mwse::lua::event::InfoResponseEvent::getEventEnabled()) {
				auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
				sol::object eventResult = stateHandle.triggerEvent(new mwse::lua::event::InfoResponseEvent(command, reference, variables, dialogue, info));
				if (eventResult.valid()) {
					sol::table eventData = eventResult;
					if (eventData.get_or("block", false)) {
						return;
					}
				}
			}

			script->doCommand(compiler, command, source, reference, variables, info, dialogue);

			if (mwse::lua::event::PostInfoResponseEvent::getEventEnabled()) {
				mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::PostInfoResponseEvent(command, reference, variables, dialogue, info));
			}
		}

		//
		// Event: Dialogue link resolve topic.
		//

		TES3::Dialogue * __fastcall OnInfoLinkResolve(TES3::NonDynamicData * _this, DWORD _UNUSUED_, const char* topic) {
			if (mwse::lua::event::InfoLinkResolveEvent::getEventEnabled()) {
				auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
				sol::object eventResult = stateHandle.triggerEvent(new mwse::lua::event::InfoLinkResolveEvent(topic));
				if (eventResult.valid()) {
					sol::table eventData = eventResult;
					sol::object newTopic = eventData["topic"];
					if (newTopic.is<const char*>()) {
						return _this->findDialogue(newTopic.as<const char*>());
					}
				}
			}

			return _this->findDialogue(topic);
		}

		//
		// Event: Item Dropped.
		//

		void __fastcall OnItemDropped(TES3::DataHandler * dataHandler, DWORD _UNUSED_, TES3::Reference* reference) {
			// Call overwritten code.
			dataHandler->updateLightingForReference(reference);

			if (mwse::lua::event::ItemDroppedEvent::getEventEnabled()) {
				mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::ItemDroppedEvent(reference));
			}
		}

		//
		// Event: Calculate hit chance.
		//

		int __fastcall OnCalculateHitChance(TES3::MobileActor * attacker, int hitChance) {
			// Allow the event to override the text.
			if (mwse::lua::event::CalcHitChanceEvent::getEventEnabled()) {
				auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
				sol::object eventResult = stateHandle.triggerEvent(new mwse::lua::event::CalcHitChanceEvent(attacker, hitChance));
				if (eventResult.valid()) {
					sol::table eventData = eventResult;
					hitChance = getOptionalParam<int>(eventData, "hitChance", hitChance);
				}
			}

			// Overwritten code for this hook.
			if (TES3::WorldController::get()->menuController->unknown_0x24 % 1) {
				char* buffer = mwse::tes3::getThreadSafeStringBuffer();
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

			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::GenericEvent("fadersCreated"));
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
			if (lua::event::FilterSoulGemTargetEvent::getEventEnabled()) {
				auto& luaManager = mwse::lua::LuaManager::getInstance();
				auto stateHandle = luaManager.getThreadSafeStateHandle();
				sol::table payload = stateHandle.triggerEvent(new lua::event::FilterSoulGemTargetEvent(item, mact));
				if (payload.valid()) {
					sol::object filter = payload["filter"];
					if (filter.is<bool>()) {
						return filter.as<bool>();
					}
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
			if (actor->objectType != TES3::ObjectType::Creature) {
				return 0;
			}

			return static_cast<TES3::Creature*>(actor)->getSoulValue();
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

		TES3::IteratedList<TES3::UI::InventoryTile*>::Node* __fastcall GetNextInventoryTileToUpdate(TES3::IteratedList<TES3::UI::InventoryTile*> * iterator) {
			if (lua::event::ItemTileUpdatedEvent::getEventEnabled()) {
				lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new lua::event::ItemTileUpdatedEvent(iterator->current->data));
			}

			return iterator->cached_next();
		}

		void __inline TriggerItemTileUpdatedEventForElement(TES3::UI::Element * element, DWORD propertyAddress) {
			if (lua::event::ItemTileUpdatedEvent::getEventEnabled()) {
				TES3::UI::InventoryTile * tile = static_cast<TES3::UI::InventoryTile*>(element->getProperty(TES3::UI::PropertyType::Pointer, *reinterpret_cast<TES3::UI::Property*>(propertyAddress)).ptrValue);
				if (tile) {
					lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new lua::event::ItemTileUpdatedEvent(tile));
				}
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
		TES3::IteratedList<TES3::ItemStack*> * currentlySavingInventoryIterator = nullptr;
		unsigned int currentlySavingInventoyItemDataIndex = 0;

		// Get a hold of the inventory we're looking at.
		TES3::IteratedList<TES3::ItemStack*>::Node* __fastcall GetFirstSavedItemStack(TES3::IteratedList<TES3::ItemStack*> * iterator) {
			currentlySavingInventoryIterator = iterator;
			return iterator->cached_begin();
		}

		// Get a hold of the current index in ItemData storage we're looking at.
		int __fastcall WriteItemDataIndex(TES3::GameFile * gameFile, DWORD _UNUSED_, unsigned int tag, const unsigned int * data, unsigned int size) {
			currentlySavingInventoyItemDataIndex = *data;
			return gameFile->writeChunkData(tag, data, size);
		}

		sol::protected_function fnTableEmpty;
		sol::protected_function fnEncodeForSave;
		sol::protected_function fnDecodeFromSave;

		// The last extra data written. We'll add the lua data here if needed.
		int __fastcall WriteItemDataCondition(TES3::GameFile * gameFile, DWORD _UNUSED_, unsigned int tag, const void * data, unsigned int size) {
			// Overwritten code.
			int result = gameFile->writeChunkData(tag, data, size);

			TES3::ItemData * itemData = currentlySavingInventoryIterator->current->data->variables->at(currentlySavingInventoyItemDataIndex);
			if (itemData->luaData) {
				sol::table table = itemData->luaData->data;

				// If it is empty, don't bother saving it.
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				if (!fnTableEmpty(table, true)) {
					// Convert the table to json for storage.
					std::string json = fnEncodeForSave(table);

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
					auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
					sol::state &state = stateHandle.state;
					auto threadID = GetCurrentThreadId();
					auto saveLoadItemData = saveLoadItemDataMap[threadID];
					if (saveLoadItemData && saveLoadItemData->luaData == nullptr) {
						saveLoadItemData->setLuaDataTable(fnDecodeFromSave(buffer));
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
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				if (!fnTableEmpty(table, true)) {
					// Convert the table to json for storage.
					std::string json = fnEncodeForSave(table);

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
					auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
					sol::state &state = stateHandle.state;
					auto itemData = saveLoadReferenceMap[GetCurrentThreadId()]->getAttachedItemData();
					if (itemData) {
						if (itemData->luaData == nullptr) {
							itemData->setLuaDataTable(fnDecodeFromSave(buffer));
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
		// Patch bound item / summon saving and loading.
		//

		// Override the effect id, but only if this is a custom effect with a valid summon or bound item.
		short __stdcall PatchMagicSaveLoad_UpdateId(const TES3::EquipmentStack* const stack, short id) {
			if (id > TES3::EffectID::LastEffect && stack->object) {
				if (stack->object->objectType == TES3::ObjectType::Armor || stack->object->objectType == TES3::ObjectType::Weapon) {
					id = TES3::EffectID::BoundDagger;
				}
				else if (stack->object->objectType == TES3::ObjectType::Reference) {
					id = TES3::EffectID::SummonScamp;
				}
			}
			return id;
		}

		static DWORD postPatchMagicSaveLoad = TES3_PATCH_MAGIC_SAVE_LOAD_RETURN;

		// Intercept the effect id. If it's a custom effect and there's a summon or bound item, change it to an appropriate vanilla effect,
		// so that the game will select the appropriate branch of the switch. This will allow the game to properly serialize the bound item or 
		// summon associated with this effect.
		static __declspec(naked) short PatchMagicSaveLoad() {
			_asm {
				// Restore overwritten code. Read the id from the appropriate effect and place it in eax.
				mov ecx, dword ptr[esp + 0x20]
				movsx eax, word ptr[eax + ecx]

				// short id
				push eax

				// esi might be a TES3::EquipmentStack*
				// The game has already checked that esi is not null.
				push esi

				call PatchMagicSaveLoad_UpdateId

				// eax now contains the potentially updated effect id. Resume execution.
				jmp postPatchMagicSaveLoad
			}
		}

		//
		// Event: Crime witnessed.
		//

		TES3::MobileActor * lastProcessedCrimeActor = nullptr;

		const auto TES3_MobileActor_ProcessCrimes = reinterpret_cast<void(__thiscall*)(TES3::MobileActor*)>(0x522040);
		void __fastcall OnProcessCrimes(TES3::MobileActor * mact) {
			lastProcessedCrimeActor = mact;
			TES3_MobileActor_ProcessCrimes(mact);
		}

		void __fastcall OnCrimeWitnessedEnd(TES3::CrimeEvent * crimeEvent) {
			if (lua::event::CrimeWitnessedEvent::getEventEnabled()) {
				LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new lua::event::CrimeWitnessedEvent(lastProcessedCrimeActor, crimeEvent));
			}

			// Call overwritten code.
			crimeEvent->dtor();
		}

		//
		// Event: Barter Offered
		//

		static float lastBarterSkillProgress = 0.0f;

		// Store the last attempt to raise mercantile based on bartering, so we can deliver it slightly later.
		void __fastcall GameBarterOffer_BufferSkillGain(TES3::MobilePlayer* macp, DWORD _EDX_, TES3::SkillID::SkillID skill, float progress) {
			lastBarterSkillProgress = progress;
		}

		const auto TES3_GameBarterOffer = reinterpret_cast<bool(__stdcall*)()>(0x5A66C0);
		bool __stdcall GameBarterOffer() {
			// Reset skill progress.
			lastBarterSkillProgress = 0.0f;
			bool success = TES3_GameBarterOffer();

			// Fire off the event.
			if (lua::event::BarterOfferEvent::getEventEnabled()) {
				TES3::MobileActor* mact = TES3::UI::getServiceActor();
				int value = *reinterpret_cast<int*>(0x7D2878);
				int offer = *reinterpret_cast<int*>(0x7D287C);
				sol::object eventResult = LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new lua::event::BarterOfferEvent(mact, success, value, offer));
				if (eventResult.valid()) {
					sol::table eventData = eventResult;
					if (eventData.get_or("block", false)) {
						return false;
					}

					success = eventData.get_or("success", success);
				}
			}
			
			// Only exercise the skill if the barter was a success (and the event didn't block).
			if (success && lastBarterSkillProgress > 0.0f) {
				TES3::WorldController::get()->getMobilePlayer()->exerciseSkill(TES3::SkillID::Mercantile, lastBarterSkillProgress);
			}

			return success;
		}

		//
		// Event: Prevent Rest
		//

		float __fastcall OnCheckActionWeightFightForRest(TES3::MobileActor* self, DWORD _EDX_, TES3::MobilePlayer* player) {
			auto result = self->getActionWeightFight(player);

			if (event::PreventRestEvent::getEventEnabled() && result >= 100.0f) {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::object eventResult = stateHandle.triggerEvent(new event::PreventRestEvent(self));
				if (eventResult.valid()) {
					sol::table eventData = eventResult;
					if (eventData.get_or("block", false)) {
						return 0.0f;
					}
				}
			}

			return result;
		}

		//
		// Event: cell/reference activated/deactivated
		//

		// General cell activation.
		const auto TES3_DataHandler_AddMobilesToCell = reinterpret_cast<void(__thiscall*)(TES3::DataHandler*, TES3::Cell*)>(0x484E50);
		void __fastcall AddMobilesToCell(TES3::DataHandler* dataHandler, DWORD _EDX_, TES3::Cell* cell) {
			TES3_DataHandler_AddMobilesToCell(dataHandler, cell);
			if (cell) {
				cell->setCellActive();
			}
		}

		// General cell deactivation.
		const auto TES3_DataHandler_RemoveMobilesFromCell = reinterpret_cast<void(__thiscall*)(TES3::DataHandler*, TES3::Cell*)>(0x484D80);
		void __fastcall RemoveMobilesFromCell(TES3::DataHandler* dataHandler, DWORD _EDX_, TES3::Cell* cell) {
			TES3_DataHandler_RemoveMobilesFromCell(dataHandler, cell);
			if (cell) {
				cell->setCellInactive();
			}
		}

		void __cdecl ScriptRelocateReference(TES3::Reference* reference, TES3::Cell* cell, TES3::Vector3* position, float rotation) {
			reference->relocate(cell, position, rotation);
		}

		//
		// Event: Power recharged
		//

		using PowersHashMap = decltype(TES3::MobileActor::powers);
		void __fastcall OnDeletePowerHashMapKVP(PowersHashMap* self, DWORD edx, PowersHashMap::Node* node) {
			if (event::PowerRechargedEvent::getEventEnabled()) {
				auto mobile = reinterpret_cast<TES3::MobileActor*>(DWORD(self) - offsetof(TES3::MobileActor, powers));
				LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new lua::event::PowerRechargedEvent(node->key, mobile));
			}
		}

		//
		// Patch: Getting the correct radius of a light attached to a non-light entity.
		//
		
		__declspec(naked) void patchGetEntityLightRadius() {
			__asm {
				push ebx
				mov ebx, [esi+0x28]			// ebx = esi->Reference.baseEntity

				mov eax, [eax]				// eax = eax->RefrAttachment_Light.sgLight
				cmp dword ptr [ebx+4], 0x4847494C		// tag == 'LIGH'
				jnz non_light
				mov ecx, [ebx+0x64]			// ecx = ebx->EntityLight.radius
				jmp done

			non_light:
				fld dword ptr [eax+0xC4]	// Load radius from eax->NiLight.specular
				push ecx
				fistp dword ptr [esp]
				pop ecx						// Radius converted to integer

			done:
				mov edx, [esp+0x10]			// edx = <reference argument>
				nop
				push 1
				push 1
				push ecx
				push edx
				push eax
			}
		}
		const size_t patchGetEntityLightRadius_size = 0x2B;

		//
		// Patch: Play animation while retaining player control.
		// 

		// Patches ActorAnimData::ctor.
		__declspec(naked) void patchActorAnimInit() {
			__asm {
				// Initialize members ActorAnimData+0x98..0x9B in one op. Patch uses ActorAnimData+0x9B for new state.
				mov dword ptr [esi+0x98], 0xFF00FFFF
				nop
				nop
			}
		}
		const size_t patchActorAnimInit_size = 0xC;

		// Defer switching between first and third person during animation override.
		__declspec(naked) bool CheckTogglePOV() {
			__asm {
				xor al, al
				cmp [esi+0x9B], 0xFF // esi->PlayerAnimData.patchedOverrideState == 0xFF
				jnz done

				mov al, byte ptr [esi+0xE9] // al = esi->PlayerAnimData.togglePOV
			done:
				ret
			}
		}

		// Call our custom function when updating animation playback.
		void __fastcall ActorAnimController_selectMovementAnimAndUpdate(TES3::ActorAnimationController* animController, DWORD _UNUSED_, float deltaTime, bool flag) {
			animController->selectMovementAnimAndUpdate(deltaTime, flag);
		}

		//
		//
		//

		// Write errors to mwse.log as well.
		BOOL WINAPI WriteToWarningsFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped) {
			// Overwritten code.
			auto result = WriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);

			// Also log to mwse.log with stack trace.
			if (Configuration::LogWarningsWithLuaStack && LuaManager::getInstance().getReadOnlyStateView().stack_top() != 0) {
				auto trimmedWarning = std::move(string::trim_copy((const char*)lpBuffer));
				log::getLog() << "Morrowind has raised a warning with a lua stack trace: " << trimmedWarning << std::endl;
				logStackTrace();
			}

			return result;
		}

		//
		// Patch sourceless objects so they don't get deleted when drunk.
		//

		bool __fastcall SafeSourcelessDeleteFromRetiredEffect(TES3::Alchemy* alchemy) {
			return alchemy->sourceMod || alchemy->getSourceless();
		}
		
		//
		//
		//

		ThreadedStateHandle::ThreadedStateHandle(LuaManager * manager) : luaManager(manager), state(manager->luaState) {
			luaManager->claimLuaThread();
		}

		ThreadedStateHandle::~ThreadedStateHandle() {
			luaManager->releaseLuaThread();
		}

		sol::object ThreadedStateHandle::triggerEvent(event::BaseEvent* e) {
			return luaManager->triggerEvent(e);
		}

		//
		//
		//

		ThreadedStateHandle LuaManager::getThreadSafeStateHandle() {
			return ThreadedStateHandle(this);
		}

		const sol::state_view& LuaManager::getReadOnlyStateView() {
			return luaState;
		}

		// Override for how os.exit works to clear up a few system things.
		void customOSExit(sol::optional<int> code) {
			auto game = TES3::Game::get();
			if (game) {
				game->setGamma(1.0f);
			}

			_exit(code.value_or(1));
		}

		void LuaManager::hook() {
			// Execute mwse_init.lua
			try {
				sol::protected_function_result result = luaState.safe_script_file("Data Files\\MWSE\\core\\mwse_init.lua");
				if (!result.valid()) {
					sol::error error = result;
					log::getLog() << "[LuaManager] ERROR: Failed to initialize MWSE Lua interface." << std::endl << error.what() << std::endl;
					return;
				}
			}
			catch (sol::error& e) {
				log::getLog() << "[LuaManager] ERROR: Failed to initialize MWSE Lua interface." << std::endl << e.what() << std::endl;
				return;
			}

			// Bind libraries.
			bindMWSEMemoryUtil();
			bindMWSEStack();
			bindScriptUtil();
			bindStringUtil();
			bindTES3Util();

			// Alter existing libraries.
			luaState["os"]["exit"] = customOSExit;

			// Cache some often used functions.
			fnTableEmpty = luaState["table"]["empty"];
			fnEncodeForSave = luaState["mwse"]["encodeForSave"];
			fnDecodeFromSave = luaState["json"]["decode"];

			// Hook the RunScript function so we can intercept Lua scripts and invoke Lua code if needed.
			genJumpUnprotected(TES3_HOOK_RUNSCRIPT_LUACHECK, reinterpret_cast<DWORD>(HookRunScript), TES3_HOOK_RUNSCRIPT_LUACHECK_SIZE);

			// Hook the MACP creation functions to update lua variables that point to the player.
			genCallEnforced(0x5661A0, 0x4E5770, reinterpret_cast<DWORD>(OnPlayerReferenceAssigned));
			genCallEnforced(0x4C0180, 0x4E4510, reinterpret_cast<DWORD>(OnPlayerReferenceCreated));

			// Event: initialized. Hook just before we return successfully from where game data is loaded.
			genCallEnforced(0x4BB440, 0x47E280, reinterpret_cast<DWORD>(FinishInitialization));
			genCallEnforced(0x4BBC07, 0x47E280, reinterpret_cast<DWORD>(FinishInitialization));

			// Event: enterFrame. This hook can be in a couple of locations, because of MCP.
			genCallEnforced(0x41ABB0, 0x40F610, reinterpret_cast<DWORD>(EnterFrame));
			genCallEnforced(0x41ABA5, 0x40F610, reinterpret_cast<DWORD>(EnterFrame));

			// Event: equip.
			genCallEnforced(0x5CB8E7, 0x5CE130, reinterpret_cast<DWORD>(OnPCEquip));
			genCallEnforced(0x60E70F, 0x5CE130, reinterpret_cast<DWORD>(OnPCEquip));
			genCallEnforced(0x60E9BE, 0x5CE130, reinterpret_cast<DWORD>(OnPCEquip));
			// ui_inventoryEquipItemToPlayer calls
			genCallEnforced(0x5E4399, 0x5D1190, reinterpret_cast<DWORD>(OnPCEquipItemDoubled)); //magic menu
			genCallEnforced(0x5E43A0, 0x5D1190, reinterpret_cast<DWORD>(OnPCEquipItemDoubledFollowUp)); //magic menu
			genCallEnforced(0x60878B, 0x5D1190, reinterpret_cast<DWORD>(OnPCEquipItem)); //quick slots
			// TODO: cosmetic issue when readying enchantments- item name notification pops up even when the equip is blocked.

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

			// Event: bodyPartsUpdated.
			genCallEnforced(0x4E6BFB, 0x4E8B50, reinterpret_cast<DWORD>(OnUpdateBipedParts));
			genCallEnforced(0x508CFC, 0x4E8B50, reinterpret_cast<DWORD>(OnUpdateBipedParts));
			genCallEnforced(0x508D86, 0x4E8B50, reinterpret_cast<DWORD>(OnUpdateBipedParts));
			genCallEnforced(0x508DE8, 0x4E8B50, reinterpret_cast<DWORD>(OnUpdateBipedParts));
			genCallEnforced(0x5240CE, 0x4E8B50, reinterpret_cast<DWORD>(OnUpdateBipedParts));
			genCallEnforced(0x52C6E6, 0x4E8B50, reinterpret_cast<DWORD>(OnUpdateBipedParts));
			genCallEnforced(0x52C95C, 0x4E8B50, reinterpret_cast<DWORD>(OnUpdateBipedParts));
			genCallEnforced(0x567D17, 0x4E8B50, reinterpret_cast<DWORD>(OnUpdateBipedParts));
			genCallEnforced(0x567D1F, 0x4E8B50, reinterpret_cast<DWORD>(OnUpdateBipedParts));
			genCallEnforced(0x56AA8E, 0x4E8B50, reinterpret_cast<DWORD>(OnUpdateBipedParts));
			genCallEnforced(0x56AAC0, 0x4E8B50, reinterpret_cast<DWORD>(OnUpdateBipedParts));
			genCallEnforced(0x56B0DE, 0x4E8B50, reinterpret_cast<DWORD>(OnUpdateBipedParts));
			genCallEnforced(0x56B110, 0x4E8B50, reinterpret_cast<DWORD>(OnUpdateBipedParts));
			genCallEnforced(0x5D054F, 0x4E8B50, reinterpret_cast<DWORD>(OnUpdateBipedParts));
			genCallEnforced(0x5D0583, 0x4E8B50, reinterpret_cast<DWORD>(OnUpdateBipedParts));
			genCallEnforced(0x5D0A1D, 0x4E8B50, reinterpret_cast<DWORD>(OnUpdateBipedParts));
			genCallEnforced(0x5D0A4F, 0x4E8B50, reinterpret_cast<DWORD>(OnUpdateBipedParts));

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
			genCallEnforced(0x541489, TES3_ActorAnimController_attackCheckMeleeHit, reinterpret_cast<DWORD>(OnAttack));
			genCallEnforced(0x5414CD, TES3_ActorAnimController_attackCheckMeleeHit, reinterpret_cast<DWORD>(OnAttack));
			genCallEnforced(0x569E78, TES3_ActorAnimController_attackCheckMeleeHit, reinterpret_cast<DWORD>(OnAttack));

			// Event: Begin physical attack
			genCallEnforced(0x54A4BA, 0x5411C0, reinterpret_cast<DWORD>(OnAttackStart));
			genCallEnforced(0x569DFB, 0x5411C0, reinterpret_cast<DWORD>(OnAttackStart));

			// Collision events: Mobile Object
			auto mobileObjectCollideActor = &TES3::MobileObject::onActorCollision;
			auto mobileObjectCollidObject = &TES3::MobileObject::onObjectCollision;
			auto mobileObjectCollideTerrain = &TES3::MobileObject::onTerrainCollision;
			auto mobileObjectCollideWater = &TES3::MobileObject::onWaterCollision;
			auto mobileObjectCollideActivator = &TES3::MobileObject::onActivatorCollision;
			overrideVirtualTableEnforced(0x74B0C0, 0x80, 0x5615A0, *reinterpret_cast<DWORD*>(&mobileObjectCollideActor));
			overrideVirtualTableEnforced(0x74B0C0, 0x84, 0x5615C0, *reinterpret_cast<DWORD*>(&mobileObjectCollidObject));
			overrideVirtualTableEnforced(0x74B0C0, 0x88, 0x5615E0, *reinterpret_cast<DWORD*>(&mobileObjectCollideTerrain));
			overrideVirtualTableEnforced(0x74B0C0, 0x8C, 0x5615E0, *reinterpret_cast<DWORD*>(&mobileObjectCollideTerrain));
			overrideVirtualTableEnforced(0x74B0C0, 0x90, 0x561600, *reinterpret_cast<DWORD*>(&mobileObjectCollideActivator));

			// Collision events: Mobile Actor
			auto mobileActorCollideActor = &TES3::MobileActor::onActorCollision;
			auto mobileActorCollidObject = &TES3::MobileActor::onObjectCollision;
			auto mobileActorCollideTerrain = &TES3::MobileActor::onTerrainCollision;
			auto mobileActorCollideActivator = &TES3::MobileActor::onActivatorCollision;
			overrideVirtualTableEnforced(0x74AB4C, 0x80, 0x5234A0, *reinterpret_cast<DWORD*>(&mobileActorCollideActor));
			overrideVirtualTableEnforced(0x74AB4C, 0x84, 0x5233B0, *reinterpret_cast<DWORD*>(&mobileActorCollidObject));
			overrideVirtualTableEnforced(0x74AB4C, 0x88, 0x523310, *reinterpret_cast<DWORD*>(&mobileActorCollideTerrain));
			overrideVirtualTableEnforced(0x74AB4C, 0x8C, 0x5615E0, *reinterpret_cast<DWORD*>(&mobileObjectCollideTerrain));
			overrideVirtualTableEnforced(0x74AB4C, 0x90, 0x523590, *reinterpret_cast<DWORD*>(&mobileActorCollideActivator));

			// Collision events: Mobile Creature
			overrideVirtualTableEnforced(0x74AFA4, 0x80, 0x5234A0, *reinterpret_cast<DWORD*>(&mobileActorCollideActor));
			overrideVirtualTableEnforced(0x74AFA4, 0x84, 0x5233B0, *reinterpret_cast<DWORD*>(&mobileActorCollidObject));
			overrideVirtualTableEnforced(0x74AFA4, 0x88, 0x523310, *reinterpret_cast<DWORD*>(&mobileActorCollideTerrain));
			overrideVirtualTableEnforced(0x74AFA4, 0x8C, 0x5615E0, *reinterpret_cast<DWORD*>(&mobileObjectCollideTerrain));
			overrideVirtualTableEnforced(0x74AFA4, 0x90, 0x523590, *reinterpret_cast<DWORD*>(&mobileActorCollideActivator));

			// Collision events: Mobile NPC
			overrideVirtualTableEnforced(0x74AE6C, 0x80, 0x5234A0, *reinterpret_cast<DWORD*>(&mobileActorCollideActor));
			overrideVirtualTableEnforced(0x74AE6C, 0x84, 0x5233B0, *reinterpret_cast<DWORD*>(&mobileActorCollidObject));
			overrideVirtualTableEnforced(0x74AE6C, 0x88, 0x523310, *reinterpret_cast<DWORD*>(&mobileActorCollideTerrain));
			overrideVirtualTableEnforced(0x74AE6C, 0x8C, 0x5615E0, *reinterpret_cast<DWORD*>(&mobileObjectCollideTerrain));
			overrideVirtualTableEnforced(0x74AE6C, 0x90, 0x523590, *reinterpret_cast<DWORD*>(&mobileActorCollideActivator));

			// Collision events: Mobile Player
			overrideVirtualTableEnforced(0x74B174, 0x80, 0x5234A0, *reinterpret_cast<DWORD*>(&mobileActorCollideActor));
			overrideVirtualTableEnforced(0x74B174, 0x84, 0x5233B0, *reinterpret_cast<DWORD*>(&mobileActorCollidObject));
			overrideVirtualTableEnforced(0x74B174, 0x88, 0x523310, *reinterpret_cast<DWORD*>(&mobileActorCollideTerrain));
			overrideVirtualTableEnforced(0x74B174, 0x8C, 0x5615E0, *reinterpret_cast<DWORD*>(&mobileObjectCollideTerrain));
			overrideVirtualTableEnforced(0x74B174, 0x90, 0x523590, *reinterpret_cast<DWORD*>(&mobileActorCollideActivator));

			// Collision events: Mobile Projectile
			auto mobileProjectileCollideActor = &TES3::MobileProjectile::onActorCollision;
			auto mobileProjectileCollideObject = &TES3::MobileProjectile::onObjectCollision;
			auto mobileProjectileCollideTerrain = &TES3::MobileProjectile::onTerrainCollision;
			auto mobileProjectileCollideWater = &TES3::MobileProjectile::onWaterCollision;
			overrideVirtualTableEnforced(0x74B2B4, 0x80, 0x573860, *reinterpret_cast<DWORD*>(&mobileProjectileCollideActor));
			overrideVirtualTableEnforced(0x74B2B4, 0x84, 0x573820, *reinterpret_cast<DWORD*>(&mobileProjectileCollideObject));
			overrideVirtualTableEnforced(0x74B2B4, 0x88, 0x5737F0, *reinterpret_cast<DWORD*>(&mobileProjectileCollideTerrain));
			overrideVirtualTableEnforced(0x74B2B4, 0x8C, 0x573790, *reinterpret_cast<DWORD*>(&mobileProjectileCollideWater));
			overrideVirtualTableEnforced(0x74B2B4, 0x90, 0x561600, *reinterpret_cast<DWORD*>(&mobileObjectCollideActivator));

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
			genCallEnforced(0x50B72C, 0x557CF0, reinterpret_cast<DWORD>(OnApplyDamageFromScript));
			genCallEnforced(0x50B7AB, 0x557CF0, reinterpret_cast<DWORD>(OnApplyDamageFromScript));
			genCallEnforced(0x50D138, 0x557CF0, reinterpret_cast<DWORD>(OnApplyDamageFromScript));
			genCallEnforced(0x50D1B4, 0x557CF0, reinterpret_cast<DWORD>(OnApplyDamageFromScript));
			genCallEnforced(0x524884, 0x557CF0, reinterpret_cast<DWORD>(OnApplyDamageFromSuffocation));
			genCallEnforced(0x52978F, 0x557CF0, reinterpret_cast<DWORD>(OnApplyDamageFromFalling));
			genCallEnforced(0x5299CB, 0x557CF0, reinterpret_cast<DWORD>(OnApplyDamageFromSuffocation));
			genCallEnforced(0x555789, 0x557CF0, reinterpret_cast<DWORD>(OnApplyDamageFromMagicShield_Wrapper));
			genCallEnforced(0x556AE0, 0x557CF0, reinterpret_cast<DWORD>(OnApplyDamageFromAttack));
			genCallEnforced(0x55782C, 0x557CF0, reinterpret_cast<DWORD>(OnApplyDamageFromMagic));
			auto mobileActorApplyPhysicalHit = &TES3::MobileActor::applyPhysicalHit;
			genCallEnforced(0x5577C6, 0x5568F0, *reinterpret_cast<DWORD*>(&mobileActorApplyPhysicalHit));
			genCallEnforced(0x573C51, 0x5568F0, *reinterpret_cast<DWORD*>(&mobileActorApplyPhysicalHit));
			genCallEnforced(0x518526, 0x519110, reinterpret_cast<DWORD>(AttributeSpellEffect));
			genCallEnforced(0x51889D, 0x519110, reinterpret_cast<DWORD>(AttributeSpellEffect));
			genCallEnforced(0x518D5C, 0x519110, reinterpret_cast<DWORD>(AttributeSpellEffect));
			genCallEnforced(0x518F9F, 0x519110, reinterpret_cast<DWORD>(AttributeSpellEffect));

			// Event: Damage(d)HandToHand
			genCallEnforced(0x5576D4, 0x5581B0, reinterpret_cast<DWORD>(OnApplyFatigueDamageFromAttack_Wrapper));

			// Event: Spell cast resolution
			genCallEnforced(0x5156B2, 0x4AA950, reinterpret_cast<DWORD>(OnSpellCastResolution));

			// Event: Magic cast success
			genJumpUnprotected(TES3_HOOK_MAGIC_CAST_SUCCESS, reinterpret_cast<DWORD>(HookMagicCastSuccess), TES3_HOOK_MAGIC_CAST_SUCCESS_SIZE);

			// Event: Spell cast failure
			genJumpUnprotected(TES3_HOOK_SPELL_CAST_FAILURE, reinterpret_cast<DWORD>(HookSpellCastFailure), TES3_HOOK_SPELL_CAST_FAILURE_SIZE);

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

			// Event: Brew potion (failed).
			genCallEnforced(0x59C010, 0x59C030, reinterpret_cast<DWORD>(OnBrewPotionAttempt));
			genCallEnforced(0x59D2A9, 0x6313E0, reinterpret_cast<DWORD>(CacheLastBrewedPotion));

			// Event: Create enchanted item (failed).
			genCallUnprotected(0x5C494B, reinterpret_cast<DWORD>(CacheLastEnchantedItem), 0x6); // ARMO
			genCallUnprotected(0x5C49EF, reinterpret_cast<DWORD>(CacheLastEnchantedItem), 0x6); // WEAP/AMMO
			genCallUnprotected(0x5C4A96, reinterpret_cast<DWORD>(CacheLastEnchantedItem), 0x6); // CLOT
			genCallUnprotected(0x5C4B3A, reinterpret_cast<DWORD>(CacheLastEnchantedItem), 0x6); // BOOK
			genPushEnforced(0x5C28D8, reinterpret_cast<DWORD>(OnCreateEnchantedItemAttempt));

			// Clean unused alchemy attribute and skill IDs on loading.
			auto alchemyLoadObjectSpecific = &TES3::Alchemy::loadObjectSpecific;
			overrideVirtualTableEnforced(0x749684, offsetof(TES3::BaseObjectVirtualTable, TES3::BaseObjectVirtualTable::loadObjectSpecific), 0x4ABD90, *reinterpret_cast<DWORD*>(&alchemyLoadObjectSpecific));

			// Event: Spell created from service menu.
			genCallEnforced(0x622D05, 0x4B8980, reinterpret_cast<DWORD>(OnAddNewlyCreatedSpell));

			// Event: Player is about to level.
			genCallEnforced(0x5DA091, 0x5D90A0, reinterpret_cast<DWORD>(OnPreLevelUp));

			// Event: Player leveled.
			genCallEnforced(0x5DA620, 0x626220, reinterpret_cast<DWORD>(OnLevelUp));

			// Event: Calculate movement speed. Called after the below speed events.
			auto calculateMoveSpeed = &TES3::ActorAnimationController::calculateMovementSpeed;
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
			genCallEnforced(0x5B2E56, 0x595350, reinterpret_cast<DWORD>(OnUICreatedAfterConsoleMenuShown)); // MenuConsole
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
			genCallEnforced(0x5E63BF, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuMagic
			genCallEnforced(0x5E75C6, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuMagicSelect
			genCallEnforced(0x5E9789, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuMap
			genCallEnforced(0x5F0A97, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuMapNoteEdit
			genCallEnforced(0x5F1ED6, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuMessage
			genCallEnforced(0x5F4823, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuMulti
			genCallEnforced(0x5F8634, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuName
			genCallEnforced(0x5FCA28, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuOptions
			genCallEnforced(0x5FF250, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuOptions
			genCallEnforced(0x60016F, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuPersuasion, without MCP Persuasion Improvements
			genCallEnforced(0x600174, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuPersuasion, with MCP Persuation Improvements
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
			auto displayObjectTooltip = &TES3::UI::MenuInputController::displayObjectTooltip;
			genCallEnforced(0x41CC2E, 0x590D90, *reinterpret_cast<DWORD*>(&displayObjectTooltip));
			genCallEnforced(0x58FF1D, 0x590D90, *reinterpret_cast<DWORD*>(&displayObjectTooltip));
			genCallEnforced(0x59E10D, 0x590D90, *reinterpret_cast<DWORD*>(&displayObjectTooltip));
			genCallEnforced(0x5A7633, 0x590D90, *reinterpret_cast<DWORD*>(&displayObjectTooltip));
			genCallEnforced(0x5B6FD1, 0x590D90, *reinterpret_cast<DWORD*>(&displayObjectTooltip));
			genCallEnforced(0x5C663C, 0x590D90, *reinterpret_cast<DWORD*>(&displayObjectTooltip));
			genCallEnforced(0x5CE054, 0x590D90, *reinterpret_cast<DWORD*>(&displayObjectTooltip));
			genCallEnforced(0x5CE071, 0x590D90, *reinterpret_cast<DWORD*>(&displayObjectTooltip));
			genCallEnforced(0x5D24B2, 0x590D90, *reinterpret_cast<DWORD*>(&displayObjectTooltip));
			genCallEnforced(0x5D4B5C, 0x590D90, *reinterpret_cast<DWORD*>(&displayObjectTooltip));
			genCallEnforced(0x5E4FAD, 0x590D90, *reinterpret_cast<DWORD*>(&displayObjectTooltip));
			genCallEnforced(0x5E802D, 0x590D90, *reinterpret_cast<DWORD*>(&displayObjectTooltip));
			genCallEnforced(0x5F6F78, 0x590D90, *reinterpret_cast<DWORD*>(&displayObjectTooltip));
			genCallEnforced(0x607C33, 0x590D90, *reinterpret_cast<DWORD*>(&displayObjectTooltip));
			genCallEnforced(0x607C86, 0x590D90, *reinterpret_cast<DWORD*>(&displayObjectTooltip));
			genCallEnforced(0x607CA7, 0x590D90, *reinterpret_cast<DWORD*>(&displayObjectTooltip));
			genCallEnforced(0x60EE6B, 0x590D90, *reinterpret_cast<DWORD*>(&displayObjectTooltip));
			genCallEnforced(0x61550D, 0x590D90, *reinterpret_cast<DWORD*>(&displayObjectTooltip));

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
			auto inventoryResolveLeveledLists = &TES3::Inventory::resolveLeveledLists;
			genCallEnforced(0x49D5C4, 0x49A190, *reinterpret_cast<DWORD*>(&inventoryResolveLeveledLists));
			genCallEnforced(0x4A42A9, 0x49A190, *reinterpret_cast<DWORD*>(&inventoryResolveLeveledLists));
			genCallEnforced(0x4A4492, 0x4957E0, reinterpret_cast<DWORD>(CacheContainerCloseReference));
			genCallEnforced(0x4A44F8, 0x49A190, reinterpret_cast<DWORD>(ContainerCloseResolveLevelledLists));
			genCallEnforced(0x4D83A1, 0x49A190, *reinterpret_cast<DWORD*>(&inventoryResolveLeveledLists));
			genCallEnforced(0x508BB2, 0x49A190, *reinterpret_cast<DWORD*>(&inventoryResolveLeveledLists));
			genCallEnforced(0x529B72, 0x49A190, *reinterpret_cast<DWORD*>(&inventoryResolveLeveledLists));

			// Event: Leveled creature picked.
			auto leveledCreaturePick = &TES3::LeveledCreature::resolve;
			genCallEnforced(0x4B8C95, 0x4CF870, reinterpret_cast<DWORD>(PickLeveledCreatureForEmptyCell));
			genCallEnforced(0x4B8E80, 0x4CF870, reinterpret_cast<DWORD>(PickLeveledCreatureForEmptyCell));
			genCallEnforced(0x4CF9E7, 0x4CF870, *reinterpret_cast<DWORD*>(&leveledCreaturePick));
			genCallEnforced(0x4CFB34, 0x4E7EE0, reinterpret_cast<DWORD>(CacheLeveledCreatureSpawner));
			genCallEnforced(0x4CFB43, 0x4CF870, reinterpret_cast<DWORD>(PickLeveledCreatureWithCachedSpawner));
			genCallEnforced(0x635236, 0x4CF870, *reinterpret_cast<DWORD*>(&leveledCreaturePick));

			// Event: Mobile Sneak Detection.
			auto processManagerDetectSneak = &TES3::ProcessManager::detectSneak;
			genCallEnforced(0x558975, 0x570600, *reinterpret_cast<DWORD*>(&processManagerDetectSneak));
			genCallEnforced(0x570A43, 0x570600, *reinterpret_cast<DWORD*>(&processManagerDetectSneak));
			genCallEnforced(0x570C0B, 0x570600, *reinterpret_cast<DWORD*>(&processManagerDetectSneak));
			genCallEnforced(0x570E48, 0x570600, *reinterpret_cast<DWORD*>(&processManagerDetectSneak));

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
			genCallEnforced(0x4A28C6, 0x629FC0, reinterpret_cast<DWORD>(OnSkillRaisedBook));
			genCallEnforced(0x56BCF2, 0x629FC0, reinterpret_cast<DWORD>(OnSkillRaisedProgress));
			genCallEnforced(0x618355, 0x401060, reinterpret_cast<DWORD>(OnSkillTrained_GetSkill));
			genCallEnforced(0x6183A1, 0x6266D0, reinterpret_cast<DWORD>(OnSkillTrained_UpdateStatsPane));

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
			genCallEnforced(0x4B1FB2, 0x50E5A0, reinterpret_cast<DWORD>(OnRunDialogueCommand)); // Vanilla function.
			genJumpEnforced(0x50E594, 0x50E5A0, reinterpret_cast<DWORD>(OnRunDialogueCommand)); // MCP-added function.

			// Event: Dialogue link resolve.
			genCallEnforced(0x40B89E, 0x4BA8D0, reinterpret_cast<DWORD>(OnInfoLinkResolve));

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

			// Patch reading correct light culling radius from non-light entities during light updates.
			writePatchCodeUnprotected(0x485DAD, (BYTE*)&patchGetEntityLightRadius, patchGetEntityLightRadius_size);

			// Allow hand-to-hand animation speed to be changed.
			writeByteUnprotected(0x46CAD7, 0x89);

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
			overrideVirtualTableEnforced(0x748258, offsetof(TES3::ObjectVirtualTable, getTypeName), 0x4A1270, *reinterpret_cast<DWORD*>(&armorGetSlotName));

			// Allow overriding of clothing slot name.
			auto clothingGetSlotName = &TES3::Clothing::getSlotName;
			overrideVirtualTableEnforced(0x748634, offsetof(TES3::ObjectVirtualTable, getTypeName), 0x4A38E0, *reinterpret_cast<DWORD*>(&clothingGetSlotName));

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

			// Patch ui_reenableMenuDialogue
			genCallEnforced(0x5A3E76, 0x5C0B60, reinterpret_cast<DWORD>(ReenableMenuDialogue));
			genCallEnforced(0x5A6DF6, 0x5C0B60, reinterpret_cast<DWORD>(ReenableMenuDialogue));
			genCallEnforced(0x5A6F48, 0x5C0B60, reinterpret_cast<DWORD>(ReenableMenuDialogue));
			genCallEnforced(0x5A72C7, 0x5C0B60, reinterpret_cast<DWORD>(ReenableMenuDialogue));
			genCallEnforced(0x5B774F, 0x5C0B60, reinterpret_cast<DWORD>(ReenableMenuDialogue));
			genCallEnforced(0x5C34D7, 0x5C0B60, reinterpret_cast<DWORD>(ReenableMenuDialogue));
			genCallEnforced(0x5C4D22, 0x5C0B60, reinterpret_cast<DWORD>(ReenableMenuDialogue));
			genCallEnforced(0x61555C, 0x5C0B60, reinterpret_cast<DWORD>(ReenableMenuDialogue));
			genCallEnforced(0x615841, 0x5C0B60, reinterpret_cast<DWORD>(ReenableMenuDialogue));
			genCallEnforced(0x61666C, 0x5C0B60, reinterpret_cast<DWORD>(ReenableMenuDialogue));
			genCallEnforced(0x6167DF, 0x5C0B60, reinterpret_cast<DWORD>(ReenableMenuDialogue));
			genCallEnforced(0x61816C, 0x5C0B60, reinterpret_cast<DWORD>(ReenableMenuDialogue));
			genCallEnforced(0x6183E1, 0x5C0B60, reinterpret_cast<DWORD>(ReenableMenuDialogue));
			genCallEnforced(0x6195CC, 0x5C0B60, reinterpret_cast<DWORD>(ReenableMenuDialogue));
			genCallEnforced(0x621CCF, 0x5C0B60, reinterpret_cast<DWORD>(ReenableMenuDialogue));
			genCallEnforced(0x622DBE, 0x5C0B60, reinterpret_cast<DWORD>(ReenableMenuDialogue));

			// Mesh loaded event.
			auto meshDataLoadMesh = &TES3::MeshData::loadMesh;
			genCallEnforced(0x4194DB, 0x4EE0A0, *reinterpret_cast<DWORD*>(&meshDataLoadMesh));
			genCallEnforced(0x4C29C7, 0x4EE0A0, *reinterpret_cast<DWORD*>(&meshDataLoadMesh));
			genCallEnforced(0x4C2CE8, 0x4EE0A0, *reinterpret_cast<DWORD*>(&meshDataLoadMesh));
			genCallEnforced(0x4F0445, 0x4EE0A0, *reinterpret_cast<DWORD*>(&meshDataLoadMesh));
			genCallEnforced(0x4F053D, 0x4EE0A0, *reinterpret_cast<DWORD*>(&meshDataLoadMesh));
			genCallEnforced(0x4F13B8, 0x4EE0A0, *reinterpret_cast<DWORD*>(&meshDataLoadMesh));
			genCallEnforced(0x54B48F, 0x4EE0A0, *reinterpret_cast<DWORD*>(&meshDataLoadMesh));
			genCallEnforced(0x57BB39, 0x4EE0A0, *reinterpret_cast<DWORD*>(&meshDataLoadMesh));

			// Event: CrimeWitnessed
			genCallEnforced(0x521DB2, 0x522040, reinterpret_cast<DWORD>(OnProcessCrimes));
			genCallEnforced(0x53184A, 0x51F580, reinterpret_cast<DWORD>(OnCrimeWitnessedEnd));

			// Event: Reference Scene Graph Node Created
			auto referenceGetSceneGraphNode = &TES3::Reference::getSceneGraphNode;
			overrideVirtualTableEnforced(0x74A140, 0x2C, 0x4E81A0, *reinterpret_cast<DWORD*>(&referenceGetSceneGraphNode));

			// Event: Convert reference to item.
			auto inventoryAddItemByReference = &TES3::Inventory::addItemByReference;
			genCallEnforced(0x49582F, 0x497BC0, *reinterpret_cast<DWORD*>(&inventoryAddItemByReference));
			genCallEnforced(0x49586B, 0x497BC0, *reinterpret_cast<DWORD*>(&inventoryAddItemByReference));
			genCallEnforced(0x4DDF71, 0x497BC0, *reinterpret_cast<DWORD*>(&inventoryAddItemByReference));
			genCallEnforced(0x52C441, 0x497BC0, *reinterpret_cast<DWORD*>(&inventoryAddItemByReference));
			genCallEnforced(0x573E46, 0x497BC0, *reinterpret_cast<DWORD*>(&inventoryAddItemByReference));
			genCallEnforced(0x5A64CD, 0x497BC0, *reinterpret_cast<DWORD*>(&inventoryAddItemByReference));

			// Event: Calc Armor Piece Hit.
			auto mobileNPCApplyArmorRating = &TES3::MobileNPC::applyArmorRating;
			overrideVirtualTableEnforced(0x74AE6C, 0xE0, 0x54D820, *reinterpret_cast<DWORD*>(&mobileNPCApplyArmorRating)); // MACH
			overrideVirtualTableEnforced(0x74B174, 0xE0, 0x54D820, *reinterpret_cast<DWORD*>(&mobileNPCApplyArmorRating)); // MACP

			// Events: disarmTrap/pickLock
			auto referenceAttemptUnlockDisarm = &TES3::Reference::attemptUnlockDisarm;
			genCallEnforced(0x569A62, 0x4EB160, *reinterpret_cast<DWORD*>(&referenceAttemptUnlockDisarm));

			// Event: containerClosed.
			auto actorOnCloseInventory = &TES3::Actor::onCloseInventory;
			auto containerOnCloseInventory = &TES3::ContainerInstance::onCloseInventory;
			overrideVirtualTableEnforced(TES3::VirtualTableAddress::NPCBase, offsetof(TES3::ActorVirtualTable, onCloseInventory), 0x58D230, *reinterpret_cast<DWORD*>(&actorOnCloseInventory));
			overrideVirtualTableEnforced(TES3::VirtualTableAddress::NPCInstance, offsetof(TES3::ActorVirtualTable, onCloseInventory), 0x58D230, *reinterpret_cast<DWORD*>(&actorOnCloseInventory));
			overrideVirtualTableEnforced(TES3::VirtualTableAddress::CreatureBase, offsetof(TES3::ActorVirtualTable, onCloseInventory), 0x58D230, *reinterpret_cast<DWORD*>(&actorOnCloseInventory));
			overrideVirtualTableEnforced(TES3::VirtualTableAddress::CreatureInstance, offsetof(TES3::ActorVirtualTable, onCloseInventory), 0x58D230, *reinterpret_cast<DWORD*>(&actorOnCloseInventory));
			overrideVirtualTableEnforced(TES3::VirtualTableAddress::ContainerBase, offsetof(TES3::ActorVirtualTable, onCloseInventory), 0x58D230, *reinterpret_cast<DWORD*>(&actorOnCloseInventory));
			overrideVirtualTableEnforced(TES3::VirtualTableAddress::ContainerInstance, offsetof(TES3::ActorVirtualTable, onCloseInventory), 0x4A4460, *reinterpret_cast<DWORD*>(&containerOnCloseInventory));

			// Allow overriding of guard status.
			auto npcBaseIsGuard = &TES3::NPCBase::isGuard;
			overrideVirtualTableEnforced(0x749DE8, offsetof(TES3::ActorVirtualTable, isGuard), 0x04DA5E0, *reinterpret_cast<DWORD*>(&npcBaseIsGuard));

			// Allow overriding of sun damage calculation.
			auto weatherControllerCalcSunDamageScalar = &TES3::WeatherController::calcSunDamageScalar;
			genCallEnforced(0x0464C1C, 0x440630, *reinterpret_cast<DWORD*>(&weatherControllerCalcSunDamageScalar));

			// Allow defining certain objects as sourceless.
			auto isSourcelessObject = &TES3::BaseObject::isSourcelessObject;
			genCallEnforced(0x4BC3E0, 0x4C1980, *reinterpret_cast<DWORD*>(&isSourcelessObject));
			genCallEnforced(0x4BC57F, 0x4C1980, *reinterpret_cast<DWORD*>(&isSourcelessObject));
			genCallEnforced(0x4C0C1C, 0x4C1980, *reinterpret_cast<DWORD*>(&isSourcelessObject));
			genCallEnforced(0x4C0DC8, 0x4C1980, *reinterpret_cast<DWORD*>(&isSourcelessObject));
			genCallEnforced(0x4C7715, 0x4C1980, *reinterpret_cast<DWORD*>(&isSourcelessObject));

			// Prevent sourceless objects from being deleted if they are drunk.
			genCallEnforced(0x4551E1, 0x4EEE40, reinterpret_cast<DWORD>(SafeSourcelessDeleteFromRetiredEffect));

			// Allow overriding body part assignment.
			auto bodyPartManagerSetBodyPartForObject = &TES3::BodyPartManager::setBodyPartForObject;
			genCallEnforced(0x4730DD, 0x473CB0, *reinterpret_cast<DWORD*>(&bodyPartManagerSetBodyPartForObject));
			genCallEnforced(0x473CA6, 0x473CB0, *reinterpret_cast<DWORD*>(&bodyPartManagerSetBodyPartForObject));
			genCallEnforced(0x4A12D0, 0x473CB0, *reinterpret_cast<DWORD*>(&bodyPartManagerSetBodyPartForObject));
			genCallEnforced(0x4A3B8E, 0x473CB0, *reinterpret_cast<DWORD*>(&bodyPartManagerSetBodyPartForObject));
			genCallEnforced(0x4D9F7C, 0x473CB0, *reinterpret_cast<DWORD*>(&bodyPartManagerSetBodyPartForObject));
			genCallEnforced(0x4D9FBC, 0x473CB0, *reinterpret_cast<DWORD*>(&bodyPartManagerSetBodyPartForObject));

			// Fix BPM constructor to always have a reference. 
			auto bodyPartManagerConstructor = &TES3::BodyPartManager::ctor;
			genCallEnforced(0x4D8235, 0x472580, *reinterpret_cast<DWORD*>(&bodyPartManagerConstructor));
			genCallEnforced(0x4D8248, 0x472580, *reinterpret_cast<DWORD*>(&bodyPartManagerConstructor));
			genCallEnforced(0x4DA1B0, 0x472580, *reinterpret_cast<DWORD*>(&bodyPartManagerConstructor));
			genCallEnforced(0x4DA1C3, 0x472580, *reinterpret_cast<DWORD*>(&bodyPartManagerConstructor));

			// Event: soundPlay
			auto dataHandlerAddSound = &TES3::DataHandler::addSound;
			genCallEnforced(0x46F8B4, 0x48BD40, *reinterpret_cast<DWORD*>(&dataHandlerAddSound));
			genCallEnforced(0x484D2F, 0x48BD40, *reinterpret_cast<DWORD*>(&dataHandlerAddSound));
			genCallEnforced(0x48ABDD, 0x48BD40, *reinterpret_cast<DWORD*>(&dataHandlerAddSound));
			genCallEnforced(0x48BD0E, 0x48BD40, *reinterpret_cast<DWORD*>(&dataHandlerAddSound));
			genCallEnforced(0x4EA059, 0x48BD40, *reinterpret_cast<DWORD*>(&dataHandlerAddSound));
			genCallEnforced(0x4EA371, 0x48BD40, *reinterpret_cast<DWORD*>(&dataHandlerAddSound));
			genCallEnforced(0x4EA415, 0x48BD40, *reinterpret_cast<DWORD*>(&dataHandlerAddSound));
			genCallEnforced(0x4EB8A9, 0x48BD40, *reinterpret_cast<DWORD*>(&dataHandlerAddSound));
			genCallEnforced(0x4EB9AF, 0x48BD40, *reinterpret_cast<DWORD*>(&dataHandlerAddSound));
			genCallEnforced(0x506052, 0x48BD40, *reinterpret_cast<DWORD*>(&dataHandlerAddSound));
			genCallEnforced(0x506123, 0x48BD40, *reinterpret_cast<DWORD*>(&dataHandlerAddSound));
			genCallEnforced(0x506210, 0x48BD40, *reinterpret_cast<DWORD*>(&dataHandlerAddSound));
			genCallEnforced(0x514A93, 0x48BD40, *reinterpret_cast<DWORD*>(&dataHandlerAddSound));
			genCallEnforced(0x514DE6, 0x48BD40, *reinterpret_cast<DWORD*>(&dataHandlerAddSound));
			genCallEnforced(0x51529A, 0x48BD40, *reinterpret_cast<DWORD*>(&dataHandlerAddSound));
			genCallEnforced(0x515638, 0x48BD40, *reinterpret_cast<DWORD*>(&dataHandlerAddSound));
			genCallEnforced(0x515796, 0x48BD40, *reinterpret_cast<DWORD*>(&dataHandlerAddSound));
			genCallEnforced(0x518C2F, 0x48BD40, *reinterpret_cast<DWORD*>(&dataHandlerAddSound));
			genCallEnforced(0x525DD4, 0x48BD40, *reinterpret_cast<DWORD*>(&dataHandlerAddSound));
			genCallEnforced(0x53E3F6, 0x48BD40, *reinterpret_cast<DWORD*>(&dataHandlerAddSound));
			genCallEnforced(0x5417EE, 0x48BD40, *reinterpret_cast<DWORD*>(&dataHandlerAddSound));
			genCallEnforced(0x54DA8C, 0x48BD40, *reinterpret_cast<DWORD*>(&dataHandlerAddSound));
			genCallEnforced(0x54DAB9, 0x48BD40, *reinterpret_cast<DWORD*>(&dataHandlerAddSound));
			genCallEnforced(0x54DAE7, 0x48BD40, *reinterpret_cast<DWORD*>(&dataHandlerAddSound));
			genCallEnforced(0x5573C5, 0x48BD40, *reinterpret_cast<DWORD*>(&dataHandlerAddSound));
			genCallEnforced(0x5575E3, 0x48BD40, *reinterpret_cast<DWORD*>(&dataHandlerAddSound));
			genCallEnforced(0x557FA7, 0x48BD40, *reinterpret_cast<DWORD*>(&dataHandlerAddSound));
			genCallEnforced(0x558097, 0x48BD40, *reinterpret_cast<DWORD*>(&dataHandlerAddSound));
			genCallEnforced(0x5582CE, 0x48BD40, *reinterpret_cast<DWORD*>(&dataHandlerAddSound));
			genCallEnforced(0x562156, 0x48BD40, *reinterpret_cast<DWORD*>(&dataHandlerAddSound));
			genCallEnforced(0x5621BB, 0x48BD40, *reinterpret_cast<DWORD*>(&dataHandlerAddSound));

			// Event: tempSoundPlay
			auto dataHandlerAddTemporarySound = &TES3::DataHandler::addTemporySound;
			genCallEnforced(0x48B953, 0x48C2B0, *reinterpret_cast<DWORD*>(&dataHandlerAddTemporarySound));
			genCallEnforced(0x48BF1F, 0x48C2B0, *reinterpret_cast<DWORD*>(&dataHandlerAddTemporarySound));
			genCallEnforced(0x4A28B8, 0x48C2B0, *reinterpret_cast<DWORD*>(&dataHandlerAddTemporarySound));
			genCallEnforced(0x50B997, 0x48C2B0, *reinterpret_cast<DWORD*>(&dataHandlerAddTemporarySound));
			genCallEnforced(0x52917B, 0x48C2B0, *reinterpret_cast<DWORD*>(&dataHandlerAddTemporarySound));
			genCallEnforced(0x6051C1, 0x48C2B0, *reinterpret_cast<DWORD*>(&dataHandlerAddTemporarySound));

			// Event: soundObjectPlay
			auto soundPlayRaw = &TES3::Sound::playRaw;
			genCallEnforced(0x440C11, 0x510A40, *reinterpret_cast<DWORD*>(&soundPlayRaw));
			genCallEnforced(0x4470CD, 0x510A40, *reinterpret_cast<DWORD*>(&soundPlayRaw));
			genCallEnforced(0x447EB8, 0x510A40, *reinterpret_cast<DWORD*>(&soundPlayRaw));
			genCallEnforced(0x448CC8, 0x510A40, *reinterpret_cast<DWORD*>(&soundPlayRaw));
			genCallEnforced(0x449B88, 0x510A40, *reinterpret_cast<DWORD*>(&soundPlayRaw));
			genCallEnforced(0x44B1E5, 0x510A40, *reinterpret_cast<DWORD*>(&soundPlayRaw));
			genCallEnforced(0x44B426, 0x510A40, *reinterpret_cast<DWORD*>(&soundPlayRaw));
			genCallEnforced(0x44CCD6, 0x510A40, *reinterpret_cast<DWORD*>(&soundPlayRaw));
			genCallEnforced(0x44D011, 0x510A40, *reinterpret_cast<DWORD*>(&soundPlayRaw));
			genCallEnforced(0x44D1D8, 0x510A40, *reinterpret_cast<DWORD*>(&soundPlayRaw));
			genCallEnforced(0x44E396, 0x510A40, *reinterpret_cast<DWORD*>(&soundPlayRaw));
			genCallEnforced(0x44F653, 0x510A40, *reinterpret_cast<DWORD*>(&soundPlayRaw));
			genCallEnforced(0x45086C, 0x510A40, *reinterpret_cast<DWORD*>(&soundPlayRaw));
			genCallEnforced(0x451885, 0x510A40, *reinterpret_cast<DWORD*>(&soundPlayRaw));
			genCallEnforced(0x48A8D0, 0x510A40, *reinterpret_cast<DWORD*>(&soundPlayRaw));
			genCallEnforced(0x48B9CF, 0x510A40, *reinterpret_cast<DWORD*>(&soundPlayRaw));
			genCallEnforced(0x48C07A, 0x510A40, *reinterpret_cast<DWORD*>(&soundPlayRaw));
			genCallEnforced(0x5060A5, 0x510A40, *reinterpret_cast<DWORD*>(&soundPlayRaw));
			genCallEnforced(0x5984E0, 0x510A40, *reinterpret_cast<DWORD*>(&soundPlayRaw));
			genCallEnforced(0x607711, 0x510A40, *reinterpret_cast<DWORD*>(&soundPlayRaw));
			genCallEnforced(0x63A715, 0x510A40, *reinterpret_cast<DWORD*>(&soundPlayRaw));
			genCallEnforced(0x640F86, 0x510A40, *reinterpret_cast<DWORD*>(&soundPlayRaw));
			genCallEnforced(0x64733D, 0x510A40, *reinterpret_cast<DWORD*>(&soundPlayRaw));
			genCallEnforced(0x6475F7, 0x510A40, *reinterpret_cast<DWORD*>(&soundPlayRaw));
			genCallEnforced(0x647962, 0x510A40, *reinterpret_cast<DWORD*>(&soundPlayRaw));
			genCallEnforced(0x64D3DA, 0x510A40, *reinterpret_cast<DWORD*>(&soundPlayRaw));

			// Create lua tracebacks when a warning is created.
			genCallUnprotected(0x4774D1, reinterpret_cast<DWORD>(WriteToWarningsFile), 0x6);
			genCallUnprotected(0x476EA7, reinterpret_cast<DWORD>(WriteToWarningsFile), 0x6);

			// Expand CombatSession struct.
			genPushEnforced(0x4E60C5, (BYTE)sizeof(TES3::CombatSession)) && genCallEnforced(0x4E60DE, 0x537120, reinterpret_cast<DWORD>(&TES3::CombatSession::ctor));
			genPushEnforced(0x530624, (BYTE)sizeof(TES3::CombatSession)) && genCallEnforced(0x530641, 0x537120, reinterpret_cast<DWORD>(&TES3::CombatSession::ctor));
			genCallEnforced(0x51FAFF, 0x5372F0, reinterpret_cast<DWORD>(&TES3::CombatSession::dtor));
			genCallEnforced(0x55886B, 0x5372F0, reinterpret_cast<DWORD>(&TES3::CombatSession::dtor));

			// Event: Prevent Rest
			genCallEnforced(0x564FF6, 0x530A20, reinterpret_cast<DWORD>(OnCheckActionWeightFightForRest));

			// Event: Play Item Up/Down Sound Event
			auto WorldController_playItemUpDownSound = &TES3::WorldController::playItemUpDownSound;
			genCallEnforced(0x49B2C3, 0x411050, *reinterpret_cast<DWORD*>(&WorldController_playItemUpDownSound));
			genCallEnforced(0x4EA632, 0x411050, *reinterpret_cast<DWORD*>(&WorldController_playItemUpDownSound));
			genCallEnforced(0x527F4F, 0x411050, *reinterpret_cast<DWORD*>(&WorldController_playItemUpDownSound));
			genCallEnforced(0x569591, 0x411050, *reinterpret_cast<DWORD*>(&WorldController_playItemUpDownSound));
			genCallEnforced(0x5A4450, 0x411050, *reinterpret_cast<DWORD*>(&WorldController_playItemUpDownSound));
			genCallEnforced(0x5A7315, 0x411050, *reinterpret_cast<DWORD*>(&WorldController_playItemUpDownSound));
			genCallEnforced(0x5B4EF4, 0x411050, *reinterpret_cast<DWORD*>(&WorldController_playItemUpDownSound));
			genCallEnforced(0x5B5F48, 0x411050, *reinterpret_cast<DWORD*>(&WorldController_playItemUpDownSound));
			genCallEnforced(0x5B7131, 0x411050, *reinterpret_cast<DWORD*>(&WorldController_playItemUpDownSound));
			genCallEnforced(0x5CB61D, 0x411050, *reinterpret_cast<DWORD*>(&WorldController_playItemUpDownSound));
			genCallEnforced(0x5CB703, 0x411050, *reinterpret_cast<DWORD*>(&WorldController_playItemUpDownSound));
			genCallEnforced(0x5CB94A, 0x411050, *reinterpret_cast<DWORD*>(&WorldController_playItemUpDownSound));
			genCallEnforced(0x5CE498, 0x411050, *reinterpret_cast<DWORD*>(&WorldController_playItemUpDownSound));
			genCallEnforced(0x5CE66F, 0x411050, *reinterpret_cast<DWORD*>(&WorldController_playItemUpDownSound));
			genCallEnforced(0x5CED42, 0x411050, *reinterpret_cast<DWORD*>(&WorldController_playItemUpDownSound));
			genCallEnforced(0x5CF24A, 0x411050, *reinterpret_cast<DWORD*>(&WorldController_playItemUpDownSound));
			genCallEnforced(0x5CF456, 0x411050, *reinterpret_cast<DWORD*>(&WorldController_playItemUpDownSound));
			genCallEnforced(0x5CF5DA, 0x411050, *reinterpret_cast<DWORD*>(&WorldController_playItemUpDownSound));
			genCallEnforced(0x5CF8B5, 0x411050, *reinterpret_cast<DWORD*>(&WorldController_playItemUpDownSound));
			genCallEnforced(0x5D01A5, 0x411050, *reinterpret_cast<DWORD*>(&WorldController_playItemUpDownSound));
			genCallEnforced(0x5D0282, 0x411050, *reinterpret_cast<DWORD*>(&WorldController_playItemUpDownSound));
			genCallEnforced(0x5D3A03, 0x411050, *reinterpret_cast<DWORD*>(&WorldController_playItemUpDownSound));
			genCallEnforced(0x5D3B43, 0x411050, *reinterpret_cast<DWORD*>(&WorldController_playItemUpDownSound));
			genCallEnforced(0x5D3BC8, 0x411050, *reinterpret_cast<DWORD*>(&WorldController_playItemUpDownSound));
			genCallEnforced(0x61678A, 0x411050, *reinterpret_cast<DWORD*>(&WorldController_playItemUpDownSound));

			// Event: Reference Activated/Deactivated.
			genCallEnforced(0x4849E8, 0x484E50, reinterpret_cast<DWORD>(AddMobilesToCell));
			genCallEnforced(0x484E9B, 0x484E50, reinterpret_cast<DWORD>(AddMobilesToCell));
			genCallEnforced(0x485346, 0x484E50, reinterpret_cast<DWORD>(AddMobilesToCell));
			genCallEnforced(0x4855E1, 0x484E50, reinterpret_cast<DWORD>(AddMobilesToCell));
			genCallEnforced(0x486B88, 0x484E50, reinterpret_cast<DWORD>(AddMobilesToCell));
			genCallEnforced(0x486C97, 0x484E50, reinterpret_cast<DWORD>(AddMobilesToCell));
			genCallEnforced(0x486D9A, 0x484E50, reinterpret_cast<DWORD>(AddMobilesToCell));
			genCallEnforced(0x486E6F, 0x484E50, reinterpret_cast<DWORD>(AddMobilesToCell));
			genCallEnforced(0x486FB7, 0x484E50, reinterpret_cast<DWORD>(AddMobilesToCell));
			genCallEnforced(0x4895F1, 0x484E50, reinterpret_cast<DWORD>(AddMobilesToCell));
			genCallEnforced(0x4834A8, 0x484D80, reinterpret_cast<DWORD>(RemoveMobilesFromCell));
			genCallEnforced(0x4848CE, 0x484D80, reinterpret_cast<DWORD>(RemoveMobilesFromCell));
			genCallEnforced(0x4891F3, 0x484D80, reinterpret_cast<DWORD>(RemoveMobilesFromCell));
			genCallEnforced(0x4898EB, 0x484D80, reinterpret_cast<DWORD>(RemoveMobilesFromCell));
			auto Cell_insertReference = &TES3::Cell::insertReference;
			genCallEnforced(0x4665AF, 0x4DC030, *reinterpret_cast<DWORD*>(&Cell_insertReference));
			genCallEnforced(0x4B8E5F, 0x4DC030, *reinterpret_cast<DWORD*>(&Cell_insertReference));
			genCallEnforced(0x4B8F27, 0x4DC030, *reinterpret_cast<DWORD*>(&Cell_insertReference));
			genCallEnforced(0x4C1171, 0x4DC030, *reinterpret_cast<DWORD*>(&Cell_insertReference));
			genCallEnforced(0x4C16EA, 0x4DC030, *reinterpret_cast<DWORD*>(&Cell_insertReference));
			genCallEnforced(0x4CFBAD, 0x4DC030, *reinterpret_cast<DWORD*>(&Cell_insertReference));
			genCallEnforced(0x4E7DE1, 0x4DC030, *reinterpret_cast<DWORD*>(&Cell_insertReference));
			genCallEnforced(0x5098DE, 0x4DC030, *reinterpret_cast<DWORD*>(&Cell_insertReference));
			genCallEnforced(0x509A41, 0x4DC030, *reinterpret_cast<DWORD*>(&Cell_insertReference));
			genCallEnforced(0x50A22F, 0x4DC030, *reinterpret_cast<DWORD*>(&Cell_insertReference));
			genCallEnforced(0x635368, 0x4DC030, *reinterpret_cast<DWORD*>(&Cell_insertReference));
			auto Reference_setDeleted = &TES3::Reference::setDeleted;
			genCallEnforced(0x4668FB, 0x4EEC70, *reinterpret_cast<DWORD*>(&Reference_setDeleted));
			//genCallEnforced(0x4C0D42, 0x4EEC70, *reinterpret_cast<DWORD*>(&Reference_setDeleted)); // Maybe
			//genCallEnforced(0x4C0D8F, 0x4EEC70, *reinterpret_cast<DWORD*>(&Reference_setDeleted)); // Maybe
			genCallEnforced(0x4DD3AB, 0x4EEC70, *reinterpret_cast<DWORD*>(&Reference_setDeleted));
			genCallEnforced(0x4DDB5F, 0x4EEC70, *reinterpret_cast<DWORD*>(&Reference_setDeleted));
			genCallEnforced(0x4DDBFA, 0x4EEC70, *reinterpret_cast<DWORD*>(&Reference_setDeleted));
			genCallEnforced(0x4DE4B7, 0x4EEC70, *reinterpret_cast<DWORD*>(&Reference_setDeleted));
			genCallEnforced(0x4E0753, 0x4EEC70, *reinterpret_cast<DWORD*>(&Reference_setDeleted));
			genCallEnforced(0x4E086B, 0x4EEC70, *reinterpret_cast<DWORD*>(&Reference_setDeleted));
			genCallEnforced(0x4E0CDA, 0x4EEC70, *reinterpret_cast<DWORD*>(&Reference_setDeleted));
			genCallEnforced(0x4E2A51, 0x4EEC70, *reinterpret_cast<DWORD*>(&Reference_setDeleted));
			genCallEnforced(0x4E2B53, 0x4EEC70, *reinterpret_cast<DWORD*>(&Reference_setDeleted));
			genCallEnforced(0x4E45F0, 0x4EEC70, *reinterpret_cast<DWORD*>(&Reference_setDeleted));
			genCallEnforced(0x4E5AEF, 0x4EEC70, *reinterpret_cast<DWORD*>(&Reference_setDeleted));
			genCallEnforced(0x4E8103, 0x4EEC70, *reinterpret_cast<DWORD*>(&Reference_setDeleted));
			genCallEnforced(0x4E95AE, 0x4EEC70, *reinterpret_cast<DWORD*>(&Reference_setDeleted));
			genCallEnforced(0x4EA98A, 0x4EEC70, *reinterpret_cast<DWORD*>(&Reference_setDeleted));
			genCallEnforced(0x4EA9C1, 0x4EEC70, *reinterpret_cast<DWORD*>(&Reference_setDeleted));
			genCallEnforced(0x4EBE19, 0x4EEC70, *reinterpret_cast<DWORD*>(&Reference_setDeleted));
			genCallEnforced(0x4EBF58, 0x4EEC70, *reinterpret_cast<DWORD*>(&Reference_setDeleted));
			genCallEnforced(0x4EBF8C, 0x4EEC70, *reinterpret_cast<DWORD*>(&Reference_setDeleted));
			genCallEnforced(0x4EC00F, 0x4EEC70, *reinterpret_cast<DWORD*>(&Reference_setDeleted));
			genCallEnforced(0x50C538, 0x4EEC70, *reinterpret_cast<DWORD*>(&Reference_setDeleted));
			genCallEnforced(0x529B86, 0x4EEC70, *reinterpret_cast<DWORD*>(&Reference_setDeleted));

			// Patch other functions to use referenceActivated/Deactivated functions.
			genCallEnforced(0x5063A6, 0x50EDD0, reinterpret_cast<DWORD>(ScriptRelocateReference));
			genCallEnforced(0x5064D8, 0x50EDD0, reinterpret_cast<DWORD>(ScriptRelocateReference));
			genCallEnforced(0x509D85, 0x50EDD0, reinterpret_cast<DWORD>(ScriptRelocateReference));
			// Disable destruction on process exit of a global temp reference. Avoids deactivation event triggering on this reference.
			genNOPUnprotected(0x49A4E9, 0xD);

			// Event: playGroup
			auto AnimationData_playAnimationGroupForIndex = &TES3::AnimationData::playAnimationGroupForIndex;
			genCallEnforced(0x4C699D, 0x470AE0, *reinterpret_cast<DWORD*>(&AnimationData_playAnimationGroupForIndex));
			genCallEnforced(0x4C69AB, 0x470AE0, *reinterpret_cast<DWORD*>(&AnimationData_playAnimationGroupForIndex));
			genCallEnforced(0x4C69B9, 0x470AE0, *reinterpret_cast<DWORD*>(&AnimationData_playAnimationGroupForIndex));
			genCallEnforced(0x4DC9AF, 0x470AE0, *reinterpret_cast<DWORD*>(&AnimationData_playAnimationGroupForIndex));
			genCallEnforced(0x4DC9BD, 0x470AE0, *reinterpret_cast<DWORD*>(&AnimationData_playAnimationGroupForIndex));
			genCallEnforced(0x4DC9CB, 0x470AE0, *reinterpret_cast<DWORD*>(&AnimationData_playAnimationGroupForIndex));
			genCallEnforced(0x524BC8, 0x470AE0, *reinterpret_cast<DWORD*>(&AnimationData_playAnimationGroupForIndex));
			genCallEnforced(0x524BD6, 0x470AE0, *reinterpret_cast<DWORD*>(&AnimationData_playAnimationGroupForIndex));
			genCallEnforced(0x524BE4, 0x470AE0, *reinterpret_cast<DWORD*>(&AnimationData_playAnimationGroupForIndex));
			genCallEnforced(0x540AF3, 0x470AE0, *reinterpret_cast<DWORD*>(&AnimationData_playAnimationGroupForIndex));
			genCallEnforced(0x540B09, 0x470AE0, *reinterpret_cast<DWORD*>(&AnimationData_playAnimationGroupForIndex));
			genCallEnforced(0x540B1E, 0x470AE0, *reinterpret_cast<DWORD*>(&AnimationData_playAnimationGroupForIndex));
			genCallEnforced(0x540EF9, 0x470AE0, *reinterpret_cast<DWORD*>(&AnimationData_playAnimationGroupForIndex));
			genCallEnforced(0x745B89, 0x470AE0, *reinterpret_cast<DWORD*>(&AnimationData_playAnimationGroupForIndex));
			genCallEnforced(0x745B9E, 0x470AE0, *reinterpret_cast<DWORD*>(&AnimationData_playAnimationGroupForIndex));
			genCallEnforced(0x745BB3, 0x470AE0, *reinterpret_cast<DWORD*>(&AnimationData_playAnimationGroupForIndex));

			// Modify actor animation controller to allow blocking animation changes.
			writePatchCodeUnprotected(0x53DAB5, (BYTE*)&patchActorAnimInit, patchActorAnimInit_size);
			genCallUnprotected(0x5428A3, reinterpret_cast<DWORD>(&CheckTogglePOV), 6);
			genCallEnforced(0x53E135, 0x540A90, reinterpret_cast<DWORD>(&ActorAnimController_selectMovementAnimAndUpdate));

			// Make mobile IdleAnim flag reset on Stop key, instead of when loopCount reaches zero.
			genJumpEnforced(0x46DA0D, 0x46E64E, 0x46E49E);
			genJumpUnprotected(0x46E498, 0x46E64E);

			// Event: Power Recharged
			overrideVirtualTableEnforced(0x74AC54, offsetof(PowersHashMap::VirtualTable, deleteKeyValuePair), 0x4F1C50, reinterpret_cast<DWORD>(OnDeletePowerHashMapKVP));

			// UI framework hooks
			TES3::UI::hook();

			// Install custom magic effect hooks.
			TES3::MagicEffectController::InstallCustomMagicEffectController();

			// Make magic effects writable.
			VirtualProtect((DWORD*)TES3_DATA_EFFECT_FLAGS, 4 * 143, PAGE_READWRITE, &OldProtect);

			// Hook generic entity deletion so that we can do any necessary cleanup.
			auto baseObjectDestructor = &TES3::BaseObject::dtor;
			genCallEnforced(0x4AA15B, 0x4F0CA0, *reinterpret_cast<DWORD*>(&baseObjectDestructor));
			genCallEnforced(0x4AAF10, 0x4F0CA0, *reinterpret_cast<DWORD*>(&baseObjectDestructor));
			genCallEnforced(0x4E49EE, 0x4F0CA0, *reinterpret_cast<DWORD*>(&baseObjectDestructor));
			genCallEnforced(0x4EEFAA, 0x4F0CA0, *reinterpret_cast<DWORD*>(&baseObjectDestructor));
			genCallEnforced(0x4F026F, 0x4F0CA0, *reinterpret_cast<DWORD*>(&baseObjectDestructor));
			genCallEnforced(0x4F0C83, 0x4F0CA0, *reinterpret_cast<DWORD*>(&baseObjectDestructor));

			// Raise event for barter attempts.
			genCallEnforced(0x5A70CF, 0x5A66C0, reinterpret_cast<DWORD>(GameBarterOffer));
			genCallEnforced(0x5A6777, reinterpret_cast<DWORD>(OnExerciseSkill), reinterpret_cast<DWORD>(GameBarterOffer_BufferSkillGain));

			// Allow area-of-effect magic effects and tes3.findActorsInProximity to hit/find incapacitated actors.
			genJumpUnprotected(0x570341, 0x57034C);

			// Patch custom magic effect saving and loading.
			genJumpUnprotected(TES3_PATCH_MAGIC_SAVE_LOAD, reinterpret_cast<DWORD>(PatchMagicSaveLoad), TES3_PATCH_MAGIC_SAVE_LOAD_SIZE);

			// Allow setting names of apparatus, lockpicks, probes, and repair tools.
			auto TES3_Apparatus_setName = &TES3::Apparatus::setName;
			auto TES3_Lockpick_setName = &TES3::Lockpick::setName;
			auto TES3_Probe_setName = &TES3::Probe::setName;
			auto TES3_RepairTool_setName = &TES3::RepairTool::setName;
			overrideVirtualTableEnforced(0x748110, offsetof(TES3::ObjectVirtualTable, setName), 0x4F1C50, *reinterpret_cast<DWORD*>(&TES3_Apparatus_setName));
			overrideVirtualTableEnforced(0x748D10, offsetof(TES3::ObjectVirtualTable, setName), 0x4F1C50, *reinterpret_cast<DWORD*>(&TES3_Lockpick_setName));
			overrideVirtualTableEnforced(0x748FA0, offsetof(TES3::ObjectVirtualTable, setName), 0x4F1C50, *reinterpret_cast<DWORD*>(&TES3_Probe_setName));
			overrideVirtualTableEnforced(0x7490E8, offsetof(TES3::ObjectVirtualTable, setName), 0x4F1C50, *reinterpret_cast<DWORD*>(&TES3_RepairTool_setName));

			// Support for MobileSpellProjectile::explode.
			// Initialize extra flag members in MobileProjectile::ctor.
			writeByteUnprotected(0x5723BC, 0x89);
			// The explode logic is deferred to execute at the same point as projectile simulation to preserve consistency.
			auto projectileControllerResolveCollisions = &TES3::ProjectileController::resolveCollisions;
			genCallEnforced(0x5638F8, 0x5753A0, *reinterpret_cast<DWORD*>(&projectileControllerResolveCollisions));

			// Look for main.lua scripts in the usual directories.
			executeMainModScripts("Data Files\\MWSE\\core");
			executeMainModScripts("Data Files\\MWSE\\mods");

			// Temporary backwards compatibility for old-style MWSE mods.
			if (Configuration::EnableLegacyLuaMods) {
				executeMainModScripts("Data Files\\MWSE\\lua", "mod_init.lua");
			}
		}

		void LuaManager::cleanup() {
			// Clean up our handles to our override tables. Helps to prevent a crash when
			// closing mid-execution.
			scriptOverrides.clear();

			// Clear cached userdata.
			TES3::BaseObject::clearCachedLuaObjects();
			TES3::MobileObject::clearCachedLuaObjects();
			TES3::Weather::clearCachedLuaObjects();
			NI::Object::clearCachedLuaObjects();
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
			auto stateHandle = getThreadSafeStateHandle();

			sol::object response = event::trigger(baseEvent->getEventName(), baseEvent->createEventTable(), baseEvent->getEventOptions());
			delete baseEvent;
			return response;
		}

		void LuaManager::setButtonPressedCallback(sol::optional<sol::protected_function> callback) {
			buttonPressedCallback = callback.value_or(sol::nil);
		}

		void LuaManager::triggerButtonPressed() {
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			if (buttonPressedCallback != sol::nil) {
				sol::protected_function callback = buttonPressedCallback;
				buttonPressedCallback = sol::nil;
				sol::state &state = stateHandle.state;
				sol::table eventData = state.create_table();
				eventData["button"] = tes3::ui::getButtonPressedIndex();
				tes3::ui::resetButtonPressedIndex();

				sol::protected_function_result result = callback(eventData);
				if (!result.valid()) {
					sol::error error = result;
					log::getLog() << "Runtime error when running tes3.messageBox button callback:\n" << error.what() << std::endl;
				}
			}
		}

		void LuaManager::updateTimers(float deltaTime, double simulationTimestamp, bool simulating) {
			realTimers->incrementClock(deltaTime);
			gameTimers->setClock(simulationTimestamp);

			if (simulating) {
				simulateTimers->incrementClock(deltaTime);
			}
		}

		void LuaManager::savePersistentTimers() {
			auto macp = TES3::WorldController::get()->getMobilePlayer();
			if (macp == nullptr) {
				return;
			}

			// Store the real time clocks.
			sol::table persists = luaState.create_table();
			persists["clockReal"] = realTimers->getClock();
			persists["clockSimulation"] = realTimers->getClock();

			// Serialize timers into a list.
			sol::table list = persists.create_named("list");
			std::vector<std::shared_ptr<TimerController>> timers = { gameTimers, simulateTimers, realTimers };
			for (auto controller : timers) {
				for (auto timer : controller->m_ActiveTimers) {
					if (timer->isPersistent) {
						auto t = timer->toTable(luaState.lua_state());
						t["p"] = sol::nil;
						list.add(t);
					}
				}
				for (auto timer : controller->m_PausedTimers) {
					if (timer->isPersistent) {
						auto t = timer->toTable(luaState.lua_state());
						t["p"] = sol::nil;
						list.add(t);
					}
				}
			}

			// No timers? Bail.
			if (list.size() == 0) {
				return;
			}

			auto playerData = macp->reference->getLuaTable();
			playerData["mwse:timers"] = persists;
		}

		void LuaManager::restorePersistentTimers() {
			auto macp = TES3::WorldController::get()->getMobilePlayer();
			if (macp == nullptr) {
				return;
			}

			auto timerData = macp->reference->getLuaTable().get_or<sol::table>("mwse:timers", sol::nil);
			if (timerData == sol::nil) {
				return;
			}

			// Restore controller values.
			realTimers->setClock(timerData.get_or("clockReal", 0.0));
			simulateTimers->setClock(timerData.get_or("clockSimulation", 0.0));

			// Restore timer values.
			auto list = timerData.get<sol::table>("list");
			for (const auto& kvp : list) {
				auto timer = Timer::createFromTable(kvp.second);
			}
		}

		void LuaManager::clearPersistentTimers() {
			auto macp = TES3::WorldController::get()->getMobilePlayer();
			if (macp == nullptr) {
				return;
			}

			auto playerData = macp->reference->getLuaTable();
			playerData["mwse:timers"] = sol::nil;
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

		TimerType LuaManager::getTimerControllerType(std::shared_ptr<TimerController> controller) {
			if (controller == realTimers) {
				return TimerType::RealTime;
			}
			else if (controller == simulateTimers) {
				return TimerType::SimulationTime;
			}
			else if (controller == gameTimers) {
				return TimerType::GameTime;
			}

			throw std::invalid_argument("Controller cannot be mapped to a type.");
		}

		void LuaManager::claimLuaThread() {
			stateThreadMutex.lock();
		}

		void LuaManager::releaseLuaThread() {
			stateThreadMutex.unlock();
		}
	}
}