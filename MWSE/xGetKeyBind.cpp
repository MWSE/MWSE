#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3WorldController.h"
#include "TES3InputController.h"
#include "TES3DataHandler.h"


namespace mwse {
	class xGetKeyBind : InstructionInterface_t {
	public:
		xGetKeyBind();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	enum xGetKeyBindReturnType {
		ReturnVirtualKeyCode,
		ReturnScanCode
	};

	static xGetKeyBind xGetKeyBindInstance;

	xGetKeyBind::xGetKeyBind() : mwse::InstructionInterface_t(OpCode::xGetKeyBind) {}

	float xGetKeyBind::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get parameters.
		long inputIndex = Stack::getInstance().popLong();

		// Validate index.
		if (inputIndex < TES3::KeyBind::FirstKey || inputIndex > TES3::KeyBind::LastKey) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetKeyBind: Index out of bounds." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		// Get our structure.
		TES3::InputConfig& config = TES3::WorldController::get()->inputController->inputMaps[inputIndex];

		// Get the basic codes/return values.
		long scanCode = config.keyCode;
		long scanDevice = config.device;
		long keyCode = 0;

		if (scanDevice == TES3::InputDevice::Keyboard) {
			keyCode = MapVirtualKey(scanCode, MAPVK_VSC_TO_VK);
		}
		else if (scanDevice == TES3::InputDevice::Mouse) {
			switch (scanCode) {
			case 0x00:
				keyCode = VK_LBUTTON;
				break;
			case 0x01:
				keyCode = VK_RBUTTON;
				break;
			case 0x02:
				keyCode = VK_MBUTTON;
				break;
			case 0x03:
				keyCode = VK_XBUTTON1;
				break;
			case 0x04:
				keyCode = VK_XBUTTON2;
				break;
			}
		}
		else {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetKeyBind: Device type " << scanDevice << " cannot be converted." << std::endl;
			}
		}

		// Return values.
		mwse::Stack::getInstance().pushLong(keyCode);
		mwse::Stack::getInstance().pushLong(scanDevice);
		mwse::Stack::getInstance().pushLong(scanCode);

		return 0.0f;
	}
}
