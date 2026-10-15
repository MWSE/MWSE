#include "PatchReferenceNumbers.h"

#include "LogUtil.h"
#include "MemoryUtil.h"
#include "PathUtil.h"
#include "Settings.h"
#include "StringUtil.h"

#include "CSCell.h"
#include "CSGameFile.h"
#include "CSGlobalVariable.h"
#include "CSRecordHandler.h"
#include "CSReference.h"

namespace se::cs::patch::reference_numbers {
	constexpr DWORD ModMask = 0xFF000000;
	// With MWSE's raised plugin limit the game reads only 22 bits of a reference number.
	constexpr DWORD MaxReferenceNumber = 0x003FFFFF;
	constexpr auto MaxRefIndexGlobal = "MWSE_MAX_REF_INDEX";

	struct FileState {
		DWORD highest = 0;
		std::unordered_set<DWORD> writtenThisSave;
		bool logged = false;
	};

	std::unordered_map<std::string, FileState> fileStates;

	// The file the numbering continues from: the loaded active file, then the last file saved to.
	GameFile* loadedActiveFile = nullptr;

	// Which file a reference was loaded from or last saved to. The CS reassigns source files when merging plugins.
	struct LoadOrigin {
		const GameFile* file;
		DWORD number;
	};
	std::unordered_map<const Reference*, LoadOrigin> loadOrigins;

	std::string getFileKey(const GameFile& file) {
		std::string key = file.fileName;
		string::to_lower(key);
		return key;
	}

	FileState& getState(const GameFile& file) {
		return fileStates[getFileKey(file)];
	}

	bool hasValidNumber(const Reference& reference) {
		const auto number = static_cast<DWORD>(reference.targetID);
		return number != 0 && number <= MaxReferenceNumber;
	}

	// A plugin's references keep their numbers. References loaded from other plugins don't, since their numbers come
	// from another plugin. References created in this session belong to the saved plugin.
	bool isReferenceOwnedByFile(const Reference& reference, const GameFile& file) {
		if (reference.isFromMaster()) {
			return false;
		}

		const auto origin = loadOrigins.find(&reference);
		if (origin == loadOrigins.end() || origin->second.number != static_cast<DWORD>(reference.targetID)) {
			return true;
		}
		return origin->second.file == &file || origin->second.file == loadedActiveFile;
	}

	bool wasLoadedFromFile(const Reference& reference, const GameFile& file) {
		const auto origin = loadOrigins.find(&reference);
		return origin != loadOrigins.end() && origin->second.file == &file && origin->second.number == static_cast<DWORD>(reference.targetID);
	}

	DWORD getNewReferenceNumber(GameFile& file, FileState& state) {
		const DWORD number = std::max(file.lastReferenceNumber, state.highest) + 1;
		file.lastReferenceNumber = number;
		state.highest = number;
		return number;
	}

	// The same check the CS uses when writing a cell's references.
	bool willBeSaved(const Cell& cell, const Reference& reference, const GameFile& file) {
		const auto CS_ShouldSaveReference = reinterpret_cast<bool(__cdecl*)(bool, const Reference*, const GameFile*, bool, bool)>(0x40393B);
		return CS_ShouldSaveReference(cell.isFromMaster(), &reference, &file, false, false);
	}

	template <typename Func>
	void forEachSavedReference(const RecordHandler& recordHandler, const GameFile& file, Func&& func) {
		for (auto cell : *recordHandler.cells) {
			if (cell == nullptr) {
				continue;
			}
			for (auto list : { &cell->cellNpcRefs, &cell->cellObjRefs }) {
				for (auto reference : *list) {
					if (reference && willBeSaved(*cell, *reference, file)) {
						func(*reference);
					}
				}
			}
		}
	}

	//
	// MWSE_MAX_REF_INDEX is read straight from the active plugin, so other plugins' copies don't matter.
	//

