
#include "TES3AudioController.h"

#include "TES3WorldController.h"

#include "LuaManager.h"
#include "LuaMusicChangeTrackEvent.h"

namespace TES3 {
	const auto TES3_AudioController_changeMusicTrack = reinterpret_cast<void(__thiscall*)(AudioController*, const char*, int, float)>(0x403AC0);
	void AudioController::changeMusicTrack(const char* _filename, int crossfadeMillis, float volume) {
		std::string filename = _filename;

		if (mwse::lua::event::MusicChangeTrackEvent::getEventEnabled()) {
			const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			sol::table eventData = stateHandle.triggerEvent(new mwse::lua::event::MusicChangeTrackEvent(_filename, volume, crossfadeMillis, (int)WorldController::get()->musicSituation));
			if (eventData.valid()) {
				sol::optional<std::string> musicPath = eventData["music"];
				if (musicPath) {
					filename = std::move(musicPath.value());
				}
				crossfadeMillis = eventData.get_or("crossfade", crossfadeMillis);
				volume = eventData.get_or("volume", volume);
			}
			mwse::lua::event::MusicChangeTrackEvent::ms_Context = "invalid";

			if (eventData.get_or("block", false)) {
				return;
			}
		}

		TES3_AudioController_changeMusicTrack(this, filename.c_str(), crossfadeMillis, volume);
	}

	const auto TES3_AudioController_setMusicVolume = reinterpret_cast<void(__thiscall*)(AudioController*, float)>(0x403A10);
	void AudioController::setMusicVolume(float volume) {
		TES3_AudioController_setMusicVolume(this, volume);
	}

	const auto TES3_AudioController_pauseMusic = reinterpret_cast<void(__thiscall*)(AudioController*)>(0x403570);
	void AudioController::pauseMusic() {
		TES3_AudioController_pauseMusic(this);
	}

	const auto TES3_AudioController_unpauseMusic = reinterpret_cast<void(__thiscall*)(AudioController*)>(0x4035E0);
	void AudioController::unpauseMusic() {
		TES3_AudioController_unpauseMusic(this);
	}

	bool AudioController::getAudioFlag(AudioFlag::Flag flag) const {
		return (audioFlags & flag) != 0;
	}

	void AudioController::setAudioFlag(AudioFlag::Flag flag, bool set) {
		if (set) {
			audioFlags |= flag;
		}
		else {
			audioFlags &= ~flag;
		}
	}

	bool AudioController::getHasStaticBuffers() const {
		return getAudioFlag(AudioFlag::HasStaticBuffers);
	}

	void AudioController::setHasStaticBuffers(bool set) {
		setAudioFlag(AudioFlag::HasStaticBuffers, set);
	}

	bool AudioController::getDirectSoundInitFailed() const {
		return getAudioFlag(AudioFlag::DirectSoundInitFailed);
	}

	void AudioController::setDirectSoundInitFailed(bool set) {
		setAudioFlag(AudioFlag::DirectSoundInitFailed, set);
	}

	bool AudioController::getHasStreamingBuffers() const {
		return getAudioFlag(AudioFlag::HasStreamingBuffers);
	}

	void AudioController::setHasStreamingBuffers(bool set) {
		setAudioFlag(AudioFlag::HasStreamingBuffers, set);
	}

	bool AudioController::getMusicFlag(MusicFlag::Flag flag) const {
		return (musicFlags & flag) != 0;
	}

	void AudioController::setMusicFlag(MusicFlag::Flag flag, bool set) {
		if (set) {
			musicFlags |= flag;
		}
		else {
			musicFlags &= ~flag;
		}
	}

	bool AudioController::getIsFilterGraphValid() const {
		return getMusicFlag(MusicFlag::FilterGraphValid);
	}

	void AudioController::setIsFilterGraphValid(bool set) {
		setMusicFlag(MusicFlag::FilterGraphValid, set);
	}

