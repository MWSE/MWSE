/************************************************************************
	
	xNextRef.cpp - Copyright (c) 2008 The MWSE Project
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
#include "TES3Reference.h"
#include "TES3Script.h"
#include "TES3GameFile.h"

#include <Windows.h>

using namespace mwse;

namespace mwse
{
	class xNextRef : mwse::InstructionInterface_t
	{
	public:
		xNextRef();
		virtual float execute(VMExecuteInterface &virtualMachine);
		virtual void loadParameters(VMExecuteInterface &virtualMachine);
	};

	static xNextRef xNextRefInstance;

	xNextRef::xNextRef() : mwse::InstructionInterface_t(OpCode::xNextRef) {}

	void xNextRef::loadParameters(mwse::VMExecuteInterface &virtualMachine) {}

	float xNextRef::execute(mwse::VMExecuteInterface &virtualMachine)
	{
		// Get REFR pointer as an argument.
		TES3::Reference* reference = (TES3::Reference*)mwse::Stack::getInstance().popLong();

		// Start looking for our next reference.
		TES3::Reference* next = NULL;
		__try {
			if (reference) {
				// Try to get the next non-removed reference linked down from the passed one.
				next = mwse::tes3::skipDeletedObjects<TES3::Reference>(reference->nextInCollection);

				// If we found nothing, check the stored exterior references.
				if (next == NULL && mwse::tes3::exteriorRefs[0] != NULL) {
					next = mwse::tes3::exteriorRefs[0];
					for (int i = 0; i < 8; i++) {
						mwse::tes3::exteriorRefs[i] = mwse::tes3::exteriorRefs[i + 1];
					}
				}
			}
			else {
#if _DEBUG
				mwse::log::getLog() << "xNextRef: Null argument." << std::endl;
#endif
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			TES3::Script * script = virtualMachine.getScript();
#if _DEBUG
			mwse::log::getLog() << "xNextRef: Invalid object given in script " << script->sourceMod->filename << "/" << script->name << ". Fix script to not save variables across saves!" << std::endl;
#endif
			next = NULL;
		}

		mwse::Stack::getInstance().pushLong((long)next);

		return 0.0f;
	}
}