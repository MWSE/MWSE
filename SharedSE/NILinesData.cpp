#include "NILinesData.h"

#include "NIBinaryStream.h"
#include "NIStream.h"
#include "MemoryUtil.h"

namespace NI {
	const auto NI_GeometryData_loadBinary = reinterpret_cast<void(__thiscall*)(GeometryData*, Stream*)>(0x6EF5E0);
	void LinesData::loadBinary(Stream* stream) {
		NI_GeometryData_loadBinary(this, stream);
		lineSegmentFlags = se::memory::_new<bool>(vertexCount);
		stream->inStream->read(lineSegmentFlags, vertexCount);
	}
}
