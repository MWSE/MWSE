/************************************************************************

	xLogMessage.cpp - Copyright (c) 2008 The MWSE Project
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
	// Provides an interface to mwse::log, allowing scripts to write to the default MWSE.log file.
	// 
	// MWScript Usage:
	//	<none> xLogMessage sFormat ...
	//
	// MWEdit definition:
	//	Name = XLogMessage
	//	Options = MWSE | AllowGlobal | ShortVar | AllowVar
	//	Return = None
	//	Param1 = Long | String
	//	Param2 = Short | Long | Float | String | Many | Optional
	//	Opcode = 0x3c09
	class xLogMessage : mwse::InstructionInterface_t
	{
	public:
		xLogMessage();
		virtual float execute(VMExecuteInterface &virtualMachine);
		virtual void loadParameters(VMExecuteInterface &virtualMachine);
	};

	static xLogMessage xLogMessageInstance;

	xLogMessage::xLogMessage() : mwse::InstructionInterface_t(OpCode::xLogMessage) {}

	void xLogMessage::loadParameters(mwse::VMExecuteInterface &virtualMachine) {}

	float xLogMessage::execute(mwse::VMExecuteInterface &virtualMachine)
	{
		mwseString& format = virtualMachine.getString(Stack::getInstance().popLong());

		bool suppressNull = false;
		std::string badCodes;
		std::string& result = mwse::string::interpolate(format, virtualMachine, &suppressNull, &badCodes);

		mwse::log::getLog() << result << std::endl;
		if (!badCodes.empty()) {
			mwse::log::getLog() << "xLogMessage: bad format \"" << badCodes << "\" in \"" << format << "\" generating \"" << result << "\"" << badCodes << std::endl;
		}

		return 0.0f;
	}
}