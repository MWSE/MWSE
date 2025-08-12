#include "TES3MagicInstanceController.h"

#include "LuaManager.h"

#include "LuaActiveMagicEffectIconsUpdatedEvent.h"

namespace TES3 {

	const auto TES3_MagicInstanceController_activateSpell = reinterpret_cast<unsigned int (__thiscall*)(MagicInstanceController*, Reference *, EquipmentStack*, MagicSourceCombo*)>(0x454A60);
	unsigned int MagicInstanceController::activateSpell(Reference* reference, EquipmentStack* sourceItem, MagicSourceCombo* source) {
		return TES3_MagicInstanceController_activateSpell(this, reference, sourceItem, source);
	}

	const auto TES3_MagicInstanceController_removeSpellsByEffect = reinterpret_cast<MagicSourceInstance* (__thiscall*)(MagicInstanceController*, Reference*, int, int)>(0x455880);
	void MagicInstanceController::removeSpellsByEffect(Reference* reference, int effectId, int percentChance) {
		TES3_MagicInstanceController_removeSpellsByEffect(this, reference, effectId, percentChance);
	}

	const auto TES3_MagicInstanceController_clearSpellEffect = reinterpret_cast<MagicSourceInstance* (__thiscall*)(MagicInstanceController*, Reference *, int, int, bool)>(0x4556C0);
	void MagicInstanceController::clearSpellEffect(Reference* reference, int castType, int percentChance, bool removeSpell) {
		TES3_MagicInstanceController_clearSpellEffect(this, reference, castType, percentChance, removeSpell);
	}

	const auto TES3_MagicInstanceController_getInstanceFromSerial = reinterpret_cast<MagicSourceInstance* (__thiscall*)(MagicInstanceController*, unsigned int)>(0x4553B0);
	MagicSourceInstance* MagicInstanceController::getInstanceFromSerial(unsigned int serial) {
		return TES3_MagicInstanceController_getInstanceFromSerial(this, serial);
	}

	const auto TES3_MagicInstanceController_retireMagicCastedByActor = reinterpret_cast<void(__thiscall*)(MagicInstanceController*, Reference*)>(0x454EC0);
	void MagicInstanceController::retireMagicCastedByActor(Reference* reference) {
		TES3_MagicInstanceController_retireMagicCastedByActor(this, reference);
	}

	const auto TES3_MagicInstanceController_interruptCasting = reinterpret_cast<void(__thiscall*)(MagicInstanceController*, Reference*)>(0x455610);
	void MagicInstanceController::interruptCasting(Reference* reference) {
		TES3_MagicInstanceController_interruptCasting(this, reference);
	}

	const auto TES3_UI_updateActiveMagicEffectIcons = reinterpret_cast<void(__cdecl*)()>(0x5E2480);
	void MagicInstanceController::updateActiveMagicEffectIcons() {
		TES3_UI_updateActiveMagicEffectIcons();

		if (mwse::lua::event::ActiveMagicEffectIconsUpdatedEvent::getEventEnabled()) {
			auto& luaManager = mwse::lua::LuaManager::getInstance();
			const auto stateHandle = luaManager.getThreadSafeStateHandle();
			stateHandle.triggerEvent(new mwse::lua::event::ActiveMagicEffectIconsUpdatedEvent());
		}
	}
}
