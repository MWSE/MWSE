#include "TextureRenderer.h"

#include "MemoryUtil.h"
#include "WinUIUtil.h"

namespace se::cs {
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
					const auto bitmapBytes = bitmapInfo->bmiHeader.biSizeImage != 0
						? static_cast<size_t>(bitmapInfo->bmiHeader.biSizeImage)
						: static_cast<size_t>(sourceWidth) * static_cast<size_t>(sourceHeight) * 4;
					memcpy(sourceBits, bitmapInfo->bmiColors, bitmapBytes);

					const auto previousBitmap = SelectObject(sourceDC, sourceBitmap);

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

	void TextureRenderer::installPatches() {
		using memory::genJumpEnforced;

		// Patch: Fix rendering of textures larger than the target rectangle.
		auto drawItem = &TextureRenderer::drawItem;
		genJumpEnforced(0x402D38, 0x475A80, *reinterpret_cast<DWORD*>(&drawItem));
	}
}
