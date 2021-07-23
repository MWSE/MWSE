tes3spellList
====================================================================================================

A list of spells. Used by actors, birthsigns, and races.

Properties
----------------------------------------------------------------------------------------------------

`iterator`_ (`tes3iterator`_)
    Read-only. A collection of spells. Can be iterated over using *pairs*.

.. toctree::
    :hidden:

    tes3spellList/iterator

.. _`iterator`: tes3spellList/iterator.html

Methods
----------------------------------------------------------------------------------------------------

`add`_ (`boolean`_)
    Adds a spell to the list.

`contains`_ (`boolean`_)
    Checks if the specified spell is contained in the spell list.

`containsType`_ (`tes3spell`_)
    Finds the first spell with the type provided.

`getCheapest`_ (`tes3spell`_)
    Finds the castable spell in the list with the lowest casting cost.

`remove`_ (`boolean`_)
    Removes a spell from the list.

.. toctree::
    :hidden:

    tes3spellList/add
    tes3spellList/contains
    tes3spellList/containsType
    tes3spellList/getCheapest
    tes3spellList/remove

.. _`add`: tes3spellList/add.html
.. _`contains`: tes3spellList/contains.html
.. _`containsType`: tes3spellList/containsType.html
.. _`getCheapest`: tes3spellList/getCheapest.html
.. _`remove`: tes3spellList/remove.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
.. _`tes3spell`: ../../lua/type/tes3spell.html
