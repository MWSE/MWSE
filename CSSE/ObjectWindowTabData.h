#pragma once

#include "CSObject.h"
#include "CSLeveledList.h"

#include "NIIteratedList.h"

// TODO: Pull this elsewhere
#include "CSActor.h"

#include "Settings.h"

namespace se::cs::dialog::object_window {
	struct TabController;

	//
	// Abstract column interface
	//

	class TabColumnParam {
	public:
		operator WPARAM() const;

		bool getSortAsc() const;
		void setSortAsc(bool asc);
		void toggleSortOrder();

		WPARAM getSortColumn() const;
		void setSortColumn(WPARAM column);

		WPARAM& getData();

		static TabColumnParam* get();
		static TabColumnParam& get(byte tab);

	private:
		WPARAM data;
	};
	static_assert(sizeof(TabColumnParam) == sizeof(WPARAM), "");

	class TabColumn {
	public:
		const char* m_Title;
		int m_Format;

		TabColumn(const char* title, int fmt = LVCFMT_LEFT);

		virtual bool supportsObjectType(ObjectType::ObjectType objectType) const = 0;
		virtual void getDisplayInfo(LPNMLVDISPINFOA displayInfo) const = 0;
		virtual int sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const = 0;

		using ColumnSettings = Settings_t::ColumnSettings;
		virtual ColumnSettings& getSettings() const = 0;

		void insert(HWND hWnd, size_t index) const;
		void addToController(TabController* controller, HWND hWnd);
		void conditionalAddToController(TabController* controller, HWND hWnd);
	protected:
		const Object* getObjectFromDisplayInfo(LPNMLVDISPINFOA displayInfo) const;

		void display(LPNMLVDISPINFOA displayInfo, bool value) const;
		void display(LPNMLVDISPINFOA displayInfo, int value) const;
		void display(LPNMLVDISPINFOA displayInfo, float value, const char* format = "%.2f") const;
		void display(LPNMLVDISPINFOA displayInfo, const char* string) const;
		void display(LPNMLVDISPINFOA displayInfo, const NI::IteratedList<ItemStack*>& items) const;
		void display(LPNMLVDISPINFOA displayInfo, const NI::IteratedList<LeveledList::Node*>* list) const;

