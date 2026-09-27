#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"


namespace mwse {
	class xKeyPressed : InstructionInterface_t {
	public:
		xKeyPressed();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xKeyPressed xKeyPressedInstance;

	xKeyPressed::xKeyPressed() : mwse::InstructionInterface_t(OpCode::xKeyPressed) {}

	float xKeyPressed::execute(mwse::VMExecuteInterface& virtualMachine) {
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
