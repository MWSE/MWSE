#include "NILines.h"

#include "ExceptionUtil.h"

namespace NI {

	Lines::Lines(unsigned short vertexCount, Point3* vertices, PackedColor* colors, Point2* textureCoords, bool* lineSegmentFlags) {
#if defined(SE_NI_LINES_FNADDR_CTOR) && SE_NI_LINES_FNADDR_CTOR > 0
		const auto NI_Lines_ctor = reinterpret_cast<Lines*(__thiscall*)(Lines*, unsigned short, Point3*, PackedColor*, Point2*, bool*)>(SE_NI_LINES_FNADDR_CTOR);
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
#if !defined(MWSE_NO_CUSTOM_ALLOC) || MWSE_NO_CUSTOM_ALLOC == 0
		auto vertices = se::memory::_new<Point3>(vertexCount);
		auto lineSegmentFlags = se::memory::_new<bool>(vertexCount);

		PackedColor* colors = nullptr;
		if (useColors) {
			colors = se::memory::_new<PackedColor>(vertexCount);
		}

		Point2* textureCoords = nullptr;
		if (useTextureCoords) {
			textureCoords = se::memory::_new<Point2>(vertexCount);
		}

		return new Lines(vertexCount, vertices, colors, textureCoords, lineSegmentFlags);
#else
		auto vertices = new Point3[vertexCount];
		auto lineSegmentFlags = new bool[vertexCount];

		PackedColor* colors = useColors ? new PackedColor[vertexCount] : nullptr;
		Point2* textureCoords = useTextureCoords ? new Point2[vertexCount] : nullptr;

		return new Lines(vertexCount, vertices, colors, textureCoords, lineSegmentFlags);
#endif
	}

	Pointer<Lines> Lines::create(unsigned short vertexCount, Point3* vertices, PackedColor* colors, Point2* textureCoords, bool* lineSegmentFlags) {
		return new Lines(vertexCount, vertices, colors, textureCoords, lineSegmentFlags);
	}
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::Lines)
#endif
