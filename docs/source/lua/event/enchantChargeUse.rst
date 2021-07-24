enchantChargeUse
====================================================================================================

This event is triggered just before an on-strike or on-use enchantment is used by any actor, and also by the UI system to label enchant charges. It allows modification of the charge required to use an enchantment.

Event Data
----------------------------------------------------------------------------------------------------

isCast
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`boolean`_. True if the calculation is for a casting action, false if for UI enchant charge display.

charge
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`number`_. The charge required to cast the enchantment. May be modified.

source
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3enchantment`_. Read-only. The enchantment being used or examined by the UI.

caster
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3reference`_. Read-only. The caster of the enchantment.

.. _`boolean`: ../../lua/type/boolean.html
.. _`number`: ../../lua/type/number.html
.. _`tes3enchantment`: ../../lua/type/tes3enchantment.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
