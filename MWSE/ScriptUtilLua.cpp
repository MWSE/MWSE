#include "LuaUtil.h"
#include "LuaManager.h"

#include "LuaEquipEvent.h"

#include "Stack.h"

#include "ScriptUtil.h"
#include "ScriptUtilLua.h"

#include "TES3Creature.h"
#include "TES3Dialogue.h"
#include "TES3LeveledList.h"
#include "TES3Misc.h"
#include "TES3Reference.h"
#include "TES3Script.h"
#include "TES3Spell.h"
#include "TES3WorldController.h"

#include "BitUtil.h"
#include "InstructionStore.h"
#include "MGEApi.h"
#include "MGEApiLua.h"

namespace mwse::lua {
	void bindScriptUtil() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		sol::table lua_mwscript = state["mwscript"];

		//
		// Get context on the current execution.
		//

		lua_mwscript["getScript"] = []() {
			return LuaManager::getInstance().getCurrentScript();
		};

		lua_mwscript["getReference"] = []() {
			return LuaManager::getInstance().getCurrentReference();
		};

		//
		// Expose base game opcodes.
		//

		lua_mwscript["activate"] = [](sol::optional<sol::table> params) {
			auto script = getOptionalParamExecutionScript(params);
			auto reference = getOptionalParamExecutionReference(params);

			mwscript::Activate(script, reference);
		};
		lua_mwscript["addItem"] = [](sol::optional<sol::table> params) {
			auto script = getOptionalParamExecutionScript(params);
			auto reference = getOptionalParamExecutionReference(params);
			auto item = getOptionalParamObject<TES3::Item>(params, "item");
			auto count = getOptionalParam(params, "count", 1);
			if (item == nullptr || count <= 0) {
				return false;
			}

			mwscript::AddItem(script, reference, item, count);
			reference->setObjectModified(true);
			reference->baseObject->setObjectModified(true);
			return true;
		};
		lua_mwscript["addSoulGem"] = [](sol::optional<sol::table> params) {
			auto script = getOptionalParamExecutionScript(params);
			auto reference = getOptionalParamExecutionReference(params);
			auto creature = getOptionalParamObject<TES3::Creature>(params, "creature");
			auto soulGem = getOptionalParamObject<TES3::Misc>(params, "soulgem");
			if (creature == nullptr || soulGem == nullptr) {
				return false;
			}

			mwscript::AddSoulGem(script, reference, creature, soulGem);
			return true;
		};
		lua_mwscript["addToLevCreature"] = [](sol::optional<sol::table> params) {
			auto script = getOptionalParamExecutionScript(params);
			auto reference = getOptionalParamExecutionReference(params);
			auto list = getOptionalParamObject<TES3::LeveledCreature>(params, "list");
			auto actor = getOptionalParamObject<TES3::Actor>(params, "creature");
			auto level = getOptionalParam<unsigned short>(params, "level", 0);
			if (list == nullptr || actor == nullptr || level == 0) {
				return false;
			}

			mwscript::AddToLevCreature(script, reference, list, actor, level);
			return true;
		};
		lua_mwscript["addSpell"] = [](sol::optional<sol::table> params) {
			auto script = getOptionalParamExecutionScript(params);
			auto reference = getOptionalParamExecutionReference(params);
			auto spell = getOptionalParamSpell(params, "spell");
			if (spell == nullptr) {
				return false;
			}

			mwscript::AddSpell(script, reference, spell);
			return true;
		};
		lua_mwscript["addToLevItem"] = [](sol::optional<sol::table> params) {
			auto script = getOptionalParamExecutionScript(params);
			auto reference = getOptionalParamExecutionReference(params);
			auto list = getOptionalParamObject<TES3::BaseObject>(params, "list");
			auto item = getOptionalParamObject<TES3::Item>(params, "item");
			auto level = getOptionalParam<unsigned short>(params, "level", 0);
			if (list == nullptr || item == nullptr || level == 0) {
				return false;
			}

			mwscript::AddToLevItem(script, reference, list, item, level);
			return true;
		};
		lua_mwscript["addTopic"] = [](sol::optional<sol::table> params) {
			auto script = getOptionalParamExecutionScript(params);
			auto reference = getOptionalParamExecutionReference(params);
			auto topic = getOptionalParamDialogue(params, "topic");
			if (topic == nullptr || topic->type != TES3::DialogueType::Topic) {
				return false;
			}

			mwscript::AddTopic(script, reference, topic);
			return true;
		};
		lua_mwscript["aiTravel"] = [](sol::optional<sol::table> params) {
			auto script = getOptionalParamExecutionScript(params);
			auto reference = getOptionalParamExecutionReference(params);
			auto x = getOptionalParam(params, "x", 0.0f);
			auto y = getOptionalParam(params, "y", 0.0f);
			auto z = getOptionalParam(params, "z", 0.0f);

			mwscript::AITravel(script, reference, x, y, z);
			return true;
		};
		lua_mwscript["disable"] = [](sol::optional<sol::table> params) {
			auto script = getOptionalParamExecutionScript(params);
			auto reference = getOptionalParamExecutionReference(params);
			auto setModified = getOptionalParam(params, "modify", true);

			if (setModified) {
				reference->setObjectModified(true);
			}

			mwscript::Disable(script, reference);
			return true;
		};
		lua_mwscript["drop"] = [](sol::optional<sol::table> params) {
			auto script = getOptionalParamExecutionScript(params);
			auto reference = getOptionalParamExecutionReference(params);
			auto item = getOptionalParamObject<TES3::Item>(params, "item");
			auto count = getOptionalParam(params, "count", 1);
			if (item == nullptr || count <= 0) {
				return false;
			}

			mwscript::Drop(script, reference, item, count);
			return true;
		};
		lua_mwscript["enable"] = [](sol::optional<sol::table> params) {
			auto script = getOptionalParamExecutionScript(params);
			auto reference = getOptionalParamExecutionReference(params);
			auto setModified = getOptionalParam(params, "modify", true);

			if (reference == nullptr) {
				return false;
			}

			if (setModified) {
				reference->setObjectModified(true);
			}

			mwscript::Enable(script, reference);
			return true;
		};
		lua_mwscript["equip"] = [](sol::optional<sol::table> params) {
			auto script = getOptionalParamExecutionScript(params);
			auto reference = getOptionalParamExecutionReference(params);
			auto item = getOptionalParamObject<TES3::Item>(params, "item");
			if (item == nullptr || reference == nullptr) {
				return false;
			}

			// Fire off the event, because script calls don't hit the same code as our hooks.
			if (event::EquipEvent::getEventEnabled()) {
				auto& luaManager = mwse::lua::LuaManager::getInstance();
				auto stateHandle = luaManager.getThreadSafeStateHandle();
				sol::object response = stateHandle.triggerEvent(new event::EquipEvent(reference, item, nullptr));
				if (response.get_type() == sol::type::table) {
					sol::table eventData = response;
					if (eventData.get_or("block", false)) {
						return false;
					}
				}
			}

			mwscript::Equip(script, reference, item);
			return true;
		};
		lua_mwscript["explodeSpell"] = [](sol::optional<sol::table> params) {
			auto script = getOptionalParamExecutionScript(params);
			auto reference = getOptionalParamExecutionReference(params);
			auto spell = getOptionalParamSpell(params, "spell");
			if (spell == nullptr) {
				return false;
			}

			mwscript::ExplodeSpell(script, reference, spell);
			return true;
		};
		lua_mwscript["getButtonPressed"] = [](sol::optional<sol::table> params) {
			auto script = getOptionalParamExecutionScript(params);
			auto reference = getOptionalParamExecutionReference(params);

			return mwscript::GetButtonPressed(script, reference);
		};
		lua_mwscript["hasItemEquipped"] = [](sol::optional<sol::table> params) {
			auto script = getOptionalParamExecutionScript(params);
			auto reference = getOptionalParamExecutionReference(params);
			auto item = getOptionalParamObject<TES3::Item>(params, "item");
			if (item == nullptr) {
				return false;
			}

			return mwscript::HasItemEquipped(script, reference, item);
		};
		// Obsolete. Do not document.
		lua_mwscript["getDelete"] = [](sol::optional<sol::table> params) {
			auto script = getOptionalParamExecutionScript(params);
			auto reference = getOptionalParamExecutionReference(params);

			return BITMASK_TEST(reference->objectFlags, TES3::ObjectFlag::Delete);
		};
		lua_mwscript["getDetected"] = [](sol::optional<sol::table> params) {
			auto script = getOptionalParamExecutionScript(params);
			auto reference = getOptionalParamExecutionReference(params);
			auto target = getOptionalParamReference(params, "target");
			if (reference == nullptr || target == nullptr) {
				return false;
			}

			return mwscript::GetDetected(script, reference, target);
		};
		lua_mwscript["getDisabled"] = [](sol::optional<sol::table> params) {
			auto script = getOptionalParamExecutionScript(params);
			auto reference = getOptionalParamExecutionReference(params);
			return mwscript::GetDisabled(script, reference);
		};
		lua_mwscript["getDistance"] = [](sol::optional<sol::table> params) {
			auto script = getOptionalParamExecutionScript(params);
			auto reference = getOptionalParamExecutionReference(params);
			auto target = getOptionalParamReference(params, "target");
			if (reference == nullptr || target == nullptr) {
				return -1.0f;
			}

			return mwscript::GetDistance(script, reference, target);
		};
		lua_mwscript["getItemCount"] = [](sol::optional<sol::table> params) -> int {
			auto script = getOptionalParamExecutionScript(params);
			auto reference = getOptionalParamExecutionReference(params);
			auto item = getOptionalParamObject<TES3::Item>(params, "item");
			if (item == nullptr) {
				return 0;
			}

			return mwscript::GetItemCount(script, reference, item);
		};
		lua_mwscript["getPCJumping"] = [](sol::optional<sol::table> params) {
			auto script = getOptionalParamExecutionScript(params);
			return mwscript::GetPCJumping(script);
		};
		lua_mwscript["getPCRunning"] = [](sol::optional<sol::table> params) {
			auto script = getOptionalParamExecutionScript(params);
			return mwscript::GetPCRunning(script);
		};
		lua_mwscript["getPCSneaking"] = [](sol::optional<sol::table> params) {
			auto script = getOptionalParamExecutionScript(params);
			return mwscript::GetPCSneaking(script);
		};
		lua_mwscript["getSpellEffects"] = [](sol::optional<sol::table> params) {
			auto script = getOptionalParamExecutionScript(params);
			auto reference = getOptionalParamExecutionReference(params);
			auto spell = getOptionalParamSpell(params, "spell");
			if (spell == nullptr) {
				return false;
			}

			return mwscript::GetSpellEffects(script, reference, spell);
		};
		// Obsolete. Do not document.
		lua_mwscript["onActivate"] = [](sol::optional<sol::table> params) {
			auto script = getOptionalParamExecutionScript(params);
			auto reference = getOptionalParamExecutionReference(params);

			auto result = reference->testActionFlag(TES3::ActionFlags::OnActivate);
			reference->clearActionFlag(TES3::ActionFlags::UseEnabled);
			reference->clearActionFlag(TES3::ActionFlags::OnActivate);
			return result;
		};
		// Obsolete. Do not document.
		lua_mwscript["onDeath"] = [](sol::optional<sol::table> params) {
			auto reference = getOptionalParamExecutionReference(params);

			auto result = reference->testActionFlag(TES3::ActionFlags::OnDeath);
			reference->clearActionFlag(TES3::ActionFlags::OnDeath);
			return result;
		};
		// Obsolete. Do not document.
		lua_mwscript["onKnockout"] = [](sol::optional<sol::table> params) {
			auto reference = getOptionalParamExecutionReference(params);

			auto result = reference->testActionFlag(TES3::ActionFlags::OnKnockout);
			reference->clearActionFlag(TES3::ActionFlags::OnKnockout);
			return result;
		};
		// Obsolete. Do not document.
		lua_mwscript["onMurder"] = [](sol::optional<sol::table> params) {
			auto reference = getOptionalParamExecutionReference(params);

			auto result = reference->testActionFlag(TES3::ActionFlags::OnMurder);
			reference->clearActionFlag(TES3::ActionFlags::OnMurder);
			return result;
		};
		lua_mwscript["placeAtPC"] = [](sol::optional<sol::table> params) -> TES3::Reference* {
			auto script = getOptionalParamExecutionScript(params);
			auto reference = getOptionalParamExecutionReference(params);
			auto object = getOptionalParamObject<TES3::BaseObject>(params, "object");
			auto count = getOptionalParam(params, "count", 1);
			auto distance = getOptionalParam(params, "distance", 256.0f);
			auto direction = getOptionalParam(params, "direction", 1.0f);
			if (object == nullptr) {
				return nullptr;
			}

			mwscript::PlaceAtPC(script, reference, object, count, distance, direction);
			return mwscript::lastCreatedPlaceAtPCReference;
		};
		lua_mwscript["playSound"] = [](sol::optional<sol::table> params) {
			auto script = getOptionalParamExecutionScript(params);
			auto reference = getOptionalParamExecutionReference(params);
			auto sound = getOptionalParamSound(params, "sound");
			if (sound == nullptr) {
				return false;
			}

			mwscript::PlaySound(script, reference, sound);
			return true;
		};
		lua_mwscript["position"] = [](sol::optional<sol::table> params) {
			auto script = getOptionalParamExecutionScript(params);
			auto reference = getOptionalParamExecutionReference(params);
			auto x = getOptionalParam(params, "x", 0.0f);
			auto y = getOptionalParam(params, "y", 0.0f);
			auto z = getOptionalParam(params, "z", 0.0f);
			auto rotation = getOptionalParam(params, "rotation", 0.0f);

			mwscript::Position(script, reference, x, y, z, rotation);
			return true;
		};
		lua_mwscript["positionCell"] = [](sol::optional<sol::table> params) {
			auto script = getOptionalParamExecutionScript(params);
			auto reference = getOptionalParamExecutionReference(params);
			auto x = getOptionalParam(params, "x", 0.0f);
			auto y = getOptionalParam(params, "y", 0.0f);
			auto z = getOptionalParam(params, "z", 0.0f);
			auto rotation = getOptionalParam(params, "rotation", 0.0f);
			auto cell = getOptionalParam<std::string>(params, "cell", "");
			if (cell.empty()) {
				return false;
			}

			mwscript::PositionCell(script, reference, x, y, z, rotation, cell.c_str());
			return true;
		};
		lua_mwscript["removeItem"] = [](sol::optional<sol::table> params) {
			auto script = getOptionalParamExecutionScript(params);
			auto reference = getOptionalParamExecutionReference(params);
			auto item = getOptionalParamObject<TES3::Item>(params, "item");
			auto count = getOptionalParam(params, "count", 1);
			if (item == nullptr) {
				return false;
			}

			mwscript::RemoveItem(script, reference, item, count);
			reference->setObjectModified(true);
			reference->baseObject->setObjectModified(true);
			return true;
		};
		lua_mwscript["removeSpell"] = [](sol::optional<sol::table> params) {
			auto script = getOptionalParamExecutionScript(params);
			auto reference = getOptionalParamExecutionReference(params);
			auto spell = getOptionalParamSpell(params, "spell");
			if (spell == nullptr) {
				return false;
			}

			mwscript::RemoveSpell(script, reference, spell);
			return true;
		};
		lua_mwscript["scriptRunning"] = [](sol::optional<sol::table> params) {
			auto script = getOptionalParamExecutionScript(params);
			auto targetScript = getOptionalParamScript(params, "script");
			if (targetScript == nullptr) {
				return false;
			}

			return TES3::WorldController::get()->isGlobalScriptRunning(targetScript);
		};
		// Obsolete. Do not document.
		lua_mwscript["setDelete"] = [](sol::optional<sol::table> params) {
			auto script = getOptionalParamExecutionScript(params);
			auto reference = getOptionalParamExecutionReference(params);

			reference->setDeleted(getOptionalParam(params, "delete", true));
			reference->setObjectModified(true);

			return true;
		};
		lua_mwscript["setLevel"] = [](sol::optional<sol::table> params) {
			auto script = getOptionalParamExecutionScript(params);
			auto reference = getOptionalParamExecutionReference(params);
			auto level = getOptionalParam<short>(params, "level", 0);
			if (level <= 0) {
				return false;
			}

			mwscript::SetLevel(script, reference, level);
			return true;
		};
		lua_mwscript["startCombat"] = [](sol::optional<sol::table> params) {
			auto script = getOptionalParamExecutionScript(params);
			auto reference = getOptionalParamExecutionReference(params);
			auto target = getOptionalParamReference(params, "target");
			if (target == nullptr) {
				return false;
			}

			mwscript::StartCombat(script, reference, target);
			return true;
		};
		lua_mwscript["startScript"] = [](sol::optional<sol::table> params) {
			auto script = getOptionalParamExecutionScript(params);
			auto reference = getOptionalParamExecutionReference(params);
			auto targetScript = getOptionalParamScript(params, "script");
			if (targetScript == nullptr) {
				return false;
			}

			TES3::WorldController::get()->startGlobalScript(targetScript, reference);
			return true;
		};
		lua_mwscript["stopCombat"] = [](sol::optional<sol::table> params) {
			auto script = getOptionalParamExecutionScript(params);
			auto reference = getOptionalParamExecutionReference(params);
			if (reference == nullptr) {
				return false;
			}

			mwscript::StopCombat(script, reference);
			return true;
		};
		lua_mwscript["stopScript"] = [](sol::optional<sol::table> params) {
			auto script = getOptionalParamExecutionScript(params);
			auto reference = getOptionalParamExecutionReference(params);
			auto targetScript = getOptionalParamScript(params, "script");
			if (targetScript == nullptr) {
				targetScript = script;
			}

			if (targetScript) {
				TES3::WorldController::get()->stopGlobalScript(targetScript);
				return true;
			}

			return false;
		};
		lua_mwscript["stopSound"] = [](sol::optional<sol::table> params) {
			auto script = getOptionalParamExecutionScript(params);
			auto reference = getOptionalParamExecutionReference(params);
			auto sound = getOptionalParamSound(params, "sound");
			if (sound == nullptr) {
				return false;
			}

			mwscript::StopSound(script, reference, sound);
			return true;
		};
		lua_mwscript["wakeUpPC"] = []() {
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::WakeUpPC);
		};
	}

	void bindLegacyMGEScriptUtil() {
		//
		// Pre MGE XE v0.14.x support.
		//
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		sol::state& state = stateHandle.state;
		sol::table lua_mge = state["mge"];

		// General functions.
		lua_mge["getScreenHeight"] = []() {
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEGetHeight);
			return Stack::getInstance().popLong();
		};
		lua_mge["getScreenWidth"] = []() {
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEGetWidth);
			return Stack::getInstance().popLong();
		};
		lua_mge["enabled"] = []() {
			return InstructionStore::getInstance().isOpcode(OpCode::xGetGS);
		};
		lua_mge["getVersion"] = [](sol::this_state ts) -> sol::object {
			mge::VersionStruct version = {};

			// Try to run the script opcode.
			if (InstructionStore::getInstance().isOpcode(OpCode::MGEGetVersion)) {
				mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEGetVersion);

				// Convert packed version to semver table. MGE XE major version is offset for legacy reasons.
				int ver = Stack::getInstance().popLong() - 0x40000;
				int major = (ver >> 16) & 0xFF, minor = (ver >> 8) & 0xFF, patch = ver & 0xFF;
			}
			// Next try the interface.
			else if (mge::lua::CoreInterface::enabled()) {
				return mge::lua::CoreInterface::getVersion(ts);
			}
			// Fall back to the MGE GUI version.
			else if (mge::guiVersion.valid()) {
				sol::state_view state = ts;
				return state.create_table_with("major", mge::guiVersion.major, "minor", mge::guiVersion.minor, "patch", mge::guiVersion.patch);
			}

			return sol::nil;
		};
		lua_mge["log"] = [](std::string string) {
			Stack::getInstance().pushString(string);
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEOutputDebugString);
		};

		// HUD-related functions.
		lua_mge["clearHUD"] = []() {
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEWipeAll);
		};
		lua_mge["disableHUD"] = [](sol::optional<sol::table> params) {
			auto hud = getOptionalParam<std::string>(params, "hud", "");

			if (!hud.empty()) {
				Stack::getInstance().pushString(hud);
				mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEDisableHUD);
			}
			else {
				mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGENIDDisableHUD);
			}

			return true;
		};
		lua_mge["enableHUD"] = [](sol::optional<sol::table> params) {
			auto hud = getOptionalParam<std::string>(params, "hud", "");

			if (!hud.empty()) {
				Stack::getInstance().pushString(hud);
				mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEEnableHUD);
			}
			else {
				mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGENIDEnableHUD);
			}

			return true;
		};
		lua_mge["freeHUD"] = [](sol::optional<sol::table> params) {
			auto hud = getOptionalParam<std::string>(params, "hud", "");

			if (!hud.empty()) {
				Stack::getInstance().pushString(hud);
				mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEFreeHUD);
			}
			else {
				mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGENIDFreeHUD);
			}

			return true;
		};
		lua_mge["fullscreenHUD"] = [](sol::optional<sol::table> params) {
			auto hud = getOptionalParam<std::string>(params, "hud", "");

			if (!hud.empty()) {
				Stack::getInstance().pushString(hud);
				mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEFullscreenHUD);
			}
			else {
				mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGENIDFullscreenHUD);
			}

			return true;
		};
		lua_mge["loadHUD"] = [](sol::optional<sol::table> params) {
			auto hud = getOptionalParam<std::string>(params, "hud", "");
			auto texture = getOptionalParam<std::string>(params, "texture", "");
			if (hud.empty() || texture.empty()) {
				return false;
			}

			Stack::getInstance().pushString(texture);
			Stack::getInstance().pushString(hud);
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGELoadHUD);

			if (getOptionalParam<bool>(params, "enable", false)) {
				Stack::getInstance().pushString(hud);
				mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEEnableHUD);
			}

			return true;
		};
		lua_mge["positionHUD"] = [](sol::optional<sol::table> params) {
			auto hud = getOptionalParam<std::string>(params, "hud", "");
			auto x = getOptionalParam(params, "x", 0.0f);
			auto y = getOptionalParam(params, "y", 0.0f);

			if (!hud.empty()) {
				Stack::getInstance().pushFloat(y);
				Stack::getInstance().pushFloat(x);
				Stack::getInstance().pushString(hud);
				mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEPositionHUD);
			}
			else {
				Stack::getInstance().pushFloat(y);
				Stack::getInstance().pushFloat(x);
				mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGENIDPositionHUD);
			}

			return true;
		};
		lua_mge["scaleHUD"] = [](sol::optional<sol::table> params) {
			auto hud = getOptionalParam<std::string>(params, "hud", "");
			auto x = getOptionalParam(params, "x", 0.0f);
			auto y = getOptionalParam(params, "y", 0.0f);

			if (!hud.empty()) {
				Stack::getInstance().pushFloat(y);
				Stack::getInstance().pushFloat(x);
				Stack::getInstance().pushString(hud);
				mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEScaleHUD);
			}
			else {
				Stack::getInstance().pushFloat(y);
				Stack::getInstance().pushFloat(x);
				mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGENIDScaleHUD);
			}

			return true;
		};
		lua_mge["selectHUD"] = [](sol::optional<sol::table> params) {
			auto hud = getOptionalParam<std::string>(params, "hud", "");
			if (hud.empty()) {
				return false;
			}

			Stack::getInstance().pushString(hud);
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEWithHUD);
			return true;
		};
		lua_mge["setHUDEffect"] = [](sol::optional<sol::table> params) {
			auto hud = getOptionalParam<std::string>(params, "hud", "");
			auto effect = getOptionalParam<std::string>(params, "effect", "");
			if (effect.empty()) {
				return false;
			}

			if (!hud.empty()) {
				Stack::getInstance().pushString(effect);
				Stack::getInstance().pushString(hud);
				mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEChangeHUDEffect);
			}
			else {
				Stack::getInstance().pushString(effect);
				mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGENIDChangeHUDEffect);
			}

			return true;
		};
		lua_mge["setHUDEffectFloat"] = [](sol::optional<sol::table> params) {
			auto hud = getOptionalParam<std::string>(params, "hud", "");
			auto variable = getOptionalParam<std::string>(params, "variable", "");
			float value = getOptionalParam(params, "value", 0.0f);
			if (variable.empty()) {
				return false;
			}

			if (!hud.empty()) {
				Stack::getInstance().pushString(hud);
				mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEWithHUD);
			}

			Stack::getInstance().pushFloat(value);
			Stack::getInstance().pushString(variable);
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGENIDSetHUDEffectFloat);

			return true;
		};
		lua_mge["setHUDEffectLong"] = [](sol::optional<sol::table> params) {
			auto hud = getOptionalParam<std::string>(params, "hud", "");
			auto variable = getOptionalParam<std::string>(params, "variable", "");
			auto value = getOptionalParam(params, "value", 0);
			if (variable.empty()) {
				return false;
			}

			if (!hud.empty()) {
				Stack::getInstance().pushString(hud);
				mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEWithHUD);
			}

			Stack::getInstance().pushLong(value);
			Stack::getInstance().pushString(variable);
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGENIDSetHUDEffectLong);

			return true;
		};
		lua_mge["setHUDEffectVector4"] = [](sol::optional<sol::table> params) {
			auto hud = getOptionalParam<std::string>(params, "hud", "");
			auto variable = getOptionalParam<std::string>(params, "variable", "");
			auto values = getOptionalParam<sol::table>(params, "value", sol::nil);
			if (variable.empty() || values == sol::nil || values.size() != 4) {
				return false;
			}

			if (!hud.empty()) {
				Stack::getInstance().pushString(hud);
				mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEWithHUD);
			}

			for (int i = 4; i > 0; i--) {
				Stack::getInstance().pushFloat(values[i]);
			}
			Stack::getInstance().pushString(variable);
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGENIDSetHUDEffectVec);

			return true;
		};
		lua_mge["setHUDTexture"] = [](sol::optional<sol::table> params) {
			auto hud = getOptionalParam<std::string>(params, "hud", "");
			auto texture = getOptionalParam<std::string>(params, "texture", "");
			if (texture.empty()) {
				return false;
			}

			if (!hud.empty()) {
				Stack::getInstance().pushString(texture);
				Stack::getInstance().pushString(hud);
				mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEChangeHUDTexture);
			}
			else {
				Stack::getInstance().pushString(texture);
				mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGENIDChangeHUDTexture);
			}

			return true;
		};
		lua_mge["unselectHUD"] = [](sol::optional<sol::table> params) {
			auto hud = getOptionalParam<std::string>(params, "hud", "");
			if (hud.empty()) {
				return false;
			}

			Stack::getInstance().pushString(hud);
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGECancelWithHUD);
			return true;
		};

		// Shader-related functions.
		lua_mge["disableShader"] = [](sol::optional<sol::table> params) {
			auto shader = getOptionalParam<std::string>(params, "shader", "");
			if (shader.empty()) {
				return false;
			}

			Stack::getInstance().pushString(shader);
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEDisableShader);

			return true;
		};
		lua_mge["enableShader"] = [](sol::optional<sol::table> params) {
			auto shader = getOptionalParam<std::string>(params, "shader", "");
			if (shader.empty()) {
				return false;
			}

			Stack::getInstance().pushString(shader);
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEEnableShader);

			return true;
		};
		lua_mge["setShaderFloat"] = [](sol::optional<sol::table> params) {
			auto shader = getOptionalParam<std::string>(params, "shader", "");
			auto variable = getOptionalParam<std::string>(params, "variable", "");
			auto value = getOptionalParam(params, "value", 0.0f);
			if (shader.empty() || variable.empty()) {
				return false;
			}

			Stack::getInstance().pushFloat(value);
			Stack::getInstance().pushString(variable);
			Stack::getInstance().pushString(shader);
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEShaderSetFloat);

			return true;
		};
		lua_mge["setShaderLong"] = [](sol::optional<sol::table> params) {
			auto shader = getOptionalParam<std::string>(params, "shader", "");
			auto variable = getOptionalParam<std::string>(params, "variable", "");
			auto value = getOptionalParam(params, "value", 0);
			if (shader.empty() || variable.empty()) {
				return false;
			}

			Stack::getInstance().pushLong(value);
			Stack::getInstance().pushString(variable);
			Stack::getInstance().pushString(shader);
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEShaderSetLong);

			return true;
		};
		lua_mge["setShaderVector4"] = [](sol::optional<sol::table> params) {
			auto shader = getOptionalParam<std::string>(params, "shader", "");
			auto variable = getOptionalParam<std::string>(params, "variable", "");
			sol::table values = getOptionalParam<sol::table>(params, "value", sol::nil);
			if (shader.empty() || variable.empty() || values == sol::nil || values.size() != 4) {
				return false;
			}
			for (int i = 4; i > 0; i--) {
				Stack::getInstance().pushFloat(values[i]);
			}
			Stack::getInstance().pushString(variable);
			Stack::getInstance().pushString(shader);
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEShaderSetVector);

			return true;
		};

		// Camera zoom functions.
		lua_mge["disableZoom"] = []() {
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEDisableZoom);
		};
		lua_mge["enableZoom"] = []() {
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEEnableZoom);
		};
		lua_mge["toggleZoom"] = []() {
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEToggleZoom);
		};
		lua_mge["zoomIn"] = [](sol::optional<sol::table> params) {
			auto amount = getOptionalParam(params, "amount", 0.0f);

			if (amount == 0.0) {
				mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEZoomIn);
			}
			else {
				Stack::getInstance().pushFloat(amount);
				mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEZoomInBy);
			}
		};
		lua_mge["zoomOut"] = [](sol::optional<sol::table> params) {
			auto amount = getOptionalParam(params, "amount", 0.0f);

			if (amount == 0.0) {
				mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEZoomOut);
			}
			else {
				Stack::getInstance().pushFloat(amount);
				mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEZoomOutBy);
			}
		};
		lua_mge["setZoom"] = [](sol::optional<sol::table> params) {
			auto amount = getOptionalParam(params, "amount", 0.0f);
			auto animate = getOptionalParam(params, "animate", false);

			if (animate) {
				Stack::getInstance().pushFloat(amount);
				mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEZoom);
			}
			else {
				Stack::getInstance().pushFloat(amount);
				mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGESetZoom);
			}
		};
		lua_mge["getZoom"] = []() {
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEGetZoom);
			return Stack::getInstance().popFloat();
		};
		lua_mge["stopZoom"] = []() {
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEStopZoom);
		};

		// Camera shake functions.
		lua_mge["enableCameraShake"] = [](sol::optional<sol::table> params) {
			auto magnitude = getOptionalParam(params, "magnitude", 0.0f);
			if (magnitude != 0.0) {
				Stack::getInstance().pushFloat(magnitude);
				mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGESetCameraShakeMagnitude);
			}

			auto acceleration = getOptionalParam(params, "acceleration", 0.0f);
			if (acceleration != 0.0) {
				Stack::getInstance().pushFloat(acceleration);
				mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGECameraShakeZoom);
			}

			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEEnableCameraShake);
		};
		lua_mge["disableCameraShake"] = []() {
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEDisableCameraShake);
		};
		lua_mge["setCameraShakeMagnitude"] = [](sol::optional<sol::table> params) {
			auto magnitude = getOptionalParam(params, "magnitude", 0.0f);
			Stack::getInstance().pushFloat(magnitude);
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGESetCameraShakeMagnitude);
		};
		lua_mge["setCameraShakeAcceleration"] = [](sol::optional<sol::table> params) {
			auto acceleration = getOptionalParam(params, "acceleration", 0.0f);
			Stack::getInstance().pushFloat(acceleration);
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGECameraShakeZoom);
		};

		// Camera rotation functions.
		lua_mge["getScreenRotation"] = []() {
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEGetScreenRotation);
			return Stack::getInstance().popFloat();
		};
		lua_mge["modScreenRotation"] = [](sol::optional<sol::table> params) {
			auto rotation = getOptionalParam(params, "rotation", 0.0f);
			Stack::getInstance().pushFloat(rotation);
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGERotateScreenBy);
		};
		lua_mge["setScreenRotation"] = [](sol::optional<sol::table> params) {
			auto rotation = getOptionalParam(params, "rotation", 0.0f);
			Stack::getInstance().pushFloat(rotation);
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGERotateScreen);
		};
		lua_mge["startScreenRotation"] = []() {
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEScreenSpin);
		};
		lua_mge["stopScreenRotation"] = []() {
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEStopSpinSpin);
		};

		// MGE XE rendering functions.
		lua_mge["setWeatherScattering"] = [](sol::optional<sol::table> params) {
			auto outscatter = getOptionalParam<sol::table>(params, "outscatter", sol::nil);
			auto inscatter = getOptionalParam<sol::table>(params, "inscatter", sol::nil);

			if (outscatter == sol::nil || outscatter.size() != 3 || inscatter == sol::nil || inscatter.size() != 3) {
				return false;
			}

			for (int i = 3; i > 0; i--) {
				Stack::getInstance().pushFloat(inscatter[i]);
			}

			for (int i = 3; i > 0; i--) {
				Stack::getInstance().pushFloat(outscatter[i]);
			}

			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::xSetWeatherScattering);
			return true;
		};
		lua_mge["getWeatherScattering"] = []() {
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			auto& state = stateHandle.state;
			auto inscatter = state.create_table();
			auto outscatter = state.create_table();

			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::xGetWeatherScattering);
			inscatter[3] = Stack::getInstance().popFloat();
			inscatter[2] = Stack::getInstance().popFloat();
			inscatter[1] = Stack::getInstance().popFloat();
			outscatter[3] = Stack::getInstance().popFloat();
			outscatter[2] = Stack::getInstance().popFloat();
			outscatter[1] = Stack::getInstance().popFloat();

			return std::make_tuple(outscatter, inscatter);
		};
		lua_mge["getWeatherDLFog"] = [](int weatherID) {
			Stack::getInstance().pushLong(weatherID);
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEGetWeatherDLFog);

			auto fogOffset = Stack::getInstance().popFloat();
			auto fogDistMult = Stack::getInstance().popFloat();
			return std::make_tuple(fogDistMult, fogOffset);
		};
		lua_mge["setWeatherDLFog"] = [](int weatherID, float fogDistMult, float fogOffset) {
			Stack::getInstance().pushFloat(fogOffset);
			Stack::getInstance().pushFloat(fogDistMult);
			Stack::getInstance().pushLong(weatherID);
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGESetWeatherDLFog);
		};
		lua_mge["getWeatherPPLLight"] = [](int weatherID) {
			Stack::getInstance().pushLong(weatherID);
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGEGetWeatherPPLLight);

			auto ambMult = Stack::getInstance().popFloat();
			auto sunMult = Stack::getInstance().popFloat();
			return std::make_tuple(sunMult, ambMult);
		};
		lua_mge["setWeatherPPLLight"] = [](int weatherID, float sunMult, float ambMult) {
			Stack::getInstance().pushFloat(ambMult);
			Stack::getInstance().pushFloat(sunMult);
			Stack::getInstance().pushLong(weatherID);
			mwscript::RunOriginalOpCode(nullptr, nullptr, OpCode::MGESetWeatherPPLLight);
		};
	}
}
