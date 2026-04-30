#include "TES3VoiceStreamer.h"

#include <windows.h>
#include <dsound.h>

#include "Log.h"
#include "MemoryUtil.h"

#include "TES3AudioController.h"
#include "TES3CriticalSection.h"
#include "TES3DataHandler.h"
#include "TES3Reference.h"
#include "TES3Sound.h"
#include "TES3WorldController.h"

#include <atomic>
#include <cmath>
#include <condition_variable>
#include <cstring>
#include <deque>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>

namespace mwse::patch::voice {

	namespace {

		// ------------------------------------------------------------------
		// Engine functions we call into directly (not replaced).
		// ------------------------------------------------------------------

		using LoadSoundFile_t   = TES3::SoundBuffer* (__thiscall*)(TES3::AudioController*, const char*, bool);
		using PlaySoundBuffer_t = void              (__thiscall*)(TES3::AudioController*, TES3::SoundBuffer*, int);
		using SetVolume_t       = void              (__thiscall*)(TES3::AudioController*, TES3::SoundBuffer*, unsigned char);
		using SetMinMax_t       = int               (__thiscall*)(TES3::AudioController*, TES3::SoundBuffer*, float, float);

		const auto LoadSoundFile_Orig   = reinterpret_cast<LoadSoundFile_t>(0x401DB0);
		const auto PlaySoundBuffer_Orig = reinterpret_cast<PlaySoundBuffer_t>(0x402820);
		const auto SetVolume_Orig       = reinterpret_cast<SetVolume_t>(0x4029F0);
		const auto SetMinMax_Orig       = reinterpret_cast<SetMinMax_t>(0x402AC0);

		// ------------------------------------------------------------------
		// Stub representation
		// ------------------------------------------------------------------
		// A "pending" SoundBuffer is one whose decode hasn't finished. Encoded
		// by bufferDescription.dwSize == 0 (the real value, set by LoadSoundFile,
		// is sizeof(DSBUFFERDESC) == 36 — never 0 for a real buffer).
		//
		// All stub-awareness in MWSE lives in this file. The five leaf accessor
		// replacements below check isPending() and short-circuit; everything
		// else in the engine treats stubs as ordinary SoundBuffers.

		constexpr DWORD STUB_PENDING_SENTINEL = 0;
		constexpr DWORD REAL_BUFFERDESC_SIZE  = sizeof(DSBUFFERDESC); // 36

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

		// Allocate stubs from the engine heap so anything that frees them via
		// the engine's normal `delete` works without heap-mismatch corruption.
		TES3::SoundBuffer* allocateStub() {
			auto* sb = mwse::tes3::_new<TES3::SoundBuffer>();
			std::memset(sb, 0, sizeof(TES3::SoundBuffer));

			sb->lpSoundBuffer    = nullptr;
			sb->lpSound3DBuffer  = nullptr;
			sb->rawAudio         = nullptr;
			sb->isVoiceover      = true;

			// Field invariants we rely on across the leaf accessors:
			// - dwFlags must NOT have DSBCAPS_CTRL3D set, so SetSoundBufferPosition
			//   takes the 2D branch which is null-safe via our replacement.
			// - lpwfxFormat must be non-null and point at zero-initialized memory
			//   so SetSoundBufferFrequency reads nSamplesPerSec==0 and exits early.
			sb->bufferDescription.dwFlags        = 0;
			sb->bufferDescription.dwBufferBytes  = 0;
			sb->bufferDescription.lpwfxFormat    = reinterpret_cast<WAVEFORMATEX*>(sb->fileHeader);

			// Sentinel goes last so a partially-built stub never looks "real".
			sb->bufferDescription.dwSize         = STUB_PENDING_SENTINEL;
			return sb;
		}

		// Free what releaseRealSoundBuffer would, but for the stub case where
		// lpSoundBuffer/lpSound3DBuffer/rawAudio are all null. Just release the
		// SoundBuffer struct itself via the engine heap.
		void deleteStub(TES3::SoundBuffer* sb) {
			mwse::tes3::_delete(sb);
		}

