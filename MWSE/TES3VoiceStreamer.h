#pragma once

namespace mwse::patch::voice {

	// Install voice-streamer patches: the addTempSound LoadSoundFile divert and
	// the function-prologue replacements for the SoundBuffer leaf accessors.
	// Call from PatchUtil::installPatches().
	void install();

	// Stop the decode worker and join it. Call on game shutdown.
	void shutdown();

}