	// CS-saved files store globals right after game settings, so a master's global can be found without reading the whole file.
	std::optional<DWORD> readMaxRefIndex(const std::filesystem::path& path, bool stopAfterGlobals = false) {
		std::ifstream file(path, std::ios::binary);
		if (!file.is_open()) {
			return {};
		}

		struct RecordHeader {
			char tag[4];
			std::uint32_t size;
			std::uint32_t unknown;
			std::uint32_t flags;
		};
		static_assert(sizeof(RecordHeader) == 16);

		RecordHeader header = {};
		while (file.read(reinterpret_cast<char*>(&header), sizeof(header))) {
			if (stopAfterGlobals && std::memcmp(header.tag, "TES3", 4) != 0 && std::memcmp(header.tag, "GMST", 4) != 0 && std::memcmp(header.tag, "GLOB", 4) != 0) {
				break;
			}

			if (std::memcmp(header.tag, "GLOB", 4) != 0) {
				file.seekg(header.size, std::ios::cur);
				continue;
			}

			std::vector<char> data(header.size);
			if (!file.read(data.data(), data.size())) {
				break;
			}

			std::string_view name;
			std::optional<float> value;
			for (size_t i = 0; i + 8 <= data.size();) {
				std::uint32_t size = 0;
				std::memcpy(&size, &data[i + 4], sizeof(size));
				const auto payload = i + 8;
				if (payload + size > data.size()) {
					break;
				}

				if (std::memcmp(&data[i], "NAME", 4) == 0) {
					name = std::string_view(&data[payload], strnlen(&data[payload], size));
				}
				else if (std::memcmp(&data[i], "FLTV", 4) == 0 && size == sizeof(float)) {
					float f = 0.0f;
					std::memcpy(&f, &data[payload], sizeof(f));
					value = f;
				}
				i = payload + size;
			}

			if (string::iequal(name, MaxRefIndexGlobal) && value && *value >= 0.0f && *value <= float(MaxReferenceNumber)) {
				return static_cast<DWORD>(*value);
			}
		}

		return {};
	}

	void writeMaxRefIndex(RecordHandler& recordHandler, GameFile& file, DWORD highest) {
		if (highest == 0) {
			return;
		}

		auto global = recordHandler.getGlobal(MaxRefIndexGlobal);
		if (global == nullptr) {
			// The CS frees globals itself, so allocate with its allocator.
			global = new (memory::_new(sizeof(GlobalVariable))) GlobalVariable(MaxRefIndexGlobal);
			global->valueType = 'l';
			global->sourceFile = &file;
			recordHandler.globals->push_back(global);
		}

		global->value = static_cast<float>(highest);
		global->setModified(true);
	}

	//
	// Hooks.
	//

	void __cdecl OnLoadReference(Reference* reference, GameFile* file, DWORD formId) {
		reference->sourceID = 0;
		reference->targetID = 0;

		if (formId == 0 || (formId & ModMask) != 0) {
			return;
		}

		reference->targetID = static_cast<int>(formId);
		loadOrigins[reference] = { file, formId };

		auto& state = getState(*file);
		state.highest = std::max(state.highest, formId);
	}

	void warnOutOfNumbers(const GameFile& file, size_t count) {
		std::stringstream message;
		message << file.fileName << " needs more than " << MaxReferenceNumber << " reference numbers, the most the game can read. "
			<< "Its " << count << " references were numbered again from the start, as the CS does without this patch. "
			<< "Saves made with the previous version of this file won't match it.";
		log::stream << "[ReferenceNumbers] " << message.str() << std::endl;
		MessageBoxA(NULL, message.str().c_str(), "Reference numbers", MB_OK | MB_ICONWARNING);
	}

