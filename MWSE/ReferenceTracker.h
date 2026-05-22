#pragma once

#include "TES3Defines.h"

namespace mwse {
	class ReferenceTracker {
	public:
		ReferenceTracker() = delete;

		class Lock {
		public:
			Lock();
			~Lock();
		};

		static const std::vector<TES3::Reference*>& getReferences(const TES3::PhysicalObject* object);

		static void trackReferenceForLookup(TES3::Reference* reference);
		static void untrackReferenceForLookup(TES3::Reference* reference);
		static void markReferencesLookupDirty(const TES3::PhysicalObject* object);

		static void invalidateObject(TES3::BaseObject* object);

		static const TES3::PhysicalObject* getLookupKey(const TES3::BaseObject* object);
	};
}
