#pragma once

#include "TES3Defines.h"

#include "TES3IteratedList.h"

#include "NIDefines.h"
#include "NIAmbientLight.h"

namespace TES3 {
	struct Game_vTable {
		void * unknown_0x0;
		void * unknown_0x4;
		void * unknown_0x8;
		void * unknown_0xC;
		void * unknown_0x10;
		void * unknown_0x14;
		void * unknown_0x18;
		void * unknown_0x1C;
		void * unknown_0x20;
		void(__thiscall* setPixelShaderEnabled)(Game*, bool); // 0x24
		void * unknown_0x28;
		void * unknown_0x2C;
		void * unknown_0x30;
		void * unknown_0x34;
		void * unknown_0x38;
		void * unknown_0x3C;
		void * unknown_0x40;
		void * unknown_0x44;
		void * unknown_0x48;
		void * unknown_0x4C;
		void(__thiscall * setGamma)(Game*, float); // 0x50
		void * unknown_0x54;
		void * unknown_0x58;
		void * unknown_0x5C;
		void * unknown_0x60;
	};
	static_assert(sizeof(Game_vTable) == 0x64, "TES3::Game_vTable failed size validation");

	struct Game {
		Game_vTable * vTable;
		void * unknown_0x4;
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
		NI::Renderer* renderer; // 0x64
		int unknown_0x68;
		int unknown_0x6C;
		int unknown_0x70;
		char unknown_0x74;
		char unknown_0x75;
		void * unknown_0x78;
		char unknown_0x7C;
		int unknown_0x80;
		int unknown_0x84;
		void * unknown_0x88; // std::string?
		int unknown_0x8C;
		int unknown_0x90;
		int unknown_0x94;
		IteratedList<NI::ObjectNET*>* unknown_0x98;
		NI::Pointer<NI::Node> worldRoot; // 0x9C
		NI::Pointer<NI::Node> worldObjectRoot; // 0xA0
		NI::Pointer<NI::Node> worldPickObjectRoot; // 0xA4
		NI::Pointer<NI::Node> worldLandscapeRoot; // 0xA8
		NI::Pointer<NI::Node> debugRoot; // 0xAC
		NI::Pointer<NI::Property> wireframeProperty; // 0xB0
		NI::Pointer<NI::AmbientLight> activationAmbientLight; // 0xB4
		NI::Pointer<NI::Node> gridString; // 0xB8
		NI::Pointer<NI::Node> collideString; // 0xBC
		int unknown_0xC0;
		int unknown_0xC4;
		NI::Pointer<NI::Node> textureString; // 0xC8
		int unknown_0xCC;
		void * unknown_0xD0[3]; // SG nodes?
		int unknown_0xDC;
		int unknown_0xE0;
		NI::Pick* playerTargetPick; // 0xE4
		Reference* playerTarget; // 0xE8
		int unknown_0xEC;
		NI::Pointer<NI::FogProperty> fogProperty; // 0xF0
		int unknown_0xF4;
		int unknown_0xF8;
		int unknown_0xFC;
		int unknown_0x100;
		int unknown_0x104;
		int unknown_0x108;
		char unknown_0x10C;
		char unknown_0x10D[3]; // Padding.

		//
		// Extra static data fields.
		//

		static Reference* previousPlayerTarget;

		//
		// vTable accessor functions.
		//

		void setGamma(float value);

		//
		// Other related this-call functions.
		//

		bool initialize();

		void clearTarget();

		//
		// Custom functions.
		//

		// Get singleton.
		_declspec (dllexport) static Game* get();

	};
	static_assert(sizeof(Game) == 0x110, "TES3::Game failed size validation");
}
