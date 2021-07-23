--- @meta

--- Provides pattern-matching based on Parsing Expression Grammars (PEGs).
--- 	
--- The documentation provided here is only meant to be a quick reference. Please review the complete
--- documentation (with in-depth discussion and many examples) at: 
--- http://www.inf.puc-rio.br/~roberto/lpeg/
--- 
--- There is also a copy in the MWSE repo at: deps/lpeg/lpeg.html
--- @class lpeglib
lpeg = {}

--- Returns a pattern that matches only if the input string at the current position is preceded by pattern p.
--- 	
--- Pattern p must match only strings with some fixed length, and it cannot contain captures.
--- 
--- Like the and predicate, this pattern never consumes any input, independently of success or failure.
--- @param p pattern The input pattern.
--- @return pattern result No description yet available.
function lpeg.B(p) end

--- Create a simple capture pattern, which captures the substring of the subject that matches pattern p.
--- 	
--- The captured value is a string. If p has other captures, their values are returned after this one.
--- @param p pattern The pattern to capture against.
--- @return pattern result No description yet available.
function lpeg.C(p) end

--- Create an argument capture.
--- 	
--- This pattern matches the empty string and produces the value given as the nth extra argument given 
--- in the call to lpeg.match.
--- @param n number The argument to produce.
--- @return pattern result No description yet available.
function lpeg.Carg(n) end

--- Create a back capture.
--- 	
--- This pattern matches the empty string and produces the values produced by the most recent group 
--- capture named name (where name can be any Lua value).
--- 
--- Most recent means the last complete outermost group capture with the given name. A Complete capture
--- means that the entire pattern corresponding to the capture has matched. An Outermost capture means 
--- that the capture is not inside another complete capture.
--- 
--- In the same way that LPeg does not specify when it evaluates captures, it does not specify whether 
--- it reuses values previously produced by the group or re-evaluates them.
--- @param name value The group capture to produce.
--- @return pattern result No description yet available.
function lpeg.Cb(name) end

--- Create a constant capture.
--- 	
--- This pattern matches the empty string and produces all given values as its captured values.
--- @param unknown values The values to produce.
--- @return pattern result No description yet available.
function lpeg.Cc(unknown) end

--- Create a fold capture.
--- 	
--- If pattern p produces a list of captures C1 C2 ... Cn, this capture will produce the value 
--- function f(...f(f(C1, C2), C3)..., Cn), that is, it will fold (or accumulate, or reduce) the 
--- captures from p using function f.
--- 
--- This capture assumes that p should produce at least one capture with at least one value (of any 
--- type), which becomes the initial value of an accumulator. (If you need a specific initial value, 
--- you may prefix a constant capture to p.) For each subsequent capture, LPeg calls f with this
--- accumulator as the first argument and all values produced by the capture as extra arguments; the 
--- first result from this call becomes the new value for the accumulator. The final value of the 
--- accumulator becomes the captured value.
--- @param p pattern The pattern to fold with f.
--- @param f function The function to use to fold p.
--- @return pattern result No description yet available.
function lpeg.Cf(p, f) end

--- Create a group capture.
--- 	
--- It groups all values returned by pattern p into a single capture. The group may be anonymous (if no
--- name is given) or named with the given name (which can be any non-nil Lua value).
--- 
--- An anonymous group serves to join values from several captures into a single capture. A named 
--- group has a different behavior. In most situations, a named group returns no values at all. Its 
--- values are only relevant for a following back capture or when used inside a table capture.
--- @param p pattern The pattern to capture with.
--- @param name value The name of the group.
--- @return pattern result No description yet available.
function lpeg.Cg(p, name) end

--- Create a match-time capture.
--- 	
--- Unlike all other captures, this one is evaluated immediately when a match occurs (even if it is 
--- part of a larger pattern that fails later). It forces the immediate evaluation of all its nested
--- captures and then calls function f.
--- 
--- The given function gets as arguments the entire subject, the current position (after the match of 
--- pattern p), plus any capture values produced by p.
--- 
--- The first value returned by f defines how the match happens. If the call returns a number, 
--- the match succeeds and the returned number becomes the new current position. (Assuming a subject s
--- and current position i, the returned number must be in the range [i, len(s) + 1].) If the call 
--- returns true, the match succeeds without consuming any input. (So, to return true is equivalent to 
--- return i.) If the call returns false, nil, or no value, the match fails.
--- 
--- Any extra values returned by f become the values produced by the capture. 
--- @param p pattern The pattern to capture with.
--- @param f function The function to call to evaluate matches.
--- @return pattern result No description yet available.
function lpeg.Cmt(p, f) end

--- Create a position capture.
--- 	
--- It matches the empty string and captures the position in the subject where the match occurs. The 
--- captured value is a number.
--- @return pattern result No description yet available.
function lpeg.Cp() end

--- Create a substitution capture, which captures the substring of the subject that matches pattern p, with substitutions.
--- 	
--- For any capture inside p with a value, the substring that matched the capture is replaced by the 
--- capture value (which should be a string). The final captured value is the string resulting from all
--- replacements.
--- @param p pattern The pattern to capture with.
--- @return pattern result No description yet available.
function lpeg.Cs(p) end

