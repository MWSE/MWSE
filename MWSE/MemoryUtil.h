#pragma once

#include <Windows.h>

namespace mwse {
	// Container for registers, flags, and other information to help with the
	// native to MWSE code bridge.
	struct HookContext {
		// General registers.
		DWORD eax;
		DWORD ebx;
		DWORD ecx;
		DWORD edx;

		// String operators.
		DWORD esi;
		DWORD edi;

		// Stack.
		DWORD ebp;
		DWORD esp;

		// Flags (cmp, test).
		DWORD flags;

		// Address to return to.
		DWORD callbackAddress;
	};

	// Code to generate a jump in memory. Don't forget to unprotect it first!
	void genJump(DWORD Address, DWORD To);

	// Code to generate a call in memory. Don't forget to unprotect it first!
	void genCall(DWORD Address, DWORD To);

	// Code to generate a nop in memory. Don't forget to unprotect it first!
	void genNOP(DWORD Address);

	// Code to generate a call in memory. This function unprotects the memory.
	void genJumpUnprotected(DWORD address, DWORD to, DWORD size = 0x5);

	// Code to generate a call in memory. This function unprotects the memory.
	bool genJumpEnforced(DWORD address, DWORD previousTo, DWORD to, DWORD size = 0x5);

	// Code to generate a call in memory. This function unprotects the memory.
	void genCallUnprotected(DWORD address, DWORD to, DWORD size = 0x5);

	// Code to generate a call in memory. This function unprotects the memory.
	bool genCallEnforced(DWORD address, DWORD previousTo, DWORD to, DWORD size = 0x5);

	bool genPushEnforced(DWORD address, DWORD value);

	// Overrides a function in a virtual table.
	void overrideVirtualTable(DWORD address, DWORD offset, DWORD to);

	// Overrides a function in a virtual table.
	bool overrideVirtualTableEnforced(DWORD address, DWORD offset, DWORD previousTo, DWORD to);

	// Write a single byte to memory.
	void writeByteUnprotected(DWORD address, BYTE value);

	// Code to write a patch to a code segment. This function unprotects the memory.
	// WARNING: If passing a function address, always use a non-static function or it will crash.
	void writePatchCodeUnprotected(DWORD address, const BYTE* patch, DWORD size);
}
