#include "NIRenderer.h"

#include "NIPixelData.h"

namespace NI {
	char* Renderer::getDriverInfo() {
		return vTable.asRenderer->getDriverInfo(this);
	}

	unsigned int Renderer::getCapabilityFlags() {
		return vTable.asRenderer->getCapabilityFlags(this);
	}

	const Texture::FormatPrefs* Renderer::findClosestPixelFormat(Texture::FormatPrefs* toFormat) {
		return vTable.asRenderer->findClosestPixelFormat(this, toFormat);
	}

	int Renderer::getBackBufferWidth() const {
		return vTable.asRenderer->getBackBufferWidth(this);
	}

	int Renderer::getBackBufferHeight() const {
		return vTable.asRenderer->getBackBufferHeight(this);
	}

	bool Renderer::setRenderTarget(RenderedTexture* texture) {
		return vTable.asRenderer->setRenderTarget(this, texture);
	}

	PixelData* Renderer::takeScreenshot(const Rect<unsigned int>* bounds) {
		auto pixelData = vTable.asRenderer->takeScreenshot(this, bounds);
		if (pixelData == nullptr) {
			return nullptr;
		}
		const auto desiredFormat = pixelData->pixelFormat.getD3DFormat();

		// For some reason the game needs to swap B and R pixels.
		// See paper doll code, or around 0x42F939.
		if (desiredFormat == D3DFMT_X8R8G8B8) {
			const auto pixels = reinterpret_cast<NI::PixelRGBA*>(pixelData->pixels);
			const auto pixelCount = pixelData->getWidth() * pixelData->getHeight();
			for (auto i = 0u; i < pixelCount; ++i) {
				auto& pixel = pixels[i];
				std::swap(pixel.r, pixel.b);
			}
		}

		return pixelData;
	}

	bool Renderer::getTextureMemoryStats(unsigned int& total, unsigned int& available) {
		return vTable.asRenderer->getTextureMemoryStats(this, total, available);
	}

	bool Renderer::getTextureStats(unsigned int& loadedTextures, unsigned int& usedTextures, unsigned int& stateChanges, unsigned int& newTextures, unsigned int& evictedTextures, unsigned int& bytesTransferred) {
		return vTable.asRenderer->getTextureStats(this, loadedTextures, usedTextures, stateChanges, newTextures, evictedTextures, bytesTransferred);
	}
}
