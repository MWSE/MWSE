#pragma once

#include "TES3Defines.h"
#include "TES3Vectors.h"

namespace TES3 {
	enum class AudioMixType {
		Master = 0,
		Voice,
		Effects,
		Footsteps,
		Music
	};

	namespace AudioFlag {
		typedef unsigned int value_type;

		enum Flag : value_type {
			HasStaticBuffers = 0x1,
			DirectSoundInitFailed = 0x2,
			HasStreamingBuffers = 0x4,
		};

		enum FlagBit {
			HasStaticBuffersBit = 0,
			DirectSoundInitFailedBit = 1,
			HasStreamingBuffersBit = 2,
		};
	}

	namespace MusicFlag {
		typedef unsigned int value_type;

		enum Flag : value_type {
			FilterGraphValid = 0x1,
			Playing = 0x2,
			Paused = 0x4,
		};

		enum FlagBit {
			FilterGraphValidBit = 0,
			PlayingBit = 1,
			PausedBit = 2,
		};
	}

	//
	// Flag notes:
	//	unknown_0x4:
	//		- 0x1: Supports static 3D buffers?
	//		- 0x4: Supports streaming 3D buffers?
	//	unknown_0x8:
	//		- 0x1: Maybe playing state?
	//		- 0x2: Maybe pause state?
	//		- 0x4: Maybe loop state?
	//

	struct AudioController {
		bool dsound3DChanged; // 0x0
		bool dsound3DCommitted; // 0x1
		unsigned int audioFlags; // 0x4 Flags.
		unsigned int musicFlags; // 0x8 Flags.
		IDirectSound * directSound; // 0xC
		IDirectSoundBuffer * primaryBuffer; // 0x10
		IDirectSound3DListener * primary3DListener; // 0x14
		unsigned char soundQuality3D; // 0x18
		DSCAPS capabilities; // 0x1C
		char nextMusicFilePath[260]; // 0x7C
		char currentMusicFilePath[260]; // 0x180
		int timestampBeginFade; // 0x284
		int timestampNextTrackStart; // 0x288
		float volumeNextTrack; // 0x28C
		unsigned char volumeMaster; // 0x290
		float volumeMusic; // 0x294
		unsigned char volumeEffects; // 0x298
		unsigned char volumeVoice; // 0x299
		unsigned char volumeFootsteps; // 0x29A
		LPLONG musicPan; // 0x29C
		IGraphBuilder * musicGraph; // 0x2A0
		IBasicAudio * musicAudio; // 0x2A4
		bool disableAudio; // 0x2A8
		Vector3 listenerPosition; // 0x2AC
		Vector3 unknown_0x2B8; // Orientation.
		Vector3 unknown_0x2C4; // Orientation.
		float yawAxis; // 0x2D0 // In radians.
		float pitchAxisApproximated; // 0x2D4 // In radians.

		//
		// This-call functions.
		//

		void changeMusicTrack(const char* filename, int crossfadeMillis, float volume);

		void setMusicVolume(float volume);

		void pauseMusic();
		void unpauseMusic();

		//
		// Custom functions.
		//

		bool getAudioFlag(AudioFlag::Flag flag) const;
		void setAudioFlag(AudioFlag::Flag flag, bool set);
		bool getHasStaticBuffers() const;
		void setHasStaticBuffers(bool set);
		bool getDirectSoundInitFailed() const;
		void setDirectSoundInitFailed(bool set);
		bool getHasStreamingBuffers() const;
		void setHasStreamingBuffers(bool set);

		bool getMusicFlag(MusicFlag::Flag flag) const;
		void setMusicFlag(MusicFlag::Flag flag, bool set);
		bool getIsFilterGraphValid() const;
		void setIsFilterGraphValid(bool set);
		bool getIsMusicPlaying() const;
		void setIsMusicPlaying(bool set);
		bool getIsMusicPaused() const;
		void setIsMusicPaused(bool set);

		const char* getCurrentMusicFilePath() const;
		void setCurrentMusicFilePath(const char* path);

		const char* getNextMusicFilePath() const;
		void setNextMusicFilePath(const char* path);

		float getMixVolume(AudioMixType mixType) const;

		float getMusicVolume() const;

		double getMusicDuration() const;
		double getMusicPosition() const;
		void setMusicPosition(double position);

		double getMusicFileDuration(std::string_view& path);

		void changeMusicTrack_lua(const char* filename, sol::optional<int> crossfade, sol::optional<float> volume);

		static void __cdecl adjustLoopingSoundsVolume();
		void setSoundBufferVolume(SoundBuffer* soundBuffer, unsigned char volume);

		//
		// Wrapper functions to expose volumes in a consistent format.
		//

		float getNormalizedMasterVolume() const;
		void setNormalizedMasterVolume(float value);

		float getNormalizedEffectsVolume() const;
		void setNormalizedEffectsVolume(float value);

		float getNormalizedVoiceVolume() const;
		void setNormalizedVoiceVolume(float value);

		float getNormalizedFootstepsVolume() const;
		void setNormalizedFootstepsVolume(float value);

	};
	static_assert(sizeof(AudioController) == 0x2D8, "TES3::AudioController failed size validation");

	// Make sure we're looking at the same size for DirectSound structures.
	static_assert(sizeof(DSBUFFERDESC) == 0x24, "DirectSound DSBUFFERDESC failed size validation");
	static_assert(sizeof(DSCAPS) == 0x60, "DirectSound DSCAPS failed size validation");
}