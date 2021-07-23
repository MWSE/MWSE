lfs.touch
====================================================================================================

Set access and modification times of a file. Both access and modified times are measured in seconds, matching the OS time function.

Returns
----------------------------------------------------------------------------------------------------

`boolean`_, `nil`_. true in case of success, or nil on an error.

`string`_, `nil`_. In the case of an error, a string describing the issue.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

path (`string`_)
    The path to touch.

accessTime (`number`_)
    Optional. The new access time to set.

modifiedTime (`number`_)
    Optional. The new last modified time to set

.. _`boolean`: ../../../lua/type/boolean.html
.. _`nil`: ../../../lua/type/nil.html
.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
