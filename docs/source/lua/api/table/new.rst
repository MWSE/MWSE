table.new
====================================================================================================

This creates a pre-sized table. This is useful for big tables if the final table size is known and automatic table resizing is too expensive.

Returns
----------------------------------------------------------------------------------------------------

`table`_. The pre-sized table that was created.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

narray (`number`_)
    A hint for how many elements the table will have as a sequence.

nhash (`number`_)
    A hint for how many other elements the table will have.

.. _`number`: ../../../lua/type/number.html
.. _`table`: ../../../lua/type/table.html
