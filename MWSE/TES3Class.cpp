#include "TES3Class.h"

#include "TES3Archive.h"
#include "TES3MobilePlayer.h"
#include "TES3NPC.h"
#include "TES3Reference.h"
#include "TES3UIElement.h"
#include "TES3UIManager.h"
#include "TES3WorldController.h"

#include "TES3Util.h"

namespace TES3 {
	const char* Class::getObjectID() const {
		return id;
	}

	const char* Class::getName() const {
		return name;
	}

	bool Class::getServiceFlag(unsigned int flag) const {
		return (services & flag) != 0;
	}

	void Class::setName(const char* _name) {
		if (strlen(_name) >= 32) {
			throw std::invalid_argument("Path cannot be 32 or more characters.");
		}
		strncpy_s(name, _name, 32);

		// Update GUI if we're changing the player's class name.
		auto macp = TES3::WorldController::get()->getMobilePlayer();
		if (macp && macp->reference->baseObject->getClass() == this) {
			auto menuStats = TES3::UI::findMenu(*reinterpret_cast<TES3::UI::UI_ID*>(0x7D6B9C));
			if (menuStats) {
				auto menuStats_class = menuStats->findChild(*reinterpret_cast<TES3::UI::UI_ID*>(0x7D6A4E));
				if (menuStats_class) {
					menuStats_class->setText(name);
					menuStats->timingUpdate();
				}
			}
		}
	}

	const auto TES3_Class_loadDescription = reinterpret_cast<char*(__thiscall*)(Class*)>(0x4A81D0);
	char* Class::loadDescription() {
		return TES3_Class_loadDescription(this);
	}

	sol::optional<std::string> Class::getOrLoadDescription() {
		// If the description is already loaded, just return it.
		if (description) {
			return { description };
		}

		// Otherwise we need to load it from disk, then free it.
		else {
			char* description = loadDescription();
			if (description) {
				// We loaded successfully, package, free, then return.
				std::string value = description;
				freeDescription();
				return std::move(value);
			}
		}

		return {};
	}

	const auto TES3_Class_setDescription = reinterpret_cast<void(__thiscall*)(Class*, const char*)>(0x4A8200);
	void Class::setDescription(const char * description) {
		TES3_Class_setDescription(this, description);
	}

	const auto TES3_Class_freeDescription = reinterpret_cast<void(__thiscall*)(Class*)>(0x4A8450);
	void Class::freeDescription() {
		TES3_Class_freeDescription(this);
	}

	std::string Class::getLevelUpImage() const {
		char buffer[64] = {};
		sprintf(buffer, "textures\\levelup\\%s.bmp", id);
		if (mwse::tes3::resolveAssetPath(buffer) != TES3::FileLoadSource::Missing) {
			return buffer;
		}
		return "textures\\levelup\\Warrior.bmp";
	}

	std::reference_wrapper<int[2]> Class::getAttributes() {
		return std::ref(attributes);
	}

	std::reference_wrapper<int[10]> Class::getSkills() {
		return std::ref(skills);
	}

	bool Class::getIsPlayable() {
		return playable != 0;
	}

	void Class::setIsPlayable(bool value) {
		playable = value;
	}

	void Class::setServiceFlag(unsigned int flag, bool set) {
		if (set) {
			services |= flag;
		}
		else {
			services &= ~flag;
		}
	}

	bool Class::getBartersAlchemy() const {
		return getServiceFlag(ServiceFlag::BartersAlchemy);
	}

	void Class::setBartersAlchemy(bool value) {
		setServiceFlag(ServiceFlag::BartersAlchemy, value);
	}

	bool Class::getBartersApparatus() const {
		return getServiceFlag(ServiceFlag::BartersApparatus);
	}

	void Class::setBartersApparatus(bool value) {
		setServiceFlag(ServiceFlag::BartersApparatus, value);
	}

	bool Class::getBartersArmor() const {
		return getServiceFlag(ServiceFlag::BartersArmor);
	}

	void Class::setBartersArmor(bool value) {
		setServiceFlag(ServiceFlag::BartersArmor, value);
	}

	bool Class::getBartersBooks() const {
		return getServiceFlag(ServiceFlag::BartersBooks);
	}

	void Class::setBartersBooks(bool value) {
		setServiceFlag(ServiceFlag::BartersBooks, value);
	}

