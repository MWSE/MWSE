/************************************************************************
	
	xInventory.cpp - Copyright (c) 2008 The MWSE Project
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
#include "TES3Util.h"

#include "TES3Actor.h"
#include "TES3Inventory.h"
#include "TES3Reference.h"

namespace mwse
{
	class xInventory : mwse::InstructionInterface_t
	{
	public:
		xInventory();
		virtual float execute(VMExecuteInterface &virtualMachine);
		virtual void loadParameters(VMExecuteInterface &virtualMachine);
	};

	static xInventory xInventoryInstance;

	xInventory::xInventory() : mwse::InstructionInterface_t(OpCode::xInventory) {}

	void xInventory::loadParameters(mwse::VMExecuteInterface &virtualMachine) {}

	float xInventory::execute(mwse::VMExecuteInterface &virtualMachine)
	{
		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
#if _DEBUG
			mwse::log::getLog() << "xInventory: Invalid reference attachment." << std::endl;
#endif
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		if (!reference->baseObject->isActor()) {
#if _DEBUG
			mwse::log::getLog() << "xInventory: Reference is not for an actor." << std::endl;
#endif
			mwse::Stack::getInstance().pushFloat(0.0f);
			return 0.0f;
		}
		
		TES3::IteratedList<TES3::ItemStack*>::Node* firstItem = static_cast<TES3::Actor*>(reference->baseObject)->inventory.itemStacks.head;
		if (firstItem == NULL) {
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		mwse::Stack::getInstance().pushLong((long)firstItem->next);
		mwse::Stack::getInstance().pushLong(firstItem->data->count);
		mwse::Stack::getInstance().pushString(firstItem->data->object->vTable.object->getObjectID(firstItem->data->object));

		return 0.0f;
	}
}
