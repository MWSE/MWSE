#pragma once

#include <cstddef>
#include <cstdint>

// Masked Software Occlusion Culling.
//
// Based on Hasselgren, Andersson, Akenine-Moller, "Masked Software Occlusion
// Culling" (HPG 2016). Single-threaded SSE2 implementation; no binning
// rasterizer, no PRECISE_COVERAGE, no merge-two-HiZ.
//
// Depth convention: stored as 1/w (post-projection reciprocal clip-w).
// Larger value = nearer. All comparisons inside assume this convention.
//
// Framebuffer must be a multiple of the 8x4 tile. Width%8 == 0, height%4 == 0.
// Per-tile storage: { float hiZ0, float hiZ1, uint32_t mask } = 12 bytes.

namespace mwse::moc {

	enum class CullingResult : int {
		VISIBLE = 0,
		OCCLUDED = 1,
		VIEW_CULLED = 2,
	};

	class MaskedOcclusionCulling {
	public:
		MaskedOcclusionCulling();
		~MaskedOcclusionCulling();

		MaskedOcclusionCulling(const MaskedOcclusionCulling&) = delete;
		MaskedOcclusionCulling& operator=(const MaskedOcclusionCulling&) = delete;

		// Reallocate tile storage. Width rounded up to 8, height rounded up to 4.
		void SetResolution(unsigned int width, unsigned int height);
		unsigned int GetWidth() const { return mWidth; }
		unsigned int GetHeight() const { return mHeight; }

		// Reset every tile to the far plane.
		void ClearBuffer();

		// Set row-major 4x4 world-to-clip transform (= proj * worldToCam).
		void SetWorldToClip(const float worldToClip[16]);

		// Rasterize an occluder. Vertex data is interleaved floats at 'stride'
		// bytes apart, first 3 floats of each entry are the world-space
		// position. Indices are 16-bit, triangle-list only.
		void RenderTriangles(const void* vertexData, size_t vertexStride,
			unsigned int vertexCount, const uint16_t* indices,
			unsigned int triangleCount);

		// Query a world-space bounding sphere. Conservative: may return VISIBLE
		// for truly occluded spheres, will never return OCCLUDED for a visible
		// one. Returns VIEW_CULLED when the sphere lies fully outside the
		// frustum (behind the near plane or entirely off-screen).
		CullingResult TestSphere(const float center[3], float radius) const;

		// Diagnostics. Cheap to call; intended for debug UI / logs.
		struct Stats {
			uint64_t occluderTrianglesSubmitted;
			uint64_t occluderTrianglesRasterized;
			uint64_t sphereTestsTotal;
			uint64_t sphereTestsOccluded;
			uint64_t sphereTestsViewCulled;
		};
		const Stats& GetStats() const { return mStats; }
		void ResetStats();

		// Number of tiles with any recorded occluder coverage (hiZ0 > 0).
		// Call after RenderTriangles to verify the rasterizer is actually
		// writing the buffer.
		unsigned int CountCoveredTiles() const;

	private:
		struct Tile {
			float hiZ0;
			float hiZ1;
			uint32_t mask;
		};

		// Tile grid dimensions. mTilesX = width/8, mTilesY = height/4.
		unsigned int mWidth;
		unsigned int mHeight;
		unsigned int mTilesX;
		unsigned int mTilesY;
		Tile* mTiles;

		// Cached world-to-clip, row-major (so clip = matrix * column-vector).
		float mWorldToClip[16];

		// Screen-space scale/offset: x_screen = (x_ndc * +sx) + ox, etc.
		// In pixels. ox/oy already include the 0.5*width/height centering plus
		// any half-pixel alignment needed by the rasterizer's fixed-point step.
		float mScreenScaleX;
		float mScreenScaleY;
		float mScreenOffsetX;
		float mScreenOffsetY;

		mutable Stats mStats;

		// Helpers.
		void projectAndRasterize(const float v0World[3], const float v1World[3], const float v2World[3]);
	};

}
