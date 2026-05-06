#pragma once

#include "NIDefines.h"

namespace NI {
	struct PixelFormat {
		enum struct Format : int {
			RGB = 0,
			RGBA = 1,
			PALETTE = 2,
			PALETTE_ALPHA = 3,
			BGR = 4,
			BGRA = 5,
			COMPRESS1 = 6,
			COMPRESS3 = 7,
			COMPRESS5 = 8,
			RGB24_NONINTERLEAVED = 9,
			BUMP = 10,
			BUMPLUMA = 11
		};

		Format format;
		unsigned int channelMasks[4];
		unsigned int bitsPerPixel;
		unsigned int compareBits[2];

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
		// MWSE-side API (relies on D3DFORMAT from d3d8.h, force-included via MWSE stdafx.h).
		// Implementations live in MWSE-private NIPixelFormat.cpp; declarations published here
		// so that MWSE-private callers see them through the SharedSE include path after redirect.
		PixelFormat(Format format);
		PixelFormat(unsigned int maskR, unsigned int maskG, unsigned int maskB, unsigned int maskA, int bitsPerPixel);
		PixelFormat(bool hasAlpha);
		PixelFormat(D3DFORMAT d3dFormat);

		D3DFORMAT getD3DFormat() const;
#endif
	};
	static_assert(sizeof(PixelFormat) == 0x20, "NI::PixelFormat failed size validation");
}
