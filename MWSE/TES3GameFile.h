#pragma once

#include "TES3Collections.h"

namespace TES3 {
	struct GameFile {
		struct GMDT {
			float currentHealth;
			float maxHealth;
			float gameHour;
			float day;
			float month;
			float year;
			char cellName[64];
			float daysPassed;
			char playerName[32];
		};

		int errorCode;
		void* ioObjectBeforeSave;
		void* ioObject;
		char filename[260];
		char path[260];
		void* field_214;
		int field_218;
		int field_21C;
		int field_220;
		int field_224;
		int field_228;
		int currFormHeader[4];
		int currChunkHeader[2];
		int field_244;
		unsigned int fileOffsetNextForm;
		unsigned int countFormBytesRead;
		int field_250;
		int writeFormHeader[4];
		unsigned int fileOffsetCurrentForm;
		unsigned int bytesWritten;
		char findData[0x140];	// Real type: WIN32_FIND_DATAA
		float version;
		int flags_3B0;
		char author[32];
		char description[256];
		unsigned int countRecords;
		int flags_4D8;
		int loadIndex;
		LinkedList<void>* masterNames;
		LinkedList<void>* masterFileSizes;
		GameFile* arrayMasters;
		unsigned int highestFormID;
		GMDT gmdt;
		void* sgSaveImage;
		Iterator<void>* list_570;
	};
	static_assert(sizeof(GameFile) == 0x574, "TES3::Game failed size validation");
}
