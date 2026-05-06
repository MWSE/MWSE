#include "NIPixelFormat.h"

#include "ExceptionUtil.h"

#include <stdexcept>

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
namespace NI {
	PixelFormat::PixelFormat(Format format) {
#if defined(SE_NI_PIXELFORMAT_FNADDR_CTOR_FROMFORMAT) && SE_NI_PIXELFORMAT_FNADDR_CTOR_FROMFORMAT > 0
		const auto NI_PixelFormat_ctor_fromFormat = reinterpret_cast<PixelFormat * (__thiscall*)(PixelFormat*, Format)>(SE_NI_PIXELFORMAT_FNADDR_CTOR_FROMFORMAT);
		NI_PixelFormat_ctor_fromFormat(this, format);
#else
		throw not_implemented_exception();
#endif
	}

	PixelFormat::PixelFormat(unsigned int maskR, unsigned int maskG, unsigned int maskB, unsigned int maskA, int bitsPerPixel) {
#if defined(SE_NI_PIXELFORMAT_FNADDR_CTOR_FROMRGBA) && SE_NI_PIXELFORMAT_FNADDR_CTOR_FROMRGBA > 0
		const auto NI_PixelFormat_ctor_fromRGBA = reinterpret_cast<PixelFormat * (__thiscall*)(PixelFormat*, unsigned int, unsigned int, unsigned int, unsigned int, int)>(SE_NI_PIXELFORMAT_FNADDR_CTOR_FROMRGBA);
		NI_PixelFormat_ctor_fromRGBA(this, maskR, maskG, maskB, maskA, bitsPerPixel);
#else
		throw not_implemented_exception();
#endif
	}

	PixelFormat::PixelFormat(bool hasAlpha) {
#if defined(SE_NI_PIXELFORMAT_FNADDR_CTOR_FROMPALETTE) && SE_NI_PIXELFORMAT_FNADDR_CTOR_FROMPALETTE > 0
		const auto NI_PixelFormat_ctor_fromPalette = reinterpret_cast<PixelFormat * (__thiscall*)(PixelFormat*, bool)>(SE_NI_PIXELFORMAT_FNADDR_CTOR_FROMPALETTE);
		NI_PixelFormat_ctor_fromPalette(this, hasAlpha);
#else
		throw not_implemented_exception();
#endif
	}

