#pragma once

#include "TES3Defines.h"
#include "NIDefines.h"

#include "TES3IteratedList.h"
#include "TES3Vectors.h"

#include "NINode.h"

namespace TES3 {
	constexpr int WEATHER_ID_INVALID = -1;
	constexpr int VANILLA_MAX_WEATHER_COUNT = 10;
	constexpr int MAX_WEATHER_COUNT = VANILLA_MAX_WEATHER_COUNT;

	namespace WeatherType {
		enum WeatherType {
			Clear,
			Cloudy,
			Foggy,
			Overcast,
			Rain,
			Thunder,
			Ash,
			Blight,
			Snow,
			Blizzard,

			MINIMUM = 0,
			MAXIMUM = MAX_WEATHER_COUNT - 1
		};
	}

	struct WeatherController {
		struct Particle {
			struct VirtualTable {
				void* dtor; // 0x0
				void* getType; // 0x4
				void* create; // 0x8
				void* update; // 0xC
			};
			VirtualTable* vtbl;
			Vector3 unknown_0x4;
			Vector3 velocity; // 0x10
			WeatherController* weatherController; // 0x1C
			NI::Pointer<NI::Node> rainRoot; // 0x20
			float remainingLifetime; // 0x24
			float diameter; // 0x28
			int unknown_0x2C;
			NI::Pointer<NI::AVObject> object; // 0x30
			int unknown_0x34;
		};
		NI::Node* sgSunVis; // 0x0
		NI::Node* sgSunBase; // 0x4
		NI::Node* sgSunGlare; // 0x8
		int daysRemaining; // 0xC
		bool unknown_0x10;
		Weather* arrayWeathers[MAX_WEATHER_COUNT]; // 0x14
		Weather* currentWeather; // 0x3C
		Weather* nextWeather; // 0x40
		Moon* moonSecunda; // 0x44
		Moon* moonMasser; // 0x48
		NI::Pointer<NI::Node> sgSkyRoot; // 0x4C
		NI::Pointer<NI::Node> sgSkyNight; // 0x50
		NI::Pointer<NI::Node> sgSkyAtmosphere; // 0x54
		NI::Pointer<NI::Node> sgSkyClouds; // 0x58
		NI::Pointer<NI::Node> sgRainRoot; // 0x5C
		NI::Pointer<NI::Node> sgSnowRoot; // 0x60
		NI::Pointer<NI::TriShape> sgTriRain; // 0x64
		NI::Pointer<NI::Node> sgSnowflake; // 0x68
		NI::Pointer<NI::Node> sgStormRoot; // 0x6C
		NI::Pointer<NI::Node> sgAshCloud; // 0x70
		NI::Pointer<NI::Node> sgBlightCloud; // 0x74
		NI::Pointer<NI::Node> sgBlizzard; // 0x78
		NI::Pointer<NI::TriShape> sgTriAtmosphere; // 0x7C
		NI::Pointer<NI::TriShape> sgTriCloudsCurrent; // 0x80
		NI::Pointer<NI::TriShape> sgTriCloudsNext; // 0x84
		NI::Pointer<NI::TriShape> shTriSunBase; // 0x88
		NI::Pointer<NI::TriShape> sgTriSunGlare; // 0x8C
		Vector3 currentSkyColor; // 0x90
		Vector3 currentFogColor; // 0x9C
		NI::Pick* pickSunglare; // 0xA8
		NI::Pointer<NI::DirectionalLight> sgSkyLight; // 0xAC
		int unknown_0xB0;
		int unknown_0xB4;
		Vector3 windVelocityCurrWeather; // 0xB8
		Vector3 windVelocityNextWeather; // 0xC4
		float smoothedSunglareVis; // 0xD0
		float sunglareFaderMax; // 0xD4
		float sunglareFaderAngleMax; // 0xD8
		float sunriseHour; // 0xDC
		float sunsetHour; // 0xE0
		float sunriseDuration; // 0xE4
		float sunsetDuration; // 0xE8
		float fogDepthChangeSpeed; // 0xEC
		float ambientPreSunriseTime; // 0xF0
		float ambientPostSunriseTime; // 0xF4
		float ambientPreSunsetTime; // 0xF8
		float ambientPostSunsetTime; // 0xFC
		float fogPreSunriseTime; // 0x100
		float fogPostSunriseTime; // 0x104
		float fogPreSunsetTime; // 0x108
		float fogPostSunsetTime; // 0x10C
		float skyPreSunriseTime; // 0x110
		float skyPostSunriseTime; // 0x114
		float skyPreSunsetTime; // 0x118
		float skyPostSunsetTime; // 0x11C
		float sunPreSunriseTime; // 0x120
		float sunPostSunriseTime; // 0x124
		float sunPreSunsetTime; // 0x128
		float sunPostSunsetTime; // 0x12C
		float starsPostSunsetStart; // 0x130
		float starsPreSunriseFinish; // 0x134
		float starsFadingDuration; // 0x138
		int activeRainParticles; // 0x13C
		int activeSnowParticles; // 0x140
		IteratedList<Particle*> listActiveParticles; // 0x144
		IteratedList<Particle*> listInactiveParticles; // 0x158
		float hoursBetweenWeatherChanges; // 0x16C
		float transitionScalar; // 0x170
		float hoursRemaining; // 0x174
		float activeThunderFlashIntensity; // 0x178
		float precipitationFallSpeed; // 0x17C
		float snowFallSpeedScale; // 0x180
		float snowHighKill; // 0x184
		float snowLowKill; // 0x188
		Vector3 sunTransitConstants; // 0x18C
		bool inactivateWeather; // 0x198
		bool underwaterPitchbendState; // 0x199
		bool rainRipples; // 0x19A
		bool snowRipples; // 0x19B
		bool timescaleClouds; // 0x19C
		float underwaterSunriseFog; // 0x1A0
		float underwaterDayFog; // 0x1A4
		float underwaterSunsetFog; // 0x1A8
		float underwaterNightFog; // 0x1AC
		float underwaterIndoorFog; // 0x1B0
		Vector3 underwaterCol; // 0x1B4
		float underwaterColWeight; // 0x1C0
		Vector3 sunglareFaderCol; // 0x1C4
		Region* lastActiveRegion; // 0x1D0
		DataHandler* dataHandler; // 0x1D4
		Sound* soundUnderwater; // 0x1D8
		Vector3 skyDomePosition; // 0x1DC
		int sunGlareRayTestLoadBalancer; // 0x1E8
		bool isSunOccluded; // 0x1EC

