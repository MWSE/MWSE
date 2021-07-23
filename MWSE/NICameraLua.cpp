#include "NICameraLua.h"
#include "NIObjectLua.h"

#include "LuaManager.h"

#include "NICamera.h"

namespace mwse {
	namespace lua {
		void bindNICamera() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.new_usertype<NI::Camera>("niCamera");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::AVObject, NI::ObjectNET, NI::Object>();
			setUserdataForNIAVObject(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["lodAdjust"] = &NI::Camera::LODAdjust;
			usertypeDefinition["port"] = &NI::Camera::port;
			usertypeDefinition["renderer"] = sol::readonly_property(&NI::Camera::renderer);
			usertypeDefinition["scene"] = sol::readonly_property(&NI::Camera::scene);
			usertypeDefinition["viewDistance"] = &NI::Camera::viewDistance;
			usertypeDefinition["worldDirection"] = &NI::Camera::worldDirection;
			usertypeDefinition["worldRight"] = &NI::Camera::worldRight;
			usertypeDefinition["worldToCamera"] = &NI::Camera::worldToCamera;
			usertypeDefinition["worldUp"] = &NI::Camera::worldUp;

			// Basic function binding.
			usertypeDefinition["click"] = &NI::Camera::click_lua;
			usertypeDefinition["windowPointToRay"] = &NI::Camera::windowPointToRay_lua;
			usertypeDefinition["worldPointToScreenPoint"] = &NI::Camera::worldPointToScreenPoint_lua;
		}
	}
}
