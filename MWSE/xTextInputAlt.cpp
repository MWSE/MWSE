#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "BitUtil.h"
#include "StringUtil.h"


namespace mwse {
	class xTextInputAlt : InstructionInterface_t {
	public:
		xTextInputAlt();
		float execute(VMExecuteInterface& virtualMachine) override;

	private:
		bool GetKeyIsPressed(int VK_key);
	};

	static xTextInputAlt xTextInputAltInstance;

	xTextInputAlt::xTextInputAlt() : InstructionInterface_t(OpCode::xTextInputAlt) {}

	static std::map<int, char> keyCharMap;

	bool xTextInputAlt::GetKeyIsPressed(int VK_key) {
		return BITMASK_TEST(GetAsyncKeyState(VK_key), 0x8001);
	}

	float xTextInputAlt::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// 1st parameter: Message Id.
		mwseString& message = virtualMachine.getString(stack.popLong());

		// 2nd parameter: Key to use to end the input stream.
		long endCode = stack.popLong();

		// The default endcode is return.
		if (endCode == 0) {
			endCode = VK_RETURN;
		}

		// Determine if the end keycode is down, and break out if so.
		if (GetKeyIsPressed(endCode)) {
			stack.pushString(message);
			stack.pushLong(message.length());
			return 0.0f;
		}

		// If the back key is pressed, remove a character.
		if (GetKeyIsPressed(VK_BACK)) {
			if (message.length() > 0) {
				message.erase(message.end() - 1);
			}
			stack.pushString(message);
			stack.pushLong(0);
			return 0.0f;
		}

		// If the enter key is pressed, insert a newline.
		if (GetKeyIsPressed(VK_RETURN)) {
			message.append("\r\n");
			stack.pushString(message);
			stack.pushLong(0);
			return 0.0f;
		}

		// Handle spaces.
		if (GetKeyIsPressed(VK_SPACE)) {
			message.append(" ");
			stack.pushString(message);
			stack.pushLong(0);
			return 0.0f;
		}

		// Handle all other keys.
		for (int i = 0x30; i <= 0x6F; ++i) {
			if (GetKeyIsPressed(i)) {
				char character = MapVirtualKey(i, MAPVK_VK_TO_CHAR);

				// Lowercase character if shift isn't pressed.
				if (!GetAsyncKeyState(VK_SHIFT)) {
					character = tolower(character);
				}

				if (character > 0) {
					message.push_back(character);
					stack.pushString(message);
					stack.pushLong(0);
					return 0.0f;
				}
			}
		}

		return 0.0f;
	}
}
