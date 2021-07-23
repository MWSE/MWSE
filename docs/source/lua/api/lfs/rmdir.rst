lfs.rmdir
====================================================================================================

Removes an existing directory. Unless using the recursive parameter, only empty directories can be removed.

Returns
----------------------------------------------------------------------------------------------------

`boolean`_, `nil`_. true in case of success, or nil on an error.

`string`_, `nil`_. In the case of an error, a string describing the issue.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

dirname (`string`_)
    The name of the directory to remove.

recursive (`boolean`_)
    Default: ``false``. If true, all contents of the directory will be removed as well.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`nil`: ../../../lua/type/nil.html
.. _`string`: ../../../lua/type/string.html