--- Create a table capture.
--- 	
--- This capture returns a table with all values from all anonymous captures made by pattern p inside 
--- this table in successive integer keys, starting at 1. Moreover, for each named capture group 
--- created by p, the first value of the group is put into the table with the group name as its key. 
--- The captured value is only the table.
--- @param p pattern The pattern to capture with.
--- @return pattern result No description yet available.
function lpeg.Ct(p) end

--- Return a table with patterns for matching some character classes according to the current locale.
--- 	
--- The table has fields named alnum, alpha, cntrl, digit, graph, lower, print, punct, space, upper,
--- and xdigit, each one containing a correspondent pattern. Each pattern matches any single character 
--- that belongs to its class.
--- 
--- If called with an argument table, then it creates those fields inside the given table and returns 
--- that table. 
--- @param t table The table to update.
--- @return table result No description yet available.
function lpeg.locale(t) end

--- Attempt to match the given pattern against the subject string.
--- 	
--- If the match succeeds, return the index in the subject of the first character after the match, or
--- the captured values (if the pattern captured any value).
--- 
--- An optional numeric argument init makes the match start at that position in the subject string. As 
--- usual in Lua libraries, a negative value counts from the end.
--- 
--- Unlike typical pattern-matching functions, match works only in anchored mode; that is, it tries to 
--- match the pattern with a prefix of the given subject string (at position init), not with an 
--- arbitrary substring of the subject. So, if we want to find a pattern anywhere in a string, we must
--- either write a loop in Lua or write a pattern that matches anywhere. This second approach is easy 
--- and quite efficient; see examples in the full documentation for details.
---
--- [Examples available in online documentation](https://mwse.readthedocs.io/en/latest/lua/api/lpeg/match.html).
--- @param p pattern The pattern to match.
--- @param subject string The string to match against.
--- @param init number Start the match at this position in subject.
--- @return values result No description yet available.
function lpeg.match(p, subject, init) end

--- Converts the given value into a proper lpeg pattern.
--- 	
--- If the argument is a pattern, it is returned unmodified.
--- 
--- If the argument is a string, it is translated to a pattern that matches the string literally.
--- 
--- If the argument is a non-negative number n, the result is a pattern that matches exactly n 
--- characters.
--- 
--- If the argument is a negative number -n, the result is a pattern that succeeds only if the input
--- string has less than n characters left: lpeg.P(-n) is equivalent to -lpeg.P(n). See the unary minus
--- operation in the full documentation for details.
--- 
--- If the argument is a boolean, the result is a pattern that always succeeds or always fails 
--- (according to the boolean value), without consuming any input.
--- 
--- If the argument is a table, it is interpreted as a grammar. See Grammars in the full documentation 
--- for details.
--- 
--- If the argument is a function, returns a pattern equivalent to a match-time capture over the empty 
--- string.
--- @param value multiple The value to convert to a pattern.
--- @return pattern result No description yet available.
function lpeg.P(value) end

--- Returns a pattern that matches any single character belonging to one of the given ranges.
--- 	
--- Each range is a string xy of length 2, representing all characters with code between the codes of x and y (both inclusive).
--- 
--- As an example, the pattern lpeg.R("09") matches any digit, and lpeg.R("az", "AZ") matches any ASCII letter.
---
--- [Examples available in online documentation](https://mwse.readthedocs.io/en/latest/lua/api/lpeg/R.html).
--- @param p pattern The input pattern.
--- @return pattern result No description yet available.
function lpeg.R(p) end

--- Returns a pattern that matches any single character that appears in the given string.
--- 	
--- As an example, the pattern lpeg.S("+-*/") matches any arithmetic operator.
--- 
--- Note that, if s is a character (that is, a string of length 1), then lpeg.P(s) is equivalent to 
--- lpeg.S(s) which is equivalent to lpeg.R(s..s). Note also that both lpeg.S("") and lpeg.R() are 
--- patterns that always fail.
--- @param s string The input string.
--- @return pattern result No description yet available.
function lpeg.S(s) end

--- Set a limit for the size of the backtrack stack used by LPeg to track calls and choices.
--- 
--- The default limit is 400. Most well-written patterns need little backtrack levels and therefore you seldom
--- need to change this limit; before changing it you should try to rewrite your pattern to avoid the need for 
--- extra space. Nevertheless, a few useful patterns may overflow. Also, with recursive grammars, subjects with
--- deep recursion may also need larger limits.
--- @param max number The new stack size.
function lpeg.setmaxstack(max) end

--- If the given value is a pattern, return the string "pattern". Otherwise return nil.
--- @param value value The item to evaulate.
--- @return string result No description yet available.
function lpeg.type(value) end

--- Create a non-terminal (a variable) for a grammar.
--- 	
--- The created non-terminal refers to the rule indexed by v in the enclosing grammar. See Grammars
--- in the full documentation for details.
--- @param v string The index of the desired rule.
--- @return pattern result No description yet available.
function lpeg.V(v) end

--- Return a string with the running version of lpeg.
--- @return string result No description yet available.
function lpeg.version() end