		// What the engine's original ReleaseSoundBuffer does on a real buffer.
		// Pulled out as a helper so the worker's "stub was reaped mid-decode" path
		// and the replacement's "real buffer" branch share one implementation.
		void releaseRealSoundBuffer(TES3::SoundBuffer* sb) {
			if (!sb) return;
			if (sb->lpSound3DBuffer) sb->lpSound3DBuffer->Release();
			if (sb->lpSoundBuffer)   sb->lpSoundBuffer->Release();
			if (sb->rawAudio)        mwse::tes3::_delete(sb->rawAudio);
			mwse::tes3::_delete(sb);
		}

		// ------------------------------------------------------------------
		// Stub refcount
		// ------------------------------------------------------------------
		// A stub may be referenced by both the SoundEvent (returned to the game)
		// and the worker (still decoding). Whichever decrements last frees it.
		// This is the only reason we hook ReleaseSoundBuffer at all — without it,
		// the engine could free a stub while the worker is mid-decode.

		std::mutex                                                   g_stubRefMutex;
		std::unordered_map<TES3::SoundBuffer*, std::atomic<int>*>    g_stubRefcounts;

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
				deleteStub(stub);
			}
		}

		// ------------------------------------------------------------------
		// Decode worker
		// ------------------------------------------------------------------

		struct DecodeTask {
			std::string             path;
			TES3::AudioController*  audio;
			bool                    isPointSource;
			TES3::SoundBuffer*      stub;
			std::atomic<int>*       refcount;
		};

		std::mutex              g_queueMutex;
		std::condition_variable g_queueCv;
		std::deque<DecodeTask>  g_queue;
		std::thread             g_worker;
		std::atomic<bool>       g_running{ false };

		// RAII guard for the audio-lists critical section that addTempSound /
		// updateSounds / sayDialogueVoice all share. Publishing the decoded
		// buffer must not race with the main thread iterating tempSoundEvents.
		//
		// Verified: addTempSound at 0x48C326 reads `mov ecx, [ebp+0xB534]` then
		// calls NiCriticalSection::Lock — i.e. dh->criticalSectionAudioEvents.
		// We go through MWSE's enter()/leave() wrappers so we hit the same
		// NiCriticalSection::Lock path the engine itself uses.
		class AudioListsLock {
			TES3::CriticalSection* cs;
		public:
			explicit AudioListsLock(const char* id)
				: cs(TES3::DataHandler::get()->criticalSectionAudioEvents)
			{
				cs->enter(id);
			}
			~AudioListsLock() { cs->leave(); }
			AudioListsLock(const AudioListsLock&)            = delete;
			AudioListsLock& operator=(const AudioListsLock&) = delete;
		};

		// Custom-deleter unique_ptr for engine-allocated SoundBuffers. Cleans up
		// COM objects + rawAudio + the wrapper struct in one move, matching the
		// engine's own ReleaseSoundBuffer (we route through our helper rather
		// than calling the replacement, to avoid the stub-detection branch when
		// we know we have a real buffer).
		struct EngineSoundBufferDeleter {
			void operator()(TES3::SoundBuffer* sb) const noexcept {
				releaseRealSoundBuffer(sb);
			}
		};
		using EngineSoundBufferPtr = std::unique_ptr<TES3::SoundBuffer, EngineSoundBufferDeleter>;

		// Move decoded contents into the stub atomically; transition dwSize=0→36
		// is the publish edge that flips the leaf accessors out of stub mode.
		void publishDecodedBufferLocked(
			TES3::AudioController* audio,
			TES3::SoundBuffer*     stub,
			TES3::SoundBuffer*     decoded)
		{
			stub->lpSoundBuffer       = decoded->lpSoundBuffer;
			stub->lpSound3DBuffer     = decoded->lpSound3DBuffer;
			stub->rawAudio            = decoded->rawAudio;
			std::memcpy(stub->fileHeader, decoded->fileHeader, sizeof(stub->fileHeader));
			stub->bufferDescription   = decoded->bufferDescription; // dwSize becomes 36 here

			// lpwfxFormat is an INTERNAL pointer (decoded->fileHeader, inside
			// decoded's own struct that we're about to free). Redirect to
			// stub->fileHeader, where we just memcpy'd the WAVEFORMATEX bytes.
			// Without this, SetSoundBufferFrequency (or any later wfx read)
			// is a use-after-free.
			stub->bufferDescription.lpwfxFormat =
				reinterpret_cast<WAVEFORMATEX*>(stub->fileHeader);

			stub->isVoiceover         = true;

			// Do NOT touch stub->volume here. addTempSound already called
			// SetSoundBufferVolume(stub, vol) while the stub was pending,
			// which writes the volume field even though it skips the DSound call
			// (lpSoundBuffer was null at the time). decoded->volume is
			// uninitialized from operator new — clobbering with it would set
			// volume to 0 and produce a -10000 dB attenuation (silent) on the
			// next updateSounds tick.

			// Null out decoded's resource fields — DSound objects + rawAudio now
			// belong to the stub. The caller's EngineSoundBufferPtr will then
			// invoke releaseRealSoundBuffer on the husk; with all resource fields
			// null, that just frees the wrapper struct.
			decoded->lpSoundBuffer    = nullptr;
			decoded->lpSound3DBuffer  = nullptr;
			decoded->rawAudio         = nullptr;

			// Apply Volume and Min/Max distance to the now-real buffer.
			// addTempSound called both setters on the pending stub; the wrappers
			// wrote the fields but skipped the COM calls (lpSoundBuffer /
			// lpSound3DBuffer were null at the time). Without re-applying:
			//   - 3D mode (lpSound3DBuffer non-null): DSound uses defaults
			//     DS3D_DEFAULTMINDISTANCE=1m / DS3D_DEFAULTMAXDISTANCE=1e9m,
			//     attenuating to silence at any sane game distance. updateSounds
			//     only calls SetPosition each frame — it never re-applies
			//     min/max — so the attenuation stays buried forever.
			//   - 2D mode: lpSoundBuffer is at 0 dB max from LoadSoundFile_Orig's
			//     finalizer; SetVolume here corrects the brief loud frame before
			//     updateSounds re-applies via SetSoundBufferPosition's 2D path.
			//
			// stub->minDistance/maxDistance are typed `int` in MWSE's struct but
			// the engine stores float bit patterns there (see decompilation of
			// SetSoundBufferMinMaxDistance's else branch). Re-cast to float.
			SetVolume_Orig(audio, stub, stub->volume);

			float minD = 0.0f, maxD = 0.0f;
			std::memcpy(&minD, &stub->minDistance, sizeof(float));
			std::memcpy(&maxD, &stub->maxDistance, sizeof(float));
			SetMinMax_Orig(audio, stub, minD, maxD);

			// addTempSound's PlaySoundBuffer earlier was a no-op (lpSoundBuffer null);
			// now that the buffer is real, kick off playback. updateSounds will
			// reposition the stub on its next tick from the SoundEvent's reference.
			PlaySoundBuffer_Orig(audio, stub, /*flags*/ 0);
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

				// 1. Decode off the main thread, no engine lock held. Wrap in
				//    a unique_ptr so any early return correctly releases the
				//    engine-allocated buffer + its COM resources via the deleter.
				EngineSoundBufferPtr decoded(
					LoadSoundFile_Orig(task.audio, task.path.c_str(), task.isPointSource));

				// 2. Re-enter the engine to publish. RAII lock scope = the rest
				//    of this iteration; auto-release on every continue path.
				{
					AudioListsLock lk("MWSE:VoiceStreamer");

					// Stub was released while we were decoding (cell change, killSounds).
					if (task.refcount->load() <= 1) {
						decRef(task.stub, task.refcount);
						continue;  // unique_ptr drops decoded; AudioListsLock leaves
					}

					// Decode failed (file missing, bad format, OOM). Flip dwSize so
					// updateSounds reaps the SoundEvent — its status check will now
					// hit the real-buffer path and find lpSoundBuffer null. The
					// leaf replacements all guard against that.
					if (!decoded) {
						task.stub->bufferDescription.dwSize = REAL_BUFFERDESC_SIZE;
						decRef(task.stub, task.refcount);
						continue;
					}

					// Publish: move decoded fields into stub, then null decoded's
					// pointers so the unique_ptr's deleter only frees the husk
					// (releaseRealSoundBuffer's NULL checks make it a no-op for
					// the resource fields).
					publishDecodedBufferLocked(task.audio, task.stub, decoded.get());
				}

				decRef(task.stub, task.refcount);
			}
		}

		// ==================================================================
		// Function-prologue REPLACEMENTS for the five leaf accessors.
		//
		// Four are full reimplementations with an isPending() short-circuit
		// (Get/SetCurrentPosition/LipSync/Release). The fifth (SetPosition)
		// uses a trampoline because its body is too math-heavy to reimplement.
		// All installed via genJumpUnprotected, replacing the engine's first
		// 5 bytes.
		//
		// The engine's 35+ existing call sites stay completely untouched:
		// they still call 0x402B50 / 0x402E90 / 0x4029A0 / 0x402EC0 / 0x4027E0
		// exactly as before. Those addresses just route to our C++ now.
		// ==================================================================

		// Replaces 0x402B50 AudioController::SetSoundBufferPosition.
		// This one CANNOT be a full reimpl — too much trig/distance/dB math.
		// Instead, function-prologue JMP + trampoline: trampoline reproduces
		// the 5 bytes we overwrite and then `push ret`s into 0x402B55, so the
		// rest of the function executes verbatim.
		//
		// Verified: bytes at 0x402B50 are `83 EC 08 55 56` = sub esp,8; push ebp;
		// push esi. Clean 5-byte boundary. The 2D path's early `SetPan` and the
		// 3D path's `SetPosition` both dereference lpSoundBuffer / lpSound3DBuffer
		// without a null guard, hence the isPending and lpSoundBuffer guards in
		// the wrapper before we hand off to the trampoline.
		__declspec(naked) void __fastcall setSoundBufferPosition_trampoline(
			TES3::AudioController* /*self*/,
			void* /*edx*/,
			TES3::SoundBuffer*   /*sb*/,
			TES3::Vector3*       /*pos*/)
		{
			__asm {
				sub esp, 8           // reproduce 0x402B50..54
				push ebp
				push esi
				push 0x402B55        // push-ret avoids a rel32 jmp whose offset
				ret                  // depends on where MSVC places this function
			}
		}

		void __fastcall setSoundBufferPosition_replacement(
			TES3::AudioController* self,
			void* /*edx*/,
			TES3::SoundBuffer* sb,
			TES3::Vector3* pos)
		{
			if (isPending(sb)) return;             // pending stub
			if (!sb || !sb->lpSoundBuffer) return; // decode-fail stub
			setSoundBufferPosition_trampoline(self, nullptr, sb, pos);
		}

		// Replaces 0x402E90 AudioController::GetSoundBufferStatus.
		unsigned char __fastcall getSoundBufferStatus_replacement(
			TES3::AudioController* /*audio*/,
			void* /*edx*/,
			TES3::SoundBuffer* sb)
		{
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
		void __fastcall setSoundBufferCurrentPosition_replacement(
			TES3::AudioController* /*audio*/,
			void* /*edx*/,
			TES3::SoundBuffer* sb,
			float position)
		{
			if (isPending(sb)) return;
			if (!sb || !sb->lpSoundBuffer) return;
			if (position < 0.0f) return;
			DWORD bytes = static_cast<DWORD>(sb->bufferDescription.dwBufferBytes * position) & ~3u;
			// Engine global at 0x7C5F20: preserved for byte-for-byte parity with
			// the original. Some other engine code reads the most-recently-set
			// playback byte position from this address.
			*reinterpret_cast<DWORD*>(0x7C5F20) = bytes;
			sb->lpSoundBuffer->SetCurrentPosition(bytes);
		}

		// Replaces 0x402EC0 AudioController::GetSoundBufferLipSyncLevel.
		float __fastcall getSoundBufferLipSyncLevel_replacement(
			TES3::AudioController* /*audio*/,
			void* /*edx*/,
			TES3::SoundBuffer* sb)
		{
			// Mouth stays closed during the decode window. The original would have
			// returned 0.5 (rawAudio==null path) which produced a frozen yawn.
			if (isPending(sb))    return 0.0f;

			// Real buffer path — verbatim original behavior.
			if (!sb->rawAudio)    return 0.5f;
			if (!sb->lpSoundBuffer) return 0.5f; // defensive; rawAudio without DSound buffer shouldn't occur

			DWORD status = 0;
			sb->lpSoundBuffer->GetStatus(&status);
			if (!(status & 5))    return 0.0f;

			DWORD pos = 0;
			if (FAILED(sb->lpSoundBuffer->GetCurrentPosition(&pos, nullptr))) return 0.5f;

			short sample = sb->rawAudio[pos / 0x900];
			int   absSample = std::abs(static_cast<int>(sample));
			return absSample * (1.0f / 32768.0f);
		}

		// Replaces 0x4027E0 AudioController::ReleaseSoundBuffer.
		void __fastcall releaseSoundBuffer_replacement(
			TES3::AudioController* /*audio*/,
			void* /*edx*/,
			TES3::SoundBuffer* sb)
		{
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
				else    deleteStub(sb);  // unknown stub (refcount table missed it); free directly
				return;
			}

			releaseRealSoundBuffer(sb);
		}

		// ==================================================================
		// asyncLoadSoundFile — the only call-site divert. Replaces the call
		// at addTempSound 0x48C369 (originally calling 0x401DB0).
		// ==================================================================

		TES3::SoundBuffer* __fastcall asyncLoadSoundFile(
			TES3::AudioController* audio,
			void* /*edx*/,
			const char* filename,
			bool isPointSource)
		{
			if (!isVoiceoverPath(filename)) {
				return LoadSoundFile_Orig(audio, filename, isPointSource);
			}

			// Voiceover: stub now, decode on worker.
			auto* stub = allocateStub();
			auto* rc   = acquireStubRef(stub);

			{
				std::lock_guard lk(g_queueMutex);
				g_queue.push_back(DecodeTask{
					filename,
					audio,
					isPointSource,
					stub,
					rc,
				});
			}
			g_queueCv.notify_one();
			return stub;
		}

	}  // anonymous namespace

	// ----------------------------------------------------------------------
	// Public install / shutdown
	// ----------------------------------------------------------------------

	void install() {
		// 1. Divert addTempSound's call to LoadSoundFile.
		//    The two other LoadSoundFile call sites (0x51083F, 0x510859 in
		//    Sound::set3DParams) are for permanent Sound records and never see
		//    voiceover paths — leave them alone.
		//
		// The call instruction is at 0x48C369 (5-byte E8 rel32). Verified via
		// read_memory_bytes: the 5 bytes are E8 42 5A F7 FF (call 0x401DB0).
		// genCallEnforced returns false silently if the byte pattern doesn't
		// match — we log on failure so the streamer never silently no-ops.
		if (!genCallEnforced(0x48C369, 0x401DB0, reinterpret_cast<DWORD>(&asyncLoadSoundFile))) {
			log::getLog() << "Voice streamer: call-site patch at 0x48C369 failed "
			                 "(byte pattern mismatch). Streamer is INACTIVE.\n";
			return;
		}

		// 2. Replace the leaf SoundBuffer accessors at their function prologues.
		//    The engine's 35+ existing call sites stay completely untouched —
		//    they still call these addresses, which now route to our C++.
		//    All stub-awareness is encapsulated in the five replacements.
		genJumpUnprotected(0x402E90, reinterpret_cast<DWORD>(&getSoundBufferStatus_replacement));
		genJumpUnprotected(0x4029A0, reinterpret_cast<DWORD>(&setSoundBufferCurrentPosition_replacement));
		genJumpUnprotected(0x402EC0, reinterpret_cast<DWORD>(&getSoundBufferLipSyncLevel_replacement));
		genJumpUnprotected(0x4027E0, reinterpret_cast<DWORD>(&releaseSoundBuffer_replacement));
		// Trampoline-based replacement; see setSoundBufferPosition_trampoline above.
		genJumpUnprotected(0x402B50, reinterpret_cast<DWORD>(&setSoundBufferPosition_replacement));

		// 3. Spin up the decode worker.
		g_running.store(true, std::memory_order_release);
		g_worker = std::thread(&workerLoop);
	}

	void shutdown() {
		g_running.store(false, std::memory_order_release);
		g_queueCv.notify_all();
		if (g_worker.joinable()) g_worker.join();
	}

}  // namespace mwse::patch::voice
