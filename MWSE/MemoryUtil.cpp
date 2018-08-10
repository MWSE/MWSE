#include <string.h>
#include "MemAccess.h"
#include "MemoryUtil.h"
#include "Log.h"

namespace mwse {
	void genJump(DWORD Address, DWORD To) {
		MemAccess<unsigned char>::Set(Address, 0xE9);
		MemAccess<DWORD>::Set(Address + 1, To - Address - 0x5);
	}

	void genCall(DWORD Address, DWORD To) {
		MemAccess<unsigned char>::Set(Address, 0xE8);
		MemAccess<DWORD>::Set(Address + 1, To - Address - 0x5);
	}

	void genNOP(DWORD Address) {
		MemAccess<unsigned char>::Set(Address, 0x90);
	}

	void genJumpUnprotected(DWORD address, DWORD to, DWORD size) {
		// Unprotect memory.
		DWORD oldProtect;
		VirtualProtect((DWORD*)address, size, PAGE_READWRITE, &oldProtect);

		// Create our jump.
		MemAccess<unsigned char>::Set(address, 0xE9);
		MemAccess<DWORD>::Set(address + 1, to - address - 0x5);

		// NOP out the rest of the block.
		for (DWORD i = address + 5; i < address + size; i++) {
			genNOP(i);
		}

		// Protect memory again.
		VirtualProtect((DWORD*)address, size, oldProtect, &oldProtect);
	}

	bool genJumpEnforced(DWORD address, DWORD previousTo, DWORD to, DWORD size) {
		// Make sure we're doing a jump.
		BYTE instruction = *reinterpret_cast<BYTE*>(address);
		if (instruction != 0xE9) {
#if _DEBUG
			log::getLog() << "[MemoryUtil] Skipping jump generation at 0x" << std::hex << address << ". Expected 0xe9, found instruction: 0x" << (int)instruction << "." << std::endl;
#endif
			return false;
		}

		// Read previous jump address to make sure it's what we are expecting.
		DWORD currentCallAddress = *reinterpret_cast<DWORD*>(address + 1) + address + 0x5;
		if (currentCallAddress != previousTo) {
#if _DEBUG
			log::getLog() << "[MemoryUtil] Skipping jump generation at 0x" << std::hex << address << ". Expected previous jump to 0x" << previousTo << ", found 0x" << currentCallAddress << "." << std::endl;
#endif
			return false;
		}

		// Unprotect memory.
		DWORD oldProtect;
		VirtualProtect((DWORD*)address, size, PAGE_READWRITE, &oldProtect);

		// Create our jump.
		MemAccess<unsigned char>::Set(address, 0xE9);
		MemAccess<DWORD>::Set(address + 1, to - address - 0x5);

		// NOP out the rest of the block.
		for (DWORD i = address + 5; i < address + size; i++) {
			genNOP(i);
		}

		// Protect memory again.
		VirtualProtect((DWORD*)address, size, oldProtect, &oldProtect);

		return true;
	}

	void genCallUnprotected(DWORD address, DWORD to, DWORD size) {
		// Unprotect memory.
		DWORD oldProtect;
		VirtualProtect((DWORD*)address, size, PAGE_READWRITE, &oldProtect);

		// Create our call.
		MemAccess<unsigned char>::Set(address, 0xE8);
		MemAccess<DWORD>::Set(address + 1, to - address - 0x5);

		// NOP out the rest of the block.
		for (DWORD i = address + 5; i < address + size; i++) {
			genNOP(i);
		}

		// Protect memory again.
		VirtualProtect((DWORD*)address, size, oldProtect, &oldProtect);
	}

