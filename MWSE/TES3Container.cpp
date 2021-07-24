#include "TES3Container.h"

#include "BitUtil.h"

namespace TES3 {
	//
	// ContainerBase
	//

	bool ContainerBase::getIsOrganic() const {
		return BITMASK_TEST(actorFlags, TES3::ActorFlagContainer::Organic);
	}

	void ContainerBase::setIsOrganic(bool value) {
		BITMASK_SET(actorFlags, TES3::ActorFlagContainer::Organic, value);
	}

	bool ContainerBase::getRespawns() const {
		return BITMASK_TEST(actorFlags, TES3::ActorFlagContainer::Respawns);
	}

	void ContainerBase::setRespawns(bool value) {
		BITMASK_SET(actorFlags, TES3::ActorFlagContainer::Respawns, value);
	}

	//
	// Container
	//

	static const auto TES3_Container_ctor = reinterpret_cast<void(__thiscall*)(Container*)>(0x4A3BE0);
	Container::Container() {
		TES3_Container_ctor(this);
	}

	static const auto TES3_Container_dtor = reinterpret_cast<void(__thiscall*)(Container*)>(0x4A3CA0);
	Container::~Container() {
		TES3_Container_dtor(this);
	}

	//
	// ContainerInstance
	//

	const auto TES3_ContainerInstance_onCloseInventory = reinterpret_cast<void (__thiscall*)(Actor *, Reference *, int)>(0x4A4460);
	void ContainerInstance::onCloseInventory(Reference* reference, int unknown) {
		Actor::onCloseInventory(reference, unknown);
		TES3_ContainerInstance_onCloseInventory(this, reference, unknown);
	}

	float ContainerInstance::getCapacity() {
		return container->capacity;
	}

	void ContainerInstance::setCapacity(float value) {
		container->capacity = value;
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::ContainerBase)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::Container)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::ContainerInstance)
