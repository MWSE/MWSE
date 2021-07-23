#pragma once

#include "TES3Defines.h"

#include "NIDefines.h"

namespace TES3 {
	struct Moon {
		NI::Node * sgNode; // 0x0
		NI::Node * sgMoonNode; // 0x4
		NI::Node * sgShadowNode; // 0x8
		NI::TriShape * unknown_0xC;
		NI::TriShape * sgTriMoonShadow; // 0x10
		WeatherController * weatherController; // 0x14
		float fadeInStart; // 0x18
		float fadeInFinish; // 0x1C
		float fadeOutStart; // 0x20
		float fadeOutFinish; // 0x24
		float shadowEarlyFadeAngle; // 0x28
		int unknown_0x2C;
		int unknown_0x30;
		float fadeStartAngle; // 0x34
		float fadeEndAngle; // 0x38
		float dailyIncrement; // 0x3C
		float speed; // 0x40
		int unknown_0x44;
		char unknown_0x48;
		char unknown_0x49;
		bool moonIsRed; // 0x4A
		char unknown_0x4B;
		int phase; // 0x4C
		int index; // 0x50
		char texturePath[260];
		float axisOffset; // 0x158

		//
		// Custom functions.
		//

		const char* getTexturePath() const;
		void setTexturePath(const char*);

	};
	static_assert(sizeof(Moon) == 0x15C, "TES3::Moon failed size validation");
}
