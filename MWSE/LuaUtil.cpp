#include "LuaUtil.h"

#include "LuaManager.h"

#include "Log.h"

#include "NIAmbientLight.h"
#include "NIAVObject.h"
#include "NICamera.h"
#include "NICollisionSwitch.h"
#include "NIDefines.h"
#include "NIDirectionalLight.h"
#include "NIDynamicEffect.h"
#include "NIExtraData.h"
#include "NINode.h"
#include "NIObject.h"
#include "NIObjectNET.h"
#include "NIPick.h"
#include "NIPointLight.h"
#include "NIRTTI.h"
#include "NISpotLight.h"
#include "NISwitchNode.h"
#include "NITextureEffect.h"
#include "NITriShape.h"

#include "TES3Defines.h"
#include "TES3Activator.h"
#include "TES3Alchemy.h"
#include "TES3Apparatus.h"
#include "TES3Armor.h"
#include "TES3Birthsign.h"
#include "TES3BodyPart.h"
#include "TES3Book.h"
#include "TES3Cell.h"
#include "TES3Class.h"
#include "TES3Clothing.h"
#include "TES3Container.h"
#include "TES3Creature.h"
#include "TES3DataHandler.h"
#include "TES3Dialogue.h"
#include "TES3DialogueInfo.h"
#include "TES3Door.h"
#include "TES3Enchantment.h"
#include "TES3Faction.h"
#include "TES3GameFile.h"
#include "TES3GameSetting.h"
#include "TES3GlobalVariable.h"
#include "TES3Ingredient.h"
#include "TES3LeveledList.h"
#include "TES3Light.h"
#include "TES3Lockpick.h"
#include "TES3MagicEffect.h"
#include "TES3MagicEffectInstance.h"
#include "TES3MagicSourceInstance.h"
#include "TES3Misc.h"
#include "TES3MobileCreature.h"
#include "TES3MobileNPC.h"
#include "TES3MobilePlayer.h"
#include "TES3MobileProjectile.h"
#include "TES3MobileSpellProjectile.h"
#include "TES3NPC.h"
#include "TES3Probe.h"
#include "TES3Quest.h"
#include "TES3Race.h"
#include "TES3Reference.h"
#include "TES3Region.h"
#include "TES3RepairTool.h"
#include "TES3Script.h"
#include "TES3Skill.h"
#include "TES3Sound.h"
#include "TES3SoundGenerator.h"
#include "TES3Spell.h"
#include "TES3Static.h"
#include "TES3UIElement.h"
#include "TES3Vectors.h"
#include "TES3Weapon.h"
#include "TES3Weather.h"
#include "TES3WeatherAsh.h"
#include "TES3WeatherBlight.h"
#include "TES3WeatherBlizzard.h"
#include "TES3WeatherClear.h"
#include "TES3WeatherCloudy.h"
#include "TES3WeatherController.h"
#include "TES3WeatherFoggy.h"
#include "TES3WeatherOvercast.h"
#include "TES3WeatherRain.h"
#include "TES3WeatherSnow.h"
#include "TES3WeatherThunder.h"

#include "TES3UIManager.h"

#include "MemoryUtil.h"

namespace mwse {
	namespace lua {
		template <>
		std::string getOptionalParam(sol::optional<sol::table> maybeParams, const char* key, std::string defaultValue) {
			auto value = std::move(defaultValue);

			if (maybeParams) {
				sol::table params = maybeParams.value();
				sol::object maybeValue = params[key];
				if (maybeValue.valid() && maybeValue.is<std::string>()) {
					value = std::move(maybeValue.as<std::string>());
				}
			}

			return std::move(value);
		}

		TES3::BaseObject* getOptionalParamBaseObject(sol::optional<sol::table> maybeParams, const char* key) {
			if (maybeParams) {
				sol::table params = maybeParams.value();
				sol::object maybeObject = params[key];
				if (maybeObject.valid()) {
					if (maybeObject.is<std::string>()) {
						auto obj = TES3::DataHandler::get()->nonDynamicData->resolveObject(maybeObject.as<std::string>().c_str());
						if (obj) {
							return obj->getBaseObject();
						}
					}
					else if (maybeObject.is<TES3::BaseObject>()) {
						return maybeObject.as<TES3::BaseObject*>()->getBaseObject();
					}
					else if (maybeObject.is<TES3::MobileCreature>()) {
						return maybeObject.as<TES3::MobileCreature*>()->creatureInstance->baseCreature;
					}
					else if (maybeObject.is<TES3::MobileNPC>()) {
						return maybeObject.as<TES3::MobileNPC*>()->npcInstance->baseNPC;
					}
				}
			}
			return nullptr;
		}

		TES3::Script* getOptionalParamExecutionScript(sol::optional<sol::table> maybeParams) {
			if (maybeParams) {
				sol::table params = maybeParams.value();
				sol::object maybeScript = params["_script"];
				if (maybeScript.valid()) {
					if (maybeScript.is<std::string>()) {
						return TES3::DataHandler::get()->nonDynamicData->findScriptByName(maybeScript.as<std::string>().c_str());
					}
					else if (maybeScript.is<TES3::Script*>()) {
						return maybeScript.as<TES3::Script*>();
					}
				}
			}

			// Fall back to the currently executing script.
			return LuaManager::getInstance().getCurrentScript();
		}

