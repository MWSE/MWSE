// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#define DIRECT3D_VERSION 0x0800
#define DIRECTINPUT_VERSION 0x0800

// add headers that you want to pre-compile here
#include "framework.h"
#include "resource.h"

// SharedSE defines.
#define SE_IS_CS 1
#define SE_TARGETS_CS 1
// CS allocator surface mirrors MW: every NEW/DELETE/MALLOC/FREE call routes
// through the same MSVCRT heap, so cross-allocation between SharedSE and the
// CS engine is heap-paired. The MW addresses (stdafx.h:85-89) sit in front
// of the same MSVCRT imports — only the thunk addresses differ.
#define SE_MEMORY_FNADDR_NEW 0x6209F0       // operator new(size_t) thunk -> __imp_??2@YAPAXI@Z
#define SE_MEMORY_FNADDR_DELETE 0x620948    // delete-wrapper -> CRT free()
#define SE_MEMORY_FNADDR_MALLOC 0x620B5C    // malloc thunk -> __imp_malloc
#define SE_MEMORY_FNADDR_FREE 0x620B26      // free thunk   -> __imp_free
#define SE_MEMORY_FNADDR_REALLOC 0x0        // CS does not import realloc; consumers must gate

#endif //PCH_H
