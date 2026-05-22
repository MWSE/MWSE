#include "ReferenceTracker.h"

#include "TES3Object.h"

#include "TES3DataHandler.h"
#include "TES3Cell.h"
#include "TES3Reference.h"

#include "Log.h"
#include "StringUtil.h"

namespace mwse {
	struct ReferenceTrackingData {
		std::vector<TES3::Reference*> references = {};
		bool dirtyLookup = false;
	};

	static std::unordered_set<TES3::Reference*> trackedReferences;
	static std::unordered_map<const TES3::PhysicalObject*, ReferenceTrackingData> referenceDataByObject;
	static std::unordered_map<const TES3::Cell*, size_t> referenceLookupCellOrder;
	static const std::vector<TES3::Reference*> emptyReferences;
	static std::recursive_mutex referenceTrackerMutex;
	constexpr auto LOG_REFERENCE_TRACKING = false;

	static const char* getReferenceLookupLogId(const TES3::BaseObject* object) {
		__try {
			return object ? object->getObjectID() : nullptr;
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			return nullptr;
		}
	}

	static bool shouldLogReferenceLookupObject(const TES3::BaseObject* object) {
		if constexpr (!LOG_REFERENCE_TRACKING) {
			return false;
		}

		const auto id = getReferenceLookupLogId(object);
		if (id == nullptr) {
			return false;
		}

		// Enter ID(s) here to track misbehavior.
		return se::string::cicontains(id, "tl_mineguard1");
	}

	static bool shouldLogReferenceLookupReference(const TES3::Reference* reference) {
		if (reference == nullptr) {
			return false;
		}

		return shouldLogReferenceLookupObject(reference->getBaseObject());
	}

