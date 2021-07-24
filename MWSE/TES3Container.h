#pragma once

#include "TES3Defines.h"

#include "TES3Actor.h"

namespace TES3 {
	namespace ActorFlagContainer {
		typedef unsigned int value_type;

		enum Flag : value_type {
			Organic = 0x1,
			Respawns = 0x2,
			IsBase = 0x8,
		};

		enum FlagBig {
			OrganicBit = 0,
			RespawnsBit = 1,
			IsBaseBit = 2,
		};
	}

	struct ContainerBase : Actor {
		//
		// Custom functions.
		//

		bool getIsOrganic() const;
		void setIsOrganic(bool value);

		bool getRespawns() const;
		void setRespawns(bool value);
	};

	struct Container : ContainerBase {
		char * name;
		char * model;
		Script * script;
		float capacity;

		Container();
		~Container();

	};
	static_assert(sizeof(Container) == 0x7C, "TES3::Container failed size validation");

	struct ContainerInstance : ContainerBase {
		Container * container;

		//
		// Custom functions.
		//

		void onCloseInventory(Reference* reference, int unknown = 0);
		float getCapacity();
		void setCapacity(float);

	};
	static_assert(sizeof(ContainerInstance) == 0x70, "TES3::ContainerInstance failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::ContainerBase)
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::Container)
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::ContainerInstance)
