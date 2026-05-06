#include "NILinesData.h"

#include "NIBinaryStream.h"
#include "NIStream.h"
#include "MemoryUtil.h"
#include "ExceptionUtil.h"

namespace NI {
	void LinesData::loadBinary(Stream* stream) {
#if defined(SE_NI_GEOMETRYDATA_FNADDR_LOADBINARY) && SE_NI_GEOMETRYDATA_FNADDR_LOADBINARY > 0
		reinterpret_cast<void(__thiscall*)(GeometryData*, Stream*)>(SE_NI_GEOMETRYDATA_FNADDR_LOADBINARY)(this, stream);
		lineSegmentFlags = se::memory::_new<bool>(vertexCount);
		stream->inStream->read(lineSegmentFlags, vertexCount);
#else
		throw not_implemented_exception();
#endif
	}
}