		TES3::Reference* getOptionalParamExecutionReference(sol::optional<sol::table> maybeParams) {
			TES3::Reference* reference = nullptr;

			if (maybeParams) {
				sol::table params = maybeParams.value();
				sol::object maybeReference = params["reference"];
				if (maybeReference.valid()) {
					if (maybeReference.is<std::string>()) {
						reference = tes3::getReference(maybeReference.as<std::string>());
					}
					else if (maybeReference.is<TES3::Reference*>()) {
						reference = maybeReference.as<TES3::Reference*>();
					}
					else if (maybeReference.is<TES3::MobileActor*>()) {
						reference = maybeReference.as<TES3::MobileActor*>()->reference;
					}
				}
			}

			if (reference == nullptr) {
				reference = LuaManager::getInstance().getCurrentReference();
			}

			return reference;
		}

		TES3::Script* getOptionalParamScript(sol::optional<sol::table> maybeParams, const char* key) {
			if (maybeParams) {
				sol::table params = maybeParams.value();
				sol::object maybeValue = params[key];
				if (maybeValue.valid()) {
					if (maybeValue.is<std::string>()) {
						return TES3::DataHandler::get()->nonDynamicData->findScriptByName(maybeValue.as<std::string>().c_str());
					}
					else if (maybeValue.is<TES3::Script*>()) {
						return maybeValue.as<TES3::Script*>();
					}
				}
			}

			return NULL;
		}

		TES3::Reference* getOptionalParamReference(sol::optional<sol::table> maybeParams, const char* key) {
			TES3::Reference* value = nullptr;

			if (maybeParams) {
				sol::table params = maybeParams.value();
				sol::object maybeValue = params[key];
				if (maybeValue.valid()) {
					if (maybeValue.is<std::string>()) {
						value = tes3::getReference(maybeValue.as<std::string>());
					}
					else if (maybeValue.is<TES3::Reference*>()) {
						value = maybeValue.as<TES3::Reference*>();
					}
					else if (maybeValue.is<TES3::MobileActor*>()) {
						value = maybeValue.as<TES3::MobileActor*>()->reference;
					}
				}
			}

			return value;
		}

		TES3::MobileActor* getOptionalParamMobileActor(sol::optional<sol::table> maybeParams, const char* key) {
			TES3::MobileActor* value = nullptr;

			if (maybeParams) {
				sol::table params = maybeParams.value();
				sol::object maybeValue = params[key];
				if (maybeValue.valid()) {
					if (maybeValue.is<std::string>()) {
						TES3::Reference * reference = tes3::getReference(maybeValue.as<std::string>());
						if (reference) {
							value = reference->getAttachedMobileActor();
						}
					}
					else if (maybeValue.is<TES3::Reference*>()) {
						value = maybeValue.as<TES3::Reference*>()->getAttachedMobileActor();
					}
					else if (maybeValue.is<TES3::MobileActor*>()) {
						value = maybeValue.as<TES3::MobileActor*>();
					}
				}
			}

			return value;
		}

		TES3::Spell* getOptionalParamSpell(sol::optional<sol::table> maybeParams, const char* key) {
			TES3::Spell* value = nullptr;

			if (maybeParams) {
				sol::table params = maybeParams.value();
				sol::object maybeValue = params[key];
				if (maybeValue.valid()) {
					if (maybeValue.is<std::string>()) {
						value = TES3::DataHandler::get()->nonDynamicData->getSpellById(maybeValue.as<std::string>().c_str());
					}
					else if (maybeValue.is<TES3::Spell*>()) {
						value = maybeValue.as<TES3::Spell*>();
					}
				}
			}

			return value;
		}

		TES3::Dialogue* getOptionalParamDialogue(sol::optional<sol::table> maybeParams, const char* key) {
			TES3::Dialogue* value = nullptr;

			if (maybeParams) {
				sol::table params = maybeParams.value();
				sol::object maybeValue = params[key];
				if (maybeValue.valid()) {
					if (maybeValue.is<const char*>()) {
						value = TES3::DataHandler::get()->nonDynamicData->findDialogue(maybeValue.as<const char*>());
					}
					else if (maybeValue.is<TES3::Dialogue*>()) {
						value = maybeValue.as<TES3::Dialogue*>();
					}
				}
			}

			return value;
		}

		TES3::Sound* getOptionalParamSound(sol::optional<sol::table> maybeParams, const char* key) {
			TES3::Sound* value = nullptr;

			if (maybeParams) {
				sol::table params = maybeParams.value();
				sol::object maybeValue = params[key];
				if (maybeValue.valid()) {
					if (maybeValue.is<std::string>()) {
						value = TES3::DataHandler::get()->nonDynamicData->findSound(maybeValue.as<std::string>().c_str());
					}
					else if (maybeValue.is<TES3::Sound*>()) {
						value = maybeValue.as<TES3::Sound*>();
					}
				}
			}

			return value;
		}

