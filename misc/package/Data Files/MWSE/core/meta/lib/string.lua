--- @meta

--- This library provides generic functions for string manipulation, such as finding and extracting substrings, and pattern matching. When indexing a string in Lua, the first character is at position 1 (not at 0, as in C). Indices are allowed to be negative and are interpreted as indexing backwards, from the end of the string. Thus, the last character is at position -1, and so on.
--- @class stringlib
string = {}

--- Returns true if a string ends with a given pattern.
--- @param s string No description yet available.
--- @param pattern string No description yet available.
--- @return boolean result No description yet available.
function string.endswith(s, pattern) end

--- This function creates a string, given various values. The format follows the printf format, with the additional option of %q to automatically quote a string.
--- @param format string The format string to use for the output.
--- @param unknown values Values to format into the given string.
--- @return string result No description yet available.
function string.format(format, unknown) end

--- Returns a string where one string has been inserted into another at a given position.
--- @param s1 string The string to insert into.
--- @param s2 string The string to insert.
--- @param position number The position to insert s2 into s1.
--- @return any result No description yet available.
function string.insert(s1, s2, position) end

--- Performs the logic of find, using a table of patterns.
--- 
--- If any of the available patterns match, the matching pattern is returned followed by the normal results of the find.
--- @param s string The string to perform finds on.
--- @param pattern table An array-style table that contains pattern strings.
--- @param index number Start index of the find.
--- @param plain boolean If true, a normal search instead of a pattern search will be performed.
function string.multifind(s, pattern, index, plain) end

--- Returns true if a string begins with a given pattern.
--- @param s string No description yet available.
--- @param pattern string No description yet available.
--- @return boolean result No description yet available.
function string.startswith(s, pattern) end

