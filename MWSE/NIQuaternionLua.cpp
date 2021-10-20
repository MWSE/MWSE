#include "NIQuaternionLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "NIQuaternion.h"

void mwse::lua::bindNIQuaternion() {
	// Get our lua state.
	auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
	sol::state& state = stateHandle.state;

	// NiQuaternion
	{
		// Start our usertype.
		auto usertypeDefinition = state.new_usertype<NI::Quaternion>("niQuaternion");
		usertypeDefinition["new"] = sol::constructors<NI::Quaternion(), NI::Quaternion(float, float, float, float)>();

		// Operator overloading.
		usertypeDefinition[sol::meta_function::to_string] = &NI::Quaternion::toString;
		usertypeDefinition["__tojson"] = &NI::Quaternion::toJson;
		usertypeDefinition[sol::meta_function::multiplication] = sol::overload(
			sol::resolve<NI::Quaternion(const NI::Quaternion&) const>(&NI::Quaternion::operator*)
		);

		// Basic property binding.
		usertypeDefinition["w"] = &NI::Quaternion::w;
		usertypeDefinition["x"] = &NI::Quaternion::x;
		usertypeDefinition["y"] = &NI::Quaternion::y;
		usertypeDefinition["z"] = &NI::Quaternion::z;

		// Basic function binding.
		usertypeDefinition["fromRotation"] = &NI::Quaternion::fromRotation;
		usertypeDefinition["toRotation"] = &NI::Quaternion::toRotation;
		usertypeDefinition["fromAngleAxis"] = &NI::Quaternion::fromAngleAxis;
		usertypeDefinition["toAngleAxis"] = &NI::Quaternion::toAngleAxis;
		usertypeDefinition["invert"] = &NI::Quaternion::invert;
		usertypeDefinition["slerp"] = &NI::Quaternion::slerp;
		usertypeDefinition["copy"] = &NI::Quaternion::copy;
	}
}