		sol::optional<TES3::Vector2> getOptionalParamVector2(sol::optional<sol::table> maybeParams, const char* key) {
			if (maybeParams) {
				sol::table params = maybeParams.value();
				sol::object maybeValue = params[key];
				if (maybeValue.valid()) {
					// Were we given a real vector?
					if (maybeValue.is<TES3::Vector2>()) {
						return maybeValue.as<TES3::Vector2>();
					}
					// Were we given a vector3 for some reason?
					else if (maybeValue.is<TES3::Vector3>()) {
						return TES3::Vector2(maybeValue.as<TES3::Vector2&>().x, maybeValue.as<TES3::Vector2&>().y);
					}
					// Were we given a table?
					else if (maybeValue.get_type() == sol::type::table) {
						return maybeValue.as<sol::table>();
					}
				}
			}

			return {};
		}

		sol::optional<TES3::Vector3> getOptionalParamVector3(sol::optional<sol::table> maybeParams, const char* key) {
			if (maybeParams) {
				sol::table params = maybeParams.value();
				sol::object maybeValue = params[key];
				if (maybeValue.valid()) {
					// Were we given a real vector?
					if (maybeValue.is<TES3::Vector3*>()) {
						return *maybeValue.as<TES3::Vector3*>();
					}

					// Were we given a table?
					else if (maybeValue.get_type() == sol::type::table) {
						sol::table value = maybeValue.as<sol::table>();
						return TES3::Vector3(value[1], value[2], value[3]);
					}
				}
			}

			return sol::optional<TES3::Vector3>();
		}

		TES3::Cell* getOptionalParamCell(sol::optional<sol::table> maybeParams, const char* key) {
			TES3::Cell* value = nullptr;

			if (maybeParams) {
				sol::table params = maybeParams.value();
				sol::object maybeValue = params[key];
				if (maybeValue.valid()) {
					if (maybeValue.is<const char*>()) {
						value = TES3::DataHandler::get()->nonDynamicData->getCellByName(maybeValue.as<const char*>());
					}
					else if (maybeValue.is<TES3::Cell*>()) {
						value = maybeValue.as<TES3::Cell*>();
					}
					else if (maybeValue.get_type() == sol::type::table) {
						sol::table coordsTable = maybeValue.as<sol::table>();
						if (coordsTable.size() == 2) {
							value = TES3::DataHandler::get()->nonDynamicData->getCellByGrid(coordsTable[1], coordsTable[2]);
						}
					}
				}
			}

			return value;
		}

		const TES3::UI::UI_ID TES3_UI_ID_NULL = static_cast<TES3::UI::UI_ID>(TES3::UI::Property::null);

		TES3::UI::Property getPropertyFromObject(sol::object object) {
			if (object.is<TES3::UI::Property>()) {
				return object.as<TES3::UI::Property>();
			}
			else if (object.is<const char*>()) {
				return TES3::UI::registerProperty(object.as<const char*>());
			}

			throw std::invalid_argument("Could not determine property from value.");
		}

		TES3::UI::UI_ID getUIIDFromObject(sol::object object) {
			if (object.valid()) {
				if (object.is<TES3::UI::UI_ID>()) {
					return object.as<TES3::UI::UI_ID>();
				}
				else if (object.is<const char*>()) {
					return TES3::UI::registerID(object.as<const char*>());
				}
			}

			return TES3_UI_ID_NULL;
		}

		TES3::UI::UI_ID getOptionalUIID(sol::optional<sol::table> maybeParams, const char* key) {
			if (maybeParams) {
				return getUIIDFromObject(maybeParams.value()[key]);
			}
			return TES3_UI_ID_NULL;
		}

		bool setVectorFromLua(TES3::Vector2& vector, sol::stack_object value) {
			// Is it a vector?
			if (value.is<TES3::Vector2>()) {
				vector = value.as<TES3::Vector2&>();
				return true;
			}
			// Allow a simple table to be provided.
			else if (value.get_type() == sol::type::table) {
				vector = value.as<sol::table>();
				return true;
			}

			return false;
		}

		bool setVectorFromLua(TES3::Vector3& vector, sol::stack_object value) {
			// Is it a vector?
			if (value.is<TES3::Vector3*>()) {
				vector = value.as<TES3::Vector3&>();
				return true;
			}
			// Allow a simple table to be provided.
			else if (value.get_type() == sol::type::table) {
				vector = value.as<sol::table>();
				return true;
			}

			return false;
		}

		void logStackTrace(const char* message) {
			if (message != nullptr) {
				log::getLog() << message << std::endl;
			}

			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;
			sol::protected_function_result result = state["debug"]["traceback"]();
			if (result.valid()) {
				sol::optional<std::string> asString = result;
				if (asString) {
					log::getLog() << asString.value() << std::endl;
				}
			}
		}
	}
}
