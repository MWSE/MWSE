#include "LuaManager.h"

#include "BitUtil.h"
#include "BuildDate.h"
#include "Log.h"
#include "MemoryUtil.h"
#include "mwOffsets.h"
#include "MWSEConfig.h"
#include "MWSEDefs.h"
#include "MWSEUtilLua.h"
#include "ScriptUtil.h"
#include "StringUtil.h"
#include "TES3Util.h"
#include "UIUtil.h"
#include "WindowsUtil.h"

#include "LuaTimer.h"

#include "LuaScript.h"

#include "TES3Activator.h"
#include "TES3Actor.h"
#include "TES3ActorAnimationController.h"
#include "TES3Alchemy.h"
#include "TES3Apparatus.h"
#include "TES3Archive.h"
#include "TES3AudioController.h"
#include "TES3BodyPart.h"
#include "TES3BodyPartManager.h"
#include "TES3Book.h"
#include "TES3CombatSession.h"
#include "TES3Container.h"
#include "TES3Creature.h"
#include "TES3CrimeEvent.h"
#include "TES3DataHandler.h"
#include "TES3Defines.h"
#include "TES3Dialogue.h"
#include "TES3DialogueInfo.h"
#include "TES3Door.h"
#include "TES3Enchantment.h"
#include "TES3Fader.h"
#include "TES3Game.h"
#include "TES3GameFile.h"
#include "TES3GameSetting.h"
#include "TES3GlobalVariable.h"
#include "TES3Ingredient.h"
#include "TES3InputController.h"
#include "TES3ItemData.h"
#include "TES3LeveledList.h"
#include "TES3Light.h"
#include "TES3Lockpick.h"
#include "TES3MagicEffect.h"
#include "TES3MagicEffectController.h"
#include "TES3MagicEffectInstance.h"
#include "TES3MagicInstanceController.h"
#include "TES3Misc.h"
#include "TES3MobileActor.h"
#include "TES3MobileCreature.h"
#include "TES3MobilePlayer.h"
#include "TES3MobileProjectile.h"
#include "TES3MobManager.h"
#include "TES3NPC.h"
#include "TES3PlayerAnimationController.h"
#include "TES3Probe.h"
#include "TES3Reference.h"
#include "TES3RepairTool.h"
#include "TES3Script.h"
#include "TES3SoulGemData.h"
#include "TES3Sound.h"
#include "TES3Spell.h"
#include "TES3Static.h"
#include "TES3UIElement.h"
#include "TES3UIInventoryTile.h"
#include "TES3UIManager.h"
#include "TES3UIMenuController.h"
#include "TES3VFXManager.h"
#include "TES3Weather.h"
#include "TES3WeatherController.h"
#include "TES3WorldController.h"

// Lua binding files. These are split out rather than kept here to help with compile times.
#include "MemoryUtilLua.h"
#include "MGEPostShadersLua.h"
#include "MGEUtilLua.h"
#include "ScriptUtilLua.h"
#include "StackLua.h"
#include "StringUtilLua.h"
#include "TES3UtilLua.h"

#include "yamlloader.hpp"

#include "TES3ActionDataLua.h"
#include "TES3ActivatorLua.h"
#include "TES3ActorAnimationControllerLua.h"
#include "TES3AILua.h"
#include "TES3AlchemyLua.h"
#include "TES3AnimationDataLua.h"
#include "TES3AnimationGroupLua.h"
#include "TES3ApparatusLua.h"
#include "TES3ArchiveLua.h"
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
#include "TES3CrimeLua.h"
#include "TES3DataHandlerLua.h"
#include "TES3DialogueLua.h"
#include "TES3DoorLua.h"
#include "TES3EnchantmentLua.h"
#include "TES3FactionLua.h"
#include "TES3FaderLua.h"
#include "TES3GameFileLua.h"
#include "TES3GameLua.h"
#include "TES3GameSettingLua.h"
#include "TES3GlobalVariableLua.h"
#include "TES3IngredientLua.h"
#include "TES3InputControllerLua.h"
#include "TES3InventoryLua.h"
#include "TES3LandLua.h"
#include "TES3LandTextureLua.h"
#include "TES3LeveledListLua.h"
#include "TES3LightLua.h"
#include "TES3LockpickLua.h"
#include "TES3MagicEffectInstanceLua.h"
#include "TES3MagicEffectLua.h"
#include "TES3MagicSourceInstanceLua.h"
#include "TES3MiscLua.h"
#include "TES3MobileActorLua.h"
#include "TES3MobileCreatureLua.h"
#include "TES3MobileNPCLua.h"
#include "TES3MobilePlayerLua.h"
#include "TES3MobileProjectileLua.h"
#include "TES3MobManagerLua.h"
#include "TES3MoonLua.h"
#include "TES3NPCLua.h"
#include "TES3PlayerAnimationControllerLua.h"
#include "TES3ProbeLua.h"
#include "TES3RaceLua.h"
#include "TES3ReferenceListLua.h"
#include "TES3ReferenceLua.h"
#include "TES3RegionLua.h"
#include "TES3RepairToolLua.h"
#include "TES3ScriptLua.h"
#include "TES3SkillLua.h"
#include "TES3SoundLua.h"
#include "TES3SpellListLua.h"
#include "TES3SpellLua.h"
#include "TES3StaticLua.h"
#include "TES3StatisticLua.h"
#include "TES3UIElementLua.h"
#include "TES3UIInventoryTileLua.h"
#include "TES3UIManagerLua.h"
#include "TES3UIMenuControllerLua.h"
#include "TES3UIWidgetsLua.h"
#include "TES3VectorsLua.h"
#include "TES3VFXManagerLua.h"
#include "TES3WaterControllerLua.h"
#include "TES3WeaponLua.h"
#include "TES3WeatherControllerLua.h"
#include "TES3WeatherLua.h"
#include "TES3WorldControllerLua.h"

#include "NIBoundingVolumeLua.h"
#include "NIBSAnimationNodeLua.h"
#include "NICameraLua.h"
#include "NICollisionGroupLua.h"
#include "NICollisionSwitchLua.h"
#include "NIColorLua.h"
#include "NIExtraDataLua.h"
#include "NIGeometryDataLua.h"
#include "NILightLua.h"
#include "NILinesLua.h"
#include "NINodeLua.h"
#include "NIObjectLua.h"
#include "NIParticlesLua.h"
#include "NIPickLua.h"
#include "NIPixelDataLua.h"
#include "NIPropertyLua.h"
#include "NIQuaternionLua.h"
#include "NIRenderedTextureLua.h"
#include "NIRendererLua.h"
#include "NISkinInstanceLua.h"
#include "NISourceTextureLua.h"
#include "NISwitchNodeLua.h"
#include "NITextureLua.h"
#include "NITimeControllerLua.h"
#include "NITriShapeLua.h"

#include "LuaDisableableEventManager.h"

#include "LuaAbsorbedMagicEvent.h"
#include "LuaActivationTargetChangedEvent.h"
#include "LuaActiveMagicEffectIconsUpdatedEvent.h"
#include "LuaAddTopicEvent.h"
#include "LuaAttackEvent.h"
#include "LuaAttackHitEvent.h"
#include "LuaBarterOfferEvent.h"
#include "LuaCalcBarterPriceEvent.h"
#include "LuaCalcBlockChanceEvent.h"
#include "LuaCalcChargenStatsEvent.h"
#include "LuaCalcEnchantmentPriceEvent.h"
#include "LuaCalcHitArmorPieceEvent.h"
#include "LuaCalcHitChanceEvent.h"
#include "LuaCalcHitDetectionConeEvent.h"
#include "LuaCalcRepairPriceEvent.h"
#include "LuaCalcRestInterruptEvent.h"
#include "LuaCalcSoulValueEvent.h"
#include "LuaCalcSpellmakingPriceEvent.h"
#include "LuaCalcSpellmakingSpellPointCostEvent.h"
#include "LuaCalcSpellPriceEvent.h"
#include "LuaCalcTouchSpellConeEvent.h"
#include "LuaCalcTrainingPriceEvent.h"
#include "LuaCalcTravelPriceEvent.h"
#include "LuaCellChangedEvent.h"
#include "LuaCrimeWitnessedEvent.h"
#include "LuaDamageEvent.h"
#include "LuaDamageHandToHandEvent.h"
#include "LuaDialogueEnvironmentCreatedEvent.h"
#include "LuaEnchantChargeUseEvent.h"
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
#include "LuaLeveledItemPickedEvent.h"
#include "LuaLevelUpEvent.h"
#include "LuaLoadGameEvent.h"
#include "LuaLoadedGameEvent.h"
#include "LuaMagicCastedEvent.h"
#include "LuaMagicEffectRemovedEvent.h"
#include "LuaMagicReflectEvent.h"
#include "LuaMagicReflectedEvent.h"
#include "LuaMenuStateEvent.h"
#include "LuaMobileObjectActivatedEvent.h"
#include "LuaMobileObjectDeactivatedEvent.h"
#include "LuaMouseAxisEvent.h"
#include "LuaMouseButtonDownEvent.h"
#include "LuaMouseButtonUpEvent.h"
#include "LuaMouseWheelEvent.h"
#include "LuaMusicChangeTrackEvent.h"
#include "LuaMusicSelectTrackEvent.h"
#include "LuaObjectCopiedEvent.h"
#include "LuaObjectCreatedEvent.h"
#include "LuaObjectInvalidatedEvent.h"
#include "LuaPickpocketEvent.h"
#include "LuaPlayItemSoundEvent.h"
#include "LuaPostInfoResponseEvent.h"
#include "LuaPotionBrewedEvent.h"
#include "LuaPotionBrewFailedEvent.h"
#include "LuaPotionBrewSkillCheckEvent.h"
#include "LuaPowerRechargedEvent.h"
#include "LuaPreLevelUpEvent.h"
#include "LuaPreventRestEvent.h"
#include "LuaProjectileExpireEvent.h"
#include "LuaReferenceActivatedEvent.h"
#include "LuaRepairEvent.h"
#include "LuaRestInterruptEvent.h"
#include "LuaShieldBlockedEvent.h"
#include "LuaSimulateEvent.h"
#include "LuaSkillRaisedEvent.h"
#include "LuaSpellCastedEvent.h"
#include "LuaSpellCreatedEvent.h"
#include "LuaSpellMagickaUseEvent.h"
#include "LuaSpellResistedEvent.h"
#include "LuaSpellResistEvent.h"
#include "LuaSpellTickEvent.h"
#include "LuaUiRefreshedEvent.h"
#include "LuaUiSkillTooltipEvent.h"
#include "LuaUiSpellTooltipEvent.h"
#include "LuaWeaponReadiedEvent.h"
#include "LuaWeaponUnreadiedEvent.h"
#include "LuaWeatherChangedImmediateEvent.h"
#include "LuaWeatherCycledEvent.h"
#include "LuaWeatherTransitionFinishedEvent.h"
#include "LuaWeatherTransitionStartedEvent.h"

#include "NITextureEffectLua.h"

#include "InstructionStore.h"

namespace mwse::lua {
	constexpr auto TES3_HOOK_RUNSCRIPT_LUACHECK = 0x5029A4;
	constexpr auto TES3_HOOK_RUNSCRIPT_LUACHECK_SIZE = 0x6;
	constexpr auto TES3_HOOK_RUNSCRIPT_LUACHECK_RETURN = (TES3_HOOK_RUNSCRIPT_LUACHECK + TES3_HOOK_RUNSCRIPT_LUACHECK_SIZE);

	constexpr auto TES3_HOOK_SAVE_REFERENCE = 0x4E1C95;
	constexpr auto TES3_HOOK_SAVE_REFERENCE_SIZE = 0x6;
	constexpr auto TES3_HOOK_SAVE_REFERENCE_RETURN = (TES3_HOOK_SAVE_REFERENCE + TES3_HOOK_SAVE_REFERENCE_SIZE);

	constexpr auto TES3_HOOK_LOAD_REFERENCE = 0x4DE532;
	constexpr auto TES3_HOOK_LOAD_REFERENCE_SIZE = 0x5;
	constexpr auto TES3_HOOK_LOAD_REFERENCE_RETURN = (TES3_HOOK_LOAD_REFERENCE + TES3_HOOK_LOAD_REFERENCE_SIZE);
	constexpr auto TES3_HOOK_LOAD_REFERENCE_RETURN_SUCCESS = 0x4DE406;

	constexpr auto TES3_HOOK_UI_EVENT = 0x58371A;
	constexpr auto TES3_HOOK_UI_EVENT_SIZE = 0x5;
	constexpr auto TES3_HOOK_UI_EVENT_RETURN = (TES3_HOOK_UI_EVENT + TES3_HOOK_UI_EVENT_SIZE);

	constexpr auto TES3_HOOK_MAGIC_CAST_SUCCESS = 0x5157E6;
	constexpr auto TES3_HOOK_MAGIC_CAST_SUCCESS_SIZE = 0xA;
	constexpr auto TES3_HOOK_MAGIC_CAST_SUCCESS_RETURN = (TES3_HOOK_MAGIC_CAST_SUCCESS + TES3_HOOK_MAGIC_CAST_SUCCESS_SIZE);

	constexpr auto TES3_HOOK_SPELL_CAST_FAILURE = 0x5157D5;
	constexpr auto TES3_HOOK_SPELL_CAST_FAILURE_SIZE = 0xA;
	constexpr auto TES3_HOOK_SPELL_CAST_FAILURE_RETURN = (TES3_HOOK_SPELL_CAST_FAILURE + TES3_HOOK_SPELL_CAST_FAILURE_SIZE);

	constexpr auto TES3_PATCH_MAGIC_SAVE_LOAD = 0x51391F;
	constexpr auto TES3_PATCH_MAGIC_SAVE_LOAD_SIZE = 0x8;
	constexpr auto TES3_PATCH_MAGIC_SAVE_LOAD_RETURN = (TES3_PATCH_MAGIC_SAVE_LOAD + TES3_PATCH_MAGIC_SAVE_LOAD_SIZE);

	constexpr auto TES3_ActorAnimController_attackCheckMeleeHit = 0x541530;

	constexpr auto TES3_BaseObject_destructor = 0x4F0CA0;

	// Initialize singleton.
	LuaManager LuaManager::singleton;

	// Fast-access mapping from a TES3::Script* to its associated Lua module.
	static std::unordered_map<unsigned long, sol::object> scriptOverrides;

	// The currently executing overwritten script.
	static LuaScript* currentOverwrittenScript = NULL;

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

		auto script = dataHandler->nonDynamicData->findScriptByName(scriptId);
		if (script == nullptr) {
			return false;
		}

		if (target.is<sol::function>()) {
			scriptOverrides[(unsigned long)script] = target;
			script->header.dataSize = 0;
			return true;
		}
		else if (target.is<std::string>()) {
			auto& luaManager = mwse::lua::LuaManager::getInstance();
			const auto stateHandle = luaManager.getThreadSafeStateHandle();
			auto& state = stateHandle.getState();
			sol::object result = state.safe_script_file("./Data Files/MWSE/mods/" + target.as<std::string>() + ".lua");
			if (result.get_type() == sol::type::table) {
				scriptOverrides[(unsigned long)script] = result;
				script->header.dataSize = 0;
				return true;
			}
		}

		return false;
	}

	bool LuaManager::clearScriptOverride(const char* scriptId) {
		auto dataHandler = TES3::DataHandler::get();
		if (dataHandler == nullptr) {
			return false;
		}

		auto script = dataHandler->nonDynamicData->findScriptByName(scriptId);
		if (script == nullptr) {
			return false;
		}

		return scriptOverrides.erase((unsigned long)script) == 1;
	}

	void LuaManager::lua_print(sol::object message) {
		auto& luaManager = mwse::lua::LuaManager::getInstance();
		const auto stateHandle = luaManager.getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		static sol::protected_function luaTostring = state["tostring"];
		std::string result = luaTostring(message);

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
		auto luaMWSE = luaState.create_named_table("mwse");
		luaMWSE.create_named("activeLuaMods");
		luaState.create_named_table("mwscript");

		// Bind config.
		Configuration::bindToLua();

		// Expose timers.
		bindLuaTimer();
		bindMWSEUtil();

		// Extend OS library.
		luaState["os"]["createProcess"] = createProcess;
		luaState["os"]["getClipboardText"] = getClipboardText;
		luaState["os"]["openURL"] = openURL;
		luaState["os"]["setClipboardText"] = setClipboardText;
		luaState["os"]["getCommandLine"] = getCommandLine;
		LuaExecutor::defineLuaBindings();

		// Extend math library.
		luaState["math"]["nfhuge"] = std::numeric_limits<float>::min();
		luaState["math"]["fhuge"] = std::numeric_limits<float>::max();
		luaState["math"]["epsilon"] = std::numeric_limits<double>::epsilon();
		luaState["math"]["fepsilon"] = std::numeric_limits<float>::epsilon();

		// Evil YAML support.
		luaState.create_named_table("yaml");
		luaState["yaml"]["decode"] = OpenMW::LuaUtil::loadYaml;

		// Bind TES3 data types.
		bindTES3ActionData();
		bindTES3Activator();
		bindTES3ActorAnimationController();
		bindTES3AI();
		bindTES3Alchemy();
		bindTES3AnimationData();
		bindTES3AnimationGroup();
		bindTES3Apparatus();
		bindTES3Archive();
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
		bindTES3Crime();
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
		bindTES3Land();
		bindTES3LandTexture();
		bindTES3LeveledList();
		bindTES3Light();
		bindTES3Lockpick();
		bindTES3MagicEffect();
		bindTES3MagicEffectInstance();
		bindTES3MagicSourceInstance();
		bindTES3Misc();
		bindTES3MobManager();
		bindTES3MobileActor();
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
		bindTES3VFX();
		bindTES3WaterController();
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
		bindNIBoundingVolume();
		bindNIBSAnimationNode();
		bindNICamera();
		bindNICollisionGroup();
		bindNICollisionSwitch();
		bindNIColor();
		bindNIDynamicEffect();
		bindNIExtraData();
		bindNIGeometryData();
		bindNINode();
		bindNIObject();
		bindNILines();
		bindNILight();
		bindNIParticles();
		bindNIPick();
		bindNIPixelData();
		bindNIProperties();
		bindNIQuaternion();
		bindNIRenderedTexture();
		bindNIRenderer();
		bindNISkinInstance();
		bindNISourceTexture();
		bindNISwitchNode();
		bindNITextureEffect();
		bindNITexture();
		bindNITimeController();
		bindNITriShape();

		// Bind our disable event manager.
		mwse::lua::event::DisableableEventManager::bindToLua();
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
		TES3::Reference* reference = *reinterpret_cast<TES3::Reference**>(TES3_SCRIPTTARGETREF_IMAGE);
		manager.setCurrentReference(reference);
		manager.setCurrentScript(script);

		// Get and run the execute function.
		const auto stateHandle = manager.getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
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
				log::getLog() << "Lua error encountered when override of script '" << script->header.name << "':" << std::endl << error.what() << std::endl;
				TES3::WorldController::get()->stopGlobalScript(script);
			}
		}
		else {
			log::getLog() << "No execute function found for script override of '" << script->header.name << "'. Script execution stopped." << std::endl;
			TES3::WorldController::get()->stopGlobalScript(script);
		}

		manager.setCurrentReference(nullptr);
		manager.setCurrentScript(nullptr);
	}

	// Hook for HookRunScriptIndirect.
	static DWORD callbackRunScript = TES3_HOOK_RUNSCRIPT_LUACHECK_RETURN;
