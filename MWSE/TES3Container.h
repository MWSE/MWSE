#pragma once

#include "TES3ContainerFlags.h"
#include "TES3Defines.h"

#include "TES3Actor.h"

namespace TES3 {
	struct ContainerBase : Actor {
		static constexpr auto OBJECT_TYPE = ObjectType::Container;

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

		ContainerInstance() = delete;
		~ContainerInstance() = delete;

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
