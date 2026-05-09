#include "NIBoundingBox.h"

#include "NIVector3.h"

namespace NI {
	BoundingBox::BoundingBox() :
		minimum(),
		maximum()
	{

	}

	BoundingBox::BoundingBox(const Vector3& min, const Vector3& max) :
		minimum(min),
		maximum(max)
	{

	}

	BoundingBox::BoundingBox(const BoundingBox& bbox) :
		minimum(bbox.minimum),
		maximum(bbox.maximum)
	{

	}

	BoundingBox::BoundingBox(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) :
		minimum(minX, minY, minZ),
		maximum(maxX, maxY, maxZ)
	{

	}

	bool BoundingBox::operator==(const BoundingBox& other) const {
		return minimum == other.minimum && maximum == other.maximum;
	}

	bool BoundingBox::operator!=(const BoundingBox& other) const {
		return minimum != other.minimum || maximum != other.maximum;
	}

	std::ostream& operator<<(std::ostream& str, const BoundingBox& other) {
		str << "(" << other.minimum << "," << other.maximum << ")";
		return str;
	}

	std::string BoundingBox::toString() const {
		std::ostringstream ss;
		ss << std::fixed << std::setprecision(2) << std::dec << *this;
		return std::move(ss.str());
	}

	std::string BoundingBox::toJson() const {
		std::ostringstream ss;
		ss << "{"
			<< "\"min\":" << minimum.toJson() << ","
			<< "\"max\":" << maximum.toJson()
			<< "}";
		return std::move(ss.str());
	}

	BoundingBox BoundingBox::copy() const {
		return *this;
	}

	std::array<Vector3, 8> BoundingBox::vertices() const {
		return std::array{
			Vector3(minimum.x, minimum.y, minimum.z),
			Vector3(minimum.x, minimum.y, maximum.z),
			Vector3(minimum.x, maximum.y, minimum.z),
			Vector3(minimum.x, maximum.y, maximum.z),
			Vector3(maximum.x, maximum.y, minimum.z),
			Vector3(maximum.x, minimum.y, maximum.z),
			Vector3(maximum.x, minimum.y, minimum.z),
			Vector3(maximum.x, maximum.y, maximum.z),
		};
	}

	void BoundingBox::clampPoint(Vector3& point, const Vector3& origin) const {
		const auto min = minimum + origin;
		const auto max = maximum + origin;
		//const auto min = minimum;
		//const auto max = maximum;
		point.x = std::clamp(point.x, min.x, max.x);
		point.y = std::clamp(point.y, min.y, max.y);
		point.z = std::clamp(point.z, min.z, max.z);
	}

	void BoundingBox::initialize() {
		minimum = Vector3::MAX;
		maximum = Vector3::MIN;
	}

	void BoundingBox::invalidate() {
		constexpr auto float_min = std::numeric_limits<float>::lowest();
		constexpr auto float_max = std::numeric_limits<float>::max();
		static_assert(float_min == -float_max);

		minimum = { float_max, float_max, float_max };
		maximum = { float_min, float_min, float_min };
	}

	bool BoundingBox::hasUninitializedData() const {
		return minimum.x == Vector3::MAX.x || minimum.y == Vector3::MAX.y || minimum.z == Vector3::MAX.z
			|| maximum.x == Vector3::MIN.x || maximum.y == Vector3::MIN.y || maximum.z == Vector3::MIN.z;
	}
}