	bool AudioController::getIsMusicPlaying() const {
		return getMusicFlag(MusicFlag::Playing);
	}

	void AudioController::setIsMusicPlaying(bool set) {
		setMusicFlag(MusicFlag::Playing, set);
	}

	bool AudioController::getIsMusicPaused() const {
		return getMusicFlag(MusicFlag::Paused);
	}

	void AudioController::setIsMusicPaused(bool set) {
		setMusicFlag(MusicFlag::Paused, set);
	}

	float AudioController::getMixVolume(AudioMixType mix) const {
		float volume = 0.004f * volumeMaster;
		switch (mix) {
		case AudioMixType::Master:
			break;
		case AudioMixType::Voice:
			volume *= 0.004f * volumeVoice;
			break;
		case AudioMixType::Effects:
			volume *= 0.004f * volumeEffects;
			break;
		case AudioMixType::Footsteps:
			volume *= 0.004f * volumeFootsteps;
			break;
		case AudioMixType::Music:
			// Music is not linked to master volume
			volume = volumeMusic;
			break;
		}
		return volume;
	}

	float AudioController::getNormalizedMasterVolume() const {
		return 0.004f * volumeMaster;
	}

	void AudioController::setNormalizedMasterVolume(float value) {
		volumeMaster = uint8_t(std::clamp(value, 0.0f, 1.0f) * 250);
		adjustLoopingSoundsVolume();
	}

	float AudioController::getNormalizedEffectsVolume() const {
		return 0.004f * volumeEffects;
	}

	void AudioController::setNormalizedEffectsVolume(float value) {
		volumeEffects = uint8_t(std::clamp(value, 0.0f, 1.0f) * 250);
		adjustLoopingSoundsVolume();
	}

	float AudioController::getNormalizedVoiceVolume() const {
		return 0.004f * volumeVoice;
	}

	void AudioController::setNormalizedVoiceVolume(float value) {
		volumeVoice = uint8_t(std::clamp(value, 0.0f, 1.0f) * 250);
	}

	float AudioController::getNormalizedFootstepsVolume() const {
		return 0.004f * volumeFootsteps;
	}

	void AudioController::setNormalizedFootstepsVolume(float value) {
		volumeFootsteps = uint8_t(std::clamp(value, 0.0f, 1.0f) * 250);
	}

	float AudioController::getMusicVolume() const {
		return volumeMusic;
	}

	const char* AudioController::getCurrentMusicFilePath() const {
		return currentMusicFilePath;
	}

	void AudioController::setCurrentMusicFilePath(const char* path) {
		size_t newLength = strlen(path) + 1;
		if (newLength > 260) {
			throw std::invalid_argument("Given path is longer than 260 characters.");
		}

		strncpy_s(currentMusicFilePath, path, newLength);
	}

	const char* AudioController::getNextMusicFilePath() const {
		return nextMusicFilePath;
	}

	void AudioController::setNextMusicFilePath(const char* path) {
		size_t newLength = strlen(path) + 1;
		if (newLength > 260) {
			throw std::invalid_argument("Given path is longer than 260 characters.");
		}

		strncpy_s(nextMusicFilePath, path, newLength);
	}

	double AudioController::getMusicDuration() const {
		if (musicGraph == nullptr) {
			throw std::runtime_error("Music Error: No music graph exists.");
		}

		IMediaPosition * positioning;
		if (musicGraph->QueryInterface(IID_IMediaPosition, (LPVOID*)&positioning) < 0) {
			throw std::runtime_error("Music Error: Could not query IMediaPosition interface.");
		}

		REFTIME duration;
		if (positioning->get_Duration(&duration) < 0) {
			positioning->Release();
			throw std::runtime_error("Music Error: Could not fetch media duration.");
		}

		positioning->Release();
		return duration;
	}

