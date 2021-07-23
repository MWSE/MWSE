#include "NIRenderedTexture.h"

namespace NI {
	const auto NI_RenderedTexture_create = reinterpret_cast<RenderedTexture* (__cdecl*)(unsigned int, unsigned int, Renderer*, Texture::FormatPrefs*)>(0x6DC090);
	RenderedTexture* RenderedTexture::create(unsigned int width, unsigned int height, Renderer* renderer, FormatPrefs* prefs) {
		return NI_RenderedTexture_create(width, height, renderer, prefs);
	}
}
