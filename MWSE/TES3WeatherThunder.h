#pragma once

#include "TES3Weather.h"

namespace TES3 {
	struct WeatherThunder : Weather {
		static constexpr auto THUNDER_SOUND_COUNT = 4;

		char thunderSoundIds[THUNDER_SOUND_COUNT][MAX_PATH]; // 0x318
		char soundIDRainLoop[MAX_PATH]; // 0x728
		Sound* thunderSounds[THUNDER_SOUND_COUNT]; // 0x82C
		int thunderSoundCount; // 0x83C
		Sound* soundRainLoop; // 0x840
		float thunderFrequency; // 0x844
		float thunderThreshold; // 0x848
		float rainRadius; // 0x84C
		float rainHeightMin; // 0x850
		float rainHeightMax; // 0x854
		float rainThreshold; // 0x858
		float rainEntranceSpeed; // 0x85C
		float raindropsMax; // 0x860
		float flashDecrement; // 0x864
		bool rainPlaying; // 0x868

		WeatherThunder() = delete;
		~WeatherThunder() = delete;

		//
		// Custom functions.
		//

		void simulate(float transitionScalar, float deltaTime);
		void transition();
		void unload();

		void loadThunderSounds();

		const char* getThunderSoundId(size_t index) const;
		bool setThunderSoundId(size_t index, const char* path);
		Sound* getLoadedThunderSound(size_t index) const;

		template <size_t index>
		const char* getThunderSoundId() const {
			return getThunderSoundId(index);
		}

		template <size_t index>
		bool setThunderSoundId(const char* path) {
			return setThunderSoundId(index, path);
		}

		Sound* getThunderSound(size_t index) const;
		void setThunderSound(size_t index, Sound* sound);

		template <size_t index>
		Sound* getThunderSound() const {
			return getThunderSound(index);
		}

		template <size_t index>
		void setThunderSound(Sound* sound) {
			setThunderSound(index, sound);
		}

		bool setRainLoopSoundID(const char* id);

	};
	static_assert(sizeof(WeatherThunder) == 0x86C, "TES3::WeatherThunder failed size validation");
}
