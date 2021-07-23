/************************************************************************
	
	xSetGSString.cpp - Copyright (c) 2008 The MWSE Project
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

#include "TES3DataHandler.h"
#include "TES3GameSetting.h"

using namespace mwse;

namespace mwse
{
	class xSetGSString : mwse::InstructionInterface_t
	{
	public:
		xSetGSString();
		virtual float execute(VMExecuteInterface &virtualMachine);
		virtual void loadParameters(VMExecuteInterface &virtualMachine);
	};

	static xSetGSString xSetGSStringInstance;

	xSetGSString::xSetGSString() : mwse::InstructionInterface_t(OpCode::xSetGSString) {}

	void xSetGSString::loadParameters(mwse::VMExecuteInterface &virtualMachine) {}

	float xSetGSString::execute(mwse::VMExecuteInterface &virtualMachine)
	{
		long gmstId = Stack::getInstance().popLong();
		mwseString& newString = virtualMachine.getString(Stack::getInstance().popLong());

		if (gmstId < 0) {
			mwse::log::getLog() << "xSetGSString: Invalid GMST id." << std::endl;
			mwse::Stack::getInstance().pushLong(false);
			return 0.0f;
		}

		// Get the string we're going to change.
		TES3::DataHandler* dataHandler = TES3::DataHandler::get();
		char *& oldString = dataHandler->nonDynamicData->GMSTs[gmstId]->value.asString;

		// Reallocate string memory if it is growing in size.
		if (newString.length() > strlen(oldString)) {
			oldString = reinterpret_cast<char*>(tes3::realloc(oldString, newString.length() + 1));
		}
		
		// Copy over new value.
		strcpy(oldString, newString.c_str());

		mwse::Stack::getInstance().pushLong(true);
		return 0.0f;
	}
}