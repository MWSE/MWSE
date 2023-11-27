#include "NIPick.h"

#include "MemoryUtil.h"
#include "NIUtil.h"

#include "NIPointer.h"
#include "NISkinInstance.h"
#include "NITriShape.h"
#include "NiTriShapeData.h"

namespace NI {
	const auto TES3_Pick_ctor = reinterpret_cast<Pick*(__thiscall*)(Pick*)>(0x6F2DF0);
	Pick* Pick::malloc() {
		return TES3_Pick_ctor(mwse::tes3::malloc<Pick>());
	}

	const auto TES3_Pick_dtor = reinterpret_cast<void(__thiscall*)(Pick*)>(0x6F2EA0);
	void Pick::free() {
		TES3_Pick_dtor(this);
		mwse::tes3::free(this);
	}

	PickRecord* Pick::addRecord() {
		auto record = new PickRecord();
		lastAddedRecord = record;
		results.addToFirstEmptyIndex(record);
		return record;
	}

	const auto TES3_Pick_pickObjects = reinterpret_cast<bool(__thiscall*)(Pick*, const TES3::Vector3*, const TES3::Vector3*, bool, float)>(0x6F3050);
	bool Pick::pickObjects(const TES3::Vector3 * origin, const TES3::Vector3 * direction, bool append, float maxDistance) {
		return TES3_Pick_pickObjects(this, origin, direction, append, maxDistance);
	}

	const auto TES3_Pick_clearResults = reinterpret_cast<void(__thiscall*)(Pick*)>(0x6F2F80);
	void Pick::clearResults() {
		TES3_Pick_clearResults(this);
	}

	void* PickRecord::operator new(size_t size) {
		return mwse::tes3::_new(size);
	}

	void PickRecord::operator delete(void* address) {
		mwse::tes3::_delete(address);
	}

	std::reference_wrapper<unsigned short[3]> PickRecord::getVertexIndex() {
		return std::ref(vertexIndex);
	}

	TES3::Reference* PickRecord::getTES3Reference() {
		return getAssociatedReference(object);
	}
}
