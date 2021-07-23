#include "TES3SoundLua.h"

#include "LuaManager.h"
#include "TES3ObjectLua.h"

#include "TES3Sound.h"
#include "TES3SoundGenerator.h"

namespace mwse {
	namespace lua {
		void bindTES3Sound() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Binding for TES3::Sound.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<TES3::Sound>("tes3sound");
				usertypeDefinition["new"] = sol::no_constructor;

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition[sol::base_classes] = sol::bases<TES3::BaseObject>();
				setUserdataForTES3BaseObject(usertypeDefinition);

				// Base class overrides.
				usertypeDefinition[sol::meta_function::to_string] = &TES3::Sound::getObjectID;
				usertypeDefinition["id"] = sol::readonly_property(&TES3::Sound::getObjectID);

				// Allow object to be serialized to json.
				usertypeDefinition["__tojson"] = &TES3::Sound::toJson;

				// Access to other objects that need to be packaged.
				usertypeDefinition["filename"] = sol::readonly_property(&TES3::Sound::filename);

				// Basic function binding.
				usertypeDefinition["isLooping"] = &TES3::Sound::isLooping;
				usertypeDefinition["isPlaying"] = &TES3::Sound::isPlaying;
				usertypeDefinition["play"] = &TES3::Sound::play_lua;
				usertypeDefinition["stop"] = &TES3::Sound::stop;

				// Expose float-based volume control.
				usertypeDefinition["maxDistance"] = sol::property(&TES3::Sound::getMaxDistance, &TES3::Sound::setMaxDistance_lua);
				usertypeDefinition["minDistance"] = sol::property(&TES3::Sound::getMinDistance, &TES3::Sound::setMinDistance_lua);
				usertypeDefinition["volume"] = sol::property(&TES3::Sound::getVolume, &TES3::Sound::setVolume);
			}

			// Binding for TES3::SoundGenerator.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<TES3::SoundGenerator>("tes3soundGenerator");
				usertypeDefinition["new"] = sol::no_constructor;

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition[sol::base_classes] = sol::bases<TES3::BaseObject>();
				setUserdataForTES3BaseObject(usertypeDefinition);

				// Base class overrides.
				usertypeDefinition[sol::meta_function::to_string] = &TES3::SoundGenerator::getObjectID;
				usertypeDefinition["id"] = sol::readonly_property(&TES3::SoundGenerator::getObjectID);

				// Allow object to be serialized to json.
				usertypeDefinition["__tojson"] = &TES3::SoundGenerator::toJson;

				// Basic property binding.
				usertypeDefinition["sound"] = sol::readonly_property(&TES3::SoundGenerator::sound);
				usertypeDefinition["type"] = sol::readonly_property(&TES3::SoundGenerator::soundType);
			}
		}
	}
}
