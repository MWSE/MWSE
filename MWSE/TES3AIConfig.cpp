#include "TES3AIConfig.h"

#include "TES3Class.h"
#include "MemoryUtil.h"

namespace TES3 {
	AIConfig::AIConfig() {
		hello = 30;
		fight = 30;
		flee = 30;
		alarm = 0;
		merchantFlags = 0;

		travelDestinations = nullptr;
	}

	AIConfig::~AIConfig() {
		if (travelDestinations) {
			delete travelDestinations;
			travelDestinations = nullptr;
		}
	}

	bool AIConfig::tradesItemType(ObjectType::ObjectType objectType) const {
		switch (objectType) {
		case TES3::ObjectType::Alchemy:
			return (merchantFlags & TES3::ServiceFlag::BartersAlchemy);
		case TES3::ObjectType::Apparatus:
			return (merchantFlags & TES3::ServiceFlag::BartersApparatus);
		case TES3::ObjectType::Armor:
			return (merchantFlags & TES3::ServiceFlag::BartersArmor);
		case TES3::ObjectType::Book:
			return (merchantFlags & TES3::ServiceFlag::BartersBooks);
		case TES3::ObjectType::Clothing:
			return (merchantFlags & TES3::ServiceFlag::BartersClothing);
		case TES3::ObjectType::Ingredient:
			return (merchantFlags & TES3::ServiceFlag::BartersIngredients);
		case TES3::ObjectType::Light:
			return (merchantFlags & TES3::ServiceFlag::BartersLights);
		case TES3::ObjectType::Lockpick:
			return (merchantFlags & TES3::ServiceFlag::BartersLockpicks);
		case TES3::ObjectType::Misc:
			return (merchantFlags & TES3::ServiceFlag::BartersMiscItems);
		case TES3::ObjectType::Probe:
			return (merchantFlags & TES3::ServiceFlag::BartersProbes);
		case TES3::ObjectType::Repair:
			return (merchantFlags & TES3::ServiceFlag::BartersRepairTools);
		case TES3::ObjectType::Weapon:
		case TES3::ObjectType::Ammo:
			return (merchantFlags & TES3::ServiceFlag::BartersWeapons);
		}
		return false;
	}

	bool AIConfig::getServiceFlag(unsigned int flag) const {
		return (merchantFlags & flag) != 0;
	}

	void AIConfig::setServiceFlag(unsigned int flag, bool set) {
		if (set) {
			merchantFlags |= flag;
		}
		else {
			merchantFlags &= ~flag;
		}
	}

	bool AIConfig::getBartersAlchemy() const {
		return getServiceFlag(TES3::ServiceFlag::BartersAlchemy);
	}

	void AIConfig::setBartersAlchemy(bool set) {
		setServiceFlag(TES3::ServiceFlag::BartersAlchemy, set);
	}

	bool AIConfig::getBartersApparatus() const {
		return getServiceFlag(TES3::ServiceFlag::BartersApparatus);
	}

	void AIConfig::setBartersApparatus(bool set) {
		setServiceFlag(TES3::ServiceFlag::BartersApparatus, set);
	}

	bool AIConfig::getBartersArmor() const {
		return getServiceFlag(TES3::ServiceFlag::BartersArmor);
	}

	void AIConfig::setBartersArmor(bool set) {
		setServiceFlag(TES3::ServiceFlag::BartersArmor, set);
	}

	bool AIConfig::getBartersBooks() const {
		return getServiceFlag(TES3::ServiceFlag::BartersBooks);
	}

	void AIConfig::setBartersBooks(bool set) {
		setServiceFlag(TES3::ServiceFlag::BartersBooks, set);
	}

	bool AIConfig::getBartersClothing() const {
		return getServiceFlag(TES3::ServiceFlag::BartersClothing);
	}

	void AIConfig::setBartersClothing(bool set) {
		setServiceFlag(TES3::ServiceFlag::BartersClothing, set);
	}

