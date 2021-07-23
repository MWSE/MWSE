table.values
====================================================================================================

Returns an array-style table of all values in the given table, t. Optionally, it will sort the returned table.

Returns
----------------------------------------------------------------------------------------------------

`table`_. An array of all table values.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

t (`table`_)
    The table to get values for.

sort (`boolean`_, `function`_, `nil`_)
    Optional. If true, the returned table will be sorted. If a function is passed, the table will be sorted using the given function.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`function`: ../../../lua/type/function.html
.. _`nil`: ../../../lua/type/nil.html
.. _`table`: ../../../lua/type/table.html
