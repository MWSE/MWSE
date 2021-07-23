/************************************************************************
	
	xStringCapture.cpp - Copyright (c) 2008 The MWSE Project
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
	class xStringCapture : mwse::InstructionInterface_t
	{
	public:
		xStringCapture();
		virtual float execute(VMExecuteInterface &virtualMachine);
		virtual void loadParameters(VMExecuteInterface &virtualMachine);
	};

	static xStringCapture xStringCaptureInstance;

	xStringCapture::xStringCapture() : mwse::InstructionInterface_t(OpCode::xStringCapture) {}

	void xStringCapture::loadParameters(mwse::VMExecuteInterface &virtualMachine) {}

	float xStringCapture::execute(mwse::VMExecuteInterface &virtualMachine)
	{
		// Get parameters from stack.
		mwseString& string = virtualMachine.getString(Stack::getInstance().popLong());
		mwseString& pattern = virtualMachine.getString(Stack::getInstance().popLong());
		long desiredMatches = Stack::getInstance().popLong();

		// Go and try to get all of our matches, to a limit of the count given to us as our 3rd parameter.
		long matchesReturned = 0;
		try {
			boost::regex regex_pattern(pattern);
			boost::smatch regex_matches;
			if (boost::regex_search(string, regex_matches, regex_pattern)) {
				// The capture groups begin at index 1, so start there.
				for (size_t i = 1; i < regex_matches.size(); i++) {
					// Bail out if we're past our desired number of matches.
					if (matchesReturned >= desiredMatches) {
						break;
					}

					// Bring the match into string storage and push it back to mwscript.
					mwseString& match = mwse::string::store::getOrCreate(regex_matches[i].str());
					mwse::Stack::getInstance().pushString(match);
					matchesReturned++;
				}
			}
		}
		catch (boost::regex_error& e) {
			mwse::log::getLog() << "xStringCapture: A regex exception has occurred. " << e.what() << std::endl;
		}

		// If we didn't get enough matches, pad it out with zeros.
		while (matchesReturned < desiredMatches) {
			mwse::Stack::getInstance().pushLong(0);
			matchesReturned++;
		}

		return 0.0f;
	}
}