		WeatherController() = delete;
		~WeatherController() = delete;

		//
		// Other related this-call functions.
		//

		int getCurrentWeatherIndex() const;
		float calcSunDamageScalar();
		void switchWeather(int weatherId, float startingTransition);

		void enableSky();
		void disableSky();
		
		//
		// Helper functions.
		//

		std::reference_wrapper<Weather* [MAX_WEATHER_COUNT]> getWeathers();
		Weather* getWeather(int weatherId) const;
		WeatherClear* getWeatherClear() const;
		WeatherCloudy* getWeatherCloudy() const;
		WeatherFoggy* getWeatherFoggy() const;
		WeatherOvercast* getWeatherOvercast() const;
		WeatherRain* getWeatherRain() const;
		WeatherThunder* getWeatherThunder() const;
		WeatherAsh* getWeatherAsh() const;
		WeatherBlight* getWeatherBlight() const;
		WeatherSnow* getWeatherSnow() const;
		WeatherBlizzard* getWeatherBlizzard() const;

		void updateVisuals();

		void switchImmediate(int weather);
		void switchTransition(int weather);
	};
	static_assert(sizeof(WeatherController) == 0x1F0, "TES3::WeatherController failed size validation");
	static_assert(offsetof(WeatherController, currentWeather) == 0x3C, "TES3::WeatherController::currentWeather failed offset validation");
	static_assert(offsetof(WeatherController, isSunOccluded) == 0x1EC, "TES3::WeatherController::isSunOccluded failed offset validation");
	static_assert(sizeof(WeatherController::Particle) == 0x38, "TES3::WeatherController::Particle failed size validation");
	static_assert(sizeof(WeatherController::Particle::VirtualTable) == 0x10, "TES3::WeatherController::Particle::VirtualTable failed size validation");
}
