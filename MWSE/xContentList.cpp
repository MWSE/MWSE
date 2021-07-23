/************************************************************************
	
	xContentList.cpp - Copyright (c) 2008 The MWSE Project
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

using namespace mwse;

namespace mwse {
	class xContentList : mwse::InstructionInterface_t {
	public:
		xContentList();
		virtual float execute(VMExecuteInterface &virtualMachine);
		virtual void loadParameters(VMExecuteInterface &virtualMachine);
	};

	static xContentList xContentListInstance;

	xContentList::xContentList() : mwse::InstructionInterface_t(OpCode::xContentList) {}

	void xContentList::loadParameters(mwse::VMExecuteInterface &virtualMachine) {}

	float xContentList::execute(mwse::VMExecuteInterface &virtualMachine) {
		// Get parameters.
		TES3::IteratedList<TES3::ItemStack*>::Node* node = reinterpret_cast<TES3::IteratedList<TES3::ItemStack*>::Node*>(mwse::Stack::getInstance().popLong());

		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
#if _DEBUG
			mwse::log::getLog() << "xContentList: Called on invalid reference." << std::endl;
#endif
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushFloat(0.0f);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		// Results.
		const char* id = NULL;
		long count = 0;
		long type = 0;
		long value = 0;
		float weight = 0;
		char* name = NULL;
		TES3::IteratedList<TES3::ItemStack*>::Node* next = NULL;

		// If we aren't given a node, get the first one.
		if (node == NULL && reference->baseObject->isActor()) {
			node = static_cast<TES3::Actor*>(reference->baseObject)->inventory.itemStacks.head;
		}

		// Validate the node we've obtained.
		if (node && node->data && node->data->object) {
			TES3::Object* object = node->data->object;
			
			id = object->vTable.object->getObjectID(object);
			count = node->data->count;
			type = object->objectType;
			value = object->vTable.object->getValue(object);
			weight = object->vTable.object->getWeight(object);
			name = object->vTable.object->getName(object);
			
			next = node->next;
		}

		// Push values to the stack.
		mwse::Stack::getInstance().pushLong((long)next);
		mwse::Stack::getInstance().pushString(name);
		mwse::Stack::getInstance().pushFloat(weight);
		mwse::Stack::getInstance().pushLong(value);
		mwse::Stack::getInstance().pushLong(type);
		mwse::Stack::getInstance().pushLong(count);
		mwse::Stack::getInstance().pushString(id);

		return 0.0f;
	}
}