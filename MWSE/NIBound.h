#pragma once

#include "TES3Vectors.h"

namespace NI {
	struct Bound {
		TES3::Vector3 center; // 0x0
		float radius; // 0xC

		bool contains(const TES3::Vector3& point) const;
		TES3::Vector3 getClosetPointTo(const TES3::Vector3& point) const;
		TES3::Vector3 getFurthestPointFrom(const TES3::Vector3& point) const;
	};
	static_assert(sizeof(Bound) == 0x10, "NI::Bound failed size validation");
}
