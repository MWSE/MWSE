#include "NILines.h"

#include "ExceptionUtil.h"

namespace NI {

	Lines::Lines(unsigned short vertexCount, Vector3* vertices, PackedColor* colors, Vector2* textureCoords, bool* lineSegmentFlags) {
#if defined(SE_NI_LINES_FNADDR_CTOR) && SE_NI_LINES_FNADDR_CTOR > 0
		const auto NI_Lines_ctor = reinterpret_cast<Lines*(__thiscall*)(Lines*, unsigned short, Vector3*, PackedColor*, Vector2*, bool*)>(SE_NI_LINES_FNADDR_CTOR);
		NI_Lines_ctor(this, vertexCount, vertices, colors, textureCoords, lineSegmentFlags);
#else
		throw not_implemented_exception();
#endif
	}

	Lines::~Lines() {
		// Call dtor without deletion.
		vTable.asObject->destructor(this, 0);
	}

	Pointer<LinesData> Lines::getModelData() const {
		return static_cast<LinesData*>(modelData.get());
	}

	Pointer<Lines> Lines::create(unsigned short vertexCount, bool useColors, bool useTextureCoords) {
		auto vertices = se::memory::_new<Vector3>(vertexCount);
		auto lineSegmentFlags = se::memory::_new<bool>(vertexCount);

		PackedColor* colors = nullptr;
		if (useColors) {
			colors = se::memory::_new<PackedColor>(vertexCount);
		}

		Vector2* textureCoords = nullptr;
		if (useTextureCoords) {
			textureCoords = se::memory::_new<Vector2>(vertexCount);
		}

		return new Lines(vertexCount, vertices, colors, textureCoords, lineSegmentFlags);
	}

	Pointer<Lines> Lines::create(unsigned short vertexCount, Vector3* vertices, PackedColor* colors, Vector2* textureCoords, bool* lineSegmentFlags) {
		return new Lines(vertexCount, vertices, colors, textureCoords, lineSegmentFlags);
	}
}
