#include "TextureRenderer.h"

#include "DDSUtil.h"
#include "MemoryUtil.h"
#include "WinUIUtil.h"

namespace se::cs {
	namespace {
		bool isRelativePath(const char* path) {
			if (path[0] == '\\' || path[0] == '/') {
				return false;
			}

			return path[0] == '\0' || path[1] != ':';
		}

		FILE* openTextureFile(const char* path) {
			FILE* file = nullptr;
			fopen_s(&file, path, "rb");
			if (file || !isRelativePath(path)) {
				return file;
			}

			char fullPath[MAX_PATH] = {};
			const auto length = GetCurrentDirectoryA(sizeof(fullPath), fullPath);
			if (length == 0 || length + 1 >= sizeof(fullPath)) {
				return nullptr;
			}

			fullPath[length] = '\\';
			strcpy_s(fullPath + length + 1, sizeof(fullPath) - length - 1, path);
			fopen_s(&file, fullPath, "rb");
			return file;
		}
	}

	TextureRenderer* TextureRenderer::loadFromPath(const char* path) {
		bitmapInfo = nullptr;

		if (tryLoadUncompressedDds(path)) {
			return this;
		}

		const auto loadTextureRenderer = reinterpret_cast<TextureRenderer * (__thiscall*)(TextureRenderer*, const char*)>(0x4754B0);
		return loadTextureRenderer(this, path);
	}

	/// <summary>
	/// The CS by default uses the BLACKONWHITE bitmap stretching mode. For larger textures this will
	/// just result in a black rectangle. We change that to use HALFTONE, to average blocks of pixels
	/// that are stretched down to fit the button render area.
	/// </summary>
	void TextureRenderer::drawItem(DRAWITEMSTRUCT* drawItem) const {
		using winui::GetRectHeight;
		using winui::GetRectWidth;

		if (bitmapInfo == nullptr) {
			return;
		}

		// BLACKONWHITE won't work for us here.
		SetStretchBltMode(drawItem->hDC, HALFTONE);
		SetBrushOrgEx(drawItem->hDC, 0, 0, nullptr);

		const auto rcItem = &drawItem->rcItem;
		const auto sourceWidth = static_cast<int>(width);
		const auto sourceHeight = static_cast<int>(height);
		const auto destinationWidth = GetRectWidth(rcItem);
		const auto destinationHeight = GetRectHeight(rcItem);

		// Render 32-bit textures through AlphaBlend so per-pixel alpha survives.
		if (bitmapInfo->bmiHeader.biBitCount == 32) {
			bool rendered = false;
			const auto sourceDC = CreateCompatibleDC(drawItem->hDC);
			if (sourceDC) {
				void* sourceBits = nullptr;
				const auto sourceBitmap = CreateDIBSection(
					drawItem->hDC,
					bitmapInfo,
					DIB_RGB_COLORS,
					&sourceBits,
					nullptr,
					0
				);

				if (sourceBitmap && sourceBits) {
					copyBitmapDataForAlphaBlend(sourceBits);

					const auto previousBitmap = SelectObject(sourceDC, sourceBitmap);

					FillRect(drawItem->hDC, rcItem, GetSysColorBrush(COLOR_BTNFACE));

					BLENDFUNCTION blend = {};
					blend.BlendOp = AC_SRC_OVER;
					blend.BlendFlags = 0;
					blend.SourceConstantAlpha = 255;
					blend.AlphaFormat = AC_SRC_ALPHA;

					rendered = AlphaBlend(
						drawItem->hDC,
						rcItem->left, rcItem->top, destinationWidth, destinationHeight,
						sourceDC,
						0, 0, sourceWidth, sourceHeight,
						blend
					);

					SelectObject(sourceDC, previousBitmap);
					DeleteObject(sourceBitmap);
				}

				DeleteDC(sourceDC);
			}

			if (rendered) {
				return;
			}
		}

		// Fallback for opaque textures.
		StretchDIBits(drawItem->hDC,
			rcItem->left, rcItem->top, destinationWidth, destinationHeight,
			0, 0, sourceWidth, sourceHeight,
			bitmapInfo->bmiColors, bitmapInfo,
			DIB_RGB_COLORS, SRCCOPY);
	}

