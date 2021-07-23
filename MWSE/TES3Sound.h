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
		char fileHeader[16];
		short unknown_0x18;
		DSBUFFERDESC bufferDescription; // 0x1C
		bool isVoiceover; // 0x40
		short* rawAudio; // 0x44
		int unknown_0x48; // Volume related
		unsigned char volume; // 0x4C
		int minDistance; // 0x50
		int maxDistance; // 0x58
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

		std::string toJson() const;
		bool play_lua(sol::optional<sol::table> params);

	};
	static_assert(sizeof(Sound) == 0x58, "TES3::Sound failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::Sound)
