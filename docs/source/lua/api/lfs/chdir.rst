lfs.chdir
====================================================================================================

Changes the current working directory to the given path. Note that this is global. Do not use this function without restoring the previous directory at the end of your logic. Failure to do so can cause unrecoverable errors.

Returns
----------------------------------------------------------------------------------------------------

`boolean`_, `nil`_. true in case of success, or nil on an error.

`string`_, `nil`_. In the case of an error, a string describing the issue.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

path (`string`_)
    The path to change to.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`nil`: ../../../lua/type/nil.html
.. _`string`: ../../../lua/type/string.html
