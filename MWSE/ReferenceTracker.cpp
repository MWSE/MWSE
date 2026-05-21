#include "ReferenceTracker.h"

#include "TES3Object.h"

#include "TES3DataHandler.h"
#include "TES3Cell.h"
#include "TES3Reference.h"

#include "Log.h"

namespace mwse {
	struct ReferenceTrackingData {
		std::vector<TES3::Reference*> references = {};
		bool dirtyLookup = false;
	};

	static std::unordered_set<TES3::Reference*> trackedReferences;
	static std::unordered_map<const TES3::PhysicalObject*, ReferenceTrackingData> referenceDataByObject;
	static std::unordered_map<const TES3::Cell*, size_t> referenceLookupCellOrder;
	static const std::vector<TES3::Reference*> emptyReferences;
	constexpr auto LOG_REFERENCE_TRACKING = false;

	static const char* getReferenceLookupLogId(const TES3::BaseObject* object) {
		__try {
			return object ? object->getObjectID() : nullptr;
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			return "<invalid>";
		}
	}

	static bool shouldLogReferenceLookupObject(const TES3::BaseObject* object) {
		if constexpr (!LOG_REFERENCE_TRACKING) {
			return false;
		}

		const auto id = getReferenceLookupLogId(object);
		if (id == nullptr || id == reinterpret_cast<const char*>("<invalid>")) {
			return false;
		}

		// Enter ID(s) here to track misbehavior.
		return _stricmp(id, "TR_FM_Naureen") == 0;
	}

	static bool shouldLogReferenceLookupReference(const TES3::Reference* reference) {
		if (reference == nullptr) {
			return false;
		}

		return shouldLogReferenceLookupObject(reference->getBaseObject());
	}

	static const TES3::PhysicalObject* getLookupKey(const TES3::BaseObject* object) {
		if (object == nullptr) {
			return nullptr;
		}

		const auto baseObject = object->getBaseObject();
		if (baseObject == nullptr) {
			return nullptr;
		}

		return baseObject->asPhysicalObject();
	}

	static bool addReferenceToLookupImpl(TES3::Reference* reference) {
		const auto key = getLookupKey(reference);
		if (shouldLogReferenceLookupReference(reference)) {
			mwse::log::getLog()
				<< "[MWSE] Reference lookup index: ref=" << reference
				<< " refId=" << (getReferenceLookupLogId(reference) ? getReferenceLookupLogId(reference) : "<no id>")
				<< " base=" << reference->baseObject
				<< " baseId=" << (getReferenceLookupLogId(reference->baseObject) ? getReferenceLookupLogId(reference->baseObject) : "<no id>")
				<< " key=" << key
				<< " keyId=" << (getReferenceLookupLogId(key) ? getReferenceLookupLogId(key) : "<no id>")
				<< " cell=" << reference->getCell()
				<< std::endl;
		}

		if (key == nullptr) {
			if (shouldLogReferenceLookupReference(reference)) {
				mwse::log::getLog() << "[MWSE] Reference lookup skipped: null key for ref=" << reference << std::endl;
			}
			return false;
		}

		auto& references = referenceDataByObject[key].references;
		if (std::find(references.begin(), references.end(), reference) == references.end()) {
			references.push_back(reference);
			return true;
		}

		return false;
	}

	static void markReferenceLookupKeyDirty(const TES3::BaseObject* object) {
		const auto key = getLookupKey(object);
		if (key != nullptr) {
			referenceDataByObject[key].dirtyLookup = true;
		}
	}

	static void rebuildReferenceLookupCellOrder() {
		referenceLookupCellOrder.clear();
		const auto dataHandler = TES3::DataHandler::get();
		const auto nonDynamicData = dataHandler ? dataHandler->nonDynamicData : nullptr;
		const auto cells = nonDynamicData ? nonDynamicData->cells : nullptr;
		if (cells == nullptr) {
			return;
		}

		size_t index = 0;
		for (const auto cell : *cells) {
			if (cell != nullptr) {
				referenceLookupCellOrder[cell] = index++;
			}
		}
	}

