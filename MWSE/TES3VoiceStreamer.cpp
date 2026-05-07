#include "TES3VoiceStreamer.h"

#include "Log.h"
#include "MemoryUtil.h"
#include "WindowsUtil.h"

#include "TES3AudioController.h"
#include "TES3CriticalSection.h"
#include "TES3DataHandler.h"
#include "TES3GameSetting.h"
#include "TES3Reference.h"
#include "TES3Sound.h"
#include "TES3WorldController.h"

namespace mwse::patch::voice {

	namespace {

		// A "pending" SoundBuffer is one whose decode hasn't finished. Encoded by
		// bufferDescription.dwSize == 0 (the real value, set by LoadSoundFile, is
		// sizeof(DSBUFFERDESC) == 36 — never 0 for a real buffer).
		//
		// All stub-awareness in MWSE lives in this file. The six leaf accessor
		// replacements below check isPending() and short-circuit; everything else
		// in the engine treats stubs as ordinary SoundBuffers.
		constexpr DWORD STUB_PENDING_SENTINEL = 0;
		constexpr DWORD REAL_BUFFERDESC_SIZE = sizeof(DSBUFFERDESC); // 36

		bool isPending(const TES3::SoundBuffer* sb) {
			return sb && sb->bufferDescription.dwSize == STUB_PENDING_SENTINEL;
		}

		bool isVoiceoverPath(const char* filename) {
			if (!filename) return false;
			// Match the engine's own case-insensitive probe at 0x48C5F3.
			return std::strstr(filename, "vo\\") != nullptr
				|| std::strstr(filename, "Vo\\") != nullptr
				|| std::strstr(filename, "vO\\") != nullptr
				|| std::strstr(filename, "VO\\") != nullptr;
		}

		// Allocate a stub via SoundBuffer's class-specific operator new (engine
		// heap), then ctor-zero. We then set the field invariants we rely on
		// across the leaf accessors:
		// - dwFlags must NOT have DSBCAPS_CTRL3D set, so SetSoundBufferPosition
		//   takes the 2D branch which is null-safe via our replacement.
		// - lpwfxFormat must be non-null and point at zero-initialized memory
		//   so SetSoundBufferFrequency reads nSamplesPerSec==0 and exits early.
		// dwSize stays 0 (the STUB_PENDING_SENTINEL value) from the ctor's memset.
		TES3::SoundBuffer* allocateStub() {
			auto* sb = new TES3::SoundBuffer();
			sb->isVoiceover = true;
			sb->bufferDescription.lpwfxFormat = reinterpret_cast<WAVEFORMATEX*>(sb->fileHeader);
			return sb;
		}

		// A stub may be referenced by both the SoundEvent (returned to the game)
		// and the worker (still decoding). Whichever decrements last frees it.
		// This is the only reason we hook ReleaseSoundBuffer at all — without it,
		// the engine could free a stub while the worker is mid-decode.
		std::mutex g_stubRefMutex;
		std::unordered_map<TES3::SoundBuffer*, std::atomic<int>*> g_stubRefcounts;

		std::atomic<int>* acquireStubRef(TES3::SoundBuffer* stub) {
			std::lock_guard lk(g_stubRefMutex);
			auto* rc = new std::atomic<int>(2); // SoundEvent + worker
			g_stubRefcounts[stub] = rc;
			return rc;
		}

		void decRef(TES3::SoundBuffer* stub, std::atomic<int>* rc) {
			if (rc->fetch_sub(1) == 1) {
				{
					std::lock_guard lk(g_stubRefMutex);
					g_stubRefcounts.erase(stub);
				}
				delete rc;
				delete stub;
			}
		}

		struct DecodeTask {
			std::string path;
			TES3::AudioController* audio;
			bool isPointSource;
			TES3::SoundBuffer* stub;
			std::atomic<int>* refcount;
		};

		std::mutex g_queueMutex;
		std::condition_variable g_queueCv;
		std::deque<DecodeTask> g_queue;
		std::thread g_worker;
		std::atomic<bool> g_running{ false };

		// std::default_delete<TES3::SoundBuffer> calls plain `delete sb`, which
		// invokes SoundBuffer's dtor (Release COMs + free rawAudio) and then its
		// class-specific operator delete (engine heap free).
		using EngineSoundBufferPtr = std::unique_ptr<TES3::SoundBuffer>;

