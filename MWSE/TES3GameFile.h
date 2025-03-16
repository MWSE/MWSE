#pragma once

#include "TES3Defines.h"

#include "TES3IteratedList.h"
#include "TES3StlList.h"

namespace TES3 {
	struct GameFile {
		struct Header {
			char tag[4];
			unsigned int size;
			unsigned int data;
			unsigned int flags;
		};
		struct ChunkHeader {
			char tag[4];
			unsigned int size;
		};
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

		int errorCode; // 0x0
		void* ioObjectBeforeSave; // 0x4
		void* ioObject; // 0x8
		char filename[260]; // 0xC
		char path[260]; // 0x110
		void* field_214;
		int field_218;
		int field_21C;
		unsigned int field_220; // 0x200
		Cell* field_224; // 0x224
		int field_228; // 0x228
		Header currentFormHeader; // 0x22C
		ChunkHeader currentChunkHeader; // 0x23C
		int field_244;
		unsigned int fileOffsetNextForm; // 0x248
		unsigned int countFormBytesRead; // 0x24C
		int field_250;
		Header writeFormHeader; // 0x254
		unsigned int fileOffsetCurrentForm; // 0x264
		unsigned int bytesWritten; // 0x268
		char findData[0x140]; // 0x26C // Real type: WIN32_FIND_DATAA
		float version; // 0x3AC
		int flags_3B0;
		char author[32]; // 0x3B4
		char description[256]; // 0x3D4
		unsigned int countRecords; // 0x4D4
		int flags_4D8;
		int loadIndex; // 0x4DC
		StlList<const char*>* masterNames; // 0x4E0
		StlList<unsigned int>* masterFileSizes; // 0x4E4
		GameFile** arrayMasters; // 0x4E8
		unsigned int highestFormID; // 0x4EC
		GMDT gmdt; // 0x4F0
		void* sgSaveImage; // 0x56C
		IteratedList<void*>* list_570;

		//
		// Other related this-call functions.
		//

		GameFile(const char* path, const char* filename, void * unknown = nullptr);
		~GameFile();

		void deleteFile();
		bool readChunkData(void * data, unsigned int size = 0);
		int writeChunkData(unsigned int tag, const void * data, unsigned int size);
		int writeRecordHeader(unsigned int tag, unsigned int flags);
		int endRecord();

		unsigned int getFirstSubrecord();
		bool hasNextSubrecord();
		unsigned int getNextSubrecord();
		bool hasMoreRecords();
		bool nextRecord(int flag = 1);

		bool collectActiveMods(bool showMasterErrors = false);
		bool load(int unknown1 = 0, bool unknown2 = false);
		bool loadByPath(const char* path, const char* filename, int unknown1 = 0, bool unknown2 = false);
		bool setFilePointer(unsigned int offset);
		bool reopen(int mode = 0, bool writable = false);
		bool close();

		bool getToLoad() const;
		void setToLoad(bool set);

		GameFile* getMaster(unsigned int index);

		//
		// Custom functions.
		//

		std::uint64_t getFileSize() const;
		std::uint64_t getModifiedTime() const;

		const char* getFilename() const;
		const char* getPath() const;
		const char* getAuthor() const;
		const char* getDescription() const;

		float getCurrentHealth() const;
		float getMaxHealth() const;
		float getGameHour() const;
		float getDay() const;
		float getMonth() const;
		float getYear() const;
		const char* getCellName() const;
		float getDaysPassed() const;
		const char* getPlayerName() const;

		nonstd::span<GameFile*> getMasters();

	};
	static_assert(sizeof(GameFile) == 0x574, "TES3::GameFile failed size validation");
	static_assert(sizeof(GameFile::GMDT) == 0x7C, "TES3::GameFile::GMDT failed size validation");
}
