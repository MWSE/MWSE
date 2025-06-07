#include "ObjectWindowTabData.h"

#include "CSApparatus.h"
#include "CSArmor.h"
#include "CSBodyPart.h"
#include "CSBook.h"
#include "CSClothing.h"
#include "CSContainer.h"
#include "CSCreature.h"
#include "CSDataHandler.h"
#include "CSEnchantment.h"
#include "CSFaction.h"
#include "CSGameSetting.h"
#include "CSIngredient.h"
#include "CSEffect.h"
#include "CSLeveledCreature.h"
#include "CSLeveledItem.h"
#include "CSLight.h"
#include "CSLockpick.h"
#include "CSNPC.h"
#include "CSProbe.h"
#include "CSRecordHandler.h"
#include "CSRepairTool.h"
#include "CSScript.h"
#include "CSSkill.h"
#include "CSSound.h"
#include "CSSpell.h"
#include "CSWeapon.h"

#include "StringUtil.h"

namespace se::cs::dialog::object_window {

	//
	// TabColumnParam
	//

	TabColumnParam::operator WPARAM() const {
		return data;
	}

	bool TabColumnParam::getSortAsc() const {
		return (data & 1u) == 1u;
	}

	void TabColumnParam::setSortAsc(bool asc) {
		if (asc) {
			data |= 1u;
		}
		else {
			data &= ~1u;
		}
	}

	void TabColumnParam::toggleSortOrder() {
		data ^= 1u;
	}

	WPARAM TabColumnParam::getSortColumn() const {
		return data >> 1u;
	}

	void TabColumnParam::setSortColumn(WPARAM column) {
		const auto sortAsc = getSortAsc();

		data = column << 1u;

		setSortAsc(sortAsc);
	}

	WPARAM& TabColumnParam::getData() {
		return data;
	}

	TabColumnParam* TabColumnParam::get() {
		return reinterpret_cast<TabColumnParam*>(0x6CEEA0);
	}

	TabColumnParam& TabColumnParam::get(byte tab) {
		const auto params = get();
		return params[tab];
	}

	//
	// TabColumn
	//

	TabColumn::TabColumn(const char* title, int fmt) {
		m_Title = title;
		m_Format = fmt;
	}

	void TabColumn::insert(HWND hWnd, size_t index) const {
		LVCOLUMNA uiColData = {};
		uiColData.pszText = (LPTSTR)m_Title;
		uiColData.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT | LVCF_SUBITEM;
		uiColData.cx = getSettings().width;
		uiColData.fmt = m_Format;
		uiColData.iSubItem = index;
		ListView_InsertColumn(hWnd, index, &uiColData);
	}

	void TabColumn::addToController(TabController* controller, HWND hWnd) {
		controller->columns.push_back(this);
		insert(hWnd, controller->columnsActive);
		++controller->columnsActive;
	}

	void TabColumn::conditionalAddToController(TabController* controller, HWND hWnd) {
		if (supportsObjectType(controller->objectType)) {
			addToController(controller, hWnd);
		}
	}

