#pragma once

#include "TES3Defines.h"

#include "TES3IteratedList.h"
#include "TES3Spell.h"

namespace TES3 {
	struct SpellList {
		int unknown_0x0;
		IteratedList<Spell*> list;

		//
		// Other related this-call functions.
		//

		bool add(Spell*);
		bool add(const char*);
		bool add(const std::string&);

		bool remove(Spell*);
		bool remove(const char*);
		bool remove(const std::string&);

		bool contains(Spell*);
		bool contains(const char*);
		bool contains(const std::string&);

		Spell* getCheapest();

		//
		// Custom functions.
		//

		bool add_lua(sol::object value);
		bool remove_lua(sol::object value);
		bool contains_lua(sol::object value);

		bool containsType(SpellCastType::value_type);

	};
}
