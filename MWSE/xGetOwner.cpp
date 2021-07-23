/************************************************************************
	
	xGetOwner.cpp - Copyright (c) 2008 The MWSE Project
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

#include "TES3DataHandler.h"
#include "TES3ItemData.h"
#include "TES3Reference.h"

using namespace mwse;

namespace mwse
{
	class xGetOwner : mwse::InstructionInterface_t
	{
	public:
		xGetOwner();
		virtual float execute(VMExecuteInterface &virtualMachine);
		virtual void loadParameters(VMExecuteInterface &virtualMachine);
	};

	static xGetOwner xGetOwnerInstance;

	xGetOwner::xGetOwner() : mwse::InstructionInterface_t(OpCode::xGetOwner) {}

	void xGetOwner::loadParameters(mwse::VMExecuteInterface &virtualMachine) {}

	float xGetOwner::execute(mwse::VMExecuteInterface &virtualMachine)
	{
		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
#if _DEBUG
			mwse::log::getLog() << "xGetOwner: No reference provided." << std::endl;
#endif
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		const char* owner = NULL;

		// Get the attached varnode.
		auto node = reference->getAttachedItemData();
		if (node && node->owner) {
			if (node->owner) {
				owner = node->owner->getObjectID();
			}
		}
		else {
#if _DEBUG
			mwse::log::getLog() << "xGetOwner: Could not obtain attached VARNODE." << std::endl;
#endif
		}

		mwse::Stack::getInstance().pushString(owner);

		return 0.0f;
	}
}