#pragma once

#include "NIDefines.h"

#include "NIObject.h"
#include "NIPixelFormat.h"
#include "NIPointer.h"

namespace NI {
	struct PixelData : Object {
#pragma pack(push, 1)
		struct HeaderTGA {
			enum struct DataType : unsigned char {
				NO_IMAGE_DATA = 0,
				UNCOMPRESSED_PALLETIZED = 1,
				UNCOMPRESSED_RGB = 2,
				UNCOMPRESSED_BLACK_WHITE = 3,
				RLE_PALLETIZED = 9,
				RLE_RGB = 10,
				COMPRESSED_BLACK_WHITE = 11,
				COMPRESSED_1 = 32,
				COMPRESSED_2 = 33
			};
			unsigned char idLength;
			unsigned char colormapType;
			DataType datatypeCode;
			short colormapOrigin;
			short colormapLength;
			unsigned char colormapDepth;
			short xOrigin;
			short yOrigin;
			short width;
			short height;
			unsigned char bitsPerPixel;
			unsigned char imageDescriptor;
		};
#pragma pack(pop)

		PixelFormat pixelFormat;
		void * palette; // 0x28
		unsigned char * pixels; // 0x2C // The raw pixel data, in the given format.
		unsigned int * widths; // 0x30 // An array of widths by mip map level.
		unsigned int * heights; // 0x34 // An array of heights by mip map level.
		unsigned int * offsets; // 0x38 // Offsets into 'pixels' for each mip map level, including 0 (length = mipMapLevels + 1).
		unsigned int mipMapLevels; // 0x3C // The number of mip map levels.
		unsigned int bytesPerPixel; // 0x40 // Determined by format data.
		unsigned int revisionID;

		static Pointer<PixelData> create(unsigned int width, unsigned int height, unsigned int mipMapLevels = 1);
		static Pointer<PixelData> create_lua(unsigned int width, unsigned int height, sol::optional<unsigned int> mipMapLevels);

		//
		// Custom functions.
		//

		Pointer<SourceTexture> createSourceTexture();

		unsigned int getHeight(unsigned int mipMapLevel = 0) const;
		unsigned int getWidth(unsigned int mipMapLevel = 0) const;
		void exportTGA(const char* fileName) const;

		unsigned int getHeight_lua(sol::optional<unsigned int> mipMapLevel) const;
		unsigned int getWidth_lua(sol::optional<unsigned int> mipMapLevel) const;
		void setPixelsByte_lua(sol::table data, sol::optional<unsigned int> mipMapLevel);
		void setPixelsFloat_lua(sol::table data, sol::optional<unsigned int> mipMapLevel);
		void fill_lua(sol::table data, sol::optional<unsigned int> mipMapLevel);

 	};
	static_assert(sizeof(PixelData) == 0x48, "NI::PixelData failed size validation");

	struct PixelRGB {
		unsigned char r;
		unsigned char g;
		unsigned char b;
	};
	static_assert(sizeof(PixelRGB) == 0x3, "NI::PixelRGB failed size validation");

	struct PixelRGBA : PixelRGB {
		unsigned char a;
	};
	static_assert(sizeof(PixelRGBA) == 0x4, "NI::PixelRGBA failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::PixelData)
