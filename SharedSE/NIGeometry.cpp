#include "NIGeometry.h"

namespace NI {
	Pointer<GeometryData> Geometry::getModelData() const {
		return modelData;
	}

	void Geometry::setModelData(GeometryData* data) {
		vTable.asGeometry->setModelData(this, data);
	}

	void Geometry::updateDeforms() {
		updateDeforms(worldVertices, worldNormals);
	}

	void Geometry::updateDeforms(Point3* out_vertices, Point3* out_normals) {
		if (skinInstance == nullptr) {
			return;
		}

		if (worldVertices == nullptr) {
			createWorldVertices();
		}

		skinInstance->deform(modelData->vertex, modelData->normal, modelData->getActiveVertexCount(), out_vertices, out_normals);
		TransformVertices(out_vertices, modelData->getActiveVertexCount(), &worldTransform);
	}

	void __cdecl TransformVertices(Point3* vertices, unsigned short vertexCount, const Transform* transform) {
		TransformVertices(vertices, vertexCount, vertices, transform);
	}

	void __cdecl TransformVertices(Point3* out_vertices, unsigned short vertexCount, const Point3* in_vertices, const Transform* transform) {
		for (auto i = 0u; i < vertexCount; ++i) {
			out_vertices[i] = (transform->rotation * transform->scale * in_vertices[i]) + transform->translation;
		}
	}
}
