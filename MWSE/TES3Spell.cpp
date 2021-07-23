
#include "LuaManager.h"
#include "LuaSpellCastEvent.h"

#include "TES3GameSetting.h"
#include "TES3MagicEffect.h"
#include "TES3MobileActor.h"
#include "TES3NPC.h"
#include "TES3Reference.h"
#include "TES3Spell.h"

#include "LuaUtil.h"
#include "TES3Util.h"
#include "BitUtil.h"

namespace TES3 {
	const auto TES3_Spell_ctor = reinterpret_cast<TES3::Spell * (__thiscall*)(TES3::Spell*)>(0x4A9FD0);
	Spell::Spell() {
		TES3_Spell_ctor(this);
	}

	const auto TES3_Spell_dtor = reinterpret_cast<void(__thiscall*)(TES3::Spell*)>(0x4AA0E0);
	Spell::~Spell() {
		TES3_Spell_dtor(this);
	}

	const auto TES3_Spell_getLeastProficientEffect = reinterpret_cast<Effect * (__thiscall*)(TES3::Spell*, const NPC * npc)>(0x4AA850);
	Effect* Spell::getLeastProficientEffect(const NPC* npc) {
		return TES3_Spell_getLeastProficientEffect(this, npc);
	}

	Effect* Spell::getLeastProficientEffect(const MobileActor* mobile) {
		if (mobile == nullptr) {
			return nullptr;
		}

		Effect* highestEffect = nullptr;
		float highestCost = FLT_MAX;
		for (auto i = 7; i >= 0; i--) {
			auto effect = &effects[i];
			if (effect->effectID == EffectID::None) {
				continue;
			}

			auto skill = effect->getEffectData()->getSkillForSchool();
			auto cost = mobile->getSkillValue(skill) * 2 - effect->calculateCost();
			if (cost < highestCost) {
				highestEffect = effect;
				highestCost = cost;
			}
		}

		return highestEffect;
	}

	Effect* Spell::getLeastProficientEffect_lua(sol::stack_object object) {
		if (object.is<MobileActor*>()) {
			return getLeastProficientEffect(object.as<MobileActor*>());
		}
		else if (object.is<Reference*>()) {
			auto reference = object.as<Reference*>();
			auto mobile = reference->getAttachedMobileActor();
			if (mobile) {
				return getLeastProficientEffect(mobile);
			}
			else if (reference->baseObject->objectType == ObjectType::NPC) {
				return getLeastProficientEffect(static_cast<NPC*>(reference->getBaseObject()));
			}
		}
		else if (object.is<NPCInstance*>()) {
			auto npc = object.as<NPCInstance*>();
			auto mobile = static_cast<MobileActor*>(npc->getMobile());
			if (mobile) {
				return getLeastProficientEffect(mobile);
			}
			else {
				return getLeastProficientEffect(npc->baseNPC);
			}
		}
		else if (object.is<NPC*>()) {
			return getLeastProficientEffect(object.as<NPC*>());
		}

		throw std::invalid_argument("Argument must be able to resolve as a mobile actor or NPC.");
	}

	const auto TES3_Spell_getLeastProficientSchool = reinterpret_cast<int (__thiscall*)(TES3::Spell*, const NPC * npc)>(0x4AA910);
	int Spell::getLeastProficientSchool(const NPC* npc) {
		return TES3_Spell_getLeastProficientSchool(this, npc);
	}

	int Spell::getLeastProficientSchool(const MobileActor* mobile) {
		auto leastProficientEffect = getLeastProficientEffect(mobile);
		if (leastProficientEffect && leastProficientEffect->effectID != EffectID::None) {
			auto effectData = leastProficientEffect->getEffectData();
			if (effectData) {
				return effectData->school;
			}
		}
		return -1;
	}

	int Spell::getLeastProficientSchool_lua(sol::stack_object object) {
		if (object.is<MobileActor*>()) {
			return getLeastProficientSchool(object.as<MobileActor*>());
		}
		else if (object.is<Reference*>()) {
			auto reference = object.as<Reference*>();
			auto mobile = reference->getAttachedMobileActor();
			if (mobile) {
				return getLeastProficientSchool(mobile);
			}
			else if (reference->baseObject->objectType == ObjectType::NPC) {
				return getLeastProficientSchool(static_cast<NPC*>(reference->getBaseObject()));
			}
		}
		else if (object.is<NPCInstance*>()) {
			auto npc = object.as<NPCInstance*>();
			auto mobile = static_cast<MobileActor*>(npc->getMobile());
			if (mobile) {
				return getLeastProficientSchool(mobile);
			}
			else {
				return getLeastProficientSchool(npc->baseNPC);
			}
		}
		else if (object.is<NPC*>()) {
			return getLeastProficientSchool(object.as<NPC*>());
		}

		throw std::invalid_argument("Argument must be able to resolve as a mobile actor or NPC.");
	}

