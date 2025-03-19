#include "TES3AudioControllerLua.h"

#include "LuaUtil.h"
#include "LuaManager.h"

#include "TES3AudioController.h"

namespace mwse::lua {
	void bindTES3AudioController() {
		// Get our lua state.
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;

		// Start our usertype.
		auto usertypeDefinition = state.new_usertype<TES3::AudioController>("tes3audioController");
		usertypeDefinition["new"] = sol::no_constructor;

		// Basic property binding.
		usertypeDefinition["audioFlags"] = &TES3::AudioController::audioFlags;
		usertypeDefinition["disableAudio"] = &TES3::AudioController::disableAudio;
		usertypeDefinition["dsound3DChanged"] = &TES3::AudioController::dsound3DChanged;
		usertypeDefinition["dsound3DCommitted"] = &TES3::AudioController::dsound3DCommitted;
		usertypeDefinition["listenerPosition"] = &TES3::AudioController::listenerPosition;
		usertypeDefinition["musicFadeBeginTimestamp"] = &TES3::AudioController::timestampBeginFade;
		usertypeDefinition["musicFlags"] = &TES3::AudioController::musicFlags;
		usertypeDefinition["musicNextTrackStartTimestamp"] = &TES3::AudioController::timestampNextTrackStart;
		usertypeDefinition["musicNextTrackVolume"] = &TES3::AudioController::volumeNextTrack;
		usertypeDefinition["pitchAxis"] = &TES3::AudioController::pitchAxisApproximated;
		usertypeDefinition["yawAxis"] = &TES3::AudioController::yawAxis;

		// Properties exposed through functions.
		usertypeDefinition["currentMusicFilePath"] = sol::property(&TES3::AudioController::getCurrentMusicFilePath, &TES3::AudioController::setCurrentMusicFilePath);
		usertypeDefinition["directSoundInitFailed"] = sol::property(&TES3::AudioController::getDirectSoundInitFailed, &TES3::AudioController::setDirectSoundInitFailed);
		usertypeDefinition["hasStaticBuffers"] = sol::property(&TES3::AudioController::getHasStaticBuffers, &TES3::AudioController::setHasStaticBuffers);
		usertypeDefinition["hasStreamingBuffers"] = sol::property(&TES3::AudioController::getHasStreamingBuffers, &TES3::AudioController::setHasStreamingBuffers);
		usertypeDefinition["isFilterGraphValid"] = sol::property(&TES3::AudioController::getIsFilterGraphValid, &TES3::AudioController::setIsFilterGraphValid);
		usertypeDefinition["isMusicPaused"] = sol::property(&TES3::AudioController::getIsMusicPaused, &TES3::AudioController::setIsMusicPaused);
		usertypeDefinition["isMusicPlaying"] = sol::property(&TES3::AudioController::getIsMusicPlaying, &TES3::AudioController::setIsMusicPlaying);
		usertypeDefinition["nextMusicFilePath"] = sol::property(&TES3::AudioController::getNextMusicFilePath, &TES3::AudioController::setNextMusicFilePath);
		usertypeDefinition["volumeEffects"] = sol::property(&TES3::AudioController::getNormalizedEffectsVolume, &TES3::AudioController::setNormalizedEffectsVolume);
		usertypeDefinition["volumeFootsteps"] = sol::property(&TES3::AudioController::getNormalizedFootstepsVolume, &TES3::AudioController::setNormalizedFootstepsVolume);
		usertypeDefinition["volumeMaster"] = sol::property(&TES3::AudioController::getNormalizedMasterVolume, &TES3::AudioController::setNormalizedMasterVolume);
		usertypeDefinition["volumeMusic"] = sol::property(&TES3::AudioController::getMusicVolume, &TES3::AudioController::setMusicVolume);
		usertypeDefinition["volumeVoice"] = sol::property(&TES3::AudioController::getNormalizedVoiceVolume, &TES3::AudioController::setNormalizedVoiceVolume);

		// Basic function binding.
		usertypeDefinition["getMixVolume"] = &TES3::AudioController::getMixVolume;
		usertypeDefinition["pauseMusic"] = &TES3::AudioController::pauseMusic;
		usertypeDefinition["unpauseMusic"] = &TES3::AudioController::unpauseMusic;
		usertypeDefinition["getMusicFileDuration"] = &TES3::AudioController::getMusicFileDuration;

		// Functions exposed as properties.
		usertypeDefinition["musicDuration"] = sol::readonly_property(&TES3::AudioController::getMusicDuration);
		usertypeDefinition["musicPosition"] = sol::property(&TES3::AudioController::getMusicPosition, &TES3::AudioController::setMusicPosition);

		// Wrapped functions.
		usertypeDefinition["changeMusicTrack"] = &TES3::AudioController::changeMusicTrack_lua;
	}
}