	PixelFormat::PixelFormat(D3DFORMAT d3dFormat) {
#if defined(SE_NI_PIXELFORMAT_FNADDR_CTOR_FROMRGBA) && SE_NI_PIXELFORMAT_FNADDR_CTOR_FROMRGBA > 0 && defined(SE_NI_PIXELFORMAT_FNADDR_CTOR_FROMPALETTE) && SE_NI_PIXELFORMAT_FNADDR_CTOR_FROMPALETTE > 0 && defined(SE_NI_PIXELFORMAT_FNADDR_CTOR_FROMFORMAT) && SE_NI_PIXELFORMAT_FNADDR_CTOR_FROMFORMAT > 0 && defined(SE_NI_PIXELFORMAT_FNADDR_CTOR_FROMBUMPLUMA) && SE_NI_PIXELFORMAT_FNADDR_CTOR_FROMBUMPLUMA > 0
		const auto NI_PixelFormat_ctor_fromRGBA = reinterpret_cast<PixelFormat * (__thiscall*)(PixelFormat*, unsigned int, unsigned int, unsigned int, unsigned int, int)>(SE_NI_PIXELFORMAT_FNADDR_CTOR_FROMRGBA);
		const auto NI_PixelFormat_ctor_fromPalette = reinterpret_cast<PixelFormat * (__thiscall*)(PixelFormat*, bool)>(SE_NI_PIXELFORMAT_FNADDR_CTOR_FROMPALETTE);
		const auto NI_PixelFormat_ctor_fromFormat = reinterpret_cast<PixelFormat * (__thiscall*)(PixelFormat*, Format)>(SE_NI_PIXELFORMAT_FNADDR_CTOR_FROMFORMAT);
		const auto NI_PixelFormat_ctor_fromBumpLuma = reinterpret_cast<PixelFormat * (__thiscall*)(PixelFormat*, unsigned int, unsigned int, unsigned int, int)>(SE_NI_PIXELFORMAT_FNADDR_CTOR_FROMBUMPLUMA);

		switch (d3dFormat) {
		case D3DFMT_R8G8B8:
		case D3DFMT_X8R8G8B8:
			NI_PixelFormat_ctor_fromRGBA(this, 0xFF0000u, 0xFF00u, 0xFFu, 0, 32);
			break;
		case D3DFMT_R5G6B5:
			NI_PixelFormat_ctor_fromRGBA(this, 0xF800u, 0x7E0u, 0x1Fu, 0, 16);
			break;
		case D3DFMT_X1R5G5B5:
			NI_PixelFormat_ctor_fromRGBA(this, 0x7C00u, 0x3E0u, 0x1Fu, 0, 16);
			break;
		case D3DFMT_A1R5G5B5:
			NI_PixelFormat_ctor_fromRGBA(this, 0x7C00u, 0x3E0u, 0x1Fu, 0x8000u, 16);
			break;
		case D3DFMT_A4R4G4B4:
			NI_PixelFormat_ctor_fromRGBA(this, 0xF00u, 0xF0u, 0xFu, 0xF000u, 16);
			break;
		case D3DFMT_P8:
			NI_PixelFormat_ctor_fromPalette(this, false);
			break;
		case D3DFMT_A8R8G8B8:
		case D3DFMT_R3G3B2:
		case D3DFMT_A8:
		case D3DFMT_A8R3G3B2:
		case D3DFMT_X4R4G4B4:
		case D3DFMT_A8P8:
		case D3DFMT_L8:
		case D3DFMT_A8L8:
		case D3DFMT_A4L4:
		case D3DFMT_Q8W8V8U8:
		case D3DFMT_V16U16:
		case D3DFMT_W11V11U10:
		case D3DFMT_UYVY:
		case D3DFMT_YUY2:
			NI_PixelFormat_ctor_fromRGBA(this, 0xFF0000u, 0xFF00u, 0xFFu, 0xFF000000, 32);
			break;
		case D3DFMT_V8U8:
			NI_PixelFormat_ctor_fromBumpLuma(this, 0xFFu, 0xFF00u, 0, 16);
			break;
		case D3DFMT_L6V5U5:
			NI_PixelFormat_ctor_fromBumpLuma(this, 0x1Fu, 0x3E0u, 0xFC00u, 16);
			break;
		case D3DFMT_X8L8V8U8:
			NI_PixelFormat_ctor_fromBumpLuma(this, 0xFFu, 0xFF00u, 0xFF0000u, 32);
			break;
		case D3DFMT_DXT1:
			NI_PixelFormat_ctor_fromFormat(this, Format::COMPRESS1);
			break;
		case D3DFMT_DXT3:
			NI_PixelFormat_ctor_fromFormat(this, Format::COMPRESS3);
			break;
		case D3DFMT_DXT5:
			NI_PixelFormat_ctor_fromFormat(this, Format::COMPRESS5);
			break;
		case D3DFMT_DXT2:
		case D3DFMT_DXT4:
		case D3DFMT_D16_LOCKABLE:
		case D3DFMT_D32:
		case D3DFMT_D15S1:
		case D3DFMT_D24S8:
		case D3DFMT_D16:
		case D3DFMT_D24X8:
		case D3DFMT_D24X4S4:
		case D3DFMT_VERTEXDATA:
		case D3DFMT_INDEX16:
		case D3DFMT_INDEX32:
		default:
			throw std::invalid_argument("Unhandled D3D format.");
		}
#else
		throw not_implemented_exception();
#endif
	}

	D3DFORMAT PixelFormat::getD3DFormat() const {
#if defined(SE_NI_PIXELFORMAT_FNADDR_GETD3DFORMAT) && SE_NI_PIXELFORMAT_FNADDR_GETD3DFORMAT > 0
		const auto NI_PixelFormat_getD3DFormat = reinterpret_cast<D3DFORMAT(__cdecl*)(const PixelFormat*)>(SE_NI_PIXELFORMAT_FNADDR_GETD3DFORMAT);
		return NI_PixelFormat_getD3DFormat(this);
#else
		throw not_implemented_exception();
#endif
	}
}
#endif
