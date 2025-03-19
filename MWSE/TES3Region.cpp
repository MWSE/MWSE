#include "TES3Region.h"

#include "TES3Weather.h"
#include "TES3WeatherController.h"
#include "TES3WorldController.h"

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

	const auto TES3_Region_randomizeWeather = reinterpret_cast<void(__thiscall*)(Region*)>(0x4812A0);
	void Region::randomizeWeather() {
		TES3_Region_randomizeWeather(this);
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

	unsigned char Region::getWeatherChance(int weatherIndex) const {
		if (weatherIndex < WeatherType::MINIMUM || weatherIndex > WeatherType::MAXIMUM) {
			return -1;
		}

		return weatherChances[weatherIndex];
	}

	void Region::setWeatherChance(int weatherIndex, unsigned char chance) {
		if (weatherIndex < WeatherType::MINIMUM || weatherIndex > WeatherType::MAXIMUM) {
			return;
		}

		weatherChances[weatherIndex] = std::clamp((int)chance, 0, 100);
	}

	std::reference_wrapper<unsigned char[10]> Region::getWeatherChances() {
		return std::ref(weatherChances);
	}

	Weather* Region::getCurrentWeather() const {
		if (currentWeatherIndex < WeatherType::MINIMUM || currentWeatherIndex > WeatherType::MAXIMUM) {
			return nullptr;
		}
		return TES3::WorldController::get()->weatherController->arrayWeathers[currentWeatherIndex];
	}

	bool Region::setCurrentWeather(const Weather* weather) {
		return setCurrentWeather(weather ? weather->index : WeatherType::Clear);
	}

	bool Region::setCurrentWeather(int index) {
		if (index < WeatherType::MINIMUM || index > WeatherType::MAXIMUM) {
			return false;
		}

		const auto weatherController = TES3::WorldController::get()->weatherController;
		if (weatherController->arrayWeathers[index] == nullptr) {
			return false;
		}

		if (index == currentWeatherIndex) {
			return false;
		}

		currentWeatherIndex = index;
		return true;
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