	// Number all of the plugin's references before anything is written, so MWSE_MAX_REF_INDEX is final when globals are saved.
	void __cdecl OnBeforeSaveGlobals(RecordHandler* recordHandler, GameFile* file) {
		auto& state = getState(*file);
		state.writtenThisSave.clear();
		if (loadedActiveFile && loadedActiveFile != file) {
			state.highest = std::max(state.highest, getState(*loadedActiveFile).highest);
		}

		std::unordered_set<DWORD> used;
		std::vector<Reference*> saved;
		std::vector<Reference*> needNumbers;
		size_t duplicates = 0;
		DWORD masterHighest = 0;

		// Merge to Masters: the master's own references are written with their number as it is,
		// and the merged plugin's references are added after them.
		const auto savingMaster = file->getIsMasterFile();
		const auto mergingIntoMaster = savingMaster && file != loadedActiveFile;
		if (savingMaster) {
			forEachSavedReference(*recordHandler, *file, [&](Reference& reference) {
				if (reference.isFromMaster() && hasValidNumber(reference)) {
					used.insert(static_cast<DWORD>(reference.targetID));
					masterHighest = std::max(masterHighest, static_cast<DWORD>(reference.targetID));
				}
			});
			state.highest = std::max(state.highest, masterHighest);
		}

		forEachSavedReference(*recordHandler, *file, [&](Reference& reference) {
			if (reference.isFromMaster()) {
				return;
			}
			saved.push_back(&reference);

			// References from other plugins get a new number, as in vanilla.
			if (!isReferenceOwnedByFile(reference, *file) || (mergingIntoMaster && !wasLoadedFromFile(reference, *file)) || !hasValidNumber(reference)) {
				needNumbers.push_back(&reference);
			}
			else if (!used.insert(static_cast<DWORD>(reference.targetID)).second) {
				needNumbers.push_back(&reference);
				duplicates++;
			}
			else {
				state.highest = std::max(state.highest, static_cast<DWORD>(reference.targetID));
			}
		});

		if (std::max(file->lastReferenceNumber, state.highest) + needNumbers.size() > MaxReferenceNumber) {
			warnOutOfNumbers(*file, saved.size());
			used.clear();
			duplicates = 0;
			file->lastReferenceNumber = masterHighest;
			state.highest = masterHighest;
			needNumbers = saved;
		}

		for (auto reference : needNumbers) {
			reference->targetID = static_cast<int>(getNewReferenceNumber(*file, state));
		}

		// The numbering continues from the saved file, so saving under another name again keeps these numbers.
		for (auto reference : saved) {
			loadOrigins[reference] = { file, static_cast<DWORD>(reference->targetID) };
		}
		loadedActiveFile = file;

		writeMaxRefIndex(*recordHandler, *file, state.highest);

		// Only log the first save of a file, and saves that assigned new numbers.
		if (state.logged && needNumbers.empty()) {
			return;
		}
		state.logged = true;

		log::stream << "[ReferenceNumbers] Saving " << file->fileName << ": " << used.size() << " references kept their number, "
			<< needNumbers.size() << " new numbers assigned (" << duplicates << " duplicates). " << MaxRefIndexGlobal << " = " << state.highest << "." << std::endl;
	}

	DWORD __cdecl OnSaveReferenceNumber(Reference* reference, GameFile* file, DWORD current, bool vanillaWantsNew) {
		auto& state = getState(*file);

		if (isReferenceOwnedByFile(*reference, *file)) {
			if (hasValidNumber(*reference) && state.writtenThisSave.insert(static_cast<DWORD>(reference->targetID)).second) {
				return static_cast<DWORD>(reference->targetID);
			}
			reference->targetID = static_cast<int>(getNewReferenceNumber(*file, state));
			state.writtenThisSave.insert(static_cast<DWORD>(reference->targetID));
			log::stream << "[ReferenceNumbers] Late number " << reference->targetID << " for '" << reference->getObjectID() << "'; " << MaxRefIndexGlobal << " is now out of date." << std::endl;
			return static_cast<DWORD>(reference->targetID);
		}

		if (reference->isFromMaster() && !vanillaWantsNew && current != 0) {
			return current;
		}

		return getNewReferenceNumber(*file, state);
	}

	// Replaces 0x53657A. esi = reference, edi = file, [esp+18h] = FRMR.
	__declspec(naked) void PatchLoadReference() {
		__asm {
			mov eax, [esp + 0x18]
			push eax
			push edi
			push esi
			call OnLoadReference
			add esp, 0xC
			xor eax, eax
			push eax
			mov ecx, 0x536583
			jmp ecx
		}
	}

