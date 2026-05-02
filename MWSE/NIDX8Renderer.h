#pragma once

#include "NIDefines.h"

#include "NIRenderer.h"
#include "TES3HashMap.h"

namespace NI {
	struct DX8DeviceDesc {
		int deviceType; // 0x0
		D3DCAPS8 caps; // 0x4
		int screenFormats[6]; // 0xD8 // NI::TList
		bool renderWindowed; // 0xF0
	};
	static_assert(sizeof(DX8DeviceDesc) == 0xF4, "NI::DX8DeviceDesc failed size validation");

	struct DX8AdapterDesc {
		struct DX8AdapterModeDesc {
			NI::PixelFormat pixelFormat; // 0x0
			unsigned int unknown_0x20;
			int d3dFormat; // 0x24
		};
		int index; // 0x0
		D3DADAPTER_IDENTIFIER8 identifier; // 0x4
		int unknown_0x430;
		TArray<DX8AdapterModeDesc*> modeList; // 0x434
		DX8DeviceDesc* HALDeviceDesc; // 0x44C
		DX8DeviceDesc* REFDeviceDesc; // 0x450
	};
	static_assert(sizeof(DX8AdapterDesc) == 0x454, "NI::DX8AdapterDesc failed size validation");
	static_assert(sizeof(DX8AdapterDesc::DX8AdapterModeDesc) == 0x28, "NI::DX8AdapterDesc::DX8AdapterModeDesc failed size validation");

	struct DX8SystemDesc {
		size_t adapterCount; // 0x0
		NI::TArray<DX8AdapterDesc*> adapters; // 0x4
	};
	static_assert(sizeof(DX8SystemDesc) == 0x1C, "NI::DX8SystemDesc failed size validation");

