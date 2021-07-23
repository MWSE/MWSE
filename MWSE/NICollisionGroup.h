#pragma once

#include "NIAVObject.h"
#include "NITArray.h"

namespace NI {
	struct CollisionGroup {
		struct Record {
			NI::AVObject* sgCollisionRoot;
			NI::AVObject* sgObject;
			int maxDepth;
			int binSize;
		};
		static_assert(sizeof(CollisionGroup::Record) == 0x10, "NI::CollisionGroup::Record failed size validation");

		struct Intersect {
			NI::AVObject* sgCollisionRoot0;
			NI::AVObject* sgCollisionRoot1;
			NI::AVObject* sgObject0;
			NI::AVObject* sgObject1;
			float fTime;
			TES3::Vector3 point;
			TES3::Vector3 normal0;
			TES3::Vector3 normal1;
		};
		static_assert(sizeof(CollisionGroup::Intersect) == 0x38, "NI::CollisionGroup::Intersect failed size validation");

		NI::TArray<NI::AVObject*> colliders;
		NI::TArray<NI::AVObject*> collidees;


		bool containsCollider(AVObject* obj);
		void addCollider(AVObject* obj);
		void removeCollider(AVObject* obj);
	};
	static_assert(sizeof(CollisionGroup) == 0x30, "NI::CollisionGroup failed size validation");
}
