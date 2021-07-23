lpeg.Cmt
====================================================================================================

Create a match-time capture.
	
Unlike all other captures, this one is evaluated immediately when a match occurs (even if it is 
part of a larger pattern that fails later). It forces the immediate evaluation of all its nested
captures and then calls function f.

The given function gets as arguments the entire subject, the current position (after the match of 
pattern p), plus any capture values produced by p.

The first value returned by f defines how the match happens. If the call returns a number, 
the match succeeds and the returned number becomes the new current position. (Assuming a subject s
and current position i, the returned number must be in the range [i, len(s) + 1].) If the call 
returns true, the match succeeds without consuming any input. (So, to return true is equivalent to 
return i.) If the call returns false, nil, or no value, the match fails.

Any extra values returned by f become the values produced by the capture. 

Returns
----------------------------------------------------------------------------------------------------

`pattern`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

p (`pattern`_)
    The pattern to capture with.

f (`function`_)
    The function to call to evaluate matches.

.. _`function`: ../../../lua/type/function.html
