#pragma once

#include "NIDefines.h"

#include "NIObject.h"
#include "NIProperty.h"
#include "NIRect.h"
#include "NITexture.h"

namespace NI {
	struct Renderer_vTable : Object_vTable {
		char*(__thiscall* getDriverInfo)(Renderer*); // 0x2C
		unsigned int(__thiscall* getCapabilityFlags)(Renderer*); // 0x30
		const Texture::FormatPrefs* (__thiscall* findClosestPixelFormat)(Renderer*, Texture::FormatPrefs*); // 0x34
		void * unknown_0x38;
		bool(__thiscall* setRenderTarget)(Renderer*, RenderedTexture*); // 0x3C
		void * swapBuffers; // 0x40
		void * clearBuffer; // 0x44
		void * getLeftRightSwap; // 0x48
		void * setLeftRightSwap; // 0x4C
		void * setUseWBuffer; // 0x50
		void * getBackBufferWidth; // 0x54
		void * getBackBufferHeight; // 0x58
		PixelData*(__thiscall* takeScreenshot)(Renderer*, const Rect<unsigned int>*); // 0x5C
		void * imageBlt; // 0x60
		void * setDepthClear; // 0x64
		void * setBackgroundColor; // 0x68
		void * setStencilClear; // 0x6C
		void * getBackgroundColor; // 0x70
		void * getMaxFogValue; // 0x74
		void * setMaxFogValue; // 0x78
		void * PrecacheTexture; // 0x7C
		void * purgeAllTextures; // 0x80
		void * purgeTexture; // 0x84
		bool(__thiscall* getTextureMemoryStats)(Renderer*, unsigned int&, unsigned int&); // 0x88
		bool(__thiscall* getTextureStats)(Renderer*, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&, unsigned int&); // 0x8C
		void * getTextureManagerStatus; // 0x90
		void * setMipmapSkipLevel; // 0x94
		void * getMipmapSkipLevel; // 0x98
		void * unknown_0x9C;
		void * unknown_0xA0;
		void * unknown_0xA4;
		void * precacheGeometry; // 0xA8
		void * beginPaint; // 0xAC
		void * endPaint; // 0xB0
		void * purgeEffect; // 0xB4
		void * setCameraData; // 0xB8
		void * renderShape; // 0xBC
		void * renderTristrips; // 0xC0
		void * renderPoints; // 0xC4
		void * renderLines; // 0xC8
		void * renderScreenPoly; // 0xCC
		void * beginBatch; // 0xD0
		void * endBatch; // 0xD4
		void * batchRenderShape; // 0xD8
		void * batchRenderStrips; // 0xDC
		void * unknown_0xE0;
		void * unknown_0xE4;
		void * purgeGeometryData; // 0xE8
		void * purgeMaterial; // 0xEC
		void * purgeSkinPartitionRendererData; // 0xF0
		void * createSourceTextureRendererData; // 0xF4
		void * createRenderedTextureRendererData; // 0xF8
		void * createRenderedCubeMapRendererData; // 0xFC
		void * createSourceCubeMapRendererData; // 0x100
		void * createPaletteRendererData; // 0x104
		void * createBltSourceRendererData; // 0x108
	};
	static_assert(sizeof(Renderer_vTable) == 0x10C, "NI::Renderer failed size validation");

	struct Renderer : Object {
		Pointer<Object> accumulator; // 0x8
		Pointer<Property> currentProperty; // 0xC
		Pointer<Object> currentEffect; // 0x10
		unsigned int precacheCriticalSection; // 0x14
		unsigned int sourceDataCriticalSection; // 0x18

		//
		// vTable accessor functions.
		//

		char* getDriverInfo();
		unsigned int getCapabilityFlags();
		const Texture::FormatPrefs* findClosestPixelFormat(Texture::FormatPrefs* toFormat);
		bool setRenderTarget(RenderedTexture* texture = nullptr);
		PixelData* takeScreenshot(const Rect<unsigned int>* bounds);
		bool getTextureMemoryStats(unsigned int& total, unsigned int& available);
		bool getTextureStats(unsigned int& loadedTextures, unsigned int& usedTextures, unsigned int& stateChanges, unsigned int& newTextures, unsigned int& evictedTextures, unsigned int& bytesTransferred);

	};
	static_assert(sizeof(Renderer) == 0x1C, "NI::Renderer failed size validation");
}
