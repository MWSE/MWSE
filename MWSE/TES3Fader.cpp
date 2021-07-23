#include "TES3Fader.h"

#include "TES3Util.h"
#include "LuaUtil.h"

#include "TES3WorldController.h"

#include "NINode.h"

#include "Log.h"

namespace TES3 {
	static int faderCount = 5;

	const auto TES3_Fader_ctor = reinterpret_cast<Fader*(__thiscall*)(Fader*, float, bool)>(0x4091B0);
	Fader::Fader() {
		auto worldController = TES3::WorldController::get();

		float distance = 1130.0f;
		if (worldController && worldController->nodeCursor) {
			float adjustedDistance = worldController->nodeCursor->localTranslate.y - worldController->projectionDistance;
			float distanceDifference = adjustedDistance * 0.0625f;
			distance = adjustedDistance * 0.5f + worldController->projectionDistance + distanceDifference * faderCount++;
		}

		TES3_Fader_ctor(this, distance, true);

		fadeTo(0.0f, 0.0f);
	}

	Fader::Fader(float unknownFloat, bool unknownBool) {
		TES3_Fader_ctor(this, unknownFloat, unknownBool);
		faderCount++;
	}

	const auto TES3_Fader_dtor = reinterpret_cast<void(__thiscall*)(Fader*)>(0x409210);
	Fader::~Fader() {
		// Fader destructor tries to detach itself from the camera in the WorldController. Avoid this on exit to prevent a crash.
		if (TES3::WorldController::get() != nullptr) {
			TES3_Fader_dtor(this);
		}
	}

	const auto TES3_Fader_activate = reinterpret_cast<void(__thiscall*)(Fader*)>(0x409890);
	void Fader::activate() {
		TES3_Fader_activate(this);
	}

	const auto TES3_Fader_deactivate = reinterpret_cast<void(__thiscall*)(Fader*)>(0x4098D0);
	void Fader::deactivate() {
		TES3_Fader_deactivate(this);
	}

	const auto TES3_Fader_updateForFrame = reinterpret_cast<void(__thiscall*)(Fader*)>(0x409590);
	void Fader::updateForFrame() {
		TES3_Fader_updateForFrame(this);
	}

	const auto TES3_Fader_fadeTo = reinterpret_cast<void(__thiscall*)(Fader*, float, float, FaderCallback)>(0x4096B0);
	void Fader::fadeTo(float value, float duration, FaderCallback callback) {
		TES3_Fader_fadeTo(this, value, duration, callback);
	}

	const auto TES3_Fader_setColor = reinterpret_cast<void(__thiscall*)(Fader*, TES3::Vector3, bool)>(0x40A230);
	void Fader::setColor(TES3::Vector3 color, bool alpha) {
		TES3_Fader_setColor(this, color, alpha);
	}

	const auto TES3_Fader_updateMaterialProperty = reinterpret_cast<void(__thiscall*)(Fader*, float)>(0x4098F0);
	void Fader::updateMaterialProperty(float value) {
		TES3_Fader_updateMaterialProperty(this, value);
	}

	const auto TES3_Fader_setTexture = reinterpret_cast<void(__thiscall*)(Fader*, const char*)>(0x40A3E0);
	void Fader::setTexture(const char* path) {
		TES3_Fader_setTexture(this, path);
	}

	void Fader::setActive(bool value) {
		if (value) {
			activate();
		}
		else {
			deactivate();
		}
	}

	void Fader::fadeIn_lua(sol::optional<sol::table> params) {
		float duration = mwse::lua::getOptionalParam(params, "duration", 1.0f);
		fadeTo(1.0f, duration);
	}

	void Fader::fadeOut_lua(sol::optional<sol::table> params) {
		float duration = mwse::lua::getOptionalParam(params, "duration", 1.0f);
		fadeTo(0.0f, duration);
	}

	void Fader::fadeTo_lua(sol::optional<sol::table> params) {
		float value = mwse::lua::getOptionalParam(params, "value", 1.0f);
		float duration = mwse::lua::getOptionalParam(params, "duration", 1.0f);
		fadeTo(value, duration);
	}

	bool Fader::setColor_lua(sol::table params) {
		sol::optional <TES3::Vector3> color = mwse::lua::getOptionalParamVector3(params, "color");
		if (!color) {
			return false;
		}

		setColor(color.value(), mwse::lua::getOptionalParam<bool>(params, "flag", false));

		return true;
	}
}
