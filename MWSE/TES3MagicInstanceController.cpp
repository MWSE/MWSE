#include "TES3MagicInstanceController.h"

#include "LuaManager.h"

#include "MemoryUtil.h"

#include "LuaActiveMagicEffectIconsUpdatedEvent.h"

#include "TES3MagicEffectInstance.h"
#include "TES3MagicSourceInstance.h"
#include "TES3Reference.h"

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

	const auto TES3_MagicInstanceController_retireMagicBySerial = reinterpret_cast<void(__thiscall*)(MagicInstanceController*, unsigned int)>(0x455070);
	void MagicInstanceController::retireMagicBySerial(unsigned int serial) {
		TES3_MagicInstanceController_retireMagicBySerial(this, serial);
	}

	const auto TES3_MagicInstanceController_retireMagicCastedByActor = reinterpret_cast<void(__thiscall*)(MagicInstanceController*, Reference*)>(0x454EC0);
	void MagicInstanceController::retireMagicCastedByActor(Reference* reference) {
		TES3_MagicInstanceController_retireMagicCastedByActor(this, reference);
	}

	const auto TES3_MagicInstanceController_interruptCasting = reinterpret_cast<void(__thiscall*)(MagicInstanceController*, Reference*)>(0x455610);
	void MagicInstanceController::interruptCasting(Reference* reference) {
		TES3_MagicInstanceController_interruptCasting(this, reference);
	}

	// The engine function is compiled against this map's sentinel, so it is not valid for the other maps.
	const auto TES3_MagicInstanceController_nextSerialInstanceNode = reinterpret_cast<void(__thiscall*)(MagicInstanceController::StlMap::Node**)>(0x457750);
	MagicInstanceController::StlMap::Node* MagicInstanceController::getNextSerialInstanceNode(StlMap::Node* node) const {
		TES3_MagicInstanceController_nextSerialInstanceNode(&node);
		return node;
	}

	void MagicInstanceController::cleanupReference(Reference* reference) {
		if (reference == nullptr) {
			return;
		}

		// Retire any magic this reference cast. This also covers non-actor casters such as trapped doors and containers, and unlike the retire in Reference::dtor it does not require a live mobile.
		retireMagicCastedByActor(reference);

		// Only actors and lockable objects can be targets of magic effects.
		const auto baseObject = reference->baseObject;
		if (baseObject == nullptr) {
			return;
		}
		const auto canBeEffectTarget = baseObject->isMobileCapableActor()
			|| baseObject->objectType == ObjectType::Door
			|| baseObject->objectType == ObjectType::Container;
		if (!canBeEffectTarget) {
			return;
		}

		// Snapshot the live serials first, as retiring modifies the map.
		std::vector<unsigned int> serials;
		serials.reserve(mapSerialToMagicSourceInstance.itemCount);
		const auto sentinel = mapSerialToMagicSourceInstance.root;
		for (auto node = sentinel->left; node != sentinel; node = getNextSerialInstanceNode(node)) {
			serials.push_back(node->key);
		}

		for (const auto serial : serials) {
			auto instance = getInstanceFromSerial(serial);
			if (instance == nullptr) {
				continue;
			}

			// If we cast this spell, end it immediately.
			if (instance->caster == reference) {
				instance->retire();
				instance->state = SpellEffectState::Ending;
				instance->process(0.0f);
				retireMagicBySerial(serial);
				continue;
			}

			if (instance->target == reference) {
				instance->target = nullptr;
			}
			if (instance->mcpOriginalTarget == reference) {
				instance->mcpOriginalTarget = nullptr;
			}

			auto retireEffectsOnReference = false;
			for (auto effectIndex = 0; effectIndex < 8 && !retireEffectsOnReference; ++effectIndex) {
				const auto& effectMap = instance->effects[effectIndex];
				for (auto bucketIndex = 0u; bucketIndex < effectMap.bucketCount && !retireEffectsOnReference; ++bucketIndex) {
					for (auto node = effectMap.buckets[bucketIndex]; node; node = node->nextNode) {
						if (node->value.target == reference) {
							retireEffectsOnReference = true;
							break;
						}
					}
				}
			}

			if (!retireEffectsOnReference) {
				continue;
			}

			instance->retire(reference);

			// If the engine left any orphaned effect entries behind, make them inert rather than dangling.
			instance = getInstanceFromSerial(serial);
			if (instance == nullptr) {
				continue;
			}

			// Clean up any remaining effect targets.
			for (const auto& effectMap : instance->effects) {
				for (auto i = 0u; i < effectMap.bucketCount; ++i) {
					for (auto node = effectMap.buckets[i]; node; node = node->nextNode) {
						if (node->value.target == reference) {
							node->value.target = nullptr;
						}
					}
				}
			}
		}
	}

	unsigned int MagicInstanceController::getSerialCount() {
		return se::memory::ExternalGlobal<unsigned int, 0x7CF0FC>::get();
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
