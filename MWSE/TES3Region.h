#pragma once

#include "TES3Defines.h"

#include "TES3Object.h"

namespace TES3 {
	struct RegionSound {
		Sound * sound; // 0x0
		unsigned char chance; // 0x4

		RegionSound() = delete;
		~RegionSound() = delete;

		//
		// Custom functions.
		//

		void setChance(int newChance);

	};
	static_assert(sizeof(RegionSound) == 0x8, "TES3::RegionSound failed size validation");

	struct Region : BaseObject {
		char id[32]; // 0x10
		char name[32]; // 0x30
		unsigned char weatherChances[10];
		unsigned char padding_0x5A[2];
		LeveledCreature * sleepCreature; // 0x5C
		IteratedList<RegionSound*> sounds; // 0x60
		int currentWeatherIndex; // 0x74

		Region() = delete;
		~Region() = delete;

		//
		// Other related this-call functions.
		//

		void randomizeWeather();

		//
		// Custom functions.
		//

		const char* getName() const;
		void setName(const char* value);

		unsigned char getWeatherChance(int) const;
		void setWeatherChance(int, unsigned char);
		
		std::reference_wrapper<unsigned char[10]> getWeatherChances();

		Weather* getCurrentWeather() const;
		bool setCurrentWeather(const Weather* weather);
		bool setCurrentWeather(int index);
		bool setCurrentWeather_lua(sol::object objectOrId);

		unsigned char getWeatherChanceAsh() const;
		void setWeatherChanceAsh(unsigned char value);
		unsigned char getWeatherChanceBlight() const;
		void setWeatherChanceBlight(unsigned char value);
		unsigned char getWeatherChanceBlizzard() const;
		void setWeatherChanceBlizzard(unsigned char value);
		unsigned char getWeatherChanceClear() const;
		void setWeatherChanceClear(unsigned char value);
		unsigned char getWeatherChanceCloudy() const;
		void setWeatherChanceCloudy(unsigned char value);
		unsigned char getWeatherChanceFoggy() const;
		void setWeatherChanceFoggy(unsigned char value);
		unsigned char getWeatherChanceOvercast() const;
		void setWeatherChanceOvercast(unsigned char value);
		unsigned char getWeatherChanceRain() const;
		void setWeatherChanceRain(unsigned char value);
		unsigned char getWeatherChanceSnow() const;
		void setWeatherChanceSnow(unsigned char value);
		unsigned char getWeatherChanceThunder() const;
		void setWeatherChanceThunder(unsigned char value);

	};
	static_assert(sizeof(Region) == 0x78, "TES3::Region failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::Region)
