/************************************************************************
	
	xNextStack.cpp - Copyright (c) 2008 The MWSE Project
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

#include "TES3Inventory.h"

using namespace mwse;

namespace mwse
{
	class xNextStack : mwse::InstructionInterface_t
	{
	public:
		xNextStack();
		virtual float execute(VMExecuteInterface &virtualMachine);
		virtual void loadParameters(VMExecuteInterface &virtualMachine);
	};

	static xNextStack xNextStackInstance;

	xNextStack::xNextStack() : mwse::InstructionInterface_t(OpCode::xNextStack) {}

	void xNextStack::loadParameters(mwse::VMExecuteInterface &virtualMachine) {}

	float xNextStack::execute(mwse::VMExecuteInterface &virtualMachine)
	{
		// Get the passed node.
		auto node = reinterpret_cast<TES3::IteratedList<TES3::ItemStack*>::Node*>(mwse::Stack::getInstance().popLong());
		if (node == NULL) {
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		mwse::Stack::getInstance().pushLong((long)node->next);
		mwse::Stack::getInstance().pushLong(node->data->count);
		mwse::Stack::getInstance().pushString(node->data->object->vTable.object->getObjectID(node->data->object));

		return 0.0f;
	}
}