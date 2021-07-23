createImageButton
====================================================================================================

Creates an interactive button composed of images for the **idle**, **over**, and **pressed** states. The texture must have power-of-2 dimensions (i.e. 16, 32, 64, 128, 256, 512, 1024); the final display size can be trimmed by setting width and height.

Returns
----------------------------------------------------------------------------------------------------

`tes3uiElement`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

id (`number`_)
    Optional. A registered identifier to help find this element later.

idle (`string`_)
    The path to the idle image. This path is relative to ``Data Files``.

over (`string`_)
    The path to the mouse hover image. This path is relative to ``Data Files``.

path (`string`_)
    The path to the mouse pressed image. This path is relative to ``Data Files``.

.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
.. _`tes3uiElement`: ../../../lua/type/tes3uiElement.html
