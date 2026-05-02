#include "TES3Light.h"

#include "TES3Util.h"

#include "TES3Inventory.h"
#include "TES3ItemData.h"
#include "TES3Reference.h"

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
