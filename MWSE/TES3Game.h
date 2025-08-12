#pragma once

#include "TES3Defines.h"

#include "TES3IteratedList.h"

#include "NIDefines.h"
#include "NIAmbientLight.h"

namespace TES3 {
	struct Game_vTable {
		void* dtor; // 0x0
		void* SetRegistryKey; // 0x4
		void* SetWindowWidth; // 0x8
		void* SetWindowHeight; // 0xC
		void* SetBitDepth; // 0x10
		void* SetBackBuffers; // 0x14
		void* SetMultisampleEnable; // 0x18
		void* SetMultisamples; // 0x1C
		void* SetFullScreen; // 0x20
		void(__thiscall* SetPixelShaderEnabled)(Game*, bool); // 0x24
		void* SetStencil; // 0x28
		void* SetMipmap; // 0x2C
		void* SetMipmapSkipLevel; // 0x30
		void* SetHardwareTL; // 0x34
		void* SetMultipass; // 0x38
		void* SetVertexProcessing; // 0x3C
		void* SetSwapEffect; // 0x40
		void* SetRefreshRate; // 0x44
		void* SetPresentInterval; // 0x48
		void* SetAdapterID; // 0x4C
		void(__thiscall* SetGamma)(Game*, float); // 0x50
		void* ReadSettings; // 0x54
		void* WriteSettings; // 0x58
		void* CreateRenderer; // 0x5C
		void* CreateRendererFromSettings; // 0x60
	};
	static_assert(sizeof(Game_vTable) == 0x64, "TES3::Game_vTable failed size validation");

	struct Game {
		Game_vTable* vTable;
		const char* registryKey; // 0x4
		int windowWidth; // 0x8
		int windowHeight; // 0xC
		int screenDepth; // 0x10
		int backBuffers; // 0x14
		int multiSamples; // 0x18
		bool fullscreen; // 0x1C
		bool stencil; // 0x1D
		bool mipmap; // 0x1E
		bool hardware; // 0x1F
		bool pixelShader; // 0x20
		bool multiPass; // 0x21
		bool screenShotsEnabled; // 0x22
		int vertexProcessing; // 0x24
		int swapEffect; // 0x28
		int refreshRate; // 0x2C
		int adapter; // 0x30
		int mipmapSkipLevel; // 0x34
		int presentationInterval; // 0x38
		float gamma; // 0x3C
		int unknown_0x40;
		int screenX; // 0x44
		int screenY; // 0x48
		float renderDistance; // 0x4C
		unsigned char volumeMaster; // 0x50
		unsigned char volumeVoice; // 0x51
		unsigned char volumeEffect; // 0x52
		unsigned char volumeFootsteps; // 0x53
		int volumeMedia; // 0x54
		unsigned char soundQuality; // 0x58
		char unknown_0x59;
		char unknown_0x5A;
		char unknown_0x5B;
		HWND parentWindowHandle; // 0x5C
		HWND windowHandle; // 0x60
		NI::DX8Renderer* renderer; // 0x64
		NI::Color unknown_0x68; // 0x68
		char unknown_0x74;
		char unknown_0x75;
		void* unknown_0x78;
		char unknown_0x7C;
		int unknown_0x80;
		char resolutionModeStringsVectorTag; // 0x84
		void* resolutionModeStringsBegin; // 0x88 - pointer to std::string
		void* resolutionModeStringsEnd; // 0x8C
		void* resolutionModeStringsStorageEnd; // 0x90
		void* showSceneGraphStruct; // 0x94
		IteratedList<NI::ObjectNET*>* unknown_0x98;
		NI::Pointer<NI::Node> worldRoot; // 0x9C
		NI::Pointer<NI::Node> worldObjectRoot; // 0xA0
		NI::Pointer<NI::Node> worldPickObjectRoot; // 0xA4
		NI::Pointer<NI::Node> worldLandscapeRoot; // 0xA8
		NI::Pointer<NI::Node> debugRoot; // 0xAC
		NI::Pointer<NI::WireframeProperty> wireframeProperty; // 0xB0
		NI::Pointer<NI::AmbientLight> activationAmbientLight; // 0xB4
		NI::Pointer<NI::Node> gridString; // 0xB8
		NI::Pointer<NI::Node> collideString; // 0xBC
		void* unknown_0xC0;
		void* unknown_0xC4;
		NI::Pointer<NI::Node> textureString; // 0xC8
		void* unknown_0xCC;
		void* unknown_0xD0[3]; // SG nodes?
		void* unknown_0xDC;
		void* unknown_0xE0;
		NI::Pick* playerTargetPick; // 0xE4
		Reference* playerTarget; // 0xE8
		Reference* tooltipTarget; // 0xEC
		NI::Pointer<NI::FogProperty> fogProperty; // 0xF0
		int compassHeading; // 0xF4
		void* unknown_0xF8;
		NI::Color currentFogColour; // 0xFC
		LoadScreenManager* loadScreenManager; // 0x108
		char skipRenderingNextFrame; // 0x10C
		char unknown_0x10D[3]; // Padding.

		Game() = delete;
		~Game() = delete;

		//
		// vTable accessor functions.
		//

		void setGamma(float value);

		//
		// Other related this-call functions.
		//

		bool initialize();

		void clearTarget();

		void savePlayerOptions();

		//
		// Custom functions.
		//

		void setPlayerTarget(Reference* reference);

		// Get singleton.
		_declspec (dllexport) static Game* get();

	};
	static_assert(sizeof(Game) == 0x110, "TES3::Game failed size validation");
	static_assert(offsetof(Game, Game::worldPickObjectRoot) == 0xA4, "TES3::Game failed offset validation");
	static_assert(offsetof(Game, Game::playerTarget) == 0xE8, "TES3::Game failed offset validation");
}
