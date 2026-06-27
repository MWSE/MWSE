// AudioController::loadSoundFile, isolated here so the dr_libs decoder
// implementations don't bloat the core audio files.

#include "TES3AudioController.h"
#include "TES3Sound.h"

#include "MWSEConfig.h"

#pragma comment(lib, "dxguid.lib") // IID_IDirectSound3DBuffer

// Vendored single-header decoders (deps/dr_libs submodule); warnings suppressed.
#pragma warning(push, 0)
#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h"
#define DR_MP3_IMPLEMENTATION
#include "dr_mp3.h"
#define DR_FLAC_IMPLEMENTATION
#include "dr_flac.h"
#pragma warning(pop)

namespace TES3 {

	// Raw engine loader; the fallback target, called directly so loadSoundFile
	// never recurses into itself.
	static const auto engineLoadSoundFile = reinterpret_cast<SoundBuffer*(__thiscall*)(AudioController*, const char*, bool)>(0x401DB0);

	struct DecodedPcm {
		std::vector<drwav_int16> samples; // interleaved
		unsigned int channels = 0;
		unsigned int sampleRate = 0;
		bool ok() const { return !samples.empty() && channels > 0 && sampleRate > 0; }
	};

	static bool endsWithCI(const char* s, const char* suffix) {
		if (!s) return false;
		const size_t ls = std::strlen(s), lf = std::strlen(suffix);
		if (lf > ls) return false;
		return _strnicmp(s + (ls - lf), suffix, lf) == 0;
	}

	// Matches the engine's own voiceover probe at 0x48C5F3.
	static bool isVoiceoverPath(const char* filename) {
		return std::strstr(filename, "vo\\") != nullptr
			|| std::strstr(filename, "Vo\\") != nullptr
			|| std::strstr(filename, "vO\\") != nullptr
			|| std::strstr(filename, "VO\\") != nullptr;
	}

	static void downmixToMono(std::vector<drwav_int16>& samples, unsigned int channels) {
		if (channels <= 1) return;
		const size_t frames = samples.size() / channels;
		for (size_t i = 0; i < frames; ++i) {
			int acc = 0;
			const drwav_int16* frame = &samples[i * channels];
			for (unsigned int c = 0; c < channels; ++c) acc += frame[c];
			samples[i] = static_cast<drwav_int16>(acc / static_cast<int>(channels));
		}
		samples.resize(frames);
	}

	static bool decodeMp3(const char* filename, DecodedPcm& out) {
		drmp3_config config = {};
		drmp3_uint64 frames = 0;
		drmp3_int16* data = drmp3_open_file_and_read_pcm_frames_s16(filename, &config, &frames, nullptr);
		if (!data) return false;
		out.channels = config.channels;
		out.sampleRate = config.sampleRate;
		out.samples.assign(data, data + frames * config.channels);
		drmp3_free(data, nullptr);
		return out.ok();
	}

	static bool decodeFlac(const char* filename, DecodedPcm& out) {
		unsigned int channels = 0, sampleRate = 0;
		drflac_uint64 frames = 0;
		drflac_int16* data = drflac_open_file_and_read_pcm_frames_s16(filename, &channels, &sampleRate, &frames, nullptr);
		if (!data) return false;
		out.channels = channels;
		out.sampleRate = sampleRate;
		out.samples.assign(data, data + frames * channels);
		drflac_free(data, nullptr);
		return out.ok();
	}

