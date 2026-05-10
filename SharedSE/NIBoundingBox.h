#pragma once

#include "NIPoint3.h"

namespace NI {
	struct BoundingBox {
		Point3 minimum; // 0x0
		Point3 maximum; // 0xC

		BoundingBox();
		BoundingBox(const Point3& min, const Point3& max);
		BoundingBox(const BoundingBox& bbox);
		BoundingBox(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);

		bool operator==(const BoundingBox& other) const;
		bool operator!=(const BoundingBox& other) const;

		friend std::ostream& operator<<(std::ostream& str, const BoundingBox& other);
		std::string toString() const;
		std::string toJson() const;

		BoundingBox copy() const;
		std::array<Point3, 8> vertices() const;

		void initialize();
		void invalidate();
		bool hasUninitializedData() const;
		void clampPoint(Point3& point, const Point3& origin) const;
	};
	static_assert(sizeof(BoundingBox) == 0x18, "NI::BoundingBox failed size validation");
}
