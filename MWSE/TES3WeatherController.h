#pragma once

#include "TES3Defines.h"
#include "NIDefines.h"

#include "TES3IteratedList.h"
#include "TES3Vectors.h"

#include "NINode.h"

namespace TES3 {
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

			First = Clear,
			Last = Blizzard
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
		char unknown_0x10;
		Weather* arrayWeathers[10]; // 0x14
		Weather* currentWeather; // 0x3C
		Weather* nextWeather; // 0x40
		Moon* moonSecunda; // 0x44
		Moon* moonMasser; // 0x48
		NI::Node* sgSkyRoot; // 0x4C
		NI::Node* sgSkyNight; // 0x50
		NI::Node* sgSkyAtmosphere; // 0x54
		NI::Node* sgSkyClouds; // 0x58
		NI::Node* sgRainRoot; // 0x5C
		NI::Node* sgSnowRoot; // 0x60
		NI::TriShape* sgTriRain; // 0x64
		NI::Node* sgSnowflake; // 0x68
		NI::Node* sgStormRoot; // 0x6C
		NI::Node* sgAshCloud; // 0x70
		NI::Node* sgBlightCloud; // 0x74
		NI::Node* sgBlizzard; // 0x78
		NI::TriShape* sgTriAtmosphere; // 0x7C
		NI::TriShape* sgTriClouds0; // 0x80
		NI::TriShape* sgTriClouds1; // 0x84
		NI::TriShape* shTriSunBase; // 0x88
		NI::TriShape* sgTriSunGlare; // 0x8C
		Vector3 currentSkyColor; // 0x90
		Vector3 currentFogColor; // 0x9C
		NI::Pick* pickSunglare; // 0xA8
		NI::DirectionalLight* sgSkyLight; // 0xAC
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
		char bInactivateWeather; // 0x198
		char bUnderwaterPitchbendState; // 0x199
		char bRainRipples; // 0x19A
		char bSnowRipples; // 0x19B
		char bTimescaleClouds; // 0x19C
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

		std::reference_wrapper<Weather*[10]> getWeathers();

		void updateVisuals();

		void switchImmediate(int weather);
		void switchTransition(int weather);
	};
	static_assert(sizeof(WeatherController) == 0x1F0, "TES3::WeatherController failed size validation");
	static_assert(sizeof(WeatherController::Particle) == 0x38, "TES3::WeatherController::Particle failed size validation");
	static_assert(sizeof(WeatherController::Particle::VirtualTable) == 0x10, "TES3::WeatherController::Particle::VirtualTable failed size validation");
}