	// Mirrors LoadSoundFile's WAV path (flags/3D/volume) but owns its memory, so
	// the engine's CreateSoundBuffer-failure handle leak can't occur.
	static SoundBuffer* buildSoundBuffer(AudioController* audio, const DecodedPcm& pcm, unsigned int channels, bool isPointSource) {
		if (!audio || !audio->directSound) return nullptr;
		const DWORD byteCount = static_cast<DWORD>(pcm.samples.size() * sizeof(drwav_int16));
		if (byteCount == 0) return nullptr;

		auto* soundBuffer = new SoundBuffer(); // engine heap, zero-initialized

		auto* format = reinterpret_cast<WAVEFORMATEX*>(soundBuffer->fileHeader);
		format->wFormatTag = WAVE_FORMAT_PCM;
		format->nChannels = static_cast<WORD>(channels);
		format->nSamplesPerSec = pcm.sampleRate;
		format->wBitsPerSample = 16;
		format->nBlockAlign = static_cast<WORD>(channels * sizeof(drwav_int16));
		format->nAvgBytesPerSec = pcm.sampleRate * format->nBlockAlign;
		format->cbSize = 0;

		// Flags match the engine (PatchUtil's DS_FLAGS_*).
		constexpr DWORD DS_FLAGS_DEFAULT = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;
		constexpr DWORD DS_FLAGS_3D = DS_FLAGS_DEFAULT | DSBCAPS_CTRL3D | DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_MUTE3DATMAXDISTANCE;
		bool use3D = false;
		DWORD flags;
		if (isPointSource) {
			if (audio->soundQuality3D > 1) {
				flags = DS_FLAGS_3D;
				use3D = true;
			}
			else {
				flags = DS_FLAGS_DEFAULT | DSBCAPS_CTRLPAN;
			}
		}
		else {
			flags = DS_FLAGS_DEFAULT;
		}
		if (mwse::Configuration::UseGlobalAudio) flags |= DSBCAPS_GLOBALFOCUS;
		if (audio->getHasStreamingBuffers()) flags |= DSBCAPS_LOCDEFER;

		auto& description = soundBuffer->bufferDescription;
		description.dwSize = sizeof(DSBUFFERDESC);
		description.dwFlags = flags;
		description.dwBufferBytes = byteCount;
		description.lpwfxFormat = format;

		soundBuffer->isVoiceover = false;
		soundBuffer->rawAudio = nullptr;

		IDirectSoundBuffer* directSoundBuffer = nullptr;
		if (FAILED(audio->directSound->CreateSoundBuffer(&description, &directSoundBuffer, nullptr)) || !directSoundBuffer) {
			delete soundBuffer; // null COM/rawAudio fields -> no-op dtor
			return nullptr;
		}

		void* block1 = nullptr;
		void* block2 = nullptr;
		DWORD length1 = 0;
		DWORD length2 = 0;
		if (SUCCEEDED(directSoundBuffer->Lock(0, 0, &block1, &length1, &block2, &length2, DSBLOCK_ENTIREBUFFER))) {
			if (block1 && length1) std::memcpy(block1, pcm.samples.data(), length1);
			if (block2 && length2) std::memcpy(block2, reinterpret_cast<const char*>(pcm.samples.data()) + length1, length2);
			directSoundBuffer->Unlock(block1, length1, block2, length2);
		}

		soundBuffer->lpSoundBuffer = directSoundBuffer;
		if (use3D) {
			IDirectSound3DBuffer* buffer3D = nullptr;
			if (SUCCEEDED(directSoundBuffer->QueryInterface(IID_IDirectSound3DBuffer, reinterpret_cast<void**>(&buffer3D)))) {
				soundBuffer->lpSound3DBuffer = buffer3D;
			}
		}
		directSoundBuffer->SetVolume(0);
		return soundBuffer;
	}

	static SoundBuffer* finishBuild(AudioController* audio, DecodedPcm& pcm, bool isPointSource) {
		if (!pcm.ok()) return nullptr;
		// 3D point sources must be mono (DirectSound rejects stereo CTRL3D).
		if ((isPointSource && pcm.channels > 1) || pcm.channels > 2) {
			downmixToMono(pcm.samples, pcm.channels);
			pcm.channels = 1;
		}
		return buildSoundBuffer(audio, pcm, pcm.channels, isPointSource);
	}

	// Decodes WAV variants DirectSound can't take (24/32-bit, float, EXTENSIBLE)
	// and non-voiceover MP3/FLAC (no ACM codec linked), and downmixes stereo 3D
	// point sources to mono. Engine-compatible, missing, and voiceover files fall
	// through to the vanilla loader; a recognized-but-unplayable format returns
	// null (silent) instead of the engine's leaking failure path.
	SoundBuffer* AudioController::loadSoundFile(const char* filename, bool isPointSource) {
		if (!filename) return engineLoadSoundFile(this, filename, isPointSource);
		if (isVoiceoverPath(filename)) return engineLoadSoundFile(this, filename, isPointSource);
		if (disableAudio || !isDirectSoundAvailable()) return engineLoadSoundFile(this, filename, isPointSource);

		if (endsWithCI(filename, ".mp3")) {
			DecodedPcm pcm;
			if (!decodeMp3(filename, pcm)) return nullptr;
			return finishBuild(this, pcm, isPointSource);
		}
		if (endsWithCI(filename, ".flac")) {
			DecodedPcm pcm;
			if (!decodeFlac(filename, pcm)) return nullptr;
			return finishBuild(this, pcm, isPointSource);
		}

		// WAV: pass engine-compatible files through unchanged; only intercept
		// formats DirectSound can't take or stereo-on-3D.
		drwav wav;
		if (!drwav_init_file(&wav, filename, nullptr)) {
			return engineLoadSoundFile(this, filename, isPointSource);
		}

		const bool needsDownmix = isPointSource && wav.channels > 1;
		const bool engineCompatible = (wav.translatedFormatTag == DR_WAVE_FORMAT_PCM && wav.bitsPerSample == 16 && !needsDownmix);
		if (engineCompatible) {
			drwav_uninit(&wav);
			return engineLoadSoundFile(this, filename, isPointSource);
		}

		DecodedPcm pcm;
		pcm.channels = wav.channels;
		pcm.sampleRate = wav.sampleRate;
		pcm.samples.resize(static_cast<size_t>(wav.totalPCMFrameCount) * wav.channels);
		const drwav_uint64 framesRead = drwav_read_pcm_frames_s16(&wav, wav.totalPCMFrameCount, pcm.samples.data());
		pcm.samples.resize(static_cast<size_t>(framesRead) * wav.channels);
		drwav_uninit(&wav);

		return finishBuild(this, pcm, isPointSource);
	}

}
