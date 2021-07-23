json.encode
====================================================================================================

Create a string representing the object. Object can be a table, a string, a number, a boolean, nil, json.null or any object with a function __tojson in its metatable. A table can only use strings and numbers as keys and its values have to be valid objects as well. It raises an error for any invalid data types or reference cycles.

Returns
----------------------------------------------------------------------------------------------------

`string`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

object (`unknown`_)
    No description available.

state (`table`_)
    No description available.

.. _`string`: ../../../lua/type/string.html
.. _`table`: ../../../lua/type/table.html
