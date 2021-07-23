#pragma once

#include "NITexture.h"

namespace NI {
	struct RenderedTexture : Texture {
		unsigned int width; // 0x2C
		unsigned int height; // 0x30

		static RenderedTexture* create(unsigned int width, unsigned int height, Renderer* renderer, FormatPrefs* prefs);
	};
	static_assert(sizeof(RenderedTexture) == 0x34, "NI::RenderedTexture failed size validation");
}
