#pragma once

#include "CSBaseObject.h"

namespace se::cs {
	struct Script : BaseObject {
		struct Header {
			char name[32]; // 0x0
			int numShorts; // 0x20
			int numLongs; // 0x24
			int numFloats; // 0x28
			size_t scriptDataSize; // 0x2C
			size_t localVariableSize; // 0x30
		};
		struct LocalData {
			short* shortVarValues;
			int* longVarValues;
			float* floatVarValues;
			unsigned int flags;
			int listEvents;
		};
		Header header; // 0x10
		const char** shortVarNames; // 0x44
		const char** longVarNames; // 0x48
		const char** floatVarNames; // 0x4C
		const char* variableNames; // 0x50
		const char* text; // 0x54
		void* bytecode; // 0x58
		LocalData defaultLocalData; // 0x5C

		const char* getShortVarName(int index) const;
		const char* getLongVarName(int index) const;
		const char* getFloatVarName(int index) const;

		bool search(const std::string_view& needle, const SearchSettings& settings, std::regex* regex = nullptr) const;
	};
	static_assert(sizeof(Script) == 0x70, "Script failed size validation");
}