	static size_t getReferenceLookupCellOrder(const TES3::Cell* cell) {
		if (cell == nullptr) {
			return std::numeric_limits<size_t>::max();
		}

		auto orderIt = referenceLookupCellOrder.find(cell);
		if (orderIt == referenceLookupCellOrder.end()) {
			rebuildReferenceLookupCellOrder();
			orderIt = referenceLookupCellOrder.find(cell);
		}

		return orderIt != referenceLookupCellOrder.end() ? orderIt->second : std::numeric_limits<size_t>::max();
	}

	static int getReferenceLookupListOrder(const TES3::Reference* reference) {
		const auto cell = reference ? reference->getCell() : nullptr;
		const auto list = reference ? reference->owningCollection.asReferenceList : nullptr;
		if (cell == nullptr || list == nullptr) {
			return 3;
		}

		if (list == &cell->actors) {
			return 0;
		}
		if (list == &cell->persistentRefs) {
			return 1;
		}
		if (list == &cell->temporaryRefs) {
			return 2;
		}
		return 3;
	}

	static bool isReferenceBeforeInCurrentList(const TES3::Reference* lhs, const TES3::Reference* rhs) {
		const auto list = lhs ? lhs->owningCollection.asReferenceList : nullptr;
		if (list == nullptr || list != (rhs ? rhs->owningCollection.asReferenceList : nullptr)) {
			return lhs < rhs;
		}

		for (const auto reference : *list) {
			if (reference == lhs) {
				return true;
			}
			if (reference == rhs) {
				return false;
			}
		}

		return lhs < rhs;
	}

	static bool isReferenceBeforeInVanillaLookupOrder(const TES3::Reference* lhs, const TES3::Reference* rhs) {
		const auto lhsCellOrder = getReferenceLookupCellOrder(lhs ? lhs->getCell() : nullptr);
		const auto rhsCellOrder = getReferenceLookupCellOrder(rhs ? rhs->getCell() : nullptr);
		if (lhsCellOrder != rhsCellOrder) {
			return lhsCellOrder < rhsCellOrder;
		}

		const auto lhsListOrder = getReferenceLookupListOrder(lhs);
		const auto rhsListOrder = getReferenceLookupListOrder(rhs);
		if (lhsListOrder != rhsListOrder) {
			return lhsListOrder < rhsListOrder;
		}

		return isReferenceBeforeInCurrentList(lhs, rhs);
	}

	static void sortReferencesLookupForKey(const TES3::PhysicalObject* key) {
		if (key == nullptr) {
			return;
		}

		const auto dataIt = referenceDataByObject.find(key);
		if (dataIt == referenceDataByObject.end()) {
			return;
		}

		if (!dataIt->second.dirtyLookup) {
			return;
		}

		if constexpr (LOG_REFERENCE_TRACKING) {
			mwse::log::getLog()
				<< "[MWSE] Sorting reference lookup: key=" << key
				<< " keyId=" << (getReferenceLookupLogId(key) ? getReferenceLookupLogId(key) : "<no id>")
				<< std::endl;
		}

		auto& references = dataIt->second.references;
		std::sort(references.begin(), references.end(), isReferenceBeforeInVanillaLookupOrder);
		dataIt->second.dirtyLookup = false;
	}

	const std::vector<TES3::Reference*>& ReferenceTracker::getReferences(const TES3::PhysicalObject* object) {
		const auto key = getLookupKey(object);
		sortReferencesLookupForKey(key);

		const auto logLookup = shouldLogReferenceLookupObject(object) || shouldLogReferenceLookupObject(key);
		if (logLookup) {
			mwse::log::getLog()
				<< "[MWSE] Reference lookup query: object=" << object
				<< " objectId=" << (getReferenceLookupLogId(object) ? getReferenceLookupLogId(object) : "<no id>")
				<< " key=" << key
				<< " keyId=" << (getReferenceLookupLogId(key) ? getReferenceLookupLogId(key) : "<no id>")
				<< std::endl;
		}

		if (key == nullptr) {
			if (logLookup) {
				mwse::log::getLog() << "[MWSE] Reference lookup query result: null key" << std::endl;
			}
			return emptyReferences;
		}

		const auto referencesIt = referenceDataByObject.find(key);
		if (referencesIt == referenceDataByObject.end()) {
			if (logLookup) {
				mwse::log::getLog() << "[MWSE] Reference lookup query result: missing key" << std::endl;
			}
			return emptyReferences;
		}

		if (logLookup) {
			mwse::log::getLog() << "[MWSE] Reference lookup query result: count=" << referencesIt->second.references.size() << std::endl;
		}

		return referencesIt->second.references;
	}

