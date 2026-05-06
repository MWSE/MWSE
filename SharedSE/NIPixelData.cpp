#include "NIPixelData.h"

#include "NIPixelFormat.h"
#include "NISourceTexture.h"
#include "NITexture.h"

#include "ExceptionUtil.h"
#include "MemoryUtil.h"

namespace NI {

	// Create an RGBA-8bpc PixelData via engine ctor.
	Pointer<PixelData> PixelData::create(unsigned int width, unsigned int height, unsigned int mipMapLevels) {
#if defined(SE_NI_PIXELDATA_FNADDR_CTOR_ARGS) && SE_NI_PIXELDATA_FNADDR_CTOR_ARGS > 0 && defined(SE_NI_PIXELFORMAT_GLOBADDR_RGBA32) && SE_NI_PIXELFORMAT_GLOBADDR_RGBA32 > 0 && defined(SE_MEMORY_FNADDR_NEW) && SE_MEMORY_FNADDR_NEW > 0
		const auto NI_PixelData_ctor_args = reinterpret_cast<void(__thiscall*)(PixelData*, unsigned int, unsigned int, const PixelFormat*, unsigned int)>(SE_NI_PIXELDATA_FNADDR_CTOR_ARGS);
		const auto NI_PixelFormat_RGBA32 = reinterpret_cast<const PixelFormat*>(SE_NI_PIXELFORMAT_GLOBADDR_RGBA32);

		PixelData* pixelData = se::memory::_new<PixelData>();
		NI_PixelData_ctor_args(pixelData, width, height, NI_PixelFormat_RGBA32, mipMapLevels);
		return pixelData;
#else
		throw not_implemented_exception();
#endif
	}

	Pointer<SourceTexture> PixelData::createSourceTexture() {
		using FormatPrefs = Texture::FormatPrefs;
		FormatPrefs prefs;
		return SourceTexture::createFromPixelData(this, &prefs);
	}

	unsigned int PixelData::getHeight(unsigned int mipMapLevel) const {
		if (mipMapLevel >= mipMapLevels) {
			return 0;
		}
		return heights[mipMapLevel];
	}

	unsigned int PixelData::getWidth(unsigned int mipMapLevel) const {
		if (mipMapLevel >= mipMapLevels) {
			return 0;
		}
		return widths[mipMapLevel];
	}

	void PixelData::exportTGA(const char* fileName) const {
		switch (pixelFormat.format) {
		case PixelFormat::Format::RGB:
		case PixelFormat::Format::RGBA:
			break;
		default:
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
			throw std::invalid_argument("Couldn't open file.");
		}

		auto width = getWidth();
		auto height = getHeight();
		HeaderTGA header{
			0,
			0,
			HeaderTGA::DataType::RGB,
			0, 0, 0, 0, 0,
			static_cast<short>(width),
			static_cast<short>(height),
			static_cast<unsigned char>(bytesPerPixel * 8),
			0x08 | 0x20
		};

		DWORD bytesWritten = 0;
		if (!WriteFile(hFile, &header, sizeof(header), &bytesWritten, NULL) || bytesWritten != sizeof(header)) {
			CloseHandle(hFile);
			std::filesystem::remove(fileName);
			throw std::runtime_error("Couldn't write the file header.");
		}

		size_t bytesToWrite = static_cast<size_t>(width) * height * bytesPerPixel;
		std::vector<unsigned char> imageData;
		imageData.reserve(bytesToWrite);
		size_t offset = 0;

		if (pixelFormat.format == PixelFormat::Format::RGBA) {
			for (size_t y = 0; y < height; ++y) {
				for (size_t x = 0; x < width; ++x) {
					imageData.emplace_back(pixels[offset + 2]);
					imageData.emplace_back(pixels[offset + 1]);
					imageData.emplace_back(pixels[offset + 0]);
					imageData.emplace_back(pixels[offset + 3]);
					offset += bytesPerPixel;
				}
			}
		}
		else if (pixelFormat.format == PixelFormat::Format::RGB) {
			for (size_t y = 0; y < height; ++y) {
				for (size_t x = 0; x < width; ++x) {
					imageData.emplace_back(pixels[offset + 2]);
					imageData.emplace_back(pixels[offset + 1]);
					imageData.emplace_back(pixels[offset + 0]);
					imageData.emplace_back(255);
					offset += bytesPerPixel;
				}
			}
		}

		if (!WriteFile(hFile, imageData.data(), static_cast<DWORD>(bytesToWrite), &bytesWritten, NULL) || bytesWritten != bytesToWrite) {
			CloseHandle(hFile);
			std::filesystem::remove(fileName);
			throw std::runtime_error("Couldn't write pixel data.");
		}
		SetEndOfFile(hFile);
		CloseHandle(hFile);
	}

}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1

