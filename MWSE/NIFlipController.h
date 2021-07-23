#pragma once

#include "NITimeController.h"
#include "NITexture.h"

namespace NI {
	struct FlipController : TimeController {
		TArray<Pointer<Texture>> textures; // 0x34
		unsigned int currentIndex; // 0x4C
		unsigned int affectedMap; // 0x50
		float flipStartTime; // 0x54
		float duration; // 0x58
		float secondsPerFrame; // 0x5C

		//
		// Other related this-call functions.
		//

		Texture* getTextureAtIndex(size_t index) const;
		void setTexture(Texture* texture, size_t index);
		void updateTimings();

		//
		// Custom functions.
		//

		void copy(FlipController* to) const;

		//
		// Access to this type's raw functions.
		//

		static constexpr auto _getTextureAtIndex = reinterpret_cast<Texture*(__thiscall*)(const FlipController*, size_t)>(0x7159D0);
		static constexpr auto _setTexture = reinterpret_cast<void(__thiscall*)(FlipController*, Texture*, size_t)>(0x715840);
		static constexpr auto _updateTimings = reinterpret_cast<void(__thiscall*)(FlipController*)>(0x7159F0);
		static constexpr auto _copy = reinterpret_cast<void(__thiscall*)(const FlipController*, FlipController*)>(0x715D40);

	};
	static_assert(sizeof(FlipController) == 0x60, "NI::FlipController failed size validation");
}
