lfs.unlock
====================================================================================================

Unlocks a file or a part of it. This function works on open files; the file handle should be specified as the first argument.

Returns
----------------------------------------------------------------------------------------------------

`boolean`_, `nil`_. true if the operation was successful, or nil in the case of an error.

`string`_, `nil`_. In the case of an error, a string describing the issue. In particular, if the lock exists and is not stale it returns the "File exists" message.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

filehandle (`ioFile`_)
    The file handle to unlock.

mode (`string`_)
    "r" for a read/shared lock, or "w" for a write/exclusive unlock.

start (`number`_)
    Optional. The starting point of the file to unlock.

length (`number`_)
    Optional. The length of the file, relative to start, to unlock.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`ioFile`: ../../../lua/type/ioFile.html
.. _`nil`: ../../../lua/type/nil.html
.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