	static bool addReferenceToLookupImpl(TES3::Reference* reference) {
		const auto key = ReferenceTracker::getLookupKey(reference);
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

	static void removeReferenceFromLookupImpl(TES3::Reference* reference, const TES3::PhysicalObject* key) {
		if (reference == nullptr || key == nullptr) {
			return;
		}

		const auto referencesIt = referenceDataByObject.find(key);
		if (referencesIt == referenceDataByObject.end()) {
			return;
		}

		auto& references = referencesIt->second.references;
		references.erase(std::remove(references.begin(), references.end(), reference), references.end());
		referencesIt->second.dirtyLookup = true;
	}

	static void markReferenceLookupKeyDirty(const TES3::BaseObject* object) {
		ReferenceTracker::Lock lock;
		const auto key = ReferenceTracker::getLookupKey(object);
		if (key != nullptr) {
			referenceDataByObject[key].dirtyLookup = true;
		}
	}

	static void rebuildReferenceLookupCellOrder() {
		ReferenceTracker::Lock lock;
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
		ReferenceTracker::Lock lock;
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
		if (lhs == rhs) {
			return false;
		}

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
		ReferenceTracker::Lock lock;
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

	ReferenceTracker::Lock::Lock() {
		referenceTrackerMutex.lock();
	}

	ReferenceTracker::Lock::~Lock() {
		referenceTrackerMutex.unlock();
	}

	const std::vector<TES3::Reference*>& ReferenceTracker::getReferences(const TES3::PhysicalObject* object) {
		Lock lock;
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
		Lock lock;
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
		Lock lock;
		if (reference == nullptr) {
			return;
		}

		const auto key = getLookupKey(reference);
		markReferenceLookupKeyDirty(reference);
		if (trackedReferences.erase(reference)) {
			removeReferenceFromLookupImpl(reference, key);

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

	void ReferenceTracker::rekeyReference(TES3::Reference* reference, const TES3::PhysicalObject* previousLookupKey) {
		Lock lock;
		if (reference == nullptr) {
			return;
		}

		const auto currentLookupKey = getLookupKey(reference);
		if (previousLookupKey == currentLookupKey) {
			if (currentLookupKey != nullptr) {
				markReferenceLookupKeyDirty(currentLookupKey);
			}
			return;
		}

		removeReferenceFromLookupImpl(reference, previousLookupKey);

		if (currentLookupKey == nullptr) {
			trackedReferences.erase(reference);
		}
		else {
			trackedReferences.insert(reference);
			addReferenceToLookupImpl(reference);
			markReferenceLookupKeyDirty(currentLookupKey);
		}

		if (shouldLogReferenceLookupReference(reference)) {
			mwse::log::getLog()
				<< "[MWSE] Reference lookup rekeyed: ref=" << reference
				<< " refId=" << (getReferenceLookupLogId(reference) ? getReferenceLookupLogId(reference) : "<no id>")
				<< " previousKey=" << previousLookupKey
				<< " previousKeyId=" << (getReferenceLookupLogId(previousLookupKey) ? getReferenceLookupLogId(previousLookupKey) : "<no id>")
				<< " currentKey=" << currentLookupKey
				<< " currentKeyId=" << (getReferenceLookupLogId(currentLookupKey) ? getReferenceLookupLogId(currentLookupKey) : "<no id>")
				<< " trackedReferences=" << trackedReferences.size()
				<< std::endl;
		}
	}

	void ReferenceTracker::markReferencesLookupDirty(const TES3::PhysicalObject* object) {
		if (object == nullptr) {
			return;
		}

		markReferenceLookupKeyDirty(object);
	}

	bool ReferenceTracker::validate() {
		Lock lock;
		bool valid = true;

		auto& log = mwse::log::getLog();
		const auto reportFailure = [&](const auto& message) {
			log << "[MWSE] ReferenceTracker validation failed: " << message << std::endl;
			valid = false;
		};

		const auto dataHandler = TES3::DataHandler::get();
		const auto nonDynamicData = dataHandler ? dataHandler->nonDynamicData : nullptr;
		const auto cells = nonDynamicData ? nonDynamicData->cells : nullptr;
		if (cells == nullptr) {
			reportFailure("NonDynamicData::cells is null");
		}
		else {
			rebuildReferenceLookupCellOrder();
			size_t cellIndex = 0;
			for (const auto cell : *cells) {
				if (cell == nullptr) {
					++cellIndex;
					continue;
				}

				const auto orderIt = referenceLookupCellOrder.find(cell);
				if (orderIt == referenceLookupCellOrder.end()) {
					log << "[MWSE] ReferenceTracker validation failed: cell missing from referenceLookupCellOrder"
						<< " cell=" << cell << "(" << cell->getEditorName() << ")"
						<< " actualOrder=" << cellIndex
						<< std::endl;
					valid = false;
				}
				else if (orderIt->second != cellIndex) {
					log << "[MWSE] ReferenceTracker validation failed: cell order mismatch"
						<< " cell=" << cell
						<< " expectedOrder=" << cellIndex
						<< " trackedOrder=" << orderIt->second
						<< std::endl;
					valid = false;
				}

				const auto validateReferenceList = [&](const TES3::ReferenceList* referenceList, const char* listName) {
					if (referenceList == nullptr) {
						reportFailure(std::string("null reference list encountered for ") + listName);
						return;
					}

					for (const auto reference : *referenceList) {
						if (reference == nullptr) {
							continue;
						}

						if (trackedReferences.find(reference) == trackedReferences.end()) {
							log << "[MWSE] ReferenceTracker validation failed: untracked reference found in cell"
								<< " cell=" << cell
								<< " list=" << listName
								<< " reference=" << reference
								<< std::endl;
							valid = false;
						}
					}
				};

				validateReferenceList(&cell->actors, "actors");
				validateReferenceList(&cell->persistentRefs, "persistentRefs");
				validateReferenceList(&cell->temporaryRefs, "temporaryRefs");

				++cellIndex;
			}
		}

		for (const auto& [key, data] : referenceDataByObject) {
			for (const auto reference : data.references) {
				if (reference == nullptr) {
					log << "[MWSE] ReferenceTracker validation failed: null reference stored in referenceDataByObject"
						<< " key=" << key
						<< std::endl;
					valid = false;
					continue;
				}

				const auto lookupKey = getLookupKey(reference);
				if (lookupKey != key) {
					log << "[MWSE] ReferenceTracker validation failed: reference lookup key mismatch"
						<< " reference=" << reference
						<< " storedKey=" << key
						<< " lookupKey=" << lookupKey
						<< " baseObject=" << reference->baseObject
						<< std::endl;
					valid = false;
				}
			}
		}

		return valid;
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
		Lock lock;
		if (object == nullptr) {
			return;
		}

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

	const TES3::PhysicalObject* ReferenceTracker::getLookupKey(const TES3::BaseObject* object) {
		if (object == nullptr) {
			return nullptr;
		}

		const auto baseObject = object->getBaseObject();
		if (baseObject == nullptr) {
			return nullptr;
		}

		return baseObject->asPhysicalObject();
	}
}
