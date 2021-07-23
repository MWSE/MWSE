tes3.updateInventoryGUI
====================================================================================================

Forces the GUI to update inventory-relevant elements for a given reference. This can be used after many calls to inventory-manipulating functions while passing updateGUI as false to resync inventory tiles, container weights, and companion data.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

reference (`tes3reference`_, `tes3mobileActor`_, `string`_)
    The reference to update GUI elements for.

.. _`string`: ../../../lua/type/string.html
.. _`tes3mobileActor`: ../../../lua/type/tes3mobileActor.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
