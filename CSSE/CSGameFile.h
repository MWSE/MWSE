#pragma once

namespace se::cs {
	struct GameFile {
		unsigned int unknown_0x0;
		unsigned int unknown_0x4;
		void* ioObject;
		char fileName[260]; // 0xC
		char filePath[260]; // 0x110
		int unknown_0x214;
		int unknown_0x218;
		int unknown_0x21C;
		int unknown_0x220;
		int unknown_0x224;
		int unknown_0x228;
		int unknown_0x22C;
		int unknown_0x230;
		int unknown_0x234;
		int unknown_0x238;
		int unknown_0x23C;
		int unknown_0x240;
		int unknown_0x244;
		int unknown_0x248;
		int unknown_0x24C;
		int unknown_0x250;
		int unknown_0x254;
		int unknown_0x258;
		int unknown_0x25C;
		int unknown_0x260;
		int unknown_0x264;
		int unknown_0x268;
		WIN32_FIND_DATA findFileData; // 0x26C
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
		GameFile** masters; // 0x4E8
		int unknown_0x4EC;
		int unknown_0x4F0;

		bool getIsMasterFile() const;

		bool getToLoadFlag() const;
		void setToLoadFlag(bool state);

		int sortAgainst(const GameFile* other) const;
	};
	static_assert(sizeof(GameFile) == 0x4F4, "GameFile failed size validation");
}
