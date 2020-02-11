#include "sol.hpp"
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

#include "BitUtil.h"

namespace mwse {
	namespace lua {
		void bindScriptUtil() {
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			//
			// Get context on the current execution.
			//

			state["mwscript"]["getScript"] = []() {
				return LuaManager::getInstance().getCurrentScript();
			};

			state["mwscript"]["getReference"] = []() {
				return LuaManager::getInstance().getCurrentReference();
			};

			//
			// Expose base game opcodes.
			//

			state["mwscript"]["activate"] = [](sol::optional<sol::table> params) {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				TES3::Reference* reference = getOptionalParamExecutionReference(params);

				mwscript::Activate(script, reference);
			};
			state["mwscript"]["addItem"] = [](sol::optional<sol::table> params) {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				TES3::Reference* reference = getOptionalParamExecutionReference(params);
				TES3::Item* item = getOptionalParamObject<TES3::Item>(params, "item");
				int count = getOptionalParam<double>(params, "count", 1.0);
				if (item == NULL || count <= 0) {
					return false;
				}

				mwscript::AddItem(script, reference, item, count);
				reference->setObjectModified(true);
				reference->baseObject->setObjectModified(true);
				return true;
			};
			state["mwscript"]["addSoulGem"] = [](sol::optional<sol::table> params) {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				TES3::Reference* reference = getOptionalParamExecutionReference(params);
				TES3::Creature* creature = getOptionalParamObject<TES3::Creature>(params, "creature");
				TES3::Misc* soulGem = getOptionalParamObject<TES3::Misc>(params, "soulgem");
				if (creature == NULL || soulGem == NULL) {
					return false;
				}

				mwscript::AddSoulGem(script, reference, creature, soulGem);
				return true;
			};
			state["mwscript"]["addToLevCreature"] = [](sol::optional<sol::table> params) {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				TES3::Reference* reference = getOptionalParamExecutionReference(params);
				TES3::LeveledCreature* list = getOptionalParamObject<TES3::LeveledCreature>(params, "list");
				TES3::Actor* actor = getOptionalParamObject<TES3::Actor>(params, "creature");
				short level = getOptionalParam<double>(params, "level", 0.0);
				if (list == NULL || actor == NULL || level <= 0) {
					return false;
				}

				mwscript::AddToLevCreature(script, reference, list, actor, level);
				return true;
			};
			state["mwscript"]["addSpell"] = [](sol::optional<sol::table> params) {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				TES3::Reference* reference = getOptionalParamExecutionReference(params);
				TES3::Spell* spell = getOptionalParamSpell(params, "spell");
				if (spell == NULL) {
					return false;
				}

				mwscript::AddSpell(script, reference, spell);
				return true;
			};
			state["mwscript"]["addToLevItem"] = [](sol::optional<sol::table> params) {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				TES3::Reference* reference = getOptionalParamExecutionReference(params);
				TES3::BaseObject* list = getOptionalParamObject<TES3::BaseObject>(params, "list");
				TES3::Item* item = getOptionalParamObject<TES3::Item>(params, "item");
				short level = getOptionalParam<double>(params, "level", 0.0);
				if (list == NULL || item == NULL || level <= 0) {
					return false;
				}

				mwscript::AddToLevItem(script, reference, list, item, level);
				return true;
			};
			state["mwscript"]["addTopic"] = [](sol::optional<sol::table> params) {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				TES3::Reference* reference = getOptionalParamExecutionReference(params);
				TES3::Dialogue* topic = getOptionalParamDialogue(params, "topic");
				if (topic == nullptr || topic->type != TES3::DialogueType::Topic) {
					return false;
				}

				mwscript::AddTopic(script, reference, topic);
				return true;
			};
			state["mwscript"]["aiTravel"] = [](sol::optional<sol::table> params) {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				TES3::Reference* reference = getOptionalParamExecutionReference(params);
				double x = getOptionalParam<double>(params, "x", 0.0);
				double y = getOptionalParam<double>(params, "y", 0.0);
				double z = getOptionalParam<double>(params, "z", 0.0);

				mwscript::AITravel(script, reference, x, y, z);
				return true;
			};
			state["mwscript"]["disable"] = [](sol::optional<sol::table> params) {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				TES3::Reference* reference = getOptionalParamExecutionReference(params);
				bool setModified = getOptionalParam<bool>(params, "modify", true);

				if (setModified) {
					reference->setObjectModified(true);
				}

				mwscript::Disable(script, reference);
				return true;
			};
			state["mwscript"]["drop"] = [](sol::optional<sol::table> params) {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				TES3::Reference* reference = getOptionalParamExecutionReference(params);
				TES3::Item* item = getOptionalParamObject<TES3::Item>(params, "item");
				int count = getOptionalParam<double>(params, "count", 1.0);
				if (item == NULL) {
					return false;
				}

				mwscript::Drop(script, reference, item, count);
				return true;
			};
			state["mwscript"]["enable"] = [](sol::optional<sol::table> params) {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				TES3::Reference* reference = getOptionalParamExecutionReference(params);
				bool setModified = getOptionalParam<bool>(params, "modify", true);

				if (reference == NULL) {
					return false;
				}

				if (setModified) {
					reference->setObjectModified(true);
				}

				mwscript::Enable(script, reference);
				return true;
			};
			state["mwscript"]["equip"] = [](sol::optional<sol::table> params) {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				TES3::Reference* reference = getOptionalParamExecutionReference(params);
				TES3::Item* item = getOptionalParamObject<TES3::Item>(params, "item");
				if (item == NULL || reference == NULL) {
					return false;
				}

				// Fire off the event, because script calls don't hit the same code as our hooks.
				if (event::EquipEvent::getEventEnabled()) {
					auto& luaManager = mwse::lua::LuaManager::getInstance();
					auto stateHandle = luaManager.getThreadSafeStateHandle();
					sol::object response = stateHandle.triggerEvent(new event::EquipEvent(reference, item, NULL));
					if (response.get_type() == sol::type::table) {
						sol::table eventData = response;
						if (eventData["block"] == true) {
							return false;
						}
					}
				}

				mwscript::Equip(script, reference, item);
				return true;
			};
			state["mwscript"]["explodeSpell"] = [](sol::optional<sol::table> params) {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				TES3::Reference* reference = getOptionalParamExecutionReference(params);
				TES3::Spell* spell = getOptionalParamSpell(params, "spell");
				if (spell == NULL) {
					return false;
				}

				mwscript::ExplodeSpell(script, reference, spell);
				return true;
			};
			state["mwscript"]["getButtonPressed"] = [](sol::optional<sol::table> params) {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				TES3::Reference* reference = getOptionalParamExecutionReference(params);

				return mwscript::GetButtonPressed(script, reference);
			};
			state["mwscript"]["hasItemEquipped"] = [](sol::optional<sol::table> params) {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				TES3::Reference* reference = getOptionalParamExecutionReference(params);
				TES3::Item* item = getOptionalParamObject<TES3::Item>(params, "item");
				if (item == NULL) {
					return false;
				}

				return mwscript::HasItemEquipped(script, reference, item);
			};
			// Obsolete. Do not document.
			state["mwscript"]["getDelete"] = [](sol::optional<sol::table> params) {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				TES3::Reference* reference = getOptionalParamExecutionReference(params);

				return BITMASK_TEST(reference->objectFlags, TES3::ObjectFlag::Delete);
			};
			state["mwscript"]["getDetected"] = [](sol::optional<sol::table> params) {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				TES3::Reference* reference = getOptionalParamExecutionReference(params);
				TES3::Reference* target = getOptionalParamReference(params, "target");
				if (reference == NULL || target == NULL) {
					return false;
				}

				return mwscript::GetDetected(script, reference, target);
			};
			state["mwscript"]["getDisabled"] = [](sol::optional<sol::table> params) {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				TES3::Reference* reference = getOptionalParamExecutionReference(params);
				return mwscript::GetDisabled(script, reference);
			};
			state["mwscript"]["getDistance"] = [](sol::optional<sol::table> params) {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				TES3::Reference* reference = getOptionalParamExecutionReference(params);
				TES3::Reference* target = getOptionalParamReference(params, "target");
				if (reference == NULL || target == NULL) {
					return -1.0f;
				}

				return mwscript::GetDistance(script, reference, target);
			};
			state["mwscript"]["getItemCount"] = [](sol::optional<sol::table> params) -> int {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				TES3::Reference* reference = getOptionalParamExecutionReference(params);
				TES3::Item* item = getOptionalParamObject<TES3::Item>(params, "item");
				if (item == NULL) {
					return 0;
				}

				return mwscript::GetItemCount(script, reference, item);
			};
			state["mwscript"]["getPCJumping"] = [](sol::optional<sol::table> params) {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				return mwscript::GetPCJumping(script);
			};
			state["mwscript"]["getPCRunning"] = [](sol::optional<sol::table> params) {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				return mwscript::GetPCRunning(script);
			};
			state["mwscript"]["getPCSneaking"] = [](sol::optional<sol::table> params) {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				return mwscript::GetPCSneaking(script);
			};
			state["mwscript"]["getSpellEffects"] = [](sol::optional<sol::table> params) {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				TES3::Reference* reference = getOptionalParamExecutionReference(params);
				TES3::Spell* spell = getOptionalParamSpell(params, "spell");
				if (spell == NULL) {
					return false;
				}

				return mwscript::GetSpellEffects(script, reference, spell);
			};
			// Obsolete. Do not document.
			state["mwscript"]["onActivate"] = [](sol::optional<sol::table> params) {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				TES3::Reference* reference = getOptionalParamExecutionReference(params);

				bool result = reference->testActionFlag(TES3::ActionFlags::OnActivate);
				reference->clearActionFlag(TES3::ActionFlags::UseEnabled);
				reference->clearActionFlag(TES3::ActionFlags::OnActivate);
				return result;
			};
			// Obsolete. Do not document.
			state["mwscript"]["onDeath"] = [](sol::optional<sol::table> params) {
				TES3::Reference* reference = getOptionalParamExecutionReference(params);

				bool result = reference->testActionFlag(TES3::ActionFlags::OnDeath);
				reference->clearActionFlag(TES3::ActionFlags::OnDeath);
				return result;
			};
			// Obsolete. Do not document.
			state["mwscript"]["onKnockout"] = [](sol::optional<sol::table> params) {
				TES3::Reference* reference = getOptionalParamExecutionReference(params);

				bool result = reference->testActionFlag(TES3::ActionFlags::OnKnockout);
				reference->clearActionFlag(TES3::ActionFlags::OnKnockout);
				return result;
			};
			// Obsolete. Do not document.
			state["mwscript"]["onMurder"] = [](sol::optional<sol::table> params) {
				TES3::Reference* reference = getOptionalParamExecutionReference(params);

				bool result = reference->testActionFlag(TES3::ActionFlags::OnMurder);
				reference->clearActionFlag(TES3::ActionFlags::OnMurder);
				return result;
			};
			state["mwscript"]["placeAtPC"] = [](sol::optional<sol::table> params) -> sol::object {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				TES3::Reference* reference = getOptionalParamExecutionReference(params);
				TES3::BaseObject* object = getOptionalParamObject<TES3::BaseObject>(params, "object");
				int count = getOptionalParam<double>(params, "count", 1.0);
				double distance = getOptionalParam<double>(params, "distance", 256.0);
				double direction = getOptionalParam<double>(params, "direction", 1.0);
				if (object == NULL) {
					return sol::nil;
				}

				mwscript::PlaceAtPC(script, reference, object, count, distance, direction);
				return makeLuaObject(mwscript::lastCreatedPlaceAtPCReference);
			};
			state["mwscript"]["playSound"] = [](sol::optional<sol::table> params) {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				TES3::Reference* reference = getOptionalParamExecutionReference(params);
				TES3::Sound* sound = getOptionalParamSound(params, "sound");
				if (sound == NULL) {
					return false;
				}

				mwscript::PlaySound(script, reference, sound);
				return true;
			};
			state["mwscript"]["position"] = [](sol::optional<sol::table> params) {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				TES3::Reference* reference = getOptionalParamExecutionReference(params);
				double x = getOptionalParam<double>(params, "x", 0.0);
				double y = getOptionalParam<double>(params, "y", 0.0);
				double z = getOptionalParam<double>(params, "z", 0.0);
				double rotation = getOptionalParam<double>(params, "rotation", 0.0);

				mwscript::Position(script, reference, x, y, z, rotation);
				return true;
			};
			state["mwscript"]["positionCell"] = [](sol::optional<sol::table> params) {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				TES3::Reference* reference = getOptionalParamExecutionReference(params);
				double x = getOptionalParam<double>(params, "x", 0.0);
				double y = getOptionalParam<double>(params, "y", 0.0);
				double z = getOptionalParam<double>(params, "z", 0.0);
				double rotation = getOptionalParam<double>(params, "rotation", 0.0);
				std::string cell = getOptionalParam<std::string>(params, "cell", "");
				if (cell.empty()) {
					return false;
				}

				mwscript::PositionCell(script, reference, x, y, z, rotation, cell.c_str());
				return true;
			};
			state["mwscript"]["removeFromLevCreature"] = [](sol::optional<sol::table> params) {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				TES3::Reference* reference = getOptionalParamExecutionReference(params);
				TES3::LeveledCreature* list = getOptionalParamObject<TES3::LeveledCreature>(params, "list");
				TES3::Actor* actor = getOptionalParamObject<TES3::Actor>(params, "creature");
				short level = getOptionalParam<double>(params, "level", 0.0);
				if (list == NULL || actor == NULL || level <= 0) {
					return false;
				}

				mwscript::RemoveFromLevCreature(script, reference, list, actor, level);
				return true;
			};
			state["mwscript"]["removeFromLevItem"] = [](sol::optional<sol::table> params) {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				TES3::Reference* reference = getOptionalParamExecutionReference(params);
				TES3::BaseObject* list = getOptionalParamObject<TES3::BaseObject>(params, "list");
				TES3::Item* item = getOptionalParamObject<TES3::Item>(params, "item");
				short level = getOptionalParam<double>(params, "level", 0.0);
				if (list == NULL || item == NULL || level <= 0) {
					return false;
				}

				mwscript::RemoveFromLevItem(script, reference, list, item, level);
				return true;
			};
			state["mwscript"]["removeItem"] = [](sol::optional<sol::table> params) {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				TES3::Reference* reference = getOptionalParamExecutionReference(params);
				TES3::Item* item = getOptionalParamObject<TES3::Item>(params, "item");
				int count = getOptionalParam<double>(params, "count", 1.0);
				if (item == NULL) {
					return false;
				}

				mwscript::RemoveItem(script, reference, item, count);
				reference->setObjectModified(true);
				reference->baseObject->setObjectModified(true);
				return true;
			};
			state["mwscript"]["removeSpell"] = [](sol::optional<sol::table> params) {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				TES3::Reference* reference = getOptionalParamExecutionReference(params);
				TES3::Spell* spell = getOptionalParamSpell(params, "spell");
				if (spell == NULL) {
					return false;
				}

				mwscript::RemoveSpell(script, reference, spell);
				return true;
			};
			state["mwscript"]["scriptRunning"] = [](sol::optional<sol::table> params) {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				TES3::Script* targetScript = getOptionalParamScript(params, "script");
				if (targetScript == NULL) {
					return false;
				}

				return mwscript::ScriptRunning(script, targetScript);
			};
			// Obsolete. Do not document.
			state["mwscript"]["setDelete"] = [](sol::optional<sol::table> params) {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				TES3::Reference* reference = getOptionalParamExecutionReference(params);

				bool del = getOptionalParam<bool>(params, "delete", true);
				BITMASK_SET_ON(reference->objectFlags, TES3::ObjectFlag::Delete);

				reference->setObjectModified(true);

				return true;
			};
			state["mwscript"]["setLevel"] = [](sol::optional<sol::table> params) {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				TES3::Reference* reference = getOptionalParamExecutionReference(params);
				short level = getOptionalParam<double>(params, "level", 0);
				if (level <= 0) {
					return false;
				}

				mwscript::SetLevel(script, reference, level);
				return true;
			};
			state["mwscript"]["startCombat"] = [](sol::optional<sol::table> params) {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				TES3::Reference* reference = getOptionalParamExecutionReference(params);
				TES3::Reference* target = getOptionalParamReference(params, "target");
				if (target == NULL) {
					return false;
				}

				mwscript::StartCombat(script, reference, target);
				return true;
			};
			state["mwscript"]["startScript"] = [](sol::optional<sol::table> params) {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				TES3::Reference* reference = getOptionalParamExecutionReference(params);
				TES3::Script* targetScript = getOptionalParamScript(params, "script");
				if (targetScript == NULL) {
					return false;
				}

				mwscript::StartScript(script, reference, targetScript);
				return true;
			};
			state["mwscript"]["stopCombat"] = [](sol::optional<sol::table> params) {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				TES3::Reference* reference = getOptionalParamExecutionReference(params);
				if (reference == NULL) {
					return false;
				}

				mwscript::StopCombat(script, reference);
				return true;
			};
			state["mwscript"]["stopScript"] = [](sol::optional<sol::table> params) {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				TES3::Reference* reference = getOptionalParamExecutionReference(params);
				TES3::Script* targetScript = getOptionalParamScript(params, "script");
				if (targetScript == NULL) {
					targetScript = script;
				}

				if (targetScript) {
					mwscript::StopScript(script, targetScript);
					return true;
				}

				return false;
			};
			state["mwscript"]["stopSound"] = [](sol::optional<sol::table> params) {
				TES3::Script* script = getOptionalParamExecutionScript(params);
				TES3::Reference* reference = getOptionalParamExecutionReference(params);
				TES3::Sound* sound = getOptionalParamSound(params, "sound");
				if (sound == NULL) {
					return false;
				}

				mwscript::StopSound(script, reference, sound);
				return true;
			};
			state["mwscript"]["wakeUpPC"] = []() {
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::WakeUpPC);
			};