		int sort(bool a, bool b, bool sortOrderAsc) const;
		int sort(int a, int b, bool sortOrderAsc) const;
		int sort(unsigned int a, unsigned int b, bool sortOrderAsc) const;
		int sort(float a, float b, bool sortOrderAsc) const;
		int sort(const char* a, const char* b, bool sortOrderAsc) const;
		int sort(const NI::IteratedList<ItemStack*>& a, const NI::IteratedList<ItemStack*>& b, bool sortOrderAsc) const;
		int sort(const NI::IteratedList<LeveledList::Node*>* a, const NI::IteratedList<LeveledList::Node*>* b, bool sortOrderAsc) const;
	};

#define DEFINE_COLUMN(NAME) \
	class TabColumn##NAME : public TabColumn { \
	public: \
		TabColumn##NAME(); \
		bool supportsObjectType(ObjectType::ObjectType objectType) const override; \
		void getDisplayInfo(LPNMLVDISPINFOA displayInfo) const override; \
		int sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const override; \
		ColumnSettings& getSettings() const override; \
	};

	// Define basic columns.
	DEFINE_COLUMN(ActorClass);
	DEFINE_COLUMN(ActorEssential);
	DEFINE_COLUMN(ActorFaction);
	DEFINE_COLUMN(ActorFactionRank);
	DEFINE_COLUMN(ActorInventory);
	DEFINE_COLUMN(ActorLevel);
	DEFINE_COLUMN(ActorRespawns);
	DEFINE_COLUMN(AllLTEPC);
	DEFINE_COLUMN(Animation);
	DEFINE_COLUMN(ArmorRating);
	DEFINE_COLUMN(AutoCalc);
	DEFINE_COLUMN(Blocked);
	DEFINE_COLUMN(BookIsScroll);
	DEFINE_COLUMN(BookTeaches);
	DEFINE_COLUMN(ChanceNone);
	DEFINE_COLUMN(Cost);
	DEFINE_COLUMN(Count);
	DEFINE_COLUMN(CreatureIsBipedal);
	DEFINE_COLUMN(CreatureList);
	DEFINE_COLUMN(CreatureMovementType);
	DEFINE_COLUMN(CreatureSoulValue);
	DEFINE_COLUMN(CreatureSound);
	DEFINE_COLUMN(CreatureUsesWeaponAndShield);
	DEFINE_COLUMN(Enchanting);
	DEFINE_COLUMN(Enchantment);
	DEFINE_COLUMN(EnchantmentCharge);
	DEFINE_COLUMN(Female);
	DEFINE_COLUMN(Health);
	DEFINE_COLUMN(ID);
	DEFINE_COLUMN(Inventory);
	DEFINE_COLUMN(LeveledItemList);
	DEFINE_COLUMN(LightRadius);
	DEFINE_COLUMN(LightTime);
	DEFINE_COLUMN(Model);
	DEFINE_COLUMN(Modified);
	DEFINE_COLUMN(Name);
	DEFINE_COLUMN(Organic);
	DEFINE_COLUMN(Part);
	DEFINE_COLUMN(PartType);
	DEFINE_COLUMN(PCStart);
	DEFINE_COLUMN(Persists);
	DEFINE_COLUMN(Playable);
	DEFINE_COLUMN(Quality);
	DEFINE_COLUMN(Race);
	DEFINE_COLUMN(Rank);
	DEFINE_COLUMN(Script);
	DEFINE_COLUMN(Sound);
	DEFINE_COLUMN(SpellRange);
	DEFINE_COLUMN(Type);
	DEFINE_COLUMN(Uses);
	DEFINE_COLUMN(Value);
	DEFINE_COLUMN(WeaponChopMax);
	DEFINE_COLUMN(WeaponChopMin);
	DEFINE_COLUMN(WeaponIgnoresResistance);
	DEFINE_COLUMN(WeaponIsSilver);
	DEFINE_COLUMN(WeaponReach);
	DEFINE_COLUMN(WeaponSlashMax);
	DEFINE_COLUMN(WeaponSlashMin);
	DEFINE_COLUMN(WeaponSpeed);
	DEFINE_COLUMN(WeaponThrustMax);
	DEFINE_COLUMN(WeaponThrustMin);
	DEFINE_COLUMN(Weight);
	DEFINE_COLUMN(WeightClass);

	// Extended columns

	class TabColumnEffect : public TabColumn {
	public:
		TabColumnEffect(int index);
		bool supportsObjectType(ObjectType::ObjectType objectType) const override;
		void getDisplayInfo(LPNMLVDISPINFOA displayInfo) const override;
		int sortObject(const Object* lParam1, const Object* lParam2, bool sortOrderAsc) const override;
		ColumnSettings& getSettings() const override;
	protected:
		int m_EffectIndex;
	};

	//
	// Main controller and its extension
	//

	struct TabController {
		ObjectType::ObjectType objectType; // 0x0
		unsigned int columnsActive; // 0x4
		unsigned int topIndex; // 0x8
		NI::IteratedList<BaseObject*>* containedObjects; // 0xC
		std::vector<TabColumn*> columns; // 0x10

		static void* operator new(size_t size);
		static void operator delete(void* block);

		static TabColumnActorClass tabColumnActorClass;
		static TabColumnActorEssential tabColumnActorEssential;
		static TabColumnActorFaction tabColumnActorFaction;
		static TabColumnActorFactionRank tabColumnActorFactionRank;
		static TabColumnActorInventory tabColumnActorInventory;
		static TabColumnActorLevel tabColumnActorLevel;
		static TabColumnActorRespawns tabColumnActorRespawns;
		static TabColumnAllLTEPC tabColumnAllLTEPC;
		static TabColumnAnimation tabColumnAnimation;
		static TabColumnArmorRating tabColumnArmorRating;
		static TabColumnAutoCalc tabColumnAutoCalc;
		static TabColumnBlocked tabColumnBlocked;
		static TabColumnChanceNone tabColumnChanceNone;
		static TabColumnBookIsScroll tabColumnBookIsScroll;
		static TabColumnBookTeaches tabColumnBookTeaches;
		static TabColumnCost tabColumnCost;
		static TabColumnCount tabColumnCount;
		static TabColumnCreatureIsBipedal tabColumnCreatureIsBipedal;
		static TabColumnCreatureList tabColumnCreatureList;
		static TabColumnCreatureMovementType tabColumnCreatureMovementType;
		static TabColumnCreatureSoulValue tabColumnCreatureSoul;
		static TabColumnCreatureSound tabColumnCreatureSound;
		static TabColumnCreatureUsesWeaponAndShield tabColumnCreatureUsesWeaponAndShield;
		static TabColumnEffect tabColumnEffect1;
		static TabColumnEffect tabColumnEffect2;
		static TabColumnEffect tabColumnEffect3;
		static TabColumnEffect tabColumnEffect4;
		static TabColumnEnchanting tabColumnEnchanting;
		static TabColumnEnchantment tabColumnEnchantment;
		static TabColumnEnchantmentCharge tabColumnEnchantmentCharge;
		static TabColumnFemale tabColumnFemale;
		static TabColumnHealth tabColumnHealth;
		static TabColumnID tabColumnID;
		static TabColumnInventory tabColumnInventory;
		static TabColumnLeveledItemList tabColumnLeveledItemList;
		static TabColumnLightRadius tabColumnLightRadius;
		static TabColumnLightTime tabColumnLightTime;
		static TabColumnModel tabColumnModel;
		static TabColumnModified tabColumnModified;
		static TabColumnName tabColumnName;
		static TabColumnOrganic tabColumnOrganic;
		static TabColumnPart tabColumnPart;
		static TabColumnPartType tabColumnPartType;
		static TabColumnPCStart tabColumnPCStart;
		static TabColumnPersists tabColumnPersists;
		static TabColumnPlayable tabColumnPlayable;
		static TabColumnQuality tabColumnQuality;
		static TabColumnRace tabColumnRace;
		static TabColumnScript tabColumnScript;
		static TabColumnSound tabColumnSound;
		static TabColumnSpellRange tabColumnSpellRange;
		static TabColumnType tabColumnType;
		static TabColumnUses tabColumnUses;
		static TabColumnValue tabColumnValue;
		static TabColumnWeaponChopMax tabColumnWeaponChopMax;
		static TabColumnWeaponChopMin tabColumnWeaponChopMin;
		static TabColumnWeaponIgnoresResistance tabColumnWeaponIgnoresResistance;
		static TabColumnWeaponIsSilver tabColumnWeaponIsSilver;
		static TabColumnWeaponReach tabColumnWeaponReach;
		static TabColumnWeaponSlashMax tabColumnWeaponSlashMax;
		static TabColumnWeaponSlashMin tabColumnWeaponSlashMin;
		static TabColumnWeaponSpeed tabColumnWeaponSpeed;
		static TabColumnWeaponThrustMax tabColumnWeaponThrustMax;
		static TabColumnWeaponThrustMin tabColumnWeaponThrustMin;
		static TabColumnWeight tabColumnWeight;
		static TabColumnWeightClass tabColumnWeightClass;

		TabController(ObjectType::ObjectType objectType);

		void setupColumns(HWND hWnd);

		int getColumnIndexByTitle(const char* title) const;
		TabColumn* getColumnByTitle(const char* title) const;
	};
}
