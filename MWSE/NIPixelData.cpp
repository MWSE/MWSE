#include "NIPixelData.h"

#include "NIPixelFormat.h"
#include "NISourceTexture.h"
#include "NITexture.h"

#include "Log.h"
#include "MemoryUtil.h"

namespace NI {

	const auto NIPixelData_ctor_args = reinterpret_cast<void(__thiscall*)(PixelData*, unsigned int, unsigned int, const PixelFormat*, unsigned int)>(0x6D4FC0);
	const auto NIPixelFormat_RGBA32 = reinterpret_cast<const PixelFormat*>(0x7DE8B8);

	// Create an RGBA-8bpc PixelData with a single mip level.
	Pointer<PixelData> PixelData::create(unsigned int width, unsigned int height, unsigned int mipMapLevels) {
		PixelData* pixelData = mwse::tes3::_new<PixelData>();
		NIPixelData_ctor_args(pixelData, width, height, NIPixelFormat_RGBA32, mipMapLevels);
		return pixelData;
	}

	Pointer<PixelData> PixelData::create_lua(unsigned int width, unsigned int height, sol::optional<unsigned int> mipMapLevels) {
		return create(width, height, mipMapLevels.value_or(1));
	}

	Pointer<SourceTexture> PixelData::createSourceTexture() {
		using FormatPrefs = Texture::FormatPrefs;
		FormatPrefs prefs = { FormatPrefs::PixelLayout::PIX_DEFAULT, FormatPrefs::MipFlag::MIP_DEFAULT, FormatPrefs::AlphaFormat::ALPHA_DEFAULT };
		return SourceTexture::createFromPixelData(this, &prefs);
	}

	unsigned int PixelData::getHeight(unsigned int mipMapLevel) {
		if (mipMapLevel < 0 || mipMapLevel >= mipMapLevels) {
			return 0;
		}
		return heights[mipMapLevel];
	}

	unsigned int PixelData::getWidth(unsigned int mipMapLevel) {
		if (mipMapLevel < 0 || mipMapLevel >= mipMapLevels) {
			return 0;
		}
		return widths[mipMapLevel];
	}

	unsigned int PixelData::getHeight_lua(sol::optional<unsigned int> mipMapLevel) {
		return getHeight(mipMapLevel.value_or(1) - 1);
	}

	unsigned int PixelData::getWidth_lua(sol::optional<unsigned int> mipMapLevel) {
		return getWidth(mipMapLevel.value_or(1) - 1);
	}

	void PixelData::setPixelsByte_lua(sol::table data, sol::optional<unsigned int> mipMapLevel) {
		unsigned int level = mipMapLevel.value_or(1) - 1;
		if (level < 0 || level >= mipMapLevels) {
			mwse::log::getLog() << "setPixels: Invalid mip level." << std::endl;
			return;
		}

		// Check that the array is the correct size.
		size_t srcSize = data.size();
		size_t destSize = bytesPerPixel * widths[level] * heights[level];
		if (srcSize != destSize) {
			mwse::log::getLog() << "setPixels: data array argument (" << srcSize << ") is not the same size as the PixelData target (" << destSize << ")." << std::endl;
			return;
		}

		// Convert number array to byte data.
		unsigned char* dest = pixels + offsets[level];
		for (size_t i = 1; i <= srcSize; ++i) {
			*dest++ = unsigned char(data[i]);
		}

		// Indicate the pixel data has changed.
		revisionID++;
	}

	void PixelData::setPixelsFloat_lua(sol::table data, sol::optional<unsigned int> mipMapLevel) {
		unsigned int level = mipMapLevel.value_or(1) - 1;
		if (level < 0 || level >= mipMapLevels) {
			mwse::log::getLog() << "setPixels: Invalid mip level." << std::endl;
			return;
		}

		// Check that the array is the correct size.
		size_t srcSize = data.size();
		size_t destSize = bytesPerPixel * widths[level] * heights[level];
		if (srcSize != destSize) {
			mwse::log::getLog() << "setPixels: data array argument (" << srcSize << ") is not the same size as the PixelData target (" << destSize << ")." << std::endl;
			return;
		}

		// Convert number array to byte data.
		unsigned char* dest = pixels + offsets[level];
		for (size_t i = 1; i <= srcSize; ++i) {
			*dest++ = unsigned char(255.0 * double(data[i]) + 0.5);
		}

		// Indicate the pixel data has changed.
		revisionID++;
	}

	void PixelData::fill_lua(sol::table data, sol::optional<unsigned int> mipMapLevel) {
		unsigned int level = mipMapLevel.value_or(1) - 1;
		if (level < 0 || level >= mipMapLevels) {
			throw std::invalid_argument("Invalid mip level.");
		}

		if (bytesPerPixel == 3) {
			NI::PixelRGB pixel{
				unsigned char(255.0 * double(data.get_or(1, 0.0)) + 0.5),
				unsigned char(255.0 * double(data.get_or(2, 0.0)) + 0.5),
				unsigned char(255.0 * double(data.get_or(3, 0.0)) + 0.5),
			};

			size_t pixelCount = widths[level] * heights[level];
			NI::PixelRGB* dest = reinterpret_cast<NI::PixelRGB*>(pixels + offsets[level]);
			for (size_t i = 1; i <= pixelCount; ++i) {
				*dest++ = pixel;
			}
		}
		else if (bytesPerPixel == 4) {
			NI::PixelRGBA pixel{
				unsigned char(255.0 * double(data.get_or(1, 0.0)) + 0.5),
				unsigned char(255.0 * double(data.get_or(2, 0.0)) + 0.5),
				unsigned char(255.0 * double(data.get_or(3, 0.0)) + 0.5),
				unsigned char(255.0 * double(data.get_or(4, 1.0)) + 0.5),
			};

			size_t pixelCount = widths[level] * heights[level];
			NI::PixelRGBA* dest = reinterpret_cast<NI::PixelRGBA*>(pixels + offsets[level]);
			for (size_t i = 1; i <= pixelCount; ++i) {
				*dest++ = pixel;
			}
		}
		else {
			throw std::runtime_error("Pixel data does not support 3 or 4-byte pixel values.");
		}
	}

}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::PixelData)
