/************************************************************************
	
	xKeyPressed.cpp - Copyright (c) 2008 The MWSE Project
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

using namespace mwse;

namespace mwse {
	class xKeyPressed : mwse::InstructionInterface_t {
	public:
		xKeyPressed();
		virtual float execute(VMExecuteInterface &virtualMachine);
		virtual void loadParameters(VMExecuteInterface &virtualMachine);
	};

	static xKeyPressed xKeyPressedInstance;

	xKeyPressed::xKeyPressed() : mwse::InstructionInterface_t(OpCode::xKeyPressed) {}

	void xKeyPressed::loadParameters(mwse::VMExecuteInterface &virtualMachine) {}

	float xKeyPressed::execute(mwse::VMExecuteInterface &virtualMachine) {
		long keyCode = Stack::getInstance().popLong();

		// A particular key, based on virtual key codes.
		if (keyCode > 0 && keyCode < 256) {
			long state = GetAsyncKeyState(keyCode & 0xFF);
			if (state / 2) {
				state = 2 + state % 2;
			}
			Stack::getInstance().pushLong(state);
			return 0.0f;
		}

		// Any key will do, cycles through the list.
		else {
			long lastCode = 0;
			keyCode = 0;
			do {
				if (keyCode <= 0 || keyCode >= 255) {
					keyCode = 254;
				}
				else {
					keyCode--;
				}

				if (GetAsyncKeyState(keyCode) / 2) {
					// Generic Shift, Ctrl, and Alt only used if Left/Right versions aren't available
					if (keyCode == 16 && (GetAsyncKeyState(160) / 2 || GetAsyncKeyState(161) / 2)) continue;
					if (keyCode == 17 && (GetAsyncKeyState(162) / 2 || GetAsyncKeyState(163) / 2)) continue;
					if (keyCode == 18 && (GetAsyncKeyState(164) / 2 || GetAsyncKeyState(164) / 2)) continue;
					Stack::getInstance().pushLong(keyCode);
					return 0.0f;
				}
			} while (keyCode != lastCode);
		}

		// Fallthrough state.
		Stack::getInstance().pushLong(0);
		return 0.0f;
	}
}