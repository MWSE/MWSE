#include "NITriShape.h"

#include "NITriShapeData.h"

#include "ExceptionUtil.h"

namespace NI {

	TriShape::TriShape(unsigned short vertexCount, Vector3* vertices, Vector3* normals, Color* colors, Vector2* textureCoords, unsigned short triangleCount, unsigned short* triList, int flags) {
#if defined(SE_NI_TRISHAPE_FNADDR_CTOR) && SE_NI_TRISHAPE_FNADDR_CTOR > 0
		const auto NI_TriShape_ctor = reinterpret_cast<TriShape*(__thiscall*)(TriShape*, unsigned short, Vector3*, Vector3*, Color*, Vector2*, unsigned short, unsigned short*, int)>(SE_NI_TRISHAPE_FNADDR_CTOR);
		NI_TriShape_ctor(this, vertexCount, vertices, normals, colors, textureCoords, triangleCount, triList, flags);
#else
		throw not_implemented_exception();
#endif
	}

	TriShape::~TriShape() {
		// Call dtor without deletion.
		vTable.asObject->destructor(this, 0);
	}

	Pointer<TriShape> TriShape::create(unsigned short vertexCount, Vector3* vertices, Vector3* normals, Color* colors, Vector2* textureCoords, unsigned short triangleCount, unsigned short* triList, int flags) {
		return new TriShape(vertexCount, vertices, normals, colors, textureCoords, triangleCount, triList, flags);
	}

	Pointer<TriShapeData> TriShape::getModelData() const {
		return static_cast<TriShapeData*>(modelData.get());
	}
}
