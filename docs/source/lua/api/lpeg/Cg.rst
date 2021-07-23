lpeg.Cg
====================================================================================================

Create a group capture.
	
It groups all values returned by pattern p into a single capture. The group may be anonymous (if no
name is given) or named with the given name (which can be any non-nil Lua value).

An anonymous group serves to join values from several captures into a single capture. A named 
group has a different behavior. In most situations, a named group returns no values at all. Its 
values are only relevant for a following back capture or when used inside a table capture.

Returns
----------------------------------------------------------------------------------------------------

`pattern`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

p (`pattern`_)
    The pattern to capture with.

name (`value`_)
    Optional. The name of the group.

