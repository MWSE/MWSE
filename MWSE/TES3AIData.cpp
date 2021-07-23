#include "TES3AIData.h"

namespace TES3 {
	const auto TES3_AIPlanner_getActivePackage = reinterpret_cast<AIPackage *(__thiscall*)(AIPlanner*)>(0x564DF0);
	AIPackage * AIPlanner::getActivePackage() {
		return TES3_AIPlanner_getActivePackage(this);
	}

	const auto TES3_AIPlanner_allowRestingNear = reinterpret_cast<bool(__thiscall*)(AIPlanner*)>(0x564FA0);
	bool AIPlanner::allowRestingNear() {
		return TES3_AIPlanner_allowRestingNear(this);
	}

	const auto TES3_AIPlanner_assignMobileActor = reinterpret_cast<void(__thiscall*)(AIPlanner*, MobileActor*)>(0x564E40);
	void AIPlanner::assignMobileActor(MobileActor* mobile) {
		TES3_AIPlanner_assignMobileActor(this, mobile);
	}

	std::reference_wrapper<AIPackage* [32]> AIPlanner::getPackages() {
		return std::ref(packages);
	}
}