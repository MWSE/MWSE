#include "TES3WeatherController.h"

#include "TES3Cell.h"
#include "TES3DataHandler.h"
#include "TES3GlobalVariable.h"
#include "TES3Region.h"
#include "TES3Spell.h"
#include "TES3WeatherAsh.h"
#include "TES3WeatherBlight.h"
#include "TES3WeatherBlizzard.h"
#include "TES3WeatherClear.h"
#include "TES3WeatherCloudy.h"
#include "TES3WeatherCustom.h"
#include "TES3WeatherFoggy.h"
#include "TES3WeatherOvercast.h"
#include "TES3WeatherRain.h"
#include "TES3WeatherSnow.h"
#include "TES3WeatherThunder.h"
#include "TES3WorldController.h"

#include "NIProperty.h"
#include "NIRenderer.h"
#include "NISourceTexture.h"
#include "NITriShape.h"

#include "LuaManager.h"
#include "LuaCalcSunDamageScalarEvent.h"
#include "LuaWeatherChangedImmediateEvent.h"
#include "LuaWeatherTransitionFinishedEvent.h"
#include "LuaWeatherTransitionStartedEvent.h"

#include "MathUtil.h"
#include "MemoryUtil.h"
#include "TES3Util.h"

namespace TES3 {
	typedef mwse::ExternalGlobal<bool, 0x785FAC> unknown_785FAC;

	const auto TES3_WeatherController_ctor = reinterpret_cast<WeatherController*(__thiscall*)(WeatherController*, int)>(0x439F60);
	WeatherController* WeatherController::ctor(int initialWeatherId) {
		// Call vanilla function.
		TES3_WeatherController_ctor(this, initialWeatherId);

#if defined(MWSE_CUSTOM_WEATHERS) && MWSE_CUSTOM_WEATHERS == TRUE
		// Copy created weathers to the new location.
		ZeroMemory(&arrayWeathers, sizeof(arrayWeathers));
		memcpy_s(&arrayWeathers, sizeof(arrayWeathers), &vanillaWeathers, sizeof(vanillaWeathers));
		ZeroMemory(&vanillaWeathers, sizeof(vanillaWeathers));
#endif

		return this;
	}

	const auto TES3_WeatherController_dtor = reinterpret_cast<WeatherController * (__thiscall*)(WeatherController*)>(0x43B1C0);
	void WeatherController::dtor() {
		// Call vanilla function.
		TES3_WeatherController_dtor(this);

#if defined(MWSE_CUSTOM_WEATHERS) && MWSE_CUSTOM_WEATHERS == TRUE
		// Clean up where we moved the weathers.
		for (auto& weather : arrayWeathers) {
			if (weather == nullptr) {
				continue;
			}

			delete weather;
			weather = nullptr;
		}
#endif
	}

	const auto TES3_WeatherController_getCurrentWeatherIndex = reinterpret_cast<int(__thiscall*)(const WeatherController*)>(0x4424E0);
	int WeatherController::getCurrentWeatherIndex() const {
		return TES3_WeatherController_getCurrentWeatherIndex(this);
	}

	const auto TES3_WeatherController_calcSunDamageScalar = reinterpret_cast<float(__thiscall*)(WeatherController*)>(0x0440630);
	float WeatherController::calcSunDamageScalar() {
		float damage = TES3_WeatherController_calcSunDamageScalar(this);

		// Trigger calcSunDamageScalar event.
		mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
		const auto stateHandle = luaManager.getThreadSafeStateHandle();
		sol::table eventData = stateHandle.triggerEvent(new mwse::lua::event::CalcSunDamageScalarEvent(damage));
		if (eventData.valid()) {
			damage = eventData.get_or<float>("damage", damage);
		}

		return damage;
	}