	bool genCallEnforced(DWORD address, DWORD previousTo, DWORD to, DWORD size) {
		// Make sure we're doing a call.
		BYTE instruction = *reinterpret_cast<BYTE*>(address);
		if (instruction != 0xE8) {
#if _DEBUG
			log::getLog() << "[MemoryUtil] Skipping call generation at 0x" << std::hex << address << ". Expected 0xe8, found instruction: 0x" << (int)instruction << "." << std::endl;
#endif
			return false;
		}

		// Read previous call address to make sure it's what we are expecting.
		DWORD currentCallAddress = *reinterpret_cast<DWORD*>(address + 1) + address + 0x5;
		if (currentCallAddress != previousTo) {
#if _DEBUG
			log::getLog() << "[MemoryUtil] Skipping call generation at 0x" << std::hex << address << ". Expected previous call to 0x" << previousTo << ", found 0x" << currentCallAddress << "." << std::endl;
#endif
			return false;
		}

		// Unprotect memory.
		DWORD oldProtect;
		VirtualProtect((DWORD*)address, size, PAGE_READWRITE, &oldProtect);

		// Create our call.
		MemAccess<unsigned char>::Set(address, 0xE8);
		MemAccess<DWORD>::Set(address + 1, to - address - 0x5);

		// NOP out the rest of the block.
		for (DWORD i = address + 5; i < address + size; i++) {
			genNOP(i);
		}

		// Protect memory again.
		VirtualProtect((DWORD*)address, size, oldProtect, &oldProtect);

		return true;
	}

	bool genPushEnforced(DWORD address, DWORD value) {
		// Make sure we're doing a push already.
		BYTE instruction = *reinterpret_cast<BYTE*>(address);
		if (instruction != 0x68) {
#if _DEBUG
			log::getLog() << "[MemoryUtil] Skipping call generation at 0x" << std::hex << address << ". Expected 0x68, found instruction: 0x" << (int)instruction << "." << std::endl;
#endif
			return false;
		}

		// Unprotect memory.
		DWORD oldProtect;
		VirtualProtect((DWORD*)address, 0x5, PAGE_READWRITE, &oldProtect);

		// Create our call.
		MemAccess<unsigned char>::Set(address, 0x68);
		MemAccess<DWORD>::Set(address + 1, value);

		// Protect memory again.
		VirtualProtect((DWORD*)address, 0x5, oldProtect, &oldProtect);

		return true;
	}

	void overrideVirtualTable(DWORD address, DWORD offset, DWORD to) {
		DWORD location = address + offset;

		// Unprotect memory.
		DWORD oldProtect;
		VirtualProtect((DWORD*)location, 0x4, PAGE_READWRITE, &oldProtect);

		// Create our call.
		MemAccess<DWORD>::Set(location, to);

		// Protect memory again.
		VirtualProtect((DWORD*)location, 0x4, oldProtect, &oldProtect);
	}

	bool overrideVirtualTableEnforced(DWORD address, DWORD offset, DWORD previousTo, DWORD to) {
		DWORD location = address + offset;

		// Read previous call address to make sure it's what we are expecting.
		DWORD currentCallAddress = *reinterpret_cast<DWORD*>(location);
		if (currentCallAddress != previousTo) {
#if _DEBUG
			log::getLog() << "[MemoryUtil] Skipping virtual table overwrite at 0x" << std::hex << address << "+0x" << offset << ". Expected previous call to 0x" << previousTo << ", found 0x" << currentCallAddress << "." << std::endl;
#endif
			return false;
		}

		// Unprotect memory.
		DWORD oldProtect;
		VirtualProtect((DWORD*)location, 0x4, PAGE_READWRITE, &oldProtect);

		// Create our call.
		MemAccess<DWORD>::Set(location, to);
		
		// Protect memory again.
		VirtualProtect((DWORD*)location, 0x4, oldProtect, &oldProtect);

		return true;
	}

	void writeByteUnprotected(DWORD address, BYTE value) {
		// Unprotect memory.
		DWORD oldProtect;
		VirtualProtect((DWORD*)address, 0x5, PAGE_READWRITE, &oldProtect);

		// Overwrite our single byte.
		MemAccess<unsigned char>::Set(address, value);

		// Protect memory again.
		VirtualProtect((DWORD*)address, 0x5, oldProtect, &oldProtect);
	}

	// WARNING: If passing a function address, always use a non-static function or it will crash.
	void writePatchCodeUnprotected(DWORD address, const BYTE* patch, DWORD size) {
#ifdef _DEBUG
		// Read incremental linker trampoline to find real patch
		patch += 5 + *reinterpret_cast<const ptrdiff_t*>(patch + 1);
#endif
		DWORD oldProtect;
		VirtualProtect((DWORD*)address, size, PAGE_READWRITE, &oldProtect);

		memmove_s((void*)address, size, patch, size);

		VirtualProtect((DWORD*)address, size, oldProtect, &oldProtect);
	}

}