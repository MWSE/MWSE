lfs.link
====================================================================================================

Creates a link. This is a hard link by default.

Returns
----------------------------------------------------------------------------------------------------

`boolean`_, `nil`_. true in case of success, or nil on an error.

`string`_, `nil`_. In the case of an error, a string describing the issue.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

old (`string`_)
    The object to link to.

new (`string`_)
    The name of the link.

symlink (`boolean`_)
    Optional. If true, the link will be symbolic instead.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`nil`: ../../../lua/type/nil.html
.. _`string`: ../../../lua/type/string.html
