#include "PatchReferenceNumbers.h"

#include "LogUtil.h"
#include "MemoryUtil.h"
#include "PathUtil.h"
#include "Settings.h"

#include "CSGameFile.h"
#include "CSReference.h"

//
// Patch: Keep reference numbers (FRMR) of the active plugin stable between saves.
//
// Morrowind identifies a reference from a plugin by (plugin, FRMR). Saved games store their changes to references
// (opened doors, moved/disabled/deleted objects, container contents, ...) under that identifier.
//
// The vanilla Construction Set does not keep the FRMR of references it loads from a non-master file. When such a
// reference is loaded, Reference::targetID is set to 0 (0x53657A). When the plugin is saved, the per-file counter
// GameFile::lastReferenceNumber is reset to 0 (0x5026EA) and every reference without a targetID simply gets the next
// number in the order the references are written (0x538785). Additionally, references of the plugin that are placed
// in cells from a master file always get a new number, even if they had one (0x53873D).
//
// As a result, adding or deleting a single reference shifts the FRMR of every reference that is written after it.
// Saved games then apply their stored changes to the wrong references: doors appear twice (open and closed), walls
// added by the plugin vanish, items show up in the wrong containers, etc.
//
// With this patch:
//  - References loaded from the active plugin keep their FRMR in targetID.
//  - When saving, references that belong to the saved plugin keep their FRMR.
//  - New references get numbers above the highest number ever used by the plugin. Numbers of deleted references are
//    never reused. The highest number is remembered between sessions in csse_reference_numbers.txt.
//  - The number given to a new reference is stored in it, so it stays the same on the next save as well.
//  - Should two references of the plugin ever end up with the same number, the second one gets a new number.
//
// References from master files are handled exactly as in vanilla.
//

namespace se::cs::patch::reference_numbers {
	constexpr DWORD ModMask = 0xFF000000;

	struct FileState {
		DWORD highest = 0; // Highest number known to be used by the file.
		DWORD persisted = 0; // Highest number written to the persistence file.
		std::unordered_set<DWORD> usedThisSave;
		size_t preservedCount = 0;
		size_t assignedCount = 0;
		size_t conflictCount = 0;
	};

	static std::unordered_map<std::string, FileState> fileStates;

	static std::string getFileKey(const GameFile* file) {
		std::string key = file->fileName;
		std::transform(key.begin(), key.end(), key.begin(), [](unsigned char c) { return (char)std::tolower(c); });
		return key;
	}

	static FileState& getState(const GameFile* file) {
		return fileStates[getFileKey(file)];
	}

	//
	// Persistence of the highest used number.
	// Format: one line per plugin, "<lowercase file name>\t<highest number>".
	//

	static std::filesystem::path getPersistencePath() {
		return path::getInstallPath() / "csse_reference_numbers.txt";
	}