		// Move decoded contents into the stub atomically; the dwSize=0→36
		// transition is the publish edge that flips the leaf accessors out of
		// stub mode.
		void publishDecodedBufferLocked(TES3::AudioController* audio, TES3::SoundBuffer* stub, TES3::SoundBuffer* decoded) {
			stub->lpSoundBuffer = decoded->lpSoundBuffer;
			stub->lpSound3DBuffer = decoded->lpSound3DBuffer;
			stub->rawAudio = decoded->rawAudio;
			std::memcpy(stub->fileHeader, decoded->fileHeader, sizeof(stub->fileHeader));
			stub->bufferDescription = decoded->bufferDescription; // dwSize becomes 36 here

			// lpwfxFormat is an INTERNAL pointer (decoded->fileHeader, inside
			// decoded's own struct that we're about to free). Redirect to
			// stub->fileHeader, where we just memcpy'd the WAVEFORMATEX bytes.
			// Without this, SetSoundBufferFrequency (or any later wfx read) is a
			// use-after-free.
			stub->bufferDescription.lpwfxFormat = reinterpret_cast<WAVEFORMATEX*>(stub->fileHeader);

			stub->isVoiceover = true;

			// Do NOT touch stub->volume here. addTempSound already called
			// SetSoundBufferVolume(stub, vol) while the stub was pending, which
			// writes the volume field even though it skips the DSound call
			// (lpSoundBuffer was null at the time). decoded->volume is
			// uninitialized from operator new — clobbering with it would set
			// volume to 0 and produce a -10000 dB attenuation (silent) on the
			// next updateSounds tick.

			// Null out decoded's resource fields — DSound objects + rawAudio now
			// belong to the stub. The caller's EngineSoundBufferPtr will then
			// `delete` the husk; with all resource fields null, the SoundBuffer
			// dtor is a no-op and only the wrapper struct gets freed.
			decoded->lpSoundBuffer = nullptr;
			decoded->lpSound3DBuffer = nullptr;
			decoded->rawAudio = nullptr;

			// Apply Volume and Min/Max distance to the now-real buffer.
			// addTempSound called both setters on the pending stub; the wrappers
			// wrote the fields but skipped the COM calls (lpSoundBuffer /
			// lpSound3DBuffer were null at the time). Without re-applying:
			//   - 3D mode: DSound uses defaults DS3D_DEFAULTMINDISTANCE=1m /
			//     DS3D_DEFAULTMAXDISTANCE=1e9m, attenuating to silence at any
			//     sane game distance. updateSounds only calls SetPosition each
			//     frame — never re-applies min/max — so attenuation stays buried.
			//   - 2D mode: lpSoundBuffer is at 0 dB max from loadSoundFile's
			//     finalizer; setSoundBufferVolume here corrects the brief loud
			//     frame before updateSounds re-applies via SetSoundBufferPosition's
			//     2D path.
			//
			// stub->minDistance/maxDistance are typed int in MWSE's struct but
			// the engine stores float bit patterns there (see decompilation of
			// SetSoundBufferMinMaxDistance's else branch). Re-cast to float.
			audio->setSoundBufferVolume(stub, stub->volume);

			float minD = 0.0f, maxD = 0.0f;
			std::memcpy(&minD, &stub->minDistance, sizeof(float));
			std::memcpy(&maxD, &stub->maxDistance, sizeof(float));
			audio->setSoundBufferMinMaxDistance(stub, minD, maxD);

			// addTempSound's PlaySoundBuffer earlier was a no-op (lpSoundBuffer
			// null); now that the buffer is real, kick off playback. updateSounds
			// will reposition the stub on its next tick from the SoundEvent's
			// reference.
			audio->playSoundBuffer(stub, 0);
		}

