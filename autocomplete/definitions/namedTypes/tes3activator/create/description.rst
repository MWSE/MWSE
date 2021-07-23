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