	void ReferenceTracker::trackReferenceForLookup(TES3::Reference* reference) {
		if (reference == nullptr) {
			return;
		}

		if (trackedReferences.insert(reference).second) {
			addReferenceToLookupImpl(reference);
			markReferenceLookupKeyDirty(reference);
			if (shouldLogReferenceLookupReference(reference)) {
				mwse::log::getLog()
					<< "[MWSE] Reference lookup tracked: ref=" << reference
					<< " refId=" << (getReferenceLookupLogId(reference) ? getReferenceLookupLogId(reference) : "<no id>")
					<< " base=" << reference->baseObject
					<< " baseId=" << (getReferenceLookupLogId(reference->baseObject) ? getReferenceLookupLogId(reference->baseObject) : "<no id>")
					<< " trackedReferences=" << trackedReferences.size()
					<< std::endl;
			}
		}
	}

	void ReferenceTracker::untrackReferenceForLookup(TES3::Reference* reference) {
		if (reference == nullptr) {
			return;
		}

		const auto key = getLookupKey(reference);
		markReferenceLookupKeyDirty(reference);
		if (trackedReferences.erase(reference)) {
			if (key != nullptr) {
				const auto referencesIt = referenceDataByObject.find(key);
				if (referencesIt != referenceDataByObject.end()) {
					auto& references = referencesIt->second.references;
					references.erase(std::remove(references.begin(), references.end(), reference), references.end());
				}
			}

			if (shouldLogReferenceLookupReference(reference)) {
				mwse::log::getLog()
					<< "[MWSE] Reference lookup untracked: ref=" << reference
					<< " refId=" << (getReferenceLookupLogId(reference) ? getReferenceLookupLogId(reference) : "<no id>")
					<< " base=" << reference->baseObject
					<< " baseId=" << (getReferenceLookupLogId(reference->baseObject) ? getReferenceLookupLogId(reference->baseObject) : "<no id>")
					<< " trackedReferences=" << trackedReferences.size()
					<< std::endl;
			}
		}
	}

	void ReferenceTracker::markReferencesLookupDirty(const TES3::PhysicalObject* object) {
		if (object == nullptr) {
			return;
		}

		markReferenceLookupKeyDirty(object);
	}

	static void invalidateCell(const TES3::Cell* cell) {
		const auto itt = referenceLookupCellOrder.find(cell);
		if (itt == referenceLookupCellOrder.end()) {
			return;
		}

		referenceLookupCellOrder.erase(itt);
	}

	static void invalidatePhysicalObject(const TES3::PhysicalObject* object) {
		if (object == nullptr) {
			return;
		}

		const auto itt = referenceDataByObject.find(object);
		if (itt == referenceDataByObject.end()) {
			return;
		}

		referenceDataByObject.erase(itt);
	}

	void ReferenceTracker::invalidateObject(TES3::BaseObject* object) {
		if (object->objectType == TES3::ObjectType::Cell) {
			invalidateCell(static_cast<const TES3::Cell*>(object));
			return;
		}

		if (object->objectType == TES3::ObjectType::Reference) {
			untrackReferenceForLookup(static_cast<TES3::Reference*>(object));
			return;
		}

		const auto asPhysical = object->asPhysicalObject();
		if (asPhysical) {
			invalidatePhysicalObject(asPhysical);
			return;
		}
	}
}
