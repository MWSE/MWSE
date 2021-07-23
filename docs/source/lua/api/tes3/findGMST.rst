tes3.findGMST
====================================================================================================

Fetches the core game object that represents a game setting. While this function accepts a name, it is recommended to use the tes3.GMST constants.

Returns
----------------------------------------------------------------------------------------------------

`tes3gameSetting`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

id (`number`_, `string`_)
    No description available.

Examples
----------------------------------------------------------------------------------------------------

Retrieve value of a GMST
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This example shows how to read a value of a GMST and how to change it.

.. code-block:: lua

    local oldGMST = tes3.findGMST("sServiceTrainingTitle").value

    -- oldGMST is now "Training"

    -- Now let's change the message to something more appropriate.
    tes3.findGMST("sServiceTrainingTitle").value = "Cheat"


.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
.. _`tes3gameSetting`: ../../../lua/type/tes3gameSetting.html
