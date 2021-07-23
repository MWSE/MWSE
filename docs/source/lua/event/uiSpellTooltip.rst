uiSpellTooltip
====================================================================================================

The uiSpellTooltip event triggers when a new tooltip is displayed for a spell. The tooltip will be already be built.

Event Data
----------------------------------------------------------------------------------------------------

spell
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3spell`_. Read-only. The spell being examined.

tooltip
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3uiElement`_. Read-only. The newly created tooltip element. Due to timeouts and target changes, it may be destroyed at any time.

.. _`tes3spell`: ../../lua/type/tes3spell.html
.. _`tes3uiElement`: ../../lua/type/tes3uiElement.html
