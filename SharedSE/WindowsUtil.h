#pragma once

namespace se::windows {
	bool isKeyDown(int key);

	inline bool isAltDown() {
		return isKeyDown(VK_MENU);
	}

	inline bool isControlDown() {
		return isKeyDown(VK_CONTROL);
	}

	inline bool isShiftDown() {
		return isKeyDown(VK_SHIFT) || isKeyDown(VK_RSHIFT);
	}

	inline bool isLeftMouseDown() {
		return isKeyDown(VK_LBUTTON);
	}

	inline bool isRightMouseDown() {
		return isKeyDown(VK_RBUTTON);
	}

	std::filesystem::path getModulePath(HINSTANCE hInstance);
}
