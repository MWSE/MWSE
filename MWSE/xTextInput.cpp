/************************************************************************

	xTextInput.cpp - Copyright (c) 2008 The MWSE Project
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
#include "StringUtil.h"

using namespace mwse;

namespace mwse {
	class xTextInput : mwse::InstructionInterface_t {
	public:
		xTextInput();
		virtual float execute(VMExecuteInterface &virtualMachine);
		virtual void loadParameters(VMExecuteInterface &virtualMachine);

	private:
		bool GetKeyIsPressed(int VK_key);
	};

	static xTextInput xTextInputInstance;

	xTextInput::xTextInput() : mwse::InstructionInterface_t(OpCode::xTextInput) {}

	void xTextInput::loadParameters(mwse::VMExecuteInterface &virtualMachine) {}

	static std::map<int, char> keyCharMap;

	bool xTextInput::GetKeyIsPressed(int VK_key) {
		return (GetAsyncKeyState(VK_key) & 0x8001) == 0x8001;
	}

	float xTextInput::execute(mwse::VMExecuteInterface &virtualMachine) {
		// 1st parameter: Message Id.
		mwseString& message = virtualMachine.getString(mwse::Stack::getInstance().popLong());

		// 2nd parameter: Key to use to end the input stream. 
		long endCode = mwse::Stack::getInstance().popLong();

		// The default endcode is return.
		if (endCode == 0) {
			endCode = VK_RETURN;
		}

		// Loop until we get a key press.
		while (true) {
			// Determine if the end keycode is down, and break out if so.
			if (GetKeyIsPressed(endCode)) {
				mwse::Stack::getInstance().pushString(message);
				mwse::Stack::getInstance().pushLong(message.length());
				return 0.0f;
			}

			// If the back key is pressed, remove a character.
			if (GetKeyIsPressed(VK_BACK)) {
				if (message.length() > 0) {
					message.erase(message.end() - 1);
				}
				mwse::Stack::getInstance().pushString(message);
				mwse::Stack::getInstance().pushLong(0);
				return 0.0f;
			}

			// If the enter key is pressed, insert a newline.
			if (GetKeyIsPressed(VK_RETURN)) {
				message.append("\r\n");
				mwse::Stack::getInstance().pushString(message);
				mwse::Stack::getInstance().pushLong(0);
				return 0.0f;
			}

			// Handle spaces.
			if (GetKeyIsPressed(VK_SPACE)) {
				message.append(" ");
				mwse::Stack::getInstance().pushString(message);
				mwse::Stack::getInstance().pushLong(0);
				return 0.0f;
			}

			// Handle all other keys.
			for (int i = 0x30; i <= 0x6F; i++) {
				if (GetKeyIsPressed(i)) {
					char character = MapVirtualKey(i, MAPVK_VK_TO_CHAR);

					// Lowercase character if shift isn't pressed.
					if (!GetAsyncKeyState(VK_SHIFT)) {
						character = tolower(character);
					}

					if (character > 0) {
						message.push_back(character);
						mwse::Stack::getInstance().pushString(message);
						mwse::Stack::getInstance().pushLong(0);
						return 0.0f;
					}
				}
			}
		}

		return 0.0f;
	}
}