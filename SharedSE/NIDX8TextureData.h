#pragma once

#include "NIPixelFormat.h"
#include "NITexture.h"

namespace NI {
	struct DX8PixelFormat : PixelFormat {
		unsigned int extraData;
		D3DFORMAT d3dFormat;
	};
	static_assert(sizeof(DX8PixelFormat) == 0x28, "NI::DX8PixelFormat failed size validation");

	struct DX8TextureData : Texture::RendererData {
		Renderer* renderer;
		DX8PixelFormat format;
		int d3dPalette;
		int d3dPaletteRevision;
		unsigned int width;
		unsigned int height;
		unsigned int levels;
	};
	static_assert(sizeof(DX8TextureData) == 0x50, "NI::DX8TextureData failed size validation");

	struct DX8SourceTextureData : DX8TextureData {
		bool mipmap;
		char pad_0x51[3];
		int formattedSize;
		void* palette;
		int paletteRevision;
		IDirect3DTexture8* d3dTexture;
		int sourceRevision;
	};
	static_assert(sizeof(DX8SourceTextureData) == 0x68, "NI::DX8SourceTextureData failed size validation");

	struct DX8RenderedTextureData : DX8TextureData {
		DX8PixelFormat sourceFormat;
		IDirect3DTexture8* d3dTexture;
		void* renderTarget;
	};
	static_assert(sizeof(DX8SourceTextureData) == 0x68, "NI::DX8SourceTextureData failed size validation");
}