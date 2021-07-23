lpeg.B
====================================================================================================

Returns a pattern that matches only if the input string at the current position is preceded by pattern p.
	
Pattern p must match only strings with some fixed length, and it cannot contain captures.

Like the and predicate, this pattern never consumes any input, independently of success or failure.

Returns
----------------------------------------------------------------------------------------------------

`pattern`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

p (`pattern`_)
    The input pattern.

