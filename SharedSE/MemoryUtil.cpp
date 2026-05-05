#include "MemoryUtil.h"

namespace se::memory {
#if defined(SE_MEMORY_FNADDR_REALLOC) && SE_MEMORY_FNADDR_REALLOC > 0
	void* realloc(void* address, size_t size) {
		const auto TES3_operator_realloc = reinterpret_cast<void* (__cdecl**)(void*, size_t)>(SE_MEMORY_FNADDR_REALLOC);
		return (*TES3_operator_realloc)(address, size);
	}
#endif

#if defined(SE_MEMORY_FNADDR_MALLOC) && SE_MEMORY_FNADDR_MALLOC > 0
	void* malloc(size_t size) {
		const auto TES3_operator_malloc = reinterpret_cast<void* (__cdecl*)(size_t)>(SE_MEMORY_FNADDR_MALLOC);
		return TES3_operator_malloc(size);
	}
#endif

#if defined(SE_MEMORY_FNADDR_FREE) && SE_MEMORY_FNADDR_FREE > 0
	void free(void* address) {
		const auto TES3_operator_free = reinterpret_cast<void(__cdecl*)(void*)>(SE_MEMORY_FNADDR_FREE);
		TES3_operator_free(address);
	}
#endif

	void genJump(DWORD address, DWORD to) {
		MemAccess<unsigned char>::Set(address, 0xE9);
		MemAccess<DWORD>::Set(address + 1, to - address - 0x5);
	}

	void genCall(DWORD address, DWORD to) {
		MemAccess<unsigned char>::Set(address, 0xE8);
		MemAccess<DWORD>::Set(address + 1, to - address - 0x5);
	}

	void genNOP(DWORD address) {
		MemAccess<unsigned char>::Set(address, 0x90);
	}

	void genJumpUnprotected(DWORD address, DWORD to, DWORD size) {
		// Unprotect memory.
		DWORD oldProtect;
		VirtualProtect((DWORD*)address, size, PAGE_READWRITE, &oldProtect);

		// Create our jump.
		MemAccess<unsigned char>::Set(address, 0xE9);
		MemAccess<DWORD>::Set(address + 1, to - address - 0x5);

		// NOP out the rest of the block.
		for (DWORD i = address + 5; i < address + size; ++i) {
			genNOP(i);
		}

		// Protect memory again.
		VirtualProtect((DWORD*)address, size, oldProtect, &oldProtect);
	}