#include "Log.h"

namespace NI {

	Pointer<PixelData> PixelData::create_lua(unsigned int width, unsigned int height, sol::optional<unsigned int> mipMapLevels) {
		return create(width, height, mipMapLevels.value_or(1));
	}

	unsigned int PixelData::getHeight_lua(sol::optional<unsigned int> mipMapLevel) const {
		return getHeight(mipMapLevel.value_or(1) - 1);
	}

	unsigned int PixelData::getWidth_lua(sol::optional<unsigned int> mipMapLevel) const {
		return getWidth(mipMapLevel.value_or(1) - 1);
	}

	void PixelData::setPixelsByte_lua(sol::table data, sol::optional<unsigned int> mipMapLevel) {
		unsigned int level = mipMapLevel.value_or(1) - 1;
		if (level >= mipMapLevels) {
			mwse::log::getLog() << "setPixels: Invalid mip level." << std::endl;
			return;
		}

		size_t srcSize = data.size();
		size_t destSize = bytesPerPixel * widths[level] * heights[level];
		if (srcSize != destSize) {
			mwse::log::getLog() << "setPixels: data array argument (" << srcSize << ") is not the same size as the PixelData target (" << destSize << ")." << std::endl;
			return;
		}

		unsigned char* dest = pixels + offsets[level];
		for (size_t i = 1; i <= srcSize; ++i) {
			*dest++ = static_cast<unsigned char>(data[i]);
		}
		revisionID++;
	}

	void PixelData::setPixelsFloat_lua(sol::table data, sol::optional<unsigned int> mipMapLevel) {
		unsigned int level = mipMapLevel.value_or(1) - 1;
		if (level >= mipMapLevels) {
			mwse::log::getLog() << "setPixels: Invalid mip level." << std::endl;
			return;
		}

		size_t srcSize = data.size();
		size_t destSize = bytesPerPixel * widths[level] * heights[level];
		if (srcSize != destSize) {
			mwse::log::getLog() << "setPixels: data array argument (" << srcSize << ") is not the same size as the PixelData target (" << destSize << ")." << std::endl;
			return;
		}

		unsigned char* dest = pixels + offsets[level];
		for (size_t i = 1; i <= srcSize; ++i) {
			*dest++ = static_cast<unsigned char>(255.0 * double(data[i]) + 0.5);
		}
		revisionID++;
	}

	void PixelData::fill_lua(sol::table data, sol::optional<unsigned int> mipMapLevel) {
		unsigned int level = mipMapLevel.value_or(1) - 1;
		if (level >= mipMapLevels) {
			throw std::invalid_argument("Invalid mip level.");
		}

		if (bytesPerPixel == 3) {
			NI::PixelRGB pixel{
				static_cast<unsigned char>(255.0 * double(data.get_or(1, 0.0)) + 0.5),
				static_cast<unsigned char>(255.0 * double(data.get_or(2, 0.0)) + 0.5),
				static_cast<unsigned char>(255.0 * double(data.get_or(3, 0.0)) + 0.5),
			};
			size_t pixelCount = widths[level] * heights[level];
			NI::PixelRGB* dest = reinterpret_cast<NI::PixelRGB*>(pixels + offsets[level]);
			for (size_t i = 0; i < pixelCount; ++i) {
				*dest++ = pixel;
			}
		}
		else if (bytesPerPixel == 4) {
			NI::PixelRGBA pixel{
				static_cast<unsigned char>(255.0 * double(data.get_or(1, 0.0)) + 0.5),
				static_cast<unsigned char>(255.0 * double(data.get_or(2, 0.0)) + 0.5),
				static_cast<unsigned char>(255.0 * double(data.get_or(3, 0.0)) + 0.5),
				static_cast<unsigned char>(255.0 * double(data.get_or(4, 1.0)) + 0.5),
			};
			size_t pixelCount = widths[level] * heights[level];
			NI::PixelRGBA* dest = reinterpret_cast<NI::PixelRGBA*>(pixels + offsets[level]);
			for (size_t i = 0; i < pixelCount; ++i) {
				*dest++ = pixel;
			}
		}
		else {
			throw std::runtime_error("Pixel data does not support 3 or 4-byte pixel values.");
		}
	}

}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::PixelData)

#endif