	// Replaces 0x53877D. ebp = reference, esi = file, [esp+10h] = FRMR.
	__declspec(naked) void PatchSaveReferenceNumber() {
		__asm {
			mov eax, [esp + 0x10]
			push 0
			push eax
			push esi
			push ebp
			call OnSaveReferenceNumber
			add esp, 0x10
			mov [esp + 0x10], eax
			mov eax, 0x538796
			jmp eax
		}
	}

	// Replaces 0x538785, the path where vanilla always assigns a new number.
	__declspec(naked) void PatchSaveNewReferenceNumber() {
		__asm {
			mov eax, [esp + 0x10]
			push 1
			push eax
			push esi
			push ebp
			call OnSaveReferenceNumber
			add esp, 0x10
			mov [esp + 0x10], eax
			mov eax, 0x538796
			jmp eax
		}
	}

	// Replaces 0x502295, right before globals are saved. esi = record handler, ebp = file.
	__declspec(naked) void PatchBeforeSaveGlobals() {
		__asm {
			push ebp
			push esi
			call OnBeforeSaveGlobals
			add esp, 0x8
			push 0x6BD994
			mov eax, 0x402BFD
			call eax
			mov eax, 0x50229F
			jmp eax
		}
	}

	bool bytesMatch(DWORD address, std::initializer_list<BYTE> expected) {
		auto current = reinterpret_cast<const BYTE*>(address);
		for (const auto b : expected) {
			if (*current++ != b) {
				return false;
			}
		}
		return true;
	}

	bool installed = false;

	void onBeforeFilesLoaded() {
		loadOrigins.clear();
	}

	void onFilesLoaded(RecordHandler& recordHandler) {
		if (!installed) {
			return;
		}

		loadedActiveFile = recordHandler.activeFile;
		if (loadedActiveFile == nullptr) {
			return;
		}

		auto& file = *recordHandler.activeFile;
		if (const auto value = readMaxRefIndex(path::getDataFilesPath() / file.fileName)) {
			auto& state = getState(file);
			state.highest = std::max(state.highest, *value);
		}

		// Masters can be merged into, and their global may be overridden by the active plugin's.
		for (auto i = 0; i < recordHandler.activeModCount; ++i) {
			const auto master = recordHandler.activeGameFiles[i];
			if (master == nullptr || master == loadedActiveFile || !master->getIsMasterFile()) {
				continue;
			}
			if (const auto value = readMaxRefIndex(path::getDataFilesPath() / master->fileName, true)) {
				auto& state = getState(*master);
				state.highest = std::max(state.highest, *value);
			}
		}
	}

	void installPatches() {
		if (!settings.reference_numbers.preserve) {
			return;
		}

		const bool valid = bytesMatch(0x53657A, { 0x33, 0xC0, 0x89, 0x46, 0x70, 0x89, 0x46, 0x6C, 0x50 })
			&& bytesMatch(0x53877D, { 0x8B, 0x44, 0x24, 0x10, 0x85, 0xC0, 0x75, 0x11 })
			&& bytesMatch(0x538785, { 0x8B, 0x86, 0xEC, 0x04, 0x00, 0x00, 0x40, 0x89, 0x86, 0xEC, 0x04, 0x00, 0x00, 0x89, 0x44, 0x24, 0x10 })
			&& bytesMatch(0x502295, { 0x68, 0x94, 0xD9, 0x6B, 0x00, 0xE8, 0x5E, 0x09, 0xF0, 0xFF });
		if (!valid) {
			log::stream << "[ReferenceNumbers] Unexpected code in the Construction Set executable. Patch not installed." << std::endl;
			return;
		}

		using memory::genJumpUnprotected;

		genJumpUnprotected(0x53657A, reinterpret_cast<DWORD>(PatchLoadReference), 0x9);
		genJumpUnprotected(0x53877D, reinterpret_cast<DWORD>(PatchSaveReferenceNumber), 0x8);
		genJumpUnprotected(0x538785, reinterpret_cast<DWORD>(PatchSaveNewReferenceNumber), 0x11);
		genJumpUnprotected(0x502295, reinterpret_cast<DWORD>(PatchBeforeSaveGlobals), 0xA);

		installed = true;
	}
}
