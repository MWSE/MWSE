#include "TES3ContainerLua.h"

#include "LuaManager.h"
#include "TES3ActorLua.h"

#include "TES3Container.h"
#include "TES3Script.h"

namespace mwse {
	namespace lua {
		void bindTES3Container() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Binding for TES3::Container.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<TES3::Container>("tes3container");
				usertypeDefinition["new"] = sol::no_constructor;

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition[sol::base_classes] = sol::bases<TES3::Actor, TES3::PhysicalObject, TES3::Object, TES3::BaseObject>();
				setUserdataForTES3Actor(usertypeDefinition);

				// Basic property binding.
				usertypeDefinition["capacity"] = &TES3::Container::capacity;

				// Friendly access to actor flags.
				usertypeDefinition["organic"] = sol::property(&TES3::Container::getIsOrganic, &TES3::Container::setIsOrganic);
				usertypeDefinition["respawns"] = sol::property(&TES3::Container::getRespawns, &TES3::Container::setRespawns);

				// Constant values.
				usertypeDefinition["isInstance"] = sol::var(false);

				// Functions exposed as properties.
				usertypeDefinition["mesh"] = sol::property(&TES3::Container::getModelPath, &TES3::Container::setModelPath);
				usertypeDefinition["name"] = sol::property(&TES3::Container::getName, &TES3::Container::setName);
				usertypeDefinition["script"] = sol::readonly_property(&TES3::Container::getScript);

				// TODO: Deprecated. Remove before 2.1-stable.
				usertypeDefinition["clone"] = &TES3::Container::clone;
				usertypeDefinition["model"] = sol::property(&TES3::Container::getModelPath, &TES3::Container::setModelPath);
			}

			// Binding for TES3::ContainerInstance.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<TES3::ContainerInstance>("tes3containerInstance");
				usertypeDefinition["new"] = sol::no_constructor;

				// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
				usertypeDefinition[sol::base_classes] = sol::bases<TES3::Actor, TES3::PhysicalObject, TES3::Object, TES3::BaseObject>();
				setUserdataForTES3Actor(usertypeDefinition);
				
				// Friendly access to actor flags.
				usertypeDefinition["organic"] = sol::property(&TES3::ContainerInstance::getIsOrganic, &TES3::ContainerInstance::setIsOrganic);
				usertypeDefinition["respawns"] = sol::property(&TES3::ContainerInstance::getRespawns, &TES3::ContainerInstance::setRespawns);

				// Basic property binding.
				usertypeDefinition["baseObject"] = sol::readonly_property(&TES3::ContainerInstance::container);

				// Constant values.
				usertypeDefinition["isInstance"] = sol::var(true);

				// Functions exposed as properties.
				usertypeDefinition["capacity"] = sol::property(&TES3::ContainerInstance::getCapacity, &TES3::ContainerInstance::setCapacity);
				usertypeDefinition["isRespawn"] = sol::readonly_property(&TES3::ContainerInstance::isRespawn);
				usertypeDefinition["mesh"] = sol::property(&TES3::ContainerInstance::getModelPath, &TES3::ContainerInstance::setModelPath);
				usertypeDefinition["name"] = sol::property(&TES3::ContainerInstance::getName, &TES3::ContainerInstance::setName);
				usertypeDefinition["script"] = sol::readonly_property(&TES3::ContainerInstance::getScript);

				// TODO: Deprecated. Remove before 2.1-stable.
				usertypeDefinition["container"] = sol::readonly_property(&TES3::ContainerInstance::container);
				usertypeDefinition["model"] = sol::property(&TES3::ContainerInstance::getModelPath, &TES3::ContainerInstance::setModelPath);
			}
		}
	}
}
