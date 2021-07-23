/************************************************************************
	
	xFileReadLong.cpp - Copyright (c) 2008 The MWSE Project
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
	class xFileReadLong : mwse::InstructionInterface_t
	{
	public:
		xFileReadLong();
		virtual float execute(VMExecuteInterface &virtualMachine);
		virtual void loadParameters(VMExecuteInterface &virtualMachine);
	};

	static xFileReadLong xFileReadLongInstance;

	xFileReadLong::xFileReadLong() : mwse::InstructionInterface_t(OpCode::xFileReadLong) {}

	void xFileReadLong::loadParameters(mwse::VMExecuteInterface &virtualMachine) {}

	float xFileReadLong::execute(mwse::VMExecuteInterface &virtualMachine)
	{
		if (mwse::Stack::getInstance().size() < 2) {
			mwse::log::getLog() << "xFileReadLong: Function called with too few arguments." << std::endl;
			return 0.0f;
		}

		// Get arguments from the stack.
		mwseString& fileName = virtualMachine.getString(mwse::Stack::getInstance().popLong());
		long count = mwse::Stack::getInstance().popLong();

		// Gather values into a temporary list, so they aren't in reverse order.
		long valuesRead = 0;
		std::list<long> values;
		for (long i = 0; i < count; i++) {
			try {
				long value = mwse::FileSystem::getInstance().readLong(fileName.c_str());
				values.push_front(value);
				valuesRead++;
			}
			catch (std::exception&) {
				values.push_front(0);
			}
		}

		// Copy values from the temporary vector to the stack.
		while (!values.empty()) {
			mwse::Stack::getInstance().pushLong(values.front());
			values.pop_front();
		}
		mwse::Stack::getInstance().pushLong(valuesRead);

		return 0.0f;
	}
}