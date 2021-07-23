#include "TES3WorldControllerLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "NICamera.h"
#include "NINode.h"
#include "NIRenderer.h"

#include "TES3AudioController.h"
#include "TES3Fader.h"
#include "TES3GameSetting.h"
#include "TES3GlobalVariable.h"
#include "TES3InputController.h"
#include "TES3MobileActor.h"
#include "TES3MobController.h"
#include "TES3Quest.h"
#include "TES3Script.h"
#include "TES3Sound.h"
#include "TES3UIMenuController.h"
#include "TES3WeatherController.h"
#include "TES3WorldController.h"

namespace mwse {
	namespace lua {
		void bindTES3WorldController() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Binding for TES3::WorldControllerRenderCamera.
			{
				// Start our usertype. We must finish this with state.set_usertype.
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
				// Start our usertype. We must finish this with state.set_usertype.
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

			// Binding for TES3::WorldController.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<TES3::WorldController>("tes3worldController");
				usertypeDefinition["new"] = sol::no_constructor;

				// Basic property binding.
				usertypeDefinition["activeQuests"] = &TES3::WorldController::journalController;
				usertypeDefinition["aiDistanceScale"] = sol::property(&TES3::WorldController::getAIDistanceScale, &TES3::WorldController::setAIDistanceScale);
				usertypeDefinition["armCamera"] = &TES3::WorldController::armCamera;
				usertypeDefinition["audioController"] = sol::readonly_property(&TES3::WorldController::audioController);
				usertypeDefinition["blindnessFader"] = sol::readonly_property(&TES3::WorldController::blindnessFader);
				usertypeDefinition["characterRenderTarget"] = &TES3::WorldController::characterRenderTarget;
				usertypeDefinition["chargableItems"] = &TES3::WorldController::chargableItems;
				usertypeDefinition["charGenState"] = &TES3::WorldController::gvarCharGenState;
				usertypeDefinition["countMusicTracksBattle"] = &TES3::WorldController::countMusicTracksBattle;
				usertypeDefinition["countMusicTracksExplore"] = &TES3::WorldController::countMusicTracksExplore;
				usertypeDefinition["criticalDamageSound"] = &TES3::WorldController::soundCriticalDamage;
				usertypeDefinition["cursorOff"] = &TES3::WorldController::cursorOff;
				usertypeDefinition["day"] = &TES3::WorldController::gvarDay;
				usertypeDefinition["daysPassed"] = &TES3::WorldController::gvarDaysPassed;
				usertypeDefinition["deadFloatScale"] = &TES3::WorldController::deadFloatScale;
				usertypeDefinition["defaultLandSound"] = &TES3::WorldController::soundDefaultLand;
				usertypeDefinition["defaultLandWaterSound"] = &TES3::WorldController::soundDefaultLandWater;
				usertypeDefinition["deltaTime"] = &TES3::WorldController::deltaTime;
				usertypeDefinition["difficulty"] = &TES3::WorldController::difficulty;
				usertypeDefinition["drowningDamageSound"] = &TES3::WorldController::soundDrowningDamage;
				usertypeDefinition["drownSound"] = &TES3::WorldController::soundDrown;
				usertypeDefinition["enchantedItemEffect"] = &TES3::WorldController::enchantedItemEffect;
				usertypeDefinition["enchantedItemEffectCreated"] = &TES3::WorldController::enchantedItemEffectCreated;
				usertypeDefinition["enchantedItemEffectTextures"] = &TES3::WorldController::enchantedItemEffectTextures;
				usertypeDefinition["flagEventMenuModeOff"] = &TES3::WorldController::flagEventMenuModeOff;
				usertypeDefinition["flagEventMenuModeOn"] = &TES3::WorldController::flagEventMenuModeOn;
				usertypeDefinition["flagLevitationDisabled"] = &TES3::WorldController::flagLevitationDisabled;
				usertypeDefinition["flagMenuMode"] = &TES3::WorldController::flagMenuMode;
				usertypeDefinition["flagTeleportingDisabled"] = &TES3::WorldController::flagTeleportingDisabled;
				usertypeDefinition["globalScripts"] = &TES3::WorldController::globalScripts;
				usertypeDefinition["handToHandHit2Sound"] = &TES3::WorldController::soundHandToHandHit2;
				usertypeDefinition["handToHandHitSound"] = &TES3::WorldController::soundHandToHandHit;
				usertypeDefinition["healthDamageSound"] = &TES3::WorldController::soundHealthDamage;
				usertypeDefinition["heavyArmorHitSound"] = &TES3::WorldController::soundHeavyArmorHit;
				usertypeDefinition["helpDelay"] = &TES3::WorldController::helpDelay;
				usertypeDefinition["hitFader"] = sol::readonly_property(&TES3::WorldController::hitFader);
				usertypeDefinition["hour"] = &TES3::WorldController::gvarGameHour;
				usertypeDefinition["hudStyle"] = &TES3::WorldController::hudStyle;
				usertypeDefinition["inputController"] = &TES3::WorldController::inputController;
				usertypeDefinition["instance"] = &TES3::WorldController::Win32_hInstance;
				usertypeDefinition["inventoryData"] = &TES3::WorldController::inventoryData;
				usertypeDefinition["itemRepairSound"] = &TES3::WorldController::soundItemRepair;
				usertypeDefinition["lastFrameTime"] = &TES3::WorldController::lastFrameTimeMillis;
				usertypeDefinition["lightArmorHitSound"] = &TES3::WorldController::soundLightArmorHit;
				usertypeDefinition["listAllActors"] = &TES3::WorldController::allActors;
				usertypeDefinition["mapController"] = &TES3::WorldController::mapController;
				usertypeDefinition["maxFPS"] = &TES3::WorldController::maxFPS;
				usertypeDefinition["mediumArmorHitSound"] = &TES3::WorldController::soundMediumArmorHit;
				usertypeDefinition["menuAlpha"] = &TES3::WorldController::menuAlpha;
				usertypeDefinition["menuCamera"] = &TES3::WorldController::menuCamera;
				usertypeDefinition["menuClickSound"] = &TES3::WorldController::soundMenuClick;
				usertypeDefinition["menuController"] = &TES3::WorldController::menuController;
				usertypeDefinition["menuSizeSound"] = &TES3::WorldController::soundMenuSize;
				usertypeDefinition["missSound"] = &TES3::WorldController::soundMiss;
				usertypeDefinition["mobController"] = sol::readonly_property(&TES3::WorldController::mobController);
				usertypeDefinition["month"] = &TES3::WorldController::gvarMonth;
				usertypeDefinition["monthsToRespawn"] = &TES3::WorldController::gvarMonthsToRespawn;
				usertypeDefinition["mouseSensitivityX"] = &TES3::WorldController::horzSensitivity;
				usertypeDefinition["mouseSensitivityY"] = &TES3::WorldController::mouseSensitivity;
				usertypeDefinition["musicSituation"] = &TES3::WorldController::musicSituation;
				usertypeDefinition["nodeCursor"] = &TES3::WorldController::nodeCursor;
				usertypeDefinition["parentWindowHandle"] = &TES3::WorldController::Win32_hWndParent;
				usertypeDefinition["projectionDistance"] = &TES3::WorldController::projectionDistance;
				usertypeDefinition["quickSaveWhenResting"] = &TES3::WorldController::quickSaveWhenResting;
				usertypeDefinition["shaderWaterReflectTerrain"] = &TES3::WorldController::shaderWaterReflectsTerrain;
				usertypeDefinition["shaderWaterReflectUpdate"] = &TES3::WorldController::shaderWaterReflectUpdate;
				usertypeDefinition["shadowCamera"] = &TES3::WorldController::shadowCamera;
				usertypeDefinition["shadows"] = &TES3::WorldController::bShadows;
				usertypeDefinition["showSubtitles"] = &TES3::WorldController::showSubtitles;
				usertypeDefinition["splashscreenCamera"] = &TES3::WorldController::splashscreenCamera;
				usertypeDefinition["stopGameLoop"] = &TES3::WorldController::stopGameLoop;
				usertypeDefinition["sunglareFader"] = sol::readonly_property(&TES3::WorldController::sunglareFader);
				usertypeDefinition["systemTime"] = &TES3::WorldController::systemTimeMillis;
				usertypeDefinition["timescale"] = &TES3::WorldController::gvarTimescale;
				usertypeDefinition["transitionFader"] = sol::readonly_property(&TES3::WorldController::transitionFader);
				usertypeDefinition["useBestAttack"] = &TES3::WorldController::useBestAttack;
				usertypeDefinition["viewHeight"] = &TES3::WorldController::viewHeight;
				usertypeDefinition["viewWidth"] = &TES3::WorldController::viewWidth;
				usertypeDefinition["weaponSwishSound"] = &TES3::WorldController::soundWeaponSwish;
				usertypeDefinition["weatherController"] = &TES3::WorldController::weatherController;
				usertypeDefinition["werewolfFader"] = sol::readonly_property(&TES3::WorldController::werewolfFader);
				usertypeDefinition["werewolfFOV"] = &TES3::WorldController::werewolfFOV;
				usertypeDefinition["windowHandle"] = &TES3::WorldController::Win32_hWnd;
				usertypeDefinition["worldCamera"] = &TES3::WorldController::worldCamera;
				usertypeDefinition["year"] = &TES3::WorldController::gvarYear;

				// Legacy bindings.
				usertypeDefinition["aiDistance"] = sol::property(&TES3::WorldController::getAIDistanceScale, &TES3::WorldController::setAIDistanceScale);

				// Basic function binding.
				usertypeDefinition["applyEnchantEffect"] = &TES3::WorldController::applyEnchantEffect;
			}
		}
	}
}