	const auto TES3_WeatherController_switchWeather = reinterpret_cast<void(__thiscall*)(WeatherController*, int, float)>(0x441C40);
	void WeatherController::switchWeather(int weatherId, float startingTransition) {
		windVelocityCurrWeather = { 0.0f, 0.0f, 0.0f };
		windVelocityNextWeather = { 0.0f, 0.0f, 0.0f };
		sgAshCloud->setAppCulled(true);
		sgBlightCloud->setAppCulled(true);
		sgBlizzard->setAppCulled(true);

		transitionScalar = startingTransition;
		if (startingTransition < 1.0f && currentWeather) {
			if (startingTransition == 0.0f) {
				transition(weatherId);
			}
			else {
				setNextWeather(weatherId);
			}
		}
		else {
			setCurrentWeather(weatherId);
			clearNextWeather();
		}

		unknown_785FAC::set(true);
	}

	const auto TES3_WeatherController_enableSky = reinterpret_cast<void(__thiscall*)(WeatherController*)>(0x440820);
	void WeatherController::enableSky() {
		TES3_WeatherController_enableSky(this);
	}

	const auto TES3_WeatherController_disableSky = reinterpret_cast<void(__thiscall*)(WeatherController*)>(0x440870);
	void WeatherController::disableSky() {
		TES3_WeatherController_disableSky(this);
	}

	void WeatherController::transition(int weatherId) {
		if (transitionScalar == 0.0f) {
			return;
		}

		windVelocityCurrWeather = { 0, 0, 0 };
		windVelocityNextWeather = { 0, 0, 0 };

		if (currentWeather == nullptr) {
			switchWeather(weatherId, 1.0f);
		}

		transitionScalar = 0.01f;
		setNextWeather(weatherId);
	}

	void WeatherController::onInactivateWeather(DataHandler* dataHandler, float gameHour) {
		inactivateWeather = false;
		sgAshCloud->setAppCulled(true);
		sgBlightCloud->setAppCulled(true);
		sgBlizzard->setAppCulled(true);

		// Update blight spells.
		const auto weatherBlight = getWeatherBlight();
		for (const auto spell : *dataHandler->nonDynamicData->spellsList) {
			if (spell->isBlightDisease() && !spell->hasEffect(EffectID::Corprus)) {
				weatherBlight->blightDiseases.push_back(spell);
			}
		}

		if (!lastActiveRegion) {
			// Determine the weather appropriate for the cell.
			const auto playerCell = dataHandler->currentCell;
			if (playerCell->getIsOrBehavesAsExterior()) {
				const auto playerCellRegion = playerCell->getRegion();
				if (playerCellRegion) {
					setCurrentWeather(playerCellRegion->currentWeatherIndex);
					lastActiveRegion = playerCellRegion;
				}
				else {
					setCurrentWeather(WeatherType::Clear);
				}
			}
			else {
				clearCurrentWeather();
			}
		}

		if (!unknown_0x10) {
			unknown_0x10 = true;
			hoursRemaining = 0.0f;
			if (gameHour >= 0.0f) {
				do {
					hoursRemaining += hoursBetweenWeatherChanges;
				} while (hoursRemaining <= gameHour);
			}
			while (hoursRemaining >= 24.0f) {
				hoursRemaining -= 24.0f;
				daysRemaining++;
			}
		}
	}

	float WeatherController::lerpE0() const {
		if (currentWeather && currentWeather->supportsParticleLerp() && nextWeather && nextWeather->supportsParticleLerp()) {
			return mwse::math::lerp(currentWeather->unknown_0xE0, nextWeather->unknown_0xE0, transitionScalar);
		}
		else if (currentWeather && currentWeather->supportsParticleLerp()) {
			return currentWeather->unknown_0xE0;
		}
		else {
			return 0.0f;
		}
	}