	double AudioController::getMusicPosition() const {
		if (musicGraph == nullptr) {
			throw std::runtime_error("Music Error: No music graph exists.");
		}

		IMediaPosition * positioning;
		if (musicGraph->QueryInterface(IID_IMediaPosition, (LPVOID*)&positioning) < 0) {
			throw std::runtime_error("Music Error: Could not query IMediaPosition interface.");
		}

		REFTIME position;
		if (positioning->get_CurrentPosition(&position) < 0) {
			positioning->Release();
			throw std::runtime_error("Music Error: Could not fetch media position.");
		}

		positioning->Release();
		return position;
	}

	void AudioController::setMusicPosition(double position) {
		if (position < 0.0) {
			position = 0.0;
		}

		if (musicGraph == nullptr) {
			throw std::runtime_error("Music Error: No music graph exists.");
		}

		IMediaPosition * positioning;
		if (musicGraph->QueryInterface(IID_IMediaPosition, (LPVOID*)&positioning) < 0) {
			throw std::runtime_error("Music Error: Could not query IMediaPosition interface.");
		}

		REFTIME duration;
		if (positioning->get_Duration(&duration) < 0) {
			positioning->Release();
			throw std::runtime_error("Music Error: Could not fetch media duration.");
		}

		if (position > duration) {
			positioning->Release();
			return;
		}

		if (positioning->put_CurrentPosition(position) < 0) {
			positioning->Release();
			throw std::runtime_error("Music Error: Failed to put current media position.");
		}

		positioning->Release();
	}

	double AudioController::getMusicFileDuration(std::string_view& path) {
		if (!std::filesystem::exists(path)) {
			throw std::invalid_argument("No music file exists at path.");
		}

		const auto& CLSID_FilgraphManager = *reinterpret_cast<GUID*>(0x74B930);
		const auto& IID_IGraphBuilder = *reinterpret_cast<GUID*>(0x74B8E0);
		IGraphBuilder* graph = nullptr;
		if (CoCreateInstance(CLSID_FilgraphManager, 0, 1u, IID_IGraphBuilder, (LPVOID*)&graph) < 0) {
			throw std::runtime_error("Could not create graph builder.");
		}

		wchar_t buffer[MAX_PATH] = {};
		MultiByteToWideChar(0, 0, path.data(), -1, buffer, MAX_PATH);
		if (graph->RenderFile(buffer, 0) < 0) {
			graph->Release();
			throw std::runtime_error("Could not play file.");
		}

		IMediaPosition* positioning = nullptr;
		if (graph->QueryInterface(IID_IMediaPosition, (LPVOID*)&positioning) < 0) {
			graph->Release();
			throw std::runtime_error("Could not query IMediaPosition interface.");
		}

		REFTIME duration;
		if (positioning->get_Duration(&duration) < 0) {
			positioning->Release();
			graph->Release();
			throw std::runtime_error("Could not fetch media position.");
		}

		positioning->Release();
		graph->Release();
		return duration;
	}

	void AudioController::changeMusicTrack_lua(const char* filename, sol::optional<int> crossfade, sol::optional<float> volume) {
		mwse::lua::event::MusicChangeTrackEvent::ms_Context = "lua";
		WorldController::get()->musicSituation = MusicSituation::Uninterruptible;
		changeMusicTrack(filename, crossfade.value_or(1000), volume.value_or(1.0f));
	}

	const auto TES3_AdjustLoopingSoundsVolume = reinterpret_cast<void(__cdecl*)()>(0x5A1E10);
	void __cdecl AudioController::adjustLoopingSoundsVolume() {
		TES3_AdjustLoopingSoundsVolume();
	}

	const auto TES3_AudioController_SetSoundBufferVolume = reinterpret_cast<void(__thiscall*)(AudioController*, SoundBuffer*, unsigned char)>(0x4029F0);
	void AudioController::setSoundBufferVolume(SoundBuffer* soundBuffer, unsigned char volume) {
		TES3_AudioController_SetSoundBufferVolume(this, soundBuffer, volume);
	}

}