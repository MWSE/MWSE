#pragma once

#include "TES3Defines.h"

#include "TES3IteratedList.h"
#include "TES3StdString.h"
#include "TES3Vectors.h"

namespace TES3 {
	namespace CrimeType {
		enum CrimeType {
			Attack = 1, // Attacked NPCs or creatures
			Killing = 2, // Killed NPCs or creatures
			WitnessReaction = 3, // Special state that is used internally for determining bounty and the victim's reaction
			Pickpocket = 4, // Pickpocketed NPCs or creatures
			Theft = 5, // Picked up owned objects
			Trespass = 6, // Lockpicked, probed, unlocked or tried to sleep in owned objects 
			Werewolf = 7 // Detected while transforming into a werewolf
		};
	}

	struct CrimeEvent {
		// The type of the crime. Maps to TES3::CrimeType.
		int type; // 0x0

		// The time at which the crime has been committed. This is not the time at which the crime has been witnessed.
		float timestamp; // 0x4

		// Contains the stringified version of the type used for bounty-related functions.
		// This will not be in sync with the actual type after transitioning to the TES3::CrimeType::WitnessReaction step and will retain the original stringified type.
		StdString bountyKey; // 0x8

		// The position of the crime. This will be the position of the victim of an attack/killing or the position of the criminal otherwise.
		Vector3 position; // 0x18

		// Is only valid for crimes of type TES3::CrimeType::Theft or TES3::CrimeType::Pickpocket.
		int stolenValue; // 0x24

		// The player mobile will be used as a default value, meaning that there is no direct victim.
		// Other values may be possible but will generally not be reacted to by AI and discarded.
		// For crimes of type TES3::CrimeType::WitnessReaction the criminal will always be the witness,
		// unless it is reacting to a crime of type TES3::CrimeType::Killing, in which case the criminal mobile will be used.
		MobileActor* criminal; // 0x28

		// The player mobile will be used as a default value, meaning that there is no victim.
		// If the default value is set, the proper victim can be retrieved from the stolenFrom field for crimes of type TES3::CrimeType::Theft or TES3::CrimeType::Pickpocket.
		// For crimes of type TES3::CrimeType::WitnessReaction the victim will always be the player mobile.
		MobileActor* victim; // 0x2C

		// The faction of the crime victim. This field will always be filled with the correct information,
		// even if player mobile has been set as a default value for victim.
		Faction* victimFaction; // 0x30

		// Contains a list of AIPlanners that were able to detect the criminal through ProcessManager::checkAlarmRadius.
		IteratedList<AIPlanner*>* witnesses; // 0x34

		// Is only valid for crimes of type TES3::CrimeType::Theft or TES3::CrimeType::Pickpocket.
		Actor* stolenFrom;

		CrimeEvent();
		CrimeEvent(const CrimeEvent& event);
		~CrimeEvent();

		void operator=(const CrimeEvent& event);

		CrimeEvent * ctor();
		void dtor();
		void copy(const CrimeEvent* crimeEvent);

		const char* getBountyKey() const;
		void setBountyKey(const char* key);
	};
	static_assert(sizeof(CrimeEvent) == 0x3C, "TES3::CrimeEvent failed size validation");
}
