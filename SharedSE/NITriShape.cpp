#include "NITriShape.h"

#include "NITriShapeData.h"

#include "ExceptionUtil.h"

namespace NI {
	TriShape::TriShape(TriBasedGeometryData* data) : TriBasedGeometry(data) {
#if defined(SE_NI_TRISHAPE_VTBL) && SE_NI_TRISHAPE_VTBL > 0
		vTable.asObject = reinterpret_cast<Object_vTable*>(SE_NI_TRISHAPE_VTBL);
#else
		throw not_implemented_exception();
#endif
	}

	TriShape::TriShape(unsigned short vertexCount, Vector3* vertices, Vector3* normals, Color* colors, Vector2* textureCoords, unsigned short triangleCount, unsigned short* triList, int flags) : TriBasedGeometry(nullptr) {
#if defined(SE_NI_TRISHAPE_FNADDR_CTOR) && SE_NI_TRISHAPE_FNADDR_CTOR > 0
		const auto NI_TriShape_ctor = reinterpret_cast<TriShape*(__thiscall*)(TriShape*, unsigned short, Vector3*, Vector3*, Color*, Vector2*, unsigned short, unsigned short*, int)>(SE_NI_TRISHAPE_FNADDR_CTOR);
		NI_TriShape_ctor(this, vertexCount, vertices, normals, colors, textureCoords, triangleCount, triList, flags);
#else
		throw not_implemented_exception();
#endif
	}

	TriShape::~TriShape() {
		// Call dtor without deletion via vtable.
		vTable.asObject->destructor(this, 0);
	}

	Pointer<TriShape> TriShape::create(unsigned short vertexCount, bool hasNormals, bool hasColors, unsigned short textureCoordSets, unsigned short triangleCount) {
		auto data = TriShapeData::create(vertexCount, hasNormals, hasColors, textureCoordSets, triangleCount);
		return new TriShape(data);
	}

#if !defined(SE_IS_MWSE) || SE_IS_MWSE == 0
	Pointer<TriShape> TriShape::create(unsigned short vertexCount, Vector3* vertices, Vector3* normals, Color* colors, Vector2* textureCoords, unsigned short triangleCount, unsigned short* triList, int flags) {
		return new TriShape(vertexCount, vertices, normals, colors, textureCoords, triangleCount, triList, flags);
	}
#endif

	Pointer<TriShapeData> TriShape::getModelData() const {
		return static_cast<TriShapeData*>(modelData.get());
	}

	void TriShape::linkObject(Stream* stream) {
#if defined(SE_NI_GEOMETRY_FNADDR_LINKOBJECT) && SE_NI_GEOMETRY_FNADDR_LINKOBJECT > 0
		const auto NI_Geometry_LinkObject = reinterpret_cast<void(__thiscall*)(Geometry*, Stream*)>(SE_NI_GEOMETRY_FNADDR_LINKOBJECT);
		NI_Geometry_LinkObject(this, stream);

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
		// MWSE-added software-skinning flag patch-up: propagate the flag bit
		// from the TriShape's own flags into the linked TriBasedGeometryData.
		if (flags & TriShapeFlags::SoftwareSkinningFlag) {
			auto data = static_cast<TriBasedGeometryData*>(modelData.get());
			data->patchRenderFlags |= TriShapeFlags::SoftwareSkinningFlag;
		}
#endif
#else
		throw not_implemented_exception();
#endif
	}

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
	nonstd::span<Vector3> TriShape::getVertices() const {
		return getModelData()->getVertices();
	}

	nonstd::span<Vector3> TriShape::getNormals() const {
		return getModelData()->getNormals();
	}
#endif
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::TriShape)
#endif
