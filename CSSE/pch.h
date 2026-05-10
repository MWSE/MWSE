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
#define SE_IS_CS TRUE
#define SE_TARGETS_CS TRUE
#define SE_MEMORY_FNADDR_NEW 0x6209F0
#define SE_MEMORY_FNADDR_DELETE 0x620948
#define SE_MEMORY_FNADDR_MALLOC 0x0
#define SE_MEMORY_FNADDR_FREE 0x0
#define SE_MEMORY_FNADDR_REALLOC 0x0

#endif //PCH_H
