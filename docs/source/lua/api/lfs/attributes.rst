lfs.attributes
====================================================================================================

Returns a table with the file attributes corresponding to filepath (or nil followed by an error message and a system-dependent error code in case of error). If the second optional argument is given and is a string, then only the value of the named attribute is returned (this use is equivalent to lfs.attributes(filepath)[request_name], but the table is not created and only one attribute is retrieved from the O.S.). if a table is passed as the second argument, it (result_table) is filled with attributes and returned instead of a new table. The attributes are described as follows; attribute mode is a string, all the others are numbers, and the time related attributes use the same time reference of os.time: dev, mode, nlink, access, modification, change, size, permissions.

Returns
----------------------------------------------------------------------------------------------------

`table`_, `string`_, `number`_, `nil`_. All attributes in a table, the value of a specific attribute if one was requested, or nil on an error.

`string`_, `nil`_. In the case of an error, a string describing the issue.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

path (`string`_)
    The path, relative to the current directory, to get attributes for.

request (`string`_)
    Optional. The specific attribute to look for. If provided, the return value becomes the requested value (or nil) instead of a table.

resultTable (`table`_)
    Optional. If provided, this table will be reused as the result table. If not provided, a new table is created.

.. _`nil`: ../../../lua/type/nil.html
.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
.. _`table`: ../../../lua/type/table.html
