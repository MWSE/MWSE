return {
	type = "lib",
	description = [[This library provides generic functions for string manipulation, such as finding and extracting substrings, and pattern matching. When indexing a string in Lua, the first character is at position `1` (not at `0`, as in C). Indices are allowed to be negative and are interpreted as indexing backwards, from the end of the string. Thus, the last character is at position `-1`, and so on.
Several functions in this library (e.g., `find`, `gfind`, `gsub`), make use of [Lua patterns](https://www.lua.org/pil/20.2.html).]],
	link = "https://www.lua.org/manual/5.1/manual.html#5.4",
}