/************************************************************************

	xSetName.cpp - Copyright (c) 2008 The MWSE Project
	https://github.com/MWSE/MWSE/

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

**************************************************************************/

#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "MemoryUtil.h"

#include "TES3Activator.h"
#include "TES3Apparatus.h"
#include "TES3Armor.h"
#include "TES3Container.h"
#include "TES3Door.h"
#include "TES3Light.h"
#include "TES3Lockpick.h"
#include "TES3NPC.h"
#include "TES3Reference.h"

using namespace mwse;

namespace mwse
{
	class xSetName : mwse::InstructionInterface_t
	{
	public:
		xSetName();
		virtual float execute(VMExecuteInterface &virtualMachine);
		virtual void loadParameters(VMExecuteInterface &virtualMachine);
	};

	static xSetName xSetNameInstance;

	xSetName::xSetName() : mwse::InstructionInterface_t(OpCode::xSetName) {}

	void xSetName::loadParameters(mwse::VMExecuteInterface &virtualMachine) {}

	float xSetName::execute(mwse::VMExecuteInterface &virtualMachine)
	{
		// Get parameter from the stack.
		mwseString& name = virtualMachine.getString(mwse::Stack::getInstance().popLong());

		// Enforce name length.
		if (name.length() > 31) {
#if _DEBUG
			mwse::log::getLog() << "xSetName: Given name length must be 31 characters or less." << std::endl;
#endif
			mwse::Stack::getInstance().pushShort(false);
			return 0.0f;
		}

		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
#if _DEBUG
			mwse::log::getLog() << "xSetName: No reference provided." << std::endl;
#endif
			mwse::Stack::getInstance().pushShort(false);
			return 0.0f;
		}
		
		// Get the base record.
		TES3::BaseObject* recordGeneric = reference->baseObject;
		if (recordGeneric == NULL) {
#if _DEBUG
			mwse::log::getLog() << "xSetName: No record found for reference." << std::endl;
#endif
			mwse::Stack::getInstance().pushShort(false);
			return 0.0f;
		}

		// Get string pointer based on record type.
		char* namePtr = NULL;
		char** nameContainer = NULL;
		TES3::ObjectType::ObjectType recordType = recordGeneric->objectType;
		switch (recordType) {
		case TES3::ObjectType::NPC:
		case TES3::ObjectType::Creature:
			nameContainer = &reinterpret_cast<TES3::NPCInstance*>(recordGeneric)->baseNPC->name;
			namePtr = *nameContainer;
			break;
		case TES3::ObjectType::Container:
			nameContainer = &reinterpret_cast<TES3::Container*>(recordGeneric)->name;
			namePtr = *nameContainer;
			break;
		case TES3::ObjectType::Light:
			nameContainer = &reinterpret_cast<TES3::Light*>(recordGeneric)->name;
			namePtr = *nameContainer;
			break;
		case TES3::ObjectType::Alchemy:
		case TES3::ObjectType::Ammo:
		case TES3::ObjectType::Armor:
		case TES3::ObjectType::Book:
		case TES3::ObjectType::Clothing:
		case TES3::ObjectType::Misc:
		case TES3::ObjectType::Weapon:
			nameContainer = &reinterpret_cast<TES3::Armor*>(recordGeneric)->name;
			namePtr = *nameContainer;
			break;
		case TES3::ObjectType::Activator:
			nameContainer = &reinterpret_cast<TES3::Activator*>(recordGeneric)->name;
			namePtr = *nameContainer;
			break;
		case TES3::ObjectType::Door:
			namePtr = reinterpret_cast<TES3::Door*>(recordGeneric)->name;
			break;
		case TES3::ObjectType::Apparatus:
			namePtr = reinterpret_cast<TES3::Apparatus*>(recordGeneric)->name;
			break;
		case TES3::ObjectType::Ingredient:
		case TES3::ObjectType::Lockpick:
		case TES3::ObjectType::Probe:
		case TES3::ObjectType::Repair:
			namePtr = reinterpret_cast<TES3::Lockpick*>(recordGeneric)->name;
			break;
		}

		// Bail out if we haven't found the name.
		if (namePtr == NULL) {
#if _DEBUG
			mwse::log::getLog() << "xSetName: Unsupported record format: " << recordType << "." << std::endl;
#endif
			mwse::Stack::getInstance().pushShort(false);
			return 0.0f;
		}

		// Some strings might need to be recreated.
		if (nameContainer != NULL && name.length() > strlen(namePtr)) {
			namePtr = reinterpret_cast<char*>(tes3::realloc(namePtr, name.length() + 1));
			*nameContainer = namePtr;
		}

		strcpy(namePtr, name.c_str());

		mwse::Stack::getInstance().pushShort(true);
		return 0.0f;
	}
}