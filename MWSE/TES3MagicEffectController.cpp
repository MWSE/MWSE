#include "TES3MagicEffectController.h"

#include "MemoryUtil.h"

#include "TES3DataHandler.h"
#include "TES3GameFile.h"
#include "TES3GameSetting.h"
#include "TES3MagicEffect.h"
#include "TES3MagicEffectInstance.h"
#include "TES3MagicSourceInstance.h"
#include "TES3MobilePlayer.h"
#include "TES3NPC.h"
#include "TES3WorldController.h"

#include "TES3UIElement.h"
#include "TES3UIManager.h"
#include "TES3UIWidgets.h"

#include "LuaManager.h"
#include "LuaSpellTickEvent.h"
#include "LuaUtil.h"

#include "Log.h"

#include "BitUtil.h"

namespace TES3 {
	unsigned int MagicEffectController::effectNameGMSTs[MAX_EFFECT_COUNT] = {};
	unsigned int MagicEffectController::effectFlags[MAX_EFFECT_COUNT] = {};
	unsigned int MagicEffectController::effectCounters[MAX_EFFECT_COUNT][5] = {};

	MagicEffectController::MagicEffectController() {

	}

	MagicEffectController::~MagicEffectController() {
		for (auto itt : effectObjects) {
			delete itt.second;
		}
	}

	static MagicEffect* InvalidMagicEffect = nullptr;
	static std::unordered_set<int> warnedMagicEffectIds;

	MagicEffect* MagicEffectController::getEffectObject(int id) {
		if (id == -1) {
			return nullptr;
		}

		auto itt = effectObjects.find(id);
		if (itt == effectObjects.end()) {
			// Warn only once per effect ID.
			if (warnedMagicEffectIds.find(id) == warnedMagicEffectIds.end()) {
				mwse::log::getLog() << "Invalid effect ID " << id << " encountered. Spoofing substitute. Uninstalling mods that add custom effects can lead to unexpected behavior and crashes." << std::endl;
				warnedMagicEffectIds.insert(id);
			}
			return InvalidMagicEffect;
		}

		return itt->second;
	}

	void MagicEffectController::addEffectObject(MagicEffect* effect) {
		effectObjects[effect->id] = effect;
	}

	bool MagicEffectController::getEffectExists(int id) {
		return effectObjects.find(id) != effectObjects.end();
	}

	const char * MagicEffectController::getEffectName(int id) {
		if (id <= EffectID::LastEffect) {
			return DataHandler::get()->nonDynamicData->GMSTs[effectNameGMSTs[id]]->value.asString;
		}

		auto itt = effectCustomNames.find(id);
		if (itt == effectCustomNames.end()) {
			return nullptr;
		}

		return itt->second.c_str();
	}

	unsigned int MagicEffectController::getEffectFlags(int id) {
		return effectFlags[id];
	}

	void MagicEffectController::setEffectFlags(int id, unsigned int flags) {
		effectFlags[id] = flags;
	}

	bool MagicEffectController::getEffectFlag(int id, EffectFlag::FlagBit flag) {
		return BIT_TEST(effectFlags[id], flag);
	}

	void MagicEffectController::setEffectFlag(int id, EffectFlag::FlagBit flag, bool value) {
		BIT_SET(effectFlags[id], flag, value);
	}

	//
	// Functionality related to overwriting how Morrowind.exe deals with effects.
	//

	void __fastcall InitializeController(NonDynamicData * ndd) {
		auto magicEffectController = new MagicEffectController();
		ndd->magicEffects = magicEffectController;

#if MWSE_RAISED_FILE_LIMIT
		memset(ndd->activeMods, 0x0, sizeof(ndd->activeMods));
#endif
		memset(ndd->freed_0x5CC, 0x0, sizeof(ndd->freed_0x5CC));

		for (int i = EffectID::FirstEffect; i <= EffectID::LastEffect; i++) {
			auto effect = new MagicEffect(i);
			magicEffectController->addEffectObject(effect);
		}

		// Add an invalid effect for catching crashes.
		InvalidMagicEffect = new MagicEffect(EFFECT_ID_INVALID);
		InvalidMagicEffect->setDescription("This effect is invalid. This typically happens when uninstalling mods.");
		magicEffectController->effectCustomNames[EFFECT_ID_INVALID] = "Invalid Effect";
		magicEffectController->effectNameGMSTs[EFFECT_ID_INVALID] = -EFFECT_ID_INVALID;
		magicEffectController->setEffectFlags(EFFECT_ID_INVALID, 0U);
		InvalidMagicEffect->flags = 0;
		strcpy_s(InvalidMagicEffect->particleTexture, "vfx_default.tga");
		strcpy_s(InvalidMagicEffect->icon, "s\\Tx_S_sEfft_Unusd02.tga");
		strcpy_s(InvalidMagicEffect->castSoundEffect, "Conjuration Cast");
		strcpy_s(InvalidMagicEffect->boltSoundEffect, "Conjuration Bolt");
		strcpy_s(InvalidMagicEffect->hitSoundEffect, "Conjuration Hit");
		strcpy_s(InvalidMagicEffect->areaSoundEffect, "Conjuration Area");
		InvalidMagicEffect->speed = 1.0f;
		InvalidMagicEffect->size = 1.0f;
		InvalidMagicEffect->sizeCap = 50.0f;
		InvalidMagicEffect->lightingRed = 192;
		InvalidMagicEffect->lightingGreen = 192;
		InvalidMagicEffect->lightingBlue = 192;
	}

