#include "TES3MobilePlayer.h"

#include "sol.hpp"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "LuaDeathEvent.h"
#include "LuaSkillExerciseEvent.h"
#include "LuaIsMobilePlayerUnderwaterEvent.h"

#include "Log.h"

#include "TES3Skill.h"
#include "TES3PlayerAnimationData.h"

#define TES3_MobilePlayer_exerciseSkill 0x56A5D0
#define TES3_MobilePlayer_levelSkill 0x56BBE0
#define TES3_MobilePlayer_getGoldHeld 0x52B450
#define TES3_MobilePlayer_onDeath 0x56A120
#define TES3_MobilePlayer_getBounty 0x5688B0
#define TES3_MobilePlayer_setBounty 0x5688D0
#define TES3_MobilePlayer_modBounty 0x5688F0

namespace TES3 {
	void MobilePlayer::exerciseSkill(int skillId, float progress) {
		// Invoke our exercise skill event and allow skill blocking.
		if (mwse::lua::event::SkillExerciseEvent::getEventEnabled()) {
			auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			sol::table eventData = stateHandle.triggerEvent(new mwse::lua::event::SkillExerciseEvent(skillId, progress));
			if (eventData.valid()) {
				if (eventData["block"] == true) {
					return;
				}

				skillId = eventData["skill"];
				progress = eventData["progress"];

				if (skillId < SkillID::FirstSkill || skillId > SkillID::LastSkill) {
					mwse::log::getLog() << "Error: Attempted to exercise skill with id of " << skillId << "." << std::endl;
					return;
				}
			}
		}

		reinterpret_cast<void(__thiscall *)(MobilePlayer*, int, float)>(TES3_MobilePlayer_exerciseSkill)(this, skillId, progress);
	}

	void MobilePlayer::levelSkill(int skillId) {
		reinterpret_cast<void(__thiscall *)(MobilePlayer*, int)>(TES3_MobilePlayer_levelSkill)(this, skillId);
	}

	void MobilePlayer::onDeath() {
		reinterpret_cast<void(__thiscall *)(MobileActor*)>(TES3_MobilePlayer_onDeath)(this);

		// Trigger death event.
		if (mwse::lua::event::DeathEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::DeathEvent(this));
		}
	}

	bool MobilePlayer::is3rdPerson() {
		return vTable.mobileActor->is3rdPerson(this);
	}

	const auto TES3_MobilePlayer_isUnderwater = reinterpret_cast<bool(__thiscall*)(MobilePlayer*)>(0x5299F0);
	bool MobilePlayer::isUnderwater() {
		bool isUnderwater = TES3_MobilePlayer_isUnderwater(this);

		// Trigger isMobilePlayerUnderwater event.
		mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
		auto stateHandle = luaManager.getThreadSafeStateHandle();
		sol::table eventData = stateHandle.triggerEvent(new mwse::lua::event::IsMobilePlayerUnderwaterEvent(this, isUnderwater));
		if (eventData.valid()) {
			isUnderwater = eventData.get<bool>("isUnderwater");
		}

		return isUnderwater;
	}

	int MobilePlayer::getGold() {
		return reinterpret_cast<int(__thiscall *)(MobilePlayer*)>(TES3_MobilePlayer_getGoldHeld)(this);
	}

	int MobilePlayer::getBounty() {
		return reinterpret_cast<int(__thiscall *)(MobilePlayer*)>(TES3_MobilePlayer_getBounty)(this);
	}

	void MobilePlayer::setBounty(int value) {
		reinterpret_cast<void(__thiscall *)(MobilePlayer*, int)>(TES3_MobilePlayer_setBounty)(this, value);
	}

	void MobilePlayer::modBounty(int delta) {
		reinterpret_cast<void(__thiscall *)(MobilePlayer*, int)>(TES3_MobilePlayer_modBounty)(this, delta);
	}
}