		void workerLoop() {
			while (g_running.load(std::memory_order_acquire)) {
				DecodeTask task;
				{
					std::unique_lock lk(g_queueMutex);
					g_queueCv.wait(lk, [] { return !g_queue.empty() || !g_running.load(); });
					if (!g_running.load()) break;
					task = std::move(g_queue.front());
					g_queue.pop_front();
				}

				// Decode off the main thread, no engine lock held. Wrap in a
				// unique_ptr so any early return correctly releases the
				// engine-allocated buffer + its COM resources via the deleter.
				EngineSoundBufferPtr decoded(task.audio->loadSoundFile(task.path.c_str(), task.isPointSource));

				// Re-enter the engine to publish. We acquire the audio-lists CS
				// that addTempSound / updateSounds / sayDialogueVoice all share
				// so the field write below can't race with the main thread
				// iterating tempSoundEvents.
				//
				// Verified offset: addTempSound at 0x48C326 reads
				// `mov ecx, [ebp+0xB534]` then calls NiCriticalSection::Lock,
				// i.e. dh->criticalSectionAudioEvents. We go through MWSE's
				// enter()/leave() wrappers (via CriticalSection::Lock) so we
				// hit the same NiCriticalSection::Lock path the engine uses.
				{
					TES3::CriticalSection::Lock lk(*TES3::DataHandler::get()->criticalSectionAudioEvents, "MWSE:VoiceStreamer");

					// Stub was released while we were decoding (cell change, killSounds).
					if (task.refcount->load() <= 1) {
						decRef(task.stub, task.refcount);
						continue; // unique_ptr drops decoded; AudioListsLock leaves
					}

					// Decode failed (file missing, bad format, OOM). Flip dwSize so
					// updateSounds reaps the SoundEvent — its status check will now
					// hit the real-buffer path and find lpSoundBuffer null. The leaf
					// replacements all guard against that.
					if (!decoded) {
						task.stub->bufferDescription.dwSize = REAL_BUFFERDESC_SIZE;
						decRef(task.stub, task.refcount);
						continue;
					}

					// Publish: move decoded fields into stub, then null decoded's
					// pointers so the unique_ptr's `delete` invokes a no-op dtor
					// (resource fields all null) and only frees the husk.
					publishDecodedBufferLocked(task.audio, task.stub, decoded.get());
				}

				decRef(task.stub, task.refcount);
			}
		}

		// Function-prologue replacements for the six SoundBuffer leaf accessors.
		// Each is a full C++ reimplementation of the engine function with an
		// isPending() short-circuit. All installed via genJumpUnprotected, which
		// replaces the engine's first 5 bytes.
		//
		// The engine's existing call sites stay completely untouched — they still
		// call 0x402980 / 0x402B50 / 0x402E90 / 0x4029A0 / 0x402EC0 / 0x4027E0
		// exactly as before. Those addresses just route to our C++ now.

