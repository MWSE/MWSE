#include "NITriBasedGeometry.h"

#include "NIBound.h"
#include "NIPick.h"
#include "NISkinInstance.h"
#include "NITransform.h"

#include "ExceptionUtil.h"
#include "MemoryUtil.h"

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
#include "MWSEConfig.h"
#include "NIPickBVH.h"
#endif

namespace NI {
	TriBasedGeometry::TriBasedGeometry(TriBasedGeometryData* data) {
#if defined(SE_NI_TRIBASEDGEOMETRY_FNADDR_CTORFROMDATA) && SE_NI_TRIBASEDGEOMETRY_FNADDR_CTORFROMDATA > 0
		const auto NI_TriBasedGeometry_ctorFromData = reinterpret_cast<void(__thiscall*)(TriBasedGeometry*, TriBasedGeometryData*)>(SE_NI_TRIBASEDGEOMETRY_FNADDR_CTORFROMDATA);
		NI_TriBasedGeometry_ctorFromData(this, data);
#else
		throw not_implemented_exception();
#endif
	}

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
	// Engine-state globals (Morrowind.exe). Direct address access avoids
	// dragging se::memory::ExternalGlobal into SharedSE.
	static inline Point2& gPickDefaultTextureCoords() { return *reinterpret_cast<Point2*>(0x7DED80); }
	static inline PackedColor& gPickDefaultColor() { return *reinterpret_cast<PackedColor*>(0x7DE814); }

	static bool __cdecl FindIntersectRayWithTriangle(const Point3* position, const Point3* direction, const Point3* vertex1, const Point3* vertex2, const Point3* vertex3, bool frontOnly, Point3* out_intersection, float* out_distance, float* out_weight2, float* out_weight3) {
#if defined(SE_NI_FNADDR_FINDINTERSECTRAYWITHTRIANGLE) && SE_NI_FNADDR_FINDINTERSECTRAYWITHTRIANGLE > 0
		const auto NI_FindIntersectRayWithTriangle = reinterpret_cast<bool(__cdecl*)(const Point3*, const Point3*, const Point3*, const Point3*, const Point3*, bool, Point3*, float*, float*, float*)>(SE_NI_FNADDR_FINDINTERSECTRAYWITHTRIANGLE);
		return NI_FindIntersectRayWithTriangle(position, direction, vertex1, vertex2, vertex3, frontOnly, out_intersection, out_distance, out_weight2, out_weight3);
#else
		throw not_implemented_exception();
#endif
	}

	static std::vector<Point3> deformVertices;
	static std::vector<Point3> deformNormals;
#endif

