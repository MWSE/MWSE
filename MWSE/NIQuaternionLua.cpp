#include "NIQuaternionLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "NIQuaternion.h"

void mwse::lua::bindNIQuaternion() {
	// Get our lua state.
	const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
	auto& state = stateHandle.getState();

	// NiQuaternion
	{
		// Start our usertype.
		auto usertypeDefinition = state.new_usertype<NI::Quaternion>("niQuaternion");
		usertypeDefinition["new"] = sol::constructors<NI::Quaternion(), NI::Quaternion(float, float, float, float)>();

		// Operator overloading.
		usertypeDefinition[sol::meta_function::to_string] = &NI::Quaternion::toString;
		usertypeDefinition["__tojson"] = &NI::Quaternion::toJson;
		usertypeDefinition[sol::meta_function::multiplication] = sol::resolve<NI::Quaternion(const NI::Quaternion&) const>(&NI::Quaternion::operator*);
		usertypeDefinition[sol::meta_function::unary_minus] = sol::resolve<NI::Quaternion() const>(&NI::Quaternion::operator-);

		// Basic property binding.
		usertypeDefinition["w"] = &NI::Quaternion::w;
		usertypeDefinition["x"] = &NI::Quaternion::x;
		usertypeDefinition["y"] = &NI::Quaternion::y;
		usertypeDefinition["z"] = &NI::Quaternion::z;

		// Basic function binding.
		usertypeDefinition["copy"] = &NI::Quaternion::copy;
		usertypeDefinition["dot"] = &NI::Quaternion::dot;
		usertypeDefinition["exp"] = &NI::Quaternion::exp;
		usertypeDefinition["fromAngleAxis"] = &NI::Quaternion::fromAngleAxis;
		usertypeDefinition["fromRotation"] = &NI::Quaternion::fromRotation;
		usertypeDefinition["invert"] = &NI::Quaternion::invert;
		usertypeDefinition["log"] = &NI::Quaternion::log;
		usertypeDefinition["normalize"] = &NI::Quaternion::normalize;
		usertypeDefinition["normalized"] = &NI::Quaternion::normalized;
		usertypeDefinition["rotateTowards"] = &NI::Quaternion::rotateTowards;
		usertypeDefinition["slerp"] = &NI::Quaternion::slerp;
		usertypeDefinition["slerpKeyframe"] = &NI::Quaternion::slerpKeyframe;
		usertypeDefinition["toAngleAxis"] = &NI::Quaternion::toAngleAxis;
		usertypeDefinition["toRotation"] = &NI::Quaternion::toRotation;

		// Alternate Constructors
		usertypeDefinition["identity"] = sol_copy_wrapper(NI::Quaternion::IDENTITY);
	}
}