	Vector3* WeatherController::lerpE4(Vector3* out_result) const {
		if (currentWeather && currentWeather->supportsParticleLerp() && nextWeather && nextWeather->supportsParticleLerp()) {
			*out_result = currentWeather->unknown_0xE4.lerp(nextWeather->unknown_0xE4, transitionScalar);
			return out_result;
		}
		else if (currentWeather && currentWeather->supportsParticleLerp()) {
			*out_result = currentWeather->unknown_0xE4;
			return out_result;
		}
		else {
			*out_result = { 1.0f, 0.0f, 0.0f };
			return out_result;
		}
	}

	void WeatherController::setCurrentWeather(int weatherId) {
		if (weatherId < WeatherType::MINIMUM || weatherId > WeatherType::MAXIMUM) {
			return;
		}
		if (currentWeather) {
			clearCurrentWeather();
		}

		currentWeather = arrayWeathers[weatherId];

		// Validate cloud texture location.
		const auto buffer = mwse::tes3::getThreadSafeStringBuffer();
		if (mwse::tes3::resolveAssetPath(currentWeather->texturePathCloud, buffer) == 0) {
			mwse::tes3::logAndShowError("Weather %s texture not found.", currentWeather->texturePathCloud);
			sprintf(buffer, "%s\\tx_mooncircle_full_M.tga", "Textures");
		}

		// Set cloud texture.
		auto cloudTexture = NI::SourceTexture::createFromPath(buffer);
		const auto cloudTextureProperty = sgTriCloudsCurrent->getTexturingProperty();
		if (cloudTextureProperty->getBaseMap() == nullptr) {
			const auto baseMap = new NI::TexturingProperty::Map(cloudTexture);
			baseMap->filterMode = NI::TexturingProperty::FilterMode::BILERP;
			cloudTextureProperty->setBaseMap(baseMap);
		}
		cloudTextureProperty->getBaseMap()->texture = cloudTexture;
	}

	void WeatherController::clearCurrentWeather() {
		if (currentWeather == nullptr) {
			return;
		}
		currentWeather->unload();
		currentWeather = nullptr;
	}

	void WeatherController::setNextWeather(int weatherId) {
		if (weatherId < WeatherType::MINIMUM || weatherId > WeatherType::MAXIMUM) {
			return;
		}

		if (nextWeather) {
			clearNextWeather();
		}

		nextWeather = arrayWeathers[weatherId];

		// Validate cloud texture location.
		const auto buffer = mwse::tes3::getThreadSafeStringBuffer();
		if (mwse::tes3::resolveAssetPath(nextWeather->texturePathCloud, buffer) == 0) {
			mwse::tes3::logAndShowError("Weather %s texture not found.", nextWeather->texturePathCloud);
			sprintf(buffer, "%s\\tx_mooncircle_full_M.tga", "Textures");
		}

		// Set cloud texture.
		auto cloudTexture = NI::SourceTexture::createFromPath(buffer);
		const auto cloudTextureProperty = sgTriCloudsNext->getTexturingProperty();
		if (cloudTextureProperty->getBaseMap() == nullptr) {
			const auto baseMap = new NI::TexturingProperty::Map(cloudTexture);
			baseMap->filterMode = NI::TexturingProperty::FilterMode::BILERP;
			cloudTextureProperty->setBaseMap(baseMap);
		}
		cloudTextureProperty->getBaseMap()->texture = cloudTexture;
	}

	void WeatherController::clearNextWeather() {
		if (nextWeather == nullptr) {
			return;
		}
		nextWeather->unload();
		nextWeather = nullptr;
	}

	std::reference_wrapper<Weather* [MAX_WEATHER_COUNT]> WeatherController::getWeathers() {
		return std::ref(arrayWeathers);
	}

	Weather* WeatherController::getWeather(int weatherId) const {
		return arrayWeathers[weatherId];
	}

	WeatherClear* WeatherController::getWeatherClear() const {
		return static_cast<WeatherClear*>(arrayWeathers[WeatherType::Clear]);
	}

	WeatherCloudy* WeatherController::getWeatherCloudy() const {
		return static_cast<WeatherCloudy*>(arrayWeathers[WeatherType::Cloudy]);
	}