		// Replaces 0x402B50 AudioController::SetSoundBufferPosition. Faithful
		// C++ reimplementation of the engine's body. Constants and quadrant
		// logic match the original (which has its own conventions — including
		// a deliberately-imprecise "pi" of 3.1400001 — so positional audio
		// behaves bit-for-bit the same).
		void __fastcall setSoundBufferPosition_replacement(TES3::AudioController* self, void* /*edx*/, TES3::SoundBuffer* sb, TES3::Vector3* position) {
			if (isPending(sb)) return;
			if (!sb || !sb->lpSoundBuffer) return; // decode-fail stub
			if (!self->dsound3DCommitted) return;

			constexpr float k_pi = 3.1400001f;
			constexpr float k_2pi = 6.2800002f;
			constexpr float k_volume_to_dB = -13.070588f;

			// Listener-relative deltas + 3D distance.
			const float dx = position->x - self->listenerPosition.x;
			const float dy = position->y - self->listenerPosition.y;
			const float dz = position->z - self->listenerPosition.z;
			const float dist = std::sqrt(dx * dx + dy * dy + dz * dz);

			// Bearing in [0, 2π). Quadrant logic mirrors 0x402BB6..0x402CB3 —
			// each branch reproduces a labeled block of the disassembly.
			float bearing = 0.0f;
			if (dx < 0.0f) {
				if (dy < 0.0f) {
					bearing = k_pi - std::atan2(-dx / -dy, 1.0f);
				} else if (dy > 0.0f) {
					bearing = std::atan2(-dx / dy, 1.0f);
				}
			} else if (dx > 0.0f) {
				if (dy > 0.0f) {
					bearing = k_2pi - std::atan2(dx / dy, 1.0f);
				} else if (dy < 0.0f) {
					bearing = std::atan2(dx / -dy, 1.0f) + k_pi;
				}
			}

			const float angle = self->yawAxis - bearing;

			if ((sb->bufferDescription.dwFlags & DSBCAPS_CTRL3D) != 0) {
				// 3D path: position the buffer in listener-relative space.
				const D3DVALUE x_3d = std::sin(angle) * dist;
				const D3DVALUE z_3d = std::cos(angle) * dist;
				sb->lpSound3DBuffer->SetPosition(x_3d, dz, z_3d, DS3D_IMMEDIATE);
				self->dsound3DChanged = true;
				return;
			}

			// 2D path: pan + distance attenuation + volume.
			if (dist <= 0.1f) {
				sb->lpSoundBuffer->SetPan(0);
			} else {
				const float s = std::sin(angle);
				const float pan = (s < 0.0f ? -(s * s) : (s * s)) * 5000.0f;
				sb->lpSoundBuffer->SetPan(static_cast<LONG>(pan));
			}

			// Distance attenuation -> dBReduction. maxDistance is stored as a
			// float bit pattern in an int slot (engine convention).
			float maxDistAsFloat;
			std::memcpy(&maxDistAsFloat, &sb->maxDistance, sizeof(float));
			const auto* ndd = TES3::DataHandler::get()->nonDynamicData;
			const float scaled = maxDistAsFloat / ndd->GMSTs[TES3::GMST::fAudioMaxDistanceMult]->value.asFloat;

			if (scaled > 200.0f) {
				sb->dBReduction = 0;
			} else if (scaled > 99.0f) {
				sb->dBReduction = static_cast<int>(dist * 0.5f);
			} else if (scaled < 10.0f) {
				sb->dBReduction = static_cast<int>(dist * 6.0f);
			} else if (scaled >= 50.0f) {
				sb->dBReduction = static_cast<int>(dist * 3.0f);
			} else {
				sb->dBReduction = static_cast<int>(dist * 4.0f);
			}

			// DSound's IDirectSoundBuffer::SetVolume takes a value in hundredths
			// of a decibel (range [-10000, 0] = [-100 dB, 0 dB]). Formula matches
			// the engine: -3333 + (volume × 13.07) - dBReduction, clamped.
			// volume==0 short-circuits to fully silent.
			const unsigned char vol = sb->volume;
			int v24;
			if (vol == 0) {
				v24 = -10000;
			} else {
				v24 = -3333 - static_cast<int>(static_cast<float>(vol) * k_volume_to_dB);
			}
			if (vol < 253) {
				v24 -= sb->dBReduction;
			}
			int v25 = (v24 >= 0) ? 0 : v24;
			if (v25 < -10000) v25 = -10000;
			sb->lpSoundBuffer->SetVolume(v25);
			self->dsound3DChanged = true;
		}

		// Replaces 0x402E90 AudioController::GetSoundBufferStatus.
		unsigned char __fastcall getSoundBufferStatus_replacement(TES3::AudioController* /*audio*/, void* /*edx*/, TES3::SoundBuffer* sb) {
			if (isPending(sb)) return DSBSTATUS_PLAYING;
			// Decode-fail or any other path that leaves a real-state SoundBuffer
			// with null lpSoundBuffer. Returning 0 (not playing) lets updateSounds
			// reap the SoundEvent normally instead of crashing here.
			if (!sb || !sb->lpSoundBuffer) return 0;
			DWORD status = 0;
			sb->lpSoundBuffer->GetStatus(&status);
			return static_cast<unsigned char>(status);
		}

		// Replaces 0x4029A0 AudioController::SetSoundBufferCurrentPosition.
		void __fastcall setSoundBufferCurrentPosition_replacement(TES3::AudioController* /*audio*/, void* /*edx*/, TES3::SoundBuffer* sb, float position) {
			if (isPending(sb)) return;
			if (!sb || !sb->lpSoundBuffer) return;
			if (position < 0.0f) return;
			// Engine global written (but never read, per IDA xref) by the original
			// at this point. Preserved for byte-for-byte parity; some future engine
			// patch could conceivably read it.
			using gLastSetSoundBufferBytePos = mwse::ExternalGlobal<DWORD, 0x7C5F20>;
			DWORD bytes = static_cast<DWORD>(sb->bufferDescription.dwBufferBytes * position) & ~3u;
			gLastSetSoundBufferBytePos::set(bytes);
			sb->lpSoundBuffer->SetCurrentPosition(bytes);
		}

