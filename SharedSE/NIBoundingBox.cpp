#include "NIBoundingBox.h"

namespace NI {
	void BoundingBox::invalidate() {
		constexpr auto float_min = std::numeric_limits<float>::lowest();
		constexpr auto float_max = std::numeric_limits<float>::max();
		static_assert(float_min == -float_max);

		min = { float_max, float_max, float_max };
		max = { float_min, float_min, float_min };
	}
}
