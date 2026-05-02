#include "CSModelLoader.h"

namespace se::cs {
	NI::Node* ModelLoader::loadNIF(const char* path) const {
		const auto ModelLoader_loadNIF = reinterpret_cast<NI::Node*(__thiscall*)(const ModelLoader*, const char*)>(0x545E00);
		return ModelLoader_loadNIF(this, path);
	}

	bool ModelLoader::releaseNIF(const char* path) const {
		const auto ModelLoader_releaseNIF = reinterpret_cast<bool(__thiscall*)(const ModelLoader*, const char*)>(0x546180);
		return ModelLoader_releaseNIF(this, path);
	}

	bool ModelLoader::releaseNIF(NI::AVObject* nif) const {
		const auto ModelLoader_releaseNIF = reinterpret_cast<bool(__thiscall*)(const ModelLoader*, NI::AVObject*)>(0x5463F0);
		return ModelLoader_releaseNIF(this, nif);
	}

	ModelLoader::NifEntry* ModelLoader::getNifEntry(NI::AVObject* nif) const {
		for (auto b = 0; b < NIFs->bucketCount; ++b) {
			for (auto e = NIFs->buckets[b]; e; e = e->nextNode) {
				if (e->value->nif == nif) {
					return e->value;
				}
			}
		}
		return nullptr;
	}
}
