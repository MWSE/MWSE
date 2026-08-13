#include "TES3GameFile.h"

#if defined(SE_TARGETS_MW) && SE_TARGETS_MW == 1

#include "TES3DataHandler.h"

#endif

namespace TES3 {
#if defined(SE_TARGETS_MW) && SE_TARGETS_MW == 1
	
	using std::uint64_t;

	const auto TES3File_ctor = reinterpret_cast<GameFile * (__thiscall*)(GameFile*, const char*, const char*, void*)>(0x4B3F10);
	GameFile::GameFile(const char* path, const char* fileName, void* unknown) {
		TES3File_ctor(this, path, fileName, unknown);
	}

	const auto TES3File_dtor = reinterpret_cast<int(__thiscall*)(GameFile*)>(0x4B4140);
	GameFile::~GameFile() {
		TES3File_dtor(this);
	}

	const auto TES3File_delete = reinterpret_cast<void(__thiscall*)(GameFile*)>(0x4B48F0);
	void GameFile::deleteFile() {
		// Constrain to save games only
		size_t s = strlen(filename);
		if (s > 4) {
			const char* ext = filename + (s - 4);
			if (_stricmp(ext, ".ess") == 0) {
				TES3File_delete(this);
			}
		}
	}

	const auto TES3File_readChunkData = reinterpret_cast<bool(__thiscall*)(GameFile*, void*, unsigned int)>(0x4B6880);
	bool GameFile::readChunkData(void* data, unsigned int size) {
		return TES3File_readChunkData(this, data, size);
	}

	int GameFile::writeChunkString(unsigned int tag, const std::string& string) {
		return writeChunkData(tag, string.data(), string.length() + 1);
	}

	const auto TES3File_writeChunkData = reinterpret_cast<int(__thiscall*)(GameFile*, unsigned int, const void*, unsigned int)>(0x4B6BA0);
	int GameFile::writeChunkData(unsigned int tag, const void* data, unsigned int size) {
		return TES3File_writeChunkData(this, tag, data, size);
	}

	const auto TES3File_writeRecordHeader = reinterpret_cast<int(__thiscall*)(GameFile*, unsigned int, unsigned int)>(0x4B6B00);
	int GameFile::writeRecordHeader(unsigned int tag, unsigned int flags) {
		return TES3File_writeRecordHeader(this, tag, flags);
	}
	
	const auto TES3File_endRecord = reinterpret_cast<int(__thiscall*)(GameFile*)>(0x4B6C50);
	int GameFile::endRecord() {
		return TES3File_endRecord(this);
	}

	const auto TES3File_getFirstSubrecord = reinterpret_cast<unsigned int(__thiscall*)(GameFile*)>(0x4B6750);
	unsigned int GameFile::getFirstSubrecord() {
		return TES3File_getFirstSubrecord(this);
	}

	const auto TES3File_hasNextSubrecord = reinterpret_cast<bool(__thiscall*)(GameFile*)>(0x4B67F0);
	bool GameFile::hasNextSubrecord() {
		return TES3File_hasNextSubrecord(this);
	}

	const auto TES3File_getNextSubrecord = reinterpret_cast<unsigned int(__thiscall*)(GameFile*)>(0x4B67C0);
	unsigned int GameFile::getNextSubrecord() {
		return TES3File_getNextSubrecord(this);
	}

	const auto TES3File_hasMoreRecords = reinterpret_cast<int(__thiscall*)(GameFile*)>(0x4B67F0);
	bool GameFile::hasMoreRecords() {
		return TES3File_hasMoreRecords(this);
	}

	const auto TES3File_nextRecord = reinterpret_cast<bool(__thiscall*)(GameFile*, int)>(0x4B6380);
	bool GameFile::nextRecord(int flag) {
		return TES3File_nextRecord(this, flag);
	}

	const auto TES3File_collectActiveMods2 = reinterpret_cast<bool(__thiscall*)(GameFile*, void*, bool)>(0x4B5BB0);
	bool GameFile::collectActiveMods(bool showMasterErrors) {
		return TES3File_collectActiveMods2(this, DataHandler::get()->nonDynamicData->gameFiles, showMasterErrors);
	}

	bool GameFile::load(int unknown1, bool unknown2) {
		return loadByPath(path, filename, unknown1, unknown2);
	}

	const auto TES3File_loadByPath = reinterpret_cast<bool(__thiscall*)(GameFile*, const char*, const char*, int, bool)>(0x4B4530);
	bool GameFile::loadByPath(const char* path, const char* fileName, int unknown1, bool unknown2) {
		return TES3File_loadByPath(this, path, fileName, unknown1, unknown2);
	}
	
	const auto TES3File_readFirstChunk = reinterpret_cast<unsigned int(__thiscall*)(GameFile*)>(0x4B6750);
	unsigned int GameFile::readFirstChunk() {
		return TES3File_readFirstChunk(this);
	}

