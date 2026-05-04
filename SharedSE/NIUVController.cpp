#include "NIUVController.h"

namespace NI {
	const auto NI_UVController_copy = reinterpret_cast<void(__thiscall*)(const UVController*, UVController*)>(0x722330);
	void UVController::copy(UVController* to) const {
		to->textureSet = textureSet;
		NI_UVController_copy(this, to);
	}
}