	void TextureRenderer::copyBitmapDataForAlphaBlend(void* destinationBits) const {
		const auto pixelCount = static_cast<size_t>(width) * static_cast<size_t>(height);
		const auto source = reinterpret_cast<const unsigned char*>(bitmapInfo->bmiColors);
		const auto destination = reinterpret_cast<unsigned char*>(destinationBits);

		for (size_t i = 0; i < pixelCount; ++i) {
			const auto offset = i * 4;
			const auto alpha = static_cast<unsigned int>(source[offset + 3]);

			destination[offset] = static_cast<unsigned char>((static_cast<unsigned int>(source[offset]) * alpha + 127) / 255);
			destination[offset + 1] = static_cast<unsigned char>((static_cast<unsigned int>(source[offset + 1]) * alpha + 127) / 255);
			destination[offset + 2] = static_cast<unsigned char>((static_cast<unsigned int>(source[offset + 2]) * alpha + 127) / 255);
			destination[offset + 3] = source[offset + 3];
		}
	}

	bool TextureRenderer::tryLoadUncompressedDds(const char* path) {
		if (path == nullptr) {
			return false;
		}

		const auto extension = strrchr(path, '.');
		if (extension == nullptr || _stricmp(extension, ".dds") != 0) {
			return false;
		}

		auto file = openTextureFile(path);
		if (file == nullptr) {
			return false;
		}

		unsigned int magic = 0;
		dds::Header header = {};
		const auto headerRead = fread(&magic, sizeof(magic), 1, file) == 1 && fread(&header, sizeof(header), 1, file) == 1;
		if (!headerRead || magic != dds::MAGIC || !dds::isUncompressedRgb32(header)) {
			fclose(file);
			return false;
		}

		const auto pixelCount = static_cast<size_t>(header.width) * static_cast<size_t>(header.height);
		if (pixelCount == 0) {
			fclose(file);
			return false;
		}

		const auto allocationSize = sizeof(BITMAPINFOHEADER) + pixelCount * 4;
		const auto newBitmapInfo = reinterpret_cast<BITMAPINFO*>(memory::malloc(allocationSize));
		if (newBitmapInfo == nullptr) {
			fclose(file);
			return false;
		}

		memset(newBitmapInfo, 0, allocationSize);
		newBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		newBitmapInfo->bmiHeader.biWidth = static_cast<LONG>(header.width);
		newBitmapInfo->bmiHeader.biHeight = -static_cast<LONG>(header.height);
		newBitmapInfo->bmiHeader.biPlanes = 1;
		newBitmapInfo->bmiHeader.biBitCount = 32;
		newBitmapInfo->bmiHeader.biCompression = BI_RGB;

		auto pixels = reinterpret_cast<unsigned char*>(newBitmapInfo->bmiColors);
		for (size_t i = 0; i < pixelCount; ++i) {
			unsigned int sourcePixel = 0;
			if (fread(&sourcePixel, sizeof(sourcePixel), 1, file) != 1) {
				memory::free(newBitmapInfo);
				fclose(file);
				return false;
			}

			const auto offset = i * 4;
			pixels[offset] = dds::getMaskedByte(sourcePixel, header.pixelFormat.blueMask);
			pixels[offset + 1] = dds::getMaskedByte(sourcePixel, header.pixelFormat.greenMask);
			pixels[offset + 2] = dds::getMaskedByte(sourcePixel, header.pixelFormat.redMask);
			pixels[offset + 3] = dds::hasAlpha(header.pixelFormat)
				? dds::getMaskedByte(sourcePixel, header.pixelFormat.alphaMask)
				: 255;
		}

		fclose(file);

		width = header.width;
		height = header.height;
		bitmapInfo = newBitmapInfo;
		return true;
	}

	void TextureRenderer::installPatches() {
		using memory::genJumpEnforced;

		// Patch: Avoid the CS' broken NetImmerse-pixel-data to GDI-DIB conversion for 32-bit DDS icons.
		auto loadFromPath = &TextureRenderer::loadFromPath;
		genJumpEnforced(0x40102D, 0x4754B0, *reinterpret_cast<DWORD*>(&loadFromPath));

		// Patch: Fix rendering of textures larger than the target rectangle.
		auto drawItem = &TextureRenderer::drawItem;
		genJumpEnforced(0x402D38, 0x475A80, *reinterpret_cast<DWORD*>(&drawItem));
	}
}