	bool TriBasedGeometry::findIntersections(const Point3* position, const Point3* direction, Pick* pick) {
#if defined(SE_NI_TRIBASEDGEOMETRY_FNADDR_FINDINTERSECTIONS) && SE_NI_TRIBASEDGEOMETRY_FNADDR_FINDINTERSECTIONS > 0
		const auto NI_TriBasedGeometry_findIntersections = reinterpret_cast<bool(__thiscall*)(TriBasedGeometry*, const Point3*, const Point3*, Pick*)>(SE_NI_TRIBASEDGEOMETRY_FNADDR_FINDINTERSECTIONS);

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
		// Allow the MCM configuration options to disable this logic entirely and fall back to vanilla behavior.
		// This will be removed in the future when the config options are removed, once we're confident of performance and accuracy.
		// Note that tes3.rayTest turns UseSkinnedAccurateActivationRaytests off for the duration of the pick
		// unless its accurateSkinned parameter is set, so this path must not depend on that flag alone.
		const auto useAccurateSkinnedRaytests = mwse::Configuration::UseSkinnedAccurateActivationRaytests;
		if (!useAccurateSkinnedRaytests && !mwse::Configuration::UseBVHAcceleratedRaytests) {
			return NI_TriBasedGeometry_findIntersections(this, position, direction, pick);
		}

		// Skinned geometry semantics depend on the accurate-skinned option. Without it, vanilla
		// must handle skinned objects (bound-hit behavior via pickIgnoresSkinInstances).
		if (skinInstance && !useAccurateSkinnedRaytests) {
			return NI_TriBasedGeometry_findIntersections(this, position, direction, pick);
		}

		// Ignore if we don't care about culled geometry.
		if (pick->observeAppCullFlag && getAppCulled()) {
			return false;
		}

		// Check against intersection bounds first.
		auto boundsDistance = 0.0f;
		if (!intersectBounds(position, direction, &boundsDistance)) {
			return false;
		}

		// If we are just a bounds-based pick, we're basically done.
		if (pick->intersectType != PickIntersectType::TRIANGLE_INTERSECT) {
			auto result = pick->addRecord();
			result->object = this;
			result->distance = boundsDistance;
			return true;
		}

		const auto modelData = getModelData();
		auto vertices = modelData->vertex;
		auto normals = modelData->normal;
		const auto triList = modelData->getTriList();
		const auto activeTriCount = modelData->getActiveTriangleCount();
		if (!vertices || !triList || activeTriCount == 0) {
			return false;
		}

		// Deform vertices if we're looking at a skinned object. This isn't thread-safe which shouldn't be a problem.
		if (skinInstance) {
			const auto vertexCount = modelData->getActiveVertexCount();
			deformVertices.reserve(vertexCount);
			deformNormals.reserve(vertexCount);
			vertices = deformVertices.data();
			normals = deformNormals.data();
			skinInstance->deform(modelData->vertex, modelData->normal, vertexCount, vertices, normals);
		}

		// Calculate our base position/direction for non-uniform scaling.
		const auto inverseScale = 1.0f / worldTransform.scale;
		const auto worldRotationInverse = worldTransform.rotation.invert() * inverseScale;
		const auto worldScaled = worldRotationInverse * (*position - worldTransform.translation);
		const auto directionScaled = worldRotationInverse * (*direction);

		// Ask the BVH cache which triangles the ray can actually reach. Skinned
		// geometry deforms per query, so it keeps the exhaustive loop.
		const std::vector<unsigned int>* candidates = nullptr;
		if (!skinInstance && mwse::Configuration::UseBVHAcceleratedRaytests) {
			candidates = mwse::pickbvh::getCandidateTriangles(modelData.get(), triList, activeTriCount, vertices, modelData->vertexCount, worldScaled, directionScaled);
		}

		// Loop through the candidate triangles, or all of them without a BVH.
		auto addedResult = false;
		const auto testCount = candidates ? static_cast<unsigned int>(candidates->size()) : static_cast<unsigned int>(activeTriCount);
		for (auto k = 0u; k < testCount; ++k) {
			// Get some shorthand variables we'll use throughout.
			const auto i = candidates ? (*candidates)[k] : k;
			const auto& triangle = triList[i];
			const auto index1 = triangle.vertices[0];
			const auto index2 = triangle.vertices[1];
			const auto index3 = triangle.vertices[2];
			const auto vertex1 = &vertices[index1];
			const auto vertex2 = &vertices[index2];
			const auto vertex3 = &vertices[index3];

			// Perform our test for the triangle, and calculate the weight to each index.
			auto distance = std::numeric_limits<float>::infinity();
			Point3 intersection;
			float weight2, weight3;
			if (!FindIntersectRayWithTriangle(&worldScaled, &directionScaled, vertex1, vertex2, vertex3, pick->frontOnly, &intersection, &distance, &weight2, &weight3)) {
				continue;
			}

			// The above function only calculated the weight of the 2nd and 3rd vertex, so we need to get the final weight.
			const auto weight1 = 1.0f - weight2 - weight3;

			// At this point we know we have a valid result and can start allocating memory for it.
			addedResult = true;
			const auto result = pick->addRecord();
			result->object = this;
			result->triangleIndex = i;
			result->vertexIndex[0] = index1;
			result->vertexIndex[1] = index2;
			result->vertexIndex[2] = index3;
			result->distance = distance;

			// Calculate intersection.
			if (pick->coordinateType == PickCoordinateType::WORLD_COORDINATES) {
				result->intersection = worldTransform * intersection;
			}
			else {
				result->intersection = intersection;
			}

			// Calculate weighted texture coordinates.
			const auto textureCoords = modelData->textureCoords;
			if (pick->returnTexture && textureCoords) {
				result->texture = textureCoords[index1] * weight1 + textureCoords[index2] * weight2 + textureCoords[index3] * weight3;
			}
			else {
				result->texture = gPickDefaultTextureCoords();
			}

			// Calculate weighted normals.
			if (pick->returnNormal) {
				Point3 normal = {};
				if (pick->returnSmoothNormal && normals) {
					normal = normals[index1] * weight1 + normals[index2] * weight2 + normals[index3] * weight3;
				}
				else {
					const auto vertex3m1 = (*vertex3 - *vertex1);
					normal = (*vertex2 - *vertex1).crossProduct(&vertex3m1);
				}

				normal.normalize();

				if (pick->coordinateType == PickCoordinateType::WORLD_COORDINATES) {
					result->normal = worldTransform.rotation * normal;
				}
				else {
					result->normal = normal;
				}
			}

			// Calculate weighted vertex colors.
			const auto colors = modelData->color;
			if (pick->returnColor && colors) {
				// Vanilla can overflow or make less accurate color calculations. We don't care to follow that behavior and will be a bit more accurate.
				const auto r = unsigned char(float(colors[index1].r) * weight1 + float(colors[index2].r) * weight2 + float(colors[index3].r) * weight3);
				const auto g = unsigned char(float(colors[index1].g) * weight1 + float(colors[index2].g) * weight2 + float(colors[index3].g) * weight3);
				const auto b = unsigned char(float(colors[index1].b) * weight1 + float(colors[index2].b) * weight2 + float(colors[index3].b) * weight3);
				const auto a = unsigned char(float(colors[index1].a) * weight1 + float(colors[index2].a) * weight2 + float(colors[index3].a) * weight3);
				result->color = PackedColor(r, g, b, a);
			}
			else {
				result->color = gPickDefaultColor();
			}

			// We can be finished if we just want the first unsorted result.
			if (pick->pickType == PickType::FIND_FIRST && pick->sortType == PickSortType::NO_SORT) {
				break;
			}
		}

		return addedResult;
#else
		// Non-MWSE targets fall through to engine vanilla behavior.
		return NI_TriBasedGeometry_findIntersections(this, position, direction, pick);
#endif
#else
		throw not_implemented_exception();
#endif
	}

