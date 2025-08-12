#pragma once

#include "TES3Defines.h"

namespace TES3 {
	struct WearablePart {
		int bodypartID; // 0x0 // TODO: This is actually a single byte. Need to solve in a way that doesn't break existing mods.
		BodyPart * male; // 0x4
		BodyPart * female; // 0x8

		WearablePart();

		BodyPart* getPart(bool isFemale) const;
		bool isValid() const;
	};
	static_assert(sizeof(WearablePart) == 0xC, "TES3::WearablePart failed size validation");
}
