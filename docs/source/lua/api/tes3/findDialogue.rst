tes3.findDialogue
====================================================================================================

Locates a root dialogue topic that can then be filtered down for a specific actor to return a specific dialogue info. For example, a type of ``tes3.dialogueType.greeting`` and a page of ``tes3.dialoguePage.greeting.greeting0`` will return the "Greeting 0" topic.

Returns
----------------------------------------------------------------------------------------------------

`tes3dialogue`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

type (`number`_)
    The type of dialogue to look for. Uses tes3.dialogueType.* constants.

page (`number`_)
    The page of dialogue to fetch. Uses tes3.dialoguePage.* constants.

.. _`number`: ../../../lua/type/number.html
.. _`tes3dialogue`: ../../../lua/type/tes3dialogue.html