	static void loadPersistedNumbers() {
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

	static void savePersistedNumbers() {
		if (!settings.reference_numbers.remember_highest) {
			return;
		}

		bool dirty = false;
		for (const auto& [key, state] : fileStates) {
			if (state.highest > state.persisted) {
				dirty = true;
				break;
			}
		}
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
	// Hook functions.
	//

	// Called when a new reference is created while loading a non-master file.
	void __cdecl OnLoadReference(Reference* reference, GameFile* file, DWORD formId) {
		// Vanilla behavior: no runtime form ID for references from non-master files.
		reference->sourceID = 0;
		reference->targetID = 0;

		// A plugin's own references are stored without a master index in the upper byte.
		if (formId == 0 || (formId & ModMask) != 0) {
			return;
		}

		reference->targetID = static_cast<int>(formId);

		auto& state = getState(file);
		state.highest = std::max(state.highest, formId);
	}

	static DWORD getNewReferenceNumber(GameFile* file, FileState& state) {
		DWORD number = std::max(file->lastReferenceNumber, state.highest);
		do {
			++number;
		} while (state.usedThisSave.contains(number));

		file->lastReferenceNumber = number;
		state.usedThisSave.insert(number);
		state.highest = std::max(state.highest, number); // Never hand out a number twice, even one used by a foreign reference.
		return number;
	}

	static bool isReferenceOwnedByFile(const Reference* reference, const GameFile* file) {
		if (reference->isFromMaster()) {
			return false;
		}
		return reference->sourceFile == file;
	}

	// Called when the FRMR of a reference is about to be written. Returns the FRMR to write.
	// vanillaWantsNew is true when vanilla would always hand out a new number at this point.
	DWORD __cdecl OnSaveReferenceNumber(Reference* reference, GameFile* file, DWORD current, bool vanillaWantsNew) {
		auto& state = getState(file);

		if (!isReferenceOwnedByFile(reference, file)) {
			// References from master files keep their vanilla number (master index + FRMR).
			// Anything else that isn't ours, e.g. references from other non-master plugins being merged into this one,
			// had no number in vanilla and got a new one. Keep it that way: their old number belongs to another plugin
			// and would collide with ours.
			if (vanillaWantsNew || current == 0 || !reference->isFromMaster()) {
				return getNewReferenceNumber(file, state);
			}
			return current;
		}

		// Our own reference with a valid number.
		if (current != 0 && (current & ModMask) == 0) {
			if (state.usedThisSave.insert(current).second) {
				state.highest = std::max(state.highest, current);
				state.preservedCount++;
				return current;
			}

			// The number was already written for another reference in this save. Don't write duplicates.
			state.conflictCount++;
			log::stream << "[ReferenceNumbers] Duplicate reference number " << current << " for '" << reference->getObjectID() << "'; assigning a new number." << std::endl;
		}

		// New reference: give it a fresh number and remember it, so the next save keeps it.
		const auto number = getNewReferenceNumber(file, state);
		reference->targetID = static_cast<int>(number);
		state.highest = std::max(state.highest, number);
		state.assignedCount++;
		return number;
	}

	// Called before the cells and references of a file are saved.
	void __cdecl OnBeginSaveReferences(GameFile* file) {
		auto& state = getState(file);
		state.usedThisSave.clear();
		state.preservedCount = 0;
		state.assignedCount = 0;
		state.conflictCount = 0;

		// Vanilla resets this to 0. Start above every number the file has ever used instead.
		file->lastReferenceNumber = state.highest;
	}

	// Called after the cells and references of a file are saved.
	void __cdecl OnEndSaveReferences(GameFile* file) {
		if (file == nullptr) {
			return;
		}

		const auto& state = getState(file);
		log::stream << "[ReferenceNumbers] Saved " << file->fileName << ": " << state.preservedCount << " references kept their number, "
			<< state.assignedCount << " new numbers assigned, " << state.conflictCount << " duplicates fixed. Highest number: " << state.highest << "." << std::endl;

		savePersistedNumbers();
	}

	//
	// Assembly stubs.
	//

	// Replaces 0x53657A-0x536583: xor eax, eax / mov [esi+70h], eax / mov [esi+6Ch], eax / push eax
	// esi = new reference, edi = file being loaded, [esp+18h] = FRMR read from the file.
	__declspec(naked) void PatchLoadReference() {
		__asm {
			mov eax, [esp + 0x18]
			push eax
			push edi
			push esi
			call OnLoadReference
			add esp, 0xC
			xor eax, eax
			push eax // Argument for the following Object::setFromMaster(false).
			mov ecx, 0x536583
			jmp ecx
		}
	}

	// Replaces 0x53877D-0x538785: mov eax, [esp+10h] / test eax, eax / jnz 0x538796
	// ebp = reference, esi = file being saved, [esp+10h] = FRMR vanilla wants to write.
	__declspec(naked) void PatchSaveReferenceNumber() {
		__asm {
			mov eax, [esp + 0x10]
			push 0 // vanillaWantsNew
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

	// Replaces 0x538785-0x538796: vanilla's "++file->lastReferenceNumber" path.
	__declspec(naked) void PatchSaveNewReferenceNumber() {
		__asm {
			mov eax, [esp + 0x10]
			push 1 // vanillaWantsNew
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

	// Replaces 0x5026EA-0x5026F9: mov [ebp+4ECh], 0 / push offset "...SAVE all Cells and Refs"
	// ebp = file being saved.
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

	// Replaces 0x502812-0x50281C: push offset "...SAVE sound gens" / call log, the step after cells and references.
	// Reached after every save, including when there is no player reference to save (that branch skips 0x5027C3).
	// esi = record handler, [esi+4] = active file.
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

	//
	// Installation.
	//

	static bool bytesMatch(DWORD address, std::initializer_list<BYTE> expected) {
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
			log::stream << "[ReferenceNumbers] Reference number preservation is disabled." << std::endl;
			return;
		}

		// Make sure the code we're replacing is what we expect. Either everything is patched or nothing is.
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

		// Keep the FRMR of references loaded from non-master files.
		genJumpUnprotected(0x53657A, reinterpret_cast<DWORD>(PatchLoadReference), 0x9);

		// Decide which FRMR to write when saving a reference.
		genJumpUnprotected(0x53877D, reinterpret_cast<DWORD>(PatchSaveReferenceNumber), 0x8);
		genJumpUnprotected(0x538785, reinterpret_cast<DWORD>(PatchSaveNewReferenceNumber), 0x11);

		// Don't restart numbering from 1 on every save. The "sort cell list" branch jumps past the reset; route it
		// through our hook too, so both branches start a save the same way.
		genJumpUnprotected(0x5026EA, reinterpret_cast<DWORD>(PatchBeginSaveReferences), 0xF);
		writeByteUnprotected(0x5026E9, 0x00);

		// Report and remember the highest number after saving.
		genJumpUnprotected(0x502812, reinterpret_cast<DWORD>(PatchEndSaveReferences), 0xA);

		log::stream << "[ReferenceNumbers] Reference number preservation installed." << std::endl;
	}
}
