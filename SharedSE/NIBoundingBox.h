#pragma once

#include "NIVector3.h"

namespace NI {
	struct BoundingBox {
		Vector3 minimum; // 0x0
		Vector3 maximum; // 0xC

		BoundingBox();
		BoundingBox(const Vector3& min, const Vector3& max);
		BoundingBox(const BoundingBox& bbox);
		BoundingBox(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);

		bool operator==(const BoundingBox& other) const;
		bool operator!=(const BoundingBox& other) const;

		friend std::ostream& operator<<(std::ostream& str, const BoundingBox& other);
		std::string toString() const;
		std::string toJson() const;

		BoundingBox copy() const;
		std::array<Vector3, 8> vertices() const;

		void initialize();
		void invalidate();
		bool hasUninitializedData() const;
		void clampPoint(Vector3& point, const Vector3& origin) const;
	};
	static_assert(sizeof(BoundingBox) == 0x18, "NI::BoundingBox failed size validation");
}