	const Object* TabColumn::getObjectFromDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		return reinterpret_cast<Object*>(displayInfo->item.lParam);
	}

	void TabColumn::display(LPNMLVDISPINFOA displayInfo, bool value) const {
		display(displayInfo, value ? "yes" : "no");
	}

	void TabColumn::display(LPNMLVDISPINFOA displayInfo, int value) const {
		sprintf_s(displayInfo->item.pszText, displayInfo->item.cchTextMax, "%d", value);
	}

	void TabColumn::display(LPNMLVDISPINFOA displayInfo, float value, const char* format) const {
		sprintf_s(displayInfo->item.pszText, displayInfo->item.cchTextMax, format, value);
	}

	void TabColumn::display(LPNMLVDISPINFOA displayInfo, const char* string) const {
		sprintf_s(displayInfo->item.pszText, displayInfo->item.cchTextMax, "%s", string);
	}

	void TabColumn::display(LPNMLVDISPINFOA displayInfo, const NI::IteratedList<ItemStack*>& items) const {
		if (items.empty()) {
			display(displayInfo, "-NONE-");
			return;
		}

		char* buffer = displayInfo->item.pszText;
		const auto bufferSize = displayInfo->item.cchTextMax;
		size_t bytesRemaining = bufferSize - 1;
		bool first = true;
		for (const auto& itt : items) {
			const std::string_view sv = itt->object->getObjectID();
			if (bytesRemaining <= sv.length() + 8) {
				strcat_s(buffer, bufferSize, ", ...");
				break;
			}

			if (!first) {
				strcat_s(buffer, bufferSize, ", ");
				bytesRemaining -= 2;
			}

			strcat_s(buffer, bufferSize, sv.data());
			bytesRemaining -= sv.length();
			first = false;
		}
	}

	void TabColumn::display(LPNMLVDISPINFOA displayInfo, const NI::IteratedList<LeveledList::Node*>* list) const {
		if (list->empty()) {
			display(displayInfo, "-NONE-");
			return;
		}

		char* buffer = displayInfo->item.pszText;
		const auto bufferSize = displayInfo->item.cchTextMax;
		size_t bytesRemaining = bufferSize - 1;
		bool first = true;
		for (const auto& itt : *list) {
			const std::string_view sv = itt->object->getObjectID();
			if (bytesRemaining <= sv.length() + 8) {
				strcat_s(buffer, bufferSize, ", ...");
				break;
			}

			if (!first) {
				strcat_s(buffer, bufferSize, ", ");
				bytesRemaining -= 2;
			}

			strcat_s(buffer, bufferSize, sv.data());
			bytesRemaining -= sv.length();
			first = false;
		}
	}

	int TabColumn::sort(bool a, bool b, bool sortOrderAsc) const {
		if (a == b) {
			return 0;
		}

		if (sortOrderAsc) {
			return a ? 1 : -1;
		}
		else {
			return a ? -1 : 1;
		}
	}

	int TabColumn::sort(int a, int b, bool sortOrderAsc) const {
		if (a == b) {
			return 0;
		}

		if (sortOrderAsc) {
			return a > b ? 1 : -1;
		}
		else {
			return a < b ? 1 : -1;
		}
	}

	int TabColumn::sort(unsigned int a, unsigned int b, bool sortOrderAsc) const {
		if (a == b) {
			return 0;
		}

		if (sortOrderAsc) {
			return a > b ? 1 : -1;
		}
		else {
			return a < b ? 1 : -1;
		}
	}

	int TabColumn::sort(float a, float b, bool sortOrderAsc) const {
		if (a == b) {
			return 0;
		}

		if (sortOrderAsc) {
			return a > b ? 1 : -1;
		}
		else {
			return a < b ? 1 : -1;
		}
	}

	int TabColumn::sort(const char* a, const char* b, bool sortOrderAsc) const {
		auto cmp = _stricmp(a, b);
		return sortOrderAsc ? cmp : -cmp;
	}

	int TabColumn::sort(const NI::IteratedList<ItemStack*>& a, const NI::IteratedList<ItemStack*>& b, bool sortOrderAsc) const {
		const auto smallestListSize = std::min(a.size(), b.size());
		for (auto i = 0u; i < smallestListSize; ++i) {
			auto r = sort(a[i]->object->getObjectID(), b[i]->object->getObjectID(), sortOrderAsc);
			if (r != 0) {
				return r;
			}
		}

		return sort(a.size(), b.size(), sortOrderAsc);
	}

	int TabColumn::sort(const NI::IteratedList<LeveledList::Node*>* a, const NI::IteratedList<LeveledList::Node*>* b, bool sortOrderAsc) const {
		const auto smallestListSize = std::min(a->size(), b->size());
		for (auto i = 0u; i < smallestListSize; ++i) {
			auto r = sort(a->at(i)->object->getObjectID(), b->at(i)->object->getObjectID(), sortOrderAsc);
			if (r != 0) {
				return r;
			}
		}

		return sort(a->size(), b->size(), sortOrderAsc);
	}
	
	//
	// Column: Actor Class
	//

	TabColumnActorClass::TabColumnActorClass() : TabColumn("Class") {

	}

	bool TabColumnActorClass::supportsObjectType(ObjectType::ObjectType objectType) const {
		switch (objectType) {
		case ObjectType::Creature:
		case ObjectType::NPC:
			return true;
		}
		return false;
	}

	void TabColumnActorClass::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = getObjectFromDisplayInfo(displayInfo);
		display(displayInfo, object->getClassName());
	}

	int TabColumnActorClass::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		return sort(lParam1->getClassName(), lParam2->getClassName(), sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnActorClass::getSettings() const {
		return settings.object_window.column_actor_class;
	}

	//
	// Column: Actor Essential
	//

	TabColumnActorEssential::TabColumnActorEssential() : TabColumn("Essential", LVCFMT_CENTER) {

	}

	bool TabColumnActorEssential::supportsObjectType(ObjectType::ObjectType objectType) const {
		return objectType == ObjectType::Container;
	}

	void TabColumnActorEssential::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		const auto object = static_cast<const Container*>(getObjectFromDisplayInfo(displayInfo));
		display(displayInfo, object->getIsEssential());
	}

	int TabColumnActorEssential::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		const auto a = static_cast<const Container*>(lParam1);
		const auto b = static_cast<const Container*>(lParam2);
		return sort(a->getIsEssential(), b->getIsEssential(), sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnActorEssential::getSettings() const {
		return settings.object_window.column_actor_essential;
	}

	//
	// Column: Actor Faction
	//

	TabColumnActorFaction::TabColumnActorFaction() : TabColumn("Faction") {

	}

	bool TabColumnActorFaction::supportsObjectType(ObjectType::ObjectType objectType) const {
		return objectType == ObjectType::NPC;
	}

	void TabColumnActorFaction::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = getObjectFromDisplayInfo(displayInfo);
		display(displayInfo, object->getFactionName());
	}

	int TabColumnActorFaction::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		return sort(lParam1->getFactionName(), lParam2->getFactionName(), sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnActorFaction::getSettings() const {
		return settings.object_window.column_actor_faction;
	}

	//
	// Column: Actor Faction Rank
	//

	TabColumnActorFactionRank::TabColumnActorFactionRank() : TabColumn("Rank") {

	}

	bool TabColumnActorFactionRank::supportsObjectType(ObjectType::ObjectType objectType) const {
		return objectType == ObjectType::NPC;
	}

	void TabColumnActorFactionRank::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = static_cast<const NPC*>(getObjectFromDisplayInfo(displayInfo));
		display(displayInfo, object->getFactionRankName());
	}

	int TabColumnActorFactionRank::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		const auto a = static_cast<const NPC*>(lParam1);
		const auto b = static_cast<const NPC*>(lParam2);
		return sort(a->factionRank, b->factionRank, sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnActorFactionRank::getSettings() const {
		return settings.object_window.column_actor_faction_rank;
	}

	//
	// Column: Actor training
	//

	TabColumnTraining::TabColumnTraining() : TabColumn("Training") {

	}

	bool TabColumnTraining::supportsObjectType(ObjectType::ObjectType objectType) const {
		return objectType == ObjectType::NPC;
	}

	void TabColumnTraining::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		char buffer[256];
		auto object = static_cast<const NPC*>(getObjectFromDisplayInfo(displayInfo));
		object->getTraining(buffer, sizeof(buffer));
		display(displayInfo, buffer);
	}

	int TabColumnTraining::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		char bufferA[256];
		char bufferB[256];
		const auto a = static_cast<const NPC*>(lParam1);
		const auto b = static_cast<const NPC*>(lParam2);
		a->getTraining(bufferA, sizeof(bufferA));
		b->getTraining(bufferB, sizeof(bufferB));
  		return sort(bufferA, bufferB, sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnTraining::getSettings() const {
		return settings.object_window.column_actor_faction_rank;
	}

	//
	// Column: Actor fight
	//

	TabColumnFight::TabColumnFight() : TabColumn("Fight") {

	}

	bool TabColumnFight::supportsObjectType(ObjectType::ObjectType objectType) const {
		return objectType == ObjectType::NPC;
	}

	void TabColumnFight::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = static_cast<const NPC*>(getObjectFromDisplayInfo(displayInfo));
		display(displayInfo, object->getFight());
	}

	int TabColumnFight::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		const auto a = static_cast<const NPC*>(lParam1);
		const auto b = static_cast<const NPC*>(lParam2);
		return sort(a->getFight(), b->getFight(), sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnFight::getSettings() const {
		return settings.object_window.column_actor_faction_rank;
	}

	//
	// Column: Actor Inventory
	//

	TabColumnActorInventory::TabColumnActorInventory() : TabColumn("Item List") {

	}

	bool TabColumnActorInventory::supportsObjectType(ObjectType::ObjectType objectType) const {
		switch (objectType) {
		case ObjectType::Creature:
		case ObjectType::Container:
		case ObjectType::NPC:
			return true;
		}
		return false;
	}

	void TabColumnActorInventory::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		const auto object = static_cast<const Actor*>(getObjectFromDisplayInfo(displayInfo));
		display(displayInfo, object->inventory.items);
	}

	int TabColumnActorInventory::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		const auto& a = static_cast<const Actor*>(lParam1);
		const auto& b = static_cast<const Actor*>(lParam2);
		return sort(a->inventory.items, b->inventory.items, sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnActorInventory::getSettings() const {
		return settings.object_window.column_actor_item_list;
	}

	//
	// Column: Actor Level
	//

	TabColumnActorLevel::TabColumnActorLevel() : TabColumn("Level", LVCFMT_CENTER) {

	}

	bool TabColumnActorLevel::supportsObjectType(ObjectType::ObjectType objectType) const {
		switch (objectType) {
		case ObjectType::Creature:
		case ObjectType::NPC:
			return true;
		}
		return false;
	}

	void TabColumnActorLevel::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = getObjectFromDisplayInfo(displayInfo);
		display(displayInfo, object->getLevel());
	}

	int TabColumnActorLevel::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		return sort(lParam1->getLevel(), lParam2->getLevel(), sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnActorLevel::getSettings() const {
		return settings.object_window.column_actor_level;
	}

	//
	// Column: Actor Respawns
	//

	TabColumnActorRespawns::TabColumnActorRespawns() : TabColumn("Respawns", LVCFMT_CENTER) {

	}

	bool TabColumnActorRespawns::supportsObjectType(ObjectType::ObjectType objectType) const {
		switch (objectType) {
		case ObjectType::Creature:
		case ObjectType::NPC:
		case ObjectType::Container:
			return true;
		}
		return false;
	}

	void TabColumnActorRespawns::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		const auto object = getObjectFromDisplayInfo(displayInfo);
		display(displayInfo, object->getRespawns());
	}

	int TabColumnActorRespawns::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		return sort(lParam1->getRespawns(), lParam2->getRespawns(), sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnActorRespawns::getSettings() const {
		return settings.object_window.column_actor_respawns;
	}

	//
	// Column: All <= PC
	//

	TabColumnAllLTEPC::TabColumnAllLTEPC() : TabColumn("All <= PC", LVCFMT_CENTER) {

	}

	bool TabColumnAllLTEPC::supportsObjectType(ObjectType::ObjectType objectType) const {
		switch (objectType) {
		case ObjectType::LeveledCreature:
		case ObjectType::LeveledItem:
			return true;
		}
		return false;
	}

	void TabColumnAllLTEPC::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = getObjectFromDisplayInfo(displayInfo);
		bool allLTEPC = false;
		switch (object->objectType) {
		case ObjectType::LeveledCreature:
			allLTEPC = static_cast<const LeveledCreature*>(object)->getCalculateFromAllLevelsLessThanEqualToPCLevel();
			break;
		case ObjectType::LeveledItem:
			allLTEPC = static_cast<const LeveledItem*>(object)->getCalculateFromAllLevelsLessThanEqualToPCLevel();
			break;
		}

		display(displayInfo, allLTEPC);
	}

	int TabColumnAllLTEPC::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		bool a = false;
		bool b = false;
		switch (lParam1->objectType) {
		case ObjectType::LeveledCreature:
			a = static_cast<const LeveledCreature*>(lParam1)->getCalculateFromAllLevelsLessThanEqualToPCLevel();
			b = static_cast<const LeveledCreature*>(lParam2)->getCalculateFromAllLevelsLessThanEqualToPCLevel();
			break;
		case ObjectType::LeveledItem:
			a = static_cast<const LeveledItem*>(lParam1)->getCalculateFromAllLevelsLessThanEqualToPCLevel();
			b = static_cast<const LeveledItem*>(lParam2)->getCalculateFromAllLevelsLessThanEqualToPCLevel();
			break;
		}

		return sort(a, b, sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnAllLTEPC::getSettings() const {
		return settings.object_window.column_all_lte_pc;
	}

	//
	// Column: Animation
	//

	TabColumnAnimation::TabColumnAnimation() : TabColumn("Animation") {

	}

	bool TabColumnAnimation::supportsObjectType(ObjectType::ObjectType objectType) const {
		return objectType == ObjectType::NPC;
	}

	void TabColumnAnimation::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = getObjectFromDisplayInfo(displayInfo);
		auto animation = object->getModel();

		if (DataHandler::get()->recordHandler->isBaseAnimation(animation)) {
			return;
		}

		display(displayInfo, animation);
	}

	int TabColumnAnimation::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		return sort(lParam1->getModel(), lParam2->getModel(), sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnAnimation::getSettings() const {
		return settings.object_window.column_animation;
	}

	//
	// Column: Armor Rating
	//

	TabColumnArmorRating::TabColumnArmorRating() : TabColumn("Rating", LVCFMT_CENTER) {

	}

	bool TabColumnArmorRating::supportsObjectType(ObjectType::ObjectType objectType) const {
		return objectType == ObjectType::Armor;
	}

	void TabColumnArmorRating::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = static_cast<const Armor*>(getObjectFromDisplayInfo(displayInfo));
		display(displayInfo, object->armorRating);
	}

	int TabColumnArmorRating::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		auto a = static_cast<const Armor*>(lParam1);
		auto b = static_cast<const Armor*>(lParam2);
		return sort(a->armorRating, b->armorRating, sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnArmorRating::getSettings() const {
		return settings.object_window.column_rating;
	}

	//
	// Column: Auto Calc
	//

	TabColumnAutoCalc::TabColumnAutoCalc() : TabColumn("AutoCalc", LVCFMT_CENTER) {

	}

	bool TabColumnAutoCalc::supportsObjectType(ObjectType::ObjectType objectType) const {
		switch (objectType) {
		case ObjectType::Alchemy:
		case ObjectType::Enchantment:
		case ObjectType::NPC:
		case ObjectType::Spell:
			return true;
		}
		return false;
	}

	void TabColumnAutoCalc::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = getObjectFromDisplayInfo(displayInfo);
		display(displayInfo, object->getAutoCalc());
	}

	int TabColumnAutoCalc::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		return sort(lParam1->getAutoCalc(), lParam2->getAutoCalc(), sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnAutoCalc::getSettings() const {
		return settings.object_window.column_autocalc;
	}

	//
	// Column: Chance None
	//

	TabColumnChanceNone::TabColumnChanceNone() : TabColumn("Chance for Nothing", LVCFMT_CENTER) {

	}

	bool TabColumnChanceNone::supportsObjectType(ObjectType::ObjectType objectType) const {
		switch (objectType) {
		case ObjectType::LeveledCreature:
		case ObjectType::LeveledItem:
			return true;
		}
		return false;
	}

	void TabColumnChanceNone::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = getObjectFromDisplayInfo(displayInfo);
		switch (object->objectType) {
		case ObjectType::LeveledCreature:
			display(displayInfo, static_cast<const LeveledCreature*>(object)->chanceForNone);
			break;
		case ObjectType::LeveledItem:
			display(displayInfo, static_cast<const LeveledItem*>(object)->chanceForNone);
			break;
		}
	}

	int TabColumnChanceNone::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		int chanceNone = 0;
		switch (lParam1->objectType) {
		case ObjectType::LeveledCreature:
			return sort(static_cast<const LeveledCreature*>(lParam1)->chanceForNone, static_cast<const LeveledCreature*>(lParam2)->chanceForNone, sortOrderAsc);
		case ObjectType::LeveledItem:
			return sort(static_cast<const LeveledItem*>(lParam1)->chanceForNone, static_cast<const LeveledItem*>(lParam2)->chanceForNone, sortOrderAsc);
		}
		return 0;
	}

	TabColumn::ColumnSettings& TabColumnChanceNone::getSettings() const {
		return settings.object_window.column_chance_for_none;
	}

	//
	// Column: Creature Is Bipedal
	//

	TabColumnCreatureIsBipedal::TabColumnCreatureIsBipedal() : TabColumn("Bipedal", LVCFMT_CENTER) {

	}

	bool TabColumnCreatureIsBipedal::supportsObjectType(ObjectType::ObjectType objectType) const {
		return objectType == ObjectType::Creature;
	}

	void TabColumnCreatureIsBipedal::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = static_cast<const Creature*>(getObjectFromDisplayInfo(displayInfo));
		display(displayInfo, object->getIsBipedal());
	}

	int TabColumnCreatureIsBipedal::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		const auto a = static_cast<const Creature*>(lParam1)->getIsBipedal();
		const auto b = static_cast<const Creature*>(lParam2)->getIsBipedal();
		return sort(a, b, sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnCreatureIsBipedal::getSettings() const {
		return settings.object_window.column_creature_bipedal;
	}

	//
	// Column: Creature Movement Type
	//

	TabColumnCreatureMovementType::TabColumnCreatureMovementType() : TabColumn("Movement Type", LVCFMT_CENTER) {

	}

	bool TabColumnCreatureMovementType::supportsObjectType(ObjectType::ObjectType objectType) const {
		return objectType == ObjectType::Creature;
	}

	void TabColumnCreatureMovementType::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = static_cast<const Creature*>(getObjectFromDisplayInfo(displayInfo));
		display(displayInfo, object->getMovementType());
	}

	int TabColumnCreatureMovementType::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		const auto a = static_cast<const Creature*>(lParam1)->getMovementType();
		const auto b = static_cast<const Creature*>(lParam2)->getMovementType();
		return sort(a, b, sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnCreatureMovementType::getSettings() const {
		return settings.object_window.column_creature_movement_type;
	}

	//
	// Column: Creature Soul Value
	//

	TabColumnCreatureSoulValue::TabColumnCreatureSoulValue() : TabColumn("Soul", LVCFMT_CENTER) {

	}

	bool TabColumnCreatureSoulValue::supportsObjectType(ObjectType::ObjectType objectType) const {
		return objectType == ObjectType::Creature;
	}

	void TabColumnCreatureSoulValue::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = static_cast<const Creature*>(getObjectFromDisplayInfo(displayInfo));
		display(displayInfo, object->soul);
	}

	int TabColumnCreatureSoulValue::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		const auto a = static_cast<const Creature*>(lParam1)->soul;
		const auto b = static_cast<const Creature*>(lParam2)->soul;
		return sort(a, b, sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnCreatureSoulValue::getSettings() const {
		return settings.object_window.column_creature_soul;
	}

	//
	// Column: Creature Sound
	//

	TabColumnCreatureSound::TabColumnCreatureSound() : TabColumn("Sound") {

	}

	bool TabColumnCreatureSound::supportsObjectType(ObjectType::ObjectType objectType) const {
		return objectType == ObjectType::Creature;
	}

	void TabColumnCreatureSound::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = static_cast<const Creature*>(getObjectFromDisplayInfo(displayInfo));
		const auto soundCreature = object->soundGenerator;
		display(displayInfo, soundCreature ? soundCreature->getObjectID() : "");
	}

	int TabColumnCreatureSound::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		const auto a = static_cast<const Creature*>(lParam1)->soundGenerator;
		const auto b = static_cast<const Creature*>(lParam2)->soundGenerator;
		return sort(a ? a->getObjectID() : "", b ? b->getObjectID() : "", sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnCreatureSound::getSettings() const {
		return settings.object_window.column_creature_sound;
	}

	//
	// Column: Creature Uses Weapon and Shield
	//

	TabColumnCreatureUsesWeaponAndShield::TabColumnCreatureUsesWeaponAndShield() : TabColumn("WeaponAndShield", LVCFMT_CENTER) {

	}

	bool TabColumnCreatureUsesWeaponAndShield::supportsObjectType(ObjectType::ObjectType objectType) const {
		return objectType == ObjectType::Creature;
	}

	void TabColumnCreatureUsesWeaponAndShield::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = static_cast<const Creature*>(getObjectFromDisplayInfo(displayInfo));
		display(displayInfo, object->getUsesWeaponAndShield());
	}

	int TabColumnCreatureUsesWeaponAndShield::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		const auto a = static_cast<const Creature*>(lParam1)->getUsesWeaponAndShield();
		const auto b = static_cast<const Creature*>(lParam2)->getUsesWeaponAndShield();
		return sort(a, b, sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnCreatureUsesWeaponAndShield::getSettings() const {
		return settings.object_window.column_creature_use_weapon_and_shield;
	}

	//
	// Column: Blocked
	//

	TabColumnBlocked::TabColumnBlocked() : TabColumn("Blocked", LVCFMT_CENTER) {

	}

	bool TabColumnBlocked::supportsObjectType(ObjectType::ObjectType objectType) const {
		return true;
	}

	void TabColumnBlocked::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = getObjectFromDisplayInfo(displayInfo);
		display(displayInfo, object->getBlocked());
	}

	int TabColumnBlocked::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		return sort(lParam1->getBlocked(), lParam2->getBlocked(), sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnBlocked::getSettings() const {
		return settings.object_window.column_blocked;
	}

	//
	// Column: Book is Scroll
	//

	TabColumnBookIsScroll::TabColumnBookIsScroll() : TabColumn("Scroll", LVCFMT_CENTER) {

	}

	bool TabColumnBookIsScroll::supportsObjectType(ObjectType::ObjectType objectType) const {
		return objectType == ObjectType::Book;
	}

	void TabColumnBookIsScroll::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = static_cast<const Book*>(getObjectFromDisplayInfo(displayInfo));
		display(displayInfo, object->isScroll);
	}

	int TabColumnBookIsScroll::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		const auto a = static_cast<const Book*>(lParam1);
		const auto b = static_cast<const Book*>(lParam2);
		return sort(a->isScroll, b->isScroll, sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnBookIsScroll::getSettings() const {
		return settings.object_window.column_book_is_scroll;
	}

	//
	// Column: Book Teaches
	//

	TabColumnBookTeaches::TabColumnBookTeaches() : TabColumn("Teaches") {

	}

	bool TabColumnBookTeaches::supportsObjectType(ObjectType::ObjectType objectType) const {
		return objectType == ObjectType::Book;
	}

	void TabColumnBookTeaches::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		const auto object = static_cast<const Book*>(getObjectFromDisplayInfo(displayInfo));
		display(displayInfo, object->getTaughtSkillName());
	}

	int TabColumnBookTeaches::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		const auto a = static_cast<const Book*>(lParam1);
		const auto b = static_cast<const Book*>(lParam2);
		return sort(a->getTaughtSkillName(), b->getTaughtSkillName(), sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnBookTeaches::getSettings() const {
		return settings.object_window.column_book_teaches;
	}

	//
	// Column: Cost
	//

	int GetObjectCost(const Object* object) {
		if (object->objectType == ObjectType::Enchantment) {
			return static_cast<const Enchantment*>(object)->chargeCost;
		}
		else if (object->objectType == ObjectType::Spell) {
			return static_cast<const Spell*>(object)->magickaCost;
		}
		return 0;
	}

	TabColumnCost::TabColumnCost() : TabColumn("Cost", LVCFMT_CENTER) {

	}

	bool TabColumnCost::supportsObjectType(ObjectType::ObjectType objectType) const {
		switch (objectType) {
		case ObjectType::Spell:
		case ObjectType::Enchantment:
			return true;
		}
		return false;
	}

	void TabColumnCost::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = getObjectFromDisplayInfo(displayInfo);
		display(displayInfo, GetObjectCost(object));
	}

	int TabColumnCost::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		return sort(GetObjectCost(lParam1), GetObjectCost(lParam2), sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnCost::getSettings() const {
		return settings.object_window.column_cost;
	}

	//
	// Column: Count
	//

	TabColumnCount::TabColumnCount() : TabColumn("Count", LVCFMT_CENTER) {

	}

	bool TabColumnCount::supportsObjectType(ObjectType::ObjectType objectType) const {
		switch (objectType) {
		case ObjectType::Bodypart:
			return false;
		}
		return true;
	}

	void TabColumnCount::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = getObjectFromDisplayInfo(displayInfo);
		display(displayInfo, object->getCount());
	}

	int TabColumnCount::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		return sort(lParam1->getCount(), lParam2->getCount(), sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnCount::getSettings() const {
		return settings.object_window.column_count;
	}

	//
	// Column: Effect
	//

	void GetEffectName(char* buffer, size_t bufferSize, const Object* object, int index) {
		switch (object->objectType) {
		case ObjectType::Ingredient:
			static_cast<const Ingredient*>(object)->getEffectName(buffer, bufferSize, index);
			break;
		default:
			*buffer = '\0';
		}
	}

	TabColumnEffect::TabColumnEffect(int index) : TabColumn("Effect", LVCFMT_CENTER) {
		m_EffectIndex = index;

		constexpr auto titleLength = sizeof"Effect X";
		char* title = new char[titleLength];
		sprintf_s(title, titleLength, "Effect %d", m_EffectIndex + 1);
		m_Title = title;
	}

	bool TabColumnEffect::supportsObjectType(ObjectType::ObjectType objectType) const {
		switch (objectType) {
		case ObjectType::Ingredient:
			return true;
		}
		return false;
	}

	void TabColumnEffect::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = getObjectFromDisplayInfo(displayInfo);
		char buffer[64];
		GetEffectName(buffer, sizeof(buffer), object, m_EffectIndex);
		display(displayInfo, buffer);
	}

	int TabColumnEffect::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		char a[64];
		char b[64];
		GetEffectName(a, sizeof(a), lParam1, m_EffectIndex);
		GetEffectName(b, sizeof(b), lParam2, m_EffectIndex);
		return sort(a, b, sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnEffect::getSettings() const {
		return settings.object_window.column_effect;
	}

	//
	// Column: Enchanting
	//

	TabColumnEnchanting::TabColumnEnchanting() : TabColumn("Enchanting", LVCFMT_CENTER) {

	}

	bool TabColumnEnchanting::supportsObjectType(ObjectType::ObjectType objectType) const {
		switch (objectType) {
		case ObjectType::Armor:
		case ObjectType::Book:
		case ObjectType::Clothing:
		case ObjectType::Weapon:
			return true;
		}
		return false;
	}

	void TabColumnEnchanting::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = getObjectFromDisplayInfo(displayInfo);
		auto enchantment = object->getEnchantment();
		if (enchantment) {
			display(displayInfo, enchantment->getObjectID());
		}
	}

	int TabColumnEnchanting::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		const auto a = lParam1->getEnchantment();
		const auto b = lParam2->getEnchantment();
		return sort(a ? a->getObjectID() : "", b ? b->getObjectID() : "", sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnEnchanting::getSettings() const {
		return settings.object_window.column_enchanting;
	}

	//
	// Column: Enchantment
	//

	int GetObjectEnchantment(const Object* object) {
		switch (object->objectType) {
		case ObjectType::Armor:
			return static_cast<const Armor*>(object)->enchantPoints;
		case ObjectType::Book:
			return static_cast<const Book*>(object)->enchantPoints;
		case ObjectType::Clothing:
			return static_cast<const Clothing*>(object)->enchantPoints;
		case ObjectType::Weapon:
			return static_cast<const Weapon*>(object)->enchantPoints;
		}
		return 0;
	}

	TabColumnEnchantment::TabColumnEnchantment() : TabColumn("Enchantment", LVCFMT_CENTER) {

	}

	bool TabColumnEnchantment::supportsObjectType(ObjectType::ObjectType objectType) const {
		switch (objectType) {
		case ObjectType::Armor:
		case ObjectType::Book:
		case ObjectType::Clothing:
		case ObjectType::Weapon:
			return true;
		}
		return false;
	}

	void TabColumnEnchantment::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = getObjectFromDisplayInfo(displayInfo);
		display(displayInfo, GetObjectEnchantment(object));
	}

	int TabColumnEnchantment::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		return sort(GetObjectEnchantment(lParam1), GetObjectEnchantment(lParam2), sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnEnchantment::getSettings() const {
		return settings.object_window.column_enchantment;
	}

	//
	// Column: Enchantment Charge
	//

	TabColumnEnchantmentCharge::TabColumnEnchantmentCharge() : TabColumn("Charge", LVCFMT_CENTER) {

	}

	bool TabColumnEnchantmentCharge::supportsObjectType(ObjectType::ObjectType objectType) const {
		return objectType == ObjectType::Enchantment;
	}

	void TabColumnEnchantmentCharge::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = static_cast<const Enchantment*>(getObjectFromDisplayInfo(displayInfo));
		display(displayInfo, object->maxCharge);
	}

	int TabColumnEnchantmentCharge::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		const auto a = static_cast<const Enchantment*>(lParam1);
		const auto b = static_cast<const Enchantment*>(lParam2);
		return sort(a->maxCharge, b->maxCharge, sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnEnchantmentCharge::getSettings() const {
		return settings.object_window.column_charge;
	}

	//
	// Column: Female
	//

	bool GetObjectIsFemale(const Object* object) {
		switch (object->objectType) {
		case ObjectType::Bodypart:
			return static_cast<const BodyPart*>(object)->getIsFemale();
		case ObjectType::NPC:
			return object->getIsFemale();
		}
		return false;
	}

	TabColumnFemale::TabColumnFemale() : TabColumn("Female", LVCFMT_CENTER) {

	}

	bool TabColumnFemale::supportsObjectType(ObjectType::ObjectType objectType) const {
		switch (objectType) {
		case ObjectType::Bodypart:
		case ObjectType::NPC:
			return true;
		}
		return false;
	}

	void TabColumnFemale::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = static_cast<const Creature*>(getObjectFromDisplayInfo(displayInfo));
		display(displayInfo, GetObjectIsFemale(object));
	}

	int TabColumnFemale::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		return sort(GetObjectIsFemale(lParam1), GetObjectIsFemale(lParam2), sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnFemale::getSettings() const {
		return settings.object_window.column_female;
	}

	//
	// Column: Health
	//

	int GetObjectHealth(const Object* object) {
		switch (object->objectType) {
		case ObjectType::Armor:
			return static_cast<const Armor*>(object)->maxCondition;
		case ObjectType::Weapon:
			return static_cast<const Weapon*>(object)->maxCondition;
		}
		return 0;
	}

	TabColumnHealth::TabColumnHealth() : TabColumn("Health", LVCFMT_CENTER) {

	}

	bool TabColumnHealth::supportsObjectType(ObjectType::ObjectType objectType) const {
		switch (objectType) {
		case ObjectType::Armor:
		case ObjectType::Weapon:
			return true;
		}
		return false;
	}

	void TabColumnHealth::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = getObjectFromDisplayInfo(displayInfo);
		display(displayInfo, GetObjectHealth(object));
	}

	int TabColumnHealth::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		return sort(GetObjectHealth(lParam1), GetObjectHealth(lParam2), sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnHealth::getSettings() const {
		return settings.object_window.column_health;
	}

	//
	// Column: ID
	//

	TabColumnID::TabColumnID() : TabColumn("ID") {

	}

	bool TabColumnID::supportsObjectType(ObjectType::ObjectType objectType) const {
		return true;
	}

	void TabColumnID::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = getObjectFromDisplayInfo(displayInfo);
		sprintf_s(displayInfo->item.pszText, displayInfo->item.cchTextMax, "%s", object->getObjectID());
	}

	int TabColumnID::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		return sort(lParam1->getObjectID(), lParam2->getObjectID(), sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnID::getSettings() const {
		return settings.object_window.column_id;
	}

	//
	// Column: Inventory
	//

	TabColumnInventory::TabColumnInventory() : TabColumn("Inventory") {

	}

	bool TabColumnInventory::supportsObjectType(ObjectType::ObjectType objectType) const {
		switch (objectType) {
		case ObjectType::Activator:
		case ObjectType::Bodypart:
		case ObjectType::Container:
		case ObjectType::Creature:
		case ObjectType::Door:
		case ObjectType::Enchantment:
		case ObjectType::LeveledCreature:
		case ObjectType::LeveledItem:
		case ObjectType::NPC:
		case ObjectType::Spell:
		case ObjectType::Static:
			return false;
		}
		return true;
	}

	void TabColumnInventory::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = getObjectFromDisplayInfo(displayInfo);
		display(displayInfo, object->getIcon());
	}

	int TabColumnInventory::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		return sort(lParam1->getIcon(), lParam2->getIcon(), sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnInventory::getSettings() const {
		return settings.object_window.column_inventory;
	}

	//
	// Column: Leveled Creature List
	//

	TabColumnCreatureList::TabColumnCreatureList() : TabColumn("Creature List") {

	}

	bool TabColumnCreatureList::supportsObjectType(ObjectType::ObjectType objectType) const {
		return objectType == ObjectType::LeveledCreature;
	}

	void TabColumnCreatureList::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		const auto object = static_cast<const LeveledCreature*>(getObjectFromDisplayInfo(displayInfo));
		display(displayInfo, object->list);
	}

	int TabColumnCreatureList::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		const auto a = static_cast<const LeveledCreature*>(lParam1);
		const auto b = static_cast<const LeveledCreature*>(lParam2);
		return sort(a->list, b->list, sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnCreatureList::getSettings() const {
		return settings.object_window.column_leveled_creature_list;
	}

	//
	// Column: Leveled Item List
	//

	TabColumnLeveledItemList::TabColumnLeveledItemList() : TabColumn("Item List") {

	}

	bool TabColumnLeveledItemList::supportsObjectType(ObjectType::ObjectType objectType) const {
		return objectType == ObjectType::LeveledItem;
	}

	void TabColumnLeveledItemList::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		const auto object = static_cast<const LeveledItem*>(getObjectFromDisplayInfo(displayInfo));
		display(displayInfo, object->list);
	}

	int TabColumnLeveledItemList::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		const auto a = static_cast<const LeveledItem*>(lParam1);
		const auto b = static_cast<const LeveledItem*>(lParam2);
		return sort(a->list, b->list, sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnLeveledItemList::getSettings() const {
		return settings.object_window.column_leveled_item_list;
	}

	//
	// Column: LightRadius
	//

	TabColumnLightRadius::TabColumnLightRadius() : TabColumn("Radius", LVCFMT_CENTER) {

	}

	bool TabColumnLightRadius::supportsObjectType(ObjectType::ObjectType objectType) const {
		return objectType == ObjectType::Light;
	}

	void TabColumnLightRadius::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		const auto object = static_cast<const Light*>(getObjectFromDisplayInfo(displayInfo));
		display(displayInfo, object->radius);
	}

	int TabColumnLightRadius::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		const auto& a = static_cast<const Light*>(lParam1);
		const auto& b = static_cast<const Light*>(lParam2);
		return sort(a->radius, b->radius, sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnLightRadius::getSettings() const {
		return settings.object_window.column_light_radius;
	}

	//
	// Column: LightTime
	//

	TabColumnLightTime::TabColumnLightTime() : TabColumn("Time", LVCFMT_CENTER) {

	}

	bool TabColumnLightTime::supportsObjectType(ObjectType::ObjectType objectType) const {
		return objectType == ObjectType::Light;
	}

	void TabColumnLightTime::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		const auto object = static_cast<const Light*>(getObjectFromDisplayInfo(displayInfo));
		display(displayInfo, object->time);
	}

	int TabColumnLightTime::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		const auto& a = static_cast<const Light*>(lParam1);
		const auto& b = static_cast<const Light*>(lParam2);
		return sort(a->time, b->time, sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnLightTime::getSettings() const {
		return settings.object_window.column_light_time;
	}

	//
	// Column: Model
	//

	TabColumnModel::TabColumnModel() : TabColumn("Model") {

	}

	bool TabColumnModel::supportsObjectType(ObjectType::ObjectType objectType) const {
		switch (objectType) {
		case ObjectType::Creature:
		case ObjectType::Enchantment:
		case ObjectType::LeveledCreature:
		case ObjectType::LeveledItem:
		case ObjectType::NPC:
		case ObjectType::Spell:
			return false;
		}
		return true;
	}

	void TabColumnModel::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = getObjectFromDisplayInfo(displayInfo);
		display(displayInfo, object->getModel());
	}

	int TabColumnModel::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		return sort(lParam1->getModel(), lParam2->getModel(), sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnModel::getSettings() const {
		return settings.object_window.column_model;
	}

	//
	// Column: Modified
	//

	TabColumnModified::TabColumnModified() : TabColumn("Modified", LVCFMT_CENTER) {

	}

	bool TabColumnModified::supportsObjectType(ObjectType::ObjectType objectType) const {
		return true;
	}

	void TabColumnModified::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = getObjectFromDisplayInfo(displayInfo);
		display(displayInfo, object->getModified());
	}

	int TabColumnModified::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		return sort(lParam1->getModified(), lParam2->getModified(), sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnModified::getSettings() const {
		return settings.object_window.column_modified;
	}

	//
	// Column: Name
	//

	TabColumnName::TabColumnName() : TabColumn("Name") {

	}

	bool TabColumnName::supportsObjectType(ObjectType::ObjectType objectType) const {
		switch (objectType) {
		case ObjectType::Bodypart:
		case ObjectType::Enchantment:
		case ObjectType::LeveledCreature:
		case ObjectType::LeveledItem:
		case ObjectType::Static:
			return false;
		}
		return true;
	}

	void TabColumnName::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = getObjectFromDisplayInfo(displayInfo);
		display(displayInfo, object->getName());
	}

	int TabColumnName::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		return sort(lParam1->getName(), lParam2->getName(), sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnName::getSettings() const {
		return settings.object_window.column_name;
	}

	//
	// Column: Part
	//

	TabColumnPart::TabColumnPart() : TabColumn("Part") {

	}

	bool TabColumnPart::supportsObjectType(ObjectType::ObjectType objectType) const {
		return objectType == ObjectType::Bodypart;
	}

	void TabColumnPart::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = static_cast<const BodyPart*>(getObjectFromDisplayInfo(displayInfo));
		display(displayInfo, object->getPartName());
	}

	int TabColumnPart::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		const auto a = static_cast<const BodyPart*>(lParam1);
		const auto b = static_cast<const BodyPart*>(lParam2);
		return sort(a->getPartName(), b->getPartName(), sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnPart::getSettings() const {
		return settings.object_window.column_part;
	}

	//
	// Column: Organic
	//

	TabColumnOrganic::TabColumnOrganic() : TabColumn("Organic", LVCFMT_CENTER) {

	}

	bool TabColumnOrganic::supportsObjectType(ObjectType::ObjectType objectType) const {
		return objectType == ObjectType::Container;
	}

	void TabColumnOrganic::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		const auto object = static_cast<const Container*>(getObjectFromDisplayInfo(displayInfo));
		display(displayInfo, object->getIsOrganic());
	}

	int TabColumnOrganic::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		const auto a = static_cast<const Container*>(lParam1);
		const auto b = static_cast<const Container*>(lParam2);
		return sort(a->getIsOrganic(), b->getIsOrganic(), sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnOrganic::getSettings() const {
		return settings.object_window.column_organic;
	}

	//
	// Column: PartType
	//

	TabColumnPartType::TabColumnPartType() : TabColumn("Type", LVCFMT_CENTER) {

	}

	bool TabColumnPartType::supportsObjectType(ObjectType::ObjectType objectType) const {
		return objectType == ObjectType::Bodypart;
	}

	void TabColumnPartType::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		const auto object = static_cast<const BodyPart*>(getObjectFromDisplayInfo(displayInfo));
		display(displayInfo, object->getTypeName());
	}

	int TabColumnPartType::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		const auto a = static_cast<const BodyPart*>(lParam1);
		const auto b = static_cast<const BodyPart*>(lParam2);
		return sort(a->getTypeName(), b->getTypeName(), sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnPartType::getSettings() const {
		return settings.object_window.column_part_type;
	}

	//
	// Column: PCStart
	//

	TabColumnPCStart::TabColumnPCStart() : TabColumn("PC Start", LVCFMT_CENTER) {

	}

	bool TabColumnPCStart::supportsObjectType(ObjectType::ObjectType objectType) const {
		return objectType == ObjectType::Spell;
	}

	void TabColumnPCStart::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = static_cast<const Spell*>(getObjectFromDisplayInfo(displayInfo));
		display(displayInfo, object->getPlayerStart());
	}

	int TabColumnPCStart::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		const auto a = static_cast<const Spell*>(lParam1);
		const auto b = static_cast<const Spell*>(lParam2);
		return sort(a->getPlayerStart(), b->getPlayerStart(), sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnPCStart::getSettings() const {
		return settings.object_window.column_spell_pc_start;
	}

	//
	// Column: Playable
	//

	TabColumnPlayable::TabColumnPlayable() : TabColumn("Playable", LVCFMT_CENTER) {

	}

	bool TabColumnPlayable::supportsObjectType(ObjectType::ObjectType objectType) const {
		return objectType == ObjectType::Bodypart;
	}

	void TabColumnPlayable::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = static_cast<const BodyPart*>(getObjectFromDisplayInfo(displayInfo));
		display(displayInfo, object->getIsPlayable());
	}

	int TabColumnPlayable::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		const auto a = static_cast<const BodyPart*>(lParam1);
		const auto b = static_cast<const BodyPart*>(lParam2);
		return sort(a->getIsPlayable(), b->getIsPlayable(), sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnPlayable::getSettings() const {
		return settings.object_window.column_playable;
	}

	//
	// Column: Persists
	//

	TabColumnPersists::TabColumnPersists() : TabColumn("Persists", LVCFMT_CENTER) {

	}

	bool TabColumnPersists::supportsObjectType(ObjectType::ObjectType objectType) const {
		return true;
	}

	void TabColumnPersists::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = getObjectFromDisplayInfo(displayInfo);
		display(displayInfo, object->getPersists());
	}

	int TabColumnPersists::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		return sort(lParam1->getPersists(), lParam2->getPersists(), sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnPersists::getSettings() const {
		return settings.object_window.column_persists;
	}

	//
	// Column: Quality
	//

	float GetObjectQuality(const Object* object) {
		switch (object->objectType) {
		case ObjectType::Apparatus:
			return static_cast<const Apparatus*>(object)->quality;
		case ObjectType::Lockpick:
			return static_cast<const Lockpick*>(object)->quality;
		case ObjectType::Probe:
			return static_cast<const Probe*>(object)->quality;
		case ObjectType::Repair:
			return static_cast<const RepairTool*>(object)->quality;
		}
		return 0.0f;
	}

	TabColumnQuality::TabColumnQuality() : TabColumn("Quality", LVCFMT_CENTER) {

	}

	bool TabColumnQuality::supportsObjectType(ObjectType::ObjectType objectType) const {
		switch (objectType) {
		case ObjectType::Apparatus:
		case ObjectType::Lockpick:
		case ObjectType::Probe:
		case ObjectType::Repair:
			return true;
		}
		return false;
	}

	void TabColumnQuality::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = static_cast<const Weapon*>(getObjectFromDisplayInfo(displayInfo));
		display(displayInfo, GetObjectQuality(object));
	}

	int TabColumnQuality::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		return sort(GetObjectQuality(lParam1), GetObjectQuality(lParam2), sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnQuality::getSettings() const {
		return settings.object_window.column_quality;
	}

	//
	// Column: Race
	//

	const char* GetRaceID(const Object* object) {
		if (object->objectType == ObjectType::Bodypart) {
			return static_cast<const BodyPart*>(object)->getRaceName();
		}
		return object->getRaceName();
	}

	TabColumnRace::TabColumnRace() : TabColumn("Race") {

	}

	bool TabColumnRace::supportsObjectType(ObjectType::ObjectType objectType) const {
		switch (objectType) {
		case ObjectType::Bodypart:
		case ObjectType::NPC:
			return true;
		}
		return false;
	}

	void TabColumnRace::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = getObjectFromDisplayInfo(displayInfo);
		display(displayInfo, GetRaceID(object));
	}

	int TabColumnRace::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		return sort(GetRaceID(lParam1), GetRaceID(lParam2), sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnRace::getSettings() const {
		return settings.object_window.column_race;
	}

	//
	// Column: Script
	//

	TabColumnScript::TabColumnScript() : TabColumn("Script") {

	}

	bool TabColumnScript::supportsObjectType(ObjectType::ObjectType objectType) const {
		switch (objectType) {
		case ObjectType::Bodypart:
		case ObjectType::Enchantment:
		case ObjectType::LeveledCreature:
		case ObjectType::LeveledItem:
		case ObjectType::Static:
			return false;
		}
		return true;
	}

	void TabColumnScript::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = getObjectFromDisplayInfo(displayInfo);
		auto script = object->getScript();
		if (script) {
			display(displayInfo, script->getObjectID());
		}
	}

	int TabColumnScript::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		const auto scriptA = lParam1->getScript();
		const auto scriptB = lParam2->getScript();
		return sort(scriptA ? scriptA->getObjectID() : "", scriptB ? scriptB->getObjectID() : "", sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnScript::getSettings() const {
		return settings.object_window.column_script;
	}

	//
	// Column: Sound
	//

	TabColumnSound::TabColumnSound() : TabColumn("Sound") {

	}

	bool TabColumnSound::supportsObjectType(ObjectType::ObjectType objectType) const {
		switch (objectType) {
		case ObjectType::Light:
			return true;
		}
		return false;
	}

	void TabColumnSound::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = getObjectFromDisplayInfo(displayInfo);
		auto sound = object->getSound();
		if (sound) {
			display(displayInfo, sound->getObjectID());
		}
	}

	int TabColumnSound::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		const auto soundA = lParam1->getSound();
		const auto soundB = lParam2->getSound();
		return sort(soundA ? soundA->getObjectID() : "", soundB ? soundB->getObjectID() : "", sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnSound::getSettings() const {
		return settings.object_window.column_sound;
	}

	//
	// Column: Spell Range
	//

	const char* GetSpellRange(const Spell* spell) {
		auto range = spell->effects[0].rangeType;
		for (auto i = 1u; i < 8; ++i) {
			const auto& effect = spell->effects[i];
			if (effect.effectID == -1) {
				break;
			}

			if (effect.rangeType != range) {
				range = Effect::Range::Invalid;
				break;
			}
		}

		switch (range) {
		case Effect::Range::Self:
			return "Self";
		case Effect::Range::Touch:
			return "Touch";
		case Effect::Range::Target:
			return "Target";
		default:
			return "Mixed";
		}
	}

	TabColumnSpellRange::TabColumnSpellRange() : TabColumn("Range", LVCFMT_CENTER) {

	}

	bool TabColumnSpellRange::supportsObjectType(ObjectType::ObjectType objectType) const {
		switch (objectType) {
		case ObjectType::Spell:
			return true;
		}
		return false;
	}

	void TabColumnSpellRange::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		const auto object = getObjectFromDisplayInfo(displayInfo);
		display(displayInfo, GetSpellRange(static_cast<const Spell*>(object)));
	}

	int TabColumnSpellRange::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		const auto a = static_cast<const Spell*>(lParam1);
		const auto b = static_cast<const Spell*>(lParam2);
		return sort(GetSpellRange(a), GetSpellRange(b), sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnSpellRange::getSettings() const {
		return settings.object_window.column_spell_range;
	}

	//
	// Column: Type
	//

	TabColumnType::TabColumnType() : TabColumn("Type", LVCFMT_CENTER) {

	}

	bool TabColumnType::supportsObjectType(ObjectType::ObjectType objectType) const {
		switch (objectType) {
		case ObjectType::Apparatus:
		case ObjectType::Armor:
		case ObjectType::Clothing:
		case ObjectType::Creature:
		case ObjectType::Enchantment:
		case ObjectType::Spell:
		case ObjectType::Weapon:
			return true;
		}
		return false;
	}

	void TabColumnType::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = getObjectFromDisplayInfo(displayInfo);
		display(displayInfo, object->getTypeName());
	}

	int TabColumnType::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		return sort(lParam1->getTypeName(), lParam2->getTypeName(), sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnType::getSettings() const {
		return settings.object_window.column_type;
	}

	//
	// Column: Uses
	//

	int GetObjectUses(const Object* object) {
		switch (object->objectType) {
		case ObjectType::Lockpick:
			return static_cast<const Lockpick*>(object)->uses;
		case ObjectType::Probe:
			return static_cast<const Probe*>(object)->uses;
		case ObjectType::Repair:
			return static_cast<const RepairTool*>(object)->uses;
		}
		return -1;
	}

	TabColumnUses::TabColumnUses() : TabColumn("Uses", LVCFMT_CENTER) {

	}

	bool TabColumnUses::supportsObjectType(ObjectType::ObjectType objectType) const {
		switch (objectType) {
		case ObjectType::Lockpick:
		case ObjectType::Probe:
		case ObjectType::Repair:
			return true;
		}
		return false;
	}

	void TabColumnUses::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = getObjectFromDisplayInfo(displayInfo);
		display(displayInfo, GetObjectUses(object));
	}

	int TabColumnUses::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		return sort(GetObjectUses(lParam1), GetObjectUses(lParam2), sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnUses::getSettings() const {
		return settings.object_window.column_uses;
	}

	//
	// Column: Value
	//

	TabColumnValue::TabColumnValue() : TabColumn("Value", LVCFMT_CENTER) {

	}

	bool TabColumnValue::supportsObjectType(ObjectType::ObjectType objectType) const {
		switch (objectType) {
		case ObjectType::Alchemy:
		case ObjectType::Apparatus:
		case ObjectType::Armor:
		case ObjectType::Book:
		case ObjectType::Clothing:
		case ObjectType::Ingredient:
		case ObjectType::Light:
		case ObjectType::Lockpick:
		case ObjectType::Misc:
		case ObjectType::Probe:
		case ObjectType::Repair:
		case ObjectType::Weapon:
			return true;
		}
		return false;
	}

	void TabColumnValue::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = getObjectFromDisplayInfo(displayInfo);
		display(displayInfo, object->getValue());
	}

	int TabColumnValue::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		return sort(lParam1->getValue(), lParam2->getValue(), sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnValue::getSettings() const {
		return settings.object_window.column_value;
	}

	//
	// Column: Weapon Chop Max
	//

	TabColumnWeaponChopMax::TabColumnWeaponChopMax() : TabColumn("ChopMax", LVCFMT_CENTER) {

	}

	bool TabColumnWeaponChopMax::supportsObjectType(ObjectType::ObjectType objectType) const {
		return objectType == ObjectType::Weapon;
	}

	void TabColumnWeaponChopMax::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = static_cast<const Weapon*>(getObjectFromDisplayInfo(displayInfo));
		display(displayInfo, object->chopMax);
	}

	int TabColumnWeaponChopMax::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		const auto a = static_cast<const Weapon*>(lParam1);
		const auto b = static_cast<const Weapon*>(lParam2);
		return sort(a->chopMax, b->chopMax, sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnWeaponChopMax::getSettings() const {
		return settings.object_window.column_weapon_chop_max;
	}

	//
	// Column: Weapon Chop Min
	//

	TabColumnWeaponChopMin::TabColumnWeaponChopMin() : TabColumn("ChopMin", LVCFMT_CENTER) {

	}

	bool TabColumnWeaponChopMin::supportsObjectType(ObjectType::ObjectType objectType) const {
		return objectType == ObjectType::Weapon;
	}

	void TabColumnWeaponChopMin::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = static_cast<const Weapon*>(getObjectFromDisplayInfo(displayInfo));
		display(displayInfo, object->chopMin);
	}

	int TabColumnWeaponChopMin::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		const auto a = static_cast<const Weapon*>(lParam1);
		const auto b = static_cast<const Weapon*>(lParam2);
		return sort(a->chopMin, b->chopMin, sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnWeaponChopMin::getSettings() const {
		return settings.object_window.column_weapon_chop_min;
	}

	//
	// Column: Weapon Ignores Resistance
	//

	TabColumnWeaponIgnoresResistance::TabColumnWeaponIgnoresResistance() : TabColumn("Ignores Resistance", LVCFMT_CENTER) {

	}

	bool TabColumnWeaponIgnoresResistance::supportsObjectType(ObjectType::ObjectType objectType) const {
		return objectType == ObjectType::Weapon;
	}

	void TabColumnWeaponIgnoresResistance::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = static_cast<const Weapon*>(getObjectFromDisplayInfo(displayInfo));
		display(displayInfo, object->getIgnoresResistance());
	}

	int TabColumnWeaponIgnoresResistance::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		const auto a = static_cast<const Weapon*>(lParam1);
		const auto b = static_cast<const Weapon*>(lParam2);
		return sort(a->getIgnoresResistance(), b->getIgnoresResistance(), sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnWeaponIgnoresResistance::getSettings() const {
		return settings.object_window.column_weapon_ignores_resistance;
	}

	//
	// Column: Weapon Is Silver
	//

	TabColumnWeaponIsSilver::TabColumnWeaponIsSilver() : TabColumn("Silver", LVCFMT_CENTER) {

	}

	bool TabColumnWeaponIsSilver::supportsObjectType(ObjectType::ObjectType objectType) const {
		return objectType == ObjectType::Weapon;
	}

	void TabColumnWeaponIsSilver::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = static_cast<const Weapon*>(getObjectFromDisplayInfo(displayInfo));
		display(displayInfo, object->getIsSilver());
	}

	int TabColumnWeaponIsSilver::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		const auto a = static_cast<const Weapon*>(lParam1);
		const auto b = static_cast<const Weapon*>(lParam2);
		return sort(a->getIsSilver(), b->getIsSilver(), sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnWeaponIsSilver::getSettings() const {
		return settings.object_window.column_weapon_silver;
	}

	//
	// Column: Weapon Reach
	//

	TabColumnWeaponReach::TabColumnWeaponReach() : TabColumn("Reach", LVCFMT_CENTER) {

	}

	bool TabColumnWeaponReach::supportsObjectType(ObjectType::ObjectType objectType) const {
		return objectType == ObjectType::Weapon;
	}

	void TabColumnWeaponReach::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = static_cast<const Weapon*>(getObjectFromDisplayInfo(displayInfo));
		display(displayInfo, object->reach);
	}

	int TabColumnWeaponReach::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		const auto a = static_cast<const Weapon*>(lParam1);
		const auto b = static_cast<const Weapon*>(lParam2);
		return sort(a->reach, b->reach, sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnWeaponReach::getSettings() const {
		return settings.object_window.column_weapon_reach;
	}

	//
	// Column: Weapon Slash Max
	//

	TabColumnWeaponSlashMax::TabColumnWeaponSlashMax() : TabColumn("SlashMax", LVCFMT_CENTER) {

	}

	bool TabColumnWeaponSlashMax::supportsObjectType(ObjectType::ObjectType objectType) const {
		return objectType == ObjectType::Weapon;
	}

	void TabColumnWeaponSlashMax::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = static_cast<const Weapon*>(getObjectFromDisplayInfo(displayInfo));
		display(displayInfo, object->slashMax);
	}

	int TabColumnWeaponSlashMax::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		const auto a = static_cast<const Weapon*>(lParam1);
		const auto b = static_cast<const Weapon*>(lParam2);
		return sort(a->slashMax, b->slashMax, sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnWeaponSlashMax::getSettings() const {
		return settings.object_window.column_weapon_slash_max;
	}

	//
	// Column: Weapon Slash Min
	//

	TabColumnWeaponSlashMin::TabColumnWeaponSlashMin() : TabColumn("SlashMin", LVCFMT_CENTER) {

	}

	bool TabColumnWeaponSlashMin::supportsObjectType(ObjectType::ObjectType objectType) const {
		return objectType == ObjectType::Weapon;
	}

	void TabColumnWeaponSlashMin::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = static_cast<const Weapon*>(getObjectFromDisplayInfo(displayInfo));
		display(displayInfo, object->slashMin);
	}

	int TabColumnWeaponSlashMin::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		const auto a = static_cast<const Weapon*>(lParam1);
		const auto b = static_cast<const Weapon*>(lParam2);
		return sort(a->slashMin, b->slashMin, sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnWeaponSlashMin::getSettings() const {
		return settings.object_window.column_weapon_slash_min;
	}

	//
	// Column: Weapon Speed
	//

	TabColumnWeaponSpeed::TabColumnWeaponSpeed() : TabColumn("Speed", LVCFMT_CENTER) {

	}

	bool TabColumnWeaponSpeed::supportsObjectType(ObjectType::ObjectType objectType) const {
		return objectType == ObjectType::Weapon;
	}

	void TabColumnWeaponSpeed::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = static_cast<const Weapon*>(getObjectFromDisplayInfo(displayInfo));
		display(displayInfo, object->speed);
	}

	int TabColumnWeaponSpeed::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		const auto a = static_cast<const Weapon*>(lParam1);
		const auto b = static_cast<const Weapon*>(lParam2);
		return sort(a->speed, b->speed, sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnWeaponSpeed::getSettings() const {
		return settings.object_window.column_weapon_speed;
	}

	//
	// Column: Weapon Thrust Max
	//

	TabColumnWeaponThrustMax::TabColumnWeaponThrustMax() : TabColumn("ThrustMax", LVCFMT_CENTER) {

	}

	bool TabColumnWeaponThrustMax::supportsObjectType(ObjectType::ObjectType objectType) const {
		return objectType == ObjectType::Weapon;
	}

	void TabColumnWeaponThrustMax::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = static_cast<const Weapon*>(getObjectFromDisplayInfo(displayInfo));
		display(displayInfo, object->thrustMax);
	}

	int TabColumnWeaponThrustMax::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		const auto a = static_cast<const Weapon*>(lParam1);
		const auto b = static_cast<const Weapon*>(lParam2);
		return sort(a->thrustMax, b->thrustMax, sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnWeaponThrustMax::getSettings() const {
		return settings.object_window.column_weapon_thrust_max;
	}

	//
	// Column: Weapon Thrust Min
	//

	TabColumnWeaponThrustMin::TabColumnWeaponThrustMin() : TabColumn("ThrustMin", LVCFMT_CENTER) {

	}

	bool TabColumnWeaponThrustMin::supportsObjectType(ObjectType::ObjectType objectType) const {
		return objectType == ObjectType::Weapon;
	}

	void TabColumnWeaponThrustMin::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = static_cast<const Weapon*>(getObjectFromDisplayInfo(displayInfo));
		display(displayInfo, object->thrustMin);
	}

	int TabColumnWeaponThrustMin::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		const auto a = static_cast<const Weapon*>(lParam1);
		const auto b = static_cast<const Weapon*>(lParam2);
		return sort(a->thrustMin, b->thrustMin, sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnWeaponThrustMin::getSettings() const {
		return settings.object_window.column_weapon_thrust_min;
	}

	//
	// Column: Weight
	//

	TabColumnWeight::TabColumnWeight() : TabColumn("Weight", LVCFMT_CENTER) {

	}

	bool TabColumnWeight::supportsObjectType(ObjectType::ObjectType objectType) const {
		switch (objectType) {
		case ObjectType::Alchemy:
		case ObjectType::Apparatus:
		case ObjectType::Armor:
		case ObjectType::Book:
		case ObjectType::Clothing:
		case ObjectType::Ingredient:
		case ObjectType::Light:
		case ObjectType::Lockpick:
		case ObjectType::Misc:
		case ObjectType::Probe:
		case ObjectType::Repair:
		case ObjectType::Weapon:
			return true;
		}
		return false;
	}

	void TabColumnWeight::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = getObjectFromDisplayInfo(displayInfo);
		display(displayInfo, object->getWeight());
	}

	int TabColumnWeight::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		return sort(lParam1->getWeight(), lParam2->getWeight(), sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnWeight::getSettings() const {
		return settings.object_window.column_weight;
	}

	//
	// Column: Weight Class
	//

	TabColumnWeightClass::TabColumnWeightClass() : TabColumn("Weight Class", LVCFMT_CENTER) {

	}

	bool TabColumnWeightClass::supportsObjectType(ObjectType::ObjectType objectType) const {
		return objectType == ObjectType::Armor;
	}

	void TabColumnWeightClass::getDisplayInfo(LPNMLVDISPINFOA displayInfo) const {
		auto object = static_cast<const Armor*>(getObjectFromDisplayInfo(displayInfo));
		display(displayInfo, object->getWeightClassName());
	}

	int TabColumnWeightClass::sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const {
		const auto a = static_cast<const Armor*>(lParam1);
		const auto b = static_cast<const Armor*>(lParam2);
		return sort(a->getWeightClassName(), b->getWeightClassName(), sortOrderAsc);
	}

	TabColumn::ColumnSettings& TabColumnWeightClass::getSettings() const {
		return settings.object_window.column_weight_class;
	}

	//
	// TabController
	//

	void* TabController::operator new(size_t size) {
		return memory::_new(size);
	}

	void TabController::operator delete(void* block) {
		memory::_delete(block);
	}

	TabColumnActorClass TabController::tabColumnActorClass;
	TabColumnActorEssential TabController::tabColumnActorEssential;
	TabColumnActorFaction TabController::tabColumnActorFaction;
	TabColumnActorFactionRank TabController::tabColumnActorFactionRank;
	TabColumnTraining TabController::tabColumnTraining;
	TabColumnFight TabController::tabColumnFight;
	TabColumnActorInventory TabController::tabColumnActorInventory;
	TabColumnActorLevel TabController::tabColumnActorLevel;
	TabColumnActorRespawns TabController::tabColumnActorRespawns;
	TabColumnAllLTEPC TabController::tabColumnAllLTEPC;
	TabColumnAnimation TabController::tabColumnAnimation;
	TabColumnArmorRating TabController::tabColumnArmorRating;
	TabColumnAutoCalc TabController::tabColumnAutoCalc;
	TabColumnBlocked TabController::tabColumnBlocked;
	TabColumnBookIsScroll TabController::tabColumnBookIsScroll;
	TabColumnBookTeaches TabController::tabColumnBookTeaches;
	TabColumnChanceNone TabController::tabColumnChanceNone;
	TabColumnCost TabController::tabColumnCost;
	TabColumnCount TabController::tabColumnCount;
	TabColumnCreatureIsBipedal TabController::tabColumnCreatureIsBipedal;
	TabColumnCreatureList TabController::tabColumnCreatureList;
	TabColumnCreatureMovementType TabController::tabColumnCreatureMovementType;
	TabColumnCreatureSoulValue TabController::tabColumnCreatureSoul;
	TabColumnCreatureSound TabController::tabColumnCreatureSound;
	TabColumnCreatureUsesWeaponAndShield TabController::tabColumnCreatureUsesWeaponAndShield;
	TabColumnEffect TabController::tabColumnEffect1(0);
	TabColumnEffect TabController::tabColumnEffect2(1);
	TabColumnEffect TabController::tabColumnEffect3(2);
	TabColumnEffect TabController::tabColumnEffect4(3);
	TabColumnEnchanting TabController::tabColumnEnchanting;
	TabColumnEnchantment TabController::tabColumnEnchantment;
	TabColumnEnchantmentCharge TabController::tabColumnEnchantmentCharge;
	TabColumnFemale TabController::tabColumnFemale;
	TabColumnHealth TabController::tabColumnHealth;
	TabColumnID TabController::tabColumnID;
	TabColumnInventory TabController::tabColumnInventory;
	TabColumnLeveledItemList TabController::tabColumnLeveledItemList;
	TabColumnLightRadius TabController::tabColumnLightRadius;
	TabColumnLightTime TabController::tabColumnLightTime;
	TabColumnModel TabController::tabColumnModel;
	TabColumnModified TabController::tabColumnModified;
	TabColumnName TabController::tabColumnName;
	TabColumnOrganic TabController::tabColumnOrganic;
	TabColumnPart TabController::tabColumnPart;
	TabColumnPartType TabController::tabColumnPartType;
	TabColumnPCStart TabController::tabColumnPCStart;
	TabColumnPersists TabController::tabColumnPersists;
	TabColumnPlayable TabController::tabColumnPlayable;
	TabColumnQuality TabController::tabColumnQuality;
	TabColumnRace TabController::tabColumnRace;
	TabColumnScript TabController::tabColumnScript;
	TabColumnSound TabController::tabColumnSound;
	TabColumnSpellRange TabController::tabColumnSpellRange;
	TabColumnType TabController::tabColumnType;
	TabColumnUses TabController::tabColumnUses;
	TabColumnValue TabController::tabColumnValue;
	TabColumnWeaponChopMax TabController::tabColumnWeaponChopMax;
	TabColumnWeaponChopMin TabController::tabColumnWeaponChopMin;
	TabColumnWeaponIgnoresResistance TabController::tabColumnWeaponIgnoresResistance;
	TabColumnWeaponIsSilver TabController::tabColumnWeaponIsSilver;
	TabColumnWeaponReach TabController::tabColumnWeaponReach;
	TabColumnWeaponSlashMax TabController::tabColumnWeaponSlashMax;
	TabColumnWeaponSlashMin TabController::tabColumnWeaponSlashMin;
	TabColumnWeaponSpeed TabController::tabColumnWeaponSpeed;
	TabColumnWeaponThrustMax TabController::tabColumnWeaponThrustMax;
	TabColumnWeaponThrustMin TabController::tabColumnWeaponThrustMin;
	TabColumnWeight TabController::tabColumnWeight;
	TabColumnWeightClass TabController::tabColumnWeightClass;

	TabController::TabController(ObjectType::ObjectType objectType) {
		const auto TabController_ctor = reinterpret_cast<TabController * (__thiscall*)(TabController*, ObjectType::ObjectType)>(0x43BF60);
		TabController_ctor(this, objectType);
	}

	void TabController::setupColumns(HWND hWnd) {
		tabColumnID.addToController(this, hWnd);
		tabColumnCount.conditionalAddToController(this, hWnd);

		switch (objectType) {
		case ObjectType::Activator:
			tabColumnName.addToController(this, hWnd);
			tabColumnScript.addToController(this, hWnd);
			tabColumnModel.addToController(this, hWnd);
			tabColumnPersists.addToController(this, hWnd);
			break;
		case ObjectType::Apparatus:
			tabColumnType.addToController(this, hWnd);
			tabColumnName.addToController(this, hWnd);
			tabColumnScript.addToController(this, hWnd);
			tabColumnWeight.addToController(this, hWnd);
			tabColumnValue.addToController(this, hWnd);
			tabColumnQuality.addToController(this, hWnd);
			tabColumnInventory.addToController(this, hWnd);
			tabColumnModel.addToController(this, hWnd);
			tabColumnPersists.addToController(this, hWnd);
			break;
		case ObjectType::Armor:
			tabColumnType.addToController(this, hWnd);
			tabColumnName.addToController(this, hWnd);
			tabColumnScript.addToController(this, hWnd);
			tabColumnWeight.addToController(this, hWnd);
			tabColumnWeightClass.addToController(this, hWnd);
			tabColumnValue.addToController(this, hWnd);
			tabColumnHealth.addToController(this, hWnd);
			tabColumnEnchanting.addToController(this, hWnd);
			tabColumnEnchantment.addToController(this, hWnd);
			tabColumnArmorRating.addToController(this, hWnd);
			tabColumnInventory.addToController(this, hWnd);
			tabColumnModel.addToController(this, hWnd);
			tabColumnPersists.addToController(this, hWnd);
			break;
		case ObjectType::Bodypart:
			tabColumnPartType.addToController(this, hWnd);
			tabColumnRace.addToController(this, hWnd);
			tabColumnPart.addToController(this, hWnd);
			tabColumnFemale.addToController(this, hWnd);
			tabColumnPlayable.addToController(this, hWnd);
			tabColumnModel.addToController(this, hWnd);
			break;
		case ObjectType::Book:
			tabColumnName.addToController(this, hWnd);
			tabColumnScript.addToController(this, hWnd);
			tabColumnWeight.addToController(this, hWnd);
			tabColumnValue.addToController(this, hWnd);
			tabColumnBookIsScroll.addToController(this, hWnd);
			tabColumnBookTeaches.addToController(this, hWnd);
			tabColumnEnchanting.addToController(this, hWnd);
			tabColumnEnchantment.addToController(this, hWnd);
			tabColumnInventory.addToController(this, hWnd);
			tabColumnModel.addToController(this, hWnd);
			tabColumnPersists.addToController(this, hWnd);
			break;
		case ObjectType::Clothing:
			tabColumnType.addToController(this, hWnd);
			tabColumnName.addToController(this, hWnd);
			tabColumnScript.addToController(this, hWnd);
			tabColumnWeight.addToController(this, hWnd);
			tabColumnValue.addToController(this, hWnd);
			tabColumnEnchanting.addToController(this, hWnd);
			tabColumnEnchantment.addToController(this, hWnd);
			tabColumnInventory.addToController(this, hWnd);
			tabColumnModel.addToController(this, hWnd);
			tabColumnPersists.addToController(this, hWnd);
			break;
		case ObjectType::Container:
			tabColumnName.addToController(this, hWnd);
			tabColumnScript.addToController(this, hWnd);
			tabColumnWeight.addToController(this, hWnd);
			tabColumnOrganic.addToController(this, hWnd);
			tabColumnActorRespawns.addToController(this, hWnd);
			tabColumnActorInventory.addToController(this, hWnd);
			tabColumnModel.addToController(this, hWnd);
			tabColumnPersists.addToController(this, hWnd);
			break;
		case ObjectType::Door:
			tabColumnName.addToController(this, hWnd);
			tabColumnScript.addToController(this, hWnd);
			tabColumnModel.addToController(this, hWnd);
			tabColumnPersists.addToController(this, hWnd);
			break;
		case ObjectType::Ingredient:
			tabColumnName.addToController(this, hWnd);
			tabColumnScript.addToController(this, hWnd);
			tabColumnWeight.addToController(this, hWnd);
			tabColumnValue.addToController(this, hWnd);
			tabColumnEffect1.addToController(this, hWnd);
			tabColumnEffect2.addToController(this, hWnd);
			tabColumnEffect3.addToController(this, hWnd);
			tabColumnEffect4.addToController(this, hWnd);
			tabColumnInventory.addToController(this, hWnd);
			tabColumnModel.addToController(this, hWnd);
			tabColumnPersists.addToController(this, hWnd);
			break;
		case ObjectType::Light:
			tabColumnName.addToController(this, hWnd);
			tabColumnScript.addToController(this, hWnd);
			tabColumnSound.addToController(this, hWnd);
			tabColumnWeight.addToController(this, hWnd);
			tabColumnValue.addToController(this, hWnd);
			tabColumnLightTime.addToController(this, hWnd);
			tabColumnLightRadius.addToController(this, hWnd);
			tabColumnInventory.addToController(this, hWnd);
			tabColumnModel.addToController(this, hWnd);
			tabColumnPersists.addToController(this, hWnd);
			break;
		case ObjectType::Lockpick:
			tabColumnName.addToController(this, hWnd);
			tabColumnScript.addToController(this, hWnd);
			tabColumnWeight.addToController(this, hWnd);
			tabColumnValue.addToController(this, hWnd);
			tabColumnUses.addToController(this, hWnd);
			tabColumnQuality.addToController(this, hWnd);
			tabColumnInventory.addToController(this, hWnd);
			tabColumnModel.addToController(this, hWnd);
			tabColumnPersists.addToController(this, hWnd);
			break;
		case ObjectType::Misc:
			tabColumnName.addToController(this, hWnd);
			tabColumnScript.addToController(this, hWnd);
			tabColumnWeight.addToController(this, hWnd);
			tabColumnValue.addToController(this, hWnd);
			tabColumnInventory.addToController(this, hWnd);
			tabColumnModel.addToController(this, hWnd);
			tabColumnPersists.addToController(this, hWnd);
			break;
		case ObjectType::Probe:
			tabColumnName.addToController(this, hWnd);
			tabColumnScript.addToController(this, hWnd);
			tabColumnWeight.addToController(this, hWnd);
			tabColumnValue.addToController(this, hWnd);
			tabColumnUses.addToController(this, hWnd);
			tabColumnQuality.addToController(this, hWnd);
			tabColumnInventory.addToController(this, hWnd);
			tabColumnModel.addToController(this, hWnd);
			tabColumnPersists.addToController(this, hWnd);
			break;
		case ObjectType::Repair:
			tabColumnName.addToController(this, hWnd);
			tabColumnScript.addToController(this, hWnd);
			tabColumnWeight.addToController(this, hWnd);
			tabColumnValue.addToController(this, hWnd);
			tabColumnUses.addToController(this, hWnd);
			tabColumnQuality.addToController(this, hWnd);
			tabColumnInventory.addToController(this, hWnd);
			tabColumnModel.addToController(this, hWnd);
			tabColumnPersists.addToController(this, hWnd);
			break;
		case ObjectType::Static:
			tabColumnModel.addToController(this, hWnd);
			tabColumnPersists.addToController(this, hWnd);
			break;
		case ObjectType::Weapon:
			tabColumnType.addToController(this, hWnd);
			tabColumnName.addToController(this, hWnd);
			tabColumnScript.addToController(this, hWnd);
			tabColumnWeight.addToController(this, hWnd);
			tabColumnHealth.addToController(this, hWnd);
			tabColumnValue.addToController(this, hWnd);
			tabColumnEnchanting.addToController(this, hWnd);
			tabColumnEnchantment.addToController(this, hWnd);
			tabColumnWeaponSpeed.addToController(this, hWnd);
			tabColumnWeaponReach.addToController(this, hWnd);
			tabColumnWeaponChopMin.addToController(this, hWnd);
			tabColumnWeaponChopMax.addToController(this, hWnd);
			tabColumnWeaponSlashMin.addToController(this, hWnd);
			tabColumnWeaponSlashMax.addToController(this, hWnd);
			tabColumnWeaponThrustMin.addToController(this, hWnd);
			tabColumnWeaponThrustMax.addToController(this, hWnd);
			tabColumnInventory.addToController(this, hWnd);
			tabColumnWeaponIgnoresResistance.addToController(this, hWnd);
			tabColumnWeaponIsSilver.addToController(this, hWnd);
			tabColumnModel.addToController(this, hWnd);
			tabColumnPersists.addToController(this, hWnd);
			break;
		case ObjectType::NPC:
			tabColumnName.addToController(this, hWnd);
			tabColumnScript.addToController(this, hWnd);
			tabColumnActorLevel.addToController(this, hWnd);
			tabColumnRace.addToController(this, hWnd);
			tabColumnFemale.addToController(this, hWnd);
			tabColumnActorClass.addToController(this, hWnd);
			tabColumnActorFaction.addToController(this, hWnd);
			tabColumnActorFactionRank.addToController(this, hWnd);
			tabColumnAutoCalc.addToController(this, hWnd);
			tabColumnActorEssential.addToController(this, hWnd);
			tabColumnActorRespawns.addToController(this, hWnd);
			tabColumnAnimation.addToController(this, hWnd);
			tabColumnPersists.addToController(this, hWnd);
			tabColumnTraining.addToController(this, hWnd);
			tabColumnFight.addToController(this, hWnd);
			break;
		case ObjectType::Creature:
			tabColumnName.addToController(this, hWnd);
			tabColumnType.addToController(this, hWnd);
			tabColumnScript.addToController(this, hWnd);
			tabColumnCreatureSound.addToController(this, hWnd);
			tabColumnActorLevel.addToController(this, hWnd);
			tabColumnActorEssential.addToController(this, hWnd);
			tabColumnActorRespawns.addToController(this, hWnd);
			tabColumnCreatureMovementType.addToController(this, hWnd);
			tabColumnCreatureUsesWeaponAndShield.addToController(this, hWnd);
			tabColumnCreatureIsBipedal.addToController(this, hWnd);
			tabColumnCreatureSoul.addToController(this, hWnd);
			tabColumnModel.addToController(this, hWnd);
			tabColumnPersists.addToController(this, hWnd);
			break;
		case ObjectType::LeveledCreature:
			tabColumnAllLTEPC.addToController(this, hWnd);
			tabColumnChanceNone.addToController(this, hWnd);
			tabColumnCreatureList.addToController(this, hWnd);
			break;
		case ObjectType::Spell:
			tabColumnName.addToController(this, hWnd);
			tabColumnType.addToController(this, hWnd);
			tabColumnCost.addToController(this, hWnd);
			tabColumnSpellRange.addToController(this, hWnd);
			tabColumnAutoCalc.addToController(this, hWnd);
			tabColumnPCStart.addToController(this, hWnd);
			break;
		case ObjectType::Enchantment:
			tabColumnEnchantmentCharge.addToController(this, hWnd);
			tabColumnType.addToController(this, hWnd);
			tabColumnCost.addToController(this, hWnd);
			tabColumnAutoCalc.addToController(this, hWnd);
			break;
		case ObjectType::Alchemy:
			tabColumnName.addToController(this, hWnd);
			tabColumnScript.addToController(this, hWnd);
			tabColumnWeight.addToController(this, hWnd);
			tabColumnValue.addToController(this, hWnd);
			tabColumnAutoCalc.addToController(this, hWnd);
			tabColumnInventory.addToController(this, hWnd);
			tabColumnModel.addToController(this, hWnd);
			tabColumnPersists.addToController(this, hWnd);
			break;
		case ObjectType::LeveledItem:
			tabColumnAllLTEPC.addToController(this, hWnd);
			tabColumnChanceNone.addToController(this, hWnd);
			tabColumnLeveledItemList.addToController(this, hWnd);
			break;
		}

		tabColumnBlocked.addToController(this, hWnd);
		tabColumnModified.addToController(this, hWnd);
	}

	int TabController::getColumnIndexByTitle(const char* title) const {
		for (auto i = 0u; i < columns.size(); ++i) {
			if (string::equal(columns.at(i)->m_Title, title)) {
				return i;
			}
		}
		return -1;
	}

	TabColumn* TabController::getColumnByTitle(const char* title) const {
		auto index = getColumnIndexByTitle(title);
		if (index == -1) {
			return nullptr;
		}
		return columns.at(index);
	}

}
