#include "PatchAudio.h"

#include "MemoryUtil.h"
#include "MWSEConfig.h"

#include "TES3AudioController.h"
#include "TES3DataHandler.h"
#include "TES3Sound.h"
#include "TES3WorldController.h"

namespace mwse::patch::audio {
	//
	// Patch: When adjusting effects mix volume, update looping audio volume correctly.
	//

	void __fastcall PatchSetLoopingSoundBufferVolume(TES3::AudioController* audio, DWORD unused, TES3::SoundEvent* soundEvent, unsigned char volume) {
		if (soundEvent->sound) {
			volume = (unsigned char)(float(volume) * float(soundEvent->sound->volume) / 255.0f);
		}
		audio->setSoundBufferVolume(soundEvent->soundBuffer, volume);
	}

	//
	// Patch: Ensure exclusive sound buffer for water audio.
	//

	static TES3::Sound* waterSound = nullptr;
	static TES3::SoundBuffer* waterSoundBuffer = nullptr;

	static void PatchWaterSoundInstantiate(TES3::Sound* sound) {
		if (waterSound == sound && waterSoundBuffer) {
			return;
		}

		if (waterSoundBuffer) {
			delete waterSoundBuffer;
			waterSoundBuffer = nullptr;
		}

		waterSound = sound;
		waterSoundBuffer = sound->createSoundBuffer(false);
	}

	static void __fastcall PatchWaterSoundSet3DParams(TES3::Sound* sound, DWORD, bool isPointSource) {
		const auto ac = TES3::WorldController::get()->audioController;
		PatchWaterSoundInstantiate(sound);
		ac->setSoundBuffer3DParams(waterSoundBuffer, isPointSource);
	}

	static bool __fastcall PatchWaterSoundIsPlaying(TES3::Sound* sound) {
		const auto ac = TES3::WorldController::get()->audioController;
		PatchWaterSoundInstantiate(sound);
		return ac->getSoundBufferIsPlaying(waterSoundBuffer);
	}

	static void __fastcall PatchWaterSoundPlay(TES3::Sound* sound, DWORD, DWORD flags, uint8_t volume, float pitch, bool isNot3D) {
		const auto ac = TES3::WorldController::get()->audioController;
		PatchWaterSoundInstantiate(sound);
		ac->playSoundBuffer(waterSoundBuffer, flags, volume, pitch, isNot3D);
	}

	static void __fastcall PatchWaterSoundStop(TES3::Sound* sound) {
		const auto ac = TES3::WorldController::get()->audioController;
		if (!ac->getSoundBufferIsPlaying(waterSoundBuffer)) {
			return;
		}
		PatchWaterSoundInstantiate(sound);
		ac->stopSoundBuffer(waterSoundBuffer);
	}

	static void __fastcall PatchWaterSoundReset(TES3::Sound* sound, DWORD, int) {
		if (waterSoundBuffer) {
			delete waterSoundBuffer;
			waterSoundBuffer = nullptr;
		}
		waterSound = nullptr;
	}

	static void __fastcall PatchWaterSoundSetFrequency(TES3::AudioController* ac, DWORD, TES3::SoundBuffer* _, float frequency) {
		ac->setSoundBufferFrequency(waterSoundBuffer, frequency);
	}

	static void __fastcall PatchWaterSoundSetLoopVolume(TES3::SoundBuffer* _, DWORD, int volume) {
		if (!waterSoundBuffer) {
			return;
		}

		const auto newVolume = static_cast<uint8_t>(waterSound->volume * float(volume) / 250.0f);
		const auto ac = TES3::WorldController::get()->audioController;
		ac->setSoundBufferVolume(waterSoundBuffer, newVolume);
	}

	static void __stdcall PatchWaterSoundSetLoopVolumeMCP(TES3::SoundBuffer* _, int volume) {
		if (!waterSoundBuffer || !waterSoundBuffer->lpSoundBuffer) {
			return;
		}

		volume = std::clamp(volume, -10000, 0);
		waterSoundBuffer->lpSoundBuffer->SetVolume(volume);
	}

