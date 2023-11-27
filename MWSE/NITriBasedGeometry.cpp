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

	using gPickIgnoresSkinInstances = mwse::ExternalGlobal<bool, 0x7DEA4C>;
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
		if (!mwse::Configuration::UseSkinnedAccurateActivationRaytests) {
			return NI_TriBasedGeometry_findIntersections(this, position, direction, pick);
		}

		// Ignore if we don't care about culled geometry.
		if (pick->observeAppCullFlag && getAppCulled()) {
			return false;
		}

		// Get the bounds distance.
		auto boundsDistance = 0.0f;
		if (!intersectBounds(position, direction, &boundsDistance)) {
			return false;
		}

		// Perform a basic bounds intersection test.
		if (pick->intersectType == PickIntersectType::BOUND_INTERSECT ||
			pick->intersectType == PickIntersectType::UNKNOWN_2 ||
			skinInstance && gPickIgnoresSkinInstances::get())
		{
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

		// Deform vertices if we're looking at a skinned object.
		if (skinInstance) {
			const auto vertexCount = modelData->getActiveVertexCount();
			deformVertices.reserve(vertexCount);
			deformNormals.reserve(vertexCount);
			vertices = deformVertices.data();
			normals = deformNormals.data();
			skinInstance->deform(modelData->vertex, modelData->normal, vertexCount, vertices, normals);
		}

		// Calculate for non-uniform scaling.
		const auto inverseScale = 1.0f / worldTransform.scale;
		const auto worldRotationInverse = worldTransform.rotation.invert() * inverseScale;
		const auto worldScaled = worldRotationInverse * (*position - worldTransform.translation);
		const auto directionScaled = worldRotationInverse * (*direction);

		auto addedResult = false;
		for (auto i = 0u; i < activeTriCount; ++i) {
			const auto triOffset = i * 3;
			const auto index1 = triList[triOffset];
			const auto index2 = triList[triOffset + 1];
			const auto index3 = triList[triOffset + 2];
			const auto vertex1 = &vertices[index1];
			const auto vertex2 = &vertices[index2];
			const auto vertex3 = &vertices[index3];

			auto distance = std::numeric_limits<float>::infinity();
			TES3::Vector3 intersection = {};
			float weight2, weight3;
			if (!FindIntersectRayWithTriangle(&worldScaled, &directionScaled, vertex1, vertex2, vertex3, pick->frontOnly, &intersection, &distance, &weight2, &weight3)) {
				continue;
			}

			const auto weight1 = 1.0f - weight2 - weight3;

			addedResult = true;
			const auto result = pick->addRecord();
			result->object = this;
			result->triangleIndex = i;
			result->vertexIndex[0] = index1;
			result->vertexIndex[1] = index2;
			result->vertexIndex[2] = index3;
			result->distance = distance;

			if (pick->coordinateType == PickCoordinateType::WORLD_COORDINATES) {
				result->intersection = worldTransform * intersection;
			}
			else {
				result->intersection = intersection;
			}

			const auto textureCoords = modelData->textureCoords;
			if (pick->returnTexture && textureCoords) {
				result->texture = textureCoords[index1] * weight1 + textureCoords[index2] * weight2 + textureCoords[index3] * weight3;
			}
			else {
				result->texture = gPickDefaultTextureCoords::get();
			}

			if (pick->returnNormal) {
				TES3::Vector3 normal = {};
				if (pick->returnSmoothNormal && normals) {
					normal = normals[index1] * weight1 + normals[index2] * weight2 + normals[index3] * weight3;
				}
				else {
					const auto vertex3m1 = (*vertex3 - *vertex1);
					normal = (*vertex2 - *vertex1).crossProduct(&vertex3m1);
				}
				normal = normal * (1.0f / normal.length());
				if (pick->coordinateType == PickCoordinateType::WORLD_COORDINATES) {
					result->normal = worldTransform.rotation * normal;
				}
				else {
					result->normal = normal;
				}
			}

			const auto colors = modelData->color;
			if (pick->returnColor && colors) {
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
