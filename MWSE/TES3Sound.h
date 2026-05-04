#pragma once

#include "TES3Defines.h"

#include "TES3Object.h"

namespace TES3 {
	namespace SoundPlayFlags {
		typedef int value_type;

		enum Flag {
			Loop = DSBPLAY_LOOPING,
		};
	}

	struct SoundBuffer {
		IDirectSoundBuffer * lpSoundBuffer; // 0x0
		IDirectSound3DBuffer * lpSound3DBuffer; // 0x4
		// fileHeader is the in-place WAVEFORMATEX that bufferDescription.lpwfxFormat
		// points at (set by LoadSoundFile). Bytes 0..15 hold WAVEFORMAT + wBitsPerSample,
		// bytes 16..17 hold cbSize (always 0 for PCM, which is all the engine produces).
		char fileHeader[18]; // 0x8
		// 2 bytes implicit alignment padding here (0x1A..0x1B) to align the
		// DSBUFFERDESC's first DWORD to 4 bytes.
		DSBUFFERDESC bufferDescription; // 0x1C
		bool isVoiceover; // 0x40
		short* rawAudio; // 0x44
		int dBReduction; // 0x48 - distance attenuation in DSound dB units, set by SetSoundBufferPosition's 2D path and subtracted from volume.
		unsigned char volume; // 0x4C
		int minDistance; // 0x50
		int maxDistance; // 0x58

		static constexpr auto OBJECT_TYPE = ObjectType::Sound;

		// The engine has no separate ctor/dtor for SoundBuffer; LoadSoundFile
		// and ReleaseSoundBuffer each inline their own setup/teardown. These
		// match that behavior so MWSE can `new` / `delete` SoundBuffer objects
		// (via the engine heap, see operator new/delete) without heap mismatch
		// when those instances cross the engine boundary.
		SoundBuffer();
		~SoundBuffer();

		// Class-specific operator new/delete that route through the engine
		// heap (0x727692 / 0x727530). Required because instances allocated by
		// MWSE may be released by engine code (and vice versa).
		static void* operator new(size_t size);
		static void operator delete(void* p);
	};
	static_assert(sizeof(SoundBuffer) == 0x58, "TES3::SoundBuffer failed size validation");
	static_assert(sizeof(DSBUFFERDESC) == 0x24, "DSBUFFERDESC failed size validation");

	struct Sound : BaseObject {
		char field_10;
		char id[32];
		char filename[32];
		unsigned char volume;
		unsigned char minDistance;
		unsigned char maxDistance;
		SoundBuffer* soundBuffer;

		Sound();
		~Sound();

		Sound* ctor();
		void dtor();

		//
		// Virtual table overrides.
		//

		char * getObjectID();

		//
		// Other related this-call functions.
		//

		bool play(int playbackFlags = 0, unsigned char volume = 250, float pitch = 1.0f, bool isNot3D = true);
		bool playRaw(int playbackFlags = 0, unsigned char volume = 250, float pitch = 1.0f, bool isNot3D = true);
		void stop();
		void loadBuffer(bool isPointSound);
		void release();
		void setVolumeRaw(unsigned char volume);

		bool isPlaying() const;
		bool isLooping() const;

		//
		// Custom functions.
		//

		void setObjectID(const char* id);

		const char* getFilename() const;
		void setFilename(const char* filename);

		unsigned char getMinDistance() const;
		void setMinDistance(unsigned char value);
		void setMinDistance_lua(double value);

		unsigned char getMaxDistance() const;
		void setMaxDistance(unsigned char value);
		void setMaxDistance_lua(double value);

		float getVolume();
		void setVolume(float volume);
		void adjustPlayingSoundVolume(unsigned char volume);

		std::string toJson() const;
		bool play_lua(sol::optional<sol::table> params);

	};
	static_assert(sizeof(Sound) == 0x58, "TES3::Sound failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::Sound)
