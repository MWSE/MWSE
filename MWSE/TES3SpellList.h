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

		//
		// Container wrapping methods.
		// This allows lua and C++ to interface with this container as if it were the wrapped object.
		//

		using value_type = decltype(list)::value_type;
		using size_type = decltype(list)::size_type;
		using difference_type = decltype(list)::difference_type;
		using pointer = decltype(list)::pointer;
		using const_pointer = decltype(list)::const_pointer;
		using reference = decltype(list)::const_reference;
		using const_reference = decltype(list)::const_reference;
		using iterator = decltype(list)::iterator;
		using const_iterator = decltype(list)::const_iterator;
		using reverse_iterator = decltype(list)::reverse_iterator;
		using const_reverse_iterator = decltype(list)::const_reverse_iterator;

		iterator begin() const { return list.begin(); }
		iterator end() const { return list.end(); }
		reverse_iterator rbegin() const { return list.rbegin(); }
		reverse_iterator rend() const { return list.rend(); }
		const_iterator cbegin() const { return list.cbegin(); }
		const_iterator cend() const { return list.cend(); }
		const_reverse_iterator crbegin() const { return list.crbegin(); }
		const_reverse_iterator crend() const { return list.crend(); }
		size_type size() const noexcept { return list.size(); }
		bool empty() const noexcept { return list.empty(); }
	};
}
