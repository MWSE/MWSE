#include "NITriBasedGeometry.h"

#include "NIPick.h"
#include "NISkinInstance.h"

#include "MWSEConfig.h"

#include "MemoryUtil.h"

namespace NI {
	const auto NI_TriBasedGeometry_ctorFromData = reinterpret_cast<void(__thiscall*)(TriBasedGeometry*, TriBasedGeometryData*)>(0x6EFEA0);
	TriBasedGeometry::TriBasedGeometry(TriBasedGeometryData* data) {
		NI_TriBasedGeometry_ctorFromData(this, data);
	}

	using gPickDefaultTextureCoords = mwse::ExternalGlobal<TES3::Vector2, 0x7DED80>;
	using gPickDefaultColor = mwse::ExternalGlobal<NI::PackedColor, 0x7DE814>;

	static bool __cdecl FindIntersectRayWithTriangle(const TES3::Vector3* position, const TES3::Vector3* direction, const TES3::Vector3* vertex1, const TES3::Vector3* vertex2, const TES3::Vector3* vertex3, bool frontOnly, TES3::Vector3* out_intersection, float* out_distance, float* out_weight2, float* out_weight3) {
		const auto NI_FindIntersectRayWithTriangle = reinterpret_cast<bool(__cdecl*)(const TES3::Vector3*, const TES3::Vector3*, const TES3::Vector3*, const TES3::Vector3*, const TES3::Vector3*, bool, TES3::Vector3*, float*, float*, float*)>(0x6EFF90);
		return NI_FindIntersectRayWithTriangle(position, direction, vertex1, vertex2, vertex3, frontOnly, out_intersection, out_distance, out_weight2, out_weight3);
	}

	static std::vector<TES3::Vector3> deformVertices;
	static std::vector<TES3::Vector3> deformNormals;

	const auto NI_TriBasedGeometry_findIntersections = reinterpret_cast<bool(__thiscall*)(TriBasedGeometry*, const TES3::Vector3*, const TES3::Vector3*, Pick*)>(0x6F0350);
	bool TriBasedGeometry::findIntersections(const TES3::Vector3* position, const TES3::Vector3* direction, Pick* pick) {
		// Allow the MCM configuration option to disable this logic entirely and fall back to vanilla behavior.
		// This will be removed in the future when the config option is removed, once we're confident of performance and accuracy.
		if (!mwse::Configuration::UseSkinnedAccurateActivationRaytests) {
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

		// Loop through all the triangles 
		auto addedResult = false;
		for (auto i = 0u; i < activeTriCount; ++i) {
			// Get some shorthand variables we'll use throughout.
			const auto& triangle = triList[i];
			const auto index1 = triangle.vertices[0];
			const auto index2 = triangle.vertices[1];
			const auto index3 = triangle.vertices[2];
			const auto vertex1 = &vertices[index1];
			const auto vertex2 = &vertices[index2];
			const auto vertex3 = &vertices[index3];

			// Perform our test for the triangle, and calculate the weight to each index.
			auto distance = std::numeric_limits<float>::infinity();
			TES3::Vector3 intersection;
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
				result->texture = gPickDefaultTextureCoords::get();
			}

			// Calculate weighted normals. 
			if (pick->returnNormal) {
				TES3::Vector3 normal = {};
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
				result->color = gPickDefaultColor::get();
			}

			// We can be finished if we just want the first unsorted result.
			if (pick->pickType == PickType::FIND_FIRST && pick->sortType == PickSortType::NO_SORT) {
				break;
			}
		}

		return addedResult;
	}
}
