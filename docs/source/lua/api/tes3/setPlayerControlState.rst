tes3.setPlayerControlState
====================================================================================================

Enables or disables player's controls state.

Returns
----------------------------------------------------------------------------------------------------

`boolean`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

enabled (`boolean`_)
    Default: ``false``. Setting this to false will disable any kind of control.

attack (`boolean`_)
    Default: ``false``. If this is false, it will block player from attacking.

jumping (`boolean`_)
    Default: ``false``. If this is false, it will block player from jumping.

magic (`boolean`_)
    Default: ``false``. If this is false, it will block player from using magic.

vanity (`boolean`_)
    Default: ``false``. If this is false, it will block player from going to vanity mode.

viewSwitch (`boolean`_)
    Default: ``false``. If this is false, it will block player changing view mod from 1st to 3rd person camera and vice versa.

.. _`boolean`: ../../../lua/type/boolean.html