	bool Class::getBartersClothing() const {
		return getServiceFlag(ServiceFlag::BartersClothing);
	}

	void Class::setBartersClothing(bool value) {
		setServiceFlag(ServiceFlag::BartersClothing, value);
	}

	bool Class::getBartersEnchantedItems() const {
		return getServiceFlag(ServiceFlag::BartersEnchantedItems);
	}

	void Class::setBartersEnchantedItems(bool value) {
		setServiceFlag(ServiceFlag::BartersEnchantedItems, value);
	}

	bool Class::getBartersIngredients() const {
		return getServiceFlag(ServiceFlag::BartersIngredients);
	}

	void Class::setBartersIngredients(bool value) {
		setServiceFlag(ServiceFlag::BartersIngredients, value);
	}

	bool Class::getBartersLights() const {
		return getServiceFlag(ServiceFlag::BartersLights);
	}

	void Class::setBartersLights(bool value) {
		setServiceFlag(ServiceFlag::BartersLights, value);
	}

	bool Class::getBartersLockpicks() const {
		return getServiceFlag(ServiceFlag::BartersLockpicks);
	}

	void Class::setBartersLockpicks(bool value) {
		setServiceFlag(ServiceFlag::BartersLockpicks, value);
	}

	bool Class::getBartersMiscItems() const {
		return getServiceFlag(ServiceFlag::BartersMiscItems);
	}

	void Class::setBartersMiscItems(bool value) {
		setServiceFlag(ServiceFlag::BartersMiscItems, value);
	}

	bool Class::getBartersProbes() const {
		return getServiceFlag(ServiceFlag::BartersProbes);
	}

	void Class::setBartersProbes(bool value) {
		setServiceFlag(ServiceFlag::BartersProbes, value);
	}

	bool Class::getBartersRepairTools() const {
		return getServiceFlag(ServiceFlag::BartersRepairTools);
	}

	void Class::setBartersRepairTools(bool value) {
		setServiceFlag(ServiceFlag::BartersRepairTools, value);
	}

	bool Class::getBartersWeapons() const {
		return getServiceFlag(ServiceFlag::BartersWeapons);
	}

	void Class::setBartersWeapons(bool value) {
		setServiceFlag(ServiceFlag::BartersWeapons, value);
	}

	bool Class::getOffersBartering() const {
		return services & ServiceFlag::OffersBarteringMask;
	}

	bool Class::getOffersEnchanting() const {
		return getServiceFlag(ServiceFlag::OffersEnchanting);
	}

	void Class::setOffersEnchanting(bool value) {
		setServiceFlag(ServiceFlag::OffersEnchanting, value);
	}

	bool Class::getOffersRepairs() const {
		return getServiceFlag(ServiceFlag::OffersRepairs);
	}

	void Class::setOffersRepairs(bool value) {
		setServiceFlag(ServiceFlag::OffersRepairs, value);
	}

	bool Class::getOffersSpellmaking() const {
		return getServiceFlag(ServiceFlag::OffersSpellmaking);
	}

	void Class::setOffersSpellmaking(bool value) {
		setServiceFlag(ServiceFlag::OffersSpellmaking, value);
	}

	bool Class::getOffersSpells() const {
		return getServiceFlag(ServiceFlag::OffersSpells);
	}

	void Class::setOffersSpells(bool value) {
		setServiceFlag(ServiceFlag::OffersSpells, value);
	}

	bool Class::getOffersTraining() const {
		return getServiceFlag(ServiceFlag::OffersTraining);
	}

	void Class::setOffersTraining(bool value) {
		setServiceFlag(ServiceFlag::OffersTraining, value);
	}

	void Class::setDescription_lua(const char* value) {
		setDescription(value);
		setObjectModified(true);
	}

	sol::table Class::getMajorSkills_lua(sol::this_state ts) {
		sol::state_view state = ts;
		sol::table result = state.create_table();
		for (int i = 0; i < 5; ++i) {
			result[i + 1] = skills[i * 2 + 1];
		}
		return result;
	}

	sol::table Class::getMinorSkills_lua(sol::this_state ts) {
		sol::state_view state = ts;
		sol::table result = state.create_table();
		for (int i = 0; i < 5; ++i) {
			result[i + 1] = skills[i * 2];
		}
		return result;
	}

}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::Class)
