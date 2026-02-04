#include "NIUtil.h"

#include "StringUtil.h"

#include "NIAVObject.h"

constexpr auto NI_global_pick = 0x7D12E8;

constexpr auto NI_getAssociatedReference = 0x4C3C40;

namespace NI {
	Pick* getGlobalPick() {
		return *reinterpret_cast<Pick**>(NI_global_pick);
	}

	TES3::Reference* getAssociatedReference(AVObject* object) {
		return reinterpret_cast<TES3::Reference * (__cdecl*)(AVObject*)>(NI_getAssociatedReference)(object);
	}

	bool passesTraverseFilters(const AVObject* object, const std::unordered_set<unsigned int>& typeFilters, std::string_view prefix) {
		bool passesFilter = typeFilters.empty() ? true : false;
		if (!passesFilter) {
			for (const auto type : typeFilters) {
				if (object->isInstanceOfType((uintptr_t)type)) {
					passesFilter = true;
					break;
				}
			}
		}
		bool passesPrefix = prefix.empty() ? true : object->name && mwse::string::starts_with(object->name, prefix);
		return passesFilter && passesPrefix;
	}
}