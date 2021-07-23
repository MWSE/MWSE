lpeg.P
====================================================================================================

Converts the given value into a proper lpeg pattern.
	
If the argument is a pattern, it is returned unmodified.

If the argument is a string, it is translated to a pattern that matches the string literally.

If the argument is a non-negative number n, the result is a pattern that matches exactly n 
characters.

If the argument is a negative number -n, the result is a pattern that succeeds only if the input
string has less than n characters left: lpeg.P(-n) is equivalent to -lpeg.P(n). See the unary minus
operation in the full documentation for details.

If the argument is a boolean, the result is a pattern that always succeeds or always fails 
(according to the boolean value), without consuming any input.

If the argument is a table, it is interpreted as a grammar. See Grammars in the full documentation 
for details.

If the argument is a function, returns a pattern equivalent to a match-time capture over the empty 
string.

Returns
----------------------------------------------------------------------------------------------------

`pattern`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

value (`multiple`_)
    The value to convert to a pattern.

