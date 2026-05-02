#include "TES3LoadScreenManager.h"

#include "TES3AudioController.h"
#include "TES3CutscenePlayer.h"
#include "TES3WorldController.h"

namespace TES3 {
	void LoadScreenManager::open(const char* _movieFile, bool canSkip, bool alwaysFalse1, bool alwaysFalse2, bool unknown1, bool alwaysFalse3, bool unknown2) {
		const auto TES3_LoadScreenManager_open = reinterpret_cast<void(__thiscall*)(LoadScreenManager*, const char*, bool, bool, bool, bool, bool, bool)>(0x4587E0);
		TES3_LoadScreenManager_open(this, _movieFile, canSkip, alwaysFalse1, alwaysFalse2, unknown1, alwaysFalse3, unknown2);

	}

	bool LoadScreenManager::render(bool unknown1, bool unknown2, bool unknown3) {
		const auto TES3_LoadScreenManager_render = reinterpret_cast<bool(__thiscall*)(LoadScreenManager*, bool, bool, bool)>(0x458910);
		return TES3_LoadScreenManager_render(this, unknown1, unknown2, unknown3);
	}

	void LoadScreenManager::clearMovieState(bool unknown) {
		const auto TES3_LoadScreenManager_clearMovieState = reinterpret_cast<void(__thiscall*)(LoadScreenManager*, bool)>(0x4599E0);
		TES3_LoadScreenManager_clearMovieState(this, unknown);
	}

	bool LoadScreenManager::play(const char* movieFile, bool canSkip) {
		// Clear existing movie state.
		clearMovieState();

		// Load up the new movie.
		open(movieFile);

		// Store background volumes.
		auto audioController = WorldController::get()->audioController;
		const auto voiceVolume = audioController->getNormalizedVoiceVolume();
		const auto footVolume = audioController->getNormalizedFootstepsVolume();
		const auto musicVolume = audioController->getMusicVolume();

		// Clear the volumes so only music plays.
		audioController->setNormalizedVoiceVolume(0.0f);
		audioController->setNormalizedFootstepsVolume(0.0f);
		audioController->setMusicVolume(0.0f);

		// Keep rendering until we are done.
		while (shouldContinueRendering()) {
			render();
		}

		// Clean up from rendering.
		clearMovieState();

		// Restore background audio.
		audioController->setNormalizedVoiceVolume(voiceVolume);
		audioController->setNormalizedFootstepsVolume(footVolume);
		audioController->setMusicVolume(musicVolume);

		return true;
	}

	bool LoadScreenManager::shouldContinueRendering() const {
		return cutscenePlayer->state == CutscenePlayer::State::Playing
			|| cutscenePlayer->state == CutscenePlayer::State::Paused;
	}
}
