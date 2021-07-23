lpeg.Cb
====================================================================================================

Create a back capture.
	
This pattern matches the empty string and produces the values produced by the most recent group 
capture named name (where name can be any Lua value).

Most recent means the last complete outermost group capture with the given name. A Complete capture
means that the entire pattern corresponding to the capture has matched. An Outermost capture means 
that the capture is not inside another complete capture.

In the same way that LPeg does not specify when it evaluates captures, it does not specify whether 
it reuses values previously produced by the group or re-evaluates them.

Returns
----------------------------------------------------------------------------------------------------

`pattern`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

name (`value`_)
    The group capture to produce.

