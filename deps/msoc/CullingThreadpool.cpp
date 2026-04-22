////////////////////////////////////////////////////////////////////////////////
// Copyright 2017 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not
// use this file except in compliance with the License.  You may obtain a copy
// of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
// License for the specific language governing permissions and limitations
// under the License.
////////////////////////////////////////////////////////////////////////////////
#include <assert.h>
#include <chrono>
#include <fstream>
#include "CullingThreadpool.h"

#define SAFE_DELETE(X) {if (X != nullptr) delete X; X = nullptr;}
#define SAFE_DELETE_ARRAY(X) {if (X != nullptr) delete[] X; X = nullptr;}

template<class T> CullingThreadpool::StateData<T>::StateData(unsigned int maxJobs) :
	mMaxJobs(maxJobs),
	mCurrentIdx(~0)
{
	mData = new T[mMaxJobs];
}

template<class T> CullingThreadpool::StateData<T>::~StateData() 
{
	SAFE_DELETE_ARRAY(mData);
}

template<class T> void CullingThreadpool::StateData<T>::AddData(const T &data) 
{ 
	mCurrentIdx++; mData[mCurrentIdx % mMaxJobs] = data; 
}

template<class T> const T *CullingThreadpool::StateData<T>::GetData() const
{ 
	return &mData[mCurrentIdx % mMaxJobs];
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Helper class: Mostly lockless queue for render jobs
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CullingThreadpool::RenderJobQueue::RenderJobQueue(unsigned int nBins, unsigned int maxJobs) : 
	mNumBins(nBins),
	mMaxJobs(maxJobs)
{
	mRenderPtrs = new std::atomic_uint[mNumBins];
	mBinMutexes = new std::atomic_uint[mNumBins];
	for (unsigned int i = 0; i < mNumBins; ++i)
		mBinMutexes[i] = 0;

	mJobs = new Job[mMaxJobs];
	for (unsigned int i = 0; i < mMaxJobs; ++i)
		mJobs[i].mRenderJobs = new TriList[mNumBins];

	// Compute worst case job size (we allocate memory for the worst case)
	const unsigned int TriSize = 3 * 3;
	const unsigned int MaxTrisPerJob = TRIS_PER_JOB * 6;
	const unsigned int MaxJobSize = MaxTrisPerJob * TriSize;
	mTrilistData = new float[MaxJobSize * mMaxJobs * mNumBins];

	// Setup trilist objects used for binning
	for (unsigned int i = 0; i < mMaxJobs; ++i)
	{
		for (unsigned int j = 0; j < mNumBins; ++j)
		{
			int idx = i*mNumBins + j;
			TriList &tList = mJobs[i].mRenderJobs[j];
			tList.mNumTriangles = MaxTrisPerJob;
			tList.mTriIdx = 0;
			tList.mPtr = mTrilistData + idx*MaxJobSize;
		}
	}

	// Clear render queue
	Reset();
}

CullingThreadpool::RenderJobQueue::~RenderJobQueue()
{
	SAFE_DELETE_ARRAY(mRenderPtrs);
	SAFE_DELETE_ARRAY(mBinMutexes);
	for (unsigned int i = 0; i < mMaxJobs; ++i)
		SAFE_DELETE_ARRAY(mJobs[i].mRenderJobs);
	SAFE_DELETE_ARRAY(mJobs);
	SAFE_DELETE_ARRAY(mTrilistData);
}

inline unsigned int CullingThreadpool::RenderJobQueue::GetMinRenderPtr() const
{
	unsigned int minRenderPtr = mRenderPtrs[0];
	for (unsigned int i = 1; i < mNumBins; ++i)
	{
		unsigned int renderPtr = mRenderPtrs[i];
		minRenderPtr = renderPtr < minRenderPtr ? renderPtr : minRenderPtr;
	}
	return minRenderPtr;
}

inline void CullingThreadpool::RenderJobQueue::AdvanceRenderJob(int binIdx)
{
	mRenderPtrs[binIdx]++;
	mBinMutexes[binIdx] = 0;
}

inline unsigned int CullingThreadpool::RenderJobQueue::GetBestGlobalQueue() const
{
	// Find least advanced queue
	unsigned int bestBin = ~0, bestPtr = mWritePtr;
	for (unsigned int i = 0; i < mNumBins; ++i)
	{
		if (mRenderPtrs[i] < bestPtr && mBinMutexes[i] == 0)
		{
			bestBin = i;
			bestPtr = mRenderPtrs[i];
		}
	}
	return bestBin;
}

inline bool CullingThreadpool::RenderJobQueue::IsPipelineEmpty() const
{
	return GetMinRenderPtr() == mWritePtr;
}

inline bool CullingThreadpool::RenderJobQueue::CanWrite() const
{
	return mWritePtr - GetMinRenderPtr() < mMaxJobs;
}

inline bool CullingThreadpool::RenderJobQueue::CanBin() const
{
	return mBinningPtr < mWritePtr && mBinningPtr - GetMinRenderPtr() < mMaxJobs;
}

inline CullingThreadpool::RenderJobQueue::Job *CullingThreadpool::RenderJobQueue::GetWriteJob()
{
	return &mJobs[mWritePtr % mMaxJobs];
}

inline void CullingThreadpool::RenderJobQueue::AdvanceWriteJob()
{
	mWritePtr++;
}

inline CullingThreadpool::RenderJobQueue::Job *CullingThreadpool::RenderJobQueue::GetBinningJob()
{
	unsigned int binningPtr = mBinningPtr;
	if (binningPtr < mWritePtr && binningPtr - GetMinRenderPtr() < mMaxJobs)
	{
		if (mBinningPtr.compare_exchange_strong(binningPtr, binningPtr + 1))
		{
			mJobs[binningPtr % mMaxJobs].mBinningJobStartedIdx = binningPtr;
			return &mJobs[binningPtr % mMaxJobs];
		}
	}
	return nullptr;
}

inline void CullingThreadpool::RenderJobQueue::FinishedBinningJob(Job *job)
{
	// _Claude_ Explicit .load() because both fields are now std::atomic;
	// atomic-to-atomic operator= is deleted by std::atomic.
	job->mBinningJobCompletedIdx = job->mBinningJobStartedIdx.load();
}

inline CullingThreadpool::RenderJobQueue::Job *CullingThreadpool::RenderJobQueue::GetRenderJob(int binIdx)
{
	// Attempt to lock bin mutex
	unsigned int expected = 0;
	if (!mBinMutexes[binIdx].compare_exchange_strong(expected, 1))
		return nullptr;

	// Check any items in the queue, and bail if empty
	if (mRenderPtrs[binIdx] != mJobs[mRenderPtrs[binIdx] % mMaxJobs].mBinningJobCompletedIdx)
	{
		mBinMutexes[binIdx] = 0;
		return nullptr;
	}

	return &mJobs[mRenderPtrs[binIdx] % mMaxJobs];
}

void CullingThreadpool::RenderJobQueue::Reset()
{
	// _Claude_ HISTORICAL: This poison-mJobs-first ordering was the
	// cae2593f76 fix for the original freeze. It closed one specific
	// reproduction but RELIED on cross-thread visibility ordering that
	// volatile (the original field types) does not provide — a second
	// freeze with mRenderPtrs=1,1,1,...  proved the same class of bug
	// could still occur. The complete fix lives in two later changes:
	//   1. The mJobs/mWritePtr/mSuspendThreads/mNumSuspendedThreads/
	//      mKillThreads fields are now std::atomic, so writes here have
	//      seq_cst ordering relative to worker reads (see CullingThreadpool.h).
	//   2. Flush() now SuspendThreads + polls until mNumSuspendedThreads
	//      == mNumThreads BEFORE calling this method, so workers are
	//      parked in cv.wait while Reset runs — they cannot read these
	//      fields concurrent with the writes at all.
	// The ordering below is kept (rather than reverted to Intel's) so
	// the defense remains layered: even if a future caller bypasses
	// the suspend protocol, poisoning mJobs first still blocks the
	// specific stale-comparison race the original cae2593f76 commit
	// targeted.
	//
	// Original cae2593f76 reasoning, kept verbatim for historical context:
	// The original Intel ordering (write pointers first, render pointers
	// second, mJobs last) caused 30-second freezes at stage=3(clearBuffer)
	// for at least one MWSE user — diagnosed via MSOC.flushdump.txt showing
	// mWritePtr=0 alongside mRenderPtrs= 6 2 6 6 6 6 3 4 6 1 6 6 6 6 3 4.
	// Reproduction:
	//   1. Reset zeros mRenderPtrs[i] = 0 (still hasn't touched mJobs).
	//   2. Worker calls GetRenderJob: mRenderPtrs[binIdx]=0, but reads
	//      mJobs[0].mBinningJobCompletedIdx which still holds the OLD
	//      value (from a prior frame whose mWritePtr wrapped past
	//      mMaxJobs and back to 0).
	//   3. If the OLD value happens to be 0, the comparison succeeds and
	//      the worker returns a stale job pointer, runs RenderTrilist on
	//      stale data, then AdvanceRenderJob increments mRenderPtrs[binIdx]
	//      from 0 to 1.
	//   4. Cascade repeats per bin until Reset finally reaches and poisons
	//      the relevant mJobs slot.
	//   5. Pipeline ends up wedged with mRenderPtrs > mWritePtr=0; next
	//      ClearBuffer's Flush spins forever waiting for IsPipelineEmpty
	//      (= GetMinRenderPtr == mWritePtr) which can never be true.
	// The reorder closes the window: once mJobs[*] are all -1 (UINT_MAX
	// when read as unsigned), GetRenderJob's comparison
	// `mRenderPtrs[binIdx] != mJobs[...].mBinningJobCompletedIdx` always
	// fails (mRenderPtrs is a small frame counter, never UINT_MAX), so
	// no spurious AdvanceRenderJob can fire while Reset is in progress.
	for (unsigned int i = 0; i < mMaxJobs; ++i)
	{
		mJobs[i].mBinningJobCompletedIdx = -1;
		mJobs[i].mBinningJobStartedIdx = -1;
	}

	for (unsigned int i = 0; i < mNumBins; ++i)
		mRenderPtrs[i] = 0;

	mWritePtr = 0;
	mBinningPtr = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Culling threadpool private helper functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CullingThreadpool::SetupScissors()
{
	unsigned int width, height;
	mMOC->GetResolution(width, height);

	unsigned int binWidth;
	unsigned int binHeight;
	mMOC->ComputeBinWidthHeight(mBinsW, mBinsH, binWidth, binHeight);

	for (unsigned int ty = 0; ty < mBinsH; ++ty)
	{
		for (unsigned int tx = 0; tx < mBinsW; ++tx)
		{
			unsigned int threadIdx = tx + ty*mBinsW;

			// Adjust rects on final row / col to match resolution
			mRects[threadIdx].mMinX = tx*binWidth;
			mRects[threadIdx].mMaxX = tx + 1 == mBinsW ? width : (tx + 1) * binWidth;
			mRects[threadIdx].mMinY = ty * binHeight;
			mRects[threadIdx].mMaxY = ty + 1 == mBinsH ? height : (ty + 1) * binHeight;
		}
	}
}

void CullingThreadpool::ThreadRun(CullingThreadpool *threadPool, unsigned int threadId)
{ 
	threadPool->ThreadMain(threadId); 
}

void CullingThreadpool::ThreadMain(unsigned int threadIdx)
{
	while (true)
	{
		bool threadIsIdle = true;
		unsigned int threadBinIdx = threadIdx;

		// Wait for threads to be woken up (low CPU load sleep)
		std::unique_lock<std::mutex> lock(mSuspendedMutex);
		mNumSuspendedThreads++;
		mSuspendedCV.wait(lock, [&] {return !mSuspendThreads; });
		mNumSuspendedThreads--;
		lock.unlock();

		// Loop until suspended again
		while (!mSuspendThreads || !threadIsIdle)
		{
			if (mKillThreads)
				return;

			threadIsIdle = false;

			// Prio 1: Process any render jobs local to this thread
			unsigned int binIdx = threadBinIdx;
			threadBinIdx = threadBinIdx + mNumThreads < mNumBins ? threadBinIdx + mNumThreads : threadIdx;
			RenderJobQueue::Job *job = mRenderQueue->GetRenderJob(binIdx);
			if (job != nullptr)
			{
				if (job->mRenderJobs[binIdx].mTriIdx > 0)
					mMOC->RenderTrilist(job->mRenderJobs[binIdx], &mRects[binIdx]);

				mRenderQueue->AdvanceRenderJob(binIdx);
				continue;
			}

			// Prio 2: Process any outstanding setup/binning jobs
			if (mRenderQueue->CanBin())
			{
				// If no more rasterization jobs, get next binning job
				RenderJobQueue::Job *job = mRenderQueue->GetBinningJob();
				if (job != nullptr)
				{
					RenderJobQueue::BinningJob &sjob = job->mBinningJob;
					for (unsigned int i = 0; i < mNumBins; ++i)
						job->mRenderJobs[i].mTriIdx = 0;
					mMOC->BinTriangles(sjob.mVerts, sjob.mTris, sjob.nTris, job->mRenderJobs, mBinsW, mBinsH, sjob.mMatrix, sjob.mBfWinding, sjob.mClipPlanes, *sjob.mVtxLayout);
					mRenderQueue->FinishedBinningJob(job);
				}
				continue;
			}

			// Prio 3: No work is available, work steal from another thread's queue
			if (mNumBins > mNumThreads)
			{
				binIdx = mRenderQueue->GetBestGlobalQueue();
				if (binIdx < mRenderQueue->mNumBins)
				{
					RenderJobQueue::Job *job = mRenderQueue->GetRenderJob(binIdx);
					if (job != nullptr)
					{
						if (job->mRenderJobs[binIdx].mTriIdx > 0)
							mMOC->RenderTrilist(job->mRenderJobs[binIdx], &mRects[binIdx]);

						mRenderQueue->AdvanceRenderJob(binIdx);
					}
					continue;
				}
			}

			// No work available: Yield this thread
			std::this_thread::yield();
			threadIsIdle = true;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Culling threadpool public API, similar to the MaskedOcclusionCulling class
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CullingThreadpool::CullingThreadpool(unsigned int numThreads, unsigned int binsW, unsigned int binsH, unsigned int maxJobs) :
	mNumThreads(numThreads),
	mMaxJobs(maxJobs),
	mBinsW(binsW),
	mBinsH(binsH),
	mKillThreads(false),
	mSuspendThreads(true),
	mNumSuspendedThreads(0),
	mModelToClipMatrices(maxJobs),
	mVertexLayouts(maxJobs),
	mMOC(nullptr)
{
	mNumBins = mBinsW*mBinsH;
	assert(mNumBins >= mNumThreads);	// Having less bins than threads is a bad idea!

	mRects = new ScissorRect[mNumBins];
	mRenderQueue = new RenderJobQueue(mNumBins, mMaxJobs);

	// Add default vertex layout and matrix
	mVertexLayouts.AddData(VertexLayout(16, 4, 12));
	mCurrentMatrix = nullptr;

	mThreads = new std::thread[mNumThreads];
	for (unsigned int i = 0; i < mNumThreads; ++i)
		mThreads[i] = std::thread(ThreadRun, this, i);

}

CullingThreadpool::~CullingThreadpool()
{
	// Wait for threads to terminate
	if (mThreads != nullptr || !mKillThreads)
	{
		WakeThreads();
		mKillThreads = true;
		for (unsigned int i = 0; i < mNumThreads; ++i)
			mThreads[i].join();

	}

	// Free memory
	SAFE_DELETE(mRenderQueue);
	SAFE_DELETE_ARRAY(mRects);
	SAFE_DELETE_ARRAY(mThreads);
}

void CullingThreadpool::WakeThreads()
{
	// Wait for all threads to be in suspended mode
	while (mNumSuspendedThreads < mNumThreads)
		std::this_thread::yield();

	// Send wake up event
	std::unique_lock<std::mutex> lock(mSuspendedMutex);
	mSuspendThreads = false;
	lock.unlock();
	mSuspendedCV.notify_all();
}

void CullingThreadpool::SuspendThreads()
{
	// Signal threads to go into suspended mode (after finishing all outstanding work)
	mSuspendThreads = true;
}

void CullingThreadpool::Flush()
{
	// _Claude_ Diagnostic for MWSE freeze investigation. Flush() can spin
	// forever on IsPipelineEmpty() if the renderqueue gets into a bad
	// state (stuck mBinMutexes[i], missed visibility on mWritePtr, etc.)
	// This is the prime suspect for the user-reported stage=3(clearBuffer)
	// 30-second freezes at 98% CPU. After kFlushStuckThresholdMs of
	// continuous spinning, write a one-shot snapshot of the queue's
	// internal state to MSOC.flushdump.txt (truncate mode, alongside
	// MSOC.forensics.txt) so we have actual data to diagnose with rather
	// than guesses. The spin continues afterward — this is observation
	// only, no behavioural change.
	constexpr long long kFlushStuckThresholdMs = 2000;
	const auto spinStart = std::chrono::steady_clock::now();
	bool dumped = false;

	while (!mRenderQueue->IsPipelineEmpty())
	{
		std::this_thread::yield();

		if (!dumped)
		{
			const auto now = std::chrono::steady_clock::now();
			const auto spinMs = std::chrono::duration_cast<std::chrono::milliseconds>(
				now - spinStart).count();
			if (spinMs >= kFlushStuckThresholdMs)
			{
				dumped = true;
				try
				{
					std::ofstream f("MSOC.flushdump.txt", std::ios::trunc);
					if (f)
					{
						const auto epochMs = std::chrono::duration_cast<std::chrono::milliseconds>(
							now.time_since_epoch()).count();
						f << "flush_dump_at_ms=" << epochMs
							<< " spin_ms=" << spinMs << "\n";
						f << "mWritePtr=" << mRenderQueue->mWritePtr
							<< " mBinningPtr=" << mRenderQueue->mBinningPtr.load()
							<< " mNumBins=" << mRenderQueue->mNumBins
							<< " mMaxJobs=" << mRenderQueue->mMaxJobs << "\n";
						f << "mSuspendThreads=" << (mSuspendThreads ? 1 : 0)
							<< " mNumSuspendedThreads=" << mNumSuspendedThreads
							<< "/" << mNumThreads
							<< " mKillThreads=" << (mKillThreads ? 1 : 0) << "\n";
						f << "mRenderPtrs=";
						for (unsigned int i = 0; i < mRenderQueue->mNumBins; ++i)
							f << " " << mRenderQueue->mRenderPtrs[i].load();
						f << "\n";
						f << "mBinMutexes=";
						for (unsigned int i = 0; i < mRenderQueue->mNumBins; ++i)
							f << " " << mRenderQueue->mBinMutexes[i].load();
						f << "\n";
						for (unsigned int i = 0; i < mRenderQueue->mMaxJobs; ++i)
						{
							f << "mJobs[" << i << "].started="
								<< (int)mRenderQueue->mJobs[i].mBinningJobStartedIdx
								<< " .completed="
								<< (int)mRenderQueue->mJobs[i].mBinningJobCompletedIdx
								<< "\n";
						}
					}
				}
				catch (...)
				{
					// Diagnostic only — never let dump failure escalate
					// into a real bug.
				}
			}
		}
	}

	// _Claude_ Suspend workers fully before Reset so they cannot race with
	// Reset's writes. The cae2593f76 reordering (poison mJobs first) was
	// inadequate on its own — even with the correct atomic ordering on
	// the fields (now in place), workers in the inner loop can pick up
	// stale snapshots between Reset writes if they observe the writes
	// out of order, then call AdvanceRenderJob and leave mRenderPtrs
	// above mWritePtr. The cv-based suspension establishes a release/
	// acquire edge: Reset's writes (after suspend) happen-before the
	// next round of worker reads (after wake). Polling waits for
	// mNumSuspendedThreads == mNumThreads so the post-Reset Wake reaches
	// only parked workers.
	//
	// Cost per Flush: one cv broadcast + one wake spin (≤ a few µs since
	// workers are already idle by the time the spin loop above exits).
	// Saves the entire freeze failure mode, which is unbounded.
	//
	// If workers were already suspended (defensive — not the normal
	// caller pattern), don't wake them at the end so the caller's state
	// expectation is preserved.
	const bool wasAlreadySuspended = mSuspendThreads.load();
	if (!wasAlreadySuspended) {
		SuspendThreads();
	}
	while (mNumSuspendedThreads < mNumThreads) {
		std::this_thread::yield();
	}

	// Reset queue counters — race-free now: every worker is parked in
	// mSuspendedCV.wait, so they cannot read mWritePtr / mJobs / etc.
	// concurrent with the writes below.
	mRenderQueue->Reset();

	if (!wasAlreadySuspended) {
		WakeThreads();
	}
}

void CullingThreadpool::SetBuffer(MaskedOcclusionCulling *moc)
{
	Flush();
	mMOC = moc;
	SetupScissors();
}

void CullingThreadpool::SetResolution(unsigned int width, unsigned int height)
{
	Flush();
	mMOC->SetResolution(width, height);
	SetupScissors();
}

void CullingThreadpool::SetNearClipPlane(float nearDist)
{
	Flush();
	mMOC->SetNearClipPlane(nearDist);
}

void CullingThreadpool::SetMatrix(const float *modelToClipMatrix)
{
	// Treat nullptr matrix as a special case, otherwise copy the contents of the pointer and add to state
	if (modelToClipMatrix == nullptr)
		mCurrentMatrix = nullptr;
	else
	{
		mModelToClipMatrices.AddData(Matrix4x4(modelToClipMatrix));
		mCurrentMatrix = mModelToClipMatrices.GetData()->mValues;
	}
}

void CullingThreadpool::SetVertexLayout(const VertexLayout &vtxLayout)
{
	mVertexLayouts.AddData(vtxLayout);
}

void CullingThreadpool::ClearBuffer()
{
	Flush();
	mMOC->ClearBuffer();
}

void CullingThreadpool::RenderTriangles(const float *inVtx, const unsigned int *inTris, int nTris, BackfaceWinding bfWinding, ClipPlanes clipPlaneMask)
{
#if MOC_RECORDER_ENABLE != 0
    mMOC->RecordRenderTriangles( inVtx, inTris, nTris, mCurrentMatrix, clipPlaneMask, bfWinding, *mVertexLayouts.GetData( ) );
#endif

    for (int i = 0; i < nTris; i += TRIS_PER_JOB)
	{
		// Yield if work queue is full 
		while (!mRenderQueue->CanWrite())
			std::this_thread::yield();

		// Create new renderjob
		RenderJobQueue::Job *job = mRenderQueue->GetWriteJob();
		job->mBinningJob.mVerts = inVtx;
		job->mBinningJob.mTris = inTris + i * 3;
		job->mBinningJob.nTris = nTris - i < TRIS_PER_JOB ? nTris - i : TRIS_PER_JOB;
		job->mBinningJob.mMatrix = mCurrentMatrix;
		job->mBinningJob.mClipPlanes = clipPlaneMask;
		job->mBinningJob.mBfWinding = bfWinding;
		job->mBinningJob.mVtxLayout = mVertexLayouts.GetData();
		mRenderQueue->AdvanceWriteJob();
	}
}

CullingThreadpool::CullingResult CullingThreadpool::TestRect(float xmin, float ymin, float xmax, float ymax, float wmin)
{
	return mMOC->TestRect(xmin, ymin, xmax, ymax, wmin);
}

CullingThreadpool::CullingResult CullingThreadpool::TestTriangles(const float *inVtx, const unsigned int *inTris, int nTris, BackfaceWinding bfWinding, ClipPlanes clipPlaneMask)
{
	return mMOC->TestTriangles(inVtx, inTris, nTris, mCurrentMatrix, bfWinding, clipPlaneMask, *mVertexLayouts.GetData());
}

void CullingThreadpool::ComputePixelDepthBuffer(float *depthData, bool flipY)
{
	Flush();
	mMOC->ComputePixelDepthBuffer(depthData, flipY);
}