	WeatherFoggy* WeatherController::getWeatherFoggy() const {
		return static_cast<WeatherFoggy*>(arrayWeathers[WeatherType::Foggy]);
	}

	WeatherOvercast* WeatherController::getWeatherOvercast() const {
		return static_cast<WeatherOvercast*>(arrayWeathers[WeatherType::Overcast]);
	}

	WeatherRain* WeatherController::getWeatherRain() const {
		return static_cast<WeatherRain*>(arrayWeathers[WeatherType::Rain]);
	}

	WeatherThunder* WeatherController::getWeatherThunder() const {
		return static_cast<WeatherThunder*>(arrayWeathers[WeatherType::Thunder]);
	}

	WeatherAsh* WeatherController::getWeatherAsh() const {
		return static_cast<WeatherAsh*>(arrayWeathers[WeatherType::Ash]);
	}

	WeatherBlight* WeatherController::getWeatherBlight() const {
		return static_cast<WeatherBlight*>(arrayWeathers[WeatherType::Blight]);
	}

	WeatherSnow* WeatherController::getWeatherSnow() const {
		return static_cast<WeatherSnow*>(arrayWeathers[WeatherType::Snow]);
	}

	WeatherBlizzard* WeatherController::getWeatherBlizzard() const {
		return static_cast<WeatherBlizzard*>(arrayWeathers[WeatherType::Blizzard]);
	}

	const auto TES3_WeatherController_setBackgroundToFog = reinterpret_cast<void(__thiscall*)(WeatherController*, NI::Object*)>(0x43EB20);
	const auto TES3_WeatherController_setFogColour = reinterpret_cast<void(__thiscall*)(WeatherController*, NI::Property*)>(0x43EB80);
	const auto TES3_WeatherController_updateAmbient = reinterpret_cast<void(__thiscall*)(WeatherController*, float)>(0x43EF80);
	const auto TES3_WeatherController_updateColours = reinterpret_cast<void(__thiscall*)(WeatherController*, float)>(0x43E000);
	const auto TES3_WeatherController_updateClouds = reinterpret_cast<void(__thiscall*)(WeatherController*, float)>(0x43EC20);
	const auto TES3_WeatherController_updateCloudVertexCols = reinterpret_cast<void(__thiscall*)(WeatherController*)>(0x43EDE0);
	const auto TES3_WeatherController_updateSunCols = reinterpret_cast<void(__thiscall*)(WeatherController*, float)>(0x43F5F0);
	const auto TES3_WeatherController_updateSun = reinterpret_cast<void(__thiscall*)(WeatherController*, float)>(0x43FF80);
	const auto TES3_WeatherController_updateTick = reinterpret_cast<void(__thiscall*)(WeatherController*, NI::Property*, float, bool, float)>(0x440C80);
	void WeatherController::updateVisuals() {
		// Allows weather visuals to update when simulation is paused.
		auto gameHour = WorldController::get()->gvarGameHour->value;
		auto renderer = WorldController::get()->renderer;
		auto fogProperty = static_cast<NI::Property*>(DataHandler::get()->sgFogProperty);

		TES3_WeatherController_updateTick(this, fogProperty, 0.0, true, gameHour);
		TES3_WeatherController_updateClouds(this, 0.0);
		TES3_WeatherController_updateColours(this, gameHour);
		TES3_WeatherController_updateCloudVertexCols(this);
		TES3_WeatherController_updateAmbient(this, gameHour);
		TES3_WeatherController_updateSunCols(this, gameHour);
		TES3_WeatherController_updateSun(this, gameHour);

		// setBackgroundToFog decrements the refCount
		renderer->refCount++;
		TES3_WeatherController_setBackgroundToFog(this, renderer);

		// setFogColour decrements the refCount
		fogProperty->refCount++;
		TES3_WeatherController_setFogColour(this, fogProperty);
	}

