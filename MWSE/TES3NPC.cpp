#include "TES3NPC.h"

#include "BitUtil.h"

#include "TES3UIManager.h"

#include "TES3MobilePlayer.h"
#include "TES3Race.h"
#include "TES3Reference.h"
#include "TES3UIElement.h"
#include "TES3WorldController.h"

#include "LuaManager.h"

#include "LuaCalcSoulValueEvent.h"
#include "LuaDispositionEvent.h"
#include "LuaEquipmentReevaluatedEvent.h"
#include "LuaIsGuardEvent.h"

#define TES3_UI_ID_MenuDialog 0x7D3442
#define TES3_UI_ID_MenuDialog_start_disposition 0x7D3486

namespace TES3 {
	//
	// NPC Base
	//

	int NPCBase::getBaseDisposition(bool clamp = true) {
		return vTable.actor->getDispositionRaw(this);
	}

	const auto TES3_NPCBase_isGuard = reinterpret_cast<bool(__thiscall*)(NPCBase*)>(0x04DA5E0);
	bool NPCBase::isGuard() {
		bool isGuard = TES3_NPCBase_isGuard(this);

		// Trigger isGuard event.
		mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
		const auto stateHandle = luaManager.getThreadSafeStateHandle();
		sol::table eventData = stateHandle.triggerEvent(new mwse::lua::event::IsGuardEvent(this, isGuard));
		if (eventData.valid()) {
			isGuard = eventData.get_or<bool>("isGuard", isGuard);
		}

		return isGuard;
	}

	bool NPCBase::getIsFemale() const {
		return BIT_TEST(actorFlags, TES3::ActorFlagNPC::FemaleBit);
	}

	void NPCBase::setIsFemale(bool value) {
		BIT_SET(actorFlags, ActorFlagNPC::FemaleBit, value);
	}

	bool NPCBase::getIsAutoCalc() const {
		return BIT_TEST(actorFlags, TES3::ActorFlagNPC::AutoCalcBit);
	}

	void NPCBase::setIsAutoCalc(bool value) {
		BIT_SET(actorFlags, ActorFlagNPC::AutoCalcBit, value);
	}

	bool NPCBase::getIsEssential_legacy() const {
		return BIT_TEST(actorFlags, TES3::ActorFlagNPC::EssentialBit);
	}

	void NPCBase::setIsEssential_legacy(bool value) {
		BIT_SET(actorFlags, ActorFlagNPC::EssentialBit, value);
	}

	bool NPCBase::getRespawns_legacy() const {
		return BIT_TEST(actorFlags, TES3::ActorFlagNPC::RespawnBit);
	}

	void NPCBase::setRespawns_legacy(bool value) {
		BIT_SET(actorFlags, ActorFlagNPC::RespawnBit, value);
	}

	float NPCBase::getWeight() const {
		const auto& weight = getRace()->weight;
		return getIsFemale() ? weight.female : weight.male;
	}

	float NPCBase::getHeight() const {
		const auto& height = getRace()->height;
		return getIsFemale() ? height.female : height.male;
	}

	//
	// NPC
	//

	Race* NPC::getRace() const {
		return race;
	}

	void NPC::setRace(Race* newRace) {
		race = newRace;
		if (race) {
			setModelPath("");
		}

		const auto worldController = WorldController::get();
		const auto macp = worldController ? worldController->getMobilePlayer() : nullptr;
		if (macp && macp->npcInstance->baseNPC == this) {
			macp->firstPerson->setRace(race);
		}
	}

	std::reference_wrapper<unsigned char[8]> NPC::getAttributes() {
		return std::ref(attributes);
	}

	std::reference_wrapper<unsigned char[27]> NPC::getSkills() {
		return std::ref(skills);
	}

	sol::optional<int> NPC::getSoulValue() {
		// Allow lua to determine the soul's value.
		if (mwse::lua::event::CalculateSoulValueEvent::getEventEnabled()) {
			auto& luaManager = mwse::lua::LuaManager::getInstance();
			const auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::table payload = stateHandle.triggerEvent(new mwse::lua::event::CalculateSoulValueEvent(this));
			if (payload.valid()) {
				return payload["value"];
			}
		}

		return {};
	}

	//
	// Shared code
	//
	// The engine functions getDisposition and modDisposition are a special case.
	// These functions only uses virtual functions, and can be used with both NPC and NPCInstance objects.
	// This may have been due to templating or compiler de-duplication.
	// The majority of uses are with NPCInstances. It may be called on the base NPC class if the NPC has not been cloned yet.
	//

