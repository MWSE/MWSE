#pragma once

#include "NITexture.h"
#include "NIPixelData.h"

namespace NI {
	struct RenderedTexture : Texture {
		unsigned int width; // 0x2C
		unsigned int height; // 0x30

		// Both methods are implemented only in Morrowind-context builds
		// (require TES3::WorldController + DX8).
		bool readback(NI::PixelData* pixelData);

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
		static Pointer<RenderedTexture> create(unsigned int width, unsigned int height, sol::optional<const FormatPrefs*> prefs);
#endif
	};
	static_assert(sizeof(RenderedTexture) == 0x34, "NI::RenderedTexture failed size validation");
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::RenderedTexture)
#endif
