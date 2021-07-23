#include "TES3AudioControllerLua.h"

#include "LuaUtil.h"
#include "LuaManager.h"

#include "TES3AudioController.h"

namespace mwse {
	namespace lua {
		void bindTES3AudioController() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.new_usertype<TES3::AudioController>("tes3audioController");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["disableAudio"] = &TES3::AudioController::disableAudio;
			usertypeDefinition["listenerPosition"] = &TES3::AudioController::listenerPosition;
			usertypeDefinition["musicFadeBeginTimestamp"] = &TES3::AudioController::timestampBeginFade;
			usertypeDefinition["musicNextTrackStartTimestamp"] = &TES3::AudioController::timestampNextTrackStart;
			usertypeDefinition["musicNextTrackVolume"] = &TES3::AudioController::volumeNextTrack;
			usertypeDefinition["pitchAxis"] = &TES3::AudioController::pitchAxisApproximated;
			usertypeDefinition["yawAxis"] = &TES3::AudioController::yawAxis;

			// Properties exposed through functions.
			usertypeDefinition["currentMusicFilePath"] = sol::property(&TES3::AudioController::getCurrentMusicFilePath, &TES3::AudioController::setCurrentMusicFilePath);
			usertypeDefinition["nextMusicFilePath"] = sol::property(&TES3::AudioController::getNextMusicFilePath, &TES3::AudioController::setNextMusicFilePath);
			usertypeDefinition["volumeEffects"] = sol::property(&TES3::AudioController::getNormalizedEffectsVolume, &TES3::AudioController::setNormalizedEffectsVolume);
			usertypeDefinition["volumeFootsteps"] = sol::property(&TES3::AudioController::getNormalizedFootstepsVolume, &TES3::AudioController::setNormalizedFootstepsVolume);
			usertypeDefinition["volumeMaster"] = sol::property(&TES3::AudioController::getNormalizedMasterVolume, &TES3::AudioController::setNormalizedMasterVolume);
			usertypeDefinition["volumeMusic"] = sol::property(&TES3::AudioController::getMusicVolume, &TES3::AudioController::setMusicVolume);
			usertypeDefinition["volumeVoice"] = sol::property(&TES3::AudioController::getNormalizedVoiceVolume, &TES3::AudioController::setNormalizedVoiceVolume);

			// Basic function binding.
			usertypeDefinition["getMixVolume"] = &TES3::AudioController::getMixVolume;

			// Functions exposed as properties.
			usertypeDefinition["musicDuration"] = sol::readonly_property(&TES3::AudioController::getMusicDuration);
			usertypeDefinition["musicPosition"] = sol::property(&TES3::AudioController::getMusicPosition, &TES3::AudioController::setMusicPosition);
			
			// Wrapped functions.
			usertypeDefinition["changeMusicTrack"] = &TES3::AudioController::changeMusicTrack_lua;
		}
	}
}
