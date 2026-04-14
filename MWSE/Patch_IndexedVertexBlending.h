#pragma once

namespace mwse::patch::IndexedVertexBlending {
	// Installs hooks that switch Morrowind's skinning pipeline from D3D8
	// Geometry Blending (per-partition matrix palette, up to 4 bones, XYZB1-3
	// vertex formats) to D3D8 Indexed Vertex Blending (global matrix palette,
	// up to MaxVertexBlendMatrixIndex bones, XYZB5 | LASTBETA_UBYTE4 vertex
	// format) so each skinned mesh collapses to one draw call.
	//
	// Runtime gate lives on the cap-probe hook (Hook A): if the device reports
	// MaxVertexBlendMatrixIndex < 4 all other detours fall through to vanilla
	// and the skinning pipeline is behaviourally unchanged.
	//
	// Called from PatchUtil::installPatches() at startup. Safe to skip the
	// call to disable the feature; no partial state is left behind.
	void install();
}
