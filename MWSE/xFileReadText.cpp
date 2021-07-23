/************************************************************************
	
	xFileReadText.cpp - Copyright (c) 2008 The MWSE Project
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
#include "StringUtil.h"

using namespace mwse;

namespace mwse
{
	class xFileReadText : mwse::InstructionInterface_t
	{
	public:
		xFileReadText();
		virtual float execute(VMExecuteInterface &virtualMachine);
		virtual void loadParameters(VMExecuteInterface &virtualMachine);
	};

	static xFileReadText xFileReadTextInstance;

	xFileReadText::xFileReadText() : mwse::InstructionInterface_t(OpCode::xFileReadText) {}

	void xFileReadText::loadParameters(mwse::VMExecuteInterface &virtualMachine) {}

	float xFileReadText::execute(mwse::VMExecuteInterface &virtualMachine)
	{
		if (mwse::Stack::getInstance().size() < 2) {
			mwse::log::getLog() << "xFileReadText: Function called with too few arguments." << std::endl;
			return 0.0f;
		}

		mwseString& fileName = virtualMachine.getString(mwse::Stack::getInstance().popLong());
		mwseString& format = virtualMachine.getString(mwse::Stack::getInstance().popLong());

		// Count how many results there should be based on the format string
		bool stopAtEndOfLine = false;
		int maxResults = 0;
		mwse::string::enumerate(format.c_str(), maxResults, stopAtEndOfLine);
		maxResults++;

		// Store results so we can push them on in reverse.
		long* results = new long[maxResults];

		// Read the string from the file. If we can't read a string back, push 0s.
		std::string readString = mwse::FileSystem::getInstance().readString(fileName.c_str(), stopAtEndOfLine);
		if (readString.empty()) {
			while (maxResults--) {
				mwse::Stack::getInstance().pushLong(0);
			}
			return 0.0f;
		}

		// If we did get a string back, secernate and return.
		mwse::string::secernate(format.c_str(), readString.c_str(), results, maxResults);
		while (maxResults--) {
			mwse::Stack::getInstance().pushLong(results[maxResults]);
		}

		return 0.0f;
	}
}