#pragma once

#include "NIDefines.h"

#include "TES3Defines.h"
#include "TES3UIDefines.h"

#include "TES3DataHandler.h"
#include "TES3Vectors.h"

namespace mwse {
	namespace lua {
		template <typename T>
		T getOptionalParam(sol::optional<sol::table> maybeParams, const char* key, T defaultValue) {
			T value = defaultValue;

			if (maybeParams) {
				sol::table params = maybeParams.value();
				sol::object maybeValue = params[key];
				if (maybeValue.valid() && maybeValue.is<T>()) {
					value = maybeValue.as<T>();
				}
			}

			return value;
		}

		template <>
		std::string getOptionalParam(sol::optional<sol::table> maybeParams, const char* key, std::string defaultValue);

		template <typename T>
		sol::optional<T> getOptionalParam(sol::optional<sol::table> maybeParams, const char* key) {
			if (maybeParams) {
				sol::table params = maybeParams.value();
				sol::object maybeValue = params[key];
				if (maybeValue.valid() && maybeValue.is<T>()) {
					return maybeValue.as<T>();
				}
			}
			return {};
		}

		template <typename T>
		T* getOptionalParamObject(sol::optional<sol::table> maybeParams, const char* key) {
			T* value = NULL;

			if (maybeParams) {
				sol::table params = maybeParams.value();
				sol::object maybeValue = params[key];
				if (maybeValue.valid()) {
					if (maybeValue.is<std::string>()) {
						value = TES3::DataHandler::get()->nonDynamicData->resolveObjectByType<T>(maybeValue.as<std::string>().c_str());
					}
					else if (maybeValue.is<T*>()) {
						value = maybeValue.as<T*>();
					}
				}
			}

			return value;
		}

		TES3::BaseObject* getOptionalParamBaseObject(sol::optional<sol::table> maybeParams, const char* key);
		TES3::Script* getOptionalParamExecutionScript(sol::optional<sol::table> maybeParams);
		TES3::Reference* getOptionalParamExecutionReference(sol::optional<sol::table> maybeParams);
		TES3::Script* getOptionalParamScript(sol::optional<sol::table> maybeParams, const char* key);
		TES3::Reference* getOptionalParamReference(sol::optional<sol::table> maybeParams, const char* key);
		TES3::MobileActor* getOptionalParamMobileActor(sol::optional<sol::table> maybeParams, const char* key);
		TES3::Spell* getOptionalParamSpell(sol::optional<sol::table> maybeParams, const char* key);
		TES3::Dialogue* getOptionalParamDialogue(sol::optional<sol::table> maybeParams, const char* key);
		TES3::Sound* getOptionalParamSound(sol::optional<sol::table> maybeParams, const char* key);
		sol::optional<TES3::Vector2> getOptionalParamVector2(sol::optional<sol::table> maybeParams, const char* key);
		sol::optional<TES3::Vector3> getOptionalParamVector3(sol::optional<sol::table> maybeParams, const char* key);
		TES3::Cell* getOptionalParamCell(sol::optional<sol::table> maybeParams, const char* key);

		TES3::UI::Property getPropertyFromObject(sol::object object);
		TES3::UI::UI_ID getUIIDFromObject(sol::object object);
		TES3::UI::UI_ID getOptionalUIID(sol::optional<sol::table> maybeParams, const char* key);

		bool setVectorFromLua(TES3::Vector2&, sol::stack_object);
		bool setVectorFromLua(TES3::Vector3&, sol::stack_object);

		// Allow handling a default value as an unsatisfied optional.
		template <typename T>
		sol::optional<T> valueDefaultAsNil(const T& value, const T& defaultValue) {
			if (value == defaultValue) {
				return {};
			}
			return value;
		}

		// Dumps the current stacktrace to the log.
		void logStackTrace(const char* message = nullptr);
	}
}
