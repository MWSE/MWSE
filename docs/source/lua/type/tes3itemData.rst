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

`data`_ (`table`_)
    A generic lua table that data can be written to, and synced to/from the save. All information stored must be valid for serialization to json.

`owner`_ (`tes3npc`_, `tes3faction`_, `nil`_)
    The script associated with the scriptVariables.

`requirement`_ (`tes3globalVariable`_, `number`_, `nil`_)
    A requirement, typically associated with ownership and when the player may freely interact with an object. The type depends on the owner. Faction owners provide a required rank as a number, while NPCs provide a global variable to check.

`script`_ (`tes3script`_)
    The script associated with the scriptVariables.

`scriptVariables`_ (`tes3scriptVariables`_)
    Access to the structure where individual mwscript data is stored.

`soul`_ (`tes3actor`_)
    Only available on misc items. The actor that is stored inside the soul gem.

`timeLeft`_ (`number`_)
    The time remaining on a light. Provides incorrect values on non-light items, which instead have a condition property.

.. toctree::
    :hidden:

    tes3itemData/charge
    tes3itemData/condition
    tes3itemData/context
    tes3itemData/data
    tes3itemData/owner
    tes3itemData/requirement
    tes3itemData/script
    tes3itemData/scriptVariables
    tes3itemData/soul
    tes3itemData/timeLeft

.. _`charge`: tes3itemData/charge.html
.. _`condition`: tes3itemData/condition.html
.. _`context`: tes3itemData/context.html
.. _`data`: tes3itemData/data.html
.. _`owner`: tes3itemData/owner.html
.. _`requirement`: tes3itemData/requirement.html
.. _`script`: tes3itemData/script.html
.. _`scriptVariables`: tes3itemData/scriptVariables.html
.. _`soul`: tes3itemData/soul.html
.. _`timeLeft`: tes3itemData/timeLeft.html

.. _`tes3globalVariable`: ../../lua/type/tes3globalVariable.html
.. _`table`: ../../lua/type/table.html
.. _`number`: ../../lua/type/number.html
.. _`tes3npc`: ../../lua/type/tes3npc.html
.. _`tes3actor`: ../../lua/type/tes3actor.html
.. _`tes3faction`: ../../lua/type/tes3faction.html
.. _`nil`: ../../lua/type/nil.html
