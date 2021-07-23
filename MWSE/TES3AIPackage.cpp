#include "TES3AIPackage.h"

#include "LuaManager.h"
#include "LuaObjectInvalidatedEvent.h"

namespace TES3 {
	void AIPackage::simulate() {
		vTable->simulate(this);
	}

	bool AIPackage::movement() {
		return vTable->movement(this);
	}

	bool AIPackage::initialize() {
		return vTable->initialize(this);
	}

	void AIPackage::cleanup() {
		vTable->cleanup(this);
	}

	MobileActor* AIPackage::getTargetActor() const {
		return targetActor;
	}

	void AIPackage::setTargetActor(TES3::MobileActor* target) {
		targetActor = target;
	}

	const auto TES3_AIPackage_setTargetActorAsFriend = reinterpret_cast<void(__thiscall*)(AIPackage*, MobileActor*)>(0x534D70);
	void AIPackage::setTargetActorAsFriend(TES3::MobileActor* target) {
		TES3_AIPackage_setTargetActorAsFriend(this, target);
	}

	const auto TES3_AIPackage_setTargetActorAsFriendIfActive = reinterpret_cast<void(__thiscall*)(AIPackage*, MobileActor*)>(0x535730);
	void AIPackage::setTargetActorAsFriendIfActive(TES3::MobileActor* target) {
		TES3_AIPackage_setTargetActorAsFriendIfActive(this, target);
	}

	sol::object AIPackage::getOrCreateLuaObject(lua_State* L) const {
		if (this == nullptr) {
			return sol::nil;
		}

		// Make sure we're looking at the main state.
		L = sol::main_thread(L);

		sol::object ref = sol::nil;
		switch (packageType) {
		case TES3::AIPackageType::Wander:
			ref = sol::make_object(L, static_cast<const TES3::AIPackageWander*>(this));
			break;
		case TES3::AIPackageType::Travel:
			ref = sol::make_object(L, static_cast<const TES3::AIPackageTravel*>(this));
			break;
		case TES3::AIPackageType::Escort:
			ref = sol::make_object(L, static_cast<const TES3::AIPackageEscort*>(this));
			break;
		case TES3::AIPackageType::Follow:
			ref = sol::make_object(L, static_cast<const TES3::AIPackageFollow*>(this));
			break;
		case TES3::AIPackageType::Activate:
			ref = sol::make_object(L, static_cast<const TES3::AIPackageActivate*>(this));
			break;
		default:
			ref = sol::make_object_userdata(L, this);
		}

		return ref;;
	}

	AIPackageType AIPackageConfig::toPackageType() const {
		switch (type) {
		case AIPackageConfigType::Travel:
			return AIPackageType::Travel;
		case AIPackageConfigType::Wander:
			return AIPackageType::Wander;
		case AIPackageConfigType::Escort:
			return AIPackageType::Escort;
		case AIPackageConfigType::Follow:
			return AIPackageType::Follow;
		case AIPackageConfigType::Activate:
			return AIPackageType::Activate;
		}
		return AIPackageType::Wander;
	}


	std::reference_wrapper<AIPackageWander::IdleNode[8]> AIPackageWander::getIdles() {
		return idles;
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3_AIPACKAGE(TES3::AIPackage)
