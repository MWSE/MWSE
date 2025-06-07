#include "TES3MobilePlayer.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3UIManager.h"

#include "LuaCameraControlEvent.h"
#include "LuaDeathEvent.h"
#include "LuaSkillExerciseEvent.h"

#include "Log.h"

#include "TES3AIData.h"
#include "TES3AIPackage.h"
#include "TES3NPC.h"
#include "TES3PlayerAnimationController.h"
#include "TES3Race.h"
#include "TES3Reference.h"
#include "TES3Skill.h"
#include "TES3WorldController.h"

namespace TES3 {
	const auto TES3_BountyData_getValue = reinterpret_cast<int(__thiscall*)(const BountyData*, const StdString*)>(0x55D220);
	int BountyData::getValue(const StdString* crimeType) const {
		return TES3_BountyData_getValue(this, crimeType);
	}

	const auto TES3_BountyData_setValue = reinterpret_cast<int(__thiscall*)(BountyData*, const StdString*, int)>(0x55D300);
	void BountyData::setValue(const StdString* crimeType, int value) {
		TES3_BountyData_setValue(this, crimeType, value);
	}

	sol::table BountyData::getKeys_lua(sol::this_state ts) const {
		sol::state_view state = ts;
		auto results = state.create_table();

		for (auto i = 0; i < storageSize; ++i) {
			results[i+1] = keys[i].c_str;
		}

		return results;
	}

	int BountyData::getValue_lua(const char* type) const {
		StdString indirect = type;
		return getValue(&indirect);
	}

	void BountyData::setValue_lua(const char* type, int value) {
		StdString indirect = type;
		setValue(&indirect, value);
	}

	int BountyData::modValue_lua(const char* type, int delta) {
		StdString indirect = type;
		const auto newValue = getValue(&indirect) + delta;
		setValue(&indirect, newValue);
		return newValue;
	}

	const auto TES3_MobilePlayer_exerciseSkill = reinterpret_cast<void(__thiscall*)(MobilePlayer*, int, float)>(0x56A5D0);
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