	const auto TES3_NPCorNPCInstance_calculateDisposition = reinterpret_cast<int (__thiscall*)(const NPCInstance*, bool)>(0x4DA330);
	int NPCInstance::getDisposition(bool clamp) {
		// As a shared function, the `this` pointer may be TES3::NPC* or TES3::NPCInstance*.

		// Call the base function without clamping.
		auto disposition = TES3_NPCorNPCInstance_calculateDisposition(this, false);

		// Fire off our event.
		// Avoid firing events on a base NPC, due to the lack of a reference.
		if (isClone() && mwse::lua::event::DispositionEvent::getEventEnabled()) {
			const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			sol::table payload = stateHandle.triggerEvent(new mwse::lua::event::DispositionEvent(this, disposition, clamp));
			if (payload.valid()) {
				disposition = payload.get_or("disposition", disposition);
			}
		}

		// Re-clamp.
		if (clamp) {
			disposition = std::clamp(disposition, 0, 100);
		}

		return disposition;
	}

	//
	// NPC Instance
	//

	const auto TES3_NPCInstance_reevaluateEquipment = reinterpret_cast<void(__thiscall*)(NPCInstance*)>(0x4D9A20);
	void NPCInstance::reevaluateEquipment() {
		if (!BIT_TEST(inventory.flags, 0)) {
			return;
		}

		// Make sure to unequip all items before reevaluating equipment.
		// This is normally always done before calling TES3_NPCInstance_reevaluateEquipment in vanilla logic.
		// TES3_NPCInstance_reevaluateEquipment will lead to calls to TES3_Actor_equipItem/unequipItem without passing the mobile.
		// TES3_Actor_unequipItem will delete EquipmentStack pointers, therefore invalidating pointers present on the mobile.
		// If no mobile has been passed to TES3_Actor_unequipItem, those pointers will remain broken and lead to a crash.
		// To prevent this, TES3_Actor_unequipAllItems should get called before TES3_NPCInstance_reevaluateEquipment.
		// This is superfluous for vanilla logic, but prevents this function from being used incorrectly and has little to no cost if called twice.
		unequipAllItems(static_cast<MobileActor*>(getMobile()));

		// Call original function.
		TES3_NPCInstance_reevaluateEquipment(this);

		// Fire off event to let people know equipment has been reevaluated so custom slots can be equipped.
		if (mwse::lua::event::EquipmentReevaluatedEvent::getEventEnabled()) {
			const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			stateHandle.triggerEvent(new mwse::lua::event::EquipmentReevaluatedEvent(this));
		}
	}

	unsigned char NPCInstance::getReputation() {
		return reputation;
	}

	void NPCInstance::setReputation(unsigned char value) {
		reputation = value;
	}

	short NPCInstance::getBaseDisposition() {
		return baseDisposition;
	}

	void NPCInstance::setBaseDisposition(short value) {
		vTable.actor->setDispositionRaw(this, value);

		// Handle case where we're in dialog with this character.
		auto menuDialog = TES3::UI::findMenu(*reinterpret_cast<short*>(TES3_UI_ID_MenuDialog));
		auto serviceActor = TES3::UI::getServiceActor();
		if (menuDialog && serviceActor && serviceActor->actorType == TES3::MobileActorType::NPC && reinterpret_cast<TES3::MobileNPC*>(serviceActor)->npcInstance == this) {
			menuDialog->setProperty(static_cast<TES3::UI::Property>(*reinterpret_cast<short*>(TES3_UI_ID_MenuDialog_start_disposition)), baseDisposition);
			TES3::UI::updateDialogDisposition();
		}
	}

	void NPCInstance::setFactionRank(unsigned char value) {
		baseNPC->factionRank = value;
	}

	int NPCInstance::getDisposition_lua() {
		return getDisposition();
	}

	std::reference_wrapper<unsigned char[8]> NPCInstance::getAttributes() {
		return baseNPC->getAttributes();
	}
	
	std::reference_wrapper<unsigned char[27]> NPCInstance::getSkills() {
		return baseNPC->getSkills();
	}

	sol::optional<int> NPCInstance::getBaseSoulValue() {
		return baseNPC->getSoulValue();
	}

	Class* NPCInstance::getBaseClass() {
		return baseNPC->class_;
	}
	
	Faction* NPCInstance::getBaseFaction() {
		return baseNPC->faction;
	}

	unsigned char NPCInstance::getBaseFactionRank() {
		return baseNPC->factionRank;
	}

	Race* NPCInstance::getBaseRace() {
		return baseNPC->race;
	}

	Script* NPCInstance::getBaseScript() {
		return baseNPC->getScript();
	}

	SpellList* NPCInstance::getBaseSpellList() {
		return &baseNPC->spellList;
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::NPCBase)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::NPC)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::NPCInstance)
