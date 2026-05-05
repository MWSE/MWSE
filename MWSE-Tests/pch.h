// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// Required for DWORD / BYTE / etc. types referenced by SharedSE/MemoryUtil.h
// (which the test project consumes transitively via SharedSE shim headers
// like NITArray.h). MWSE.dll and CSSE.dll get this for free from their own
// pch.h's heavier Windows includes; MWSE-Tests didn't need it until SharedSE
// shim adoption pulled MemoryUtil.h into its translation units.
#include <windows.h>

// SharedSE engine-address macros. SharedSE/MemoryUtil.h gates its template
// helpers (e.g. se::memory::_delete) behind these; without them the
// templates are #ifdef'd out and any consuming TArray<T>::operator delete
// fails to compile. Tests use the same Morrowind.exe addresses as MWSE.dll.
#define SE_MEMORY_FNADDR_NEW 0x727692
#define SE_MEMORY_FNADDR_DELETE 0x727530
#define SE_MEMORY_FNADDR_MALLOC 0x0
#define SE_MEMORY_FNADDR_FREE 0x0
#define SE_MEMORY_FNADDR_REALLOC 0x0

#include <list>

#include <algorithm>
#include <numeric>

#include <iostream>

// Core lua binding library.
#include <sol/sol.hpp>

#endif //PCH_H
