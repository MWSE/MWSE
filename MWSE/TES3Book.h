#pragma once

#include "TES3Defines.h"

#include "TES3Item.h"
#include "TES3IteratedList.h"

namespace TES3 {
	enum class BookType {
		Book = 0,
		Scroll = 1
	};

	struct Book : Item {
		IteratedList<TES3::BaseObject*> stolenList; // 0x30
		char * name; // 0x44
		Script * script; // 0x48
		char * model; // 0x4C
		char * icon; // 0x50
		float weight; // 0x54
		long value; // 0x58
		BookType bookType;
		int skillToRaise;
		int enchantCapacity;
		Enchantment * enchantment;
		int unknown_0x6C;

		static constexpr auto OBJECT_TYPE = ObjectType::Book;

		Book();
		~Book();

		static constexpr auto BOOK_TEXT_CACHE = reinterpret_cast<char**>(0x7CA44C);

		//
		// Other related this-call functions.
		//

		const char* getBookText();

		void setIconPath(const char* path);

	};
	static_assert(sizeof(Book) == 0x70, "TES3::Book failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::Book)
