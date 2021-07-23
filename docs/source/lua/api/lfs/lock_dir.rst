lfs.lock_dir
====================================================================================================

Creates a lockfile (called lockfile.lfs) in path if it does not exist and returns the lock. If the lock already exists checks if it's stale, using the second parameter (default for the second parameter is INT_MAX, which in practice means the lock will never be stale. The lock object has a method to free it.

Returns
----------------------------------------------------------------------------------------------------

`lfsLock`_, `nil`_. The lock object to manage, or nil in the case of an error.

`string`_, `nil`_. In the case of an error, a string describing the issue. In particular, if the lock exists and is not stale it returns the "File exists" message.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

path (`string`_)
    The path to lock.

.. _`lfsLock`: ../../../lua/type/lfsLock.html
.. _`nil`: ../../../lua/type/nil.html
.. _`string`: ../../../lua/type/string.html