	static std::atomic<bool> weatherEventGuard = false;

	void WeatherController::switchImmediate(int weather) {
		if (lastActiveRegion) {
			lastActiveRegion->currentWeatherIndex = weather;
		}
		switchWeather(weather, 1.0f);

		// Fire off the event, after function completes.
		// Prevent recursive triggering of weather change events.
		if (!weatherEventGuard && mwse::lua::event::WeatherChangedImmediateEvent::getEventEnabled()) {
			mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
			const auto stateHandle = luaManager.getThreadSafeStateHandle();

			weatherEventGuard = true;
			stateHandle.triggerEvent(new mwse::lua::event::WeatherChangedImmediateEvent());
			weatherEventGuard = false;
		}
	}

	void WeatherController::switchTransition(int weather) {
		switchWeather(weather, 0.001f);
		if (lastActiveRegion) {
			lastActiveRegion->currentWeatherIndex = weather;
		}

		// Fire off the event after the transition starts.
		// Prevent recursive triggering of weather change events.
		if (!weatherEventGuard && mwse::lua::event::WeatherTransitionStartedEvent::getEventEnabled()) {
			mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
			const auto stateHandle = luaManager.getThreadSafeStateHandle();

			weatherEventGuard = true;
			stateHandle.triggerEvent(new mwse::lua::event::WeatherTransitionStartedEvent());
			weatherEventGuard = false;
		}
	}

	static void __fastcall Patch_WeatherController_SwitchWeather_WithEvent(WeatherController* wc, DWORD _EDX_, int weatherId, float startingTransition) {
		wc->switchWeather(weatherId, startingTransition);

		if (mwse::lua::event::WeatherChangedImmediateEvent::getEventEnabled()) {
			mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
			const auto stateHandle = luaManager.getThreadSafeStateHandle();

			stateHandle.triggerEvent(new mwse::lua::event::WeatherChangedImmediateEvent());
		}
	}

