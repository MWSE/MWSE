#include "TES3Creature.h"

#include "BitUtil.h"

#include "TES3MobileActor.h"

#include "LuaCalcSoulValueEvent.h"
#include "LuaEquipmentReevaluatedEvent.h"

#include "LuaManager.h"

namespace TES3 {
	bool CreatureBase::getIsBiped() const {
		return BIT_TEST(actorFlags, ActorFlagCreature::BipedBit);
	}

	void CreatureBase::setIsBiped(bool value) {
		BIT_SET(actorFlags, ActorFlagCreature::BipedBit, value);
	}

	bool CreatureBase::getIsEssential() const {
		return BIT_TEST(actorFlags, ActorFlagCreature::EssentialBit);
	}

	void CreatureBase::setIsEssential(bool value) {
		BIT_SET(actorFlags, ActorFlagCreature::EssentialBit, value);
	}

	bool CreatureBase::getRespawns() const {
		return BIT_TEST(actorFlags, ActorFlagCreature::RespawnBit);
	}

	void CreatureBase::setRespawns(bool value) {
		BIT_SET(actorFlags, ActorFlagCreature::RespawnBit, value);
	}

	bool CreatureBase::getUsesEquipment() const {
		return BIT_TEST(actorFlags, ActorFlagCreature::WeaponAndShieldBit);
	}

	void CreatureBase::setUsesEquipment(bool value) {
		BIT_SET(actorFlags, ActorFlagCreature::WeaponAndShieldBit, value);
	}

	bool CreatureBase::getSwims() const {
		return BIT_TEST(actorFlags, ActorFlagCreature::SwimsBit);
	}

	void CreatureBase::setSwims(bool value) {
		BIT_SET(actorFlags, ActorFlagCreature::SwimsBit, value);
	}

	bool CreatureBase::getFlies() const {
		return BIT_TEST(actorFlags, ActorFlagCreature::FliesBit);
	}

	void CreatureBase::setFlies(bool value) {
		BIT_SET(actorFlags, ActorFlagCreature::FliesBit, value);
	}

	bool CreatureBase::getWalks() const {
		return BIT_TEST(actorFlags, ActorFlagCreature::WalksBit);
	}

	void CreatureBase::setWalks(bool value) {
		BIT_SET(actorFlags, ActorFlagCreature::WalksBit, value);
	}

	const auto TES3_Creature_ctor = reinterpret_cast<void(__thiscall*)(Creature*)>(0x49C0C0);
	Creature::Creature() {
		TES3_Creature_ctor(this);
	}

	const auto TES3_Creature_dtor = reinterpret_cast<void(__thiscall*)(Creature*)>(0x49C280);
	Creature::~Creature() {
		TES3_Creature_dtor(this);
	}

	std::reference_wrapper<int[8]> Creature::getAttributes() {
		return std::ref(attributes);
	}

	std::reference_wrapper<int[3]> Creature::getSkills() {
		return std::ref(skills);
	}

	std::reference_wrapper<Range<int>[3]> Creature::getAttacks() {
		return std::ref(attacks);
	}

	int Creature::getSoulValue() {
		int value = soul;

		// Allow lua to determine the soul's value.
		if (mwse::lua::event::CalculateSoulValueEvent::getEventEnabled()) {
			auto& luaManager = mwse::lua::LuaManager::getInstance();
			const auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::table payload = stateHandle.triggerEvent(new mwse::lua::event::CalculateSoulValueEvent(this, soul));
			if (payload.valid()) {
				sol::object eventValue = payload["value"];
				if (eventValue.is<int>()) {
					value = eventValue.as<int>();
				}
			}
		}

		return value;
	}

	void Creature::setSoulValue(int value) {
		soul = value;
	}

	AIConfig* CreatureInstance::getBaseAIConfig() const {
		return baseCreature->aiConfig;
	}

	std::reference_wrapper<int[8]> CreatureInstance::getBaseAttributes() const {
		return baseCreature->getAttributes();
	}

	std::reference_wrapper<int[3]> CreatureInstance::getBaseSkills() const {
		return baseCreature->getSkills();
	}

	std::reference_wrapper<Range<int>[3]> CreatureInstance::getBaseAttacks() const {
		return baseCreature->getAttacks();
	}

	Creature* CreatureInstance::getBaseSoundGenerator() const {
		return baseCreature->soundGenerator;
	}

	int CreatureInstance::getBaseCreatureType() const {
		return baseCreature->creatureType;
	}

	SpellList* CreatureInstance::getBaseSpells() const {
		return baseCreature->spellList;
	}

	const auto TES3_CreatureInstance_reevaluateEquipment = reinterpret_cast<void(__thiscall*)(CreatureInstance*)>(0x49EF60);
	void CreatureInstance::reevaluateEquipment() {
		if (!BIT_TEST(inventory.flags, 0)) {
			return;
		}

		// Make sure to unequip all items before reevaluating equipment.
		// This is normally always done before calling TES3_NPCInstance_reevaluateEquipment in vanilla logic.
		// TES3_CreatureInstance_reevaluateEquipment will lead to calls to TES3_Actor_equipItem/unequipItem without passing the mobile.
		// TES3_Actor_unequipItem will delete EquipmentStack pointers, therefore invalidating pointers present on the mobile.
		// If no mobile has been passed to TES3_Actor_unequipItem, those pointers will remain broken and lead to a crash.
		// To prevent this, TES3_Actor_unequipAllItems should get called before TES3_CreatureInstance_reevaluateEquipment.
		// This is superfluous for vanilla logic, but prevents this function from being used incorrectly and has little to no cost if called twice.
		unequipAllItems(static_cast<MobileActor*>(getMobile()));

		// Call original function.
		TES3_CreatureInstance_reevaluateEquipment(this);

		// Fire off event to let people know equipment has been reevaluated so custom slots can be equipped.
		if (mwse::lua::event::EquipmentReevaluatedEvent::getEventEnabled()) {
			const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			stateHandle.triggerEvent(new mwse::lua::event::EquipmentReevaluatedEvent(this));
		}
	}

	int CreatureInstance::getBaseSoulValue() const {
		return baseCreature->getSoulValue();
	}

	void CreatureInstance::setBaseSoulValue(int value) {
		baseCreature->setSoulValue(value);
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::CreatureBase)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::Creature)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::CreatureInstance)
