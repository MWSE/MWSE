#include "TES3MagicInstanceController.h"

namespace TES3 {

	const auto TES3_MagicInstanceController_activateSpell = reinterpret_cast<unsigned int (__thiscall*)(MagicInstanceController*, Reference *, EquipmentStack*, MagicSourceCombo*)>(0x454A60);
	unsigned int MagicInstanceController::activateSpell(Reference * reference, EquipmentStack* sourceItem, MagicSourceCombo* source) {
		return TES3_MagicInstanceController_activateSpell(this, reference, sourceItem, source);
	}

	const auto TES3_MagicInstanceController_removeSpellsByEffect = reinterpret_cast<MagicSourceInstance* (__thiscall*)(MagicInstanceController*, Reference*, int, int)>(0x455880);
	void MagicInstanceController::removeSpellsByEffect(Reference * reference, int effectId, int percentChance) {
		TES3_MagicInstanceController_removeSpellsByEffect(this, reference, effectId, percentChance);
	}

	const auto TES3_MagicInstanceController_clearSpellEffect = reinterpret_cast<MagicSourceInstance* (__thiscall*)(MagicInstanceController*, Reference *, int, int, bool)>(0x4556C0);
	void MagicInstanceController::clearSpellEffect(Reference * reference, int castType, int percentChance, bool removeSpell) {
		TES3_MagicInstanceController_clearSpellEffect(this, reference, castType, percentChance, removeSpell);
	}

	const auto TES3_MagicInstanceController_getInstanceFromSerial = reinterpret_cast<MagicSourceInstance* (__thiscall*)(MagicInstanceController*, unsigned int)>(0x4553B0);
	MagicSourceInstance * MagicInstanceController::getInstanceFromSerial(unsigned int serial) {
		return TES3_MagicInstanceController_getInstanceFromSerial(this, serial);
	}
}
