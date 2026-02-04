#include "NIRenderedTexture.h"

#include "NIDX8Renderer.h"
#include "NIDX8TextureData.h"
#include "NIPixelData.h"
#include "TES3WorldController.h"

namespace NI {
	const auto NI_RenderedTexture_create = reinterpret_cast<RenderedTexture* (__cdecl*)(unsigned int, unsigned int, Renderer*, const Texture::FormatPrefs*)>(0x6DC090);
	Pointer<RenderedTexture> RenderedTexture::create(unsigned int width, unsigned int height, sol::optional<const FormatPrefs*> prefs) {
		auto renderer = TES3::WorldController::get()->renderer;
		const auto prefsVal = prefs.value_or(&Texture::FormatPrefs::DEFAULT_LUA_PREFS);
		auto texture = NI_RenderedTexture_create(width, height, renderer, prefsVal);
		return texture;
	}

	//
	// Copies the render target contents from the GPU into CPU accessible pixelData.
	//

	bool RenderedTexture::readback(NI::PixelData* pixelData) {
		auto renderer = static_cast<NI::DX8Renderer*>(TES3::WorldController::get()->renderer);

		if (pixelData == nullptr) {
			return false;
		}

		// Check source and dest sizes match.
		if (pixelData->getWidth() != width && pixelData->getHeight() != height) {
			return false;
		}

		IDirect3DTexture8* rtTexture = static_cast<DX8RenderedTextureData*>(rendererData)->d3dTexture;
		IDirect3DSurface8* rtSurface = nullptr;
		IDirect3DSurface8* readbackSurface = nullptr;

		// Create system memory surface to copy to.
		rtTexture->GetSurfaceLevel(0, &rtSurface);
		const auto desiredFormat = pixelData->pixelFormat.getD3DFormat();
		if (renderer->d3dDevice->CreateImageSurface(width, height, desiredFormat, &readbackSurface) != D3D_OK) {
			rtSurface->Release();
			return false;
		}

		if (renderer->d3dDevice->CopyRects(rtSurface, nullptr, 0, readbackSurface, nullptr) != D3D_OK) {
			readbackSurface->Release();
			rtSurface->Release();
			return false;
		}

		D3DLOCKED_RECT lock;
		if (readbackSurface->LockRect(&lock, nullptr, D3DLOCK_NOSYSLOCK | D3DLOCK_READONLY) != D3D_OK) {
			readbackSurface->Release();
			rtSurface->Release();
			return false;
		}

		// Copy from system memory surface to pixelData.
		memcpy(pixelData->pixels, lock.pBits, pixelData->offsets[1] - pixelData->offsets[0]);
		readbackSurface->Release();
		readbackSurface = nullptr;
		rtSurface->Release();
		rtSurface = nullptr;

		// For some reason the game needs to swap B and R pixels.
		// See paper doll code, or around 0x42F939.
		if (desiredFormat == D3DFMT_A8R8G8B8) {
			const auto pixels = reinterpret_cast<PixelRGBA*>(pixelData->pixels);
			const auto pixelCount = pixelData->getWidth() * pixelData->getHeight();
			for (auto i = 0u; i < pixelCount; ++i) {
				auto& pixel = pixels[i];
				std::swap(pixel.r, pixel.b);
			}
		}

		return true;
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::RenderedTexture)
