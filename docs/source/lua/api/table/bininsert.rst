table.bininsert
====================================================================================================

Inserts a given value through BinaryInsert into the table sorted by [, comp].

If 'comp' is given, then it must be a function that receives two table elements, and returns true when the first is less than the second, e.g. comp = function(a, b) return a > b end, will give a sorted table, with the biggest value on position 1. [, comp] behaves as in table.sort(table, value [, comp]) returns the index where 'value' was inserted

Returns
----------------------------------------------------------------------------------------------------

`number`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

t (`table`_)
    No description available.

value (`unknown`_)
    No description available.

comp (`unknown`_)
    Optional. No description available.

.. _`number`: ../../../lua/type/number.html
.. _`table`: ../../../lua/type/table.html