	void __stdcall DestroyController() {
		delete DataHandler::get()->nonDynamicData->magicEffects;
	}

	void __stdcall ResolveAllLinks() {
		auto nonDynamicData = DataHandler::get()->nonDynamicData;
		auto controller = nonDynamicData->magicEffects;
		for (auto itt : controller->effectObjects) {
			itt.second->resolveLinks(nonDynamicData);
		}

		// Finish up the invalid effect.
		InvalidMagicEffect->castEffect = static_cast<PhysicalObject*>(nonDynamicData->resolveObject("VFX_DefaultCast"));
		InvalidMagicEffect->boltEffect = static_cast<PhysicalObject*>(nonDynamicData->resolveObject("VFX_DefaultBolt"));
		InvalidMagicEffect->hitEffect = static_cast<PhysicalObject*>(nonDynamicData->resolveObject("VFX_DefaultHit"));
		InvalidMagicEffect->areaEffect = static_cast<PhysicalObject*>(nonDynamicData->resolveObject("VFX_DefaultArea"));

		mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::GenericEvent("magicEffectsResolved"));
	}

	MagicEffect* _fastcall getMagicEffectData(NonDynamicData* ndd, DWORD _UNUSED_, int effectId) {
		return ndd->magicEffects->getEffectObject(effectId);
	}

	__declspec(naked) void patchInitializeMEC() {
		__asm {
			mov ecx, esi	// Size: 0x2
			nop				// Replaced with a call generation. Can't do so here, because offsets aren't accurate.
			nop				// ^
			nop				// ^
			nop				// ^
			nop				// ^
		}
	}
	const size_t patchInitializeMEC_size = 0x7;

	const auto TES3_MagicEffect_copyFromLoading = reinterpret_cast<void(__thiscall*)(MagicEffect*, MagicEffect*)>(0x4A9990);
	void __fastcall ReadMagicEffectIdForLoading(GameFile * sourceMod, DWORD EDX, int& effectId, int size) {
		auto nonDynamicData = DataHandler::get()->nonDynamicData;
		auto controller = nonDynamicData->magicEffects;

		// Call overwritten code.
		sourceMod->readChunkData(&effectId, size);

		// Vanilla effect loading.
		if (!(sourceMod->flags_4D8 & 1)) {
			MagicEffect tempEffect;

			bool wasFlag1Set = BIT_TEST(tempEffect.objectFlags, 0);
			tempEffect.vTable.base->loadObjectSpecific(&tempEffect, sourceMod);
			tempEffect.sourceMod = sourceMod;

			if (wasFlag1Set) {
				BIT_SET_ON(tempEffect.objectFlags, 0);
			}
			else {
				BIT_SET(tempEffect.objectFlags, 0, BIT_TEST(sourceMod->flags_4D8, 0));
			}

			if (sourceMod->flags_4D8 & 0x8) {
				tempEffect.setObjectModified(true);
			}

			auto effect = controller->getEffectObject(effectId);
			if (effect == nullptr) {
				effect = new MagicEffect(effectId);
				controller->addEffectObject(effect);
			}

			TES3_MagicEffect_copyFromLoading(effect, &tempEffect);

			if (!(sourceMod->flags_4D8 & 0x20)) {
				effect->sourceMod = sourceMod;
				BIT_SET(effect->objectFlags, ObjectFlag::DeleteBit, BIT_TEST(sourceMod->flags_4D8, ObjectFlag::DeleteBit));
			}

			if (sourceMod->flags_4D8 & 0x8) {
				effect->setObjectModified(true);
			}
		}
		else {
			auto effect = controller->getEffectObject(effectId);
			if (effect == nullptr) {
				effect = new MagicEffect(effectId);
				controller->addEffectObject(effect);
			}

			// 
			effect->clearData();
			BIT_SET_OFF(effect->objectFlags, ObjectFlag::DeleteBit);

			// 
			bool wasFlag1Set = BIT_TEST(effect->objectFlags, 0);
			effect->vTable.base->loadObjectSpecific(effect, sourceMod);
			effect->sourceMod = sourceMod;

			if (wasFlag1Set) {
				BIT_SET_ON(effect->objectFlags, 0);
			}
			else {
				BIT_SET(effect->objectFlags, 0, BIT_TEST(sourceMod->flags_4D8, ObjectFlag::ModifiedBit));
			}

			if (sourceMod->flags_4D8 & 0x8) {
				effect->setObjectModified(true);
			}

			effect->id = effectId;
		}
	}

	const auto TES3_TriggerSpellSummonEvent = reinterpret_cast<void(__cdecl *)(MagicSourceInstance *, float, MagicEffectInstance *, int, const char*)>(0x466950);
	void triggerSpellSummonEvent(sol::table data, std::string id) {
		MagicSourceInstance * sourceInstance = data["sourceInstance"];
		float deltaTime = data["deltaTime"];
		MagicEffectInstance * effectInstance = data["effectInstance"];
		int effectIndex = data["effectIndex"];

		TES3_TriggerSpellSummonEvent(sourceInstance, deltaTime, effectInstance, effectIndex, id.c_str());
	}

	const auto TES3_TriggerSpellBoundArmorEvent = reinterpret_cast<void(__cdecl *)(MagicSourceInstance *, float, MagicEffectInstance *, int, const char*, const char*)>(0x466270);
	void triggerSpellBoundArmorEvent(sol::table data, std::string id, sol::optional<std::string> id2) {
		MagicSourceInstance * sourceInstance = data["sourceInstance"];
		float deltaTime = data["deltaTime"];
		MagicEffectInstance * effectInstance = data["effectInstance"];
		int effectIndex = data["effectIndex"];

		const char * id2cs = nullptr;
		if (id2) {
			id2cs = id2.value().c_str();
		}

		TES3_TriggerSpellBoundArmorEvent(sourceInstance, deltaTime, effectInstance, effectIndex, id.c_str(), id2cs);
	}

	const auto TES3_TriggerSpellBoundWeaponEvent = reinterpret_cast<void(__cdecl *)(MagicSourceInstance *, float, MagicEffectInstance *, int, const char*)>(0x465B70);
	void triggerSpellBoundWeaponEvent(sol::table data, std::string& id) {
		MagicSourceInstance * sourceInstance = data["sourceInstance"];
		float deltaTime = data["deltaTime"];
		MagicEffectInstance * effectInstance = data["effectInstance"];
		int effectIndex = data["effectIndex"];

		TES3_TriggerSpellBoundWeaponEvent(sourceInstance, deltaTime, effectInstance, effectIndex, id.c_str());
	}

	sol::protected_function genericLuaSpellResistCallback = sol::nil;
	bool __cdecl genericLuaSpellResist(MagicSourceInstance * sourceInstance, MagicEffectInstance * effectInstance, int effectIndex) {
		if (genericLuaSpellResistCallback != sol::nil) {
			auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			sol::optional<bool> result = genericLuaSpellResistCallback(stateHandle.state.create_table_with(
				"sourceInstance", sourceInstance,
				"effectInstance", effectInstance,
				"effectIndex", effectIndex
			));
			return result.value_or(false);
		}

		return false;
	}

	const auto TES3_TriggerSpellEffectEvent = reinterpret_cast<bool(__cdecl *)(MagicSourceInstance *, float, MagicEffectInstance *, int, bool, bool, void *, DWORD, unsigned int, bool(__cdecl *)(MagicSourceInstance *, MagicEffectInstance *, int))>(0x518460);
	std::tuple<bool, sol::object> triggerSpellEffectEvent(sol::table self, sol::optional<sol::table> maybe_data, sol::this_state s) {
		sol::state_view state = s;

		// Get the important data from the table/effect info.
		int effectId = self["effectId"];
		MagicSourceInstance * sourceInstance = self["sourceInstance"];
		float deltaTime = self["deltaTime"];
		MagicEffectInstance * effectInstance = self["effectInstance"];
		int effectIndex = self["effectIndex"];

		// Get other params from a custom table.
		sol::table data = maybe_data.value_or(state.create_table());
		bool negateOnExpiry = data.get_or("negateOnExpiry", true);
		bool isUncapped = data.get_or("isUncapped", (MagicEffectController::effectFlags[effectId] >> 12) & 0xFFFFFF01);
		unsigned int attribute = data.get_or("attribute", (unsigned int)MagicEffectAttribute::NonResistable);

		// Provide values to modify.
		union {
			int asInt;
			float asFloat;
		} genericEventValue;
		genericEventValue.asInt = 0;
		Statistic * statisticEventValue = nullptr;
		void * eventValue = nullptr;

		// Figure out what kind of return value we're looking for.
		DWORD eventType = 0;
		int eventTypeEnum = data.get_or("type", 0);
		switch (eventTypeEnum) {
		case 0:
		case 1:
			eventType = 0x7886F0;
			genericEventValue.asInt = data.get_or("value", 0);
			eventValue = &genericEventValue;
			break;
		case 2:
			eventType = 0x788700;
			genericEventValue.asFloat = data.get_or("value", 0.0f);
			eventValue = &genericEventValue;
			break;
		case 3:
			eventType = 0x788760;
			statisticEventValue = data["value"];
			eventValue = statisticEventValue;
			break;
		}

		// Allow defining a custom resist function.
		bool(__cdecl * resistFunction)(MagicSourceInstance *, MagicEffectInstance *, int) = nullptr;
		sol::optional<sol::protected_function> resistLuaFunction = data["resistanceCheck"];
		if (resistLuaFunction) {
			genericLuaSpellResistCallback = resistLuaFunction.value();
			resistFunction = genericLuaSpellResist;
		}

		// Run the actual event trigger.
		bool eventResult = TES3_TriggerSpellEffectEvent(sourceInstance, deltaTime, effectInstance, effectIndex, negateOnExpiry, isUncapped, eventValue, eventType, attribute, resistFunction);

		// Figure out our return type.
		sol::object modifiedValue = sol::nil;
		switch (eventTypeEnum) {
		case 0:
			modifiedValue = sol::make_object(s, genericEventValue.asInt > 0);
			break;
		case 1:
			modifiedValue = sol::make_object(s, genericEventValue.asInt);
			break;
		case 2:
			modifiedValue = sol::make_object(s, genericEventValue.asFloat);
			break;
		case 3:
			modifiedValue = sol::make_object(s, (Statistic*)eventValue);
			break;
		}

		return std::make_tuple(eventResult, modifiedValue);
	}

	sol::table packageSpellTickTable(sol::state& state, EffectID::EffectID effectId, MagicSourceInstance * sourceInstance, float deltaTime, MagicEffectInstance * effectInstance, int effectIndex) {
		auto tbl = state.create_table();

		tbl["effectId"] = effectId;
		tbl["sourceInstance"] = sourceInstance;
		tbl["deltaTime"] = deltaTime;
		tbl["effectInstance"] = effectInstance;
		tbl["effectIndex"] = effectIndex;

		tbl["trigger"] = triggerSpellEffectEvent;
		tbl["triggerSummon"] = triggerSpellSummonEvent;
		tbl["triggerBoundArmor"] = triggerSpellBoundArmorEvent;
		tbl["triggerBoundWeapon"] = triggerSpellBoundWeaponEvent;

		return tbl;
	}

	void __cdecl MagicEffectDispatch(EffectID::EffectID effectId, MagicSourceInstance * sourceInstance, float deltaTime, MagicEffectInstance * effectInstance, int effectIndex) {
		if (mwse::lua::event::SpellTickEvent::getEventEnabled()) {
			auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			sol::table eventData = stateHandle.triggerEvent(new mwse::lua::event::SpellTickEvent(effectId, sourceInstance, deltaTime, effectInstance, effectIndex));
			if (eventData.valid()) {
				if (eventData.get_or("block", false)) {
					// We still need the main effect event function to be called for visual effects and durations to be handled.
					int flags = (DataHandler::get()->nonDynamicData->magicEffects->getEffectFlags(effectId) >> 12) & 0xFFFFFF01;
					int value = 0;
					TES3_TriggerSpellEffectEvent(sourceInstance, deltaTime, effectInstance, effectIndex, true, flags, &value, 0x7886F0, 0x1C, nullptr);
					return;
				}
			}
		}

		if (effectId <= EffectID::LastEffect) {
			reinterpret_cast<void(__cdecl **)(MagicSourceInstance *, float, MagicEffectInstance *, int)>(0x7884B0)[effectId](sourceInstance, deltaTime, effectInstance, effectIndex);
		}
		else {
			auto magicEffectController = DataHandler::get()->nonDynamicData->magicEffects;
			auto itt = magicEffectController->effectLuaTickFunctions.find(effectId);

			// No result? Do some basic things.
			if (itt == magicEffectController->effectLuaTickFunctions.end()) {
				int flags = (DataHandler::get()->nonDynamicData->magicEffects->getEffectFlags(effectId) >> 12) & 0xFFFFFF01;
				int value = 0;
				TES3_TriggerSpellEffectEvent(sourceInstance, deltaTime, effectInstance, effectIndex, true, flags, &value, 0x7886F0, 0x1C, nullptr);
				return;
			}

			auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			sol::protected_function_result result = itt->second(packageSpellTickTable(stateHandle.state, effectId, sourceInstance, deltaTime, effectInstance, effectIndex));
			if (!result.valid()) {
				sol::error error = result;
				mwse::log::getLog() << "Lua error encountered in magic effect dispatch function:" << std::endl << error.what() << std::endl;
				return;
			}
		}
	}

	// Rewrite of the enchanting population code to make use of our custom effect collection.
	const auto TES3_UI_SortEnchantingMenu = reinterpret_cast<void(__stdcall*)()>(0x5C36D0);
	void __cdecl PopulateEnchantingMenu() {
		auto menuEnchanting = UI::findMenu(*reinterpret_cast<UI::UI_ID*>(0x7D36BC));
		if (menuEnchanting == nullptr) {
			return;
		}

		auto effectsScroll = reinterpret_cast<UI::WidgetScrollPane*>(menuEnchanting->getProperty(UI::PropertyType::Pointer, *reinterpret_cast<UI::Property*>(0x7D35AC)).ptrValue);
		auto effectsScrollContent = effectsScroll->getContentPane();
		if (effectsScrollContent == nullptr) {
			return;
		}

		auto ndd = DataHandler::get()->nonDynamicData;
		auto magicEffectController = ndd->magicEffects;
		auto macp = WorldController::get()->getMobilePlayer();
		auto spellList = macp->getCombatSpellList();

		for (auto effectItt : magicEffectController->effectObjects) {
			auto effect = effectItt.second;
			bool hasEffect = false;
			if (effect->flags & EffectFlag::AllowEnchanting) {
				for (auto& spell : *spellList) {
					if (hasEffect) {
						break;
					}

					if (spell->castType != SpellCastType::Spell) {
						continue;
					}

					for (size_t i = 0; i < 8; i++) {
						if (spell->effects[i].effectID == effect->id) {
							hasEffect = true;
							break;
						}
					}
				}
			}

			if (hasEffect) {
				auto effectText = effectsScrollContent->createTextSelect(static_cast<UI::UI_ID>(UI::Property::null));
				effectText->setText(magicEffectController->getEffectName(effect->id));
				effectText->setProperty(*reinterpret_cast<UI::Property*>(0x7D36BA), effect);
				effectText->setProperty(UI::Property::help_tooltip, (UI::EventCallback)0x5C6650);
				effectText->setProperty(UI::Property::event_mouse_click, (UI::EventCallback)0x5C2E50);
			}
		}

		// Sort the list.
		TES3_UI_SortEnchantingMenu();
	}

	// Rewrite of the spellmaking population code to make use of our custom effect collection.
	const auto TES3_UI_SortSpellmakingMenu = reinterpret_cast<void (__stdcall*)()>(0x621EB0);
	void __cdecl PopulateSpellmakingMenu() {
		auto menuSpellmaking = UI::findMenu(*reinterpret_cast<UI::UI_ID*>(0x7D698C));
		if (menuSpellmaking == nullptr) {
			return;
		}

		auto effectsScroll = reinterpret_cast<UI::WidgetScrollPane*>(menuSpellmaking->getProperty(UI::PropertyType::Pointer, *reinterpret_cast<UI::Property*>(0x7D6990)).ptrValue);
		auto effectsScrollContent = effectsScroll->getContentPane();
		if (effectsScrollContent == nullptr) {
			return;
		}
		
		auto ndd = DataHandler::get()->nonDynamicData;
		auto magicEffectController = ndd->magicEffects;
		auto macp = WorldController::get()->getMobilePlayer();
		auto spellList = macp->getCombatSpellList();

		for (auto effectItt : magicEffectController->effectObjects) {
			auto effect = effectItt.second;
			bool hasEffect = false;
			if (effect->flags & EffectFlag::AllowSpellmaking) {
				for (auto& spell : *spellList) {
					if (hasEffect) {
						break;
					}

					if (spell->castType != SpellCastType::Spell) {
						continue;
					}

					for (size_t i = 0; i < 8; i++) {
						if (spell->effects[i].effectID == effect->id) {
							hasEffect = true;
							break;
						}
					}
				}
			}

			if (hasEffect) {
				auto effectText = effectsScrollContent->createTextSelect(static_cast<UI::UI_ID>(UI::Property::null));
				effectText->setText(magicEffectController->getEffectName(effect->id));
				effectText->setProperty(*reinterpret_cast<UI::Property*>(0x7D698E), effect);
				effectText->setProperty(UI::Property::help_tooltip, (UI::EventCallback)0x622E70);
				effectText->setProperty(UI::Property::event_mouse_click, (UI::EventCallback)0x621990);
			}
		}

		// Sort the list.
		TES3_UI_SortSpellmakingMenu();
	}

	GameSetting temporaryNameGMST;
	GameSetting * __fastcall getSpellNameGMST(DataHandler * dataHandler, DWORD EDX, int gmstId) {
		if (gmstId < 0) {
			temporaryNameGMST.value.asString = (char*)dataHandler->nonDynamicData->magicEffects->effectCustomNames[gmstId * -1].c_str();
			return &temporaryNameGMST;
		}
		else {
			return dataHandler->nonDynamicData->GMSTs[gmstId];
		}
	}
	char * __fastcall getSpellNameString(WorldController * worldController, DWORD EDX, int gmstId) {
		return getSpellNameGMST(DataHandler::get(), EDX, gmstId)->value.asString;
	}

	void __fastcall OnSpellProjectileHit(MagicSourceInstance * instance, DWORD EDX, MobileObject::Collision * collision) {
		auto magicEffectController = DataHandler::get()->nonDynamicData->magicEffects;
		magicEffectController->spellProjectileHit(instance, collision);
	}

	void MagicEffectController::spellProjectileHit(MagicSourceInstance * instance, MobileObject::Collision * collision) {
		instance->projectileHit(collision);

		auto effects = instance->sourceCombo.getSourceEffects();
		for (size_t i = 0; i < 8; i++) {
			auto effectId = effects[i].effectID;
			if (effectId == -1) {
				break;
			}

			auto itt = effectLuaCollisionFunctions.find(effectId);
			if (itt != effectLuaCollisionFunctions.end()) {
				auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();

				sol::table params = stateHandle.state.create_table();
				params["effectId"] = effectId;
				params["effectIndex"] = i;
				params["sourceInstance"] = instance;
				params["collision"] = collision;

				sol::protected_function_result result = itt->second(params);
				if (!result.valid()) {
					sol::error error = result;
					mwse::log::getLog() << "Lua error encountered in magic effect collision function:" << std::endl << error.what() << std::endl;
					return;
				}
			}
		}
	}

	void patchMagicEffectDispatch(DWORD address, BYTE pushInstruction) {
		// Push the switch index onto the stack.
		mwse::writeByteUnprotected(address, pushInstruction);

		// Call our dispatch handler.
		mwse::genCallUnprotected(address + 1, reinterpret_cast<DWORD>(MagicEffectDispatch));

		// Fix ESP by popping the value back. We throw the value into a register that won't be used.
		// This is done to try to keep the instructions all of the same size, so that stepping over the 
		// instructions in another debugger doesn't show as being ugly.
		mwse::writeByteUnprotected(address + 6, 0x59);
	}

	void MagicEffectController::InstallCustomMagicEffectController() {
		// Remove initial clearing of magic effect data, instead create our controller.
		mwse::writePatchCodeUnprotected(0x4B7E8F, (BYTE*)&patchInitializeMEC, patchInitializeMEC_size);
		mwse::genCallUnprotected(0x4B7E91, (DWORD)InitializeController, 0x16);

		// Clear some more initialization of magic effect IDs.
		mwse::genNOPUnprotected(0x4B831D, 0x22);

		// Prevent free of spell effect description when help menu display it both for populating enchanting menu and spell making menu
		mwse::genNOPUnprotected(0x6231E1, 0x5);
		mwse::genNOPUnprotected(0x5C69C1, 0x5);

		// When destructing the effect list, 
		mwse::genCallUnprotected(0x4B821B, (DWORD)DestroyController, 0x20);

		// Fix object loading.
		mwse::genCallEnforced(0x4BE4E7, 0x4B6880, (DWORD)ReadMagicEffectIdForLoading);
		mwse::genNOPUnprotected(0x4BE4EC, 0x201);

		// Resolve links.
		mwse::genCallUnprotected(0x4BB6E7, (DWORD)ResolveAllLinks, 0x1C);

		// Trigger any events on spell collision.
		mwse::genCallEnforced(0x573775, 0x5175C0, (DWORD)OnSpellProjectileHit);
		mwse::genCallEnforced(0x573D96, 0x5175C0, (DWORD)OnSpellProjectileHit);
		mwse::genCallEnforced(0x574D75, 0x5175C0, (DWORD)OnSpellProjectileHit);
		mwse::genCallEnforced(0x574DF1, 0x5175C0, (DWORD)OnSpellProjectileHit);
		mwse::genCallEnforced(0x574E90, 0x5175C0, (DWORD)OnSpellProjectileHit);
		mwse::genCallEnforced(0x574F02, 0x5175C0, (DWORD)OnSpellProjectileHit);

		// Make it so that every lookup for the name GMST instead looks to our custom temporary spell name GMST.
		mwse::genCallEnforced(0x4A92BC, 0x488810, (DWORD)getSpellNameGMST);
		mwse::genCallEnforced(0x4A934D, 0x488810, (DWORD)getSpellNameGMST);
		mwse::genCallEnforced(0x4A93DE, 0x488810, (DWORD)getSpellNameGMST);
		mwse::genCallEnforced(0x4A946F, 0x488810, (DWORD)getSpellNameGMST);
		mwse::genCallEnforced(0x4AB7F6, 0x40F930, (DWORD)getSpellNameString);
		mwse::genCallEnforced(0x51601F, 0x40F930, (DWORD)getSpellNameString);
		mwse::genCallEnforced(0x5160EC, 0x40F930, (DWORD)getSpellNameString);
		mwse::genCallEnforced(0x5161E3, 0x40F930, (DWORD)getSpellNameString);
		mwse::genCallEnforced(0x5162B0, 0x40F930, (DWORD)getSpellNameString);
		mwse::genCallEnforced(0x591BEC, 0x40F930, (DWORD)getSpellNameString);
		mwse::genCallEnforced(0x5922A0, 0x40F930, (DWORD)getSpellNameString);
		mwse::genCallEnforced(0x592683, 0x40F930, (DWORD)getSpellNameString);
		mwse::genCallEnforced(0x59E9CA, 0x40F930, (DWORD)getSpellNameString);
		mwse::genCallEnforced(0x5A9882, 0x40F930, (DWORD)getSpellNameString);
		mwse::genCallEnforced(0x5A9F0B, 0x40F930, (DWORD)getSpellNameString);
		mwse::genCallEnforced(0x5AA593, 0x40F930, (DWORD)getSpellNameString);
		mwse::genCallEnforced(0x5C3665, 0x40F930, (DWORD)getSpellNameString);
		mwse::genCallEnforced(0x5C3791, 0x40F930, (DWORD)getSpellNameString);
		mwse::genCallEnforced(0x5C37D8, 0x40F930, (DWORD)getSpellNameString);
		mwse::genCallEnforced(0x5C5202, 0x40F930, (DWORD)getSpellNameString);
		mwse::genCallEnforced(0x5C68A0, 0x40F930, (DWORD)getSpellNameString);
		mwse::genCallEnforced(0x5E4B8C, 0x40F930, (DWORD)getSpellNameString);
		mwse::genCallEnforced(0x5E513E, 0x40F930, (DWORD)getSpellNameString);
		mwse::genCallEnforced(0x5E7C73, 0x40F930, (DWORD)getSpellNameString);
		mwse::genCallEnforced(0x608160, 0x40F930, (DWORD)getSpellNameString);
		mwse::genCallEnforced(0x60C123, 0x40F930, (DWORD)getSpellNameString);
		mwse::genCallEnforced(0x616CDD, 0x40F930, (DWORD)getSpellNameString);
		mwse::genCallEnforced(0x61AD67, 0x40F930, (DWORD)getSpellNameString);
		mwse::genCallEnforced(0x61C5CA, 0x40F930, (DWORD)getSpellNameString);
		mwse::genCallEnforced(0x621E3E, 0x40F930, (DWORD)getSpellNameString);
		mwse::genCallEnforced(0x621F71, 0x40F930, (DWORD)getSpellNameString);
		mwse::genCallEnforced(0x621FB8, 0x40F930, (DWORD)getSpellNameString);
		mwse::genCallEnforced(0x6230C0, 0x40F930, (DWORD)getSpellNameString);
		mwse::genCallEnforced(0x62F10D, 0x40F930, (DWORD)getSpellNameString);

		// Fix enchanting/spellmaking list population.
		mwse::genCallEnforced(0x5C24F1, 0x5C3500, (DWORD)PopulateEnchantingMenu);
		mwse::genCallEnforced(0x6210B4, 0x621CE0, (DWORD)PopulateSpellmakingMenu);

		// Call custom effect dispatchers (and the spell tick event).
		patchMagicEffectDispatch(0x4647E9, 0x50);
		patchMagicEffectDispatch(0x464A56, 0x50);
		patchMagicEffectDispatch(0x515AB1, 0x51);

		// Replace all calls to NonDynamicData::getMagicEffectData.
		mwse::genCallEnforced(0x410AAA, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x410B40, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x410BDD, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x460F74, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x4AA710, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x4AA892, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x4AA933, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x4AAA89, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x4AACA3, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x4AB430, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x4AB5F6, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x4AC31F, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x4F284E, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x514664, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x514CB1, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x514D47, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x515210, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x5155B1, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x51570C, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x515C13, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x51631D, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x51634F, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x5165D8, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x516B16, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x516B54, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x5176A6, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x5176C9, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x517BD7, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x518A2A, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x518BA6, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x558584, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x5742CA, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x591954, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x592009, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x5923AE, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x59CA0E, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x59CA5E, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x59E44A, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x59E4A2, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x5A95EC, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x5A9C75, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x5AA2FD, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x5C355F, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x5D1B6E, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x5E159E, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x5E1603, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x5E19CB, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x5E233D, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x5E25E1, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x5E4263, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x5E4441, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x5E4731, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x5E485C, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x5E7888, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x5E797D, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x5EBB05, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x5ECF2D, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x5ECF4D, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x5ECF64, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x607A76, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x607DE7, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x607EDB, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x60BE9C, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x616951, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x616A45, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x621D3F, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x62ED91, 0x4BA590, (DWORD)getMagicEffectData);
		mwse::genCallEnforced(0x62EE7F, 0x4BA590, (DWORD)getMagicEffectData);

		// Change all access to magic effect name GMSTs.
		DWORD effectNameGMSTsAddress = (DWORD)&effectNameGMSTs;
		mwse::writeDoubleWordEnforced(0x4A92AE + 0x3, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x4A933F + 0x3, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x4A93D0 + 0x3, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x4A9461 + 0x3, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x4AB613 + 0x2, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x4AB6C0 + 0x2, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x4AB7E9 + 0x2, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x4F8E03 + 0x1, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x4FC5A9 + 0x4, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x516006 + 0x3, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x5160DE + 0x3, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x5161CA + 0x3, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x516297 + 0x3, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x591A09 + 0x2, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x591AB6 + 0x2, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x591BDF + 0x2, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x5920B9 + 0x2, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x59215B + 0x2, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x5924A9 + 0x2, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x592550 + 0x2, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x59E7B1 + 0x3, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x59E7CF + 0x3, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x59E7FE + 0x3, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x59E82D + 0x3, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x59E84B + 0x3, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x59E87B + 0x3, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x59E8C1 + 0x3, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x59E907 + 0x3, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x59E94D + 0x3, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x59E990 + 0x3, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x59E9BC + 0x3, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x5A96A2 + 0x2, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x5A974F + 0x2, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x5A9875 + 0x2, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x5A9D2B + 0x2, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x5A9DD8 + 0x2, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x5A9EFE + 0x2, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x5AA3B3 + 0x2, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x5AA460 + 0x2, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x5AA586 + 0x2, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x5C365D + 0x3, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x5C3783 + 0x3, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x5C37CA + 0x3, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x5C503B + 0x3, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x5C50CE + 0x3, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x5C6892 + 0x3, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x5E49A9 + 0x2, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x5E4A56 + 0x2, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x5E4B7F + 0x2, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x5E512C + 0x3, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x5E574C + 0x3, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x5E581C + 0x3, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x5E59C5 + 0x3, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x5E5A58 + 0x3, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x5E5BEB + 0x3, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x5E5C7C + 0x3, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x5E7A90 + 0x2, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x5E7B3D + 0x2, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x5E7C66 + 0x2, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x607F86 + 0x2, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x60802D + 0x2, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x60BF40 + 0x2, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x60BFED + 0x2, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x60C116 + 0x2, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x616AFD + 0x2, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x616BAA + 0x2, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x616CD0 + 0x2, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x61AD59 + 0x3, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x61C3EE + 0x3, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x61C496 + 0x3, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x621E30 + 0x3, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x621F63 + 0x3, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x621FAA + 0x3, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x6230B2 + 0x3, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x62EF33 + 0x2, 0x79454C, effectNameGMSTsAddress);
		mwse::writeDoubleWordEnforced(0x62EFDA + 0x2, 0x79454C, effectNameGMSTsAddress);

		// Write the previous data to the new effect array.
		std::fill_n(effectNameGMSTs, MAX_EFFECT_COUNT, 1409);
		memcpy_s(effectNameGMSTs, sizeof(effectNameGMSTs), (void*)0x79454C, sizeof(DWORD) * 143);

		// Change all access to magic effect flags.
		DWORD effectFlagAddresses = (DWORD)&effectFlags;
		mwse::writeDoubleWordEnforced(0x45481F + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x454CE4 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x45F1A6 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x45F206 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x45F269 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x45F2E6 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x45F347 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x45F3C7 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x45F447 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x45F4CA + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x45F55B + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x45F5FA + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x45F68B + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x45F726 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x45F79F + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x45F879 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x45F9D7 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x45FB92 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x45FF56 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x45FFB6 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x460016 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x4600A9 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x460156 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x4601BB + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x4602AC + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x46033B + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x4603F6 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x460456 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x4604B6 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x460516 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x460576 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x4605D6 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x460636 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x460696 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x4606F6 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x460756 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x460C24 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x460D74 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x460EA6 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x460F19 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x460FC4 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x461131 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x461295 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x46138B + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x4614C6 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x461542 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x461643 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x4616EA + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x46184E + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x4618D9 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x461949 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x4619B9 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x461A2E + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x461AB9 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x461B29 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x461B99 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x461BEE + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x461C6E + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x461CEE + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x461D6E + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x461DEE + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x461E6E + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x461EEE + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x461FAA + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x46206A + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x4620EA + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x4621BA + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x4622B6 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x462398 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x462446 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x4624AB + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x46256B + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x46265C + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x46271A + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x462899 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x4629C8 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x462A8A + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x462BEA + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x462D74 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x462E36 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x462E96 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x462EF6 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x462F56 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x462FB6 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x463016 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x463076 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x4630D6 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x463136 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x463196 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x463241 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x4632A9 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x463511 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x4635C1 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x4636A5 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x46386C + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x46394C + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x463A18 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x463AC8 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x463B48 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x463BB6 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x464256 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x4642C5 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x4644D2 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x464549 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x464725 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x46498E + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x464BE7 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x464CC6 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x464F57 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x465227 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x465BA7 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x4662A9 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x466988 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x4AA1BB + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x4AA71B + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x4AB439 + 0x3, 0x747D88 + 0x1, effectFlagAddresses + 0x1);
		mwse::writeDoubleWordEnforced(0x4AB604 + 0x2, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x4AB8A7 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x4AB932 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x4AC23D + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x4AC328 + 0x3, 0x747D88 + 0x1, effectFlagAddresses + 0x1);
		mwse::writeDoubleWordEnforced(0x4D7E3C + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x4D800A + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x516F06 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x51700A + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x51703A + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x5170ED + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x517212 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x51722D + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x51733A + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x5181ED + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x518269 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x51842E + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x51891E + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x51901A + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x519215 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x5193CE + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x519548 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x519687 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x538A2A + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x538A4F + 0x3, 0x747D88 + 0x2, effectFlagAddresses + 0x2);
		mwse::writeDoubleWordEnforced(0x538A72 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x538C59 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x538C81 + 0x3, 0x747D88 + 0x2, effectFlagAddresses + 0x2);
		mwse::writeDoubleWordEnforced(0x5919FA + 0x2, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x591D2D + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x591DFA + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x5920AA + 0x2, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x59249A + 0x2, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x5927C0 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x592885 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x59CA16 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x59CA26 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x59CA70 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x59CAB2 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x59CBD8 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x59CC2C + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x59CD88 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x59CE9E + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x59E452 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x59E466 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x59E4B4 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x59E4E8 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x59E7A3 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x59E86D + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x5A9693 + 0x2, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x5A99BE + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x5A9AAB + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x5A9D1C + 0x2, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x5AA047 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x5AA134 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x5AA3A4 + 0x2, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x5AA6CF + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x5AA7BC + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x5C2FFC + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x5C4545 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x5C457C + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x5C502B + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x5C6A7A + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x5D1D7B + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x5E499A + 0x2, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x5E4CCD + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x5E4DA4 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x5E51BE + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x5E5257 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x5E53F8 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x5E5591 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x5E5725 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x5E59A0 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x5E5BC4 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x5E7A81 + 0x2, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x5E7DB4 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x5E7E8B + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x607F77 + 0x2, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x6082A1 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x608380 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x60BF31 + 0x2, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x60C260 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x60C337 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x616AEE + 0x2, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x616E15 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x61AEC1 + 0x3, 0x747D88, effectFlagAddresses); // Without MCP
		mwse::writeDoubleWordEnforced(0x61AEB8 + 0x3, 0x747D88, effectFlagAddresses); // With MCP
		mwse::writeDoubleWordEnforced(0x61AF4D + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x61B131 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x61B29F + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x61B3B2 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x61B608 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x61BEC4 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x61BF0A + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x61C3D8 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x621B32 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x622BD2 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x622C0B + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x622C36 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x623290 + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x62EF24 + 0x2, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x62F24E + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x62F32D + 0x3, 0x747D88, effectFlagAddresses);
		mwse::writeDoubleWordEnforced(0x745F36 + 0x3, 0x747D88, effectFlagAddresses);

		// Write the previous data to the new effect array.
		memcpy_s(effectFlags, sizeof(effectFlags), (void*)0x747D88, sizeof(DWORD) * 143);

		// Patch effect counters.
		DWORD effectCounterAddress = (DWORD)&effectCounters;
		mwse::writeDoubleWordEnforced(0x537EE8 + 0x3, 0x787950, effectCounterAddress);
		mwse::writeDoubleWordEnforced(0x538475 + 0x3, 0x787950, effectCounterAddress);

		// Write the previous data to the new effect counters array.
		std::fill_n((int*)effectCounters, 5 * MAX_EFFECT_COUNT, 143);
		memcpy_s(effectCounters, sizeof(effectCounters), (void*)0x787950, sizeof(DWORD) * 5 * 143);
	}
}
