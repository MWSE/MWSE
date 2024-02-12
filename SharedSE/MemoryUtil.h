#pragma once

namespace se::memory {
	//
	// Memory (de)allocation functions.
	//
	// Morrowind is compiled as a release build, and so typical new/delete/malloc/realloc/free
	// calls will not pass debug checks. Anything that comes from or is passed to Morrowind that
	// might be later freed needs to use these functions.
	//

#if defined(SE_MEMORY_FNADDR_NEW) && SE_MEMORY_FNADDR_NEW > 0
	template <typename T>
	inline constexpr T* _new(size_t count = 1) {
		return reinterpret_cast<T * (__cdecl*)(size_t)>(SE_MEMORY_FNADDR_NEW)(sizeof(T) * count);
	}

	void* _new(size_t size);
#endif

#if defined(SE_MEMORY_FNADDR_DELETE) && SE_MEMORY_FNADDR_DELETE > 0
	template <typename T>
	inline constexpr void _delete(T* address) {
		const auto __delete = reinterpret_cast<void(__cdecl*)(T*)>(SE_MEMORY_FNADDR_DELETE);
		__delete(address);
	}
#endif

#if defined(SE_MEMORY_FNADDR_REALLOC) && SE_MEMORY_FNADDR_REALLOC > 0
	void* realloc(void* address, size_t size);
#endif

#if defined(SE_MEMORY_FNADDR_MALLOC) && SE_MEMORY_FNADDR_MALLOC > 0
	void* malloc(size_t size);

	template <typename T>
	inline constexpr T* malloc(size_t count = 1) {
		T* ret = reinterpret_cast<T*>(malloc(sizeof(T) * count));
		memset(ret, 0, sizeof(T));
		return ret;
	}
#endif

#if defined(SE_MEMORY_FNADDR_FREE) && SE_MEMORY_FNADDR_FREE > 0
	void free(void* address);
#endif

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

	/// <summary>
	/// General helper function that serves as a typed wrapper around reinterpret_cast. It supports simple getting and setting.
	/// </summary>
	/// <typeparam name="T">The type to get/set.</typeparam>
	template <typename T>
	class MemAccess {
	public:
		MemAccess() = delete;
		~MemAccess() = delete;

		static inline T& Get(uintptr_t address) {
			return *reinterpret_cast<T*>(address);
		}

		static inline void Set(uintptr_t address, T value) {
			*reinterpret_cast<T*>(address) = value;
		}
	};

	/// <summary>
	/// As MemAccess, but with a constant address. Useful to define globals in the base executable that will never change location.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <typeparam name="address"></typeparam>
	template <typename T, DWORD address>
	class ExternalGlobal {
	public:
		ExternalGlobal() = delete;
		~ExternalGlobal() = delete;

		static inline T& get() {
			return *reinterpret_cast<T*>(address);
		}

		static inline void set(T value) {
			*reinterpret_cast<T*>(address) = value;
		}
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

	template <typename T>
	bool genJumpEnforced(DWORD address, DWORD previousTo, T to, DWORD size = 0x5) {
		return genJumpEnforced(address, previousTo, *reinterpret_cast<DWORD*>(&to), size);
	}

	// Code to generate a call in memory. This function unprotects the memory.
	void genCallUnprotected(DWORD address, DWORD to, DWORD size = 0x5);

	// Code to generate a call in memory. This function unprotects the memory.
	bool genCallEnforced(DWORD address, DWORD previousTo, DWORD to, DWORD size = 0x5);

	bool genPushEnforced(DWORD address, BYTE value);
	bool genPushEnforced(DWORD address, DWORD value);

	bool genPushUnprotected(DWORD address, DWORD value);

	// Overrides a function in a virtual table.
	void overrideVirtualTable(DWORD address, DWORD offset, DWORD to);

	// Overrides a function in a virtual table.
	bool overrideVirtualTableEnforced(DWORD address, DWORD offset, DWORD previousTo, DWORD to);

	// Write NOP instructions over a space.
	bool genNOPUnprotected(DWORD address, DWORD size);

	// Write a single value to memory.
	void writeByteUnprotected(DWORD address, BYTE value);
	void writeBytesUnprotected(DWORD address, const BYTE* value, size_t count);
	void writeDoubleWordUnprotected(DWORD address, DWORD value);
	bool writeDoubleWordEnforced(DWORD address, DWORD previousValue, DWORD value);

	template <typename T>
	bool writeValueEnforced(DWORD address, T previousValue, T value) {
		T currentValue = *reinterpret_cast<T*>(address);
		if (currentValue != previousValue) {
			return false;
		}

		// Unprotect memory.
		DWORD oldProtect;
		VirtualProtect((DWORD*)address, sizeof(T), PAGE_READWRITE, &oldProtect);

		// Overwrite our single byte.
		MemAccess<T>::Set(address, value);

		// Protect memory again.
		VirtualProtect((DWORD*)address, sizeof(T), oldProtect, &oldProtect);

		return true;
	}

	// Code to write a patch to a code segment. This function unprotects the memory.
	// WARNING: If passing a function address, always use a non-static function or it will crash.
	void writePatchCodeUnprotected(DWORD address, const BYTE* patch, DWORD size);

	// Code to determine what function an address calls.
	DWORD getCallAddress(DWORD address);

	// Map from a pointer to a child element to the main structure.
	template <typename T>
	T* getMainStructureFromOffset(void* childPtr, DWORD offset) {
		return reinterpret_cast<T*>(reinterpret_cast<BYTE*>(childPtr) - offset);
	}
}
