#pragma once

namespace TES3 {
	namespace CellFlag {
		typedef unsigned int value_type;

		enum Flag : value_type {
			Interior = 0x1,
			HasWater = 0x2,
			SleepIsIllegal = 0x4,
			WasLoaded = 0x8,
			TempRefsLoaded = 0x10,
			MarkerDrawn = 0x20,
			BehavesAsExterior = 0x80
		};
	}
}