		// Replaces 0x402EC0 AudioController::GetSoundBufferLipSyncLevel.
		float __fastcall getSoundBufferLipSyncLevel_replacement(TES3::AudioController* /*audio*/, void* /*edx*/, TES3::SoundBuffer* sb) {
			// Mouth stays closed during the decode window. The original would have
			// returned 0.5 (rawAudio==null path) which produced a frozen yawn.
			if (isPending(sb)) return 0.0f;

			// Real buffer path — verbatim original behavior.
			if (!sb->rawAudio) return 0.5f;
			if (!sb->lpSoundBuffer) return 0.5f; // defensive; rawAudio without DSound buffer shouldn't occur

			DWORD status = 0;
			sb->lpSoundBuffer->GetStatus(&status);
			if (!(status & 5)) return 0.0f;

			DWORD pos = 0;
			if (FAILED(sb->lpSoundBuffer->GetCurrentPosition(&pos, nullptr))) return 0.5f;

			short sample = sb->rawAudio[pos / 0x900];
			int absSample = std::abs(static_cast<int>(sample));
			return absSample * (1.0f / 32768.0f);
		}

		// Replaces 0x402980 AudioController::StopSoundBuffer. The original
		// null-checks the wrapper but not lpSoundBuffer, so any caller that
		// reaches a pending stub (lpSoundBuffer==null) or a decode-failed real
		// stub crashes on the vtable read at 0x40298A. removeSound's status
		// gate doesn't help — getSoundBufferStatus_replacement returns
		// DSBSTATUS_PLAYING for pending stubs, so the engine falls through to
		// this function. killSounds and Sound::dtor reach it without any gate.
		void __fastcall stopSoundBuffer_replacement(TES3::AudioController* /*audio*/, void* /*edx*/, TES3::SoundBuffer* sb) {
			if (!sb) return;
			if (isPending(sb)) return;
			if (!sb->lpSoundBuffer) return;
			sb->lpSoundBuffer->Stop();
		}

		// Replaces 0x4027E0 AudioController::ReleaseSoundBuffer.
		void __fastcall releaseSoundBuffer_replacement(TES3::AudioController* /*audio*/, void* /*edx*/, TES3::SoundBuffer* sb) {
			if (!sb) return;

			if (isPending(sb)) {
				// Worker may still hold a reference. Refcount; whoever zeros it deletes.
				std::atomic<int>* rc = nullptr;
				{
					std::lock_guard lk(g_stubRefMutex);
					auto it = g_stubRefcounts.find(sb);
					if (it != g_stubRefcounts.end()) rc = it->second;
				}
				if (rc) decRef(sb, rc);
				else delete sb; // unknown stub (refcount table missed it); free directly
				return;
			}

			delete sb;
		}

		// Replaces the call to LoadSoundFile inside addTempSound (0x48C369). For
		// non-voiceover paths, falls straight through to the engine's loadSoundFile.
		// For voiceover paths, allocates a pending stub and queues the actual decode
		// onto the worker thread.
		TES3::SoundBuffer* __fastcall asyncLoadSoundFile(TES3::AudioController* audio, void* /*edx*/, const char* filename, bool isPointSource) {
			if (!isVoiceoverPath(filename)) {
				return audio->loadSoundFile(filename, isPointSource);
			}

			auto* stub = allocateStub();
			auto* rc = acquireStubRef(stub);

			{
				std::lock_guard lk(g_queueMutex);
				g_queue.push_back(DecodeTask{ filename, audio, isPointSource, stub, rc });
			}
			g_queueCv.notify_one();
			return stub;
		}

	} // anonymous namespace

