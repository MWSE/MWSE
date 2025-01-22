#include "NIUVController.h"

namespace NI {
	const auto NI_UVController_copy = reinterpret_cast<void(__thiscall*)(const UVController*, UVController*)>(0x722330);
	void UVController::copy(UVController* to) const {
		// Make sure the texture set index is cloned.
		to->textureSet = textureSet;

		// Call original code.
		NI_UVController_copy(this, to);
	}
}
