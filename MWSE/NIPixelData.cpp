#include "NIPixelData.h"

#include "NIPixelFormat.h"
#include "NISourceTexture.h"
#include "NITexture.h"

#include "Log.h"
#include "MemoryUtil.h"

namespace NI {

	const auto NIPixelData_ctor_args = reinterpret_cast<void(__thiscall*)(PixelData*, unsigned int, unsigned int, const PixelFormat*, unsigned int)>(0x6D4FC0);
	const auto NIPixelFormat_RGBA32 = reinterpret_cast<const PixelFormat*>(0x7DE8B8);

	// Create an RGBA-8bpc PixelData.
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
		FormatPrefs prefs;
		return SourceTexture::createFromPixelData(this, &prefs);
	}

	unsigned int PixelData::getHeight(unsigned int mipMapLevel) const {
		if (mipMapLevel < 0 || mipMapLevel >= mipMapLevels) {
			return 0;
		}
		return heights[mipMapLevel];
	}

	unsigned int PixelData::getWidth(unsigned int mipMapLevel) const {
		if (mipMapLevel < 0 || mipMapLevel >= mipMapLevels) {
			return 0;
		}
		return widths[mipMapLevel];
	}

	void PixelData::exportTGA(const char* fileName) const {
		if (pixelFormat.format != PixelFormat::Format::RGBA) {
			throw std::runtime_error("Unsupported pixel format for export to TGA.");
		}
		HANDLE hFile = CreateFileA(
			fileName,
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);
		if (hFile == INVALID_HANDLE_VALUE) {
			throw std::invalid_argument(fmt::format("Couldn't open file: {}", fileName));
		}
		auto width = getWidth();
		auto height = getHeight();
		HeaderTGA header{
			0, // idLength - No image description
			0, // colormapType - No color pallete
			HeaderTGA::DataType::UNCOMPRESSED_RGB,
			0, // colormapOrigin
			0, // colormapLength
			0, // colormapDepth
			0, // xOrigin
			0, // yOrigin
			static_cast<short>(width),
			static_cast<short>(height),
			static_cast<unsigned char>(bytesPerPixel * 8), // bitsPerPixel
			0x08 | 0x20 // Flag bit 3: 8 bits for alpha + flag bit 5: origin in the upper left-hand corner
		};

		// Write the TGA header
		DWORD bytesWritten = 0;
		if (!WriteFile(hFile, &header, sizeof(header), &bytesWritten, NULL) || bytesWritten != sizeof(header)) {
			CloseHandle(hFile);
			std::filesystem::remove(fileName);
			throw std::runtime_error("Couldn't write the file header.");
		}

		// Write the pixel data
		size_t bytesToWrite = width * height * bytesPerPixel;
		std::vector<unsigned char> imageData;
		imageData.reserve(bytesToWrite);

		size_t offset = 0;
		// TGA stores channels in BGR(A) format, so we need to swap blue and red channels
		for (size_t y = 0; y < height; ++y) {
			for (size_t x = 0; x < width; ++x) {
				imageData.emplace_back(pixels[offset + 2]); // Blue
				imageData.emplace_back(pixels[offset + 1]); // Green
				imageData.emplace_back(pixels[offset + 0]); // Red
				imageData.emplace_back(pixels[offset + 3]); // Alpha
				offset += bytesPerPixel;
			}
		}

		if (!WriteFile(hFile, imageData.data(), bytesToWrite, &bytesWritten, NULL) || bytesWritten != bytesToWrite) {
			CloseHandle(hFile);
			std::filesystem::remove(fileName);
			throw std::runtime_error("Couldn't write pixel data.");
		}
		SetEndOfFile(hFile);
		CloseHandle(hFile);
	}

	unsigned int PixelData::getHeight_lua(sol::optional<unsigned int> mipMapLevel) const {
		return getHeight(mipMapLevel.value_or(1) - 1);
	}

	unsigned int PixelData::getWidth_lua(sol::optional<unsigned int> mipMapLevel) const {
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