#define HookRunScript_TargetReference 0x7CEBECu
	static_assert(TES3_SCRIPTTARGETREF_IMAGE == HookRunScript_TargetReference, "Script reference address failed validation for hook. constexpr cannot be used in assembly reliably.");
	static __declspec(naked) void HookRunScript() {
		__asm
		{
			// Save all registers.
			pushad

			// Actually use our hook.
			push ebx
			call RunScript

			// Restore all registers.
			popad

			// Overwritten code.
			mov ecx, dword ptr ds:[HookRunScript_TargetReference]

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
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		state["tes3"]["mobilePlayer"] = macp;
		state["tes3"]["player"] = player;
		state["tes3"]["player1stPerson"] = macp->firstPersonReference;

		// Ensure that the player reference gets activated.
		player->setReferenceActive();
	}

	TES3::Reference* __fastcall OnPlayerReferenceCreated(TES3::Reference* player) {
		// Call overwritten function.
		player->ctor();

		// Grab the new player pointers for lua.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		state["tes3"]["player"] = player;

		return player;
	}

	void __fastcall OnPlayerMobileDestruction(TES3::MobilePlayer* macp) {
		// Call overwritten code.
		const auto TES3_MobilePlayer_Dtor = reinterpret_cast<void(__thiscall*)(TES3::MobilePlayer*)>(0x5662F0);
		TES3_MobilePlayer_Dtor(macp);

		// Clear the player shortcuts.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		state["tes3"]["mobilePlayer"] = sol::nil;
		state["tes3"]["player"] = sol::nil;
		state["tes3"]["player1stPerson"] = sol::nil;
	}

	//
	// Hook: Finished initializing game code.
	//

	void __fastcall InitDataHandler(TES3::WorldController* worldController, DWORD _EDX_, NI::Node* objectRoot, NI::Node* pickObjectRoot, NI::Node* landRoot, NI::Light* skyLight, NI::FogProperty* fogProperty) {
		// Call overwritten code.
		const auto TES3_WorldController_InitDataHandler = reinterpret_cast<void(__thiscall*)(TES3::WorldController*, NI::Node*, NI::Node*, NI::Node*, NI::Light*, NI::FogProperty*)>(0x40E790);
		TES3_WorldController_InitDataHandler(worldController, objectRoot, pickObjectRoot, landRoot, skyLight, fogProperty);

		// Hook up shorthand access to data handler, world controller, and game.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		state["tes3"]["dataHandler"] = TES3::DataHandler::get();
		state["tes3"]["worldController"] = TES3::WorldController::get();
		state["tes3"]["game"] = TES3::Game::get();
	}

	bool __fastcall FinishInitialization(TES3::Game* game) {
		// Call overwritten code.
		const auto TES3Game_loadAllPlugins = reinterpret_cast<bool(__thiscall*)(TES3::Game*)>(0x419CE0);
		TES3Game_loadAllPlugins(game);

		// Update compatibility globals.
		const auto mwseBuildGlobal = TES3::DataHandler::get()->nonDynamicData->findGlobalVariable("MWSE_BUILD");
		if (mwseBuildGlobal) {
			mwseBuildGlobal->value = Configuration::BuildNumber;
		}

		// Trigger initialization.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		stateHandle.triggerEvent(new event::GenericEvent("initialized"));

		// Return success.
		return true;
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
		auto dataHandler = TES3::DataHandler::get();
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
			const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
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
			const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
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
			const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
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

	const auto TES3_Game_initPlayerAndStartScripts = reinterpret_cast<void(__thiscall*)(TES3::Game*)>(0x419A90);

	void __fastcall OnNewGameViaStartingCell1(TES3::Game* game) {
		// Call overwritten code.
		// It must run before the event, so that the player and scripts are resolved before being exposed to lua.
		TES3_Game_initPlayerAndStartScripts(game);

		// Fire off the load event.
		LuaManager& luaManager = LuaManager::getInstance();
		const auto stateHandle = luaManager.getThreadSafeStateHandle();
		if (event::LoadGameEvent::getEventEnabled()) {
			stateHandle.triggerEvent(new event::LoadGameEvent(nullptr, false, true));
		}
	}

	void __fastcall OnNewGameViaStartingCell2(TES3::MobManager* mobManager) {
		// Call overwritten code.
		mobManager->checkPlayerDistance();

		// Fire off the loaded/cellChanged events.
		LuaManager& luaManager = LuaManager::getInstance();
		const auto stateHandle = luaManager.getThreadSafeStateHandle();
		if (event::LoadedGameEvent::getEventEnabled()) {
			stateHandle.triggerEvent(new event::LoadedGameEvent(nullptr, false, true));
		}

		// Update compatibility globals.
		const auto mwseBuildGlobal = TES3::DataHandler::get()->nonDynamicData->findGlobalVariable("MWSE_BUILD");
		if (mwseBuildGlobal) {
			mwseBuildGlobal->value = Configuration::BuildNumber;
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
		reinterpret_cast<void(__thiscall*)(TES3::ActorAnimationController*)>(TES3_ActorAnimController_attackCheckMeleeHit)(animController);

		// Prepare our event data.
		if (event::AttackEvent::getEventEnabled()) {
			LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::AttackEvent(animController));
		}
	}

	void __fastcall OnAttackStart(TES3::ActorAnimationController* animController, DWORD _UNUSED_, float swing) {
		// Call our wrapper for the function so that events are triggered.
		animController->startAttackAnimation(swing);
	}

	void __fastcall OnAttackStrike(TES3::MobileActor* mobileActor) {
		// Invoke event before the action.
		if (event::AttackHitEvent::getEventEnabled()) {
			LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::AttackHitEvent(mobileActor));
		}

		// Call original function.
		const auto TES3_MACT_combatWeaponStrike = reinterpret_cast<void(__thiscall*)(TES3::MobileActor*)>(0x556F40);
		TES3_MACT_combatWeaponStrike(mobileActor);
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

	bool __cdecl OnUIEvent(TES3::UI::EventCallback callback, TES3::UI::Element* parent, TES3::UI::Property prop, DWORD b, DWORD c, TES3::UI::Element* source) {
		// Execute event. If the event blocked the call, bail.
		mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
		if (event::GenericUiPreEvent::getEventEnabled()) {
			const auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::table eventData = stateHandle.triggerEvent(new event::GenericUiPreEvent(parent, source, (unsigned int)prop, b, c));
			if (eventData.valid() && eventData.get_or("block", false)) {
				return 0;
			}
		}

		const auto result = callback(parent, prop, b, c, source);

		if (event::GenericUiPostEvent::getEventEnabled()) {
			luaManager.getThreadSafeStateHandle().triggerEvent(new event::GenericUiPostEvent(parent, source, (unsigned int)prop, b, c));
		}

		return result;
	}

	static DWORD callbackUIEvent = TES3_HOOK_UI_EVENT_RETURN;
	static __declspec(naked) void HookUIEvent() {
		__asm
		{
			push edi
			call OnUIEvent
			add esp, 0x18
			jmp callbackUIEvent
		}
	}

	TES3::UI::Element* __cdecl OnUITooltipEvent(TES3::UI::EventCallback callback, TES3::UI::Element* parent, TES3::UI::Property prop, DWORD b, DWORD c, TES3::UI::Element* source) {
		TES3::UI::Element* sourceCache = source;
		std::swap(TES3::UI::MenuInputController::lastTooltipSource, sourceCache);
		callback(parent, prop, b, c, source);
		std::swap(TES3::UI::MenuInputController::lastTooltipSource, sourceCache);
		return TES3::UI::findHelpLayerMenu(TES3::UI::UI_ID(TES3::UI::Property::HelpMenu));
	}

	__declspec(naked) void HookOnUITooltipEvent() {
		__asm
		{
			push edi					// Size: 0x1
			nop							// Replaced with a call generation. Can't do so here, because offsets aren't accurate.
			nop							// ^
			nop							// ^
			nop							// ^
			nop							// ^
			add esp, 0x18				// Size: 0x3
		}
	}
	const size_t HookOnUITooltipEvent_size = 0x9;

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
		const auto stateHandle = luaManager.getThreadSafeStateHandle();
		for (BYTE i = 0; i < UINT8_MAX; ++i) {
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
		for (BYTE i = 0; i < 8; ++i) {
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
		mwse::lua::event::DamageHandToHandEvent::m_Source = "attack";
		auto result = mobileActor->applyFatigueDamage(damage, swing, alwaysPlayHitVoice);
		mwse::lua::event::DamageHandToHandEvent::m_Source = nullptr;
		mwse::lua::event::DamageHandToHandEvent::m_Attacker = nullptr;
		return result;
	}

	static __declspec(naked) float OnApplyFatigueDamageFromAttack_Wrapper() {
		__asm {
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
		__asm {
			mov edx, ebx
			jmp OnApplyDamageFromMagicShield
		}
	}

	const auto TES3_AttributeSpellEffect = reinterpret_cast<bool(__cdecl*)(TES3::MagicSourceInstance*, bool, TES3::Statistic*, void*, TES3::MagicEffectInstance*, float, int)>(0x519110);
	bool __cdecl AttributeSpellEffect(TES3::MagicSourceInstance* sourceInstance, bool positiveModifier, TES3::Statistic* statistic, void* attributeTypeInfo, TES3::MagicEffectInstance* effectInstance, float delta, int effectIndex) {
		mwse::lua::event::DamageEvent::m_MagicSourceInstance = sourceInstance;
		mwse::lua::event::DamageEvent::m_MagicEffectInstance = effectInstance;
		mwse::lua::event::DamageEvent::m_MagicEffectIndex = effectIndex;
		auto result = TES3_AttributeSpellEffect(sourceInstance, positiveModifier, statistic, attributeTypeInfo, effectInstance, delta, effectIndex);
		mwse::lua::event::DamageEvent::m_MagicSourceInstance = nullptr;
		mwse::lua::event::DamageEvent::m_MagicEffectInstance = nullptr;
		mwse::lua::event::DamageEvent::m_MagicEffectIndex = -1;
		return result;
	}

	//
	// Shield blocked event.
	//

	__declspec(naked) void patchOnShieldWearFromBlocking() {
		__asm {
			push eax	// damage
			push esi	// attacker
			push ebp	// target
		}
	}
	const size_t patchOnShieldWearFromBlocking_size = 0x3;

	const auto TES3_MobileActor_ApplyShieldWear = reinterpret_cast<void(__thiscall*)(TES3::MobileActor*, float)>(0x558690);
	void __stdcall OnShieldWearFromBlocking(TES3::MobileActor* target, TES3::MobileActor* attacker, float damage) {
		// Trigger event. Allow event to modify shield wear.
		if (event::ShieldBlockedEvent::getEventEnabled()) {
			auto& luaManager = mwse::lua::LuaManager::getInstance();
			const auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::table eventData = stateHandle.triggerEvent(new event::ShieldBlockedEvent(target, attacker, damage));
			if (eventData.valid()) {
				damage = eventData["conditionDamage"];
			}
		}

		// Call original function.
		TES3_MobileActor_ApplyShieldWear(target, damage);
	}

	//
	// Projectile expire event.
	//

	void __fastcall OnProjectileExpire(TES3::MobManager* mobManager, DWORD _UNUSED_, TES3::Reference* reference) {
		// Get the fired projectile, and trigger an event for it.
		if (event::ProjectileExpireEvent::getEventEnabled()) {
			TES3::MobileProjectile* projectile = reference->getAttachedMobileProjectile();
			LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::ProjectileExpireEvent(projectile));
		}

		// Call overwritten function.
		const auto removeMob = reinterpret_cast<void(__thiscall*)(TES3::MobManager*, TES3::Reference*)>(0x5637F0);
		removeMob(mobManager, reference);
	}

	//
	// Spell resist event.
	//

	void __cdecl OnSpellResist(TES3::MagicSourceInstance* spellInstance, TES3::MagicEffectInstance* effectInstance, int effectIndex, int resistAttribute) {
		// Call original function.
		reinterpret_cast<void(__cdecl*)(TES3::MagicSourceInstance*, TES3::MagicEffectInstance*, int, int)>(0x517E40)(spellInstance, effectInstance, effectIndex, resistAttribute);

		// Trigger event, see if we want to overwrite the resisted percentage.
		if (event::SpellResistEvent::getEventEnabled()) {
			auto& luaManager = mwse::lua::LuaManager::getInstance();
			const auto stateHandle = luaManager.getThreadSafeStateHandle();
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
		TES3::UI::Element* dialog = TES3::UI::findMenu("MenuDialog");
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
		reinterpret_cast<void(__stdcall*)()>(0x626220)();

		// Launch our event.
		if (event::LevelUpEvent::getEventEnabled()) {
			LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::LevelUpEvent());
		}
	}

	//
	// Alchemy brewing potion strength and skill check event.
	//

	TES3::Item* getAlchemyMenuObject(DWORD parentIdAddress) {
		using namespace TES3::UI;

		auto menuAlchemy = findMenu(*reinterpret_cast<UI_ID*>(0x7D225C));
		if (!menuAlchemy) {
			return nullptr;
		}

		auto child = menuAlchemy->findChild(*reinterpret_cast<UI_ID*>(parentIdAddress));
		if (!child) {
			return nullptr;
		}

		Property MenuAlchemy_object = *reinterpret_cast<Property*>(0x7D2344);
		return reinterpret_cast<TES3::Item*>(child->getProperty(PropertyType::Pointer, MenuAlchemy_object).ptrValue);
	}

	const auto TES3_alchemySkillRoll = reinterpret_cast<float(__cdecl*)()>(0x59D610);
	float __cdecl OnAlchemySkillRoll() {
		float potionStrength = TES3_alchemySkillRoll();

		TES3::AlchemyBrewingItems items = {
			static_cast<TES3::Apparatus*>(getAlchemyMenuObject(0x7D221C)),
			static_cast<TES3::Apparatus*>(getAlchemyMenuObject(0x7D2270)),
			static_cast<TES3::Apparatus*>(getAlchemyMenuObject(0x7D2298)),
			static_cast<TES3::Apparatus*>(getAlchemyMenuObject(0x7D2314)),
			static_cast<TES3::Ingredient*>(getAlchemyMenuObject(0x7D22F0)),
			static_cast<TES3::Ingredient*>(getAlchemyMenuObject(0x7D22F2)),
			static_cast<TES3::Ingredient*>(getAlchemyMenuObject(0x7D22C0)),
			static_cast<TES3::Ingredient*>(getAlchemyMenuObject(0x7D2292)),
		};

		if (event::PotionBrewSkillCheckEvent::getEventEnabled()) {
			const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::table eventData = stateHandle.triggerEvent(new event::PotionBrewSkillCheckEvent(potionStrength, items));

			if (eventData.valid()) {
				if (eventData.get_or("success", true)) {
					potionStrength = eventData.get_or("potionStrength", potionStrength);
				}
				else {
					potionStrength = -1.0f;
				}
			}
		}
		return potionStrength;
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

	const auto TES3_AttemptPotionBrew = reinterpret_cast<bool(__cdecl*)()>(0x59C030);
	bool __cdecl OnBrewPotionAttempt() {
		// Reset success state.
		lastBrewedPotion = nullptr;

		// Store apparatus used. MobilePlayer::lastUsed* members do not reflect what is currently equipped by the game.
		// Store the last used ingredients, since they get nuked.
		TES3::AlchemyBrewingItems items = {
			static_cast<TES3::Apparatus*>(getAlchemyMenuObject(0x7D221C)),
			static_cast<TES3::Apparatus*>(getAlchemyMenuObject(0x7D2270)),
			static_cast<TES3::Apparatus*>(getAlchemyMenuObject(0x7D2298)),
			static_cast<TES3::Apparatus*>(getAlchemyMenuObject(0x7D2314)),
			static_cast<TES3::Ingredient*>(getAlchemyMenuObject(0x7D22F0)),
			static_cast<TES3::Ingredient*>(getAlchemyMenuObject(0x7D22F2)),
			static_cast<TES3::Ingredient*>(getAlchemyMenuObject(0x7D22C0)),
			static_cast<TES3::Ingredient*>(getAlchemyMenuObject(0x7D2292)),
		};

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
				LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::PotionBrewedEvent(lastBrewedPotion, items));
			}
		}
		else {
			if (event::PotionBrewFailedEvent::getEventEnabled()) {
				LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::PotionBrewFailedEvent(items));
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
		auto soulUsed = soulGemItemData ? soulGemItemData->getSoul() : nullptr;

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
		const auto stateHandle = luaManager.getThreadSafeStateHandle();
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
		__asm
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
		__asm
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
	// Event: Spell resisted
	//

	void __stdcall OnSpellResisted() {
		// Get cached spellEffectEvent function parameters.
		TES3::MagicSourceInstance* sourceInstance = TES3::MagicEffectController::cachedSpellEffectEventSourceInstance;
		TES3::MagicEffectInstance* effectInstance = TES3::MagicEffectController::cachedSpellEffectEventEffectInstance;
		int effectIndex = TES3::MagicEffectController::cachedSpellEffectEventEffectIndex;

		// Overwritten code from 0x51880F.
		effectInstance->state = int(TES3::SpellEffectState::Retired);

		// Trigger the event.
		if (event::SpellResistedEvent::getEventEnabled()) {
			LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::SpellResistedEvent(sourceInstance, effectInstance, effectIndex));
		}
	}

	void __declspec(naked) OnSpellResistedWrapper() {
		__asm {
			// Call our resisted function safely.
			push eax
			push ecx
			call OnSpellResisted
			pop ecx
			pop eax
			ret
		}
	}

	//
	// Event: Magic effect removed
	//

	bool __fastcall OnMagicEffectRemoved(TES3::Deque<TES3::ActiveMagicEffect>* activeMagicEffects, DWORD _UNUSED_, int serial, int effectIndex) {
		// Overwritten code from 0x55C9D0.
		if (!activeMagicEffects->count) {
			return false;
		}
		TES3::Deque<TES3::ActiveMagicEffect>::Node* sentinel = activeMagicEffects->sentinel;
		TES3::Deque<TES3::ActiveMagicEffect>::Node* node = sentinel->next;
		if (sentinel->next == sentinel) {
			return false;
		}
		while (node->data.magicInstanceSerial != serial || node->data.magicInstanceEffectIndex != effectIndex) {
			node = node->next;
			if (node == sentinel) {
				return false;
			}
		}

		// Dispatch the event.
		TES3::MagicSourceInstance* magicSourceInstance = TES3::WorldController::get()->magicInstanceController->getInstanceFromSerial(serial);
		TES3::MobileActor* mobileActor = reinterpret_cast<TES3::MobileActor*>(reinterpret_cast<BYTE*>(activeMagicEffects) - offsetof(TES3::MobileActor, activeMagicEffects));

		if (event::MagicEffectRemovedEvent::getEventEnabled()) {
			LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::MagicEffectRemovedEvent(mobileActor, magicSourceInstance, effectIndex));
		}

		// Overwritten code from 0x55C9D0.
		node->previous->next = node->next;
		node->next->previous = node->previous;
		delete(node);
		activeMagicEffects->count--;
		return true;
	}

	//
	// Event: Calculate Rest Interruption
	//

	void __cdecl OnInterruptRestCheck(bool interruptRest) {
		// Call original function.
		reinterpret_cast<void(__cdecl*)(bool)>(0x634B90)(interruptRest);

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
			const auto stateHandle = luaManager.getThreadSafeStateHandle();
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

	TES3::Object* __fastcall OnInterruptRest(TES3::LeveledCreature* leveledCreature, DWORD _UNUSED_) {
		// Creature that we return.
		TES3::Object* creature = NULL;

		// Fire off an event and change the determined creature.
		if (event::RestInterruptEvent::getEventEnabled()) {
			mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
			const auto stateHandle = luaManager.getThreadSafeStateHandle();
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
				if (maybeCreature.is<TES3::Creature*>() || maybeCreature.is<TES3::NPC*>()) {
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

	void __fastcall OnAddTopicAtIndex(TES3::IteratedList<TES3::Dialogue*>* topicList, DWORD _UNUSED_, TES3::Dialogue* topic, unsigned int index) {
		// Run overwritten function.
		topicList->insert(index, topic);

		// Raise event.
		if (event::AddTopicEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::AddTopicEvent(topic));
		}
	}

	void __fastcall OnAddTopic(TES3::IteratedList<TES3::Dialogue*>* topicList, DWORD _UNUSED_, TES3::Dialogue* topic) {
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

	bool __fastcall OnJournalAdd(TES3::Dialogue* dialogue, DWORD _UNUSED_, int index, TES3::MobileActor* actor) {
		return dialogue->addToJournal(index, actor);
	}

	void __fastcall OnJournalSet(TES3::Dialogue* dialogue, DWORD _UNUSED_, int index) {
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
			const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
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
			const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			stateHandle.triggerEvent(new event::GenericUiActivatedEvent(element));

			// DEPRECATED. TODO: Remove before 2.1 final.
			stateHandle.triggerEvent(new event::GenericUiCreatedEvent(element));
		}
	}

	void __cdecl OnUICreatedAfterConsoleMenuFocus(TES3::UI::Element* widget, int event, int data0, int data1, TES3::UI::Element* element) {
		// Overwritten code.
		const auto ui_ConsoleMenu_onFocus = reinterpret_cast<void(__cdecl*)(TES3::UI::Element*, int, int, int, TES3::UI::Element*)>(0x5B23E0);
		ui_ConsoleMenu_onFocus(widget, event, data0, data1, element);

		// Fire off the event.
		if (event::GenericUiActivatedEvent::getEventEnabled()) {
			const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
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
			const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			stateHandle.triggerEvent(new event::GenericUiActivatedEvent(element));

			// DEPRECATED. TODO: Remove before 2.1 final.
			stateHandle.triggerEvent(new event::GenericUiCreatedEvent(element));
		}
	}

	void __fastcall OnUICreatedAfterMenuVisibleEvent(TES3::UI::Element* element, DWORD _UNUSED_, bool unknown) {
		// Overwritten code.
		const auto ui_setMenuVisibleWithEvent = reinterpret_cast<void(__thiscall*)(TES3::UI::Element*, bool)>(0x582F20);
		ui_setMenuVisibleWithEvent(element, unknown);

		// Fire off the event.
		if (event::GenericUiActivatedEvent::getEventEnabled()) {
			const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			stateHandle.triggerEvent(new event::GenericUiActivatedEvent(element));

			// DEPRECATED. TODO: Remove before 2.1 final.
			stateHandle.triggerEvent(new event::GenericUiCreatedEvent(element));
		}
	}

	void __cdecl OnUICreatedAfterScrollPaneUpdate(TES3::UI::Element* element) {
		// Overwritten code.
		const auto ui_ScrollPane_UpdateScrollBars = reinterpret_cast<void(__cdecl*)(TES3::UI::Element*)>(0x649E40);
		ui_ScrollPane_UpdateScrollBars(element);

		// Fire off the event.
		if (event::GenericUiActivatedEvent::getEventEnabled()) {
			const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
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
		bool created = reinterpret_cast<bool(__cdecl*)(TES3::UI::Element*, int, int, int, TES3::UI::Element*)>(0x5E45C0)(widget, event, data0, data1, element);

		// Fire off the event.
		if (created && event::UiSpellTooltipEvent::getEventEnabled()) {
			TES3::UI::Element* tooltip = TES3::UI::findHelpLayerMenu(TES3::UI::UI_ID(TES3::UI::Property::HelpMenu));
			TES3::Spell* spell = reinterpret_cast<TES3::Spell*>(element->getProperty(TES3::UI::PropertyType::Pointer, *reinterpret_cast<TES3::UI::Property*>(0x7D431C)).ptrValue);
			LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::UiSpellTooltipEvent(tooltip, spell));
		}

		return created;
	}

	//
	// Event: UI Skill Tooltip (in stat menu) post-creation.
	//

	bool __cdecl OnUISkillStatTooltip(TES3::UI::Element* widget, int event, int data0, int data1, TES3::UI::Element* element) {
		// Call original function.
		bool created = reinterpret_cast<bool(__cdecl*)(TES3::UI::Element*, int, int, int, TES3::UI::Element*)>(0x627D70)(widget, event, data0, data1, element);

		// Fire off the event.
		if (created && event::UiSkillTooltipEvent::getEventEnabled()) {
			TES3::UI::Element* tooltip = TES3::UI::findHelpLayerMenu(TES3::UI::UI_ID(TES3::UI::Property::HelpMenu));
			int skillID = element->getProperty(TES3::UI::PropertyType::Integer, *reinterpret_cast<TES3::UI::Property*>(0x7D6B82)).integerValue;
			LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::UiSkillTooltipEvent(tooltip, skillID, 1));
		}

		return created;
	}

	//
	// Event: UI Skill Tooltip post-creation.
	//

	void __cdecl OnUISkillTooltip(char skillID) {
		// Call original function.
		reinterpret_cast<void(__cdecl*)(char)>(0x6297F0)(skillID);

		// Fire off the event.
		if (event::UiSkillTooltipEvent::getEventEnabled()) {
			TES3::UI::Element* tooltip = TES3::UI::findHelpLayerMenu(TES3::UI::UI_ID(TES3::UI::Property::HelpMenu));
			LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::UiSkillTooltipEvent(tooltip, skillID, 0));
		}
	}

	//
	// Event: Activation Target Changed
	//

	static void __fastcall OnCheckPlayerActivationTarget(TES3::Game* game) {
		auto& previous = event::ActivationTargetChangedEvent::ms_PreviousReference;
		previous = game->playerTarget;

		const auto TES3_Game_CheckPlayerActivationTarget = reinterpret_cast<void(__thiscall*)(TES3::Game*)>(0x41CA50);
		TES3_Game_CheckPlayerActivationTarget(game);

		if (game->playerTarget != previous && event::ActivationTargetChangedEvent::getEventEnabled()) {
			const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			stateHandle.triggerEvent(new event::ActivationTargetChangedEvent(game->playerTarget));
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
		return reinterpret_cast<bool(__thiscall*)(TES3::Cell*)>(0x4E22F0)(cell);
	}

	void __fastcall OnWeatherImmediateChange(TES3::WeatherController* controller, DWORD _UNUSED_, int weatherId, float transitionScalar) {
		// Call original function.
		controller->switchWeather(weatherId, transitionScalar);

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
		return reinterpret_cast<void* (__cdecl*)(const char*, void*)>(0x6DE7F0)(texturePath, data);
	}

	void __fastcall OnWeatherTransitionEnd(void* modelData, DWORD _UNUSED_) {
		// Fire off the event.
		if (event::WeatherTransitionFinishedEvent::getEventEnabled()) {
			LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::WeatherTransitionFinishedEvent());
		}

		// Call original function.
		reinterpret_cast<void(__thiscall*)(void*)>(0x414890)(modelData);
	}

	//
	// Event: Music, new track
	//

	bool __fastcall OnSelectMusicTrack(TES3::WorldController* controller, DWORD _UNUSED_, TES3::MusicSituation situation) {
		return controller->selectNextMusicTrack(situation);
	}

	//
	// Event: Music, change track
	//

	void __fastcall OnChangeMusic_Explore(TES3::AudioController* audioController, DWORD _EDX_, const char* path, int crossfadeMS, float volume) {
		event::MusicChangeTrackEvent::ms_Context = "explore";
		TES3::WorldController::get()->musicSituation = TES3::MusicSituation::Explore;
		audioController->changeMusicTrack(path, crossfadeMS, volume);
	}

	void __fastcall OnChangeMusic_Combat(TES3::AudioController* audioController, DWORD _EDX_, const char* path, int crossfadeMS, float volume) {
		event::MusicChangeTrackEvent::ms_Context = "combat";
		TES3::WorldController::get()->musicSituation = TES3::MusicSituation::Combat;
		audioController->changeMusicTrack(path, crossfadeMS, volume);
	}

	void __fastcall OnChangeMusic_TitleLoop(TES3::AudioController* audioController, DWORD _EDX_, const char* path, int crossfadeMS, float volume) {
		event::MusicChangeTrackEvent::ms_Context = "title";
		TES3::WorldController::get()->musicSituation = TES3::MusicSituation::Uninterruptible;
		audioController->changeMusicTrack(path, crossfadeMS, volume);
	}

	void __fastcall OnChangeMusic_Event(TES3::AudioController* audioController, DWORD _EDX_, const char* path, int crossfadeMS, float volume) {
		TES3::WorldController::get()->musicSituation = TES3::MusicSituation::Uninterruptible;
		audioController->changeMusicTrack(path, crossfadeMS, volume);
	}

	const auto TES3_WorldController_PlayEventMusic = reinterpret_cast<void(__thiscall*)(TES3::WorldController*, const char*, bool)>(0x410FA0);

	void __fastcall OnChangeMusic_Event_Death(TES3::WorldController* worldController, DWORD _EDX_, const char* path, bool isMusic) {
		event::MusicChangeTrackEvent::ms_Context = "death";
		TES3_WorldController_PlayEventMusic(worldController, path, isMusic);
	}

	void __fastcall OnChangeMusic_Event_MainMenu(TES3::WorldController* worldController, DWORD _EDX_, const char* path, bool isMusic) {
		event::MusicChangeTrackEvent::ms_Context = "title";
		TES3_WorldController_PlayEventMusic(worldController, path, isMusic);
	}

	void __fastcall OnChangeMusic_Event_LevelUp(TES3::WorldController* worldController, DWORD _EDX_, const char* path, bool isMusic) {
		event::MusicChangeTrackEvent::ms_Context = "levelup";
		TES3_WorldController_PlayEventMusic(worldController, path, isMusic);
	}

	void __fastcall OnChangeMusic_MwscriptStreamMusic(TES3::AudioController* audioController, DWORD _EDX_, const char* path, int crossfadeMS, float volume) {
		event::MusicChangeTrackEvent::ms_Context = "mwscript";
		TES3::WorldController::get()->musicSituation = TES3::MusicSituation::Uninterruptible;
		audioController->changeMusicTrack(path, 1000, volume);
	}

	//
	// Event: Weapon Ready
	//

	const auto TES3_MobileActor_offhandAnimState = reinterpret_cast<void(__thiscall*)(const TES3::MobileActor*)>(0x52D5B0);

	void __fastcall OnReadyNoWeapon(TES3::MobileActor* actor) {
		TES3_MobileActor_offhandAnimState(actor);
		if (actor->reference && event::WeaponReadiedEvent::getEventEnabled()) {
			LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::WeaponReadiedEvent(actor->reference));
		}
	}

	const auto TES3_MobileActor_unreadyWeapon = reinterpret_cast<void(__thiscall*)(const TES3::MobileActor*)>(0x528000);

	void __fastcall OnUnreadyWeapon(TES3::MobileActor* mobile) {
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
	void __fastcall ContainerCloseResolveLeveledLists(TES3::Inventory* inventory, DWORD _EDX_, TES3::MobileActor* mobile) {
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

	// Patch NPCClone::ctor_args to make the reference available during inventory resolve.
	__declspec(naked) void patchNPCCloneResolveLeveledLists() {
		__asm {
			push edi		// push reference
			jmp $ + 0xF		// skip replaced code
		}
	}
	const size_t patchNPCCloneResolveLeveledLists_size = 0x6;

	void __fastcall NPCCloneResolveLeveledLists(TES3::Inventory* inventory, DWORD _EDX_, TES3::Reference* reference) {
		mwse::lua::event::LeveledItemPickedEvent::m_Reference = reference;

		TES3::MobileActor* mobile = reference ? reference->getAttachedMobileActor() : nullptr;
		TES3_Inventory_resolveLeveledLists(inventory, mobile);

		mwse::lua::event::LeveledItemPickedEvent::m_Reference = nullptr;
	}

	//
	// Event: Calculate barter price.
	//

	// Store the last item stack sold/bought.
	static TES3::EquipmentStack* OnCalculateBarterPrice_stack = nullptr;

	// Store the last item stack's calculated base price.
	static int OnCalculateBarterPrice_value = 0;

	// Hook point for calculating an item's price, before our real event. We'll store the above values here.
	int __cdecl OnCalculateBarterPrice_CalcItemValue(TES3::EquipmentStack* stack) {
		OnCalculateBarterPrice_stack = stack;
		OnCalculateBarterPrice_value = stack->getAdjustedValue();
		return OnCalculateBarterPrice_value;
	}

	// Use the above stored values and finish our event.
	int __fastcall OnCalculateBarterPrice(TES3::MobileActor* mobile, DWORD _UNUSED_, int basePrice, bool buying) {
		int price = mobile->determineModifiedPrice(basePrice, buying);
		int count = 1;
		if (OnCalculateBarterPrice_value > 0) {
			count = std::abs(basePrice / OnCalculateBarterPrice_value);
		}

		if (event::CalculateBarterPriceEvent::getEventEnabled()) {
			auto& luaManager = mwse::lua::LuaManager::getInstance();
			const auto stateHandle = luaManager.getThreadSafeStateHandle();
			auto item = static_cast<TES3::Item*>(OnCalculateBarterPrice_stack ? OnCalculateBarterPrice_stack->object : nullptr);
			auto itemData = OnCalculateBarterPrice_stack ? OnCalculateBarterPrice_stack->itemData : nullptr;
			sol::table result = stateHandle.triggerEvent(new event::CalculateBarterPriceEvent(mobile, basePrice, price, buying, count, item, itemData));
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
	int __fastcall OnCalculateRepairPriceForList(TES3::MobileActor* mobile, DWORD _UNUSED_, int basePrice, bool buying) {
		int price = mobile->determineModifiedPrice(basePrice, buying);

		if (basePrice <= 0) {
			basePrice = 1;
		}

		if (price <= 0) {
			price = 1;
		}

		if (event::CalculateRepairPriceEvent::getEventEnabled()) {
			auto& luaManager = mwse::lua::LuaManager::getInstance();
			const auto stateHandle = luaManager.getThreadSafeStateHandle();
			auto stack = OnCalculateRepairPriceForList_CurrentInventoryList->current->data;
			auto itemData = stack->variables && !stack->variables->empty() ? stack->variables->at(0) : nullptr;
			sol::table result = stateHandle.triggerEvent(new event::CalculateRepairPriceEvent(mobile, basePrice, price, stack->object, itemData));
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
	int __fastcall OnCalculateRepairPrice(TES3::MobileActor* mobile, DWORD _UNUSED_, int basePrice, bool buying) {
		int price = mobile->determineModifiedPrice(basePrice, buying);

		if (basePrice <= 0) {
			basePrice = 1;
		}

		if (price <= 0) {
			price = 1;
		}

		if (event::CalculateRepairPriceEvent::getEventEnabled()) {
			auto& luaManager = mwse::lua::LuaManager::getInstance();
			const auto stateHandle = luaManager.getThreadSafeStateHandle();
			auto itemData = OnCalculateRepairPrice_ItemStack->variables && !OnCalculateRepairPrice_ItemStack->variables->empty() ? OnCalculateRepairPrice_ItemStack->variables->at(0) : nullptr;
			sol::table result = stateHandle.triggerEvent(new event::CalculateRepairPriceEvent(mobile, basePrice, price, OnCalculateRepairPrice_ItemStack->object, itemData));
			if (result.valid()) {
				price = result["price"];
			}
		}

		return price;
	}

	//
	// Event: Calculate spellmaking price.
	//

	void __fastcall OnCalculateSpellmakingPrice(TES3::UI::Element* self, DWORD _UNUSED_, TES3::UI::Property id, TES3::UI::PropertyValue value, TES3::UI::PropertyType type) {
		if (event::CalculateSpellmakingPriceEvent::getEventEnabled()) {
			auto& luaManager = mwse::lua::LuaManager::getInstance();
			const auto stateHandle = luaManager.getThreadSafeStateHandle();

			auto serviceActor = TES3::UI::getSpellmakingServiceActor();

			sol::table result = stateHandle.triggerEvent(new event::CalculateSpellmakingPriceEvent(serviceActor, value.floatValue));
			if (result.valid()) {
				value.floatValue = result["price"];
			}
		}
		self->setProperty(id, value, type);
	}

	//
	// Event: Calculate enchantment price.
	//

	int __fastcall OnCalculateEnchantmentPrice(TES3::MobileActor* mobile, DWORD _UNUSED_, int basePrice, bool buying) {
		int price = mobile->determineModifiedPrice(basePrice, buying);

		if (event::CalculateEnchantmentPriceEvent::getEventEnabled()) {
			auto& luaManager = mwse::lua::LuaManager::getInstance();
			const auto stateHandle = luaManager.getThreadSafeStateHandle();

			sol::table result = stateHandle.triggerEvent(new event::CalculateEnchantmentPriceEvent(mobile, basePrice, price));
			if (result.valid()) {
				price = result["price"];
			}
		}

		return price;
	}

	//
	// Event: Calculate spellmaking spell point cost.
	//

	void __fastcall OnCalculateSpellmakingSpellPointCost(TES3::UI::Element* self, DWORD _UNUSED_, TES3::UI::Property id, TES3::UI::PropertyValue value, TES3::UI::PropertyType type) {
		if (event::CalculateSpellmakingSpellPointCostEvent::getEventEnabled()) {
			auto& luaManager = mwse::lua::LuaManager::getInstance();
			const auto stateHandle = luaManager.getThreadSafeStateHandle();

			auto serviceActor = TES3::UI::getSpellmakingServiceActor();

			sol::table result = stateHandle.triggerEvent(new event::CalculateSpellmakingSpellPointCostEvent(serviceActor, value.floatValue));
			if (result.valid()) {
				value.floatValue = result["spellPointCost"];
			}
		}
		self->setProperty(id, value, type);
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
	int __fastcall OnCalculateSpellPriceForList(TES3::MobileActor* mobile, DWORD _UNUSED_, int basePrice, bool buying) {
		int price = mobile->determineModifiedPrice(basePrice, buying);

		if (event::CalculateSpellPriceEvent::getEventEnabled()) {
			auto& luaManager = mwse::lua::LuaManager::getInstance();
			const auto stateHandle = luaManager.getThreadSafeStateHandle();
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

	int __fastcall OnCalculateSpellPrice(TES3::MobileActor* mobile, DWORD _UNUSED_, int basePrice, bool buying) {
		int price = mobile->determineModifiedPrice(basePrice, buying);

		if (event::CalculateSpellPriceEvent::getEventEnabled()) {
			auto& luaManager = mwse::lua::LuaManager::getInstance();
			const auto stateHandle = luaManager.getThreadSafeStateHandle();
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
	int __fastcall OnCalculateTrainingPrice(TES3::MobileActor* mobile, DWORD _UNUSED_, int basePrice, bool buying) {
		int price = mobile->determineModifiedPrice(basePrice, buying);

		if (event::CalculateTrainingPriceEvent::getEventEnabled()) {
			auto& luaManager = mwse::lua::LuaManager::getInstance();
			const auto stateHandle = luaManager.getThreadSafeStateHandle();
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
	static TES3::IteratedList<TES3::TravelDestination*>* OnCalculateTravelPrice_DestinationList;

	// The player's friendly actor list.
	static TES3::IteratedList<TES3::MobileActor*>* OnCalculateTravelPrice_CompanionList;

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
	int __fastcall OnCalculateTravelPriceForList(TES3::MobileActor* mobile, DWORD _UNUSED_, int basePrice, bool buying) {
		int price = mobile->determineModifiedPrice(basePrice, buying);

		if (event::CalculateTravelPriceEvent::getEventEnabled()) {
			auto& luaManager = mwse::lua::LuaManager::getInstance();
			const auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::table result = stateHandle.triggerEvent(new event::CalculateTravelPriceEvent(mobile, basePrice, price, OnCalculateTravelPrice_DestinationList->current->data, &OnCalculateTravelPrice_TravelCompanionList));
			if (result.valid()) {
				price = result["price"];
			}
		}

		return price;
	}

	// Send off our travel price event when actually selecting a destination.
	int __fastcall OnCalculateTravelPriceForDestination(TES3::MobileActor* mobile, DWORD _UNUSED_, int basePrice, bool buying) {
		int price = mobile->determineModifiedPrice(basePrice, buying);

		// Destination is off by one, so we need the previous entry.
		auto destination = OnCalculateTravelPrice_DestinationList->tail->data;
		if (OnCalculateTravelPrice_DestinationList->current != nullptr) {
			destination = OnCalculateTravelPrice_DestinationList->current->previous->data;
		}

		if (event::CalculateTravelPriceEvent::getEventEnabled()) {
			auto& luaManager = mwse::lua::LuaManager::getInstance();
			const auto stateHandle = luaManager.getThreadSafeStateHandle();
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
	static TES3::SkillStatistic* OnSkillTrained_Skill;
	static int OnSkillTrained_SkillId = -1;

	void __fastcall OnSkillTrained_GetSkill(TES3::SkillStatistic* skill, DWORD _UNDEFINED_, float delta, bool capAt0, bool capAt100) {
		skill->modSkillCapped(delta, capAt0, capAt100);

		OnSkillTrained_SkillId = -1;
		auto macp = TES3::WorldController::get()->getMobilePlayer();
		for (int i = TES3::SkillID::FirstSkill; i <= TES3::SkillID::LastSkill; ++i) {
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

	std::optional<std::string> getLowerPath(const std::filesystem::directory_entry& path) {
		try {
			auto lowerPath = path.path().string();
			string::to_lower(lowerPath);

			return lowerPath;
		}
		catch (std::exception&) {
			return {};
		}
	}

	void LuaManager::gatherModMetadata() {
		sol::table luaMWSE = luaState["mwse"];
		sol::table activeLuaMods = luaMWSE["activeLuaMods"];

		// Try to match any mwse information from -metadata.toml files to an active lua mod.
		for (const auto& p : std::filesystem::directory_iterator("Data Files", std::filesystem::directory_options::follow_directory_symlink)) {
			auto maybeLowerPath = getLowerPath(p);
			if (!maybeLowerPath) {
				continue;
			}
			const auto& lowerPath = maybeLowerPath.value();

			// We only care about *-metadata.toml files.
			if (!string::ends_with(lowerPath, "-metadata.toml")) {
				continue;
			}

			// Load the metadata.
			const sol::protected_function tomlLoadFile = luaState["toml"]["loadFile"];
			auto metadata_result = tomlLoadFile(lowerPath);
			if (metadata_result.valid()) {
				sol::optional<sol::table> metadata = metadata_result;
				if (!metadata) {
					continue;
				}

				sol::optional<sol::table> metadata_tools = metadata.value()["tools"];
				if (!metadata_tools) {
					continue;
				}

				sol::optional<sol::table> metadata_tools_mwse = metadata_tools.value()["mwse"];
				if (!metadata_tools_mwse) {
					continue;
				}

				sol::optional<std::string> luaKey = metadata_tools_mwse.value()["lua-mod"];
				if (!luaKey) {
					continue;
				}

				// Ensure that keys are lowercased for lookup.
				string::to_lower(luaKey.value());

				sol::optional<sol::table> runtime = activeLuaMods[luaKey.value()];
				if (!runtime) {
					continue;
				}

				// Make sure we don't already have a metadata assigned.
				if (runtime.value()["metadata"] != sol::nil) {
					log::getLog() << "[LuaManager] WARNING: More than one metadata found claiming mod '" << luaKey.value() << "'." << std::endl;
					continue;
				}

				runtime.value()["metadata"] = metadata;
			}
			else {
				sol::error error = metadata_result;
				log::getLog() << "[LuaManager] ERROR: Could not parse mod metadata '" << p.path().string() << "':" << std::endl << error.what() << std::endl;
				continue;
			}
		}
	}

	const std::array<std::string, 2> disabledMarkers = { ".disabled", ".mohidden" };

	bool isPathDisabled(const std::string_view& path) {
		const auto disabledPathItt = std::find_if(disabledMarkers.begin(), disabledMarkers.end(),
			[&](const std::string& s) {
				return path.find(s) != std::string::npos;
			});
		return disabledPathItt != disabledMarkers.end();
	}

	void LuaManager::gatherMainModScripts(const std::string_view& path, bool core, const std::string_view& scriptFilename) {
		if (!std::filesystem::exists(path)) {
			return;
		}

		// Do some precomputing for storing and calculating active lua mods.
		sol::table luaMWSE = luaState["mwse"];
		sol::table activeLuaMods = luaMWSE["activeLuaMods"];
		bool isLegacy = !string::equal(scriptFilename, "main.lua");

		auto subclassOrder = 0u;
		for (auto it = std::filesystem::recursive_directory_iterator(path, std::filesystem::directory_options::follow_directory_symlink);
			 it != std::filesystem::recursive_directory_iterator();
			 ++it) {

			try {
				auto filename = it->path().filename();
				
				if (filename == ".git") {
					// Skip .git subdirectories, as scanning their contents are likely to take the path length over the path limit and crash.
					it.disable_recursion_pending();
					continue;
				}

				if (filename == scriptFilename) {
					// If a parent directory is marked .disabled, ignore files in it.
					const auto pathString = it->path().string();
					if (isPathDisabled(pathString)) {
						log::getLog() << "[LuaManager] Skipping mod initializer in disabled directory: " << pathString << std::endl;
						continue;
					}

					// Get a version of its path as a key.
					auto luaModKey = pathString.substr(path.length() + 1, pathString.length() - path.length() - scriptFilename.length() - 2);
					std::replace(luaModKey.begin(), luaModKey.end(), '\\', '.');
					string::to_lower(luaModKey);

					// Check for key conflicts.
					if (activeLuaMods[luaModKey] != sol::nil) {
						log::getLog() << "[LuaManager] Skipping mod with duplicate key '" << luaModKey << "' in directory: " << pathString << std::endl;
						continue;
					}

					// Prepare runtime data.
					auto runtime = luaState.create_table();
					runtime["path"] = pathString;
					runtime["parent_path"] = it->path().parent_path().string();
					runtime["key"] = luaModKey;
					runtime["core_mod"] = core;
					runtime["legacy_mod"] = isLegacy;
					runtime["load_std_order"] = subclassOrder++;

					activeLuaMods[luaModKey] = runtime;
				}
			}
			catch (std::exception&) {
				// TODO: Figure out how to handle paths that contain unicode files without causing exceptions.
			}
		}
	}

	void LuaManager::executeMainModScripts() {
		sol::protected_function_result result = luaState.safe_script_file("Data Files\\MWSE\\core\\startLuaMods.lua");
		if (!result.valid()) {
			sol::error error = result;
			log::getLog() << "[LuaManager] ERROR: Failed to start up lua mods!" << std::endl << error.what() << std::endl;
		}
	}

	//
	// Event: UI Refreshed
	//

	const auto TES3_UpdateStatsScrollPane = reinterpret_cast<bool(__cdecl*)(TES3::UI::Element*)>(0x649870);

	bool __cdecl OnRefreshedStatsPane(TES3::UI::Element* element) {
		bool result = TES3_UpdateStatsScrollPane(element);

		if (event::UiRefreshedEvent::getEventEnabled()) {
			LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::UiRefreshedEvent(element));
		}

		return result;
	}

	const auto TES3_ui_MenuMap_updateLocalPlayer = reinterpret_cast<bool(__cdecl*)(TES3::UI::Element*, int, int, int, TES3::UI::Element*)>(0x5EB760);
	void __cdecl OnRefreshedLocalMap(TES3::UI::Element* menu, int eventId, int data0, int data1, TES3::UI::Element* source) {
		TES3_ui_MenuMap_updateLocalPlayer(menu, eventId, data0, data1, source);

		if (event::UiRefreshedEvent::getEventEnabled()) {
			const auto& TES3_UI_ID_MenuMap_local = *reinterpret_cast<TES3::UI::UI_ID*>(0x7D4640);
			auto localMap = menu->findChild(TES3_UI_ID_MenuMap_local);

			if (localMap) {
				LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::UiRefreshedEvent(localMap));
			}
		}
	}

	//
	// Event: Filter Inventory/Barter Tile
	//

	const auto TES3_FilterInventoryTile = reinterpret_cast<bool(__cdecl*)(TES3::UI::InventoryTile*, TES3::Item*)>(0x5CC720);

	bool __cdecl OnFilterInventoryTile(TES3::UI::InventoryTile* tile, TES3::Item* item) {
		if (event::FilterInventoryEvent::getEventEnabled()) {
			const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
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

	const auto TES3_FilterBarterTile = reinterpret_cast<bool(__cdecl*)(TES3::UI::InventoryTile*, TES3::Item*)>(0x5A5430);

	bool __cdecl OnFilterBarterTile(TES3::UI::InventoryTile* tile, TES3::Item* item) {
		if (event::FilterBarterMenuEvent::getEventEnabled()) {
			const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
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

	void __fastcall OnFilterContentsTile(TES3::IteratedList<TES3::UI::InventoryTile*>* list, DWORD _UNUSUED_, TES3::UI::InventoryTile* tile) {
		if (event::FilterContentsMenuEvent::getEventEnabled()) {
			const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
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

	void __fastcall OnFilterContentsTileForTakeAll(TES3::IteratedList<TES3::UI::InventoryTile*>* list, DWORD _UNUSUED_, TES3::UI::InventoryTile* tile) {
		if (event::FilterContentsMenuEvent::getEventEnabled()) {
			const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
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

	bool __fastcall OnFilterInventorySelect(TES3::UI::Element* element, TES3::UI::EventCallback callback) {
		TES3::Item* item = reinterpret_cast<TES3::Item*>(element->getProperty(TES3::UI::PropertyType::Pointer, *reinterpret_cast<TES3::UI::Property*>(0x7D3C88)).ptrValue);
		TES3::ItemData* itemData = reinterpret_cast<TES3::ItemData*>(element->getProperty(TES3::UI::PropertyType::Pointer, *reinterpret_cast<TES3::UI::Property*>(0x7D3C16)).ptrValue);

		if (event::FilterInventorySelectEvent::getEventEnabled()) {
			const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
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

	static TES3::DialogueInfo* lastReadDialogueInfo = nullptr;

	TES3::GameFile* __fastcall PatchGetDialogueInfoText_GetSourceMod(TES3::DialogueInfo* dialogueInfo) {
		lastReadDialogueInfo = dialogueInfo;
		return dialogueInfo->sourceMod;
	}

	bool __fastcall PatchGetDialogueInfoText_ReadFromFile(TES3::GameFile* gameFile, DWORD _UNUSUED_, char* dialogueTextBuffer, size_t size) {
		// Allow the event to override the text.
		if (mwse::lua::event::InfoGetTextEvent::getEventEnabled()) {
			const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			sol::object eventResult = stateHandle.triggerEvent(new mwse::lua::event::InfoGetTextEvent(lastReadDialogueInfo));
			if (eventResult.valid()) {
				sol::table eventData = eventResult;
				sol::object text = eventData["text"];
				if (text.is<const char*>()) {
					TES3::DialogueInfo::setLastLoadedText(text.as<const char*>());
					return true;
				}
			}
		}

		return gameFile->readChunkData(dialogueTextBuffer, size);
	}

	//
	// Event: Dialogue Info response script
	//

	void __fastcall OnSetDialogueInfoHasCommand(TES3::DialogueInfo* info, TES3::GameFile* file) {
		// Clear the flag first.
		info->objectFlags &= ~TES3::ObjectFlag::HasResultText;

		// Read the command from the file.
		auto commandLength = file->currentChunkHeader.size;
		auto command = new char[commandLength + 1];
		file->readChunkData(command);
		command[commandLength] = '\0';

		// See if we have any non-comments/non-whitespace.
		bool hasNonCommentContent = false;
		bool inComment = false;
		for (auto i = 0u; i <= commandLength; ++i) {
			auto c = command[i];
			switch (c) {
			case '\t':
			case ' ':
				break;
			case '\r':
			case '\n':
				inComment = false;
				break;
			case ';':
				inComment = true;
				break;
			default:
				hasNonCommentContent = !inComment;
			}

			// Allow lua commands.
			if (inComment && strncmp(&command[i], ";lua ", sizeof(";lua ") - 1) == 0 && (i == 0 || command[i - 1] == '\n')) {
				info->objectFlags |= TES3::ObjectFlag::HasResultText;
				break;
			}

			// If we had non-comment content, we're done. Set the flag.
			if (hasNonCommentContent) {
				info->objectFlags |= TES3::ObjectFlag::HasResultText;
				break;
			}
		}

		delete[] command;
	}

	__declspec(naked) void PatchOnSetDialogueInfoHasCommand() {
		__asm {
			mov ecx, edi // Size: 0x2
			mov edx, esi // Size: 0x2
		}
	}
	const size_t PatchOnSetDialogueInfoHasCommand_Size = 0x4;

	void __fastcall OnRunDialogueCommand(TES3::Script* script, DWORD _UNUSUED_, TES3::ScriptCompiler* compiler, const char* command, int source, TES3::Reference* reference, TES3::ScriptVariables* variables, TES3::DialogueInfo* info, TES3::Dialogue* dialogue) {
		// Allow the event to override the text.
		if (mwse::lua::event::InfoResponseEvent::getEventEnabled()) {
			const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			sol::object eventResult = stateHandle.triggerEvent(new mwse::lua::event::InfoResponseEvent(command, reference, variables, dialogue, info));
			if (eventResult.valid()) {
				sol::table eventData = eventResult;
				if (eventData.get_or("block", false)) {
					return;
				}
			}
		}

		// Do we have any lua commands?
		bool noMorrowindScript = false;
		std::string_view commandView = command;
		if (commandView.find(";lua ", 0) != std::string_view::npos) {
			auto handle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::environment env(handle.getState(), sol::create, handle.getState().globals());

			env["reference"] = reference;
			env["dialogue"] = dialogue;
			env["info"] = info;

			if (mwse::lua::event::DialogueEnvironmentCreatedEvent::getEventEnabled()) {
				handle.triggerEvent(new mwse::lua::event::DialogueEnvironmentCreatedEvent(env));
			}

			// Allow running lua in comments.
			for (auto posStart = commandView.find(";lua "); posStart != std::string_view::npos; posStart = commandView.find(";lua ", posStart)) {
				// If we aren't at the start of the line, skip.
				if (posStart != 0 && commandView[posStart - 1] != '\n') {
					posStart = commandView.find("\r\n");
					continue;
				}

				// Jump up above the lua prefix.
				posStart += sizeof(";lua ") - 1;

				// Find where our command ends.
				auto posEnd = commandView.find("\r\n", posStart);
				if (posEnd == std::string_view::npos) {
					posEnd = commandView.length();
				}

				// Bail if no actual command text was provided.
				if (posStart == posEnd) {
					continue;
				}

				// Execute the command.
				auto luaCommand = commandView.substr(posStart, posEnd - posStart);
				sol::protected_function_result result = handle.getState().safe_script(luaCommand, env, &sol::script_pass_on_error);
				if (!result.valid()) {
					sol::error error = result;
					std::stringstream ss;
					ss << "Failed to run mod dialogue lua script:" << std::endl << error.what();

					auto message = ss.str();
					log::getLog() << "[LuaManager] ERROR: " << message << std::endl;

					mwse::tes3::logAndShowError(message.c_str());

					posStart = posEnd;
					continue;
				}

				posStart = posEnd;
			}

			// Check for if we will block mwscript.
			noMorrowindScript = env.get_or("noMorrowindScript", noMorrowindScript);
		}

		if (!noMorrowindScript) {
			script->doCommand(compiler, command, source, reference, variables, info, dialogue);
		}

		if (mwse::lua::event::PostInfoResponseEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::PostInfoResponseEvent(command, reference, variables, dialogue, info));
		}
	}

	//
	// Event: Dialogue link resolve topic.
	//

	TES3::Dialogue* __fastcall OnInfoLinkResolve(TES3::NonDynamicData* _this, DWORD _UNUSUED_, const char* topic) {
		if (mwse::lua::event::InfoLinkResolveEvent::getEventEnabled()) {
			const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
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

	void __fastcall OnItemDropped(TES3::DataHandler* dataHandler, DWORD _UNUSED_, TES3::Reference* reference) {
		// Call overwritten code.
		dataHandler->updateLightingForReference(reference);

		if (event::ItemDroppedEvent::getEventEnabled()) {
			LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::ItemDroppedEvent(reference));
		}

		// Work-around for items being dropped not triggering activation. This should probably be consolidated elsewhere at some point.
		if (event::ReferenceActivatedEvent::getEventEnabled()) {
			auto cell = reference->getCell();
			if (cell && cell->getCellActive()) {
				reference->setReferenceActive();
			}
		}
	}

	static TES3::Reference* OnItemDroppedExterior_LastCreatedReference = nullptr;

	void __fastcall OnItemDropped_ReferenceCreated(TES3::Reference* reference) {
		// Call overwritten code.
		reference->ctor();

		// Store the last created reference for later use.
		OnItemDroppedExterior_LastCreatedReference = reference;
	}

	const auto TES3_DataHandler_updateAllLights = reinterpret_cast<void(__thiscall*)(TES3::DataHandler*)>(0x485C50);
	void __fastcall OnItemDropped_UpdateExteriors(TES3::DataHandler* dataHandler) {
		// Call overwritten code.
		dataHandler->updateLightingForExteriorCells();

		if (OnItemDroppedExterior_LastCreatedReference) {
			if (event::ItemDroppedEvent::getEventEnabled()) {
				LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::ItemDroppedEvent(OnItemDroppedExterior_LastCreatedReference));
			}

			// Work-around for items being dropped not triggering activation. This should probably be consolidated elsewhere at some point.
			if (event::ReferenceActivatedEvent::getEventEnabled()) {
				auto cell = OnItemDroppedExterior_LastCreatedReference->getCell();
				if (cell && cell->getCellActive()) {
					OnItemDroppedExterior_LastCreatedReference->setReferenceActive();
				}
			}
		}
	}

	//
	// Event: Calculate hit chance.
	//

	const auto TES3_CombatHitRoll = reinterpret_cast<bool(__cdecl*)(TES3::MobileActor*, TES3::MobileProjectile*, unsigned int*)>(0x555290);
	bool __cdecl OnCalculateHitChanceWrapper(TES3::MobileActor* attacker, TES3::MobileProjectile* projectile, unsigned int* out_serialCreated) {
		event::CalcHitChanceEvent::m_Attacker = attacker;
		event::CalcHitChanceEvent::m_Projectile = projectile;
		return TES3_CombatHitRoll(attacker, projectile, out_serialCreated);
	}

	int __fastcall OnCalculateHitChance(int initialHitChance) {
		auto hitChance = initialHitChance;

		// Allow the event to override the text.
		if (mwse::lua::event::CalcHitChanceEvent::getEventEnabled()) {
			const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			sol::object eventResult = stateHandle.triggerEvent(new mwse::lua::event::CalcHitChanceEvent(hitChance));
			if (eventResult.valid()) {
				sol::table eventData = eventResult;
				hitChance = getOptionalParam<int>(eventData, "hitChance", initialHitChance);
			}
		}

		// Overwritten code for this hook.
		if (TES3::WorldController::get()->menuController->getShowCombatStats()) {
			char* buffer = mwse::tes3::getThreadSafeStringBuffer();
			const auto attacker = event::CalcHitChanceEvent::m_Attacker;
			const auto attackerId = attacker->reference->baseObject->getObjectID();
			const auto targetId = attacker->actionData.hitTarget->reference->baseObject->getObjectID();
			sprintf(buffer, "Attack Chance %d%%, for %s to hit %s", hitChance, attackerId, targetId);
			TES3::UI::logToConsole(buffer, false);
		}

		return hitChance;
	}

	__declspec(naked) void patchCalculateHitChance() {
		__asm {
			mov ecx, [ebp + 0x8]	// Size: 0x3
			nop // Replaced with a call generation. Can't do so here, because offsets aren't accurate.
			nop // ^
			nop // ^
			nop // ^
			nop // ^
			mov [ebp + 0x8], eax	// Size: 0x3
		}
	}
	const size_t patchCalculateHitChance_size = 0xB;

	//
	// Event: Calculate block chance.
	//

	int __fastcall OnCalcBlockChance(TES3::MobileActor* attacker, int blockChance) {
		// Allow the event to override the text.
		if (mwse::lua::event::CalcBlockChanceEvent::getEventEnabled()) {
			const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			sol::object eventResult = stateHandle.triggerEvent(new mwse::lua::event::CalcBlockChanceEvent(attacker, blockChance));
			if (eventResult.valid()) {
				sol::table eventData = eventResult;
				blockChance = getOptionalParam<int>(eventData, "blockChance", blockChance);
			}
		}

		// Overwritten code for this hook.
		if (TES3::WorldController::get()->menuController->getShowCombatStats()) {
			char* buffer = mwse::tes3::getThreadSafeStringBuffer();
			sprintf(buffer, "Block Chance %d%%, for %s to hit %s", blockChance, attacker->reference->baseObject->getObjectID(), attacker->actionData.hitTarget->reference->baseObject->getObjectID());
			const auto TES3_ConsoleLogResult = reinterpret_cast<void(__cdecl*)(const char*, bool)>(0x5B2C20);
			TES3_ConsoleLogResult(buffer, false);
		}

		return blockChance;
	}

	__declspec(naked) void patchCalcBlockChance() {
		__asm {
			mov ecx, edi // Size: 0x2
			mov edx, [ebp + 0x8] // Size: 0x3
			nop // Replaced with a call generation. Can't do so here, because offsets aren't accurate.
			nop // ^
			nop // ^
			nop // ^
			nop // ^
			mov [ebp + 0x8], eax // Size: 0x3
		}
	}
	const size_t patchCalcBlockChance_size = 0xD;

	//
	// Event: Jump.
	//

	void __fastcall OnJump(TES3::MobileActor* mobile, TES3::Vector3* velocity) {
		mobile->doJump(*velocity, true, true);
	}

	__declspec(naked) void patchJump() {
		__asm {
			mov ecx, [esi + 0x38]	// Size: 0x3
			lea edx, [esp + 0x34 - 0x24]	// Size: 0x4
			nop // Replaced with a call generation. Can't do so here, because offsets aren't accurate.
			nop // ^
			nop // ^
			nop // ^
			nop // ^
		}
	}
	const size_t patchJump_size = 0xC;

	//
	// Event: Filters created.
	//

	const auto TES3_WorldController_CreateFilters = reinterpret_cast<void(__thiscall*)(TES3::WorldController*)>(0x411400);
	void __fastcall CreateFilters(TES3::WorldController* worldController) {
		TES3_WorldController_CreateFilters(worldController);

		mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::GenericEvent("fadersCreated"));
	}

	//
	// Events related to soul gems.
	//

	bool __fastcall PatchWriteEnchantSoulData(TES3::ItemStack* stack) {
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

	bool __fastcall PatchCheckSoulTrapSoulGem(TES3::Misc* item, TES3::MobileActor* mact) {
		if (!tes3::isSoulGem(item)) {
			return false;
		}

		// Allow lua to dictate what souls are allowed in which soulgems.
		if (lua::event::FilterSoulGemTargetEvent::getEventEnabled()) {
			auto& luaManager = mwse::lua::LuaManager::getInstance();
			const auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::table payload = stateHandle.triggerEvent(new lua::event::FilterSoulGemTargetEvent(item, mact));
			if (payload.valid()) {
				// Allow event blocking.
				if (payload.get_or("block", false)) {
					return false;
				}

				// Allow filter setting.
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

	int __stdcall PatchGetSoulValueForActor(TES3::Actor* actor) {
		if (actor->objectType != TES3::ObjectType::Creature) {
			return 0;
		}

		return static_cast<TES3::Creature*>(actor)->getSoulValue();
	}

	int __fastcall PatchGetSoulValueForMobileActor(TES3::MobileActor* mact) {
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

	int PatchSPrintFSoulValue(char* buffer, const char* format, TES3::Actor* actor) {
		return sprintf(buffer, format, PatchGetSoulValueForActor(actor));
	}

	void __fastcall PatchSetSoulValueProperty(TES3::UI::Element* element, DWORD _UNUSED_, TES3::UI::Property property, TES3::Actor* actor, TES3::UI::PropertyType type) {
		TES3::UI::PropertyValue value;
		value.integerValue = PatchGetSoulValueForActor(actor);
		element->setProperty(property, value, type);
	}

	static TES3::Creature* PatchGetAliasedSoulValueProperty_AliasedCreature = new TES3::Creature();
	static bool PatchGetAliasedSoulValueProperty_UseAliasedCreature = false;

	TES3::UI::PropertyValue* __fastcall PatchGetAliasedSoulValueProperty_GetRepairTool(const TES3::UI::Element* self, DWORD _UNUSED_, TES3::UI::PropertyValue* propValue, TES3::UI::Property prop, TES3::UI::PropertyType propType, const TES3::UI::Element* element, bool checkInherited) {
		// Get the original value.
		auto value = self->getProperty(propValue, prop, propType, element, checkInherited);

		// Determine if we need a patch or not.
		auto repairTool = reinterpret_cast<TES3::Item*>(value->ptrValue);
		PatchGetAliasedSoulValueProperty_UseAliasedCreature = repairTool && repairTool->objectType == TES3::ObjectType::Misc;

		return value;
	}

	TES3::UI::PropertyValue* __fastcall PatchGetAliasedSoulValueProperty_GetRepairItemData(const TES3::UI::Element* self, DWORD _UNUSED_, TES3::UI::PropertyValue* propValue, TES3::UI::Property prop, TES3::UI::PropertyType propType, const TES3::UI::Element* element, bool checkInherited) {
		// Do we need to set our aliased soul?
		auto value = self->getProperty(propValue, prop, propType, element, checkInherited);
		if (!PatchGetAliasedSoulValueProperty_UseAliasedCreature) {
			return value;
		}

		// Change the soul to the aliased creature. The item will be destroyed (or the value unset) soon,
		// but we must make sure that future events don't require the original value.
		auto itemData = reinterpret_cast<TES3::ItemData*>(value->ptrValue);
		auto actor = itemData->getSoul();
		if (actor->objectType == TES3::ObjectType::Creature) {
			PatchGetAliasedSoulValueProperty_AliasedCreature->soul = static_cast<TES3::Creature*>(actor)->getSoulValue();
			itemData->soul = PatchGetAliasedSoulValueProperty_AliasedCreature;
		}
		else if (actor->objectType == TES3::ObjectType::NPC) {
			PatchGetAliasedSoulValueProperty_AliasedCreature->soul = static_cast<TES3::NPC*>(actor)->getSoulValue().value_or(0);
			itemData->soul = PatchGetAliasedSoulValueProperty_AliasedCreature;
		}

		return value;
	}

	__declspec(naked) void PatchGetSoulValueForTooltip_LoadObject() {
		__asm {
			mov edx, esi // Size: 0x2
			nop          // Size: 0x1
		}
	}
	constexpr size_t PatchGetSoulValueForTooltip_LoadObject_Size = 0x3;

	int __fastcall PatchGetSoulValueForTooltip(TES3::ItemData* itemData, TES3::Misc* soulGem) {
		auto actor = itemData->getSoul();

		int baseValue = 0;
		if (actor->objectType == TES3::ObjectType::Creature) {
			auto creature = static_cast<TES3::Creature*>(actor);
			baseValue = creature->getSoulValue();
		}
		else if (actor->objectType == TES3::ObjectType::NPC) {
			auto npc = static_cast<TES3::NPC*>(actor);
			baseValue = npc->getSoulValue().value_or(0);
		}

		if (baseValue <= 0) {
			return 0;
		}

		// MCP value change. Uses floating point to avoid risk of overflow.
		double v = baseValue;
		return int(0.0001 * (v * v * v) + 2 * v);
	}

	__declspec(naked) void PatchGetSoulValueForTooltip_NoMCPLoader() {
		__asm {
			mov ecx, [esp + 0x30 - 0x20] // Size: 0x4
			mov edx, esi                 // Size: 0x2
		}
	}
	constexpr size_t PatchGetSoulValueForTooltip_NoMCPLoader_Size = 0x6;

	int __fastcall PatchGetSoulValueForTooltip_NoMCP(TES3::ItemData* itemData, TES3::Misc* soulGem) {
		auto actor = itemData->getSoul();

		int baseValue = 0;
		if (actor->objectType == TES3::ObjectType::Creature) {
			auto creature = static_cast<TES3::Creature*>(actor);
			baseValue = creature->getSoulValue();
		}
		else if (actor->objectType == TES3::ObjectType::NPC) {
			auto npc = static_cast<TES3::NPC*>(actor);
			baseValue = npc->getSoulValue().value_or(0);
		}

		if (baseValue <= 0) {
			return 0;
		}

		return soulGem->getValue() * baseValue;
	}

	__declspec(naked) void PatchGetSoulValueForRechargeTitle_Setup() {
		__asm {
			mov ecx, ebp // Size: 0x2
		}
	}

	constexpr size_t PatchGetSoulValueForRechargeTitle_Setup_Size = 0x2;

	int __fastcall PatchGetSoulValueForRechargeTitle(TES3::UI::InventoryTile* tile) {
		auto actor = tile->itemData->getSoul();
		if (actor->objectType == TES3::ObjectType::Creature) {
			auto creature = static_cast<TES3::Creature*>(actor);
			return creature->getSoulValue();
		}
		else if (actor->objectType == TES3::ObjectType::NPC) {
			auto npc = static_cast<TES3::NPC*>(actor);
			return npc->getSoulValue().value_or(0);
		}
		return 0;
	}

	const auto& TES3_UI_ID_MenuEnchantment_SoulGem = *reinterpret_cast<TES3::UI::UI_ID*>(0x7D35A6);
	const auto& TES3_UI_ID_MenuEnchantment_Item = *reinterpret_cast<TES3::UI::Property*>(0x7D358C);
	const auto& TES3_UI_ID_MenuEnchantment_soulcharge = *reinterpret_cast<TES3::UI::Property*>(0x7D36C4);
	const auto& TES3_UI_ID_MenuEnchantment_Effect = *reinterpret_cast<TES3::UI::Property*>(0x7D36BA);

	void __fastcall PatchSetCorrectSoulValueForEnchanting(TES3::UI::Element* self, DWORD _UNUSED_, char* text) {
		// Get a hook to our soul gem so we can figure out its item data.
		auto soulGemElement = self->getTopLevelParent()->findChild(TES3_UI_ID_MenuEnchantment_SoulGem);
		auto itemData = reinterpret_cast<TES3::ItemData*>(soulGemElement->getProperty(TES3::UI::PropertyType::Pointer, TES3_UI_ID_MenuEnchantment_Item).ptrValue);

		// Calculate the new soul value.
		auto soul = itemData->getSoul();
		int soulValue = soul ? soul->getSoulValue().value_or(0) : 0;

		// Update the stored soul value on the GUI elements.
		self->setProperty(TES3_UI_ID_MenuEnchantment_soulcharge, soulValue);
		self->setProperty(TES3_UI_ID_MenuEnchantment_Effect, soulValue);

		// Override replaced code, with the new value. The text buffer here is the thread safe game string buffer and can fit all values.
		sprintf(text, "%d", soulValue);
		self->setText(text);
	}

	//
	// Fire an event when item tiles are updated.
	//

	TES3::IteratedList<TES3::UI::InventoryTile*>::Node* __fastcall GetNextInventoryTileToUpdate(TES3::IteratedList<TES3::UI::InventoryTile*>* iterator) {
		if (lua::event::ItemTileUpdatedEvent::getEventEnabled()) {
			lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new lua::event::ItemTileUpdatedEvent(iterator->current->data));
		}

		return iterator->cached_next();
	}

	void __inline TriggerItemTileUpdatedEventForElement(TES3::UI::Element* element, DWORD propertyAddress) {
		if (lua::event::ItemTileUpdatedEvent::getEventEnabled()) {
			TES3::UI::InventoryTile* tile = static_cast<TES3::UI::InventoryTile*>(element->getProperty(TES3::UI::PropertyType::Pointer, *reinterpret_cast<TES3::UI::Property*>(propertyAddress)).ptrValue);
			if (tile) {
				lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new lua::event::ItemTileUpdatedEvent(tile));
			}
		}
	}

	void __fastcall OnSetItemTileIcon(TES3::UI::Element* element, DWORD _UNUSED_, const char* iconPath) {
		// Overwritten function.
		element->setIcon(iconPath);

		// Get the associated tile and fire off the update event.
		TriggerItemTileUpdatedEventForElement(element, 0x7D3A70);
	}

	void __fastcall OnMenuContentsInventoryTileChildPropertySet(TES3::UI::Element* element, DWORD _UNUSED_, TES3::UI::Property property, TES3::UI::PropertyValue value, TES3::UI::PropertyType type) {
		// Overwritten code.
		element->setProperty(property, value, type);

		// Get the associated tile and fire off the update event.
		TriggerItemTileUpdatedEventForElement(element, 0x7D308C);
	}

	void __fastcall OnMenuBarterInventoryTileChildPropertySet(TES3::UI::Element* element, DWORD _UNUSED_, TES3::UI::Property property, TES3::UI::PropertyValue value, TES3::UI::PropertyType type) {
		// Overwritten code.
		element->setProperty(property, value, type);

		// Get the associated tile and fire off the update event.
		TriggerItemTileUpdatedEventForElement(element, 0x7D284C);
	}

	const auto TES3_AttachTileToCursor = reinterpret_cast<TES3::UI::Element * (__cdecl*)(TES3::UI::InventoryTile*, int)>(0x5D14F0);
	TES3::UI::Element* __cdecl OnAttachTileToCursor(TES3::UI::InventoryTile* tile, int location) {
		auto element = TES3_AttachTileToCursor(tile, location);

		if (element && lua::event::ItemTileUpdatedEvent::getEventEnabled()) {
			lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new lua::event::ItemTileUpdatedEvent(tile));
		}

		return element;
	}

	//
	// Handle all the hacks needed to get ItemData transferred over to a bigger structure.
	//

	TES3::ItemData* __fastcall OnDeletingItemData(TES3::ItemData* itemData, DWORD _UNUSED_, bool deleting) {
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
	TES3::IteratedList<TES3::ItemStack*>* currentlySavingInventoryIterator = nullptr;
	unsigned int currentlySavingInventoryItemDataIndex = 0;

	// Get a hold of the inventory we're looking at.
	TES3::IteratedList<TES3::ItemStack*>::Node* __fastcall GetFirstSavedItemStack(TES3::IteratedList<TES3::ItemStack*>* iterator) {
		currentlySavingInventoryIterator = iterator;
		return iterator->cached_begin();
	}

	// Get a hold of the current index in ItemData storage we're looking at.
	int __fastcall WriteItemDataIndex(TES3::GameFile* gameFile, DWORD _UNUSED_, unsigned int tag, const unsigned int* data, unsigned int size) {
		currentlySavingInventoryItemDataIndex = *data;
		return gameFile->writeChunkData(tag, data, size);
	}

	sol::protected_function fnTableEmpty;
	sol::protected_function fnEncodeForSave;
	sol::protected_function fnDecodeFromSave;

	// The last extra data written. We'll add the lua data here if needed.
	int __fastcall WriteItemDataCondition(TES3::GameFile* gameFile, DWORD _UNUSED_, unsigned int tag, const void* data, unsigned int size) {
		// Overwritten code.
		int result = gameFile->writeChunkData(tag, data, size);

		TES3::ItemData* itemData = currentlySavingInventoryIterator->current->data->variables->at(currentlySavingInventoryItemDataIndex);
		if (itemData->luaData) {
			// If it is empty, don't bother saving it.
			const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			auto& table = itemData->luaData->data;
			if (!fnTableEmpty(table, true)) {
				// Convert the table to json for storage.
				std::string json = fnEncodeForSave(table);

				// Call original writechunk function.
				gameFile->writeChunkData('TAUL', json.c_str(), json.length() + 1);
			}
		}
		if (itemData->flags) {
			// Save added flags.
			gameFile->writeChunkData('GLFE', &itemData->flags, sizeof(TES3::ItemData::flags));
		}

		return result;
	}

	// Get a handle on the last created ItemData when loading.
	std::unordered_map<DWORD, TES3::ItemData*> saveLoadItemDataMap;
	TES3::ItemData* __fastcall CreateItemDataFromLoading(TES3::ItemData* itemData) {
		saveLoadItemDataMap[GetCurrentThreadId()] = itemData;
		return TES3::ItemData::ctor(itemData);
	}

	// When we load a next record while loading an actor, check to see if it's a lua extension and save it to the last loaded ItemData.
	int __fastcall LoadNextRecordForActor(TES3::GameFile* gameFile) {
		int result = gameFile->getNextSubrecord();

		// Load the lua data into the last loaded ItemData.
		if (result == 'TAUL') {
			char* buffer = new char[gameFile->currentChunkHeader.size];
			bool success = gameFile->readChunkData(buffer, gameFile->currentChunkHeader.size);

			// If we for whatever reason failed to load this chunk, bail.
			if (success) {
				// Get our lua table, and replace it with our new table.
				const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				auto& state = stateHandle.getState();
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
		else if (result == 'GLFE') {
			auto threadID = GetCurrentThreadId();
			auto saveLoadItemData = saveLoadItemDataMap[threadID];
			gameFile->readChunkData(&saveLoadItemData->flags, sizeof(TES3::ItemData::flags));
		}

		// It's safe to return LUAT here, it will just pass to the next load for us.
		return result;
	}

	// Get ItemData for reference being saved.
	TES3::ItemData* __fastcall GetItemDataForReferenceSaving(TES3::Reference* reference) {
		auto itemData = reference->getAttachedItemData();
		saveLoadItemDataMap[GetCurrentThreadId()] = itemData;
		return itemData;
	}

	// The last extra data written for references. We'll add the lua data here if needed.
	int __fastcall WriteReferenceItemDataCondition(TES3::GameFile* gameFile, DWORD _UNUSED_, unsigned int tag, const void* data, unsigned int size) {
		// Overwritten code.
		int result = gameFile->writeChunkData(tag, data, size);

		auto saveLoadItemData = saveLoadItemDataMap[GetCurrentThreadId()];
		if (saveLoadItemData->luaData) {

			// If it is empty, don't bother saving it.
			const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			auto& state = stateHandle.getState();
			auto& table = saveLoadItemData->luaData->data;
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
	TES3::MobileActor* __fastcall LoadReferenceGetMACT(TES3::Reference* reference) {
		saveLoadReferenceMap[GetCurrentThreadId()] = reference;
		return reference->getAttachedMobileActor();
	}

	// When we load a next record while loading an actor, check to see if it's a lua extension and save it to the last loaded ItemData.
	int __fastcall LoadNextRecordForReference(TES3::GameFile* gameFile) {
		int result = gameFile->getNextSubrecord();

		// Load the lua data into the last loaded ItemData.
		if (result == 'TAUL') {
			char* buffer = new char[gameFile->currentChunkHeader.size];
			bool success = gameFile->readChunkData(buffer, gameFile->currentChunkHeader.size);

			// If we for whatever reason failed to load this chunk, bail.
			if (success) {
				// Get our lua table, and replace it with our new table.
				const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				auto& state = stateHandle.getState();
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
	// Patch: Use added flags on ItemData to track bound items, instead of matching ids against bound item GMSTs.
	//

	__declspec(naked) void PatchCheckIfBoundEquipmentStack() {
		__asm {
			mov ecx, [esi + 4]			// ecx = equipmentStack->itemData
			test ecx, ecx
			jz nonbound
			test [ecx + 0x20], 1		// if itemData->flags & ItemDataFlag_BoundItem
			jz nonbound
			jmp $						// Replace with bound item jump destination
		nonbound:
			jmp $						// Replace with non-bound item jump destination
		}
	}
	const size_t PatchCheckIfBoundEquipmentStack_size = 0x17;

	__declspec(naked) void PatchCheckIfBoundItemTile() {
		__asm {
			mov ecx, [ebp + 4]				// ecx = inventoryTile->itemData
			test ecx, ecx
			jz done
			test [ecx + 0x20], 1			// if itemData->flags & ItemDataFlag_BoundItem
			jz done
			mov byte ptr [ebp + 0x40], 1	// inventoryTile->isBoundItem = true
		done:
		}
	}
	const size_t PatchCheckIfBoundItemTile_size = 0x11;

	//
	// Patch bound item / summon saving and loading.
	//

	// Override the effect id, but only if this is a custom effect with a valid summon or bound item.
	short __stdcall PatchMagicSaveLoad_UpdateId(const TES3::EquipmentStack* const stack, short id) {
		// The original code's switch statement only covers magic effect IDs up to Vampirism (0x85).
		// However, there are more vanilla summon effects with higher effect IDs, as well our custom effects.
		// Corprus and vampirism must be excluded, because they may use integers in the created data field.
		const short overrideSavingMagicEffectsFromID = TES3::EffectID::SummonCenturionSphere;

		if (id >= overrideSavingMagicEffectsFromID && stack->object) {
			if (stack->object->objectType == TES3::ObjectType::Armor
				|| stack->object->objectType == TES3::ObjectType::Clothing
				|| stack->object->objectType == TES3::ObjectType::Weapon) {
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
		__asm {
			// Restore overwritten code. Read the id from the appropriate effect and place it in eax.
			mov ecx, dword ptr [esp + 0x20]
			movsx eax, word ptr [eax + ecx]

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

	TES3::MobileActor* lastProcessedCrimeActor = nullptr;

	const auto TES3_MobileActor_ProcessCrimes = reinterpret_cast<void(__thiscall*)(TES3::MobileActor*)>(0x522040);
	void __fastcall OnProcessCrimes(TES3::MobileActor* mact) {
		lastProcessedCrimeActor = mact;
		TES3_MobileActor_ProcessCrimes(mact);
	}

	void __fastcall OnCrimeWitnessedEnd(TES3::CrimeEvent* crimeEvent) {
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
			const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
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
	// Event: Mobile activated/deactivated
	//

	const auto TES3_MobileObject_EnterLeaveSimulation = reinterpret_cast<void(__thiscall*)(TES3::MobileObject*, bool)>(0x561CB0);
	void __fastcall onMobileObjectEnterLeaveSimulation(TES3::MobileObject* mobileObject, DWORD _EDX_, bool active) {
		// Store previous ActiveInSimulation state.
		bool wasActive = mobileObject->actorFlags & TES3::MobileActorFlag::ActiveInSimulation;

		// Call original function.
		TES3_MobileObject_EnterLeaveSimulation(mobileObject, active);

		bool isPlayer = false;
		if (mobileObject->isActor()) {
			// Check if the current mobile is the player.
			TES3::MobileActor* mobileActor = static_cast<TES3::MobileActor*>(mobileObject);
			isPlayer = mobileActor->actorType == TES3::MobileActorType::Player;

			// Remove projectiles fired by this actor from simulation since they will contain invalid data shortly after.
			if (wasActive && !active) {
				mobileActor->removeFiredProjectiles(false);
			}
		}

		// Fire off mobile activation events for anything but the player if ActiveInSimulation state changed.
		if (!isPlayer) {
			if (mwse::lua::event::MobileObjectActivatedEvent::getEventEnabled() && !wasActive && active) {
				mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::MobileObjectActivatedEvent(mobileObject));
			}
			else if (mwse::lua::event::MobileObjectDeactivatedEvent::getEventEnabled() && wasActive && !active) {
				mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::MobileObjectDeactivatedEvent(mobileObject));
			}
		}
	}

	//
	// Event: Power recharged
	//

	using PowersHashMap = decltype(TES3::MobileActor::powers);
	void __fastcall OnDeletePowerHashMapKVP(PowersHashMap* self, DWORD edx, PowersHashMap::Node* node) {
		if (event::PowerRechargedEvent::getEventEnabled()) {
			auto mobile = reinterpret_cast<TES3::MobileActor*>(DWORD(self) - offsetof(TES3::MobileActor, powers));

			// Ignore calls from cleanup in the mobile dtor.
			if (mobile->getFlagActiveAI()) {
				LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new lua::event::PowerRechargedEvent(node->key, mobile));
			}
		}
	}

	//
	// Event: Spell magicka cost on cast
	//

	double __stdcall onSpellCastMagickaRequired(TES3::MagicSourceInstance* source) {
		// Restore overwritten code.
		unsigned int magickaCost = source->sourceCombo.source.asSpell->magickaCost;

		if (event::SpellMagickaUseEvent::getEventEnabled()) {
			const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();

			sol::object eventResult = stateHandle.triggerEvent(new event::SpellMagickaUseEvent(source));
			if (eventResult.valid()) {
				sol::table eventData = eventResult;
				sol::optional<unsigned int> newCost = eventData["cost"];
				if (newCost) {
					magickaCost = newCost.value();
				}
			}
		}

		return magickaCost;
	}

	__declspec(naked) void patchSpellCastMagickaRequired() {
		__asm {
			add edi, 0x2C0						// edi += offset of MACT.magicka
			push esi							// magicSourceInstance
			call onSpellCastMagickaRequired		// Replace with call generation
			jmp short $ + 6
		}
	}
	const size_t patchSpellCastMagickaRequired_size = 0xE;

	//
	// Event: Enchanted item charge needed to cast
	//

	static TES3::Enchantment* lastExaminedEnchantment = nullptr;

	float __stdcall onEnchantItemChargeRequired(TES3::MobileActor* mobile, TES3::Enchantment* enchant, float charge, TES3::MagicSourceInstance* magicSource) {
		// Restore overwritten code.
		charge = std::max(1.0f, charge);

		// Ignore cast once items, the charge calculated is later ignored.
		if (enchant->castType != TES3::EnchantmentCastType::Once) {
			// Fire off the event.
			if (event::EnchantChargeUseEvent::getEventEnabled()) {
				const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();

				sol::object eventResult = stateHandle.triggerEvent(new event::EnchantChargeUseEvent(enchant, mobile, magicSource, charge));
				if (eventResult.valid()) {
					sol::table eventData = eventResult;
					sol::optional<float> newCharge = eventData["charge"];
					if (newCharge) {
						charge = newCharge.value();
					}
				}
			}
		}

		return charge;
	}

	TES3::Enchantment* __fastcall onEnchantItemChargeRequired_getEnchantment(TES3::Object* object) {
		auto ench = object->getEnchantment();
		lastExaminedEnchantment = ench;
		return ench;
	}

	// _ftol substitute used to replace final operation of inlined calculations.
	int onEnchantItemChargeRequired_ftol() {
		float charge;
		__asm { fstp[charge] }

		auto castType = lastExaminedEnchantment->castType;
		auto macp = TES3::WorldController::get()->getMobilePlayer();
		charge = onEnchantItemChargeRequired(macp, lastExaminedEnchantment, charge, nullptr);

		return int(charge);
	}

	__declspec(naked) void patchEnchantItemChargeRequired_onCast() {
		__asm {
			push esi							// magicSourceInstance
			push ecx
			fstp [esp]							// Charge required
			push [esi + 0xA0]					// Casting enchantment
			push edi							// Casting mobileActor
			call onEnchantItemChargeRequired	// Replace with call generation
			fstp [ebp - 0x18]					// Store updated charge required
		}
	}
	const size_t patchEnchantItemChargeRequired_onCast_size = 0x14;

	//
	// Patch: Getting the correct radius of a light attached to a non-light entity.
	//

	__declspec(naked) void patchGetEntityLightRadius() {
		__asm {
			push ebx
			mov ebx, [esi + 0x28]		// ebx = esi->Reference.baseEntity

			mov eax, [eax]				// eax = eax->RefrAttachment_Light.sgLight
			cmp dword ptr [ebx + 4], 0x4847494C		// tag == 'LIGH'
			jnz non_light
			mov ecx, [ebx + 0x64]		// ecx = ebx->EntityLight.radius
			jmp done

		non_light:
			fld dword ptr [eax + 0xC4]	// Load radius from eax->NiLight.specular
			push ecx
			fistp dword ptr [esp]
			pop ecx						// Radius converted to integer

		done:
			mov edx, [esp + 0x10]		// edx = <reference argument>
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
			mov dword ptr [esi + 0x98], 0xFF00FFFF
			nop
			nop
		}
	}
	const size_t patchActorAnimInit_size = 0xC;

	// Defer switching between first and third person during animation override.
	__declspec(naked) bool CheckTogglePOV() {
		__asm {
			xor al, al
			cmp [esi + 0x9B], 0xFF			// esi->PlayerAnimData.patchedOverrideState == 0xFF
			jnz done

			mov al, byte ptr [esi + 0xE9]	// al = esi->PlayerAnimData.togglePOV

		done:
			ret
		}
	}

	// Call our custom function when updating animation playback.
	void __fastcall ActorAnimController_selectMovementAnimAndUpdate(TES3::ActorAnimationController* animController, DWORD _UNUSED_, float deltaTime, bool flag) {
		animController->selectMovementAnimAndUpdate(deltaTime, flag);
	}

	//
	// Patch: Allow changing cast animation speed. Custom speed is read and applied on initial cast.
	//

#pragma warning(push)
#pragma warning(disable: 4102)
	__declspec(naked) bool patchApplyAnimationSpeed() {
		__asm {
			fld [ebp + 0x64]		// ebp->AnimationData.deltaTime

		isMovementAnim:
			cmp cl, 0x2B			// AnimGroup_SwimWalkForward
			jb isWeaponAnim
			cmp cl, 0x7F			// AnimGroup_Jump2w
			ja isWeaponAnim
		movement:
			fmul [ebp + 0x4D8]		// * ebp->AnimationData.movementSpeed
			jmp done

		isWeaponAnim:
			cmp cl, 0x80			// AnimGroup_SpellCast (added test)
			je weapon
			cmp cl, 0x83			// AnimGroup_Attack1 (added creature and h2h attacks)
			jb done
			cmp cl, 0x8F			// AnimGroup_WeaponTwoWide
			ja done
		weapon:
			fmul [ebp + 0x4DC]		// * ebp->AnimationData.weaponSpeed
			nop
			nop
			nop
		done:
		}
	}
#pragma warning(pop)

	const size_t patchApplyAnimationSpeed_size = 0x2D;

	void __fastcall SetAnimSpeedOnCast(TES3::AnimationData* animData) {
		// Ensure non-zero weaponSpeed to bypass the actor controller resetting the value on zero.
		animData->weaponSpeed = animData->getCastSpeed() + FLT_MIN;
	}

	//
	// Patch: calcMoveSpeed event for creatures.
	//
	
	__declspec(naked) void patchCreatureCalcMoveSpeed() {
		__asm {
			mov ecx, esi	// Size: 0x2
			nop				// Replaced with a call generation. Can't do so here, because offsets aren't accurate.
			nop				// ^
			nop				// ^
			nop				// ^
			nop				// ^ Size: 0x5
		}
	}
	const size_t patchCreatureCalcMoveSpeed_size = 0x7;

	//
	// Allow changing the delta time scalar in a safer spot.
	//

	const auto TES3_WorldController_updateDeltaTime = reinterpret_cast<void(__thiscall*)(TES3::WorldController*)>(0x453630);
	void __fastcall OnUpdateDeltaTime(TES3::WorldController* worldController) {
		// Overwritten code.
		TES3_WorldController_updateDeltaTime(worldController);

		// Store the calculated deltaTime for if someone needs to know the real value.
		TES3::WorldController::realDeltaTime = worldController->deltaTime;

		// Multiply our delta time by our scalar.
		worldController->deltaTime *= TES3::WorldController::simulationTimeScalar;
	}

	//
	// Event: dialogueFiltered
	//

	template<TES3::Dialogue::GetFilteredInfoContext context>
	TES3::DialogueInfo* __fastcall PatchDialogueFilteredFor(TES3::Dialogue* self, DWORD _EDX_, TES3::Actor* actor, TES3::Reference* reference, bool flag) {
		return self->getFilteredInfoWithContext(actor, reference, flag, context);
	}

	//
	// Event: Repair
	//

	static TES3::UI::Element* AttemptRepair_LastTarget = nullptr;

	const auto TES3_OnClickRepairOrRecharge = reinterpret_cast<bool(__cdecl*)(TES3::UI::Element*, TES3::UI::Property, int, int, TES3::UI::Element*)>(0x60E260);
	bool __cdecl OnClickRepairOrRecharge(TES3::UI::Element* source, TES3::UI::Property prop, int dataA, int dataB, TES3::UI::Element* target) {
		AttemptRepair_LastTarget = target;
		return TES3_OnClickRepairOrRecharge(source, prop, dataA, dataB, target);
	}

	int __cdecl AttemptRepair() {
		auto repairMenu = TES3::UI::findMenu("MenuRepair");
		if (!repairMenu) {
			return 0;
		}

		// Get the tool data that is used to do the repair.
		auto tool = repairMenu->getPropertyPointer<TES3::RepairTool>("MenuRepair_Object");
		auto toolData = repairMenu->getPropertyPointer<TES3::ItemData>("MenuRepair_extra");

		// Get the item and its data that is being repaired.
		const auto listNumber = AttemptRepair_LastTarget->getProperty(TES3::UI::PropertyType::Integer, TES3::UI::registerProperty("MenuRepair_ListNumber")).integerValue;
		auto clickedStack = AttemptRepair_LastTarget->getPropertyPointer<TES3::ItemStack>("MenuRepair_Object");
		auto item = static_cast<TES3::Item*>(clickedStack->object);
		auto itemData = clickedStack->variables->at(listNumber);

		// Calculate the chance of the repair.
		const auto macp = TES3::WorldController::get()->getMobilePlayer();
		const auto skillTerm = macp->getSkillValue(TES3::SkillID::Armorer);
		const auto luckTerm = macp->getAttributeLuck()->getCurrent() * 0.1f;
		const auto strengthTerm = macp->getAttributeStrength()->getCurrent() * 0.1f;
		const auto fatigueTerm = macp->getFatigueTerm();
		auto chance = fatigueTerm * (skillTerm + strengthTerm + luckTerm);
		const auto roll = tes3::rand() % 100;

		// Calculate the repair amount.
		const auto fRepairAmountMult = TES3::DataHandler::get()->nonDynamicData->GMSTs[TES3::GMST::fRepairAmountMult]->value.asFloat;
		auto repairAmount = int(tool ? tool->getQuality() * fRepairAmountMult * roll : 1.0f);
		if (repairAmount < 1) {
			repairAmount = 1;
		}

		// Allow changing the repair chance/amount.
		if (event::RepairEvent::getEventEnabled()) {
			auto& luaManager = mwse::lua::LuaManager::getInstance();
			const auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::table result = stateHandle.triggerEvent(new event::RepairEvent(macp, item, itemData, tool, toolData, chance, repairAmount));
			if (result.valid()) {
				chance = result.get_or("chance", chance);
				repairAmount = result.get_or("repairAmount", repairAmount);
			}
		}

		// If it was a failure, return -1.
		if (roll >= chance) {
			return -1;
		}

		return std::max(repairAmount, 1);
	}

	// Event: CalculateChargenStats
	const auto TES3_sub_5A9150 = reinterpret_cast<char(__cdecl*)(int)>(0x5A9150);
	void __fastcall CalcChargenStats(int unknown) {
		TES3_sub_5A9150(unknown);
		// Check for chargen in progress to raise event.
		if (TES3::WorldController::get()->isChargenRunning()) {
			if (event::CalcChargenStatsEvent::getEventEnabled()) {
				LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::CalcChargenStatsEvent());
			}
		}
	}

	const auto TES3_ui_showStatReviewMenu = reinterpret_cast<char(__cdecl*)()>(0x62CE60);
	void __fastcall ShowStatReviewMenu() {
		if (TES3::WorldController::get()->isChargenRunning()) {
			if (event::CalcChargenStatsEvent::getEventEnabled()) {
				LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new event::CalcChargenStatsEvent());
			}
		}
		// Raise event first, allowing changes to be made before the menu is shown.
		TES3_ui_showStatReviewMenu();
	}

	//
	// Patch: Error reporting for mesh loading
	//

	static TES3::Object* currentlyLoadingMeshObject = nullptr;

	bool __fastcall PatchObjectLoadMeshMethod(TES3::Object* object) {
		const auto TES3_Object_LoadMesh = reinterpret_cast<bool(__thiscall*)(TES3::Object*)>(0x4F1380);

		currentlyLoadingMeshObject = object;
		bool result = TES3_Object_LoadMesh(object);
		currentlyLoadingMeshObject = nullptr;
		return result;
	}

	bool __fastcall PatchAnimatedObjectLoadMeshMethod(TES3::Object* object) {
		const auto TES3_AnimatedObject_LoadMesh = reinterpret_cast<bool(__thiscall*)(TES3::Object*)>(0x4F0400);

		currentlyLoadingMeshObject = object;
		bool result = TES3_AnimatedObject_LoadMesh(object);
		currentlyLoadingMeshObject = nullptr;
		return result;
	}

	void __cdecl PatchModelLoaderErrorReport(const char* format, const char* errorText, const char* filename) {
		if (currentlyLoadingMeshObject) {
			const char* id = currentlyLoadingMeshObject->getObjectID();
			const char* mod = currentlyLoadingMeshObject->sourceMod ? currentlyLoadingMeshObject->sourceMod->getFilename() : "no source";
			tes3::logAndShowError("Object \"%s\" (%s)\r\n\r\nModel Load Error: %s\r\nin %s.", id, mod, errorText, filename);
		}
		else {
			tes3::logAndShowError(format, errorText, filename);
		}
	}

	void __cdecl PatchStreamLoaderErrorReport(const char* errorText, const char* caption) {
		if (currentlyLoadingMeshObject) {
			const char* id = currentlyLoadingMeshObject->getObjectID();
			const char* mod = currentlyLoadingMeshObject->sourceMod ? currentlyLoadingMeshObject->sourceMod->getFilename() : "no source";
			tes3::logAndShowError("Object \"%s\" (%s)\r\n\r\n%s", id, mod, errorText);
		}
		else {
			tes3::logAndShowError("%s", errorText);
		}
	}

	//
	// Patch: Don't show warnings for empty, deleted dialogues
	//

	void __fastcall LoadOverDialogue(TES3::Dialogue* self, DWORD _EDX_, TES3::Dialogue* from) {
		// Ignore type changes entirely for deleted data.
		if (self->name == nullptr && from->name == nullptr && self->type != from->type && self->getDeleted() && from->getDeleted()) {
			return;
		}

		// Call overwritten code.
		const auto TES3_Dialogue_LoadOver = reinterpret_cast<void(__thiscall*)(TES3::Dialogue*, TES3::Dialogue*)>(0x4B2790);
		TES3_Dialogue_LoadOver(self, from);
	}

	//
	// Patch: Modifiable physical hit detection cone and weapon reach;
	//

	static float attackReach_saved, fCombatAngleXY_saved, fCombatAngleZ_saved;

	// Override of possible hit detection at the start of a swing. Saves data to be used in generic hit cone check later.
	void __cdecl PatchCombatPhysicalHitDetection(TES3::MobileActor* attacker) {
		auto ndd = TES3::DataHandler::get()->nonDynamicData;
		float attackReach = attacker->getAttackReach();
		float fCombatAngleXY = ndd->GMSTs[TES3::GMST::fCombatAngleXY]->value.asFloat;
		float fCombatAngleZ = ndd->GMSTs[TES3::GMST::fCombatAngleZ]->value.asFloat;

		// Fire event.
		if (event::CalcHitDetectionConeEvent::getEventEnabled()) {
			const double radiansToDegrees = 57.29577951308232;
			double degreesXY = radiansToDegrees * std::asin(fCombatAngleXY);
			double degreesZ = radiansToDegrees * std::asin(fCombatAngleZ);

			const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			sol::object response = stateHandle.triggerEvent(new event::CalcHitDetectionConeEvent(attacker, nullptr, attackReach, degreesXY, degreesZ));
			if (response.get_type() == sol::type::table) {
				sol::table eventData = response;
				attackReach = eventData["reach"];
				degreesXY = eventData["angleXY"];
				degreesZ = eventData["angleZ"];
				fCombatAngleXY = float(std::sin(std::min(90.0, degreesXY) / radiansToDegrees));
				fCombatAngleZ = float(std::sin(std::min(90.0, degreesZ) / radiansToDegrees));
			}
		}

		// Save values for use in cone test function.
		attackReach_saved = attackReach;
		fCombatAngleXY_saved = fCombatAngleXY;
		fCombatAngleZ_saved = fCombatAngleZ;

		// Call original function.
		const auto TES3_CombatPhysicalHitDetection = reinterpret_cast<void (__cdecl*)(TES3::MobileActor*)>(0x554960);
		TES3_CombatPhysicalHitDetection(attacker);
	}

	// Override of touch magic hit cone check. Saves data to be used in generic hit cone check later.
	TES3::MobileActor* __stdcall PatchCombatTouchMagicHitDetection2(TES3::MobileActor* attacker, TES3::MagicSourceInstance* sourceInstance) {
		auto ndd = TES3::DataHandler::get()->nonDynamicData;
		float attackReach = ndd->GMSTs[TES3::GMST::fCombatDistance]->value.asFloat;
		float fCombatAngleXY = ndd->GMSTs[TES3::GMST::fCombatAngleXY]->value.asFloat;
		float fCombatAngleZ = ndd->GMSTs[TES3::GMST::fCombatAngleZ]->value.asFloat;

		// Fire event.
		if (event::CalcTouchSpellConeEvent::getEventEnabled()) {
			const double radiansToDegrees = 57.29577951308232;
			double degreesXY = radiansToDegrees * std::asin(fCombatAngleXY);
			double degreesZ = radiansToDegrees * std::asin(fCombatAngleZ);

			const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			sol::object response = stateHandle.triggerEvent(new event::CalcTouchSpellConeEvent(attacker, sourceInstance, attackReach, degreesXY, degreesZ));
			if (response.get_type() == sol::type::table) {
				sol::table eventData = response;
				attackReach = eventData["reach"];
				degreesXY = eventData["angleXY"];
				degreesZ = eventData["angleZ"];
				fCombatAngleXY = float(std::sin(std::min(90.0, degreesXY) / radiansToDegrees));
				fCombatAngleZ = float(std::sin(std::min(90.0, degreesZ) / radiansToDegrees));
			}
		}

		// Save values for use in cone test function.
		attackReach_saved = attackReach;
		fCombatAngleXY_saved = fCombatAngleXY;
		fCombatAngleZ_saved = fCombatAngleZ;

		// Call original function.
		const auto TES3_CombatTouchMagicHitDetection = reinterpret_cast<TES3::MobileActor* (__cdecl*)(TES3::MobileActor*)>(0x554C30);
		return TES3_CombatTouchMagicHitDetection(attacker);
	}

	__declspec(naked) TES3::MobileActor* PatchCombatTouchMagicHitDetection() {
		__asm {
			push esi	// magicSourceInstance
			push ecx	// attacker mobile
			call PatchCombatTouchMagicHitDetection2
			ret
		}
	}

	// Generic hit cone check overrides.
	float __stdcall PatchCombatHitDetectionCone1() {
		return attackReach_saved;
	}
	float __stdcall PatchCombatHitDetectionCone2(int) {
		return fCombatAngleXY_saved;
	}
	float __stdcall PatchCombatHitDetectionCone3(int) {
		return fCombatAngleZ_saved;
	}

	// Override of reach calculation only for MACT::combatWeaponStrike caller.
	bool PatchCombatStrikeIsActorInReach(TES3::MobileActor* attacker, TES3::MobileActor* target, float* out_distanceBetweenActors) {
		// Original code.
		if (attacker == nullptr || target == nullptr) {
			return false;
		}
		if (!target->getMobileObjectFlag(TES3::MobileActorFlag::ActiveInSimulation)) {
			return false;
		}
		if (target->reference && target->reference->getDisabled()) {
			return false;
		}

		// Use event to allow attack reach to be modified.
		auto ndd = TES3::DataHandler::get()->nonDynamicData;
		float attackReach = attacker->getAttackReach();
		float fCombatAngleXY = ndd->GMSTs[TES3::GMST::fCombatAngleXY]->value.asFloat;
		float fCombatAngleZ = ndd->GMSTs[TES3::GMST::fCombatAngleZ]->value.asFloat;

		// Fire event.
		if (event::CalcHitDetectionConeEvent::getEventEnabled()) {
			const double radiansToDegrees = 57.29577951308232;
			double degreesXY = radiansToDegrees * std::asin(fCombatAngleXY);
			double degreesZ = radiansToDegrees * std::asin(fCombatAngleZ);

			const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			sol::object response = stateHandle.triggerEvent(new event::CalcHitDetectionConeEvent(attacker, target, attackReach, degreesXY, degreesZ));
			if (response.get_type() == sol::type::table) {
				sol::table eventData = response;
				attackReach = eventData["reach"];
			}
		}

		// Original code.
		const auto TES3_getDistanceToActorBounds = reinterpret_cast<float (__cdecl*)(TES3::MobileActor*, TES3::MobileObject*, bool)>(0x53ADD0);
		*out_distanceBetweenActors = TES3_getDistanceToActorBounds(attacker, target, true);
		auto fCombatDistance = TES3::DataHandler::get()->nonDynamicData->GMSTs[TES3::GMST::fCombatDistance]->value.asFloat;
		if (attackReach < fCombatDistance) {
			attackReach *= fCombatDistance;
		}

		return std::fabs(attacker->getPosition()->z - target->getPosition()->z) < attackReach
			&& *out_distanceBetweenActors < attackReach;
	}

	//
	// Event: objectCopied
	//

	void DispatchCopiedObjectEvent(TES3::Object* copy, TES3::Object* original) {
		using mwse::lua::LuaManager;
		using mwse::lua::event::ObjectCopiedEvent;

		ObjectCopiedEvent::ms_LastCopied = copy;
		ObjectCopiedEvent::ms_LastCopiedFrom = original;

		if (ObjectCopiedEvent::getEventEnabled()) {
			const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			stateHandle.triggerEvent(new ObjectCopiedEvent(copy, original));
		}
	}

	template <typename T, DWORD address>
	void __fastcall CopyObject(T* self, DWORD _EDX_, T* from, BYTE unknown) {
		reinterpret_cast<void(__thiscall*)(T*, const T*, BYTE)>(address)(self, from, unknown);
		DispatchCopiedObjectEvent(self, from);
	}

	template <typename T, DWORD address>
	bool OverwriteCopyObjectVirtualCall(TES3::VirtualTableAddress::VirtualTableAddress vTableAddress) {
		static_assert(std::is_base_of<TES3::Object, T>::value, "Attempt to override virtual table of non-TES3::Object class.");
		return overrideVirtualTableEnforced(vTableAddress, 0x24, address, reinterpret_cast<DWORD>(&CopyObject<T, address>));
	}

	//
	// Event: objectCreated
	//

	const auto TES3_AddObjectToHashMapById = reinterpret_cast<int(__thiscall*)(void*, const char*, TES3::BaseObject*)>(0x481D80);
	int __fastcall AddObjectToHashMapById(void* hashMap, DWORD _EDX_, const char* id, TES3::BaseObject* object) {
		const auto result = TES3_AddObjectToHashMapById(hashMap, id, object);

		if (mwse::lua::event::ObjectCreatedEvent::getEventEnabled()) {
			const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			stateHandle.triggerEvent(new mwse::lua::event::ObjectCreatedEvent(object));
		}

		return result;
	}

	//
	// Event: Play item consumption sound event.
	//

	TES3::Sound* __fastcall PatchConsumeItemAddSoundById(TES3::DataHandler* dataHandler, DWORD _EDX_, const char* id, TES3::Reference* reference, int playbackFlags, unsigned __int8 volume, float pitch, TES3::BaseObject* patched_item) {
		// Allow event overrides.
		if (mwse::lua::event::PlayItemSoundEvent::getEventEnabled()) {
			auto& luaManager = mwse::lua::LuaManager::getInstance();
			const auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::table result = stateHandle.triggerEvent(new mwse::lua::event::PlayItemSoundEvent(patched_item, int(TES3::ItemSoundState::Consume), reference));
			if (result.valid() && result.get_or("block", false)) {
				return nullptr;
			}
		}

		// Run original code.
		return dataHandler->addSoundById(id, reference, playbackFlags, volume, pitch, 0);
	}

	__declspec(naked) bool patchConsumeItemDrinkArgs() {
		__asm {
			push ebx
			nop
		}
	}
	const size_t patchConsumeItemDrinkArgs_size = 2;

	__declspec(naked) bool patchConsumeItemSwallowArgs() {
		__asm {
			push edi
			nop
		}
	}
	const size_t patchConsumeItemSwallowArgs_size = 2;

	//
	// Patch: Magic reflect events.
	//
	
	const auto vfxReflectPtr = reinterpret_cast<TES3::PhysicalObject**>(0x7CF110);

	bool __stdcall OnMagicReflect2(TES3::MagicSourceInstance* sourceInstance, TES3::Reference* hitReference, TES3::ActiveMagicEffect* reflectEffect) {
		float reflectChance = float(reflectEffect->unresistedMagnitude);

		// Allow event overrides.
		if (mwse::lua::event::MagicReflectEvent::getEventEnabled()) {
			auto& luaManager = mwse::lua::LuaManager::getInstance();
			const auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::table result = stateHandle.triggerEvent(new mwse::lua::event::MagicReflectEvent(sourceInstance, hitReference, reflectEffect, reflectChance));
			if (result.valid()) {
				if (result.get_or("block", false)) {
					return false;
				}
				reflectChance = result["reflectChance"];
			}
		}

		// Return true if the magic was reflected.
		int roll = tes3::rand() % 100;
		bool success = roll < reflectChance;

		if (success) {
			if (mwse::lua::event::MagicReflectedEvent::getEventEnabled()) {
				auto& luaManager = mwse::lua::LuaManager::getInstance();
				const auto stateHandle = luaManager.getThreadSafeStateHandle();
				stateHandle.triggerEvent(new mwse::lua::event::MagicReflectedEvent(sourceInstance, hitReference, reflectEffect));
			}
		}
		return success;
	}

	__declspec(naked) bool OnMagicReflect() {
		__asm {
			lea eax, [edi + 8]		// lea eax, [edi + ActiveMagicEffectNode.data]
			push eax				// push activeMagicEffect
			push ebp				// push hitReference
			push ebx				// push magicSourceInstance
			call OnMagicReflect2
			ret
		}
	}

	__declspec(naked) bool patchMagicReflect() {
		// This patch alters the last stage of the reflect magic mechanic, so that all other tests run first.
		// It also avoids conflicting with the magic effects flag patch at 0x516F06.
		__asm {
			call OnMagicReflect		// Replace with call to correct address
			test al, al
			jz $ + 0x97				// Skip reflect if event blocked or roll fails

			// Original code, relocated
			__asm _emit 0xA1 __asm _emit 0x10  __asm _emit 0xF1 __asm _emit 0x7C __asm _emit 0x00	// mov eax, global_vfx_reflect (the assembler won't output it correctly)
			test eax, eax
			__asm _emit 0x74 __asm _emit 0x1B	// jz short $+0x1D (the assembler won't output short jumps)

			push 0
			push esi
			push eax
			push 0
			push ebp
			push 0
			push 0
			push 0
		}
	}
	const size_t patchMagicReflect_size = 0x23;

	//
	// Patch: Pickpocket event.
	//

	const auto TES3_MobileActor_Pickpocket = reinterpret_cast<int (__thiscall*)(TES3::MobileActor*, int, TES3::MobilePlayer*)>(0x52AFD0);

	bool __fastcall OnPickpocket2(TES3::MobileActor* mobile, DWORD _EDX_, TES3::UI::InventoryTile* tile, int count, int difficulty) {
		auto macp = TES3::WorldController::get()->getMobilePlayer();

		// The vanilla pickpocket function is patched to return the % chance instead of a boolean success.
		int chance = TES3_MobileActor_Pickpocket(mobile, difficulty, macp);

		if (mwse::lua::event::PickpocketEvent::getEventEnabled()) {
			const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			sol::object response;
			
			if (tile) {
				// On pickpocketing an item.
				response = stateHandle.triggerEvent(new event::PickpocketEvent(mobile, tile->item, tile->itemData, count, chance));
			}
			else {
				// On closing the window.
				response = stateHandle.triggerEvent(new event::PickpocketEvent(mobile, nullptr, nullptr, 0, chance));
			}

			if (response.get_type() == sol::type::table) {
				sol::table eventData = response;
				chance = eventData["chance"];
			}
		}

		int roll = tes3::rand() % 100;
		bool success = roll < chance;
		return success;
	}

	__declspec(naked) bool OnPickpocket(int difficulty, TES3::MobilePlayer* macp) {
		__asm {
			mov eax, [esp + 0xA4]		// mov eax, [esp + count]
			push [esp + 0x4]			// push [difficulty]
			push eax					// push count
			push ebp					// push tile
			call OnPickpocket2
			ret 0x8
		}
	}

	__declspec(naked) bool OnPickpocketOnExit(int difficulty, TES3::MobilePlayer* macp) {
		__asm {
			push [esp + 0x4]			// push [difficulty]
			push 0						// push count
			push 0						// push tile
			call OnPickpocket2
			ret 0x8
		}
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
			logStackTrace(nullptr);
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

	ThreadedStateHandle::ThreadedStateHandle(LuaManager* manager) :
		luaManager(manager),
		mutexGuard(manager->stateThreadMutex)
	{

	}

	ThreadedStateHandle::~ThreadedStateHandle() {

	}

	sol::state& ThreadedStateHandle::getState() const {
		return luaManager->luaState;
	}

	sol::object ThreadedStateHandle::triggerEvent(event::BaseEvent* e) const {
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
		// Add core/lib directories to path.
		{
			std::stringstream envPath;
			envPath << (std::filesystem::current_path() / "Data Files" / "MWSE" / "core").string() << ";";
			envPath << (std::filesystem::current_path() / "Data Files" / "MWSE" / "lib").string() << ";";
			envPath << getenv("Path");
			_putenv_s("Path", envPath.str().c_str());
		}

		// Execute initialize.lua
		try {
			sol::protected_function_result result = luaState.safe_script_file("Data Files\\MWSE\\core\\initialize.lua");
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

		// Bind MGE.
		if (isMGEAPIAvailable()) {
			// MGE API available from 0.14+.
			bindMGEPostShaders();
			bindMGEUtil();
		}
		else {
			// Keep legacy MWScript based commands available.
			bindLegacyMGEScriptUtil();
		}

		// Alter existing libraries.
		luaState["os"]["exit"] = customOSExit;

		// Cache some often used functions.
		fnTableEmpty = luaState["table"]["empty"];
		fnEncodeForSave = luaState["mwse"]["encodeForSave"];
		fnDecodeFromSave = luaState["json"]["decode"];

		// Grab early-load data.
		luaState["tes3"]["bsaLoader"] = TES3::BSALoader::get();

		// Hook the RunScript function so we can intercept Lua scripts and invoke Lua code if needed.
		genJumpUnprotected(TES3_HOOK_RUNSCRIPT_LUACHECK, reinterpret_cast<DWORD>(HookRunScript), TES3_HOOK_RUNSCRIPT_LUACHECK_SIZE);

		// Hook the MACP creation functions to update lua variables that point to the player.
		genCallEnforced(0x5661A0, 0x4E5770, reinterpret_cast<DWORD>(OnPlayerReferenceAssigned));
		genCallEnforced(0x4C0180, 0x4E4510, reinterpret_cast<DWORD>(OnPlayerReferenceCreated));
		genCallEnforced(0x565F53, 0x5662F0, reinterpret_cast<DWORD>(OnPlayerMobileDestruction));

		// Event: initialized. Hook initial plugin loading and merging function.
		genCallEnforced(0x419EE7, 0x40E790, reinterpret_cast<DWORD>(InitDataHandler));
		genCallEnforced(0x418F88, 0x419CE0, reinterpret_cast<DWORD>(FinishInitialization));

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
		genCallEnforced(0x41A44D, 0x419A90, reinterpret_cast<DWORD>(OnNewGameViaStartingCell1));
		genCallEnforced(0x41A6E4, 0x563CE0, reinterpret_cast<DWORD>(OnNewGameViaStartingCell2));

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

		// Event: Melee strike
		genCallEnforced(0x5419D7, 0x556F40, reinterpret_cast<DWORD>(OnAttackStrike));

		// Event: Begin physical attack
		genCallEnforced(0x54A4BA, 0x5411C0, reinterpret_cast<DWORD>(OnAttackStart));
		genCallEnforced(0x569DFB, 0x5411C0, reinterpret_cast<DWORD>(OnAttackStart));

		// Collision events: Mobile Object
		auto mobileObjectCollideActor = &TES3::MobileObject::onActorCollision;
		auto mobileObjectCollidObject = &TES3::MobileObject::onObjectCollision;
		auto mobileObjectCollideTerrain = &TES3::MobileObject::onTerrainCollision;
		auto mobileObjectCollideWater = &TES3::MobileObject::onWaterCollision;
		auto mobileObjectCollideActivator = &TES3::MobileObject::onActivatorCollision;
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::MobileObject, 0x80, 0x5615A0, *reinterpret_cast<DWORD*>(&mobileObjectCollideActor));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::MobileObject, 0x84, 0x5615C0, *reinterpret_cast<DWORD*>(&mobileObjectCollidObject));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::MobileObject, 0x88, 0x5615E0, *reinterpret_cast<DWORD*>(&mobileObjectCollideTerrain));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::MobileObject, 0x8C, 0x5615E0, *reinterpret_cast<DWORD*>(&mobileObjectCollideTerrain));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::MobileObject, 0x90, 0x561600, *reinterpret_cast<DWORD*>(&mobileObjectCollideActivator));

		// Collision events: Mobile Actor
		auto mobileActorCollideActor = &TES3::MobileActor::onActorCollision;
		auto mobileActorCollidObject = &TES3::MobileActor::onObjectCollision;
		auto mobileActorCollideTerrain = &TES3::MobileActor::onTerrainCollision;
		auto mobileActorCollideActivator = &TES3::MobileActor::onActivatorCollision;
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::MobileActor, 0x80, 0x5234A0, *reinterpret_cast<DWORD*>(&mobileActorCollideActor));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::MobileActor, 0x84, 0x5233B0, *reinterpret_cast<DWORD*>(&mobileActorCollidObject));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::MobileActor, 0x88, 0x523310, *reinterpret_cast<DWORD*>(&mobileActorCollideTerrain));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::MobileActor, 0x8C, 0x5615E0, *reinterpret_cast<DWORD*>(&mobileObjectCollideTerrain));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::MobileActor, 0x90, 0x523590, *reinterpret_cast<DWORD*>(&mobileActorCollideActivator));

		// Collision events: Mobile Creature
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::MobileCreature, 0x80, 0x5234A0, *reinterpret_cast<DWORD*>(&mobileActorCollideActor));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::MobileCreature, 0x84, 0x5233B0, *reinterpret_cast<DWORD*>(&mobileActorCollidObject));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::MobileCreature, 0x88, 0x523310, *reinterpret_cast<DWORD*>(&mobileActorCollideTerrain));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::MobileCreature, 0x8C, 0x5615E0, *reinterpret_cast<DWORD*>(&mobileObjectCollideTerrain));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::MobileCreature, 0x90, 0x523590, *reinterpret_cast<DWORD*>(&mobileActorCollideActivator));

		// Collision events: Mobile NPC
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::MobileNPC, 0x80, 0x5234A0, *reinterpret_cast<DWORD*>(&mobileActorCollideActor));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::MobileNPC, 0x84, 0x5233B0, *reinterpret_cast<DWORD*>(&mobileActorCollidObject));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::MobileNPC, 0x88, 0x523310, *reinterpret_cast<DWORD*>(&mobileActorCollideTerrain));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::MobileNPC, 0x8C, 0x5615E0, *reinterpret_cast<DWORD*>(&mobileObjectCollideTerrain));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::MobileNPC, 0x90, 0x523590, *reinterpret_cast<DWORD*>(&mobileActorCollideActivator));

		// Collision events: Mobile Player
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::MobilePlayer, 0x80, 0x5234A0, *reinterpret_cast<DWORD*>(&mobileActorCollideActor));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::MobilePlayer, 0x84, 0x5233B0, *reinterpret_cast<DWORD*>(&mobileActorCollidObject));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::MobilePlayer, 0x88, 0x523310, *reinterpret_cast<DWORD*>(&mobileActorCollideTerrain));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::MobilePlayer, 0x8C, 0x5615E0, *reinterpret_cast<DWORD*>(&mobileObjectCollideTerrain));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::MobilePlayer, 0x90, 0x523590, *reinterpret_cast<DWORD*>(&mobileActorCollideActivator));

		// Collision events: Mobile Projectile
		auto mobileProjectileCollideActor = &TES3::MobileProjectile::onActorCollision;
		auto mobileProjectileCollideObject = &TES3::MobileProjectile::onObjectCollision;
		auto mobileProjectileCollideTerrain = &TES3::MobileProjectile::onTerrainCollision;
		auto mobileProjectileCollideWater = &TES3::MobileProjectile::onWaterCollision;
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::MobileProjectile, 0x80, 0x573860, *reinterpret_cast<DWORD*>(&mobileProjectileCollideActor));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::MobileProjectile, 0x84, 0x573820, *reinterpret_cast<DWORD*>(&mobileProjectileCollideObject));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::MobileProjectile, 0x88, 0x5737F0, *reinterpret_cast<DWORD*>(&mobileProjectileCollideTerrain));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::MobileProjectile, 0x8C, 0x573790, *reinterpret_cast<DWORD*>(&mobileProjectileCollideWater));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::MobileProjectile, 0x90, 0x561600, *reinterpret_cast<DWORD*>(&mobileObjectCollideActivator));

		// Event: Mobile Projectile Expire
		genCallEnforced(0x57548A, 0x5637F0, reinterpret_cast<DWORD>(OnProjectileExpire));

		// Event: UI Event
		genJumpUnprotected(TES3_HOOK_UI_EVENT, reinterpret_cast<DWORD>(HookUIEvent), TES3_HOOK_UI_EVENT_SIZE);

		// Hook tooltip events.
		genNOPUnprotected(0x58363B, 0x58364A - 0x58363B);
		writePatchCodeUnprotected(0x58363B, (BYTE*)&HookOnUITooltipEvent, HookOnUITooltipEvent_size);
		genCallUnprotected(0x58363B + 0x1, reinterpret_cast<DWORD>(OnUITooltipEvent));

		// Event: Show Rest/Wait Menu
		genCallEnforced(0x41ADB6, 0x610170, reinterpret_cast<DWORD>(OnShowRestWaitMenu));
		genCallEnforced(0x5097BA, 0x610170, reinterpret_cast<DWORD>(OnShowRestWaitMenuScripted));
		genCallEnforced(0x5097DD, 0x610170, reinterpret_cast<DWORD>(OnShowRestWaitMenuScripted));

		// Event: Key
		genCallEnforced(0x40F633, 0x4065E0, reinterpret_cast<DWORD>(OnKeyReadState));
		genCallEnforced(0x736C04, 0x4065E0, reinterpret_cast<DWORD>(OnKeyReadState));

		// Event: Dying/death.
		overrideVirtualTable(TES3::VirtualTableAddress::MobileActor, 0xB8, reinterpret_cast<DWORD>(OnMobileActorDeath)); // MACT
		overrideVirtualTable(TES3::VirtualTableAddress::MobileCreature, 0xB8, reinterpret_cast<DWORD>(OnMobileActorDeath)); // MACR
		overrideVirtualTable(TES3::VirtualTableAddress::MobileNPC, 0xB8, reinterpret_cast<DWORD>(OnMobileActorDeath)); // MACH
		overrideVirtualTable(TES3::VirtualTableAddress::MobilePlayer, 0xB8, reinterpret_cast<DWORD>(OnMobilePlayerDeath)); // MACP

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

		// Allow changes to hitstun test function.
		auto hitStunTestAndReportCrime = &TES3::MobileActor::hitStunTestAndReportCrime;
		genCallEnforced(0x556AE9, 0x557840, *reinterpret_cast<DWORD*>(&hitStunTestAndReportCrime));

		// Event: Shield blocked
		writePatchCodeUnprotected(0x5575EC, (BYTE*)&patchOnShieldWearFromBlocking, patchOnShieldWearFromBlocking_size);
		genCallEnforced(0x5575EF, 0x558690, reinterpret_cast<DWORD>(OnShieldWearFromBlocking));

		// Event: Spell cast resolution
		genCallEnforced(0x5156B2, 0x4AA950, reinterpret_cast<DWORD>(OnSpellCastResolution));

		// Event: Magic cast success
		genJumpUnprotected(TES3_HOOK_MAGIC_CAST_SUCCESS, reinterpret_cast<DWORD>(HookMagicCastSuccess), TES3_HOOK_MAGIC_CAST_SUCCESS_SIZE);

		// Event: Spell cast failure
		genJumpUnprotected(TES3_HOOK_SPELL_CAST_FAILURE, reinterpret_cast<DWORD>(HookSpellCastFailure), TES3_HOOK_SPELL_CAST_FAILURE_SIZE);

		// Event: Spell Resist
		genCallEnforced(0x518616, 0x517E40, reinterpret_cast<DWORD>(OnSpellResist));

		// Event: Spell Resisted
		genCallUnprotected(0x51880F, reinterpret_cast<DWORD>(OnSpellResistedWrapper), 0x518816 - 0x51880F);

		// Event: Magic effect removed
		genCallEnforced(0x5125F9, 0x55C9D0, reinterpret_cast<DWORD>(OnMagicEffectRemoved)); // Magic Source Instance: Destructor
		genCallEnforced(0x512A17, 0x55C9D0, reinterpret_cast<DWORD>(OnMagicEffectRemoved)); // Magic Source Instance: Retire Effects
		genCallEnforced(0x515AEF, 0x55C9D0, reinterpret_cast<DWORD>(OnMagicEffectRemoved)); // Magic Source Instance: Process
		genCallEnforced(0x518FCC, 0x55C9D0, reinterpret_cast<DWORD>(OnMagicEffectRemoved)); // Magic Source Instance: Spell Effect Event

		// Event: Reflect magic
		genNOPUnprotected(0x516EA0, 0x1C);
		writePatchCodeUnprotected(0x516F17, (BYTE*)&patchMagicReflect, patchMagicReflect_size);
		genCallUnprotected(0x516F17, reinterpret_cast<DWORD>(OnMagicReflect));

		// Event: Absorbed magic
		auto onAbsorbedMagic = &TES3::MagicSourceInstance::onAbsorbedMagic;
		genCallEnforced(0x51783E, 0x519900, *reinterpret_cast<DWORD*>(&onAbsorbedMagic));
		genCallEnforced(0x5178E9, 0x519900, *reinterpret_cast<DWORD*>(&onAbsorbedMagic));
		genCallEnforced(0x517A50, 0x519900, *reinterpret_cast<DWORD*>(&onAbsorbedMagic));
		genCallEnforced(0x517CA7, 0x519900, *reinterpret_cast<DWORD*>(&onAbsorbedMagic));
		genCallEnforced(0x517D90, 0x519900, *reinterpret_cast<DWORD*>(&onAbsorbedMagic));

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

		// Event: Potion strength and skill check.
		genCallEnforced(0x59CB62, 0x59D610, reinterpret_cast<DWORD>(OnAlchemySkillRoll));

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
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::Alchemy, offsetof(TES3::BaseObjectVirtualTable, TES3::BaseObjectVirtualTable::loadObjectSpecific), 0x4ABD90, *reinterpret_cast<DWORD*>(&alchemyLoadObjectSpecific));

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

		// Event: Calculate creature movement speed (animation only). Creature anim code doesn't use the 0x53E1A0 helper function.
		auto calculateCreatureMovementSpeed = &TES3::ActorAnimationController::calculateCreatureMovementSpeed;
		writePatchCodeUnprotected(0x540BB1, reinterpret_cast<BYTE*>(patchCreatureCalcMoveSpeed), patchCreatureCalcMoveSpeed_size);
		genCallUnprotected(0x540BB1 + 2, *reinterpret_cast<DWORD*>(&calculateCreatureMovementSpeed));
		genNOPUnprotected(0x540C19, 0xA);
		writePatchCodeUnprotected(0x540C19, reinterpret_cast<BYTE*>(patchCreatureCalcMoveSpeed), patchCreatureCalcMoveSpeed_size);
		genCallUnprotected(0x540C19 + 2, *reinterpret_cast<DWORD*>(&calculateCreatureMovementSpeed));

		// Event: Calculate walk speed.
		auto calculateCreatureWalkSpeed = &TES3::MobileCreature::calculateWalkSpeed;
		auto calculateNPCWalkSpeed = &TES3::MobileNPC::calculateWalkSpeed;
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::MobileCreature, 0xB4, 0x55AAF0, *reinterpret_cast<DWORD*>(&calculateCreatureWalkSpeed));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::MobileActor, 0xB4, 0x526F70, *reinterpret_cast<DWORD*>(&calculateNPCWalkSpeed));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::MobileNPC, 0xB4, 0x526F70, *reinterpret_cast<DWORD*>(&calculateNPCWalkSpeed));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::MobilePlayer, 0xB4, 0x526F70, *reinterpret_cast<DWORD*>(&calculateNPCWalkSpeed));

		// Event: Calculate run speed.
		auto calculateRunSpeed = &TES3::MobileActor::calculateRunSpeed;
		genCallEnforced(0x53E23B, 0x527050, *reinterpret_cast<DWORD*>(&calculateRunSpeed));
		genCallEnforced(0x548D95, 0x527050, *reinterpret_cast<DWORD*>(&calculateRunSpeed));

		// Event: Calculate swim speed.
		auto calculateSwimSpeed = &TES3::MobileActor::calculateSwimSpeed;
		genCallEnforced(0x53E227, 0x5270B0, *reinterpret_cast<DWORD*>(&calculateSwimSpeed));
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
		genCallEnforced(0x5F9376, 0x583B70, reinterpret_cast<DWORD>(OnUICreatedAfterPerformLayout)); // MenuNotify1-3
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

		// Event: UI Tooltip post-creation for statmenu skills.
		genPushEnforced(0x6267BF, reinterpret_cast<DWORD>(OnUISkillStatTooltip));
		genPushEnforced(0x626825, reinterpret_cast<DWORD>(OnUISkillStatTooltip));
		genPushEnforced(0x626A3D, reinterpret_cast<DWORD>(OnUISkillStatTooltip));
		genPushEnforced(0x626AA3, reinterpret_cast<DWORD>(OnUISkillStatTooltip));
		genPushEnforced(0x626CDE, reinterpret_cast<DWORD>(OnUISkillStatTooltip));
		genPushEnforced(0x626D4C, reinterpret_cast<DWORD>(OnUISkillStatTooltip));
		//genPushEnforced(0x626F88, reinterpret_cast<DWORD>(OnUISkillStatTooltip));	//factions, not skills
		//genPushEnforced(0x627083, reinterpret_cast<DWORD>(OnUISkillStatTooltip)); //birthsign, not skills
		//genPushEnforced(0x6271AD, reinterpret_cast<DWORD>(OnUISkillStatTooltip)); //reputation, not skills
		//genPushEnforced(0x627259, reinterpret_cast<DWORD>(OnUISkillStatTooltip)); //bounty, not skills

		//And for other skill tooltips.
		genCallEnforced(0x5AFBBD, 0x6297F0, reinterpret_cast<DWORD>(OnUISkillTooltip));
		genCallEnforced(0x5BAEF8, 0x6297F0, reinterpret_cast<DWORD>(OnUISkillTooltip));
		genCallEnforced(0x60C9EF, 0x6297F0, reinterpret_cast<DWORD>(OnUISkillTooltip));
		genCallEnforced(0x61811D, 0x6297F0, reinterpret_cast<DWORD>(OnUISkillTooltip));
		genCallEnforced(0x61DFED, 0x6297F0, reinterpret_cast<DWORD>(OnUISkillTooltip));
		genCallEnforced(0x62EB28, 0x6297F0, reinterpret_cast<DWORD>(OnUISkillTooltip));

		/*
		// TODO: Figure out a good hook point for this.
		genCallEnforced(0x0, 0x595A40, reinterpret_cast<DWORD>(OnBuildUI_Tooltip));
		genCallEnforced(0x0, 0x590D90, reinterpret_cast<DWORD>(OnBuildUI_ItemTooltip));
		*/

		// Event: Activation Target Changed
		genCallEnforced(0x41B3D6, 0x41CA50, reinterpret_cast<DWORD>(OnCheckPlayerActivationTarget));

		// Event: Active magic effect icons updated
		const auto onActiveMagicEffectIconsUpdated = &TES3::MagicInstanceController::updateActiveMagicEffectIcons;
		genCallEnforced(0x5E6394, 0x5E2480, reinterpret_cast<DWORD>(onActiveMagicEffectIconsUpdated)); // Delete spell
		genCallEnforced(0x5E150A, 0x5E2480, reinterpret_cast<DWORD>(onActiveMagicEffectIconsUpdated)); // Toggle magic menu
		genCallEnforced(0x5E1362, 0x5E2480, reinterpret_cast<DWORD>(onActiveMagicEffectIconsUpdated)); // Show magic menu
		genCallEnforced(0x518FE0, 0x5E2480, reinterpret_cast<DWORD>(onActiveMagicEffectIconsUpdated)); // MagicSourceInstance: Spell effect event
		genCallEnforced(0x517AB3, 0x5E2480, reinterpret_cast<DWORD>(onActiveMagicEffectIconsUpdated)); // MagicSourceInstance: Projectile hit
		genCallEnforced(0x517581, 0x5E2480, reinterpret_cast<DWORD>(onActiveMagicEffectIconsUpdated)); // MagicSourceInstance: Spell hit
		genCallEnforced(0x515B14, 0x5E2480, reinterpret_cast<DWORD>(onActiveMagicEffectIconsUpdated)); // MagicSourceInstance: Process
		genCallEnforced(0x514968, 0x5E2480, reinterpret_cast<DWORD>(onActiveMagicEffectIconsUpdated)); // MagicSourceInstance: Process
		genCallEnforced(0x512626, 0x5E2480, reinterpret_cast<DWORD>(onActiveMagicEffectIconsUpdated)); // MagicSourceInstance: Destructor
		genCallEnforced(0x50BDB3, 0x5E2480, reinterpret_cast<DWORD>(onActiveMagicEffectIconsUpdated)); // mwscript: RemoveSpell
		genCallEnforced(0x4C5310, 0x5E2480, reinterpret_cast<DWORD>(onActiveMagicEffectIconsUpdated)); // RecordsHandler: Load game - Main menu
		genCallEnforced(0x4C4CFC, 0x5E2480, reinterpret_cast<DWORD>(onActiveMagicEffectIconsUpdated)); // RecordsHandler: Load game - Ingame
		genCallEnforced(0x455856, 0x5E2480, reinterpret_cast<DWORD>(onActiveMagicEffectIconsUpdated)); // MagicInstanceController: Clear spell by cast type

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

		// Setup existing calls to change music for the change music event so we can know the situation slightly earlier.
		genCallEnforced(0x40F895, 0x403AC0, reinterpret_cast<DWORD>(OnChangeMusic_TitleLoop));
		genCallEnforced(0x40F8E1, 0x403AC0, reinterpret_cast<DWORD>(OnChangeMusic_Combat));
		genCallEnforced(0x40F918, 0x403AC0, reinterpret_cast<DWORD>(OnChangeMusic_Explore));
		genCallEnforced(0x410FC8, 0x403AC0, reinterpret_cast<DWORD>(OnChangeMusic_Event));
		genCallEnforced(0x5FC8A1, 0x410FA0, reinterpret_cast<DWORD>(OnChangeMusic_Event_MainMenu));
		genCallEnforced(0x634E92, 0x410FA0, reinterpret_cast<DWORD>(OnChangeMusic_Event_LevelUp));
		genCallEnforced(0x56A139, 0x410FA0, reinterpret_cast<DWORD>(OnChangeMusic_Event_Death));
		genCallEnforced(0x45C969, 0x403AC0, reinterpret_cast<DWORD>(OnChangeMusic_MwscriptStreamMusic)); // Non-MCP
		genCallEnforced(0x45C969, 0x403B14, reinterpret_cast<DWORD>(OnChangeMusic_MwscriptStreamMusic)); // MCP

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
		genCallEnforced(0x4A44F8, 0x49A190, reinterpret_cast<DWORD>(ContainerCloseResolveLeveledLists));
		writePatchCodeUnprotected(0x4D838E, (BYTE*)&patchNPCCloneResolveLeveledLists, patchNPCCloneResolveLeveledLists_size);
		genCallEnforced(0x4D83A1, 0x49A190, reinterpret_cast<DWORD>(NPCCloneResolveLeveledLists));
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

		// Event: Mobile activated/deactivated.
		// MobileObject::enterLeaveSimulation call at 0x57249B is not required as it is already taken care of by the other hooks.
		genCallEnforced(0x524C34, 0x561CB0, reinterpret_cast<DWORD>(onMobileObjectEnterLeaveSimulation)); // MobileActor::enterLeaveSimulation
		genCallEnforced(0x524A63, 0x561CB0, reinterpret_cast<DWORD>(onMobileObjectEnterLeaveSimulation)); // MobileActor::enterLeaveSimulation
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::MobileObject, 0x70, 0x561CB0, reinterpret_cast<DWORD>(onMobileObjectEnterLeaveSimulation));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::MobileProjectile, 0x70, 0x561CB0, reinterpret_cast<DWORD>(onMobileObjectEnterLeaveSimulation));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::SpellProjectile, 0x70, 0x561CB0, reinterpret_cast<DWORD>(onMobileObjectEnterLeaveSimulation));

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

		// Event: Calculate spell making price.
		genCallEnforced(0x622332, 0x581F30, reinterpret_cast<DWORD>(OnCalculateSpellmakingPrice));

		// Event: Calculate enchantment making price.
		genCallEnforced(0x5C3C47, 0x52AA50, reinterpret_cast<DWORD>(OnCalculateEnchantmentPrice));

		// Event: Calculate spell making point cost.
		genCallEnforced(0x6223BD, 0x581F30, reinterpret_cast<DWORD>(OnCalculateSpellmakingSpellPointCost));

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
		genCallEnforced(0x5EB319, 0x5EB760, reinterpret_cast<DWORD>(OnRefreshedLocalMap)); // MenuMap_local

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
		genJumpEnforced(0x50E594, 0x50E5A0, reinterpret_cast<DWORD>(OnRunDialogueCommand)); // MCP added functionality.

		// Allow `;lua ` to flag a dialog as having a valid script.
		genNOPUnprotected(0x4AF5EF, 0x4AF692 - 0x4AF5EF);
		writePatchCodeUnprotected(0x4AF5EF, (BYTE*)PatchOnSetDialogueInfoHasCommand, PatchOnSetDialogueInfoHasCommand_Size);
		genCallUnprotected(0x4AF5EF + PatchOnSetDialogueInfoHasCommand_Size, reinterpret_cast<DWORD>(OnSetDialogueInfoHasCommand));

		// Event: Dialogue link resolve.
		genCallEnforced(0x40B89E, 0x4BA8D0, reinterpret_cast<DWORD>(OnInfoLinkResolve));

		// Hook overriding book text.
		auto bookGetText = &TES3::Book::getBookText;
		genCallEnforced(0x4A29FA, 0x4A2A90, *reinterpret_cast<DWORD*>(&bookGetText));
		genCallEnforced(0x4A2A0F, 0x4A2A90, *reinterpret_cast<DWORD*>(&bookGetText));

		// Event: Item Dropped.
		genCallEnforced(0x49B1DF, 0x4E4510, reinterpret_cast<DWORD>(OnItemDropped_ReferenceCreated)); // Store the last created dropped reference.
		genCallEnforced(0x49B542, 0x485C50, reinterpret_cast<DWORD>(OnItemDropped_UpdateExteriors)); // Send event for exterior cells.
		genCallEnforced(0x485FCA, 0x485E40, reinterpret_cast<DWORD>(OnItemDropped)); // MCP-added function.
		genCallEnforced(0x49B550, 0x485E40, reinterpret_cast<DWORD>(OnItemDropped)); // Vanilla function.

		// Patch: Modifiable physical hit detection cone and weapon reach.
		genCallEnforced(0x514780, 0x554C30, reinterpret_cast<DWORD>(PatchCombatTouchMagicHitDetection));
		genCallEnforced(0x55682D, 0x554960, reinterpret_cast<DWORD>(PatchCombatPhysicalHitDetection));
		genCallEnforced(0x554F60, 0x5584C0, reinterpret_cast<DWORD>(PatchCombatHitDetectionCone1));
		genCallEnforced(0x55510B, 0x40F950, reinterpret_cast<DWORD>(PatchCombatHitDetectionCone2));
		genCallEnforced(0x555216, 0x40F950, reinterpret_cast<DWORD>(PatchCombatHitDetectionCone3));
		genCallEnforced(0x556FD4, 0x554E50, reinterpret_cast<DWORD>(PatchCombatStrikeIsActorInReach));

		// Event: Calculate hit chance.
		constexpr auto patchCalcHitChanceAvailableSize = 0x5554F7u - 0x55549Bu;
		static_assert(patchCalculateHitChance_size <= patchCalcHitChanceAvailableSize, "calcHitChance patch too large!");
		genNOPUnprotected(0x55549B, patchCalcHitChanceAvailableSize);
		writePatchCodeUnprotected(0x55549B, (BYTE*)&patchCalculateHitChance, patchCalculateHitChance_size);
		genCallUnprotected(0x55549B + 0x3, reinterpret_cast<DWORD>(OnCalculateHitChance));
		genCallUnprotected(0x556867, reinterpret_cast<DWORD>(OnCalculateHitChanceWrapper));
		genCallUnprotected(0x573BE6, reinterpret_cast<DWORD>(OnCalculateHitChanceWrapper));

		// Event: Calculate block chance.
		genNOPUnprotected(0x555AC3, 0x555B15 - 0x555AC3);
		writePatchCodeUnprotected(0x555AC3, (BYTE*)&patchCalcBlockChance, patchCalcBlockChance_size);
		genCallUnprotected(0x555AC3 + 0x5, reinterpret_cast<DWORD>(OnCalcBlockChance));

		// Event: Jump.
		genNOPUnprotected(0x541FF5, 0x54201E - 0x541FF5);
		writePatchCodeUnprotected(0x541FF5, (BYTE*)&patchJump, patchJump_size);
		genCallUnprotected(0x541FF5 + 0x7, reinterpret_cast<DWORD>(OnJump));

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

		// Override actor soul value checking: recharging
		genCallEnforced(0x60E2D8, 0x581440, reinterpret_cast<DWORD>(PatchGetAliasedSoulValueProperty_GetRepairTool));
		genCallEnforced(0x60E2FC, 0x581440, reinterpret_cast<DWORD>(PatchGetAliasedSoulValueProperty_GetRepairItemData));

		// Override actor soul value check: tooltip
		if (genCallEnforced(0x591760, 0x73658C, reinterpret_cast<DWORD>(PatchGetSoulValueForTooltip))) {
			writePatchCodeUnprotected(0x59175D, (BYTE*)&PatchGetSoulValueForTooltip_LoadObject, PatchGetSoulValueForTooltip_LoadObject_Size);
		}
		else {
			genNOPUnprotected(0x591759, 0x591777 - 0x591759);
			writePatchCodeUnprotected(0x591759, (BYTE*)&PatchGetSoulValueForTooltip_NoMCPLoader, PatchGetSoulValueForTooltip_NoMCPLoader_Size);
			genCallUnprotected(0x591759 + PatchGetSoulValueForTooltip_NoMCPLoader_Size, reinterpret_cast<DWORD>(PatchGetSoulValueForTooltip_NoMCP));
		}

		// Override actor soul value checking: enchanting
		genCallEnforced(0x5C5E8F, 0x58AD30, reinterpret_cast<DWORD>(PatchSetCorrectSoulValueForEnchanting));

		// Override actor soul value check: repair header
		writePatchCodeUnprotected(0x60DCD3, (BYTE*)&PatchGetSoulValueForRechargeTitle_Setup, PatchGetSoulValueForRechargeTitle_Setup_Size);
		genCallUnprotected(0x60DCD3 + PatchGetSoulValueForRechargeTitle_Setup_Size, reinterpret_cast<DWORD>(PatchGetSoulValueForRechargeTitle), 0x60DCE5 - 0x60DCD3 - PatchGetSoulValueForRechargeTitle_Setup_Size);


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
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::Armor, offsetof(TES3::ObjectVirtualTable, getTypeName), 0x4A1270, *reinterpret_cast<DWORD*>(&armorGetSlotName));

		// Allow overriding of clothing slot name.
		auto clothingGetSlotName = &TES3::Clothing::getSlotName;
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::Clothing, offsetof(TES3::ObjectVirtualTable, getTypeName), 0x4A38E0, *reinterpret_cast<DWORD*>(&clothingGetSlotName));

		// Recognize when an inventory tile is updated.
		genCallEnforced(0x5A5DA4, 0x47E720, reinterpret_cast<DWORD>(GetNextInventoryTileToUpdate)); // General barter menu update.
		genCallEnforced(0x5B6655, 0x581F30, reinterpret_cast<DWORD>(OnMenuContentsInventoryTileChildPropertySet)); // During click event into contents menu.
		genCallEnforced(0x5A5008, 0x581F30, reinterpret_cast<DWORD>(OnMenuBarterInventoryTileChildPropertySet)); // During click event into bartered menu.
		genCallEnforced(0x5B6F13, 0x47E720, reinterpret_cast<DWORD>(GetNextInventoryTileToUpdate)); // General contents menu update.
		genCallEnforced(0x5CC366, 0x58AE20, reinterpret_cast<DWORD>(OnSetItemTileIcon)); // On manual AddTile.
		genCallEnforced(0x5CD147, 0x47E720, reinterpret_cast<DWORD>(GetNextInventoryTileToUpdate)); // General inventory menu update.
		genCallEnforced(0x4EA72E, 0x5D14F0, reinterpret_cast<DWORD>(OnAttachTileToCursor));
		genCallEnforced(0x5A4475, 0x5D14F0, reinterpret_cast<DWORD>(OnAttachTileToCursor));
		genCallEnforced(0x5A44B4, 0x5D14F0, reinterpret_cast<DWORD>(OnAttachTileToCursor));
		genCallEnforced(0x5B556F, 0x5D14F0, reinterpret_cast<DWORD>(OnAttachTileToCursor));
		genCallEnforced(0x5B5C3B, 0x5D14F0, reinterpret_cast<DWORD>(OnAttachTileToCursor));
		genCallEnforced(0x5C6024, 0x5D14F0, reinterpret_cast<DWORD>(OnAttachTileToCursor));
		genCallEnforced(0x5CBBCD, 0x5D14F0, reinterpret_cast<DWORD>(OnAttachTileToCursor));
		genCallEnforced(0x5D0F5F, 0x5D14F0, reinterpret_cast<DWORD>(OnAttachTileToCursor));

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
		genCallEnforced(0x46589C, 0x4E7750, reinterpret_cast<DWORD>(&TES3::ItemData::createForBoundItem));
		genCallEnforced(0x465CFC, 0x4E7750, reinterpret_cast<DWORD>(&TES3::ItemData::createForBoundItem));
		genCallEnforced(0x465D92, 0x4E7750, reinterpret_cast<DWORD>(&TES3::ItemData::createForBoundItem));
		genCallEnforced(0x465F1A, 0x4E7750, reinterpret_cast<DWORD>(&TES3::ItemData::createForBoundItem));
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

		// Override item data re-stackable tests to ensure item data with lua variables are preserved.
		genCallEnforced(0x41089C, 0x4E7970, reinterpret_cast<DWORD>(&TES3::ItemData::isItemDataStackable));
		genCallEnforced(0x465643, 0x4E7970, reinterpret_cast<DWORD>(&TES3::ItemData::isItemDataStackable));
		genCallEnforced(0x496BF0, 0x4E7970, reinterpret_cast<DWORD>(&TES3::ItemData::isItemDataStackable));
		genCallEnforced(0x497C58, 0x4E7970, reinterpret_cast<DWORD>(&TES3::ItemData::isItemDataStackable));
		genCallEnforced(0x4E162B, 0x4E7970, reinterpret_cast<DWORD>(&TES3::ItemData::isItemDataStackable));
		genCallEnforced(0x52C400, 0x4E7970, reinterpret_cast<DWORD>(&TES3::ItemData::isItemDataStackable));
		genCallEnforced(0x52C6A2, 0x4E7970, reinterpret_cast<DWORD>(&TES3::ItemData::isItemDataStackable));
		genCallEnforced(0x5B4F07, 0x4E7970, reinterpret_cast<DWORD>(&TES3::ItemData::isItemDataStackable));
		genCallEnforced(0x5E1826, 0x4E7970, reinterpret_cast<DWORD>(&TES3::ItemData::isItemDataStackable));
		genCallEnforced(0x60E172, 0x4E7970, reinterpret_cast<DWORD>(&TES3::ItemData::isItemDataStackable));
		genCallEnforced(0x60E566, 0x4E7970, reinterpret_cast<DWORD>(&TES3::ItemData::isItemDataStackable));
		genCallEnforced(0x615789, 0x4E7970, reinterpret_cast<DWORD>(&TES3::ItemData::isItemDataStackable));
		genCallEnforced(0x633A6F, 0x4E7970, reinterpret_cast<DWORD>(&TES3::ItemData::isItemDataStackable));

		// Patch: Use added flags on ItemData to track bound items, instead of matching ids against bound item GMSTs.
		writePatchCodeUnprotected(0x465460, (BYTE*)&PatchCheckIfBoundEquipmentStack, PatchCheckIfBoundEquipmentStack_size);
		genJumpUnprotected(0x465460 + 0xD, 0x46565D);
		genJumpUnprotected(0x465460 + 0x12, 0x46563A);
		writePatchCodeUnprotected(0x496999, (BYTE*)&PatchCheckIfBoundEquipmentStack, PatchCheckIfBoundEquipmentStack_size);
		genJumpUnprotected(0x496999 + 0xD, 0x496CB0);
		genJumpUnprotected(0x496999 + 0x12, 0x496B8A);

		writePatchCodeUnprotected(0x631440, (BYTE*)&PatchCheckIfBoundItemTile, PatchCheckIfBoundItemTile_size);
		genJumpUnprotected(0x631440 + PatchCheckIfBoundItemTile_size, 0x631602);
		writePatchCodeUnprotected(0x63174D, (BYTE*)&PatchCheckIfBoundItemTile, PatchCheckIfBoundItemTile_size);
		genJumpUnprotected(0x63174D + PatchCheckIfBoundItemTile_size, 0x63190F);

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

		// Keyframes load event.
		auto meshDataLoadKeyframes = &TES3::MeshData::loadKeyframes;
		genCallEnforced(0x4C2946, 0x4EE200, *reinterpret_cast<DWORD*>(&meshDataLoadKeyframes));
		genCallEnforced(0x4C2C74, 0x4EE200, *reinterpret_cast<DWORD*>(&meshDataLoadKeyframes));
		genCallEnforced(0x4D25DB, 0x4EE200, *reinterpret_cast<DWORD*>(&meshDataLoadKeyframes));
		genCallEnforced(0x4F086B, 0x4EE200, *reinterpret_cast<DWORD*>(&meshDataLoadKeyframes));

		// Patch: Improved error reporting for mesh loading.
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::Alchemy, 0x134, 0x4F1380, reinterpret_cast<DWORD>(PatchObjectLoadMeshMethod));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::Apparatus, 0x134, 0x4F1380, reinterpret_cast<DWORD>(PatchObjectLoadMeshMethod));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::Armor, 0x134, 0x4F1380, reinterpret_cast<DWORD>(PatchObjectLoadMeshMethod));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::BodyPart, 0x134, 0x4F1380, reinterpret_cast<DWORD>(PatchObjectLoadMeshMethod));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::Book, 0x134, 0x4F1380, reinterpret_cast<DWORD>(PatchObjectLoadMeshMethod));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::Clothing, 0x134, 0x4F1380, reinterpret_cast<DWORD>(PatchObjectLoadMeshMethod));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::Enchantment, 0x134, 0x4F1380, reinterpret_cast<DWORD>(PatchObjectLoadMeshMethod));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::Ingredient, 0x134, 0x4F1380, reinterpret_cast<DWORD>(PatchObjectLoadMeshMethod));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::Lockpick, 0x134, 0x4F1380, reinterpret_cast<DWORD>(PatchObjectLoadMeshMethod));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::Miscellaneous, 0x134, 0x4F1380, reinterpret_cast<DWORD>(PatchObjectLoadMeshMethod));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::Probe, 0x134, 0x4F1380, reinterpret_cast<DWORD>(PatchObjectLoadMeshMethod));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::RepairTool, 0x134, 0x4F1380, reinterpret_cast<DWORD>(PatchObjectLoadMeshMethod));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::Spell, 0x134, 0x4F1380, reinterpret_cast<DWORD>(PatchObjectLoadMeshMethod));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::Static, 0x134, 0x4F1380, reinterpret_cast<DWORD>(PatchObjectLoadMeshMethod));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::Weapon, 0x134, 0x4F1380, reinterpret_cast<DWORD>(PatchObjectLoadMeshMethod));

		overrideVirtualTableEnforced(TES3::VirtualTableAddress::Activator, 0x134, 0x4F0400, reinterpret_cast<DWORD>(PatchAnimatedObjectLoadMeshMethod));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::ContainerBase, 0x134, 0x4F0400, reinterpret_cast<DWORD>(PatchAnimatedObjectLoadMeshMethod));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::CreatureBase, 0x134, 0x4F0400, reinterpret_cast<DWORD>(PatchAnimatedObjectLoadMeshMethod));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::Door, 0x134, 0x4F0400, reinterpret_cast<DWORD>(PatchAnimatedObjectLoadMeshMethod));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::NPCBase, 0x134, 0x4F0400, reinterpret_cast<DWORD>(PatchAnimatedObjectLoadMeshMethod));
		genCallEnforced(0x4D2470, 0x4F0400, reinterpret_cast<DWORD>(PatchAnimatedObjectLoadMeshMethod));

		genCallEnforced(0x4ED826, 0x477400, reinterpret_cast<DWORD>(PatchModelLoaderErrorReport));
		genCallEnforced(0x51AEEE, 0x694460, reinterpret_cast<DWORD>(PatchStreamLoaderErrorReport));

		// Patch: Remove pointless warning when mod makers add a new journal entry after adding a new topic.
		genCallEnforced(0x4BE98C, 0x4B2790, reinterpret_cast<DWORD>(LoadOverDialogue));

		// Event: CrimeWitnessed
		genCallEnforced(0x521DB2, 0x522040, reinterpret_cast<DWORD>(OnProcessCrimes));
		genCallEnforced(0x53184A, 0x51F580, reinterpret_cast<DWORD>(OnCrimeWitnessedEnd));

		// Event: Reference Scene Graph Node Created
		auto referenceGetSceneGraphNode = &TES3::Reference::getSceneGraphNode;
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::Reference, 0x2C, 0x4E81A0, *reinterpret_cast<DWORD*>(&referenceGetSceneGraphNode));

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
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::MobileNPC, 0xE0, 0x54D820, *reinterpret_cast<DWORD*>(&mobileNPCApplyArmorRating)); // MACH
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::MobilePlayer, 0xE0, 0x54D820, *reinterpret_cast<DWORD*>(&mobileNPCApplyArmorRating)); // MACP

		// Events: disarmTrap/pickLock
		auto referenceAttemptUnlockDisarm = &TES3::Reference::attemptUnlockDisarm;
		genCallEnforced(0x569A62, 0x4EB160, *reinterpret_cast<DWORD*>(&referenceAttemptUnlockDisarm));

		// Event: Pickpocket.
		genNOPUnprotected(0x52B1C2, 0x3); // Non-MCP, unreachable in MCP patch #77
		genNOPUnprotected(0x52B215, 0x3);
		genCallEnforced(0x5B56BE, 0x52AFD0, reinterpret_cast<DWORD>(OnPickpocket));
		genCallEnforced(0x5B74B9, 0x52AFD0, reinterpret_cast<DWORD>(OnPickpocketOnExit)); // Non-MCP
		genCallEnforced(0x745976, 0x52AFD0, reinterpret_cast<DWORD>(OnPickpocketOnExit)); // MCP patch #77

		// Event: containerClosed.
		auto actorOnCloseInventory = &TES3::Actor::onCloseInventory;
		auto containerOnCloseInventory = &TES3::ContainerInstance::onCloseInventory;
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::NPCBase, offsetof(TES3::ActorVirtualTable, onCloseInventory), 0x58D230, *reinterpret_cast<DWORD*>(&actorOnCloseInventory));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::NPCInstance, offsetof(TES3::ActorVirtualTable, onCloseInventory), 0x58D230, *reinterpret_cast<DWORD*>(&actorOnCloseInventory));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::CreatureBase, offsetof(TES3::ActorVirtualTable, onCloseInventory), 0x58D230, *reinterpret_cast<DWORD*>(&actorOnCloseInventory));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::CreatureInstance, offsetof(TES3::ActorVirtualTable, onCloseInventory), 0x58D230, *reinterpret_cast<DWORD*>(&actorOnCloseInventory));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::ContainerBase, offsetof(TES3::ActorVirtualTable, onCloseInventory), 0x58D230, *reinterpret_cast<DWORD*>(&actorOnCloseInventory));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::ContainerInstance, offsetof(TES3::ActorVirtualTable, onCloseInventory), 0x4A4460, *reinterpret_cast<DWORD*>(&containerOnCloseInventory));

		// Event: Camera control.
		auto mobilePlayerUpdateScenegraph = &TES3::MobilePlayer::updateSceneGraph;
		auto playerAnimControllerSyncRotation = &TES3::PlayerAnimationController::syncRotation;
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::MobilePlayer, 0x1C, 0x5679E0, *reinterpret_cast<DWORD*>(&mobilePlayerUpdateScenegraph));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::PlayerAnimController, 0xC, 0x5438F0, *reinterpret_cast<DWORD*>(&playerAnimControllerSyncRotation));

		// Allow overriding of guard status.
		auto npcBaseIsGuard = &TES3::NPCBase::isGuard;
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::NPCBase, offsetof(TES3::ActorVirtualTable, isGuard), 0x04DA5E0, *reinterpret_cast<DWORD*>(&npcBaseIsGuard));

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

		// Event: updateBodyPartsForItem
		auto armorSetupBodyParts = &TES3::Armor::setupBodyParts;
		genCallEnforced(0x4DA03A, 0x4A1280, *reinterpret_cast<DWORD*>(&armorSetupBodyParts));
		auto clothingSetupBodyParts = &TES3::Clothing::setupBodyParts;
		genCallEnforced(0x4DA063, 0x4A38F0, *reinterpret_cast<DWORD*>(&clothingSetupBodyParts));

		// Event: removedEquipmentBodyParts
		auto bodyPartManagerRemoveEquippedLayers = &TES3::BodyPartManager::removeEquippedLayers;
		genCallEnforced(0x4D9FC3, 0x472D70, *reinterpret_cast<DWORD*>(&bodyPartManagerRemoveEquippedLayers));

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
		auto dataHandlerAddTemporarySound = &TES3::DataHandler::addTemporarySound;
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

		// Event: Play Item Up/Down sound.
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

		// Event: Play item consumption sound.
		writePatchCodeUnprotected(0x5CF186, reinterpret_cast<BYTE*>(&patchConsumeItemDrinkArgs), patchConsumeItemDrinkArgs_size);
		genCallEnforced(0x5CF1AC, 0x48BCB0, reinterpret_cast<DWORD>(PatchConsumeItemAddSoundById));
		writePatchCodeUnprotected(0x5D1BF1, reinterpret_cast<BYTE*>(&patchConsumeItemSwallowArgs), patchConsumeItemSwallowArgs_size);
		genCallEnforced(0x5D1C2C, 0x48BCB0, reinterpret_cast<DWORD>(PatchConsumeItemAddSoundById));

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
		genCallEnforced(0x5071B5, 0x470AE0, *reinterpret_cast<DWORD*>(&AnimationData_playAnimationGroupForIndex)); // Non-MCP
		genCallEnforced(0x507213, 0x470AE0, *reinterpret_cast<DWORD*>(&AnimationData_playAnimationGroupForIndex)); // Non-MCP
		genCallEnforced(0x524BC8, 0x470AE0, *reinterpret_cast<DWORD*>(&AnimationData_playAnimationGroupForIndex));
		genCallEnforced(0x524BD6, 0x470AE0, *reinterpret_cast<DWORD*>(&AnimationData_playAnimationGroupForIndex));
		genCallEnforced(0x524BE4, 0x470AE0, *reinterpret_cast<DWORD*>(&AnimationData_playAnimationGroupForIndex));
		genCallEnforced(0x540AF3, 0x470AE0, *reinterpret_cast<DWORD*>(&AnimationData_playAnimationGroupForIndex));
		genCallEnforced(0x540B09, 0x470AE0, *reinterpret_cast<DWORD*>(&AnimationData_playAnimationGroupForIndex));
		genCallEnforced(0x540B1E, 0x470AE0, *reinterpret_cast<DWORD*>(&AnimationData_playAnimationGroupForIndex));
		genCallEnforced(0x540EF9, 0x470AE0, *reinterpret_cast<DWORD*>(&AnimationData_playAnimationGroupForIndex));
		genCallEnforced(0x745B89, 0x470AE0, *reinterpret_cast<DWORD*>(&AnimationData_playAnimationGroupForIndex)); // MCP-132 added function
		genCallEnforced(0x745B9E, 0x470AE0, *reinterpret_cast<DWORD*>(&AnimationData_playAnimationGroupForIndex)); // MCP-132 added function
		genCallEnforced(0x745BB3, 0x470AE0, *reinterpret_cast<DWORD*>(&AnimationData_playAnimationGroupForIndex)); // MCP-132 added function

		// Modify actor animation controller to allow blocking animation changes.
		writePatchCodeUnprotected(0x53DAB5, (BYTE*)&patchActorAnimInit, patchActorAnimInit_size);
		genCallUnprotected(0x5428A3, reinterpret_cast<DWORD>(&CheckTogglePOV), 6);
		genCallEnforced(0x53E135, 0x540A90, reinterpret_cast<DWORD>(&ActorAnimController_selectMovementAnimAndUpdate));

		// Make mobile IdleAnim flag reset on Stop key, instead of when loopCount reaches zero.
		genJumpEnforced(0x46DA0D, 0x46E64E, 0x46E49E);
		genJumpUnprotected(0x46E498, 0x46E64E);

		// Override AnimationData creation.
		auto AnimationData_ctor = &TES3::AnimationData::ctor;
		genCallEnforced(0x4E63A5, 0x46B7A0, *reinterpret_cast<DWORD*>(&AnimationData_ctor));
		genCallEnforced(0x4E64E6, 0x46B7A0, *reinterpret_cast<DWORD*>(&AnimationData_ctor));
		genCallEnforced(0x4E6766, 0x46B7A0, *reinterpret_cast<DWORD*>(&AnimationData_ctor));
		genCallEnforced(0x4E69EC, 0x46B7A0, *reinterpret_cast<DWORD*>(&AnimationData_ctor));
		genCallEnforced(0x4E6C5F, 0x46B7A0, *reinterpret_cast<DWORD*>(&AnimationData_ctor));

		// Patch: Allow changing cast animation speed. Custom speed is read and applied on initial cast.
		writePatchCodeUnprotected(0x46CAC0, (BYTE*)&patchApplyAnimationSpeed, patchApplyAnimationSpeed_size);
		genCallUnprotected(0x541B81, reinterpret_cast<DWORD>(&SetAnimSpeedOnCast), 0xA);

		// Event: Power Recharged
		overrideVirtualTableEnforced(0x74AC54, offsetof(PowersHashMap::VirtualTable, deleteKeyValuePair), 0x4F1C50, reinterpret_cast<DWORD>(OnDeletePowerHashMapKVP));

		// Event: Spell magicka cost on cast
		writePatchCodeUnprotected(0x51517A, (BYTE*)&patchSpellCastMagickaRequired, patchSpellCastMagickaRequired_size);
		genCallUnprotected(0x515181, reinterpret_cast<DWORD>(onSpellCastMagickaRequired));

		// Event: Enchanted item charge needed to cast.
		writePatchCodeUnprotected(0x514F1E, (BYTE*)&patchEnchantItemChargeRequired_onCast, patchEnchantItemChargeRequired_onCast_size);
		genCallUnprotected(0x514F2A, reinterpret_cast<DWORD>(onEnchantItemChargeRequired));
		genCallUnprotected(0x5E014A, reinterpret_cast<DWORD>(onEnchantItemChargeRequired_getEnchantment), 6);
		genCallUnprotected(0x5E04EF, reinterpret_cast<DWORD>(onEnchantItemChargeRequired_getEnchantment), 6);
		genCallUnprotected(0x5E1FE3, reinterpret_cast<DWORD>(onEnchantItemChargeRequired_getEnchantment), 6);
		genCallUnprotected(0x5E3264, reinterpret_cast<DWORD>(onEnchantItemChargeRequired_getEnchantment), 6);
		genCallUnprotected(0x5E35F6, reinterpret_cast<DWORD>(onEnchantItemChargeRequired_getEnchantment), 6);
		genCallEnforced(0x5E02F6, 0x72769E, reinterpret_cast<DWORD>(onEnchantItemChargeRequired_ftol));
		genCallEnforced(0x5E05D1, 0x72769E, reinterpret_cast<DWORD>(onEnchantItemChargeRequired_ftol));
		genCallEnforced(0x5E201C, 0x72769E, reinterpret_cast<DWORD>(onEnchantItemChargeRequired_ftol));
		genCallEnforced(0x5E33FB, 0x72769E, reinterpret_cast<DWORD>(onEnchantItemChargeRequired_ftol));
		genCallEnforced(0x5E3724, 0x72769E, reinterpret_cast<DWORD>(onEnchantItemChargeRequired_ftol));

		// Event: Equipment reevaluated.
		auto CreatureInstance_reevaluateEquipment = &TES3::CreatureInstance::reevaluateEquipment;
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::CreatureInstance, offsetof(TES3::ObjectVirtualTable, reevaluateEquipment), 0x49EF60, *reinterpret_cast<DWORD*>(&CreatureInstance_reevaluateEquipment));
		genCallEnforced(0x49D5CB, 0x49EF60, *reinterpret_cast<DWORD*>(&CreatureInstance_reevaluateEquipment));
		auto NPCInstance_reevaluateEquipment = &TES3::NPCInstance::reevaluateEquipment;
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::NPCInstance, offsetof(TES3::ObjectVirtualTable, reevaluateEquipment), 0x4D9A20, *reinterpret_cast<DWORD*>(&NPCInstance_reevaluateEquipment));
		genCallEnforced(0x4D83AB, 0x4D9A20, *reinterpret_cast<DWORD*>(&NPCInstance_reevaluateEquipment));

		// Event: vfxCreated
		auto TES3_VFXManager_createForMagicEffect = &TES3::VFXManager::createForMagicEffect;
		genCallEnforced(0x41CCB0, 0x4696A0, *reinterpret_cast<DWORD*>(&TES3_VFXManager_createForMagicEffect));
		auto TES3_VFXManager_createForSource = &TES3::VFXManager::createForSource;
		genCallEnforced(0x514D32, 0x469570, *reinterpret_cast<DWORD*>(&TES3_VFXManager_createForSource));
		genCallEnforced(0x518B0A, 0x469570, *reinterpret_cast<DWORD*>(&TES3_VFXManager_createForSource));
		auto TES3_VFXManager_createForReference = &TES3::VFXManager::createForReference;
		genCallEnforced(0x516093, 0x468370, *reinterpret_cast<DWORD*>(&TES3_VFXManager_createForReference));
		auto TES3_VFXManager_createAtPosition = &TES3::VFXManager::createAtPosition;
		genCallEnforced(0x5162FD, 0x468470, *reinterpret_cast<DWORD*>(&TES3_VFXManager_createAtPosition));
		auto TES3_VFXManager_createForAVObject = &TES3::VFXManager::createForAVObject;
		genCallEnforced(0x516227, 0x468560, *reinterpret_cast<DWORD*>(&TES3_VFXManager_createForAVObject));

		// Event: consoleReferenceChanged
		genCallEnforced(0x590414, 0x5B2F00, reinterpret_cast<DWORD>(TES3::UI::setConsoleReference));
		genCallEnforced(0x5B2EEE, 0x5B2F00, reinterpret_cast<DWORD>(TES3::UI::setConsoleReference));

		// Event: barterOffer
		genCallEnforced(0x5A70CF, 0x5A66C0, reinterpret_cast<DWORD>(GameBarterOffer));
		genCallEnforced(0x5A6777, reinterpret_cast<DWORD>(OnExerciseSkill), reinterpret_cast<DWORD>(GameBarterOffer_BufferSkillGain));

		// Event: keybindTested
		auto TES3_InputController_testKeybind = &TES3::InputController::keybindTest;
		genCallEnforced(0x41AC14, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x41AC86, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x41ADE4, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x41AEC6, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x41AF9A, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x41B083, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x41B170, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x41B1B0, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x41B1E4, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x41B232, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x41B2D3, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x41CC7E, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x425325, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x5425C8, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x54266F, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x542685, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x568A85, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x568ABB, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x568AEC, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x568B02, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x568B16, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x568B2C, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x568B71, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x568BAD, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x568C46, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x568CDF, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x568D79, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x568E35, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x568F16, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x568F60, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x568FA9, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x569005, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x56927F, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x569538, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x56965F, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x5697BE, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x56985A, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x5699A4, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x5699BA, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x569E2B, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x569E40, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x569EC9, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x569FC3, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x5B7B49, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind));
		genCallEnforced(0x745AA8, 0x406F40, *reinterpret_cast<DWORD*>(&TES3_InputController_testKeybind)); // MCP-89: Vanity keyboard lock.

		// Event: Disposition
		auto TES3_NPCInstance_getDisposition = &TES3::NPCInstance::getDisposition;
		genCallEnforced(0x4D5D26, 0x4DA330, *reinterpret_cast<DWORD*>(&TES3_NPCInstance_getDisposition));
		genCallEnforced(0x4D5D3D, 0x4DA330, *reinterpret_cast<DWORD*>(&TES3_NPCInstance_getDisposition));
		genCallEnforced(0x4D5D58, 0x4DA330, *reinterpret_cast<DWORD*>(&TES3_NPCInstance_getDisposition));
		genCallEnforced(0x4D5D75, 0x4DA330, *reinterpret_cast<DWORD*>(&TES3_NPCInstance_getDisposition));
		genCallEnforced(0x4D5D8F, 0x4DA330, *reinterpret_cast<DWORD*>(&TES3_NPCInstance_getDisposition));
		genCallEnforced(0x4D5DB4, 0x4DA330, *reinterpret_cast<DWORD*>(&TES3_NPCInstance_getDisposition));
		genCallEnforced(0x4D5DCD, 0x4DA330, *reinterpret_cast<DWORD*>(&TES3_NPCInstance_getDisposition));
		genCallEnforced(0x5077EA, 0x4DA330, *reinterpret_cast<DWORD*>(&TES3_NPCInstance_getDisposition));
		genCallEnforced(0x54DC88, 0x4DA330, *reinterpret_cast<DWORD*>(&TES3_NPCInstance_getDisposition));

		// Event: dialogueFiltered
		genCallEnforced(0x6004E9, 0x4B29E0, reinterpret_cast<DWORD>(PatchDialogueFilteredFor<TES3::Dialogue::GetFilteredInfoContext::Persuasion>)); // Persuasion
		genCallEnforced(0x5C0A67, 0x4B29E0, reinterpret_cast<DWORD>(PatchDialogueFilteredFor<TES3::Dialogue::GetFilteredInfoContext::ClickTopicFallback>)); // Click topic fallback
		genCallEnforced(0x5C0A48, 0x4B29E0, reinterpret_cast<DWORD>(PatchDialogueFilteredFor<TES3::Dialogue::GetFilteredInfoContext::ClickTopic>)); // Click topic
		genCallEnforced(0x5C05F7, 0x4B29E0, reinterpret_cast<DWORD>(PatchDialogueFilteredFor<TES3::Dialogue::GetFilteredInfoContext::ClickAnswer>)); // Click answer
		genCallEnforced(0x5BF62C, 0x4B29E0, reinterpret_cast<DWORD>(PatchDialogueFilteredFor<TES3::Dialogue::GetFilteredInfoContext::ServiceEnchanting>)); // Service: Enchanting
		genCallEnforced(0x5BF51C, 0x4B29E0, reinterpret_cast<DWORD>(PatchDialogueFilteredFor<TES3::Dialogue::GetFilteredInfoContext::ServiceSpellmaking>)); // Service: Spellmaking
		genCallEnforced(0x5BF43C, 0x4B29E0, reinterpret_cast<DWORD>(PatchDialogueFilteredFor<TES3::Dialogue::GetFilteredInfoContext::ServiceTravel>)); // Service: Travel
		genCallEnforced(0x5BF33C, 0x4B29E0, reinterpret_cast<DWORD>(PatchDialogueFilteredFor<TES3::Dialogue::GetFilteredInfoContext::ServiceTraining>)); // Service: Training
		genCallEnforced(0x5BF25C, 0x4B29E0, reinterpret_cast<DWORD>(PatchDialogueFilteredFor<TES3::Dialogue::GetFilteredInfoContext::ServiceSpells>)); // Service: Spells
		genCallEnforced(0x5BF17C, 0x4B29E0, reinterpret_cast<DWORD>(PatchDialogueFilteredFor<TES3::Dialogue::GetFilteredInfoContext::ServiceRepair>)); // Service: Repair
		genCallEnforced(0x5BF01C, 0x4B29E0, reinterpret_cast<DWORD>(PatchDialogueFilteredFor<TES3::Dialogue::GetFilteredInfoContext::ServiceBarter>)); // Service: Barter
		genCallEnforced(0x52931A, 0x4B29E0, reinterpret_cast<DWORD>(PatchDialogueFilteredFor<TES3::Dialogue::GetFilteredInfoContext::Greeting>)); // Greeting
		genCallEnforced(0x5290B2, 0x4B29E0, reinterpret_cast<DWORD>(PatchDialogueFilteredFor<TES3::Dialogue::GetFilteredInfoContext::Voice>)); // Say voice
		genCallEnforced(0x4B2F51, 0x4B29E0, reinterpret_cast<DWORD>(PatchDialogueFilteredFor<TES3::Dialogue::GetFilteredInfoContext::TopicPopulation>)); // Topic population
		genCallEnforced(0x40B8EE, 0x4B29E0, reinterpret_cast<DWORD>(PatchDialogueFilteredFor<TES3::Dialogue::GetFilteredInfoContext::HyperlinkParser>)); // Font calculation

		// Event: Repair
		genCallEnforced(0x59A3AE, 0x60E260, reinterpret_cast<DWORD>(OnClickRepairOrRecharge));
		genPushEnforced(0x60DAF6, reinterpret_cast<DWORD>(OnClickRepairOrRecharge));
		genCallEnforced(0x60E37C, 0x60ECD0, reinterpret_cast<DWORD>(AttemptRepair));

		// Event: CalcChargenStats
		genCallEnforced(0x5A901B, 0x5A9150, reinterpret_cast<DWORD>(CalcChargenStats));
		genCallEnforced(0x5A931E, 0x5A9150, reinterpret_cast<DWORD>(CalcChargenStats));
		genCallEnforced(0x5B164D, 0x5A9150, reinterpret_cast<DWORD>(CalcChargenStats));
		// Special call to raise an event when at the last stage of character creation (caused by script call to EnableStatReviewMenu)
		genCallEnforced(0x50B940, 0x62CE60, reinterpret_cast<DWORD>(ShowStatReviewMenu));

		// Event: magicSelectionChanged
		genCallEnforced(0x49B110, 0x5F4E70, reinterpret_cast<DWORD>(TES3::UI::updateCurrentMagicFromSpell));
		genCallEnforced(0x56DC7C, 0x5F4E70, reinterpret_cast<DWORD>(TES3::UI::updateCurrentMagicFromSpell));
		genCallEnforced(0x5DFF81, 0x5F4E70, reinterpret_cast<DWORD>(TES3::UI::updateCurrentMagicFromSpell));
		genCallEnforced(0x5E19D9, 0x5F4E70, reinterpret_cast<DWORD>(TES3::UI::updateCurrentMagicFromSpell));
		genCallEnforced(0x5E23C4, 0x5F4E70, reinterpret_cast<DWORD>(TES3::UI::updateCurrentMagicFromSpell));
		genCallEnforced(0x5E426E, 0x5F4E70, reinterpret_cast<DWORD>(TES3::UI::updateCurrentMagicFromSpell));
		genCallEnforced(0x5E4569, 0x5F4E70, reinterpret_cast<DWORD>(TES3::UI::updateCurrentMagicFromSpell));
		genCallEnforced(0x5E6264, 0x5F4E70, reinterpret_cast<DWORD>(TES3::UI::updateCurrentMagicFromSpell));
		genCallEnforced(0x5E234B, 0x5F4DB0, reinterpret_cast<DWORD>(TES3::UI::updateCurrentMagicFromEquipmentStack));
		genCallEnforced(0x5E444C, 0x5F4DB0, reinterpret_cast<DWORD>(TES3::UI::updateCurrentMagicFromEquipmentStack));

		// Event: objectCopied
		OverwriteCopyObjectVirtualCall<TES3::Activator, 0x49FD20>(TES3::VirtualTableAddress::Activator);
		OverwriteCopyObjectVirtualCall<TES3::Alchemy, 0x4AC450>(TES3::VirtualTableAddress::Alchemy);
		OverwriteCopyObjectVirtualCall<TES3::Apparatus, 0x4A01E0>(TES3::VirtualTableAddress::Apparatus);
		OverwriteCopyObjectVirtualCall<TES3::Armor, 0x4A0BF0>(TES3::VirtualTableAddress::Armor);
		OverwriteCopyObjectVirtualCall<TES3::BodyPart, 0x4A1710>(TES3::VirtualTableAddress::BodyPart);
		OverwriteCopyObjectVirtualCall<TES3::Book, 0x4A24E0>(TES3::VirtualTableAddress::Book);
		OverwriteCopyObjectVirtualCall<TES3::Clothing, 0x4A3590>(TES3::VirtualTableAddress::Clothing);
		OverwriteCopyObjectVirtualCall<TES3::ContainerBase, 0x4A4070>(TES3::VirtualTableAddress::ContainerBase);
		OverwriteCopyObjectVirtualCall<TES3::ContainerInstance, 0x4A5250>(TES3::VirtualTableAddress::ContainerInstance);
		OverwriteCopyObjectVirtualCall<TES3::CreatureBase, 0x49D1B0>(TES3::VirtualTableAddress::CreatureBase);
		OverwriteCopyObjectVirtualCall<TES3::CreatureInstance, 0x49E8B0>(TES3::VirtualTableAddress::CreatureInstance);
		OverwriteCopyObjectVirtualCall<TES3::Door, 0x4A56B0>(TES3::VirtualTableAddress::Door);
		OverwriteCopyObjectVirtualCall<TES3::Enchantment, 0x4AB530>(TES3::VirtualTableAddress::Enchantment);
		OverwriteCopyObjectVirtualCall<TES3::Ingredient, 0x4A5C70>(TES3::VirtualTableAddress::Ingredient);
		OverwriteCopyObjectVirtualCall<TES3::LeveledCreature, 0x4CF5C0>(TES3::VirtualTableAddress::LeveledCreature);
		OverwriteCopyObjectVirtualCall<TES3::LeveledItem, 0x4D0920>(TES3::VirtualTableAddress::LeveledItem);
		OverwriteCopyObjectVirtualCall<TES3::Light, 0x4D2120>(TES3::VirtualTableAddress::Light);
		OverwriteCopyObjectVirtualCall<TES3::Lockpick, 0x4A61B0>(TES3::VirtualTableAddress::Lockpick);
		OverwriteCopyObjectVirtualCall<TES3::Misc, 0x4A67A0>(TES3::VirtualTableAddress::Miscellaneous);
		OverwriteCopyObjectVirtualCall<TES3::NPCBase, 0x4D7340>(TES3::VirtualTableAddress::NPCBase);
		OverwriteCopyObjectVirtualCall<TES3::NPCInstance, 0x4D9870>(TES3::VirtualTableAddress::NPCInstance);
		OverwriteCopyObjectVirtualCall<TES3::Probe, 0x4A6C70>(TES3::VirtualTableAddress::Probe);
		OverwriteCopyObjectVirtualCall<TES3::RepairTool, 0x4A7160>(TES3::VirtualTableAddress::RepairTool);
		OverwriteCopyObjectVirtualCall<TES3::Spell, 0x4AA7B0>(TES3::VirtualTableAddress::Spell);
		OverwriteCopyObjectVirtualCall<TES3::Static, 0x4A74D0>(TES3::VirtualTableAddress::Static);
		OverwriteCopyObjectVirtualCall<TES3::Weapon, 0x4F2600>(TES3::VirtualTableAddress::Weapon);

		// Event: objectCreated
		genCallEnforced(0x49711F, 0x481D80, reinterpret_cast<DWORD>(&AddObjectToHashMapById));
		genCallEnforced(0x4A427A, 0x481D80, reinterpret_cast<DWORD>(&AddObjectToHashMapById));
		genCallEnforced(0x4B8B07, 0x481D80, reinterpret_cast<DWORD>(&AddObjectToHashMapById));
		genCallEnforced(0x4BA922, 0x481D80, reinterpret_cast<DWORD>(&AddObjectToHashMapById));
		genCallEnforced(0x4BEA1B, 0x481D80, reinterpret_cast<DWORD>(&AddObjectToHashMapById));
		genCallEnforced(0x4BEBC6, 0x481D80, reinterpret_cast<DWORD>(&AddObjectToHashMapById));
		genCallEnforced(0x4C0CE5, 0x481D80, reinterpret_cast<DWORD>(&AddObjectToHashMapById));
		genCallEnforced(0x4C1CDF, 0x481D80, reinterpret_cast<DWORD>(&AddObjectToHashMapById));
		genCallEnforced(0x4C1D8D, 0x481D80, reinterpret_cast<DWORD>(&AddObjectToHashMapById));
		genCallEnforced(0x4C1E36, 0x481D80, reinterpret_cast<DWORD>(&AddObjectToHashMapById));
		genCallEnforced(0x4C1F01, 0x481D80, reinterpret_cast<DWORD>(&AddObjectToHashMapById));
		genCallEnforced(0x4C1FAB, 0x481D80, reinterpret_cast<DWORD>(&AddObjectToHashMapById));
		genCallEnforced(0x4C2055, 0x481D80, reinterpret_cast<DWORD>(&AddObjectToHashMapById));
		genCallEnforced(0x4C2160, 0x481D80, reinterpret_cast<DWORD>(&AddObjectToHashMapById));
		genCallEnforced(0x4C2249, 0x481D80, reinterpret_cast<DWORD>(&AddObjectToHashMapById));
		genCallEnforced(0x4C2320, 0x481D80, reinterpret_cast<DWORD>(&AddObjectToHashMapById));
		genCallEnforced(0x4C2406, 0x481D80, reinterpret_cast<DWORD>(&AddObjectToHashMapById));
		genCallEnforced(0x4C24D0, 0x481D80, reinterpret_cast<DWORD>(&AddObjectToHashMapById));
		genCallEnforced(0x4C259A, 0x481D80, reinterpret_cast<DWORD>(&AddObjectToHashMapById));
		genCallEnforced(0x4C2664, 0x481D80, reinterpret_cast<DWORD>(&AddObjectToHashMapById));

		// Event: startGlobalScript
		auto startGlobalScript = &TES3::WorldController::startGlobalScript;
		genCallEnforced(0x419AE7, 0x40FA80, *reinterpret_cast<DWORD*>(&startGlobalScript));
		genCallEnforced(0x419B2B, 0x40FA80, *reinterpret_cast<DWORD*>(&startGlobalScript));
		genCallEnforced(0x4C4E11, 0x40FA80, *reinterpret_cast<DWORD*>(&startGlobalScript));
		genCallEnforced(0x4C4E54, 0x40FA80, *reinterpret_cast<DWORD*>(&startGlobalScript));
		genCallEnforced(0x4C5425, 0x40FA80, *reinterpret_cast<DWORD*>(&startGlobalScript));
		genCallEnforced(0x4C5468, 0x40FA80, *reinterpret_cast<DWORD*>(&startGlobalScript));
		genCallEnforced(0x5073F7, 0x40FA80, *reinterpret_cast<DWORD*>(&startGlobalScript));
		genCallEnforced(0x5FB1A9, 0x40FA80, *reinterpret_cast<DWORD*>(&startGlobalScript));
		genCallEnforced(0x5FB1EB, 0x40FA80, *reinterpret_cast<DWORD*>(&startGlobalScript));
		genCallEnforced(0x5FE94D, 0x40FA80, *reinterpret_cast<DWORD*>(&startGlobalScript));
		genCallEnforced(0x5FE991, 0x40FA80, *reinterpret_cast<DWORD*>(&startGlobalScript));
		auto startGlobalScriptBySourceID = &TES3::WorldController::startGlobalScriptBySourceID;
		genCallEnforced(0x4FF826, 0x40FA80, *reinterpret_cast<DWORD*>(&startGlobalScriptBySourceID));

		// Event: topicsListUpdated
		mwse::genCallEnforced(0x5C03A6, 0x5BE6C0, reinterpret_cast<DWORD>(TES3::UI::updateTopicsList));
		mwse::genCallEnforced(0x5C06D0, 0x5BE6C0, reinterpret_cast<DWORD>(TES3::UI::updateTopicsList));
		mwse::genCallEnforced(0x5C0AF6, 0x5BE6C0, reinterpret_cast<DWORD>(TES3::UI::updateTopicsList));

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
		
		// Also clean up references, but do it just before deletion so we have more information.
		auto referenceObjectDestructor = &TES3::Reference::dtor;
		genJumpEnforced(0x49A675, 0x4E45C0, *reinterpret_cast<DWORD*>(&referenceObjectDestructor));
		genCallEnforced(0x4DE528, 0x4E45C0, *reinterpret_cast<DWORD*>(&referenceObjectDestructor));
		genCallEnforced(0x4E45A3, 0x4E45C0, *reinterpret_cast<DWORD*>(&referenceObjectDestructor));
		genJumpEnforced(0x72D1B4, 0x4E45C0, *reinterpret_cast<DWORD*>(&referenceObjectDestructor));

		// Allow area-of-effect magic effects and tes3.findActorsInProximity to hit/find incapacitated actors.
		genJumpUnprotected(0x570341, 0x57034C);

		// Patch custom magic effect saving and loading.
		genJumpUnprotected(TES3_PATCH_MAGIC_SAVE_LOAD, reinterpret_cast<DWORD>(PatchMagicSaveLoad), TES3_PATCH_MAGIC_SAVE_LOAD_SIZE);

		// Allow setting names of apparatus, lockpicks, probes, and repair tools.
		auto TES3_Apparatus_setName = &TES3::Apparatus::setName;
		auto TES3_Lockpick_setName = &TES3::Lockpick::setName;
		auto TES3_Probe_setName = &TES3::Probe::setName;
		auto TES3_RepairTool_setName = &TES3::RepairTool::setName;
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::Apparatus, offsetof(TES3::ObjectVirtualTable, setName), 0x4F1C50, *reinterpret_cast<DWORD*>(&TES3_Apparatus_setName));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::Lockpick, offsetof(TES3::ObjectVirtualTable, setName), 0x4F1C50, *reinterpret_cast<DWORD*>(&TES3_Lockpick_setName));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::Probe, offsetof(TES3::ObjectVirtualTable, setName), 0x4F1C50, *reinterpret_cast<DWORD*>(&TES3_Probe_setName));
		overrideVirtualTableEnforced(TES3::VirtualTableAddress::RepairTool, offsetof(TES3::ObjectVirtualTable, setName), 0x4F1C50, *reinterpret_cast<DWORD*>(&TES3_RepairTool_setName));

		// Allow changing simulation time scalar.
		genCallEnforced(0x40F62B, 0x453630, reinterpret_cast<DWORD>(OnUpdateDeltaTime));

		// Support for MobileSpellProjectile::explode.
		// Initialize extra flag members in MobileProjectile::ctor.
		writeByteUnprotected(0x5723BC, 0x89);
		// The explode logic is deferred to execute at the same point as projectile simulation to preserve consistency.
		auto projectileManagerResolveCollisions = &TES3::ProjectileManager::resolveCollisions;
		genCallEnforced(0x5638F8, 0x5753A0, *reinterpret_cast<DWORD*>(&projectileManagerResolveCollisions));

		// Warn about MGE being disabled.
		if (!InstructionStore::getInstance().isOpcode(OpCode::xGetGS)) {
			log::getLog() << "WARNING: MGE XE is flagged as disabled. Some mods may have unintended behavior." << std::endl;
		}

		// Look for main.lua scripts in the usual directories.
		gatherMainModScripts("Data Files\\MWSE\\core", true);
		gatherMainModScripts("Data Files\\MWSE\\mods", false);

		// Temporary backwards compatibility for old-style MWSE mods.
		if (Configuration::EnableLegacyLuaMods) {
			gatherMainModScripts("Data Files\\MWSE\\lua", false, "mod_init.lua");
		}

		// Gather any metadata for the mods.
		gatherModMetadata();

		// Finally execute the scripts.
		executeMainModScripts();
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
		const auto stateHandle = getThreadSafeStateHandle();

		sol::object response = event::trigger(baseEvent->getEventName(), baseEvent->createEventTable(), baseEvent->getEventOptions());
		delete baseEvent;
		return response;
	}

	void LuaManager::setButtonPressedCallback(sol::optional<sol::protected_function> callback) {
		buttonPressedCallback = callback.value_or(sol::nil);
	}

	void LuaManager::triggerButtonPressed() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		if (buttonPressedCallback != sol::nil) {
			sol::protected_function callback = buttonPressedCallback;
			buttonPressedCallback = sol::nil;
			auto& state = stateHandle.getState();
			auto eventData = state.create_table();
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
		realTimers->incrementClock(TES3::WorldController::realDeltaTime);
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
		for (auto& controller : timers) {
			for (auto& timer : controller->m_ActiveTimers) {
				auto t = timer->toTable(luaState.lua_state());
				if (t != sol::nil) {
					list.add(t);
				}
			}
			for (auto& timer : controller->m_PausedTimers) {
				auto t = timer->toTable(luaState.lua_state());
				if (t != sol::nil) {
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
}