	Pointer<TriBasedGeometryData> TriBasedGeometry::getModelData() const {
		return static_cast<TriBasedGeometryData*>(modelData.get());
	}

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
	// Conservative world AABB of an alternate bounding volume. Returns false for
	// volume types we don't handle; the caller then falls back to vanilla.
	static bool computeAbvWorldAabb(const BoundingVolume* abv, Point3& out_min, Point3& out_max) {
		switch (abv->getType()) {
		case BoundingVolumeType::Sphere:
		{
			const auto& sphere = static_cast<const SphereBoundingVolume*>(abv)->bounds;
			out_min = { sphere.center.x - sphere.radius, sphere.center.y - sphere.radius, sphere.center.z - sphere.radius };
			out_max = { sphere.center.x + sphere.radius, sphere.center.y + sphere.radius, sphere.center.z + sphere.radius };
			return true;
		}
		case BoundingVolumeType::Box:
		{
			const auto& box = static_cast<const BoxBoundingVolume*>(abv)->bounds;
			// The basis holds the box axes. Take the larger of the row-vector and
			// column-vector interpretations per world axis so the result bounds
			// the box regardless of storage convention.
			const auto& m = box.basis;
			const auto& e = box.extent;
			const Point3 rows = {
				std::fabs(m.m0.x) * e.x + std::fabs(m.m1.x) * e.y + std::fabs(m.m2.x) * e.z,
				std::fabs(m.m0.y) * e.x + std::fabs(m.m1.y) * e.y + std::fabs(m.m2.y) * e.z,
				std::fabs(m.m0.z) * e.x + std::fabs(m.m1.z) * e.y + std::fabs(m.m2.z) * e.z,
			};
			const Point3 cols = {
				std::fabs(m.m0.x) * e.x + std::fabs(m.m0.y) * e.y + std::fabs(m.m0.z) * e.z,
				std::fabs(m.m1.x) * e.x + std::fabs(m.m1.y) * e.y + std::fabs(m.m1.z) * e.z,
				std::fabs(m.m2.x) * e.x + std::fabs(m.m2.y) * e.y + std::fabs(m.m2.z) * e.z,
			};
			const Point3 extent = { std::max(rows.x, cols.x), std::max(rows.y, cols.y), std::max(rows.z, cols.z) };
			out_min = box.center - extent;
			out_max = box.center + extent;
			return true;
		}
		case BoundingVolumeType::Union:
		{
			// Actor volumes are unions of two boxes (body + step box); bound the
			// merge of all handled members. An unhandled member type makes the
			// whole union unhandled.
			const auto& children = static_cast<const UnionBoundingVolume*>(abv)->children;
			auto anyChild = false;
			for (const auto child : children) {
				if (!child) {
					continue;
				}
				Point3 childMin, childMax;
				if (!computeAbvWorldAabb(child, childMin, childMax)) {
					return false;
				}
				if (!anyChild) {
					out_min = childMin;
					out_max = childMax;
					anyChild = true;
				}
				else {
					out_min = { std::min(out_min.x, childMin.x), std::min(out_min.y, childMin.y), std::min(out_min.z, childMin.z) };
					out_max = { std::max(out_max.x, childMax.x), std::max(out_max.y, childMax.y), std::max(out_max.z, childMax.z) };
				}
			}
			return anyChild;
		}
		default:
			return false;
		}
	}
#endif