	struct DX8Renderer : Renderer {
		Pointer<void> propertyStatePtr; // 0x1C
		Pointer<void> effectStatePtr; // 0x20
		IDirect3DDevice8* d3dDevice; // 0x24
		HWND deviceWindowHandle; // 0x28
		char driverDesc[512]; // 0x2C
		DWORD currentAdapterIndex; // 0x22C
		DWORD d3dDeviceType; // 0x230
		HWND focusWindow; // 0x234
		DWORD d3dBehaviorFlags; // 0x238
		D3DPRESENT_PARAMETERS d3dPresentParameters; // 0x23C
		DX8SystemDesc* systemDesc; // 0x270
		int unknown_0x274;
		int unknown_0x278;
		PackedColor backgroundColor; // 0x27C
		float depthClearValue; // 0x280
		DWORD stencilClearValue; // 0x284
		DWORD capabilityFlags; // 0x288
		int unknown_0x28C;
		int unknown_0x290;
		int unknown_0x294;
		int unknown_0x298;
		int unknown_0x29C;
		int unknown_0x2A0;
		int unknown_0x2A4;
		int unknown_0x2A8;
		TES3::Vector3 cameraRight; // 0x2AC
		TES3::Vector3 cameraUp; // 0x2B8
		TES3::Vector3 modelCameraRight; // 0x2C4
		TES3::Vector3 modelCameraUp; // 0x2D0
		int unknown_0x2DC;
		int unknown_0x2E0;
		int unknown_0x2E4;
		int unknown_0x2E8;
		int unknown_0x2EC;
		int unknown_0x2F0;
		int unknown_0x2F4;
		int unknown_0x2F8;
		float frustumNear;
		float frustumDistance;
		int unknown_0x304;
		int unknown_0x308;
		int unknown_0x30C;
		int unknown_0x310;
		int unknown_0x314;
		int unknown_0x318;
		int unknown_0x31C;
		int unknown_0x320;
		int unknown_0x324;
		int unknown_0x328;
		int unknown_0x32C;
		int unknown_0x330;
		int unknown_0x334;
		int unknown_0x338;
		int unknown_0x33C;
		int unknown_0x340;
		int unknown_0x344;
		int unknown_0x348;
		int unknown_0x34C;
		int unknown_0x350;
		int unknown_0x354;
		int unknown_0x358;
		int unknown_0x35C;
		int unknown_0x360;
		int unknown_0x364;
		int unknown_0x368;
		int unknown_0x36C;
		int unknown_0x370;
		int unknown_0x374;
		int unknown_0x378;
		int unknown_0x37C;
		int unknown_0x380;
		int unknown_0x384;
		int unknown_0x388;
		int unknown_0x38C;
		int unknown_0x390;
		int unknown_0x394;
		int unknown_0x398;
		int unknown_0x39C;
		int unknown_0x3A0;
		int unknown_0x3A4;
		int unknown_0x3A8;
		int unknown_0x3AC;
		int unknown_0x3B0;
		int unknown_0x3B4;
		int unknown_0x3B8;
		int unknown_0x3BC;
		int unknown_0x3C0;
		int unknown_0x3C4;
		int unknown_0x3C8;
		int unknown_0x3CC;
		int unknown_0x3D0;
		int unknown_0x3D4;
		int unknown_0x3D8;
		int unknown_0x3DC;
		int unknown_0x3E0;
		int unknown_0x3E4;
		int unknown_0x3E8;
		int unknown_0x3EC;
		int unknown_0x3F0;
		int unknown_0x3F4;
		int unknown_0x3F8;
		int unknown_0x3FC;
		int unknown_0x400;
		int unknown_0x404;
		int unknown_0x408;
		int unknown_0x40C;
		int unknown_0x410;
		int unknown_0x414;
		int unknown_0x418;
		int unknown_0x41C;
		int unknown_0x420;
		int unknown_0x424;
		int unknown_0x428;
		int unknown_0x42C;
		int unknown_0x430;
		int unknown_0x434;
		int unknown_0x438;
		int unknown_0x43C;
		int unknown_0x440;
		int unknown_0x444;
		int unknown_0x448;
		int unknown_0x44C;
		int unknown_0x450;
		int unknown_0x454;
		int unknown_0x458;
		int unknown_0x45C;
		int unknown_0x460;
		int unknown_0x464;
		int unknown_0x468;
		int unknown_0x46C;
		int unknown_0x470;
		int unknown_0x474;
		int unknown_0x478;
		int unknown_0x47C;
		int unknown_0x480;
		int unknown_0x484;
		int unknown_0x488;
		int unknown_0x48C;
		int unknown_0x490;
		int unknown_0x494;
		int unknown_0x498;
		int unknown_0x49C;
		int unknown_0x4A0;
		int unknown_0x4A4;
		int unknown_0x4A8;
		int unknown_0x4AC;
		int unknown_0x4B0;
		int unknown_0x4B4;
		int unknown_0x4B8;
		int unknown_0x4BC;
		int unknown_0x4C0;
		int unknown_0x4C4;
		int unknown_0x4C8;
		int unknown_0x4CC;
		int unknown_0x4D0;
		int unknown_0x4D4;
		int unknown_0x4D8;
		int unknown_0x4DC;
		int unknown_0x4E0;
		int unknown_0x4E4;
		int unknown_0x4E8;
		int unknown_0x4EC;
		int unknown_0x4F0;
		int unknown_0x4F4;
		int unknown_0x4F8;
		int unknown_0x4FC;
		int unknown_0x500;
		int unknown_0x504;
		int unknown_0x508;
		int unknown_0x50C;
		int unknown_0x510;
		int unknown_0x514;
		int unknown_0x518;
		int unknown_0x51C;
		int unknown_0x520;
		int unknown_0x524;
		int unknown_0x528;
		int unknown_0x52C;
		int unknown_0x530;
		int unknown_0x534;
		int unknown_0x538;
		int unknown_0x53C;
		int unknown_0x540;
		int unknown_0x544;
		int unknown_0x548;
		int unknown_0x54C;
		int unknown_0x550;
		int unknown_0x554;
		int unknown_0x558;
		int unknown_0x55C;
		TES3::HashMap<void*, void*> unknown_0x560;
		TES3::HashMap<void*, void*> unknown_0x570;
		TES3::HashMap<void*, void*> unknown_0x580;
		TES3::HashMap<void*, void*> unknown_0x590;
		int unknown_0x5A0;
		int unknown_0x5A4;
		int unknown_0x5A8;
		int unknown_0x5AC;
		int unknown_0x5B0;
		int unknown_0x5B4;
		int unknown_0x5B8;
		int unknown_0x5BC;
		int unknown_0x5C0;
		int unknown_0x5C4;
		int unknown_0x5C8;
		int unknown_0x5CC;
		int unknown_0x5D0;
		int unknown_0x5D4;
		int unknown_0x5D8;
		int unknown_0x5DC;
		int unknown_0x5E0;
		int unknown_0x5E4;
		int unknown_0x5E8;
		int unknown_0x5EC;
		int unknown_0x5F0;
		int unknown_0x5F4;
		int unknown_0x5F8;
		int unknown_0x5FC;
		int unknown_0x600;
		int unknown_0x604;
		int unknown_0x608;
		int unknown_0x60C;
		int unknown_0x610;
		int unknown_0x614;
		int unknown_0x618;
		int unknown_0x61C;
		int unknown_0x620;
		int unknown_0x624;
		int unknown_0x628;
		int unknown_0x62C;
		int unknown_0x630;
		int unknown_0x634;
		int unknown_0x638;
		int unknown_0x63C;
		int unknown_0x640;
		int unknown_0x644;
		int unknown_0x648;
		int unknown_0x64C;
		int unknown_0x650;
		int unknown_0x654;
		int unknown_0x658;
		int unknown_0x65C;
		int unknown_0x660;
		int unknown_0x664;
		int unknown_0x668;
		int unknown_0x66C;
		int unknown_0x670;
		int unknown_0x674;
		int unknown_0x678;
		int unknown_0x67C;
		int unknown_0x680;
		int unknown_0x684;
		int unknown_0x688;
		int unknown_0x68C;
		int unknown_0x690;
		int unknown_0x694;
		int unknown_0x698;
		int unknown_0x69C;

		DX8AdapterDesc* getCurrentAdapter() const;
	};
	static_assert(sizeof(DX8Renderer) == 0x6A0, "NI::DX8Renderer failed size validation");
}
