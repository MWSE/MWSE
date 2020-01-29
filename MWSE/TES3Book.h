#pragma once

#include "TES3Defines.h"

#include "TES3Collections.h"
#include "TES3Item.h"

#include <string_view>

namespace TES3 {
	struct Book : Item {
		Book();
		~Book();

		Iterator<TES3::BaseObject> stolenList; // 0x30
		char * name; // 0x44
		Script * script; // 0x48
		char * model; // 0x4C
		char * icon; // 0x50
		float weight; // 0x54
		long value; // 0x58
		int bookType;
		int skillToRaise;
		int enchantCapacity;
		Enchantment * enchantment;
		int unknown_0x6C;

		//
		// Other related this-call functions.
		//

		const char* getBookText();
		void setDynamicText( std::string_view text );
		void clearDynamicText();

		// utility constants used in create function
		constexpr static auto BOOK_TYPE_BOOK = 0u;
		constexpr static auto BOOK_TYPE_SCROLL = 1u;
		constexpr static auto BOOK_TYPE_MIN = BOOK_TYPE_BOOK;
		constexpr static auto BOOK_TYPE_MAX = BOOK_TYPE_SCROLL;

	};
	static_assert(sizeof(Book) == 0x70, "TES3::Book failed size validation");
}
