#pragma once

#include "CSBaseObject.h"

#include "NIDefines.h"

namespace se::cs {
	struct Object_VirtualTable : BaseObject_VirtualTable {
		void* unknown_0x24;
		void(__thiscall* setObjectID)(Object*, const char*); // 0x28
		void* unknown_0x2C;
		void* unknown_0x30;
		void* unknown_0x34;
		char* (__thiscall* getName)(const Object*); // 0x38
		char* (__thiscall* getIconPath)(const Object*); // 0x3C
		void* unknown_0x40;
		void* unknown_0x44;
		char* (__thiscall* getModelPath)(const Object*); // 0x48
		Script* (__thiscall* getScript)(const Object*); // 0x4C
		Sound* (__thiscall* getSound)(const Object*); // 0x50
		const char* (__thiscall* getRaceName)(const Object*); // 0x54
		const char* (__thiscall* getClassName)(const Object*); // 0x58
		const char* (__thiscall* getFactionName)(const Object*); // 0x5C
		void* unknown_0x60;
		void* unknown_0x64;
		Faction*(__thiscall* getFaction)(const Object*); // 0x68
		bool(__thiscall* getIsFemale)(const Object*); // 0x6C
		void* unknown_0x70;
		int(__thiscall* getLevel)(const Object*); // 0x74
		void* unknown_0x78;
		void* unknown_0x7C;
		void* unknown_0x80;
		void* unknown_0x84;
		void* unknown_0x88;
		void* unknown_0x8C;
		void* unknown_0x90;
		void* unknown_0x94;
		const char* (__thiscall* getTypeName)(const Object*); // 0x98
		float (__thiscall* getWeight)(const Object*); // 0x9C
		int (__thiscall* getValue)(const Object*); // 0xA0
		void* unknown_0xA4;
		void* unknown_0xA8;
		void* unknown_0xAC;
		void* unknown_0xB0;
		void* unknown_0xB4;
		void* unknown_0xB8;
		bool(__thiscall* getIsEssential)(const Object*); // 0xBC
		bool(__thiscall* getRespawns)(const Object*); // 0xC0
		void* unknown_0xC4;
		void* unknown_0xC8;
		void* unknown_0xCC;
		Object* (__thiscall* getEnchantment)(const Object*); // 0xD0
		void* unknown_0xD4;
		void* unknown_0xD8;
		void* unknown_0xDC;
		void* unknown_0xE0;
		void* unknown_0xE4;
		bool (__thiscall* getAutoCalc)(const Object*); // 0xE8
		void* unknown_0xEC;
		void* unknown_0xF0;
		void* unknown_0xF4;
		void* unknown_0xF8;
		void* unknown_0xFC;
		void(__thiscall* populateObjectWindow)(const Object*, HWND); // 0x100
		void* unknown_0x104;
		bool(__thiscall* isMarker)(const Object*); // 0x108
		void* unknown_0x10C;
		void* unknown_0x110;
		void* unknown_0x114;
		void* unknown_0x118;
		void* unknown_0x11C;
		float(__thiscall* getScale)(const Object*); // 0x120
		void(__thiscall* setScale)(Object*, float, bool); // 0x124
		void* unknown_0x128;
		void* unknown_0x12C;
		void* unknown_0x130;
		void* unknown_0x134;
	};

	struct Object : BaseObject {
		NI::Node* sceneNode; // 0x10
		BaseObject* unknown_0x14;
		void* referenceToThis; // 0x18
		Object* previousInCollection; // 0x1C
		Object* nextInCollection; // 0x20
		int unknown_0x24;

		//
		// Virtual table access
		//

		const char* getName() const;
		bool isMarker() const;
		char* getIcon() const;
		char* getModel() const;
		Object* getEnchantment() const;
		Script* getScript() const;
		float getScale() const;
		void setScale(float scale, bool clamp = true);
		int getCount() const;
		const char* getTypeName() const;
		Sound* getSound() const;
		const char* getRaceName() const;
		const char* getClassName() const;
		const char* getFactionName() const;
		Faction* getFaction() const;
		bool getIsFemale() const;
		bool getIsEssential() const;
		bool getRespawns() const;
		int getLevel() const;
		bool getAutoCalc() const;
		float getWeight() const;
		int getValue() const;
		void populateObjectWindow(HWND hWnd) const;

		//
		// Custom functions
		//

		bool search(const std::string_view& needle, const SearchSettings& settings, std::regex* regex = nullptr) const;
		bool searchWithInheritance(const std::string_view& needle, const SearchSettings& settings, std::regex* regex = nullptr) const;
	};
	static_assert(sizeof(Object) == 0x28, "CS::Object failed size validation");
	static_assert(sizeof(Object_VirtualTable) == 0x138, "CS::Object's virtual table failed size validation");
}
