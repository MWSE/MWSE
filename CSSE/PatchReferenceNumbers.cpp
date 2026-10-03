#include "PatchReferenceNumbers.h"

#include "LogUtil.h"
#include "MemoryUtil.h"
#include "PathUtil.h"
#include "Settings.h"
#include "StringUtil.h"

#include "CSGameFile.h"
#include "CSReference.h"

namespace se::cs::patch::reference_numbers {
	constexpr DWORD ModMask = 0xFF000000;

	struct FileState {
		DWORD highest = 0;
		DWORD persisted = 0;
		std::unordered_set<DWORD> usedThisSave;
		size_t preservedCount = 0;
		size_t assignedCount = 0;
		size_t conflictCount = 0;
	};

	std::unordered_map<std::string, FileState> fileStates;

	std::string getFileKey(const GameFile& file) {
		std::string key = file.fileName;
		string::to_lower(key);
		return key;
	}

	FileState& getState(const GameFile& file) {
		return fileStates[getFileKey(file)];
	}

	//
	// Persistence of the highest used number per plugin.
	//

	std::filesystem::path getPersistencePath() {
		return path::getInstallPath() / "csse_reference_numbers.txt";
	}

	void loadPersistedNumbers() {
		std::ifstream file(getPersistencePath());
		if (!file.is_open()) {
			return;
		}

		std::string line;
		while (std::getline(file, line)) {
			const auto separator = line.find('\t');
			if (separator == std::string::npos) {
				continue;
			}

			try {
				const auto value = std::stoul(line.substr(separator + 1));
				auto& state = fileStates[line.substr(0, separator)];
				state.persisted = std::max<DWORD>(state.persisted, value);
				state.highest = std::max<DWORD>(state.highest, value);
			}
			catch (std::exception&) {
				continue;
			}
		}
	}

	void savePersistedNumbers() {
		if (!settings.reference_numbers.remember_highest) {
			return;
		}

		const auto dirty = std::any_of(fileStates.begin(), fileStates.end(), [](const auto& entry) {
			return entry.second.highest > entry.second.persisted;
		});
		if (!dirty) {
			return;
		}

		std::ofstream file(getPersistencePath(), std::ios::trunc);
		if (!file.is_open()) {
			log::stream << "[ReferenceNumbers] Could not write " << getPersistencePath().string() << "." << std::endl;
			return;
		}

		for (auto& [key, state] : fileStates) {
			if (state.highest == 0) {
				continue;
			}
			file << key << '\t' << state.highest << '\n';
			state.persisted = state.highest;
		}
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

		auto& state = getState(*file);
		state.highest = std::max(state.highest, formId);
	}

	DWORD getNewReferenceNumber(GameFile& file, FileState& state) {
		DWORD number = std::max(file.lastReferenceNumber, state.highest);
		do {
			++number;
		} while (state.usedThisSave.contains(number));

		file.lastReferenceNumber = number;
		state.usedThisSave.insert(number);
		state.highest = std::max(state.highest, number);
		return number;
	}

	bool isReferenceOwnedByFile(const Reference& reference, const GameFile& file) {
		return !reference.isFromMaster() && reference.sourceFile == &file;
	}

	DWORD __cdecl OnSaveReferenceNumber(Reference* reference, GameFile* file, DWORD current, bool vanillaWantsNew) {
		auto& state = getState(*file);

		if (!isReferenceOwnedByFile(*reference, *file)) {
			// Master references keep their number. References from other plugins get a new one, as in vanilla.
			if (vanillaWantsNew || current == 0 || !reference->isFromMaster()) {
				return getNewReferenceNumber(*file, state);
			}
			return current;
		}

		if (current != 0 && (current & ModMask) == 0) {
			if (state.usedThisSave.insert(current).second) {
				state.highest = std::max(state.highest, current);
				state.preservedCount++;
				return current;
			}

			state.conflictCount++;
			log::stream << "[ReferenceNumbers] Duplicate reference number " << current << " for '" << reference->getObjectID() << "'; assigning a new number." << std::endl;
		}

		const auto number = getNewReferenceNumber(*file, state);
		reference->targetID = static_cast<int>(number);
		state.assignedCount++;
		return number;
	}

