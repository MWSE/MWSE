/************************************************************************
	
	xFileReadString.cpp - Copyright (c) 2008 The MWSE Project
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
#include "FileUtil.h"

using namespace mwse;

namespace mwse
{
	class xFileReadString : mwse::InstructionInterface_t
	{
	public:
		xFileReadString();
		virtual float execute(VMExecuteInterface &virtualMachine);
		virtual void loadParameters(VMExecuteInterface &virtualMachine);
	};

	static xFileReadString xFileReadStringInstance;

	xFileReadString::xFileReadString() : mwse::InstructionInterface_t(OpCode::xFileReadString) {}

	void xFileReadString::loadParameters(mwse::VMExecuteInterface &virtualMachine) {}

	float xFileReadString::execute(mwse::VMExecuteInterface &virtualMachine)
	{
		if (mwse::Stack::getInstance().size() < 1) {
			mwse::log::getLog() << "xFileReadString: Function called with too few arguments." << std::endl;
			return 0.0f;
		}

		// Get filename as an argument.
		mwseString& fileName = virtualMachine.getString(mwse::Stack::getInstance().popLong());

		// Read the string from the file.
		std::string readString = mwse::FileSystem::getInstance().readString(fileName.c_str(), true);
		
		// Push the found string to the stack.
		if (!readString.empty()) {
			mwse::Stack::getInstance().pushString(readString);
		}
		else {
			// If we didn't read a string, "null" is expected.
			mwse::Stack::getInstance().pushString("null");
		}

		return 0.0f;
	}
}