	void install() {
		// Divert addTempSound's call to LoadSoundFile. The two other call sites
		// of LoadSoundFile (Sound::set3DParams at 0x51083F / 0x510859) are for
		// permanent Sound records and never see voiceover paths — leave them.
		//
		// The call instruction is at 0x48C369 (5-byte E8 rel32). Verified bytes:
		// E8 42 5A F7 FF (= call 0x401DB0). genCallEnforced returns false silently
		// on byte-pattern mismatch — log on failure so the streamer never silently
		// no-ops.
		if (!genCallEnforced(0x48C369, 0x401DB0, reinterpret_cast<DWORD>(&asyncLoadSoundFile))) {
			log::getLog() << "Voice streamer: call-site patch at 0x48C369 failed (byte pattern mismatch). Streamer is INACTIVE.\n";
			return;
		}

		// Replace the leaf SoundBuffer accessors at their function prologues. The
		// engine's existing call sites stay untouched; they still call these
		// addresses, which now route to our C++. All stub-awareness is encapsulated
		// in the six replacements.
		genJumpUnprotected(0x402E90, reinterpret_cast<DWORD>(&getSoundBufferStatus_replacement));
		genJumpUnprotected(0x4029A0, reinterpret_cast<DWORD>(&setSoundBufferCurrentPosition_replacement));
		genJumpUnprotected(0x402EC0, reinterpret_cast<DWORD>(&getSoundBufferLipSyncLevel_replacement));
		genJumpUnprotected(0x4027E0, reinterpret_cast<DWORD>(&releaseSoundBuffer_replacement));
		genJumpUnprotected(0x402B50, reinterpret_cast<DWORD>(&setSoundBufferPosition_replacement));
		genJumpUnprotected(0x402980, reinterpret_cast<DWORD>(&stopSoundBuffer_replacement));

		// Patch: Replace the calls to stopSoundBuffer to guard against potential nullptr error on lpSoundBuffer.
		auto AudioController_stopSoundBuffer = &TES3::AudioController::stopSoundBuffer;
		genCallEnforced(0x48B72B, 0x402980, *reinterpret_cast<DWORD*>(&AudioController_stopSoundBuffer));
		genCallEnforced(0x48B753, 0x402980, *reinterpret_cast<DWORD*>(&AudioController_stopSoundBuffer));
		genCallEnforced(0x48BA2E, 0x402980, *reinterpret_cast<DWORD*>(&AudioController_stopSoundBuffer));
		genCallEnforced(0x48BA66, 0x402980, *reinterpret_cast<DWORD*>(&AudioController_stopSoundBuffer));
		genCallEnforced(0x48C121, 0x402980, *reinterpret_cast<DWORD*>(&AudioController_stopSoundBuffer));
		genCallEnforced(0x48C257, 0x402980, *reinterpret_cast<DWORD*>(&AudioController_stopSoundBuffer));
		genCallEnforced(0x48C8BD, 0x402980, *reinterpret_cast<DWORD*>(&AudioController_stopSoundBuffer));
		genCallEnforced(0x48C985, 0x402980, *reinterpret_cast<DWORD*>(&AudioController_stopSoundBuffer));
		genCallEnforced(0x48CCB0, 0x402980, *reinterpret_cast<DWORD*>(&AudioController_stopSoundBuffer));
		genCallEnforced(0x48CCD4, 0x402980, *reinterpret_cast<DWORD*>(&AudioController_stopSoundBuffer));
		genCallEnforced(0x48CCED, 0x402980, *reinterpret_cast<DWORD*>(&AudioController_stopSoundBuffer));
		genCallEnforced(0x510551, 0x402980, *reinterpret_cast<DWORD*>(&AudioController_stopSoundBuffer));
		genCallEnforced(0x510A19, 0x402980, *reinterpret_cast<DWORD*>(&AudioController_stopSoundBuffer));
		genCallEnforced(0x510BE9, 0x402980, *reinterpret_cast<DWORD*>(&AudioController_stopSoundBuffer));
		genCallEnforced(0x51760E, 0x402980, *reinterpret_cast<DWORD*>(&AudioController_stopSoundBuffer));
		genCallEnforced(0x518460, 0x402980, *reinterpret_cast<DWORD*>(&AudioController_stopSoundBuffer));

		g_running.store(true, std::memory_order_release);
		g_worker = std::thread(&workerLoop);
		mwse::windows::SetThreadDescription(g_worker.native_handle(), L"MWSEVoiceDecoder");
	}

	void shutdown() {
		g_running.store(false, std::memory_order_release);
		g_queueCv.notify_all();
		if (g_worker.joinable()) g_worker.join();
	}

} // namespace mwse::patch::voice
