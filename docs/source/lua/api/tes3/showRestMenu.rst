tes3.showRestMenu
====================================================================================================

This function opens the resting menu and returns true on success. If the player can't rest currently, it returns false.

Various parameters can be used to allow resting in situations not normally possible.

Returns
----------------------------------------------------------------------------------------------------

`boolean`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

checkForEnemies (`boolean`_)
    Default: ``true``. Perform a check weather there are enemies nearby before opening rest menu. If there are, false is returned.

checkForSolidGround (`boolean`_)
    Default: ``true``. Perform a check if the player is underwater. If underwater, false is returned.

checkSleepingIllegal (`boolean`_)
    Default: ``true``. Perform a check if the sleeping in the current cell is illegal. If illegal, false if returned.

checkIsWerewolf (`boolean`_)
    Default: ``true``. Perform a check if the player is Werewolf. If he/she is, then false is returned.

showMessage (`boolean`_)
    Default: ``true``. Should a messagebox be shown if the player can't open resting menu because some condition isn't met.

resting (`boolean`_)
    Default: ``true``. Should this be a rest?

waiting (`boolean`_)
    Default: ``false``. Or, is this a wait?

.. _`boolean`: ../../../lua/type/boolean.html
