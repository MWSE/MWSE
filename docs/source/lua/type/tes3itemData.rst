tes3itemData
====================================================================================================

A set of variables that differentiates one item from another.

Properties
----------------------------------------------------------------------------------------------------

`charge`_ (`number`_)
    The charge of the item. Provides incorrect values on misc items, which instead have a soul actor.

`condition`_ (`number`_)
    The condition/health of the item. Provides incorrect values on light items, which instead have a timeLeft property.

`context`_ (`tes3scriptContext`_)
    Returns an ease of use script context for variable access.

`count`_ (`number`_)
    The total number of identical items represented by this data. Reserved for use by in-world references. It should always be 1 for items in inventories. Inventories utilize the stack count in tes3itemStack.

`data`_ (`table`_)
    A generic lua table that data can be written to, and synced to/from the save. All information stored must be valid for serialization to json. For data that doesn't persist, use tempData.

`owner`_ (`tes3npc`_, `tes3faction`_, `nil`_)
    The script associated with the scriptVariables.

`requirement`_ (`tes3globalVariable`_, `number`_, `nil`_)
    A requirement, typically associated with ownership and when the player may freely interact with an object. The type depends on the owner. Faction owners provide a required rank as a number, while NPCs provide a global variable to check.

`script`_ (`tes3script`_)
    Read-only. The script associated with the scriptVariables.

`scriptVariables`_ (`tes3scriptVariables`_)
    Access to the structure where individual mwscript data is stored.

`soul`_ (`tes3actor`_)
    Only available on misc items. The actor that is stored inside the soul gem.

`tempData`_ (`table`_)
    A generic lua table that data can be written to, but is not synced to/from a save.

`timeLeft`_ (`number`_)
    The time remaining on a light. Provides incorrect values on non-light items, which instead have a condition property.

.. toctree::
    :hidden:

    tes3itemData/charge
    tes3itemData/condition
    tes3itemData/context
    tes3itemData/count
    tes3itemData/data
    tes3itemData/owner
    tes3itemData/requirement
    tes3itemData/script
    tes3itemData/scriptVariables
    tes3itemData/soul
    tes3itemData/tempData
    tes3itemData/timeLeft

.. _`charge`: tes3itemData/charge.html
.. _`condition`: tes3itemData/condition.html
.. _`context`: tes3itemData/context.html
.. _`count`: tes3itemData/count.html
.. _`data`: tes3itemData/data.html
.. _`owner`: tes3itemData/owner.html
.. _`requirement`: tes3itemData/requirement.html
.. _`script`: tes3itemData/script.html
.. _`scriptVariables`: tes3itemData/scriptVariables.html
.. _`soul`: tes3itemData/soul.html
.. _`tempData`: tes3itemData/tempData.html
.. _`timeLeft`: tes3itemData/timeLeft.html

.. _`nil`: ../../lua/type/nil.html
.. _`number`: ../../lua/type/number.html
.. _`table`: ../../lua/type/table.html
.. _`tes3actor`: ../../lua/type/tes3actor.html
.. _`tes3faction`: ../../lua/type/tes3faction.html
.. _`tes3globalVariable`: ../../lua/type/tes3globalVariable.html
.. _`tes3npc`: ../../lua/type/tes3npc.html
