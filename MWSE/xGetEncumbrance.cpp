/************************************************************************

	xGetEncumbrance.cpp - Copyright (c) 2008 The MWSE Project
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

#include "TES3MobileNPC.h"
#include "TES3NPC.h"
#include "TES3Reference.h"
#include "TES3WorldController.h"

using namespace mwse;

namespace mwse
{
	class xGetEncumbrance : mwse::InstructionInterface_t
	{
	public:
		xGetEncumbrance();
		virtual float execute(VMExecuteInterface &virtualMachine);
		virtual void loadParameters(VMExecuteInterface &virtualMachine);

		enum EncumbranceQueryType {
			CurrentEncumbrance = 0,
			MaxEncumbrance = 1,
			BaseEncumbrance = 2
		};
	};

	static xGetEncumbrance xGetEncumbranceInstance;

	xGetEncumbrance::xGetEncumbrance() : mwse::InstructionInterface_t(OpCode::xGetEncumbrance) {}

	void xGetEncumbrance::loadParameters(mwse::VMExecuteInterface &virtualMachine) {}

	float xGetEncumbrance::execute(mwse::VMExecuteInterface &virtualMachine)
	{
		// Get parameters.
		EncumbranceQueryType queryType = static_cast<EncumbranceQueryType>(mwse::Stack::getInstance().popLong());
		bool roundResult = mwse::Stack::getInstance().popLong();

		// Get reference to target.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
#if _DEBUG
			mwse::log::getLog() << "xGetEncumbrance: No reference provided." << std::endl;
#endif
			mwse::Stack::getInstance().pushFloat(0.0f);
			return 0.0f;
		}

		// Get record for reference.
		TES3::BaseObject* record = reference->baseObject;
		if (record == NULL) {
#if _DEBUG
			mwse::log::getLog() << "xGetEncumbrance: No record found for reference." << std::endl;
#endif
			mwse::Stack::getInstance().pushFloat(0.0f);
			return 0.0f;
		}

		// This function only supports creatures and NPCs.
		if (record->objectType != TES3::ObjectType::NPC && record->objectType != TES3::ObjectType::Creature) {
#if _DEBUG
			mwse::log::getLog() << "xGetEncumbrance: Called on unsupported record type " << record->objectType << "." << std::endl;
#endif
			mwse::Stack::getInstance().pushFloat(0.0f);
			return 0.0f;
		}

		double encumbrance = -999999.0;

		// Get associated MACP node.
		auto mobileObject = reference->getAttachedMobileActor();
		if (mobileObject == NULL) {
#if _DEBUG
			mwse::log::getLog() << "xGetEncumbrance: No associated macp record found for reference." << std::endl;
#endif
			mwse::Stack::getInstance().pushFloat(0.0f);
			return 0.0f;
		}

		// Get encumbrance.
		if (queryType == MaxEncumbrance) {
			encumbrance = mobileObject->encumbrance.base;
		}
		else if (queryType == BaseEncumbrance || queryType == CurrentEncumbrance) {
			encumbrance = mobileObject->encumbrance.current;
		}

		// Round result.
		if (roundResult) {
			encumbrance = round(encumbrance * 100.0) / 100.0;
		}

		mwse::Stack::getInstance().pushFloat(encumbrance);

		return 0.0f;
	}
}