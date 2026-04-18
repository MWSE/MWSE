#include "MaskedOcclusionCulling.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>
#include <new>

namespace mwse::moc {

	namespace {
		constexpr unsigned int kTileWidth = 8;
		constexpr unsigned int kTileHeight = 4;
		constexpr unsigned int kPixelsPerTile = kTileWidth * kTileHeight;
		constexpr float kNearWEpsilon = 1e-4f;

		inline unsigned int roundUp(unsigned int v, unsigned int step) {
			return (v + step - 1) & ~(step - 1);
		}

		struct Vec4 {
			float x, y, z, w;
		};

		inline Vec4 transformRow(const float m[16], const float v[3]) {
			Vec4 r;
			r.x = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];
			r.y = m[4] * v[0] + m[5] * v[1] + m[6] * v[2] + m[7];
			r.z = m[8] * v[0] + m[9] * v[1] + m[10] * v[2] + m[11];
			r.w = m[12] * v[0] + m[13] * v[1] + m[14] * v[2] + m[15];
			return r;
		}
	}

	MaskedOcclusionCulling::MaskedOcclusionCulling()
		: mWidth(0), mHeight(0), mTilesX(0), mTilesY(0), mTiles(nullptr),
		mScreenScaleX(0.0f), mScreenScaleY(0.0f),
		mScreenOffsetX(0.0f), mScreenOffsetY(0.0f),
		mStats{} {
		for (int i = 0; i < 16; ++i) {
			mWorldToClip[i] = (i % 5 == 0) ? 1.0f : 0.0f;
		}
	}

	MaskedOcclusionCulling::~MaskedOcclusionCulling() {
		delete[] mTiles;
	}

	void MaskedOcclusionCulling::SetResolution(unsigned int width, unsigned int height) {
		const unsigned int w = roundUp(width, kTileWidth);
		const unsigned int h = roundUp(height, kTileHeight);
		if (w == mWidth && h == mHeight && mTiles) {
			return;
		}

		delete[] mTiles;

		mWidth = w;
		mHeight = h;
		mTilesX = w / kTileWidth;
		mTilesY = h / kTileHeight;
		const size_t tileCount = size_t(mTilesX) * size_t(mTilesY);
		mTiles = new Tile[tileCount];

		ClearBuffer();

		mScreenScaleX = 0.5f * float(mWidth);
		mScreenScaleY = 0.5f * float(mHeight);
		mScreenOffsetX = 0.5f * float(mWidth);
		mScreenOffsetY = 0.5f * float(mHeight);
	}

	void MaskedOcclusionCulling::ClearBuffer() {
		if (!mTiles) return;
		const size_t tileCount = size_t(mTilesX) * size_t(mTilesY);
		for (size_t i = 0; i < tileCount; ++i) {
			mTiles[i].hiZ0 = 0.0f;
			mTiles[i].hiZ1 = 0.0f;
			mTiles[i].mask = 0;
		}
	}

	void MaskedOcclusionCulling::SetWorldToClip(const float worldToClip[16]) {
		std::memcpy(mWorldToClip, worldToClip, sizeof(mWorldToClip));
	}

	void MaskedOcclusionCulling::ResetStats() {
		mStats = Stats{};
	}

	namespace {

		inline float edgeFn(float ax, float ay, float bx, float by, float px, float py) {
			return (bx - ax) * (py - ay) - (by - ay) * (px - ax);
		}

		struct ProjVert {
			float x, y;
			float invW;
			bool clippedOut;
		};

		// Project one world-space vertex to pixel coordinates. Returns
		// clippedOut=true if the vertex is behind the near plane or has
		// non-finite w (triangle must be rejected).
		ProjVert projectVertex(const float worldToClip[16], const float v[3],
			float sx, float sy, float ox, float oy) {
			ProjVert r{};
			const Vec4 clip = transformRow(worldToClip, v);
			if (!(clip.w > kNearWEpsilon)) {
				r.clippedOut = true;
				return r;
			}
			const float invW = 1.0f / clip.w;
			r.x = clip.x * invW * sx + ox;
			r.y = -clip.y * invW * sy + oy;
			r.invW = invW;
			r.clippedOut = false;
			return r;
		}

	}

	void MaskedOcclusionCulling::RenderTriangles(const void* vertexData,
		size_t vertexStride, unsigned int vertexCount, const uint16_t* indices,
		unsigned int triangleCount) {
		if (!mTiles || triangleCount == 0) return;

		const uint8_t* base = static_cast<const uint8_t*>(vertexData);
		for (unsigned int t = 0; t < triangleCount; ++t) {
			const uint16_t i0 = indices[t * 3 + 0];
			const uint16_t i1 = indices[t * 3 + 1];
			const uint16_t i2 = indices[t * 3 + 2];
			if (i0 >= vertexCount || i1 >= vertexCount || i2 >= vertexCount) continue;

			const float* p0 = reinterpret_cast<const float*>(base + i0 * vertexStride);
			const float* p1 = reinterpret_cast<const float*>(base + i1 * vertexStride);
			const float* p2 = reinterpret_cast<const float*>(base + i2 * vertexStride);
			mStats.occluderTrianglesSubmitted++;
			projectAndRasterize(p0, p1, p2);
		}
	}

	void MaskedOcclusionCulling::projectAndRasterize(const float v0World[3], const float v1World[3], const float v2World[3]) {
		const ProjVert v0 = projectVertex(mWorldToClip, v0World, mScreenScaleX, mScreenScaleY, mScreenOffsetX, mScreenOffsetY);
		const ProjVert v1 = projectVertex(mWorldToClip, v1World, mScreenScaleX, mScreenScaleY, mScreenOffsetX, mScreenOffsetY);
		const ProjVert v2 = projectVertex(mWorldToClip, v2World, mScreenScaleX, mScreenScaleY, mScreenOffsetX, mScreenOffsetY);
		if (v0.clippedOut || v1.clippedOut || v2.clippedOut) return;

		const float area = edgeFn(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y);
		if (!(std::fabs(area) > 1e-6f)) return;

		const ProjVert* a = &v0;
		const ProjVert* b = &v1;
		const ProjVert* c = &v2;
		if (area < 0.0f) {
			std::swap(b, c);
		}

		const float invArea = 1.0f / std::fabs(area);
		const float invW0 = a->invW;
		const float invW1 = b->invW;
		const float invW2 = c->invW;

		float minX = std::min(a->x, std::min(b->x, c->x));
		float maxX = std::max(a->x, std::max(b->x, c->x));
		float minY = std::min(a->y, std::min(b->y, c->y));
		float maxY = std::max(a->y, std::max(b->y, c->y));

		const float fbMaxX = float(mWidth);
		const float fbMaxY = float(mHeight);
		if (maxX < 0.0f || minX > fbMaxX) return;
		if (maxY < 0.0f || minY > fbMaxY) return;

		minX = std::max(minX, 0.0f);
		minY = std::max(minY, 0.0f);
		maxX = std::min(maxX, fbMaxX);
		maxY = std::min(maxY, fbMaxY);

		const int pxMinX = int(std::floor(minX));
		const int pxMinY = int(std::floor(minY));
		const int pxMaxX = int(std::ceil(maxX));
		const int pxMaxY = int(std::ceil(maxY));

		const int tileMinX = std::max(0, pxMinX / int(kTileWidth));
		const int tileMaxX = std::min(int(mTilesX) - 1, (pxMaxX - 1) / int(kTileWidth));
		const int tileMinY = std::max(0, pxMinY / int(kTileHeight));
		const int tileMaxY = std::min(int(mTilesY) - 1, (pxMaxY - 1) / int(kTileHeight));
		if (tileMinX > tileMaxX || tileMinY > tileMaxY) return;

		mStats.occluderTrianglesRasterized++;

		for (int ty = tileMinY; ty <= tileMaxY; ++ty) {
			for (int tx = tileMinX; tx <= tileMaxX; ++tx) {
				Tile& tile = mTiles[ty * int(mTilesX) + tx];

				uint32_t triCov = 0;
				float triMinInvW = std::numeric_limits<float>::infinity();

				for (unsigned int py = 0; py < kTileHeight; ++py) {
					const float sampleY = float(ty * int(kTileHeight) + int(py)) + 0.5f;
					for (unsigned int px = 0; px < kTileWidth; ++px) {
						const float sampleX = float(tx * int(kTileWidth) + int(px)) + 0.5f;
						const float e0 = edgeFn(b->x, b->y, c->x, c->y, sampleX, sampleY);
						const float e1 = edgeFn(c->x, c->y, a->x, a->y, sampleX, sampleY);
						const float e2 = edgeFn(a->x, a->y, b->x, b->y, sampleX, sampleY);
						if (e0 < 0.0f || e1 < 0.0f || e2 < 0.0f) continue;

						const float w0 = e0 * invArea;
						const float w1 = e1 * invArea;
						const float w2 = e2 * invArea;
						const float sampleInvW = w0 * invW0 + w1 * invW1 + w2 * invW2;

						triCov |= (1u << (py * kTileWidth + px));
						if (sampleInvW < triMinInvW) triMinInvW = sampleInvW;
					}
				}

				if (triCov == 0) continue;

				const uint32_t newMask = tile.mask | triCov;
				if (newMask == 0xFFFFFFFFu) {
					const float promotedFar = std::min(tile.hiZ1, triMinInvW);
					tile.hiZ0 = promotedFar;
					tile.hiZ1 = 0.0f;
					tile.mask = 0;
				}
				else {
					tile.hiZ1 = std::max(tile.hiZ1, triMinInvW);
					tile.mask = newMask;
				}
			}
		}
	}

	CullingResult MaskedOcclusionCulling::TestSphere(const float center[3], float radius) const {
		if (!mTiles) return CullingResult::VISIBLE;
		mStats.sphereTestsTotal++;

		const Vec4 c = transformRow(mWorldToClip, center);

		const float nearBound = kNearWEpsilon;
		if (c.w + radius <= nearBound) {
			mStats.sphereTestsViewCulled++;
			return CullingResult::VIEW_CULLED;
		}
		if (c.w - radius <= nearBound) {
			return CullingResult::VISIBLE;
		}

		const float cx = c.x;
		const float cy = c.y;
		const float cz = c.z;
		const float cw = c.w;

		(void)cz;

		const float rx = radius;
		const float ry = radius;

		const float minClipX = cx - rx;
		const float maxClipX = cx + rx;
		const float minClipY = cy - ry;
		const float maxClipY = cy + ry;
		const float minClipW = cw - radius;
		const float maxClipW = cw + radius;

		if (maxClipX < -maxClipW) { mStats.sphereTestsViewCulled++; return CullingResult::VIEW_CULLED; }
		if (minClipX >  maxClipW) { mStats.sphereTestsViewCulled++; return CullingResult::VIEW_CULLED; }
		if (maxClipY < -maxClipW) { mStats.sphereTestsViewCulled++; return CullingResult::VIEW_CULLED; }
		if (minClipY >  maxClipW) { mStats.sphereTestsViewCulled++; return CullingResult::VIEW_CULLED; }

		const float invWNear = 1.0f / minClipW;
		const float ndcMinX = minClipX / maxClipW;
		const float ndcMaxX = maxClipX / maxClipW;
		const float ndcMinY = minClipY / maxClipW;
		const float ndcMaxY = maxClipY / maxClipW;

		const float screenMinX = ndcMinX * mScreenScaleX + mScreenOffsetX;
		const float screenMaxX = ndcMaxX * mScreenScaleX + mScreenOffsetX;
		const float screenMinY = -ndcMaxY * mScreenScaleY + mScreenOffsetY;
		const float screenMaxY = -ndcMinY * mScreenScaleY + mScreenOffsetY;

		if (screenMaxX < 0.0f || screenMinX > float(mWidth) ||
			screenMaxY < 0.0f || screenMinY > float(mHeight)) {
			mStats.sphereTestsViewCulled++;
			return CullingResult::VIEW_CULLED;
		}

		const int pxMinX = std::max(0, int(std::floor(screenMinX)));
		const int pxMinY = std::max(0, int(std::floor(screenMinY)));
		const int pxMaxX = std::min(int(mWidth) - 1, int(std::ceil(screenMaxX)));
		const int pxMaxY = std::min(int(mHeight) - 1, int(std::ceil(screenMaxY)));

		const int tileMinX = pxMinX / int(kTileWidth);
		const int tileMinY = pxMinY / int(kTileHeight);
		const int tileMaxX = pxMaxX / int(kTileWidth);
		const int tileMaxY = pxMaxY / int(kTileHeight);

		const float queryInvW = invWNear;

		for (int ty = tileMinY; ty <= tileMaxY; ++ty) {
			for (int tx = tileMinX; tx <= tileMaxX; ++tx) {
				const Tile& tile = mTiles[ty * int(mTilesX) + tx];
				if (queryInvW > tile.hiZ0) {
					return CullingResult::VISIBLE;
				}
			}
		}

		mStats.sphereTestsOccluded++;
		return CullingResult::OCCLUDED;
	}

}
