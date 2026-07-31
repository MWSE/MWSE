#include "MorrowindMemoryAllocator.h"

#include "MemoryUtil.h"

namespace {
	constexpr auto mimallocVersion = 30403;

	using FreeFunction = void(__cdecl*)(void*);
	using ReallocFunction = void* (__cdecl*)(void*, size_t);

	// Morrowind frees some MSVCP60-owned std::string buffers through its own delete/free path.
	FreeFunction originalFree = nullptr;
	ReallocFunction originalRealloc = nullptr;

	void* __cdecl morrowindNew(size_t size) {
		return mi_new_nothrow(size);
	}

	void* __cdecl morrowindMalloc(size_t size) {
		return mi_malloc(size);
	}

	void* __cdecl morrowindCalloc(size_t count, size_t size) {
		return mi_calloc(count, size);
	}

	void* __cdecl morrowindRealloc(void* address, size_t size) {
		if (address && !mi_is_in_heap_region(address)) {
			return originalRealloc(address, size);
		}
		return mi_realloc(address, size);
	}

	void __cdecl morrowindFree(void* address) {
		if (address && !mi_is_in_heap_region(address)) {
			originalFree(address);
		}
		else {
			mi_free(address);
		}
	}
}

bool mwse::memory::installMimalloc() {
	static_assert(MI_MALLOC_VERSION == mimallocVersion);

	if (mi_version() != mimallocVersion) {
		return false;
	}

	const auto msvcrt = GetModuleHandleA("msvcrt.dll");
	if (!msvcrt) {
		return false;
	}

	struct ImportReplacement {
		DWORD address;
		DWORD expected;
		DWORD replacement;
	};

	const auto expectedNew = GetProcAddress(msvcrt, "??2@YAPAXI@Z");
	const auto expectedFree = GetProcAddress(msvcrt, "free");
	const auto expectedMalloc = GetProcAddress(msvcrt, "malloc");
	const auto expectedRealloc = GetProcAddress(msvcrt, "realloc");
	const auto expectedCalloc = GetProcAddress(msvcrt, "calloc");
	if (!expectedNew || !expectedFree || !expectedMalloc || !expectedRealloc || !expectedCalloc) {
		return false;
	}

	const ImportReplacement replacements[] = {
		{ 0x746258, reinterpret_cast<DWORD>(expectedNew), reinterpret_cast<DWORD>(morrowindNew) },
		{ 0x746278, reinterpret_cast<DWORD>(expectedFree), reinterpret_cast<DWORD>(morrowindFree) },
		{ 0x746280, reinterpret_cast<DWORD>(expectedMalloc), reinterpret_cast<DWORD>(morrowindMalloc) },
		{ 0x746288, reinterpret_cast<DWORD>(expectedRealloc), reinterpret_cast<DWORD>(morrowindRealloc) },
		{ 0x7462A4, reinterpret_cast<DWORD>(expectedCalloc), reinterpret_cast<DWORD>(morrowindCalloc) },
	};

	for (const auto& replacement : replacements) {
		if (*reinterpret_cast<DWORD*>(replacement.address) != replacement.expected) {
			return false;
		}
	}

	originalFree = reinterpret_cast<FreeFunction>(expectedFree);
	originalRealloc = reinterpret_cast<ReallocFunction>(expectedRealloc);

	for (const auto& replacement : replacements) {
		if (!se::memory::writeDoubleWordEnforced(replacement.address, replacement.expected, replacement.replacement)) {
			return false;
		}
	}

	return true;
}
