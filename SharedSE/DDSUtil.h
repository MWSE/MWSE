#pragma once

namespace se::dds {
	struct PixelFormat {
		unsigned int size;
		unsigned int flags;
		unsigned int fourCC;
		unsigned int rgbBitCount;
		unsigned int redMask;
		unsigned int greenMask;
		unsigned int blueMask;
		unsigned int alphaMask;
	};

	struct Header {
		unsigned int size;
		unsigned int flags;
		unsigned int height;
		unsigned int width;
		unsigned int pitchOrLinearSize;
		unsigned int depth;
		unsigned int mipMapCount;
		unsigned int reserved[11];
		PixelFormat pixelFormat;
		unsigned int caps;
		unsigned int caps2;
		unsigned int caps3;
		unsigned int caps4;
		unsigned int reserved2;
	};

	static_assert(sizeof(PixelFormat) == 32, "se::dds::PixelFormat failed size validation");
	static_assert(sizeof(Header) == 124, "se::dds::Header failed size validation");

	constexpr unsigned int MAGIC = 0x20534444;
	constexpr unsigned int PIXEL_ALPHA = 0x1;
	constexpr unsigned int PIXEL_FOURCC = 0x4;
	constexpr unsigned int PIXEL_RGB = 0x40;

	inline bool isUncompressedRgb32(const Header& header) {
		const auto pixelFlags = header.pixelFormat.flags;
		return header.size == sizeof(Header)
			&& header.pixelFormat.size == sizeof(PixelFormat)
			&& (pixelFlags & PIXEL_RGB) != 0
			&& (pixelFlags & PIXEL_FOURCC) == 0
			&& header.pixelFormat.rgbBitCount == 32;
	}

	inline bool hasAlpha(const PixelFormat& pixelFormat) {
		return (pixelFormat.flags & PIXEL_ALPHA) != 0;
	}

	inline unsigned int getMaskShift(unsigned int mask) {
		unsigned int shift = 0;
		while (mask && (mask & 1) == 0) {
			mask >>= 1;
			++shift;
		}
		return shift;
	}

	inline unsigned char getMaskedByte(unsigned int value, unsigned int mask) {
		if (mask == 0) {
			return 0;
		}

		return static_cast<unsigned char>((value & mask) >> getMaskShift(mask));
	}
}
