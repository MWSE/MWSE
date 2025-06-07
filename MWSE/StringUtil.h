#pragma once

#include "VMExecuteInterface.h"
#include "mwseString.h"

namespace mwse::string {
	namespace store {
		constexpr auto MWSE_STRING_STORE_FIRSTID = 40000;

		// Type of our string storage.
		typedef std::map<long, mwseString> StringMap_t;

		extern long nextId;

		extern StringMap_t store;

		mwseString& create(const std::string& value);

		mwseString& create(const char* value);

		mwseString& create(const char* value, size_t length);

		bool clear();

		bool exists(const long id);

		bool exists(const std::string& value);

		bool exists(const char* value);

		mwseString& get(const long id);

		mwseString& get(const std::string& value);

		mwseString& getOrCreate(const std::string& value);

		mwseString& getOrCreate(const char* value);

		mwseString& getOrCreate(const char* value, size_t length);
	}

	//
	// String pattern searching functions.
	//

	std::string interpolate(const std::string& format, mwse::VMExecuteInterface& virtualMachine, bool* suppressNull, std::string* badCodes);

	// Count how many results there should be based on the format string
	bool enumerate(const char* format, int& substitutions, bool& eolmode);

	// Extract the values from the string based on the format data and store them in the resultset array
	int secernate(const char* format, const char* string, long* results, int maxResults);

	//
	// Comparison helpers.
	//

	// Returns true if a == b, case insensitive.
	bool ciequal(char a, char b);

	// Returns true if a == b, case sensitive.
	bool equal(std::string_view a, std::string_view b);

	// Returns true if a == b, case insensitive.
	bool iequal(std::string_view a, std::string_view b);

	// Returns true if a == b, case insensitive, only compares maxCount characters.
	bool niequal(std::string_view a, std::string_view b, size_t maxCount);

	bool starts_with(const std::string_view& string, const std::string_view& substring);
	bool ends_with(const std::string_view& string, const std::string_view& substring);
	
	void strip_start(std::string& string, const std::string_view& substring);
	void strip_end(std::string& string, const std::string_view& substring);

	bool replace(std::string& str, const std::string_view from, const std::string_view to);

	//
	// Other string utility functions.
	//

	static inline bool is_printable(const char c) {
		return std::isprint(c) || std::isspace(c);
	}

	static inline std::string from_wstring(const std::wstring& wstr) {
		std::vector<char> buf(wstr.size());
		std::use_facet<std::ctype<wchar_t>>(std::locale{}).narrow(wstr.data(), wstr.data() + wstr.size(), '?', buf.data());
		return std::string(buf.data(), buf.size());
	}

	static inline void ltrim(std::string& s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) { return !std::isspace(ch); }));
	}

	static inline void rtrim(std::string& s) {
		s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) { return !std::isspace(ch); }).base(), s.end());
	}

	static inline void trim(std::string& s) {
		ltrim(s);
		rtrim(s);
	}

	static inline std::string ltrim_copy(std::string s) {
		ltrim(s);
		return s;
	}

	static inline std::string rtrim_copy(std::string s) {
		rtrim(s);
		return s;
	}

	static inline std::string trim_copy(std::string s) {
		trim(s);
		return s;
	}

	static inline void to_lower(std::string& s) {
		std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
	}
}
