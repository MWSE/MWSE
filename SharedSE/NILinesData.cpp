#include "NILinesData.h"

#include "NIBinaryStream.h"
#include "NIStream.h"
#include "MemoryUtil.h"
#include "ExceptionUtil.h"

namespace NI {
	void LinesData::loadBinary(Stream* stream) {
#if defined(SE_NI_GEOMETRYDATA_FNADDR_LOADBINARY) && SE_NI_GEOMETRYDATA_FNADDR_LOADBINARY > 0
		reinterpret_cast<void(__thiscall*)(GeometryData*, Stream*)>(SE_NI_GEOMETRYDATA_FNADDR_LOADBINARY)(this, stream);
#if !defined(MWSE_NO_CUSTOM_ALLOC) || MWSE_NO_CUSTOM_ALLOC == 0
		lineSegmentFlags = se::memory::_new<bool>(vertexCount);
#else
		lineSegmentFlags = new bool[vertexCount];
#endif
		stream->inStream->read(lineSegmentFlags, vertexCount);
#else
		throw not_implemented_exception();
#endif
	}
}
