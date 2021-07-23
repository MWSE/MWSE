tes3ui.registerProperty
====================================================================================================

Registers a property name, returning a Property. Once a property is registered, this function always returns the same Property.

The registry namespace is shared between Property and UI_ID. It is advisable to use a namespace prefix to avoid collisions with other mods.

Returns
----------------------------------------------------------------------------------------------------

`number`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

s (`string`_)
    No description available.

.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