	bool genJumpEnforced(DWORD address, DWORD previousTo, DWORD to, DWORD size) {
		// Make sure we're doing a jump.
		BYTE instruction = *reinterpret_cast<BYTE*>(address);
		if (instruction != 0xE9) {
			return false;
		}

		// Read previous jump address to make sure it's what we are expecting.
		DWORD currentCallAddress = *reinterpret_cast<DWORD*>(address + 1) + address + 0x5;
		if (currentCallAddress != previousTo) {
			return false;
		}

		// Unprotect memory.
		DWORD oldProtect;
		VirtualProtect((DWORD*)address, size, PAGE_READWRITE, &oldProtect);

		// Create our jump.
		MemAccess<unsigned char>::Set(address, 0xE9);
		MemAccess<DWORD>::Set(address + 1, to - address - 0x5);

		// NOP out the rest of the block.
		for (DWORD i = address + 5; i < address + size; ++i) {
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
		for (DWORD i = address + 5; i < address + size; ++i) {
			genNOP(i);
		}

		// Protect memory again.
		VirtualProtect((DWORD*)address, size, oldProtect, &oldProtect);
	}

	bool genCallEnforced(DWORD address, DWORD previousTo, DWORD to, DWORD size) {
		// Make sure we're doing a call.
		BYTE instruction = *reinterpret_cast<BYTE*>(address);
		if (instruction != 0xE8) {
			return false;
		}

		// Read previous call address to make sure it's what we are expecting.
		DWORD currentCallAddress = *reinterpret_cast<DWORD*>(address + 1) + address + 0x5;
		if (currentCallAddress != previousTo) {
			return false;
		}

		// Unprotect memory.
		DWORD oldProtect;
		VirtualProtect((DWORD*)address, size, PAGE_READWRITE, &oldProtect);

		// Create our call.
		MemAccess<unsigned char>::Set(address, 0xE8);
		MemAccess<DWORD>::Set(address + 1, to - address - 0x5);

		// NOP out the rest of the block.
		for (DWORD i = address + 5; i < address + size; ++i) {
			genNOP(i);
		}

		// Protect memory again.
		VirtualProtect((DWORD*)address, size, oldProtect, &oldProtect);

		return true;
	}

	bool genPushEnforced(DWORD address, BYTE value) {
		// Make sure we're doing a push already.
		BYTE instruction = *reinterpret_cast<BYTE*>(address);
		if (instruction != 0x6A) {
			return false;
		}

		// Unprotect memory.
		DWORD oldProtect;
		VirtualProtect((DWORD*)address, 0x2, PAGE_READWRITE, &oldProtect);

		// Create our call.
		MemAccess<BYTE>::Set(address, 0x6A);
		MemAccess<BYTE>::Set(address + 1, value);

		// Protect memory again.
		VirtualProtect((DWORD*)address, 0x2, oldProtect, &oldProtect);

		return true;
	}

	bool genPushEnforced(DWORD address, DWORD value) {
		// Make sure we're doing a push already.
		BYTE instruction = *reinterpret_cast<BYTE*>(address);
		if (instruction != 0x68) {
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

	bool genPushUnprotected(DWORD address, DWORD value) {
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

	bool genNOPUnprotected(DWORD address, DWORD size) {
		// Unprotect memory.
		DWORD oldProtect;
		VirtualProtect((DWORD*)address, size, PAGE_READWRITE, &oldProtect);

		for (DWORD i = 0; i < size; ++i) {
			genNOP(address + i);
		}

		// Protect memory again.
		VirtualProtect((DWORD*)address, size, oldProtect, &oldProtect);
		return true;
	}

	void writeByteUnprotected(DWORD address, BYTE value) {
		// Unprotect memory.
		DWORD oldProtect;
		VirtualProtect((DWORD*)address, sizeof(BYTE), PAGE_READWRITE, &oldProtect);

		// Overwrite our single byte.
		MemAccess<BYTE>::Set(address, value);

		// Protect memory again.
		VirtualProtect((DWORD*)address, sizeof(BYTE), oldProtect, &oldProtect);
	}

	void writeBytesUnprotected(DWORD address, const BYTE* value, size_t count) {
		DWORD oldProtect;
		VirtualProtect((DWORD*)address, count, PAGE_READWRITE, &oldProtect);
		memmove_s((void*)address, count, value, count);
		VirtualProtect((DWORD*)address, count, oldProtect, &oldProtect);
	}

	void writeDoubleWordUnprotected(DWORD address, DWORD value) {
		// Unprotect memory.
		DWORD oldProtect;
		VirtualProtect((DWORD*)address, sizeof(DWORD), PAGE_READWRITE, &oldProtect);

		// Overwrite our single byte.
		MemAccess<DWORD>::Set(address, value);

		// Protect memory again.
		VirtualProtect((DWORD*)address, sizeof(DWORD), oldProtect, &oldProtect);
	}

	bool writeDoubleWordEnforced(DWORD address, DWORD previousValue, DWORD value) {
		DWORD currentValue = *reinterpret_cast<DWORD*>(address);
		if (currentValue != previousValue) {
			return false;
		}

		// Unprotect memory.
		DWORD oldProtect;
		VirtualProtect((DWORD*)address, sizeof(DWORD), PAGE_READWRITE, &oldProtect);

		// Overwrite our single byte.
		MemAccess<DWORD>::Set(address, value);

		// Protect memory again.
		VirtualProtect((DWORD*)address, sizeof(DWORD), oldProtect, &oldProtect);

		return true;
	}

	// WARNING: If passing a function address, always use a non-static function or it will crash.
	void writePatchCodeUnprotected(DWORD address, const BYTE* patch, DWORD size) {
#ifdef _DEBUG
		// Read incremental linker trampoline to find real patch
		patch += 5 + *reinterpret_cast<const ptrdiff_t*>(patch + 1);
#endif
		writeBytesUnprotected(address, patch, size);
	}

	DWORD getCallAddress(DWORD address) {
		// Make sure we're doing a call.
		BYTE instruction = *reinterpret_cast<BYTE*>(address);
		if (instruction != 0xE8) {
			return NULL;
		}

		// Read previous call address to make sure it's what we are expecting.
		return *reinterpret_cast<DWORD*>(address + 1) + address + 0x5;
	}
}