	bool AIConfig::getBartersEnchantedItems() const {
		return getServiceFlag(TES3::ServiceFlag::BartersEnchantedItems);
	}

	void AIConfig::setBartersEnchantedItems(bool set) {
		setServiceFlag(TES3::ServiceFlag::BartersEnchantedItems, set);
	}

	bool AIConfig::getBartersIngredients() const {
		return getServiceFlag(TES3::ServiceFlag::BartersIngredients);
	}

	void AIConfig::setBartersIngredients(bool set) {
		setServiceFlag(TES3::ServiceFlag::BartersIngredients, set);
	}

	bool AIConfig::getBartersLights() const {
		return getServiceFlag(TES3::ServiceFlag::BartersLights);
	}

	void AIConfig::setBartersLights(bool set) {
		setServiceFlag(TES3::ServiceFlag::BartersLights, set);
	}

	bool AIConfig::getBartersLockpicks() const {
		return getServiceFlag(TES3::ServiceFlag::BartersLockpicks);
	}

	void AIConfig::setBartersLockpicks(bool set) {
		setServiceFlag(TES3::ServiceFlag::BartersLockpicks, set);
	}

	bool AIConfig::getBartersMiscItems() const {
		return getServiceFlag(TES3::ServiceFlag::BartersMiscItems);
	}

	void AIConfig::setBartersMiscItems(bool set) {
		setServiceFlag(TES3::ServiceFlag::BartersMiscItems, set);
	}

	bool AIConfig::getBartersProbes() const {
		return getServiceFlag(TES3::ServiceFlag::BartersProbes);
	}

	void AIConfig::setBartersProbes(bool set) {
		setServiceFlag(TES3::ServiceFlag::BartersProbes, set);
	}

	bool AIConfig::getBartersRepairTools() const {
		return getServiceFlag(TES3::ServiceFlag::BartersRepairTools);
	}

	void AIConfig::setBartersRepairTools(bool set) {
		setServiceFlag(TES3::ServiceFlag::BartersRepairTools, set);
	}

	bool AIConfig::getBartersWeapons() const {
		return getServiceFlag(TES3::ServiceFlag::BartersWeapons);
	}

	void AIConfig::setBartersWeapons(bool set) {
		setServiceFlag(TES3::ServiceFlag::BartersWeapons, set);
	}

	bool AIConfig::getOffersBartering() const {
		return merchantFlags & ServiceFlag::OffersBarteringMask;
	}

	bool AIConfig::getOffersEnchanting() const {
		return getServiceFlag(TES3::ServiceFlag::OffersEnchanting);
	}

	void AIConfig::setOffersEnchanting(bool set) {
		setServiceFlag(TES3::ServiceFlag::OffersEnchanting, set);
	}

	bool AIConfig::getOffersRepairs() const {
		return getServiceFlag(TES3::ServiceFlag::OffersRepairs);
	}

	void AIConfig::setOffersRepairs(bool set) {
		setServiceFlag(TES3::ServiceFlag::OffersRepairs, set);
	}

	bool AIConfig::getOffersSpellmaking() const {
		return getServiceFlag(TES3::ServiceFlag::OffersSpellmaking);
	}

	void AIConfig::setOffersSpellmaking(bool set) {
		setServiceFlag(TES3::ServiceFlag::OffersSpellmaking, set);
	}

	bool AIConfig::getOffersSpells() const {
		return getServiceFlag(TES3::ServiceFlag::OffersSpells);
	}

	void AIConfig::setOffersSpells(bool set) {
		setServiceFlag(TES3::ServiceFlag::OffersSpells, set);
	}

	bool AIConfig::getOffersTraining() const {
		return getServiceFlag(TES3::ServiceFlag::OffersTraining);
	}

	void AIConfig::setOffersTraining(bool set) {
		setServiceFlag(TES3::ServiceFlag::OffersTraining, set);
	}

}
