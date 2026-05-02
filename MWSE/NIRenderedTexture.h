#pragma once

#include "NITexture.h"

namespace NI {
	struct RenderedTexture : Texture {
		unsigned int width; // 0x2C
		unsigned int height; // 0x30

		static Pointer<RenderedTexture> create(unsigned int width, unsigned int height, sol::optional<const FormatPrefs*> prefs);

		//
		// Custom methods
		//

		bool readback(NI::PixelData* pixelData);
	};
	static_assert(sizeof(RenderedTexture) == 0x34, "NI::RenderedTexture failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::RenderedTexture)
