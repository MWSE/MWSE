#pragma once

#include "TES3Defines.h"

#include "TES3IteratedList.h"
#include "TES3StdString.h"
#include "TES3Vectors.h"

namespace TES3 {
	struct CrimeEvent {
		int type; // 0x0
		float timestamp; // 0x4
		StdString typeString; // 0x8
		Vector3 position; // 0x18
		int penalty; // 0x24
		MobileActor * criminal; // 0x28
		MobileActor * victim; // 0x2C
		Faction * victimFaction; // 0x30
		IteratedList<AIPlanner*> * witnesses; // 0x34
		Actor * victimBaseActor;

		CrimeEvent();
		~CrimeEvent();

		CrimeEvent * ctor();
		void dtor();

		void copy(CrimeEvent* crimeEvent);


	};
	static_assert(sizeof(CrimeEvent) == 0x3C, "TES3::CrimeEvent failed size validation");
}
