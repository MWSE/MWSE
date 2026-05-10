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


#if defined(SE_NI_FLIPCONTROLLER_FNADDR_GETTEXTUREATINDEX) && SE_NI_FLIPCONTROLLER_FNADDR_GETTEXTUREATINDEX > 0
		Texture* getTextureAtIndex(size_t index) const;
		static constexpr auto _getTextureAtIndex = reinterpret_cast<Texture*(__thiscall*)(const FlipController*, size_t)>(SE_NI_FLIPCONTROLLER_FNADDR_GETTEXTUREATINDEX);
#endif

#if defined(SE_NI_FLIPCONTROLLER_FNADDR_SETTEXTURE) && SE_NI_FLIPCONTROLLER_FNADDR_SETTEXTURE > 0
		void setTexture(Texture* texture, size_t index);
		static constexpr auto _setTexture = reinterpret_cast<void(__thiscall*)(FlipController*, Texture*, size_t)>(SE_NI_FLIPCONTROLLER_FNADDR_SETTEXTURE);
#endif

#if defined(SE_NI_FLIPCONTROLLER_FNADDR_UPDATETIMINGS) && SE_NI_FLIPCONTROLLER_FNADDR_UPDATETIMINGS > 0
		void updateTimings();
		static constexpr auto _updateTimings = reinterpret_cast<void(__thiscall*)(FlipController*)>(SE_NI_FLIPCONTROLLER_FNADDR_UPDATETIMINGS);
#endif

#if defined(SE_NI_FLIPCONTROLLER_FNADDR_COPY) && SE_NI_FLIPCONTROLLER_FNADDR_COPY > 0
		void copy(FlipController* to) const;
		static constexpr auto _copy = reinterpret_cast<void(__thiscall*)(const FlipController*, FlipController*)>(SE_NI_FLIPCONTROLLER_FNADDR_COPY);
#endif
	};
	static_assert(sizeof(FlipController) == 0x60, "NI::FlipController failed size validation");
}