		TES3_MobilePlayer_exerciseSkill(this, skillId, progress);
	}

	const auto TES3_MobilePlayer_getSkillProgressRequirement = reinterpret_cast<float(__thiscall*)(const MobilePlayer*, int)>(0x56A520);
	float MobilePlayer::getSkillProgressRequirement(int skillId) const {
		return TES3_MobilePlayer_getSkillProgressRequirement(this, skillId);
	}

	const auto TES3_MobilePlayer_progressSkillLevelIfRequirementsMet = reinterpret_cast<float(__thiscall*)(const MobilePlayer*, int)>(0x56BBE0);
	void MobilePlayer::progressSkillLevelIfRequirementsMet(int skillId) {
		TES3_MobilePlayer_progressSkillLevelIfRequirementsMet(this, skillId);
	}

	const auto TES3_MobilePlayer_onDeath = reinterpret_cast<void(__thiscall*)(MobileActor*)>(0x56A120);
	void MobilePlayer::onDeath() {
		TES3_MobilePlayer_onDeath(this);

		// Trigger death event.
		if (mwse::lua::event::DeathEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::DeathEvent(this));
		}
	}

	const auto TES3_MobilePlayer_getGoldHeld = reinterpret_cast<int(__thiscall*)(MobilePlayer*)>(0x52B450);
	int MobilePlayer::getGold() {
		return TES3_MobilePlayer_getGoldHeld(this);
	}

	const auto TES3_MobilePlayer_modGoldHeld = reinterpret_cast<void(__thiscall*)(MobilePlayer*, int)>(0x52B480);
	void MobilePlayer::modGold(int value) {
		TES3_MobilePlayer_modGoldHeld(this, value);
	}

	const auto TES3_MobilePlayer_wakeUp = reinterpret_cast<void(__thiscall*)(MobilePlayer*)>(0x56BBB0);
	void MobilePlayer::wakeUp() {
		TES3_MobilePlayer_wakeUp(this);
	}

	BountyData* MobilePlayer::getBountyData() const {
		if (bounty) {
			return bounty->data;
		}
		return nullptr;
	}

	const auto TES3_MobilePlayer_getBounty = reinterpret_cast<int(__thiscall*)(MobilePlayer*)>(0x5688B0);
	int MobilePlayer::getBounty() {
		return TES3_MobilePlayer_getBounty(this);
	}

	const auto TES3_MobilePlayer_setBounty = reinterpret_cast<void(__thiscall*)(MobilePlayer*, int)>(0x5688D0);
	void MobilePlayer::setBounty(int value) {
		TES3_MobilePlayer_setBounty(this, value);
	}

	const auto TES3_MobilePlayer_modBounty = reinterpret_cast<void(__thiscall*)(MobilePlayer*, int)>(0x5688F0);
	void MobilePlayer::modBounty(int delta) {
		TES3_MobilePlayer_modBounty(this, delta);
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

	int MobilePlayer::getCompanionCount() const {
		int count = 0;
		const auto maxDistance = getCompanionMaxDistance();

		for (const auto& mobile : listFriendlyActors) {
			if (mobile == this) {
				continue;
			}

			if (!mobile->getFlagActiveAI()) {
				continue;
			}

			if (mobile->reference == nullptr || mobile->reference->getDisabled() || mobile->reference->getDeleted()) {
				continue;
			}

			const auto activePackage = mobile->aiPlanner->getActivePackage();
			if (activePackage == nullptr) {
				continue;
			}

			if (activePackage->packageType != AIPackageType::Escort) {
				continue;
			}

			const auto distanceToPlayer = getPosition()->distance(mobile->getPosition());
			if (distanceToPlayer > maxDistance) {
				continue;
			}

			count++;
		}

		return count;
	}

	std::vector<TES3::MobileActor*> MobilePlayer::getCompanionList() const {
		std::vector<TES3::MobileActor*> results = {};
		const auto maxDistance = getCompanionMaxDistance();

		for (const auto& mobile : listFriendlyActors) {
			if (mobile == this) {
				continue;
			}

			if (!mobile->getFlagActiveAI()) {
				continue;
			}

			if (mobile->reference == nullptr || mobile->reference->getDisabled() || mobile->reference->getDeleted()) {
				continue;
			}

			const auto activePackage = mobile->aiPlanner->getActivePackage();
			if (activePackage == nullptr) {
				continue;
			}

			if (activePackage->packageType != AIPackageType::Escort) {
				continue;
			}

			const auto distanceToPlayer = getPosition()->distance(mobile->getPosition());
			if (distanceToPlayer > maxDistance) {
				continue;
			}

			results.push_back(mobile);
		}

		return std::move(results);
	}

	float MobilePlayer::getCompanionMaxDistance() const {
		constexpr auto BASE_DISTANCE = 512.0f;
		constexpr auto EXTENSION_PER_FRIENDLY = 128.0f;
		return BASE_DISTANCE + listFriendlyActors.size() * EXTENSION_PER_FRIENDLY;
	}

	static Vector3 lastPlayerPosition;
	static int cameraResetFrames = 0;

	const auto TES3_MobilePlayer_updateScenegraph = reinterpret_cast<void(__thiscall*)(MobilePlayer*)>(0x5679E0);
	void MobilePlayer::updateSceneGraph() {
		auto worldController = WorldController::get();
		auto cameraRootNode = worldController->worldCamera.cameraRoot;
		auto armCameraRootNode = worldController->armCamera.cameraRoot;

		// Call the original function.
		TES3_MobilePlayer_updateScenegraph(this);

		if (mwse::lua::event::CameraControlEvent::getEventEnabled()) {
			Transform cameraTransform = cameraRootNode->getTransforms();
			Transform armCameraTransform = armCameraRootNode->getTransforms();
			Transform prevCameraTransform = PlayerAnimationController::previousCameraTransform;
			Transform prevArmCameraTransform = PlayerAnimationController::previousArmCameraTransform;

			// Copy current transform over previous if the player teleported a significant distance.
			// This minimizes glitches on cell changes and other teleport situations.
			const float distanceLimit = 1024.0f;
			float distanceMoved = lastPlayerPosition.distance(&reference->position);
			lastPlayerPosition = reference->position;

			if (distanceMoved > distanceLimit) {
				// Multiple reset frames are needed for player ground clamping physics to resolve.
				cameraResetFrames = 3;
			}
			if (cameraResetFrames > 0) {
				prevCameraTransform = cameraTransform;
				prevArmCameraTransform = armCameraTransform;
				--cameraResetFrames;
			}

			// Fire off our event.
			auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			sol::object eventResult = stateHandle.triggerEvent(
				new mwse::lua::event::CameraControlEvent(
					animationController.asPlayer,
					prevCameraTransform, prevArmCameraTransform,
					cameraTransform, armCameraTransform
				)
			);

			if (eventResult.valid()) {
				sol::table eventData = eventResult;
				cameraTransform = eventData["cameraTransform"];
				armCameraTransform = eventData["armCameraTransform"];

				// Copy transforms in the same way as PlayerAnimationController::syncRotation and updateCameraTransforms.
				cameraRootNode->copyTransforms(&cameraTransform);
				worldController->shadowCamera.cameraRoot->copyTransforms(&cameraTransform);
				armCameraRootNode->localTranslate = armCameraTransform.translation;
				if (isNotKnockedDownOrOut()) {
					armCameraRootNode->setLocalRotationMatrix(&armCameraTransform.rotation);
				}
			}
		}
	}

	const auto TES3_MobilePlayer_addTopic = reinterpret_cast<void(__thiscall*)(MobilePlayer*, Dialogue*)>(0x56A470);
	void MobilePlayer::addTopic(Dialogue* topic) {
		TES3_MobilePlayer_addTopic(this, topic);
	}

	int MobilePlayer::progressSkillToNextLevel(int skillId) {
		auto progressNeeded = getSkillProgressRequirement(skillId);
		skillProgress[skillId] = progressNeeded;
		progressSkillLevelIfRequirementsMet(skillId);
		TES3::UI::updateStatsPane();
		return int(skills[skillId].base);
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
