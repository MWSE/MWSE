#pragma once

#include "NIFlipController.h"
#include "NIProperty.h"
#include "NITriShape.h"

namespace TES3 {
	struct WaterController {
		struct NVLake {

		};
		struct Ripple {
			NI::Pointer<NI::TriShape> triShape; // 0x0
			NI::Pointer<NI::TimeController> alphaController; // 0x4
			bool isActive; // 0x8
		};
		NVLake* waterNVLake; // 0x0
		bool pixelShaderEnabled; // 0x4
		bool waterShown; // 0x5
		const char* rippleTexturePath; // 0x8
		NI::Pointer<NI::FlipController> flipController; // 0xC
		NI::Pointer<NI::TexturingProperty> texturingProperty; // 0x10
		NI::Pointer<NI::AlphaProperty> alphaProperty; // 0x14
		unsigned int surfaceFrameCount; // 0x18
		unsigned int rippleFrameCount; // 0x1C
		int unknown_0x20;
		int unknown_0x24;
		float timing; // 0x28
		int surfaceFPS; // 0x2C
		int unknown_0x30;
		float rippleLifetime; // 0x34
		float rippleScaleX; // 0x38
		float rippleScaleY; // 03C
		float rippleAlphas[3]; // 0x40
		float rippleRotationSpeed; // 0x4C
		unsigned int maxRippleCount; // 0x50
		Ripple* ripples; // 0x54
		float nearWaterRadius; // 0x58
		float nearWaterPoints; // 0x5C
		float nearWaterUnderwaterFrequency; // 0x60
		float nearWaterUnderwaterVolume; // 0x64
		float nearWaterIndoorTolerance; // 0x68
		float nearWaterOutdoorTolerance; // 0x6C
		char nearWaterIndoorSoundId[32]; // 0x70
		char nearWaterOutdoorSoundId[32]; // 0x90
		NI::Pointer<NI::Node> rippleNode; // 0xB0
		NI::Pointer<NI::Node> waterPlane; // 0xB4
		NI::TriShape** waterTriShapes; // 0xB8
		const char* surfaceTexturePath; // 0xBC
		unsigned int surfaceTileCount; // 0xC0
		unsigned int tileTextureDivisor; // 0xC4

		//
		// Other related this-call functions.
		//

		void createRipple(float x, float y, float scale, float speed, bool active = true);
		void setRainFrequency(float frequency);
		void createRipple_lua(sol::optional<sol::table> params);

		//
		// Lua helper functions.
		//

		std::reference_wrapper<float[3]> getRippleAlphas();
		nonstd::span<Ripple> getRipples();

	};
	static_assert(sizeof(WaterController) == 0xC8, "TES3::WaterController failed size validation");
	static_assert(sizeof(WaterController::Ripple) == 0xC, "TES3::WaterController::Ripple failed size validation");
}