	const auto TES3File_readNextChunk = reinterpret_cast<unsigned int(__thiscall*)(GameFile*)>(0x4B67C0);
	unsigned int GameFile::readNextChunk() {
		return TES3File_readNextChunk(this);
	}

	const auto TES3File_nextForm = reinterpret_cast<bool(__thiscall*)(GameFile*, int)>(0x4B6380);
	bool GameFile::nextForm(int flag) {
		return TES3File_nextForm(this, flag);
	}

	const auto TES3File_setFilePointer = reinterpret_cast<bool(__thiscall*)(GameFile*, unsigned int)>(0x4B6520);
	bool GameFile::setFilePointer(unsigned int offset) {
		return TES3File_setFilePointer(this, offset);
	}

	const auto TES3File_reopen = reinterpret_cast<bool(__thiscall*)(GameFile*, int, bool)>(0x4B4510);
	bool GameFile::reopen(int mode, bool writable) {
		return TES3File_reopen(this, mode, writable);
	}

	const auto TES3File_close = reinterpret_cast<bool(__thiscall*)(GameFile*)>(0x4B47C0);
	bool GameFile::close() {
		return TES3File_close(this);
	}

	const auto TES3File_getToLoad = reinterpret_cast<bool(__thiscall*)(const GameFile*)>(0x4B6250);
	bool GameFile::getToLoad() const {
		return TES3File_getToLoad(this);
	}

	const auto TES3File_setToLoad = reinterpret_cast<void(__thiscall*)(GameFile*, bool)>(0x4B6280);
	void GameFile::setToLoad(bool set) {
		TES3File_setToLoad(this, set);
	}

	const auto TES3File_getMaster = reinterpret_cast<GameFile* (__thiscall*)(GameFile*, unsigned int)>(0x4B5CF0);
	GameFile* GameFile::getMaster(unsigned int index) {
		return TES3File_getMaster(this, index);
	}

	std::uint64_t GameFile::getFileSize() const {
		return (uint64_t(findData.nFileSizeHigh) << 32) + uint64_t(findData.nFileSizeLow);
	}

	std::uint64_t GameFile::getModifiedTime() const {
		return (uint64_t(findData.ftLastWriteTime.dwHighDateTime) << 32) + uint64_t(findData.ftLastWriteTime.dwLowDateTime);
	}

	const char* GameFile::getFilename() const {
		return filename;
	}

	const char* GameFile::getPath() const {
		return path;
	}

	const char* GameFile::getAuthor() const {
		return author;
	}

	const char* GameFile::getDescription() const {
		return description;
	}

	float GameFile::getCurrentHealth() const {
		return gmdt.currentHealth;
	}

	float GameFile::getMaxHealth() const {
		return gmdt.maxHealth;
	}

	float GameFile::getGameHour() const {
		return gmdt.gameHour;
	}

	float GameFile::getDay() const {
		return gmdt.day;
	}

	float GameFile::getMonth() const {
		return gmdt.month;
	}

	float GameFile::getYear() const {
		return gmdt.year;
	}

	const char* GameFile::getCellName() const {
		return gmdt.cellName;
	}

	float GameFile::getDaysPassed() const {
		return gmdt.daysPassed;
	}

	const char* GameFile::getPlayerName() const {
		return gmdt.playerName;
	}

	const auto TES3File_getFlag20 = reinterpret_cast<bool(__thiscall*)(const GameFile*)>(0x4B6200);
	bool GameFile::getFlag20() const {
		return TES3File_getFlag20(this);
	}

	std::span<GameFile*> GameFile::getMasters() {
		return std::span(arrayMasters, masterNames->size());
	}

#elif defined(SE_TARGETS_CS) && SE_TARGETS_CS == 1

	const auto GameFile_getIsMasterFile = reinterpret_cast<bool(__thiscall*)(const GameFile*)>(0x402B1C);
	bool GameFile::getIsMasterFile() const {
		return GameFile_getIsMasterFile(this);
	}

	const auto GameFile_getToLoadFlag = reinterpret_cast<bool(__thiscall*)(const GameFile*)>(0x40148D);
	bool GameFile::getToLoadFlag() const {
		return GameFile_getToLoadFlag(this);
	}

	const auto GameFile_setToLoadFlag = reinterpret_cast<void(__thiscall*)(GameFile*, bool)>(0x401D84);
	void GameFile::setToLoadFlag(bool state) {
		GameFile_setToLoadFlag(this, state);
	}

	int GameFile::sortAgainst(const GameFile* other) const {
		const auto isMaster = getIsMasterFile();
		const auto otherIsMaster = other->getIsMasterFile();
		if (isMaster != otherIsMaster) {
			return otherIsMaster ? 1 : -1;
		}

		return CompareFileTime(&findData.ftLastWriteTime, &other->findData.ftLastWriteTime);;
	}
#endif
}
