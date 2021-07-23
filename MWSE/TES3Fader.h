#pragma once

#include "TES3Defines.h"
#include "TES3Vectors.h"

#include "NIObject.h"

namespace TES3 {
	typedef void(__stdcall* FaderCallback)();

	struct Fader {
		struct Node {
			float duration; // 0x0
			float target; // 0x4
			FaderCallback callback; // 0x8
			Fader::Node * next; // 0xC
		};
		bool isActive; // 0x0
		float fadeToAlpha; // 0x04;
		NI::Pointer<NI::Node> node; // 0x8
		NI::Pointer<NI::AVObject> geometry; // 0xC // Actual type is NiTriShape.
		int unknown_0x10;
		int unknown_0x14;
		int unknown_0x18;
		float fadeRate; // 0x1C
		float fadeDuration; // 0x20
		int unknown_0x24;
		int fadeDirection; // 0x28
		float fadeCurrentAlpha; // 0x2C
		Fader::Node * queueTail;
		Fader::Node * queueHead;
		bool unknown_0x38;
		void (__cdecl *callback)(); // 0x3C

		//
		// Other related this-call functions.
		//

		Fader();
		Fader(float unknownFloat, bool unknownBool = true);
		~Fader();

		void activate();
		void deactivate();
		
		void updateForFrame();
		
		void fadeTo(float value, float duration, FaderCallback callback = nullptr);

		void setColor(TES3::Vector3 color, bool alpha);
		void updateMaterialProperty(float value);
		void setTexture(const char* path);

		//
		// Custom functions.
		//

		void setActive(bool value);

		void fadeIn_lua(sol::optional<sol::table> params);
		void fadeOut_lua(sol::optional<sol::table> params);
		void fadeTo_lua(sol::optional<sol::table> params);

		bool setColor_lua(sol::table params);

	};
	static_assert(sizeof(Fader) == 0x40, "TES3::Fader failed size validation");
	static_assert(sizeof(Fader::Node) == 0x10, "TES3::Fader::Node failed size validation");
}
