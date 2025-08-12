#include "NICameraLua.h"
#include "NIObjectLua.h"

#include "LuaManager.h"

#include "NICamera.h"

namespace mwse::lua {
	void bindNICamera() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Start our usertype.
		auto usertypeDefinition = state.new_usertype<NI::Camera>("niCamera");
		usertypeDefinition["new"] = sol::no_constructor;

		// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
		usertypeDefinition[sol::base_classes] = sol::bases<NI::AVObject, NI::ObjectNET, NI::Object>();
		setUserdataForNIAVObject(usertypeDefinition);

		// Basic property binding.
		usertypeDefinition["lodAdjust"] = &NI::Camera::LODAdjust;
		usertypeDefinition["port"] = &NI::Camera::port;
		usertypeDefinition["renderer"] = sol::readonly_property(&NI::Camera::renderer);
		usertypeDefinition["scene"] = &NI::Camera::scene;
		usertypeDefinition["viewDistance"] = &NI::Camera::viewDistance;
		usertypeDefinition["worldDirection"] = &NI::Camera::worldDirection;
		usertypeDefinition["worldRight"] = &NI::Camera::worldRight;
		usertypeDefinition["worldToCamera"] = &NI::Camera::worldToCamera;
		usertypeDefinition["worldUp"] = &NI::Camera::worldUp;
		usertypeDefinition["cullingPlanes"] = sol::readonly_property(&NI::Camera::getCullingPlanes_lua);

		// Basic function binding.
		usertypeDefinition["clear"] = &NI::Camera::clear_lua;
		usertypeDefinition["click"] = &NI::Camera::click_lua;
		usertypeDefinition["swapBuffers"] = &NI::Camera::swapBuffers;
		usertypeDefinition["windowPointToRay"] = &NI::Camera::windowPointToRay_lua;
		usertypeDefinition["worldPointToScreenPoint"] = &NI::Camera::worldPointToScreenPoint_lua;
		usertypeDefinition["lookAtWorldPoint"] = &NI::Camera::LookAtWorldPoint;
	}
}
