#include "NITexture.h"

namespace NI {
	Texture::FormatPrefs::FormatPrefs() {
		pixelLayout = PixelLayout::PIX_DEFAULT;
		mipMapped = MipFlag::MIP_DEFAULT;
		alphaFormat = AlphaFormat::ALPHA_DEFAULT;
	}

	Texture::FormatPrefs::FormatPrefs(PixelLayout p, MipFlag m, AlphaFormat a) {
		pixelLayout = p;
		mipMapped = m;
		alphaFormat = a;
	}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
	const Texture::FormatPrefs Texture::FormatPrefs::DEFAULT_LUA_PREFS = Texture::FormatPrefs(PixelLayout::TRUE_COLOR_32, MipFlag::NO, AlphaFormat::SMOOTH);
#endif

	unsigned int Texture::getWidth() {
		return vTable.asTexture->getWidth(this);
	}

	unsigned int Texture::getHeight() {
		return vTable.asTexture->getHeight(this);
	}
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::Texture)
#endif
