#include "RngUtil.h"

namespace mwse
{
	namespace rng
	{
		static std::mt19937 mwse_rng_mt19937(std::time(0));

		long getRandomLong(long min, long max) {
			if (min > max) {
				return 0;
			}
			else if (min == max) {
				return min;
			}

			std::uniform_int_distribution<long> dist(min, max);
			return dist(mwse_rng_mt19937);
		}

		float getRandomFloat(float min, float max) {
			if (min > max) {
				return 0.0f;
			}
			else if (min == max) {
				return min;
			}

			std::uniform_real_distribution<float> dist(min, max);
			return dist(mwse_rng_mt19937);
		}
	}
}
