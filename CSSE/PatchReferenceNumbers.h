#pragma once

namespace se::cs::patch::reference_numbers {
	// Keeps the reference numbers (FRMR) of the active plugin's references stable when saving.
	// See PatchReferenceNumbers.cpp for details.
	void installPatches();
}
