os.setClipboardText
====================================================================================================

Sets the current clipboard text. If nil is passed, the clipboard will be cleared instead. If either operation fails, the function returns false.

Returns
----------------------------------------------------------------------------------------------------

`boolean`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

text (`string`_, `nil`_)
    Optional. The text to put in the clipboard. If nil is provided instead, then the clipboard will be cleared.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`nil`: ../../../lua/type/nil.html
.. _`string`: ../../../lua/type/string.html
