#include "TES3WaterController.h"

#include "TES3WaterController.h"

#include "LuaManager.h"

namespace mwse::lua {
	void bindTES3WaterController() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Start our usertype.
		auto usertypeDefinition = state.new_usertype<TES3::WaterController>("tes3waterController");
		usertypeDefinition["new"] = sol::no_constructor;

		// Basic property binding.
		usertypeDefinition["alphaProperty"] = &TES3::WaterController::alphaProperty;
		usertypeDefinition["flipController"] = &TES3::WaterController::flipController;
		usertypeDefinition["maxRippleCount"] = &TES3::WaterController::maxRippleCount;
		usertypeDefinition["nearWaterIndoorSoundId"] = sol::readonly_property(&TES3::WaterController::nearWaterIndoorSoundId);
		usertypeDefinition["nearWaterIndoorTolerance"] = &TES3::WaterController::nearWaterIndoorTolerance;
		usertypeDefinition["nearWaterOutdoorSoundId"] = sol::readonly_property(&TES3::WaterController::nearWaterOutdoorSoundId);
		usertypeDefinition["nearWaterOutdoorTolerance"] = &TES3::WaterController::nearWaterOutdoorTolerance;
		usertypeDefinition["nearWaterPoints"] = &TES3::WaterController::nearWaterPoints;
		usertypeDefinition["nearWaterRadius"] = &TES3::WaterController::nearWaterRadius;
		usertypeDefinition["nearWaterUnderwaterFrequency"] = &TES3::WaterController::nearWaterUnderwaterFrequency;
		usertypeDefinition["nearWaterUnderwaterVolume"] = &TES3::WaterController::nearWaterUnderwaterVolume;
		usertypeDefinition["pixelShaderEnabled"] = &TES3::WaterController::pixelShaderEnabled;
		usertypeDefinition["rippleAlphas"] = sol::readonly_property(&TES3::WaterController::getRippleAlphas);
		usertypeDefinition["rippleFrameCount"] = &TES3::WaterController::rippleFrameCount;
		usertypeDefinition["rippleLifetime"] = &TES3::WaterController::rippleLifetime;
		usertypeDefinition["rippleNode"] = &TES3::WaterController::rippleNode;
		usertypeDefinition["rippleRotationSpeed"] = &TES3::WaterController::rippleRotationSpeed;
		usertypeDefinition["ripples"] = sol::readonly_property(&TES3::WaterController::getRipples);
		usertypeDefinition["rippleScaleX"] = &TES3::WaterController::rippleScaleX;
		usertypeDefinition["rippleScaleY"] = &TES3::WaterController::rippleScaleY;
		usertypeDefinition["surfaceFPS"] = &TES3::WaterController::surfaceFPS;
		usertypeDefinition["surfaceFrameCount"] = &TES3::WaterController::surfaceFrameCount;
		usertypeDefinition["surfaceTexturePath"] = sol::readonly_property(&TES3::WaterController::surfaceTexturePath);
		usertypeDefinition["surfaceTileCount"] = &TES3::WaterController::surfaceTileCount;
		usertypeDefinition["texturingProperty"] = &TES3::WaterController::texturingProperty;
		usertypeDefinition["tileTextureDivisor"] = &TES3::WaterController::tileTextureDivisor;
		usertypeDefinition["timing"] = &TES3::WaterController::timing;
		usertypeDefinition["waterPlane"] = &TES3::WaterController::waterPlane;
		usertypeDefinition["waterShown"] = &TES3::WaterController::waterShown;

		// Basic function binding.
		usertypeDefinition["createRipple"] = &TES3::WaterController::createRipple_lua;
	}
}
