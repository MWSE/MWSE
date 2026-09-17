#pragma once

#if defined(SE_TARGETS_MW) && SE_TARGETS_MW == 1
#include "Config.Morrowind.h"
#elif defined(SE_TARGETS_CS) && SE_TARGETS_CS == 1
#include "Config.TESConstructionSet.h"
#else
static_assert(false, Invalid target "Invalid target scope. Define an SE_TARGETS_ macro.");
#endif