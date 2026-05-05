#include "NIPick.h"

#include "NITriShape.h"
#include "NITriShapeData.h"
#include "NISkinInstance.h"

#include "ExceptionUtil.h"
#include "MemoryUtil.h"

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
#include "NIUtil.h"
#endif

namespace NI {
	Pick::Pick() {
#if defined(SE_NI_PICK_FNADDR_CTOR) && SE_NI_PICK_FNADDR_CTOR > 0
		// Cleanup automatic cruft.
		se::memory::_delete(results.storage);
		memset(this, 0, sizeof(Pick));

		const auto NI_Pick_ctor = reinterpret_cast<void(__thiscall*)(Pick*)>(SE_NI_PICK_FNADDR_CTOR);
		NI_Pick_ctor(this);
#else
		throw not_implemented_exception();
#endif
	}

	Pick::~Pick() {
#if defined(SE_NI_PICK_FNADDR_DTOR) && SE_NI_PICK_FNADDR_DTOR > 0
		const auto NI_Pick_dtor = reinterpret_cast<void(__thiscall*)(Pick*)>(SE_NI_PICK_FNADDR_DTOR);
		NI_Pick_dtor(this);
#else
		throw not_implemented_exception();
#endif
	}

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
	Pick* Pick::malloc() {
#if defined(SE_NI_PICK_FNADDR_CTOR) && SE_NI_PICK_FNADDR_CTOR > 0 && defined(SE_MEMORY_FNADDR_MALLOC) && SE_MEMORY_FNADDR_MALLOC > 0
		const auto NI_Pick_ctor = reinterpret_cast<Pick*(__thiscall*)(Pick*)>(SE_NI_PICK_FNADDR_CTOR);
		return NI_Pick_ctor(se::memory::malloc<Pick>());
#else
		throw not_implemented_exception();
#endif
	}

	void Pick::free() {
#if defined(SE_NI_PICK_FNADDR_DTOR) && SE_NI_PICK_FNADDR_DTOR > 0 && defined(SE_MEMORY_FNADDR_FREE) && SE_MEMORY_FNADDR_FREE > 0
		const auto NI_Pick_dtor = reinterpret_cast<void(__thiscall*)(Pick*)>(SE_NI_PICK_FNADDR_DTOR);
		NI_Pick_dtor(this);
		se::memory::free(this);
#else
		throw not_implemented_exception();
#endif
	}
#endif

	PickRecord* Pick::addRecord() {
		auto record = new PickRecord();
		lastAddedRecord = record;
		results.addToFirstEmptyIndex(record);
		return record;
	}

	bool Pick::pickObjects(const Vector3* origin, const Vector3* direction, bool append, float maxDistance) {
#if defined(SE_NI_PICK_FNADDR_PICKOBJECTS) && SE_NI_PICK_FNADDR_PICKOBJECTS > 0
		const auto NI_Pick_pickObjects = reinterpret_cast<bool(__thiscall*)(Pick*, const Vector3*, const Vector3*, bool, float)>(SE_NI_PICK_FNADDR_PICKOBJECTS);
		return NI_Pick_pickObjects(this, origin, direction, append, maxDistance);
#else
		throw not_implemented_exception();
#endif
	}

	struct SavedGeometryState {
		Pointer<TriShape> geometry;
		Pointer<TriShapeData> data;
		Pointer<SkinInstance> skinInstance;
	};

	bool Pick::pickObjectsWithSkinDeforms(const Vector3* origin, const Vector3* direction, bool append, float maxDistance) {
		// Data to restore skinned objects to their original state.
		std::vector<SavedGeometryState> savedGeometryStates;

		const auto previousPickType = pickType;
		pickType = NI::PickType::FIND_ALL;

		// Perform our first test.
		if (!pickObjects(origin, direction, append, maxDistance)) {
			pickType = previousPickType;
			return false;
		}

		// Store if a redo is needed.
		auto needsRedo = false;

		// Check to see if any skinned objects were hit.
		for (auto& result : results) {
			if (result == nullptr) {
				continue;
			}

			// Skip if we're not affecting tri-based geometry.
			if (!result->object->isInstanceOfType(RTTIStaticPtr::NiTriShape)) {
#if _DEBUG
				if (result->object->skinInstance) {
					pickType = previousPickType;
					throw std::exception("Unaccounted for type of skinned object!");
				}
#endif
				continue;
			}

			const auto geometry = static_cast<TriShape*>(result->object.get());

			// If the geometry is skinned we apply the skin deform and set the redo flag.
			// This way the redo raycast (and future raycasts) can handle the object correctly.
			if (geometry->skinInstance) {
				needsRedo = true;

				// Create our backup data.
				savedGeometryStates.push_back({});
				auto& backup = savedGeometryStates.back();
				backup.geometry = geometry;

				// Clone the data.
				backup.data = geometry->getModelData();
				auto data = backup.data->copyData(true, false, false);
				data->bounds = backup.data->bounds;
				geometry->setModelData(data);

				// Apply the skin deform to the new copy.
				geometry->skinInstance->deform(backup.data->vertex, backup.data->normal, data->vertexCount, data->vertex, data->normal);
				data->markAsChanged();

				// Backup and clear skin instance.
				backup.skinInstance = geometry->skinInstance;
				geometry->skinInstance = nullptr;

				geometry->update();
			}
		}

		// Perform another raytest if necessary.
		// TODO: To optimize this further, perform a second pick in another root with only the skinned geometry, and update the existing distances.
		auto result = true;
		if (needsRedo) {
			pickType = previousPickType;
			clearResults();
			result = pickObjects(origin, direction, append, maxDistance);
		}

		// Restore existing geomtry data.
		for (const auto& state : savedGeometryStates) {
			state.geometry->setModelData(state.data);
			state.geometry->skinInstance = state.skinInstance;
			state.geometry->update();
		}

		pickType = previousPickType;
		return result;
	}

	void Pick::clearResults() {
#if defined(SE_NI_PICK_FNADDR_CLEARRESULTS) && SE_NI_PICK_FNADDR_CLEARRESULTS > 0
		const auto NI_Pick_clearResults = reinterpret_cast<void(__thiscall*)(Pick*)>(SE_NI_PICK_FNADDR_CLEARRESULTS);
		NI_Pick_clearResults(this);
#else
		throw not_implemented_exception();
#endif
	}

	PickRecord* Pick::getFirstUnskinnedResult() const {
		for (const auto& result : results) {
			if (result && result->object && result->object->skinInstance == nullptr) {
				return result;
			}
		}
		return nullptr;
	}

	void* PickRecord::operator new(size_t size) {
#if defined(SE_MEMORY_FNADDR_NEW) && SE_MEMORY_FNADDR_NEW > 0
		return se::memory::_new(size);
#else
		throw not_implemented_exception();
#endif
	}

	void PickRecord::operator delete(void* address) {
#if defined(SE_MEMORY_FNADDR_DELETE) && SE_MEMORY_FNADDR_DELETE > 0
		se::memory::_delete(address);
#else
		throw not_implemented_exception();
#endif
	}

	std::reference_wrapper<unsigned short[3]> PickRecord::getVertexIndex() {
		return std::ref(vertexIndex);
	}

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
	TES3::Reference* PickRecord::getTES3Reference() {
		return getAssociatedReference(object);
	}
#endif
}
