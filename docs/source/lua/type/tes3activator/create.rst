create
====================================================================================================

Create a new `tes3activator`_ object which will be stored as part of the current saved game.

The example below create an activator object that could be used to create a placeable reference later on :
.. code-block:: lua
	:linenos:

local activatorId = "create_activator_id"
local activatorName = "create_activator_name"

local activator = tes3activator.create( {
	id = activatorId,
	name = activatorName,
	mesh = [[f\Act_banner_Hla_Oad.nif]]
} )

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

id (`string`_)
    Optional. The new object's ID. Must be unique if provided.

name (`string`_)
    Default: ``Activator``. The new object's name.

script (`tes3script`_)
    Optional. A script to attach to the object.

mesh (`string`_)
    Default: ````. The mesh to use for the object.

objectFlags (`number`_)
    Default: ``0``. The object flags initially set. Force set as modified.

.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
.. _`tes3activator`: ../../../lua/type/tes3activator.html