	void install() {
		using se::memory::genCallEnforced;
		using se::memory::genNOPUnprotected;
		using se::memory::writeValueEnforced;

		// Patch: When adjusting effects mix volume, update looping audio volume correctly.
		writeValueEnforced<BYTE>(0x5A1F24, 0x52, 0x56);
		genCallEnforced(0x5A1F25, 0x4029F0, reinterpret_cast<DWORD>(PatchSetLoopingSoundBufferVolume));
		writeValueEnforced<BYTE>(0x5A1FC5, 0x52, 0x56);
		genCallEnforced(0x5A1FC6, 0x4029F0, reinterpret_cast<DWORD>(PatchSetLoopingSoundBufferVolume));
		
		// Fix Sound::changeVolume scaling constant to be 1/255.
		writeValueEnforced<DWORD>(0x510C6C, 0x74A9E4, 0x746910);

		// Patch: Ensure exclusive sound buffer for water audio.
		genCallEnforced(0x48A861, 0x5107E0, reinterpret_cast<DWORD>(PatchWaterSoundSet3DParams));
		genCallEnforced(0x48A86C, 0x510B80, reinterpret_cast<DWORD>(PatchWaterSoundIsPlaying));
		genCallEnforced(0x48A89F, 0x510B80, reinterpret_cast<DWORD>(PatchWaterSoundIsPlaying));
		genCallEnforced(0x48A8EE, 0x510B80, reinterpret_cast<DWORD>(PatchWaterSoundIsPlaying));
		genCallEnforced(0x48A8D0, 0x510A40, reinterpret_cast<DWORD>(PatchWaterSoundPlay));
		genCallEnforced(0x48A892, 0x510BC0, reinterpret_cast<DWORD>(PatchWaterSoundStop));
		genCallEnforced(0x48A9C8, 0x510BC0, reinterpret_cast<DWORD>(PatchWaterSoundStop));
		genCallEnforced(0x48C7A9, 0x5109F0, reinterpret_cast<DWORD>(PatchWaterSoundReset));
		genCallEnforced(0x745F72, 0x5109F0, reinterpret_cast<DWORD>(PatchWaterSoundReset)); // MCP-183
		genNOPUnprotected(0x48A8DB, 0x48A8EE - 0x48A8DB);
		genCallEnforced(0x48A930, 0x402A60, reinterpret_cast<DWORD>(PatchWaterSoundSetFrequency));
		genCallEnforced(0x48A97F, 0x510C30, reinterpret_cast<DWORD>(PatchWaterSoundSetLoopVolume));
		genCallEnforced(0x48A989, 0x402B42, reinterpret_cast<DWORD>(PatchWaterSoundSetLoopVolumeMCP));

		// Patch: Guarded 3D audio listener access.
		auto AudioController_setListenerDistanceFactor = &TES3::AudioController::setListenerDistanceFactor;
		genCallEnforced(0x40E5DB, 0x402F40, *reinterpret_cast<DWORD*>(&AudioController_setListenerDistanceFactor));
		genCallEnforced(0x40E602, 0x402F40, *reinterpret_cast<DWORD*>(&AudioController_setListenerDistanceFactor));
		genCallEnforced(0x40E74E, 0x402F40, *reinterpret_cast<DWORD*>(&AudioController_setListenerDistanceFactor));
		genCallEnforced(0x40E775, 0x402F40, *reinterpret_cast<DWORD*>(&AudioController_setListenerDistanceFactor));
		auto AudioController_setListenerDopplerFactor = &TES3::AudioController::setListenerDopplerFactor;
		genCallEnforced(0x40E5E8, 0x402F60, *reinterpret_cast<DWORD*>(&AudioController_setListenerDopplerFactor));
		genCallEnforced(0x40E75B, 0x402F60, *reinterpret_cast<DWORD*>(&AudioController_setListenerDopplerFactor));
		genCallEnforced(0x489851, 0x402F60, *reinterpret_cast<DWORD*>(&AudioController_setListenerDopplerFactor));
		auto AudioController_setListenerRolloffFactor = &TES3::AudioController::setListenerRolloffFactor;
		genCallEnforced(0x40E5F5, 0x402F80, *reinterpret_cast<DWORD*>(&AudioController_setListenerRolloffFactor));
		genCallEnforced(0x40E768, 0x402F80, *reinterpret_cast<DWORD*>(&AudioController_setListenerRolloffFactor));
		genCallEnforced(0x489864, 0x402F80, *reinterpret_cast<DWORD*>(&AudioController_setListenerRolloffFactor));
		auto AudioController_setListenerVelocity = &TES3::AudioController::setListenerVelocity;
		genCallEnforced(0x48B112, 0x403220, *reinterpret_cast<DWORD*>(&AudioController_setListenerVelocity));
		auto AudioController_commitDeferredSettings = &TES3::AudioController::commitDeferredSettings;
		genCallEnforced(0x40F7ED, 0x403250, *reinterpret_cast<DWORD*>(&AudioController_commitDeferredSettings));
		genCallEnforced(0x48C6C3, 0x403250, *reinterpret_cast<DWORD*>(&AudioController_commitDeferredSettings));
		genCallEnforced(0x510B57, 0x403250, *reinterpret_cast<DWORD*>(&AudioController_commitDeferredSettings));

	}

	void installPostLua() {
		// Patch: Allow global audio.
		if (Configuration::UseGlobalAudio) {
			constexpr auto DS_FLAGS_DEFAULT = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;
			constexpr auto DS_FLAGS_3D = DS_FLAGS_DEFAULT | DSBCAPS_CTRL3D | DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_MUTE3DATMAXDISTANCE;
			se::memory::writeAddFlagEnforced(0x401FEA + 0x3, DS_FLAGS_DEFAULT | DSBCAPS_CTRLPAN, DSBCAPS_GLOBALFOCUS);
			se::memory::writeAddFlagEnforced(0x401FE1 + 0x3, DS_FLAGS_3D, DSBCAPS_GLOBALFOCUS);
			se::memory::writeAddFlagEnforced(0x401FF7 + 0x3, DS_FLAGS_DEFAULT, DSBCAPS_GLOBALFOCUS);
			se::memory::writeAddFlagEnforced(0x40240E + 0x3, DS_FLAGS_DEFAULT | DSBCAPS_CTRLPAN, DSBCAPS_GLOBALFOCUS);
			se::memory::writeAddFlagEnforced(0x402405 + 0x3, DS_FLAGS_3D, DSBCAPS_GLOBALFOCUS);
		}
	}
}