			//
			// MGE opcodes.
			//

			// General functions.
			state["mge"]["getScreenHeight"] = []() {
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEGetHeight);
				return Stack::getInstance().popLong();
			};
			state["mge"]["getScreenWidth"] = []() {
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEGetWidth);
				return Stack::getInstance().popLong();
			};
			state["mge"]["getVersion"] = []() {
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEGetVersion);
				return Stack::getInstance().popLong();
			};
			state["mge"]["log"] = [](std::string string) {
				Stack::getInstance().pushString(string);
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEOutputDebugString);
			};

			// HUD-related functions.
			state["mge"]["clearHUD"] = []() {
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEWipeAll);
			};
			state["mge"]["disableHUD"] = [](sol::optional<sol::table> params) {
				std::string hud = getOptionalParam<std::string>(params, "hud", "");

				if (!hud.empty()) {
					Stack::getInstance().pushString(hud);
					mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEDisableHUD);
				}
				else {
					mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGENIDDisableHUD);
				}

				return true;
			};
			state["mge"]["enableHUD"] = [](sol::optional<sol::table> params) {
				std::string hud = getOptionalParam<std::string>(params, "hud", "");

				if (!hud.empty()) {
					Stack::getInstance().pushString(hud);
					mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEEnableHUD);
				}
				else {
					mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGENIDEnableHUD);
				}

				return true;
			};
			state["mge"]["freeHUD"] = [](sol::optional<sol::table> params) {
				std::string hud = getOptionalParam<std::string>(params, "hud", "");

				if (!hud.empty()) {
					Stack::getInstance().pushString(hud);
					mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEFreeHUD);
				}
				else {
					mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGENIDFreeHUD);
				}

				return true;
			};
			state["mge"]["fullscreenHUD"] = [](sol::optional<sol::table> params) {
				std::string hud = getOptionalParam<std::string>(params, "hud", "");

				if (!hud.empty()) {
					Stack::getInstance().pushString(hud);
					mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEFullscreenHUD);
				}
				else {
					mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGENIDFullscreenHUD);
				}

				return true;
			};
			state["mge"]["loadHUD"] = [](sol::optional<sol::table> params) {
				std::string hud = getOptionalParam<std::string>(params, "hud", "");
				std::string texture = getOptionalParam<std::string>(params, "texture", "");
				if (hud.empty() || texture.empty()) {
					return false;
				}

				Stack::getInstance().pushString(texture);
				Stack::getInstance().pushString(hud);
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGELoadHUD);

				if (getOptionalParam<bool>(params, "enable", false)) {
					Stack::getInstance().pushString(hud);
					mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEEnableHUD);
				}

				return true;
			};
			state["mge"]["positionHUD"] = [](sol::optional<sol::table> params) {
				std::string hud = getOptionalParam<std::string>(params, "hud", "");
				double x = getOptionalParam<double>(params, "x", 0.0);
				double y = getOptionalParam<double>(params, "y", 0.0);

				if (!hud.empty()) {
					Stack::getInstance().pushFloat(y);
					Stack::getInstance().pushFloat(x);
					Stack::getInstance().pushString(hud);
					mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEPositionHUD);
				}
				else {
					Stack::getInstance().pushFloat(y);
					Stack::getInstance().pushFloat(x);
					mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGENIDPositionHUD);
				}

				return true;
			};
			state["mge"]["scaleHUD"] = [](sol::optional<sol::table> params) {
				std::string hud = getOptionalParam<std::string>(params, "hud", "");
				double x = getOptionalParam<double>(params, "x", 0.0);
				double y = getOptionalParam<double>(params, "y", 0.0);

				if (!hud.empty()) {
					Stack::getInstance().pushFloat(y);
					Stack::getInstance().pushFloat(x);
					Stack::getInstance().pushString(hud);
					mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEScaleHUD);
				}
				else {
					Stack::getInstance().pushFloat(y);
					Stack::getInstance().pushFloat(x);
					mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGENIDScaleHUD);
				}

				return true;
			};
			state["mge"]["selectHUD"] = [](sol::optional<sol::table> params) {
				std::string hud = getOptionalParam<std::string>(params, "hud", "");
				if (hud.empty()) {
					return false;
				}

				Stack::getInstance().pushString(hud);
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEWithHUD);
				return true;
			};
			state["mge"]["setHUDEffect"] = [](sol::optional<sol::table> params) {
				std::string hud = getOptionalParam<std::string>(params, "hud", "");
				std::string effect = getOptionalParam<std::string>(params, "effect", "");
				if (effect.empty()) {
					return false;
				}

				if (!hud.empty()) {
					Stack::getInstance().pushString(effect);
					Stack::getInstance().pushString(hud);
					mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEChangeHUDEffect);
				}
				else {
					Stack::getInstance().pushString(effect);
					mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGENIDChangeHUDEffect);
				}

				return true;
			};
			state["mge"]["setHUDEffectFloat"] = [](sol::optional<sol::table> params) {
				std::string hud = getOptionalParam<std::string>(params, "hud", "");
				std::string variable = getOptionalParam<std::string>(params, "variable", "");
				float value = getOptionalParam<double>(params, "value", 0.0);
				if (variable.empty()) {
					return false;
				}

				if (!hud.empty()) {
					Stack::getInstance().pushString(hud);
					mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEWithHUD);
				}

				Stack::getInstance().pushFloat(value);
				Stack::getInstance().pushString(variable);
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGENIDSetHUDEffectFloat);

				return true;
			};
			state["mge"]["setHUDEffectLong"] = [](sol::optional<sol::table> params) {
				std::string hud = getOptionalParam<std::string>(params, "hud", "");
				std::string variable = getOptionalParam<std::string>(params, "variable", "");
				long value = getOptionalParam<double>(params, "value", 0.0);
				if (variable.empty()) {
					return false;
				}

				if (!hud.empty()) {
					Stack::getInstance().pushString(hud);
					mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEWithHUD);
				}

				Stack::getInstance().pushLong(value);
				Stack::getInstance().pushString(variable);
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGENIDSetHUDEffectLong);

				return true;
			};
			state["mge"]["setHUDEffectVector4"] = [](sol::optional<sol::table> params) {
				std::string hud = getOptionalParam<std::string>(params, "hud", "");
				std::string variable = getOptionalParam<std::string>(params, "variable", "");
				sol::table values = getOptionalParam<sol::table>(params, "value", sol::nil);
				if (variable.empty() || values == sol::nil || values.size() != 4) {
					return false;
				}

				if (!hud.empty()) {
					Stack::getInstance().pushString(hud);
					mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEWithHUD);
				}

				for (int i = 4; i > 0; i--) {
					Stack::getInstance().pushFloat(values[i]);
				}
				Stack::getInstance().pushString(variable);
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGENIDSetHUDEffectVec);

				return true;
			};
			state["mge"]["setHUDTexture"] = [](sol::optional<sol::table> params) {
				std::string hud = getOptionalParam<std::string>(params, "hud", "");
				std::string texture = getOptionalParam<std::string>(params, "texture", "");
				if (texture.empty()) {
					return false;
				}

				if (!hud.empty()) {
					Stack::getInstance().pushString(texture);
					Stack::getInstance().pushString(hud);
					mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEChangeHUDTexture);
				}
				else {
					Stack::getInstance().pushString(texture);
					mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGENIDChangeHUDTexture);
				}

				return true;
			};
			state["mge"]["unselectHUD"] = [](sol::optional<sol::table> params) {
				std::string hud = getOptionalParam<std::string>(params, "hud", "");
				if (hud.empty()) {
					return false;
				}

				Stack::getInstance().pushString(hud);
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGECancelWithHUD);
				return true;
			};

			// Shader-related functions.
			state["mge"]["disableShader"] = [](sol::optional<sol::table> params) {
				std::string shader = getOptionalParam<std::string>(params, "shader", "");
				if (shader.empty()) {
					return false;
				}

				Stack::getInstance().pushString(shader);
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEDisableShader);

				return true;
			};
			state["mge"]["enableShader"] = [](sol::optional<sol::table> params) {
				std::string shader = getOptionalParam<std::string>(params, "shader", "");
				if (shader.empty()) {
					return false;
				}

				Stack::getInstance().pushString(shader);
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEEnableShader);

				return true;
			};
			state["mge"]["setShaderFloat"] = [](sol::optional<sol::table> params) {
				std::string shader = getOptionalParam<std::string>(params, "shader", "");
				std::string variable = getOptionalParam<std::string>(params, "variable", "");
				float value = getOptionalParam<double>(params, "value", 0.0);
				if (shader.empty() || variable.empty()) {
					return false;
				}

				Stack::getInstance().pushFloat(value);
				Stack::getInstance().pushString(variable);
				Stack::getInstance().pushString(shader);
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEShaderSetFloat);

				return true;
			};
			state["mge"]["setShaderLong"] = [](sol::optional<sol::table> params) {
				std::string shader = getOptionalParam<std::string>(params, "shader", "");
				std::string variable = getOptionalParam<std::string>(params, "variable", "");
				long value = getOptionalParam<double>(params, "value", 0.0);
				if (shader.empty() || variable.empty()) {
					return false;
				}

				Stack::getInstance().pushLong(value);
				Stack::getInstance().pushString(variable);
				Stack::getInstance().pushString(shader);
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEShaderSetLong);

				return true;
			};
			state["mge"]["setShaderVector4"] = [](sol::optional<sol::table> params) {
				std::string hud = getOptionalParam<std::string>(params, "shader", "");
				std::string variable = getOptionalParam<std::string>(params, "variable", "");
				sol::table values = getOptionalParam<sol::table>(params, "value", sol::nil);
				if (variable.empty() || values == sol::nil || values.size() != 4) {
					return false;
				}

				if (!hud.empty()) {
					Stack::getInstance().pushString(hud);
					mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEWithHUD);
				}

				for (int i = 4; i > 0; i--) {
					Stack::getInstance().pushFloat(values[i]);
				}
				Stack::getInstance().pushString(variable);
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEShaderSetVector);

				return true;
			};

			// Camera zoom functions.
			state["mge"]["disableZoom"] = []() {
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEDisableZoom);
			};
			state["mge"]["enableZoom"] = []() {
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEEnableZoom);
			};
			state["mge"]["toggleZoom"] = []() {
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEToggleZoom);
			};
			state["mge"]["zoomIn"] = [](sol::optional<sol::table> params) {
				double amount = getOptionalParam<double>(params, "amount", 0.0);

				if (amount == 0.0) {
					mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEZoomIn);
				}
				else {
					Stack::getInstance().pushFloat(amount);
					mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEZoomInBy);
				}
			};
			state["mge"]["zoomOut"] = [](sol::optional<sol::table> params) {
				double amount = getOptionalParam<double>(params, "amount", 0.0);

				if (amount == 0.0) {
					mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEZoomOut);
				}
				else {
					Stack::getInstance().pushFloat(amount);
					mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEZoomOutBy);
				}
			};
			state["mge"]["setZoom"] = [](sol::optional<sol::table> params) {
				double amount = getOptionalParam<double>(params, "amount", 0.0);
				bool animate = getOptionalParam<double>(params, "animate", false);

				if (animate) {
					Stack::getInstance().pushFloat(amount);
					mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEZoom);
				}
				else {
					Stack::getInstance().pushFloat(amount);
					mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGESetZoom);
				}
			};
			state["mge"]["getZoom"] = []() {
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEGetZoom);
				return Stack::getInstance().popFloat();
			};
			state["mge"]["stopZoom"] = []() {
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEStopZoom);
			};

			// Camera shake functions.
			state["mge"]["enableCameraShake"] = [](sol::optional<sol::table> params) {
				double magnitude = getOptionalParam<double>(params, "magnitude", 0.0);
				if (magnitude != 0.0) {
					Stack::getInstance().pushFloat(magnitude);
					mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGESetCameraShakeMagnitude);
				}

				double acceleration = getOptionalParam<double>(params, "acceleration", 0.0);
				if (acceleration != 0.0) {
					Stack::getInstance().pushFloat(acceleration);
					mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGECameraShakeZoom);
				}

				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEEnableCameraShake);
			};
			state["mge"]["disableCameraShake"] = []() {
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEDisableCameraShake);
			};
			state["mge"]["setCameraShakeMagnitude"] = [](sol::optional<sol::table> params) {
				double magnitude = getOptionalParam<double>(params, "magnitude", 0.0);
				Stack::getInstance().pushFloat(magnitude);
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGESetCameraShakeMagnitude);
			};
			state["mge"]["setCameraShakeAcceleration"] = [](sol::optional<sol::table> params) {
				double acceleration = getOptionalParam<double>(params, "acceleration", 0.0);
				Stack::getInstance().pushFloat(acceleration);
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGECameraShakeZoom);
			};

			// Camera rotation functions.
			state["mge"]["getScreenRotation"] = []() {
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEGetScreenRotation);
				return Stack::getInstance().popFloat();
			};
			state["mge"]["modScreenRotation"] = [](sol::optional<sol::table> params) {
				double rotation = getOptionalParam<double>(params, "rotation", 0.0);
				Stack::getInstance().pushFloat(rotation);
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGERotateScreenBy);
			};
			state["mge"]["setScreenRotation"] = [](sol::optional<sol::table> params) {
				double rotation = getOptionalParam<double>(params, "rotation", 0.0);
				Stack::getInstance().pushFloat(rotation);
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGERotateScreen);
			};
			state["mge"]["startScreenRotation"] = []() {
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEScreenSpin);
			};
			state["mge"]["stopScreenRotation"] = []() {
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEStopSpinSpin);
			};

			// MGE XE rendering functions.
			state["mge"]["setWeatherScattering"] = [](sol::optional<sol::table> params) {
				sol::table outscatter = getOptionalParam<sol::table>(params, "outscatter", sol::nil);
				sol::table inscatter = getOptionalParam<sol::table>(params, "inscatter", sol::nil);

				if (outscatter == sol::nil || outscatter.size() != 3 || inscatter == sol::nil || inscatter.size() != 3) {
					return false;
				}

				for (int i = 3; i > 0; i--) {
					Stack::getInstance().pushFloat(inscatter[i]);
				}
				for (int i = 3; i > 0; i--) {
					Stack::getInstance().pushFloat(outscatter[i]);
				}
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::xSetWeatherScattering);
				return true;
			};
			state["mge"]["getWeatherScattering"] = []() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table inscatter = state.create_table();
				sol::table outscatter = state.create_table();

				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::xGetWeatherScattering);
				inscatter[3] = Stack::getInstance().popFloat();
				inscatter[2] = Stack::getInstance().popFloat();
				inscatter[1] = Stack::getInstance().popFloat();
				outscatter[3] = Stack::getInstance().popFloat();
				outscatter[2] = Stack::getInstance().popFloat();
				outscatter[1] = Stack::getInstance().popFloat();

				return std::make_tuple(outscatter, inscatter);
			};
			state["mge"]["getWeatherDLFog"] = [](int weatherID) {
				Stack::getInstance().pushLong(weatherID);
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEGetWeatherDLFog);

				float fogOffset = Stack::getInstance().popFloat();
				float fogDistMult = Stack::getInstance().popFloat();
				return std::make_tuple(fogDistMult, fogOffset);
			};
			state["mge"]["setWeatherDLFog"] = [](int weatherID, float fogDistMult, float fogOffset) {
				Stack::getInstance().pushFloat(fogOffset);
				Stack::getInstance().pushFloat(fogDistMult);
				Stack::getInstance().pushLong(weatherID);
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGESetWeatherDLFog);
			};
			state["mge"]["getWeatherPPLLight"] = [](int weatherID) {
				Stack::getInstance().pushLong(weatherID);
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGEGetWeatherPPLLight);

				float ambMult = Stack::getInstance().popFloat();
				float sunMult = Stack::getInstance().popFloat();
				return std::make_tuple(sunMult, ambMult);
			};
			state["mge"]["setWeatherPPLLight"] = [](int weatherID, float sunMult, float ambMult) {
				Stack::getInstance().pushFloat(ambMult);
				Stack::getInstance().pushFloat(sunMult);
				Stack::getInstance().pushLong(weatherID);
				mwscript::RunOriginalOpCode(NULL, NULL, OpCode::MGESetWeatherPPLLight);
			};
		}
	}
}