	void __cdecl OnBeginSaveReferences(GameFile* file) {
		auto& state = getState(*file);
		state.usedThisSave.clear();
		state.preservedCount = 0;
		state.assignedCount = 0;
		state.conflictCount = 0;

		file->lastReferenceNumber = state.highest;
	}

	void __cdecl OnEndSaveReferences(GameFile* file) {
		if (file == nullptr) {
			return;
		}

		const auto& state = getState(*file);
		log::stream << "[ReferenceNumbers] Saved " << file->fileName << ": " << state.preservedCount << " references kept their number, "
			<< state.assignedCount << " new numbers assigned, " << state.conflictCount << " duplicates fixed. Highest number: " << state.highest << "." << std::endl;

		savePersistedNumbers();
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

	// Replaces 0x5026EA. ebp = file.
	__declspec(naked) void PatchBeginSaveReferences() {
		__asm {
			push ebp
			call OnBeginSaveReferences
			add esp, 0x4
			push 0x6BD848
			mov eax, 0x5026F9
			jmp eax
		}
	}

	// Replaces 0x502812. esi = record handler.
	__declspec(naked) void PatchEndSaveReferences() {
		__asm {
			push dword ptr [esi + 0x4]
			call OnEndSaveReferences
			add esp, 0x4
			push 0x6BD7F0
			mov eax, 0x402BFD
			call eax
			mov eax, 0x50281C
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

	void installPatches() {
		if (!settings.reference_numbers.preserve) {
			return;
		}

		const bool valid = bytesMatch(0x53657A, { 0x33, 0xC0, 0x89, 0x46, 0x70, 0x89, 0x46, 0x6C, 0x50 })
			&& bytesMatch(0x53877D, { 0x8B, 0x44, 0x24, 0x10, 0x85, 0xC0, 0x75, 0x11 })
			&& bytesMatch(0x538785, { 0x8B, 0x86, 0xEC, 0x04, 0x00, 0x00, 0x40, 0x89, 0x86, 0xEC, 0x04, 0x00, 0x00, 0x89, 0x44, 0x24, 0x10 })
			&& bytesMatch(0x5026E8, { 0xEB, 0x0A })
			&& bytesMatch(0x5026EA, { 0xC7, 0x85, 0xEC, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x48, 0xD8, 0x6B, 0x00 })
			&& bytesMatch(0x502812, { 0x68, 0xF0, 0xD7, 0x6B, 0x00, 0xE8, 0xE1, 0x03, 0xF0, 0xFF });
		if (!valid) {
			log::stream << "[ReferenceNumbers] Unexpected code in the Construction Set executable. Patch not installed." << std::endl;
			return;
		}

		if (settings.reference_numbers.remember_highest) {
			loadPersistedNumbers();
		}

		using memory::genJumpUnprotected;
		using memory::writeByteUnprotected;

		genJumpUnprotected(0x53657A, reinterpret_cast<DWORD>(PatchLoadReference), 0x9);
		genJumpUnprotected(0x53877D, reinterpret_cast<DWORD>(PatchSaveReferenceNumber), 0x8);
		genJumpUnprotected(0x538785, reinterpret_cast<DWORD>(PatchSaveNewReferenceNumber), 0x11);

		// Route both branches before 0x5026EA through the reset hook.
		genJumpUnprotected(0x5026EA, reinterpret_cast<DWORD>(PatchBeginSaveReferences), 0xF);
		writeByteUnprotected(0x5026E9, 0x00);

		genJumpUnprotected(0x502812, reinterpret_cast<DWORD>(PatchEndSaveReferences), 0xA);
	}
}