	float Spell::calculateCastChance(Reference* caster, bool checkMagicka, int* weakestSchoolId) {
		MobileActor* mobileCaster = nullptr;
		if (caster) {
			mobileCaster = caster->getAttachedMobileActor();
		}

		return calculateCastChance(mobileCaster, checkMagicka, weakestSchoolId);
	}

	const auto TES3_Spell_calculateCastChance = reinterpret_cast<float(__thiscall *)(Spell*, MobileActor*, bool, int*)>(0x4AA950);
	float Spell::calculateCastChance(MobileActor* caster, bool checkMagicka, int* weakestSchoolId) {
		return TES3_Spell_calculateCastChance(this, caster, checkMagicka, weakestSchoolId);
	}

	float Spell::castChanceOnCast(TES3::MobileActor* caster, bool checkMagicka, int* weakestSchoolId) {
		// Call the original function.
		float castChance = calculateCastChance(caster, checkMagicka, weakestSchoolId);

		// Ignore ability spells, as they are automatically activated as NPCs enter simulation range.
		if (castType != TES3::SpellCastType::Ability && mwse::lua::event::SpellCastEvent::getEventEnabled()) {
			// Trigger event, and update the cast chance.
			mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
			auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::table eventData = stateHandle.triggerEvent(new mwse::lua::event::SpellCastEvent(this, caster, castChance, *weakestSchoolId));
			if (eventData.valid()) {
				castChance = eventData.get_or<float>("castChance", castChance);
			}
		}

		return castChance;
	}

	bool Spell::getSpellFlag(SpellFlag::Flag flag) const {
		return BITMASK_TEST(spellFlags, flag);
	}

	void Spell::setSpellFlag(SpellFlag::Flag flag, bool value) {
		BITMASK_SET(spellFlags, flag, value);
	}

	bool Spell::getAutoCalc() const {
		return getSpellFlag(SpellFlag::Flag::AutoCalc);
	}

	void Spell::setAutoCalc(bool value) {
		setSpellFlag(SpellFlag::Flag::AutoCalc, value);
	}

	bool Spell::getPlayerStart() const {
		return getSpellFlag(SpellFlag::Flag::PCStartSpell);
	}

	void Spell::setPlayerStart(bool value) {
		setSpellFlag(SpellFlag::Flag::PCStartSpell, value);
	}

	bool Spell::getAlwaysSucceeds() const {
		return getSpellFlag(SpellFlag::Flag::AlwaysSucceeds);
	}

	void Spell::setAlwaysSucceeds(bool value) {
		setSpellFlag(SpellFlag::Flag::AlwaysSucceeds, value);
	}

	size_t Spell::getActiveEffectCount() {
		size_t count = 0;
		for (size_t i = 0; i < 8; i++) {
			if (effects[i].effectID != TES3::EffectID::None) {
				count++;
			}
		}
		return count;
	}

	int Spell::getFirstIndexOfEffect(int effectId) {
		for (size_t i = 0; i < 8; i++) {
			if (effects[i].effectID == effectId) {
				return i;
			}
		}
		return -1;
	}

	int Spell::calculateBasePuchaseCost() const {
		return int(magickaCost * TES3::DataHandler::get()->nonDynamicData->GMSTs[TES3::GMST::fSpellValueMult]->value.asFloat);
	}

	float Spell::calculateCastChance_lua(sol::table params) {
		bool checkMagicka = mwse::lua::getOptionalParam<bool>(params, "checkMagicka", true);
		sol::object caster = params["caster"];
		if (caster.is<TES3::Reference>()) {
			return calculateCastChance(caster.as<TES3::Reference*>(), checkMagicka);
		}
		else if (caster.is<TES3::MobileActor>()) {
			return calculateCastChance(caster.as<TES3::MobileActor*>(), checkMagicka);
		}

		return 0.0f;
	}

	std::reference_wrapper<Effect[8]> Spell::getEffects() {
		return std::ref(effects);
	}

}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::Spell)
