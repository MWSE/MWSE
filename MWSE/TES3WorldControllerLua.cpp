#include "TES3WorldControllerLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "NICamera.h"
#include "NINode.h"
#include "NIRenderer.h"

#include "TES3AudioController.h"
#include "TES3Enchantment.h"
#include "TES3Fader.h"
#include "TES3GameSetting.h"
#include "TES3GlobalVariable.h"
#include "TES3InputController.h"
#include "TES3ItemData.h"
#include "TES3MobileActor.h"
#include "TES3MobManager.h"
#include "TES3Quest.h"
#include "TES3Script.h"
#include "TES3Sound.h"
#include "TES3UIMenuController.h"
#include "TES3VFXManager.h"
#include "TES3WeatherController.h"
#include "TES3WorldController.h"

namespace mwse::lua {
	void bindTES3WorldController() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Binding for TES3::WorldControllerRenderCamera::CameraData.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::WorldControllerRenderCamera::CameraData>("tes3worldControllerRenderCameraData");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["camera"] = &TES3::WorldControllerRenderCamera::CameraData::camera;
			usertypeDefinition["farPlaneDistance"] = &TES3::WorldControllerRenderCamera::CameraData::farPlaneDistance;
			usertypeDefinition["fov"] = sol::property(&TES3::WorldControllerRenderCamera::CameraData::getFOV, &TES3::WorldControllerRenderCamera::CameraData::setFOV);
			usertypeDefinition["nearPlaneDistance"] = &TES3::WorldControllerRenderCamera::CameraData::nearPlaneDistance;
			usertypeDefinition["viewportHeight"] = &TES3::WorldControllerRenderCamera::CameraData::viewportHeight;
			usertypeDefinition["viewportWidth"] = &TES3::WorldControllerRenderCamera::CameraData::viewportWidth;
		}

		// Binding for TES3::WorldControllerRenderCamera.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::WorldControllerRenderCamera>("tes3worldControllerRenderCamera");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["cameraData"] = sol::readonly_property(&TES3::WorldControllerRenderCamera::cameraData);
			usertypeDefinition["cameraRoot"] = sol::readonly_property(&TES3::WorldControllerRenderCamera::cameraRoot);
			usertypeDefinition["renderer"] = sol::readonly_property(&TES3::WorldControllerRenderCamera::renderer);
			usertypeDefinition["root"] = sol::readonly_property(&TES3::WorldControllerRenderCamera::root);

			// Legacy support for substructures.
			usertypeDefinition["camera"] = sol::readonly_property(&TES3::WorldControllerRenderCamera::getCamera);
		}

		// Binding for TES3::RechargingItem.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::RechargingItem>("tes3rechargingItem");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["object"] = sol::readonly_property(&TES3::RechargingItem::item);
			usertypeDefinition["itemData"] = sol::readonly_property(&TES3::RechargingItem::itemData);
			usertypeDefinition["enchantment"] = sol::readonly_property(&TES3::RechargingItem::enchantment);
		}

		// Binding for TES3::SplashController::ActiveSplash.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::SplashController::ActiveSplash>("tes3splashControllerActiveSplash");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["age"] = &TES3::SplashController::ActiveSplash::age;
			usertypeDefinition["maxAge"] = &TES3::SplashController::ActiveSplash::maxAge;
			usertypeDefinition["node"] = sol::readonly_property(&TES3::SplashController::ActiveSplash::node);
		}

		// Binding for TES3::SplashController.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::SplashController>("tes3splashController");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["activeSplashes"] = sol::readonly_property(&TES3::SplashController::activeSplashes);
			usertypeDefinition["bloodMeshCount"] = sol::readonly_property(&TES3::SplashController::bloodMeshCount);
			usertypeDefinition["bloodMeshes"] = sol::readonly_property(&TES3::SplashController::getBloodMeshes);
			usertypeDefinition["bloodSplashDurations"] = sol::readonly_property(&TES3::SplashController::getBloodSplashDurations);
			usertypeDefinition["bloodTextureCount"] = sol::readonly_property(&TES3::SplashController::bloodTextureCount);
			usertypeDefinition["bloodTextureProperties"] = sol::readonly_property(&TES3::SplashController::getBloodTextureProperties);
			usertypeDefinition["bloodTextures"] = sol::readonly_property(&TES3::SplashController::getBloodTextures);
		}

		// Binding for TES3::WorldController.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::WorldController>("tes3worldController");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["aiDistanceScale"] = sol::property(&TES3::WorldController::getAIDistanceScale, &TES3::WorldController::setAIDistanceScale);
			usertypeDefinition["allMobileActors"] = sol::readonly_property(&TES3::WorldController::allMobileActors);
			usertypeDefinition["armCamera"] = sol::readonly_property(&TES3::WorldController::armCamera);
			usertypeDefinition["audioController"] = sol::readonly_property(&TES3::WorldController::audioController);
			usertypeDefinition["blindnessFader"] = sol::readonly_property(&TES3::WorldController::blindnessFader);
			usertypeDefinition["characterRenderTarget"] = sol::readonly_property(&TES3::WorldController::characterRenderTarget);
			usertypeDefinition["charGenState"] = sol::readonly_property(&TES3::WorldController::gvarCharGenState);
			usertypeDefinition["countMusicTracksBattle"] = sol::readonly_property(&TES3::WorldController::countMusicTracksBattle);
			usertypeDefinition["countMusicTracksExplore"] = sol::readonly_property(&TES3::WorldController::countMusicTracksExplore);
			usertypeDefinition["criticalDamageSound"] = &TES3::WorldController::soundCriticalDamage;
			usertypeDefinition["cursorOff"] = &TES3::WorldController::cursorOff;
			usertypeDefinition["day"] = sol::readonly_property(&TES3::WorldController::gvarDay);
			usertypeDefinition["daysPassed"] = sol::readonly_property(&TES3::WorldController::gvarDaysPassed);
			usertypeDefinition["deadFloatScale"] = &TES3::WorldController::deadFloatScale;
			usertypeDefinition["defaultLandSound"] = &TES3::WorldController::soundDefaultLand;
			usertypeDefinition["defaultLandWaterSound"] = &TES3::WorldController::soundDefaultLandWater;
			usertypeDefinition["deltaTime"] = &TES3::WorldController::deltaTime;
			usertypeDefinition["difficulty"] = &TES3::WorldController::difficulty;
			usertypeDefinition["drowningDamageSound"] = &TES3::WorldController::soundDrowningDamage;
			usertypeDefinition["drownSound"] = &TES3::WorldController::soundDrown;
			usertypeDefinition["enchantedItemEffect"] = sol::readonly_property(&TES3::WorldController::enchantedItemEffect);
			usertypeDefinition["enchantedItemEffectCreated"] = sol::readonly_property(&TES3::WorldController::enchantedItemEffectCreated);
			usertypeDefinition["enchantedItemEffectTextures"] = sol::readonly_property(&TES3::WorldController::enchantedItemEffectTextures);
			usertypeDefinition["flagLevitationDisabled"] = sol::property(&TES3::WorldController::getLevitationDisabled, &TES3::WorldController::setLevitationDisabled);
			usertypeDefinition["flagTeleportingDisabled"] = &TES3::WorldController::flagTeleportingDisabled;
			usertypeDefinition["globalScripts"] = sol::readonly_property(&TES3::WorldController::globalScripts);
			usertypeDefinition["handToHandHit2Sound"] = &TES3::WorldController::soundHandToHandHit2;
			usertypeDefinition["handToHandHitSound"] = &TES3::WorldController::soundHandToHandHit;
			usertypeDefinition["healthDamageSound"] = &TES3::WorldController::soundHealthDamage;
			usertypeDefinition["heavyArmorHitSound"] = &TES3::WorldController::soundHeavyArmorHit;
			usertypeDefinition["helpDelay"] = &TES3::WorldController::helpDelay;
			usertypeDefinition["hitFader"] = sol::readonly_property(&TES3::WorldController::hitFader);
			usertypeDefinition["hour"] = sol::readonly_property(&TES3::WorldController::gvarGameHour);
			usertypeDefinition["hudStyle"] = &TES3::WorldController::hudStyle;
			usertypeDefinition["inputController"] = sol::readonly_property(&TES3::WorldController::inputController);
			usertypeDefinition["instance"] = sol::readonly_property(&TES3::WorldController::Win32_hInstance);
			usertypeDefinition["itemRepairSound"] = &TES3::WorldController::soundItemRepair;
			usertypeDefinition["lastFrameTime"] = sol::readonly_property(&TES3::WorldController::lastFrameTimeMillis);
			usertypeDefinition["lightArmorHitSound"] = &TES3::WorldController::soundLightArmorHit;
			usertypeDefinition["mapController"] = sol::readonly_property(&TES3::WorldController::mapController);
			usertypeDefinition["maxFPS"] = &TES3::WorldController::maxFPS;
			usertypeDefinition["mediumArmorHitSound"] = &TES3::WorldController::soundMediumArmorHit;
			usertypeDefinition["menuAlpha"] = &TES3::WorldController::menuAlpha;
			usertypeDefinition["menuCamera"] = sol::readonly_property(&TES3::WorldController::menuCamera);
			usertypeDefinition["menuClickSound"] = &TES3::WorldController::soundMenuClick;
			usertypeDefinition["menuController"] = sol::readonly_property(&TES3::WorldController::menuController);
			usertypeDefinition["menuSizeSound"] = &TES3::WorldController::soundMenuSize;
			usertypeDefinition["missSound"] = &TES3::WorldController::soundMiss;
			usertypeDefinition["mobManager"] = sol::readonly_property(&TES3::WorldController::mobManager);
			usertypeDefinition["month"] = sol::readonly_property(&TES3::WorldController::gvarMonth);
			usertypeDefinition["monthsToRespawn"] = sol::readonly_property(&TES3::WorldController::gvarMonthsToRespawn);
			usertypeDefinition["mouseSensitivityX"] = &TES3::WorldController::horzSensitivity;
			usertypeDefinition["mouseSensitivityY"] = &TES3::WorldController::mouseSensitivity;
			usertypeDefinition["musicSituation"] = &TES3::WorldController::musicSituation;
			usertypeDefinition["nodeCursor"] = sol::readonly_property(&TES3::WorldController::nodeCursor);
			usertypeDefinition["parentWindowHandle"] = sol::readonly_property(&TES3::WorldController::Win32_hWndParent);
			usertypeDefinition["projectionDistance"] = &TES3::WorldController::projectionDistance;
			usertypeDefinition["quests"] = &TES3::WorldController::journalController;
			usertypeDefinition["quickSaveWhenResting"] = &TES3::WorldController::quickSaveWhenResting;
			usertypeDefinition["rechargingItems"] = sol::readonly_property(&TES3::WorldController::rechargingItems);
			usertypeDefinition["shaderWaterReflectTerrain"] = &TES3::WorldController::shaderWaterReflectsTerrain;
			usertypeDefinition["shaderWaterReflectUpdate"] = &TES3::WorldController::shaderWaterReflectUpdate;
			usertypeDefinition["shadowCamera"] = sol::readonly_property(&TES3::WorldController::shadowCamera);
			usertypeDefinition["shadows"] = sol::property(&TES3::WorldController::getShadowLevel, &TES3::WorldController::setShadowLevel);
			usertypeDefinition["showSubtitles"] = &TES3::WorldController::showSubtitles;
			usertypeDefinition["simulationTimeScalar"] = sol::var(std::ref(TES3::WorldController::simulationTimeScalar));
			usertypeDefinition["splashController"] = sol::readonly_property(&TES3::WorldController::splashController);
			usertypeDefinition["splashscreenCamera"] = sol::readonly_property(&TES3::WorldController::splashscreenCamera);
			usertypeDefinition["stopGameLoop"] = &TES3::WorldController::stopGameLoop;
			usertypeDefinition["sunglareFader"] = sol::readonly_property(&TES3::WorldController::sunglareFader);
			usertypeDefinition["systemTime"] = sol::readonly_property(&TES3::WorldController::systemTimeMillis);
			usertypeDefinition["timescale"] = sol::readonly_property(&TES3::WorldController::gvarTimescale);
			usertypeDefinition["transitionFader"] = sol::readonly_property(&TES3::WorldController::transitionFader);
			usertypeDefinition["useBestAttack"] = &TES3::WorldController::useBestAttack;
			usertypeDefinition["vfxManager"] = sol::readonly_property(&TES3::WorldController::vfxManager);
			usertypeDefinition["viewHeight"] = &TES3::WorldController::viewHeight;
			usertypeDefinition["viewWidth"] = &TES3::WorldController::viewWidth;
			usertypeDefinition["weaponSwishSound"] = &TES3::WorldController::soundWeaponSwish;
			usertypeDefinition["weatherController"] = sol::readonly_property(&TES3::WorldController::weatherController);
			usertypeDefinition["werewolfFader"] = sol::readonly_property(&TES3::WorldController::werewolfFader);
			usertypeDefinition["werewolfFOV"] = &TES3::WorldController::werewolfFOV;
			usertypeDefinition["windowHandle"] = sol::readonly_property(&TES3::WorldController::Win32_hWnd);
			usertypeDefinition["worldCamera"] = sol::readonly_property(&TES3::WorldController::worldCamera);
			usertypeDefinition["year"] = sol::readonly_property(&TES3::WorldController::gvarYear);

			// Basic function binding.
			usertypeDefinition["applyEnchantEffect"] = &TES3::WorldController::applyEnchantEffect;

			// Legacy bindings.
			usertypeDefinition["aiDistance"] = sol::property(&TES3::WorldController::getAIDistanceScale, &TES3::WorldController::setAIDistanceScale);
			usertypeDefinition["chargableItems"] = sol::readonly_property(&TES3::WorldController::rechargingItems);
			usertypeDefinition["flagEventMenuModeOff"] = &TES3::WorldController::flagEventMenuModeOff;
			usertypeDefinition["flagEventMenuModeOn"] = &TES3::WorldController::flagEventMenuModeOn;
			usertypeDefinition["flagMenuMode"] = &TES3::WorldController::flagMenuMode;
			usertypeDefinition["mobController"] = sol::readonly_property(&TES3::WorldController::mobManager);
		}
	}
}
