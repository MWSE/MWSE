re.compile
====================================================================================================

Compiles the given string and returns an equivalent lpeg pattern.
	
The given string may define either an expression or a grammar. The optional defs table provides 
extra Lua values to be used by the pattern.

Returns
----------------------------------------------------------------------------------------------------

`pattern`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

s (`string`_)
    The string to compile.

defs (`table`_)
    Optional. Extra lua values to be used by the pattern.

.. _`string`: ../../../lua/type/string.html
.. _`table`: ../../../lua/type/table.html
