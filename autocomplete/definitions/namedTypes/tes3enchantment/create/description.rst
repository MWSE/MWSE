Create a new `tes3enchantment`_ object which will be stored as part of the current saved game.

The example below create an enchantment object that could be used to be bound on an item later on.
.. code-block:: lua
	:linenos:

local enchantmentId = "_dynamic_enchantment_id"

local enchantment = tes3enchantment.create( {
  id = enchantmentId,
  castType = tes3.enchantmentType.castOnce,
  chargeCost = 2,
  maxCharge = 12
} )

-- effects can be set later on using enchantment.effects table
