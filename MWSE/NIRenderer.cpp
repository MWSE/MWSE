#include "NIRenderer.h"

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

	bool Renderer::setRenderTarget(RenderedTexture* texture) {
		return vTable.asRenderer->setRenderTarget(this, texture);
	}

	PixelData* Renderer::takeScreenshot(const Rect<unsigned int>* bounds) {
		return vTable.asRenderer->takeScreenshot(this, bounds);
	}

	bool Renderer::getTextureMemoryStats(unsigned int& total, unsigned int& available) {
		return vTable.asRenderer->getTextureMemoryStats(this, total, available);
	}

	bool Renderer::getTextureStats(unsigned int& loadedTextures, unsigned int& usedTextures, unsigned int& stateChanges, unsigned int& newTextures, unsigned int& evictedTextures, unsigned int& bytesTransferred) {
		return vTable.asRenderer->getTextureStats(this, loadedTextures, usedTextures, stateChanges, newTextures, evictedTextures, bytesTransferred);
	}
}
