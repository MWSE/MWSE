#pragma once

#include "NINode.h"

#include "NIHashMap.h"

namespace se::cs {
	struct ModelLoader {
		struct NifEntry {
			const char* path;
			NI::Node* nif;
			unsigned short count;
		};

		NI::HashMap<const char*, NifEntry*>* NIFs; // 0x0
		NI::HashMap<const char*, void*>* KFs; // 0x4

		NI::Node* loadNIF(const char* path) const;
		bool releaseNIF(const char* path) const;
		bool releaseNIF(NI::AVObject* nif) const;

		NifEntry* getNifEntry(NI::AVObject* nif) const;
	};
	static_assert(sizeof(ModelLoader) == 0x8, "CS::ModelLoader failed size validation");
}
