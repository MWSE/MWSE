lpeg.locale
====================================================================================================

Return a table with patterns for matching some character classes according to the current locale.
	
The table has fields named alnum, alpha, cntrl, digit, graph, lower, print, punct, space, upper,
and xdigit, each one containing a correspondent pattern. Each pattern matches any single character 
that belongs to its class.

If called with an argument table, then it creates those fields inside the given table and returns 
that table. 

Returns
----------------------------------------------------------------------------------------------------

`table`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

t (`table`_)
    Optional. The table to update.

.. _`table`: ../../../lua/type/table.html
