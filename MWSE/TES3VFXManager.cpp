#include "TES3VFXManager.h"

#include "TES3MagicInstanceController.h"
#include "TES3WorldController.h"

#include "LuaManager.h"
#include "LuaVFXCreatedEvent.h"

namespace TES3 {

	//
	// VFX
	//

	MagicSourceInstance* VFX::getSourceInstance() const {
		if (sourceInstanceSerial == 0) {
			return nullptr;
		}
		return TES3::WorldController::get()->magicInstanceController->getInstanceFromSerial(sourceInstanceSerial);
	}


	//
	// VFX Manager
	//

	const auto TES3_VFXManager_createForMagicEffect = reinterpret_cast<VFX * (__thiscall*)(VFXManager*, int, Reference*, float)>(0x4696A0);
	VFX* VFXManager::createForMagicEffect(int magicEffectID, Reference* reference, float lifespan) {
		auto vfx = TES3_VFXManager_createForMagicEffect(this, magicEffectID, reference, lifespan);
		if (vfx && mwse::lua::event::VFXCreatedEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::VFXCreatedEvent(vfx, "magicEffect"));
		}
		return vfx;
	}

	const auto TES3_VFXManager_createForSource = reinterpret_cast<VFX * (__thiscall*)(VFXManager*, MagicSourceInstance*, Reference*, float)>(0x469570);
	VFX* VFXManager::createForSource(MagicSourceInstance* source, Reference* reference, float lifespan) {
		auto vfx = TES3_VFXManager_createForSource(this, source, reference, lifespan);
		if (vfx && mwse::lua::event::VFXCreatedEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::VFXCreatedEvent(vfx, "magicSourceInstance"));
		}
		return vfx;
	}

	const auto TES3_VFXManager_createForEffect = reinterpret_cast<VFX * (__thiscall*)(VFXManager*, unsigned int, PhysicalObject*, int, float, float, float)>(0x468940);
	VFX* VFXManager::createForEffect(unsigned int serial, PhysicalObject* effect, int animLoopCount, float lifespan, float scale, float verticalOffset) {
		auto vfx = TES3_VFXManager_createForEffect(this, serial, effect, animLoopCount, lifespan, scale, verticalOffset);
		if (vfx && mwse::lua::event::VFXCreatedEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::VFXCreatedEvent(vfx, "effect"));
		}
		return vfx;
	}

	const auto TES3_VFXManager_createForReference = reinterpret_cast<VFX * (__thiscall*)(VFXManager*, unsigned int, PhysicalObject*, Reference*, int, float, float, float)>(0x468370);
	VFX* VFXManager::createForReference(unsigned int serial, PhysicalObject* effect, Reference* reference, int animLoopCount, float lifespan, float scale, float verticalOffset) {
		auto vfx = TES3_VFXManager_createForReference(this, serial, effect, reference, animLoopCount, lifespan, scale, verticalOffset);
		if (vfx && mwse::lua::event::VFXCreatedEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::VFXCreatedEvent(vfx, "reference"));
		}
		return vfx;
	}

	const auto TES3_VFXManager_createAtPosition = reinterpret_cast<VFX * (__thiscall*)(VFXManager*, unsigned int, PhysicalObject*, NI::Point3*, int, float, float, float)>(0x468470);
	VFX* VFXManager::createAtPosition(unsigned int serial, PhysicalObject* effect, NI::Point3* position, int animLoopCount, float lifespan, float scale, float verticalOffset) {
		auto vfx = TES3_VFXManager_createAtPosition(this, serial, effect, position, animLoopCount, lifespan, scale, verticalOffset);
		if (vfx && mwse::lua::event::VFXCreatedEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::VFXCreatedEvent(vfx, "position"));
		}
		return vfx;
	}

	const auto TES3_VFXManager_createForAVObject = reinterpret_cast<VFX * (__thiscall*)(VFXManager*, unsigned int, PhysicalObject*, NI::AVObject*, int, float, float, float)>(0x468560);
	VFX* VFXManager::createForAVObject(unsigned int serial, PhysicalObject* effect, NI::AVObject* avObject, int animLoopCount, float lifespan, float scale, float verticalOffset) {
		auto vfx = TES3_VFXManager_createForAVObject(this, serial, effect, avObject, animLoopCount, lifespan, scale, verticalOffset);
		if (vfx && mwse::lua::event::VFXCreatedEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::VFXCreatedEvent(vfx, "niObject"));
		}
		return vfx;
	}

	const auto TES3_VFXManager_remove = reinterpret_cast<void(__thiscall*)(VFXManager*, VFX*)>(0x468D00);
	void VFXManager::remove(VFX* vfx) {
		TES3_VFXManager_remove(this, vfx);
	}

	const auto TES3_VFXManager_removeForReference = reinterpret_cast<void(__thiscall*)(VFXManager*, Reference*)>(0x468EA0);
	void VFXManager::removeForReference(Reference* reference) {
		TES3_VFXManager_removeForReference(this, reference);
	}

	const auto TES3_VFXManager_removeFromAVObject = reinterpret_cast<void(__thiscall*)(VFXManager*, NI::AVObject*)>(0x469040);
	void VFXManager::removeFromAVObject(NI::AVObject* avObject) {
		TES3_VFXManager_removeFromAVObject(this, avObject);
	}

	const auto TES3_VFXManager_removeForSerialForReference = reinterpret_cast<void(__thiscall*)(VFXManager*, unsigned int, Reference*)>(0x4691E0);
	void VFXManager::removeForSerialForReference(unsigned int serial, Reference* reference) {
		TES3_VFXManager_removeForSerialForReference(this, serial, reference);
	}

	void VFXManager::reset() {
		// For all vfx, set maxAge to a value that will definitely be cleared.
		for (auto vfx : vfxNodes) {
			vfx->maxAge = 1.0;
		}
		resetOnNewScene();
	}

	const auto TES3_VFXManager_resetOnNewScene = reinterpret_cast<void(__thiscall*)(VFXManager*)>(0x469390);
	void VFXManager::resetOnNewScene() {
		TES3_VFXManager_resetOnNewScene(this);
	}
}
