#include "TES3MobilePlayer.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "LuaDeathEvent.h"
#include "LuaSkillExerciseEvent.h"

#include "Log.h"

#include "TES3NPC.h"
#include "TES3Race.h"
#include "TES3Skill.h"
#include "TES3PlayerAnimationController.h"

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
				if (eventData.get_or("block", false)) {
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

	int MobilePlayer::getGold() {
		return reinterpret_cast<int(__thiscall *)(MobilePlayer*)>(TES3_MobilePlayer_getGoldHeld)(this);
	}

	const auto TES3_MobilePlayer_wakeUp = reinterpret_cast<void(__thiscall*)(MobilePlayer*)>(0x56BBB0);
	void MobilePlayer::wakeUp() {
		TES3_MobilePlayer_wakeUp(this);
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

	bool MobilePlayer::is3rdPerson() {
		return vTable.mobileActor->is3rdPerson(this);
	}

	const auto TES3_data_playerCameraHeightUnscaled = reinterpret_cast<float*>(0x74B284);
	const auto TES3_global_playerCameraHeight = reinterpret_cast<float*>(0x7D1558);

	float MobilePlayer::getCameraHeight() const {
		return *TES3_global_playerCameraHeight;
	}

	void MobilePlayer::setCameraHeight_lua(sol::optional<float> height) {
		if (height) {
			*TES3_global_playerCameraHeight = height.value();
		}
		else {
			// Restore default value.
			auto raceHeights = this->npcInstance->getRace()->height;
			float h = this->npcInstance->isFemale() ? raceHeights.female : raceHeights.male;
			*TES3_global_playerCameraHeight = *TES3_data_playerCameraHeightUnscaled * h;
		}
	}

	const auto TES3_MobilePlayer_getVanityState = reinterpret_cast<int(__thiscall*)(const MobilePlayer*)>(0x567990);
	int MobilePlayer::getVanityState() const {
		return TES3_MobilePlayer_getVanityState(this);
	}

	const auto TES3_MobilePlayer_setVanityState = reinterpret_cast<void(__thiscall*)(MobilePlayer*, int)>(0x567960);
	void MobilePlayer::setVanityState(int state) {
		TES3_MobilePlayer_setVanityState(this, state);
	}

	void MobilePlayer::setMovementFlagSneaking(bool value) {
		MobileActor::setMovementFlagSneaking(value);
		flagForceSneak = value;
	}

	PlayerAnimationController* MobilePlayer::getPlayerAnimationController() const {
		return animationController.asPlayer;
	}

	std::reference_wrapper<int[8]> MobilePlayer::getLevelupsPerAttribute() {
		return std::ref(levelupPerAttributeCount);
	}

	std::reference_wrapper<int[3]> MobilePlayer::getLevelupsPerSpecialization() {
		return std::ref(levelupPerSpecialization);
	}

	std::reference_wrapper<float[27]> MobilePlayer::getSkillProgressValues() {
		return std::ref(skillProgress);
	}

}
