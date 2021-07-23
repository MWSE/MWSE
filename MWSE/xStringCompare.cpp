/************************************************************************
	
	xStringCompare.cpp - Copyright (c) 2008 The MWSE Project
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
#include "Log.h"
#include "StringUtil.h"

using namespace mwse;

namespace mwse
{
	class xStringCompare : mwse::InstructionInterface_t
	{
	public:
		xStringCompare();
		virtual float execute(VMExecuteInterface &virtualMachine);
		virtual void loadParameters(VMExecuteInterface &virtualMachine);
	};

	static xStringCompare xStringCompareInstance;

	xStringCompare::xStringCompare() : mwse::InstructionInterface_t(OpCode::xStringCompare) {}

	void xStringCompare::loadParameters(mwse::VMExecuteInterface &virtualMachine) {}

	float xStringCompare::execute(mwse::VMExecuteInterface &virtualMachine)
	{
		mwseString& string1 = virtualMachine.getString(Stack::getInstance().popLong());
		mwseString& string2 = virtualMachine.getString(Stack::getInstance().popLong());

		long result = strcmp(string1.c_str(), string2.c_str());

		mwse::Stack::getInstance().pushLong(result);

		return 0.0f;
	}
}