	void WeatherController::installPatches() {
#if defined(MWSE_CUSTOM_WEATHERS) && MWSE_CUSTOM_WEATHERS == TRUE
		// Change size of constructor.
		mwse::writeValueEnforced<DWORD>(0x417EF6 + 0x1, 0x1F0, sizeof(WeatherController));

		// Setup custom weather vtable.
		ZeroMemory(&WeatherCustom::VirtualTable, sizeof(WeatherCustom::VirtualTable));
		auto WeatherCustom_delete = &WeatherCustom::vtbl_delete;
		auto WeatherCustom_simulate = &WeatherCustom::vtbl_simulate;
		auto WeatherCustom_transition = &WeatherCustom::vtbl_transition;
		auto WeatherCustom_unload = &WeatherCustom::vtbl_unload;
		mwse::overrideVirtualTableEnforced(DWORD(&WeatherCustom::VirtualTable), offsetof(Weather_vTable, deleting), 0x0, *reinterpret_cast<DWORD*>(&WeatherCustom_delete));
		mwse::overrideVirtualTableEnforced(DWORD(&WeatherCustom::VirtualTable), offsetof(Weather_vTable, simulate), 0x0, *reinterpret_cast<DWORD*>(&WeatherCustom_simulate));
		mwse::overrideVirtualTableEnforced(DWORD(&WeatherCustom::VirtualTable), offsetof(Weather_vTable, transition), 0x0, *reinterpret_cast<DWORD*>(&WeatherCustom_transition));
		mwse::overrideVirtualTableEnforced(DWORD(&WeatherCustom::VirtualTable), offsetof(Weather_vTable, unload), 0x0, *reinterpret_cast<DWORD*>(&WeatherCustom_unload));

		// Fixup constructor.
		const auto WeatherController_ctor = &WeatherController::ctor;
		mwse::genCallEnforced(0x417F17, DWORD(TES3_WeatherController_ctor), *reinterpret_cast<const DWORD*>(&WeatherController_ctor));

		// Fixup destructor.
		const auto WeatherController_dtor = &WeatherController::dtor;
		mwse::genCallEnforced(0x40E0CB, DWORD(TES3_WeatherController_dtor), *reinterpret_cast<const DWORD*>(&WeatherController_dtor));

		// Replace function: switchWeather
		const auto WeatherController_switchWeather = &WeatherController::switchWeather;
		mwse::genCallEnforced(0x410368, 0x441C40, reinterpret_cast<const DWORD>(Patch_WeatherController_SwitchWeather_WithEvent));
		mwse::genCallEnforced(0x441084, 0x441C40, reinterpret_cast<const DWORD>(Patch_WeatherController_SwitchWeather_WithEvent));
		mwse::genCallEnforced(0x441AA7, 0x441C40, *reinterpret_cast<const DWORD*>(&WeatherController_switchWeather));
		mwse::genCallEnforced(0x45CE2D, 0x441C40, reinterpret_cast<const DWORD>(Patch_WeatherController_SwitchWeather_WithEvent));
		mwse::genCallEnforced(0x45D211, 0x441C40, reinterpret_cast<const DWORD>(Patch_WeatherController_SwitchWeather_WithEvent));
		mwse::genCallEnforced(0x4BE166, 0x441C40, *reinterpret_cast<const DWORD*>(&WeatherController_switchWeather));
		mwse::genCallEnforced(0x4BE19A, 0x441C40, *reinterpret_cast<const DWORD*>(&WeatherController_switchWeather));

		// Replace function: transition
		const auto WeatherController_transition = &WeatherController::transition;
		mwse::genCallEnforced(0x41034D, 0x441A10, *reinterpret_cast<const DWORD*>(&WeatherController_transition));
		mwse::genCallEnforced(0x441D13, 0x441A10, *reinterpret_cast<const DWORD*>(&WeatherController_transition));

		// Replace function: onInactivateWeather
		const auto WeatherController_onInactivateWeather = &WeatherController::onInactivateWeather;
		mwse::genCallEnforced(0x410245, 0x4415E0, *reinterpret_cast<const DWORD*>(&WeatherController_onInactivateWeather));

		// Replace function: lerpE0
		const auto WeatherController_lerpE0 = &WeatherController::lerpE0;
		mwse::genCallEnforced(0x451FF1, 0x442460, *reinterpret_cast<const DWORD*>(&WeatherController_lerpE0));
		mwse::genCallEnforced(0x452890, 0x442460, *reinterpret_cast<const DWORD*>(&WeatherController_lerpE0));

		// Replace function: lerpE4
		const auto WeatherController_lerpE4 = &WeatherController::lerpE4;
		mwse::genCallEnforced(0x451FB8, 0x442350, *reinterpret_cast<const DWORD*>(&WeatherController_lerpE4));
		mwse::genCallEnforced(0x451FCC, 0x442350, *reinterpret_cast<const DWORD*>(&WeatherController_lerpE4));
		mwse::genCallEnforced(0x451FE3, 0x442350, *reinterpret_cast<const DWORD*>(&WeatherController_lerpE4));
		mwse::genCallEnforced(0x45285A, 0x442350, *reinterpret_cast<const DWORD*>(&WeatherController_lerpE4));
		mwse::genCallEnforced(0x45286E, 0x442350, *reinterpret_cast<const DWORD*>(&WeatherController_lerpE4));
		mwse::genCallEnforced(0x452882, 0x442350, *reinterpret_cast<const DWORD*>(&WeatherController_lerpE4));

		// Patch mwscript: ChangeWeather
		mwse::genNOPUnprotected(0x50C361, 0x50C370 - 0x50C361);
		bool (Region::*Region_setCurrentWeather)(int) = &Region::setCurrentWeather;
		mwse::genCallEnforced(0x50C373, 0x4812F0, *reinterpret_cast<const DWORD*>(&Region_setCurrentWeather));
#endif
	}
}
