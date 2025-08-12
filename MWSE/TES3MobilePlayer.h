#pragma once

#include "TES3Defines.h"

#include "TES3MobileNPC.h"
#include "TES3StdString.h"
#include "TES3Vectors.h"

namespace TES3 {
	struct MarkData {
		Vector3 position; // 0x0
		float rotation; // 0xC
		Cell* cell; // 0x10
	};

	struct BountyData {
		int defaultValue;
		StdString tempString; // 0x4
		int* storage; // 0x14
		StdString* keys; // 0x18
		int storageSize; // 0x1C
		int maybe_usedCount; // 0x20

		//
		// Other related this-call functions.
		//

		int getValue(const StdString* crimeType) const;
		void setValue(const StdString* crimeType, int value);

		//
		// Custom functions
		//

		sol::table getKeys_lua(sol::this_state ts) const;
		int getValue_lua(const char* type) const;
		void setValue_lua(const char* type, int value);
		int modValue_lua(const char* type, int delta);
	};

	struct PlayerBounty {
		BountyData* data;
	};

	struct MobilePlayer : MobileNPC {
		int levelupPerAttributeCount[8]; // 0x56C
		int levelupPerSpecialization[3]; // 0x58C
		PlayerBounty* bounty; // 0x598
		Apparatus* lastUsedMortar; // 0x59C
		Apparatus* lastUsedAlembic; // 0x5A0
		Apparatus* lastUsedCalcinator; // 0x5A4
		Apparatus* lastUsedRetort; // 0x5A8
		GlobalVariable* clawMultiplier; // 0x5AC
		bool controlsDisabled; // 0x5B0
		bool jumpingDisabled; // 0x5B1
		bool mouseLookDisabled; // 0x5B2
		bool viewSwitchDisabled; // 0x5B3
		bool vanityDisabled; // 0x5B4
		bool attackDisabled; // 0x5B5
		bool magicDisabled; // 0x5B6
		bool alwaysRun; // 0x5B7
		bool autoRun; // 0x5B8
		bool sleeping; // 0x5B9
		char unknown_0x5BA; // 0x5BA // Resting related?
		bool waiting; // 0x5BB
		char unknown_0x5BC; // 0x5BC
		bool weaponReady; // 0x5BD
		bool castReady; // 0x5BE
		char unknown_0x5BF;
		bool playerInJail; // 0x5C0
		bool playerIsTraveling; // 0x5C1
		char unknown_0x5C2; // Undefined.
		char unknown_0x5C3; // Undefined.
		int telekinesis; // 0x5C4
		float visionBonus; // 0x5C8
		int detectLockRange; // 0x5CC
		int detectEnchantmentRange; // 0x5D0
		int detectCreatureRange; // 0x5D4
		Vector3 restWaitPosition; // 0x5D8
		int postAttackHandRecoilTimer; // 0x5E4
		int levelUpProgress; // 0x5E8
		int restHoursRemaining; // 0x5EC
		int lastUsedAmmoCount; // 0x5F0
		float skillProgress[27]; // 0x5F4
		Reference* firstPersonReference; // 0x660
		NPC* firstPerson; // 0x664
		bool bountySuspendedFlag; // 0x668
		char unknown_0x669;
		char unknown_0x66A; // Undefined.
		char unknown_0x66B; // Undefined.
		void* unknown_0x66C;
		Birthsign* birthsign; // 0x670
		IteratedList<Dialogue*>* dialogueList; // 0x674
		MarkData* markLocation; // 0x678
		Vector3 lastPositionOfFogOfWarUpdate; // 0x67C
		float inactivityTime; // 0x684
		float* humanStatsBackup; // 0x688
		GlobalVariable* knownWerewolf; // 0x690

		MobilePlayer() = delete;
		~MobilePlayer() = delete;

		//
		// Other related this-call functions.
		//

		void exerciseSkill(int skillId, float progress);
		float getSkillProgressRequirement(int skillId) const;
		void progressSkillLevelIfRequirementsMet(int skillId);
		void onDeath();
		int getGold();
		void modGold(int value);
		void wakeUp();

		BountyData* getBountyData() const;
		int getBounty();
		void setBounty(int value);
		void modBounty(int delta);

		bool is3rdPerson();
		float getCameraHeight() const;
		void setCameraHeight_lua(sol::optional<float> height);
		int getVanityState() const;
		void setVanityState(int state);

		void addTopic(Dialogue* topic);

		//
		// Custom functions.
		//

		int getCompanionCount() const;
		std::vector<TES3::MobileActor*> getCompanionList() const;
		float getCompanionMaxDistance() const;

		void updateSceneGraph();

		int progressSkillToNextLevel(int skillId);

		void setMovementFlagSneaking(bool value);

		PlayerAnimationController* getPlayerAnimationController() const;

		std::reference_wrapper<int[8]> getLevelupsPerAttribute();
		std::reference_wrapper<int[3]> getLevelupsPerSpecialization();
		std::reference_wrapper<float[27]> getSkillProgressValues();

	};
	static_assert(sizeof(MobilePlayer) == 0x694, "TES3::MobilePlayer failed size validation");
}