	int TriBasedGeometry::findCollisionsTriVsABV(float fTime, AVObject* collidee, char bCalcNormals, CollisionGroup::Intersect* intersect) {
#if defined(SE_NI_TRIBASEDGEOMETRY_FNADDR_FINDCOLLISIONSTRIVSABV) && SE_NI_TRIBASEDGEOMETRY_FNADDR_FINDCOLLISIONSTRIVSABV > 0
		const auto NI_TriBasedGeometry_findCollisionsTriVsABV = reinterpret_cast<int(__thiscall*)(TriBasedGeometry*, float, AVObject*, char, CollisionGroup::Intersect*)>(SE_NI_TRIBASEDGEOMETRY_FNADDR_FINDCOLLISIONSTRIVSABV);

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
		// Skinned geometry deforms its world vertices; the model-space BVH does
		// not describe it. Vanilla handles every case the BVH path can't.
		if (!mwse::Configuration::UseBVHAcceleratedCollisions || skinInstance) {
			return NI_TriBasedGeometry_findCollisionsTriVsABV(this, fTime, collidee, bCalcNormals, intersect);
		}

		const auto data = static_cast<TriBasedGeometryData*>(modelData.get());
		const auto worldAbv = collidee ? static_cast<BoundingVolume*>(collidee->worldABV) : nullptr;
		if (!data || !collidee->modelABV || !worldAbv) {
			return NI_TriBasedGeometry_findCollisionsTriVsABV(this, fTime, collidee, bCalcNormals, intersect);
		}

		const auto triList = data->getTriList();
		const auto triangleCount = data->triangleCount;
		if (!triList || !data->vertex || triangleCount == 0 || worldTransform.scale == 0.0f) {
			return NI_TriBasedGeometry_findCollisionsTriVsABV(this, fTime, collidee, bCalcNormals, intersect);
		}

		// Vanilla refreshes the collidee's world volume before testing;
		// FindIntersectBVGeom reads it. This is idempotent, so the vanilla
		// fallbacks below may safely repeat it.
		const auto BV_updateWorldData = reinterpret_cast<void(__thiscall*)(BoundingVolume*, const BoundingVolume*, const Transform*)>(worldAbv->vtbl->updateWorldData);
		BV_updateWorldData(worldAbv, collidee->modelABV, &collidee->worldTransform);

		Point3 worldMin, worldMax;
		if (!computeAbvWorldAabb(worldAbv, worldMin, worldMax)) {
			return NI_TriBasedGeometry_findCollisionsTriVsABV(this, fTime, collidee, bCalcNormals, intersect);
		}

		// Expand by both objects' motion over the tested interval, plus a small
		// margin. Collision callbacks may alter velocities mid-loop, but only to
		// slow or deflect; the initial velocities bound the sweep.
		static const Point3 zeroVelocity(0.0f, 0.0f, 0.0f);
		const auto& colliderVelocity = velocities ? velocities->worldVelocity : zeroVelocity;
		const auto& collideeVelocity = collidee->velocities ? collidee->velocities->worldVelocity : zeroVelocity;
		const Point3 sweep = {
			(std::fabs(colliderVelocity.x) + std::fabs(collideeVelocity.x)) * fTime + 1.0f,
			(std::fabs(colliderVelocity.y) + std::fabs(collideeVelocity.y)) * fTime + 1.0f,
			(std::fabs(colliderVelocity.z) + std::fabs(collideeVelocity.z)) * fTime + 1.0f,
		};
		worldMin = worldMin - sweep;
		worldMax = worldMax + sweep;

		// Transform the world AABB into this geometry's model space, where the
		// BVH lives, by bounding its eight transformed corners.
		const auto inverseScale = 1.0f / worldTransform.scale;
		const auto worldRotationInverse = worldTransform.rotation.invert() * inverseScale;
		Point3 modelMin, modelMax;
		for (auto corner = 0u; corner < 8u; ++corner) {
			const Point3 worldCorner = {
				(corner & 1) ? worldMax.x : worldMin.x,
				(corner & 2) ? worldMax.y : worldMin.y,
				(corner & 4) ? worldMax.z : worldMin.z,
			};
			const auto modelCorner = worldRotationInverse * (worldCorner - worldTransform.translation);
			if (corner == 0) {
				modelMin = modelCorner;
				modelMax = modelCorner;
			}
			else {
				modelMin = { std::min(modelMin.x, modelCorner.x), std::min(modelMin.y, modelCorner.y), std::min(modelMin.z, modelCorner.z) };
				modelMax = { std::max(modelMax.x, modelCorner.x), std::max(modelMax.y, modelCorner.y), std::max(modelMax.z, modelCorner.z) };
			}
		}

		const auto candidates = mwse::pickbvh::getCandidateTrianglesInBox(data, triList, triangleCount, data->vertex, data->vertexCount, modelMin, modelMax);
		if (!candidates) {
			return NI_TriBasedGeometry_findCollisionsTriVsABV(this, fTime, collidee, bCalcNormals, intersect);
		}

		// Vanilla allocates world vertices on demand before its loop.
		if (!worldVertices) {
			vTable.asAVObject->createWorldVertices(this);
			if (!worldVertices) {
				return 0;
			}
		}

		const auto NI_FindIntersectBVGeom = reinterpret_cast<bool(__cdecl*)(float, BoundingVolume*, const Point3*, const Point3*, const Point3*, const Point3*, const Point3*, float*, Point3*, char, Point3*, Point3*)>(SE_NI_FNADDR_FINDINTERSECTBVGEOM);
		const auto NI_AVObject_runCollisionCallbacks = reinterpret_cast<bool(__thiscall*)(AVObject*, CollisionGroup::Intersect*)>(SE_NI_AVOBJECT_FNADDR_RUNCOLLISIONCALLBACKS);

		for (const auto i : *candidates) {
			const auto& triangle = triList[i];

			// Vanilla rereads velocities every iteration; collision callbacks
			// may have changed them.
			const auto loopColliderVelocity = velocities ? &velocities->worldVelocity : &zeroVelocity;
			const auto loopCollideeVelocity = collidee->velocities ? &collidee->velocities->worldVelocity : &zeroVelocity;

			if (NI_FindIntersectBVGeom(fTime, worldAbv, loopCollideeVelocity, &worldVertices[triangle.vertices[0]], &worldVertices[triangle.vertices[1]], &worldVertices[triangle.vertices[2]], loopColliderVelocity, &intersect->fTime, &intersect->point, bCalcNormals, &intersect->normal1, &intersect->normal0)) {
				if (NI_AVObject_runCollisionCallbacks(this, intersect)) {
					return 1;
				}
			}
		}
		return 0;
#else
		// Non-MWSE targets fall through to engine vanilla behavior.
		return NI_TriBasedGeometry_findCollisionsTriVsABV(this, fTime, collidee, bCalcNormals, intersect);
#endif
#else
		throw not_implemented_exception();
#endif
	}
}
