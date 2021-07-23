lpeg.Cs
====================================================================================================

Create a substitution capture, which captures the substring of the subject that matches pattern p, with substitutions.
	
For any capture inside p with a value, the substring that matched the capture is replaced by the 
capture value (which should be a string). The final captured value is the string resulting from all
replacements.

Returns
----------------------------------------------------------------------------------------------------

`pattern`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

p (`pattern`_)
    The pattern to capture with.

