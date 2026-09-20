#include "TES3Light.h"

#include "MWSEConfig.h"
#include "TES3Util.h"

#include "TES3Inventory.h"
#include "TES3ItemData.h"
#include "TES3Reference.h"
#include "TES3WorldController.h"

#include "NILight.h"

namespace TES3 {
	const auto TES3_Light_ctor = reinterpret_cast<void(__thiscall*)(Light*)>(0x4D1BA0);
	Light::Light() {
		TES3_Light_ctor(this);
	}

	const auto TES3_Light_dtor = reinterpret_cast<void(__thiscall*)(Light*)>(0x4D1C90);
	Light::~Light() {
		TES3_Light_dtor(this);
	}

	const auto TES3_EntityLight_setupInternalLight = reinterpret_cast<void(__thiscall*)(const Light*, MobileObject*)>(0x4D3580);
	void Light::setupLightForMobile(MobileObject* mobile) const {
		TES3_EntityLight_setupInternalLight(this, mobile);
	}

	const auto TES3_EntityLight_updateFlickerPulse = reinterpret_cast<bool(__thiscall*)(const Light*, NI::Pointer<NI::Light>, float*, const ItemData*)>(0x4D33D0);
	bool Light::updateFlickerPulse(NI::Light* sgLight, float* flickerPhase, const ItemData* itemData) const {
		return TES3_EntityLight_updateFlickerPulse(this, sgLight, flickerPhase, itemData);
	}

	bool Light::updateFlickerPulseEased(NI::Light* sgLight, float* flickerPhase, const ItemData* itemData) const {
		const auto flickers = getFlickers() || getFlickersSlowly();
		const auto pulses = getPulses() || getPulsesSlowly();
		const auto burntOut = itemData && std::fabs(itemData->timeLeft) < 0.001f;
		if (!flickers && !pulses && !burntOut) {
			return false;
		}

		// The dimmer eases towards its target, and a new target is picked once it is this close.
		const auto isSlow = getFlickersSlowly() || getPulsesSlowly();
		const auto threshold = isSlow ? 0.05f : 0.1f;

		// Easing rates per second, for the pace the original has at 15 and at 30 FPS.
		struct EasingRate {
			float at15FPS;
			float at30FPS;
		};
		constexpr EasingRate easingRates[2][2] = {
			// Normal,        slow.
			{ { 6.3f, 7.5f }, { 4.9f, 5.2f } }, // Flicker.
			{ { 4.7f, 4.7f }, { 2.8f, 2.9f } }, // Pulse, and burnt out lights with neither flag.
		};
		const auto& easingRate = easingRates[flickers ? 0 : 1][isSlow ? 1 : 0];
		const auto rate = mwse::Configuration::LightFlickerReferenceFPS >= 30 ? easingRate.at30FPS : easingRate.at15FPS;

		// Advance the easing across the frame, picking new targets as they are reached.
		auto dimmer = sgLight->getDimmer();
		auto remainingTime = WorldController::get()->deltaTime;
		auto expired = false;
		for (auto i = 0; i < 16; ++i) {
			const auto target = *flickerPhase;
			const auto gap = target - dimmer;
			const auto distance = std::fabs(gap);
			if (distance > threshold) {
				const auto timeToThreshold = std::log(distance / threshold) / rate;
				if (timeToThreshold > remainingTime) {
					dimmer = target - gap * std::exp(-rate * remainingTime);
					break;
				}
				dimmer = target - std::copysign(threshold, gap);
				remainingTime -= timeToThreshold;
			}

			if (burntOut) {
				expired = true;
				break;
			}

			if (flickers) {
				*flickerPhase = 0.25f + 0.01f * (mwse::tes3::rand() % 75);
			}
			else {
				*flickerPhase = target <= 0.5f ? 1.0f : 0.25f;
			}
		}

		sgLight->setDimmer(dimmer);
		return expired;
	}

	bool Light::getIsDynamic() const {
		return (flags & LightFlags::Dynamic);
	}

	void Light::setIsDynamic(bool value) {
		if (value) {
			flags |= LightFlags::Dynamic;
		}
		else {
			flags &= ~LightFlags::Dynamic;
		}
	}

	bool Light::getCanCarry() const {
		return (flags & LightFlags::CanCarry);
	}

	void Light::setCanCarry(bool value) {
		if (value) {
			flags |= LightFlags::CanCarry;
		}
		else {
			flags &= ~LightFlags::CanCarry;
		}
	}

	bool Light::getIsNegative() const {
		return (flags & LightFlags::Negative);
	}

	void Light::setIsNegative(bool value) {
		if (value) {
			flags |= LightFlags::Negative;
		}
		else {
			flags &= ~LightFlags::Negative;
		}
	}

	bool Light::getFlickers() const {
		return (flags & LightFlags::Flicker);
	}

	void Light::setFlickers(bool value) {
		if (value) {
			flags |= LightFlags::Flicker;
		}
		else {
			flags &= ~LightFlags::Flicker;
		}
	}

	bool Light::getIsFire() const {
		return (flags & LightFlags::Fire);
	}

	void Light::setIsFire(bool value) {
		if (value) {
			flags |= LightFlags::Fire;
		}
		else {
			flags &= ~LightFlags::Fire;
		}
	}

	bool Light::getIsOffByDefault() const {
		return (flags & LightFlags::OffByDefault);
	}

	void Light::setIsOffByDefault(bool value) {
		if (value) {
			flags |= LightFlags::OffByDefault;
		}
		else {
			flags &= ~LightFlags::OffByDefault;
		}
	}

	bool Light::getFlickersSlowly() const {
		return (flags & LightFlags::FlickerSlow);
	}

	void Light::setFlickersSlowly(bool value) {
		if (value) {
			flags |= LightFlags::FlickerSlow;
		}
		else {
			flags &= ~LightFlags::FlickerSlow;
		}
	}

	bool Light::getPulses() const {
		return (flags & LightFlags::Pulse);
	}

	void Light::setPulses(bool value) {
		if (value) {
			flags |= LightFlags::Pulse;
		}
		else {
			flags &= ~LightFlags::Pulse;
		}
	}

	bool Light::getPulsesSlowly() const {
		return (flags & LightFlags::PulseSlow);
	}

	void Light::setPulsesSlowly(bool value) {
		if (value) {
			flags |= LightFlags::PulseSlow;
		}
		else {
			flags &= ~LightFlags::PulseSlow;
		}
	}

	void Light::setIconPath(const char* path) {
		if (strnlen_s(path, 32) >= 32) {
			throw std::invalid_argument("Path must not be 32 or more characters.");
		}
		mwse::tes3::setDataString(&icon, path);
	}

	std::reference_wrapper<unsigned char[4]> Light::getColor() {
		return std::ref(color);
	}

	sol::optional<float> Light::getTimeLeft_lua(sol::object object) const {
		if (object.is<EquipmentStack>()) {
			auto stack = object.as<EquipmentStack*>();
			if (stack->object == this) {
				return (stack->itemData) ? stack->itemData->timeLeft : float(time);
			}
		}
		else if (object.is<Reference>()) {
			auto reference = object.as<Reference*>();
			if (reference->baseObject == this) {
				auto itemData = reference->getAttachedItemData();
				return (itemData) ? itemData->timeLeft : float(time);
			}
		}
		else if (object.is<ItemData>()) {
			return object.as<ItemData*>()->timeLeft;
		}
		return float(time);
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::Light)
