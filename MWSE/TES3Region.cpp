#include "TES3Region.h"

#include "TES3Weather.h"
#include "TES3WeatherController.h"
#include "TES3WorldController.h"

#define TES3_Region_changeWeather 0x4812F0
#define TES3_Region_randomizeWeather 0x4812A0

namespace TES3 {
	//
	// TES3::RegionSound
	//

	void RegionSound::setChance(int newChance) {
		chance = std::clamp(newChance, 0, 100);
	}

	//
	// TES3::Region
	//

	void Region::changeWeather(int weather) {
		reinterpret_cast<void(__thiscall *)(Region*,int)>(TES3_Region_changeWeather)(this, weather);
	}

	void Region::randomizeWeather() {
		reinterpret_cast<void(__thiscall *)(Region*)>(TES3_Region_randomizeWeather)(this);
	}

	const char* Region::getName() const {
		return name;
	}

	void Region::setName(const char* value) {
		if (strlen(value) >= 32) {
			throw std::invalid_argument("Path cannot be 32 or more characters.");
		}
		strncpy_s(name, value, 32);
	}

	unsigned char Region::getWeatherChance(unsigned int weatherIndex) {
		if (weatherIndex < WeatherType::First || weatherIndex > WeatherType::Last) {
			return -1;
		}

		return weatherChances[weatherIndex];
	}

	void Region::setWeatherChance(unsigned int weatherIndex, unsigned char chance) {
		if (weatherIndex < WeatherType::First || weatherIndex > WeatherType::Last) {
			return;
		}

		weatherChances[weatherIndex] = std::clamp((int)chance, 0, 100);
	}

	std::reference_wrapper<unsigned char[10]> Region::getWeatherChances() {
		return std::ref(weatherChances);
	}

	Weather* Region::getCurrentWeather() const {
		if (currentWeatherIndex < WeatherType::First || currentWeatherIndex > WeatherType::Last) {
			return nullptr;
		}
		return TES3::WorldController::get()->weatherController->arrayWeathers[currentWeatherIndex];
	}

	bool Region::setCurrentWeather(const Weather* weather) {
		return setCurrentWeather(weather ? weather->index : WeatherType::Clear);
	}

	bool Region::setCurrentWeather(int index) {
		if (index < WeatherType::First || index > WeatherType::Last) {
			return false;
		}

		if (index != currentWeatherIndex) {
			changeWeather(index);
			return true;
		}

		return false;
	}

	bool Region::setCurrentWeather_lua(sol::object objectOrId) {
		if (objectOrId.is<TES3::Weather>()) {
			return setCurrentWeather(objectOrId.as<TES3::Weather*>());
		}
		else if (objectOrId.is<int>()) {
			return setCurrentWeather(objectOrId.as<int>());
		}
		return false;
	}

	unsigned char Region::getWeatherChanceAsh() const {
		return weatherChances[WeatherType::Ash];
	}

	void Region::setWeatherChanceAsh(unsigned char value) {
		weatherChances[WeatherType::Ash] = value;
	}

	unsigned char Region::getWeatherChanceBlight() const {
		return weatherChances[WeatherType::Blight];
	}

	void Region::setWeatherChanceBlight(unsigned char value) {
		weatherChances[WeatherType::Blight] = value;
	}

	unsigned char Region::getWeatherChanceBlizzard() const {
		return weatherChances[WeatherType::Blizzard];
	}

	void Region::setWeatherChanceBlizzard(unsigned char value) {
		weatherChances[WeatherType::Blizzard] = value;
	}

	unsigned char Region::getWeatherChanceClear() const {
		return weatherChances[WeatherType::Clear];
	}

	void Region::setWeatherChanceClear(unsigned char value) {
		weatherChances[WeatherType::Clear] = value;
	}

	unsigned char Region::getWeatherChanceCloudy() const {
		return weatherChances[WeatherType::Cloudy];
	}

	void Region::setWeatherChanceCloudy(unsigned char value) {
		weatherChances[WeatherType::Cloudy] = value;
	}

	unsigned char Region::getWeatherChanceFoggy() const {
		return weatherChances[WeatherType::Foggy];
	}

	void Region::setWeatherChanceFoggy(unsigned char value) {
		weatherChances[WeatherType::Foggy] = value;
	}

	unsigned char Region::getWeatherChanceOvercast() const {
		return weatherChances[WeatherType::Overcast];
	}

	void Region::setWeatherChanceOvercast(unsigned char value) {
		weatherChances[WeatherType::Overcast] = value;
	}

	unsigned char Region::getWeatherChanceRain() const {
		return weatherChances[WeatherType::Rain];
	}

	void Region::setWeatherChanceRain(unsigned char value) {
		weatherChances[WeatherType::Rain] = value;
	}

	unsigned char Region::getWeatherChanceSnow() const {
		return weatherChances[WeatherType::Snow];
	}

	void Region::setWeatherChanceSnow(unsigned char value) {
		weatherChances[WeatherType::Snow] = value;
	}

	unsigned char Region::getWeatherChanceThunder() const {
		return weatherChances[WeatherType::Thunder];
	}

	void Region::setWeatherChanceThunder(unsigned char value) {
		weatherChances[WeatherType::Thunder] = value;
	}

}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::Region)
