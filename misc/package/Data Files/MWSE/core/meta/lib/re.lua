--- @meta

--- Provides a regular expression style syntax for pattern usage with lpeg.
--- 	
--- The documentation provided here is only meant to be a quick reference. Please review the complete
--- documentation (with in-depth discussion and many examples) at: 
--- http://www.inf.puc-rio.br/~roberto/lpeg/re.html
--- 	
--- There is also a copy in the MWSE repo at: deps/lpeg/re.html
--- @class relib
re = {}

--- Compiles the given string and returns an equivalent lpeg pattern.
--- 	
--- The given string may define either an expression or a grammar. The optional defs table provides 
--- extra Lua values to be used by the pattern.
--- @param s string The string to compile.
--- @param defs table Extra lua values to be used by the pattern.
--- @return pattern result No description yet available.
function re.compile(s, defs) end

--- Searches the given pattern in the given subject.
--- 	
--- If it finds a match, returns the index where this occurrence starts and the index where it ends. 
--- Otherwise, returns nil.
--- 
--- An optional numeric argument init makes the search starts at that position in the subject string.
--- As usual in Lua libraries, a negative value counts from the end. 
---
--- [Examples available in online documentation](https://mwse.readthedocs.io/en/latest/lua/api/re/find.html).
--- @param subject string The string to search.
--- @param pattern string The pattern to search with.
--- @param init number Start at this position in the subject string.
--- @return number result No description yet available.
function re.find(subject, pattern, init) end

--- Does a global substitution, replacing all occurrences of pattern in the given subject by replacement.
---
--- [Examples available in online documentation](https://mwse.readthedocs.io/en/latest/lua/api/re/gsub.html).
--- @param subject string The string to search.
--- @param pattern string The pattern to search with.
--- @param replacement string Replace all matches with this string.
--- @return string result No description yet available.
function re.gsub(subject, pattern, replacement) end

--- Matches the given pattern against the given subject, returning all captures.
---
--- [Examples available in online documentation](https://mwse.readthedocs.io/en/latest/lua/api/re/match.html).
--- @param subject string The string to search.
--- @param pattern string The pattern to search with.
--- @return string result No description yet available.
function re.match(subject, pattern) end

--- Updates the pre-defined character classes to the current locale.
function re.updatelocale() end

