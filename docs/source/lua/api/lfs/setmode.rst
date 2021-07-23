lfs.setmode
====================================================================================================

Sets the writing mode for a file.

Returns
----------------------------------------------------------------------------------------------------

`boolean`_, `nil`_. true in case of success, or nil on an error.

`string`_. If succesful, this will be the previous file mode used. In the case of an error, a string describing the issue.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

file (`ioFile`_)
    The file to set mode for.

mode (`string`_)
    The file's new mode. Can be either "binary" or "text".

.. _`boolean`: ../../../lua/type/boolean.html
.. _`ioFile`: ../../../lua/type/ioFile.html
.. _`nil`: ../../../lua/type/nil.html
.. _`string`: ../../../lua/type/string.html
