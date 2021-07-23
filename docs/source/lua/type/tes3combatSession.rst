tes3combatSession
====================================================================================================

A structure that keeps track of combat session data.

Properties
----------------------------------------------------------------------------------------------------

`alchemyPriority`_ (`number`_)
    No description available.

`distance`_ (`number`_)
    No description available.

`mobile`_ (`tes3mobileActor`_)
    No description available.

`selectedAction`_ (`number`_)
    No description available.

`selectedAlchemy`_ (`tes3alchemy`_)
    No description available.

`selectedShield`_ (`tes3equipmentStack`_)
    Read-only. No description available.

`selectedSpell`_ (`tes3spell`_)
    No description available.

`selectedWeapon`_ (`tes3equipmentStack`_)
    Read-only. No description available.

`selectionPriority`_ (`number`_)
    No description available.

.. toctree::
    :hidden:

    tes3combatSession/alchemyPriority
    tes3combatSession/distance
    tes3combatSession/mobile
    tes3combatSession/selectedAction
    tes3combatSession/selectedAlchemy
    tes3combatSession/selectedShield
    tes3combatSession/selectedSpell
    tes3combatSession/selectedWeapon
    tes3combatSession/selectionPriority

.. _`alchemyPriority`: tes3combatSession/alchemyPriority.html
.. _`distance`: tes3combatSession/distance.html
.. _`mobile`: tes3combatSession/mobile.html
.. _`selectedAction`: tes3combatSession/selectedAction.html
.. _`selectedAlchemy`: tes3combatSession/selectedAlchemy.html
.. _`selectedShield`: tes3combatSession/selectedShield.html
.. _`selectedSpell`: tes3combatSession/selectedSpell.html
.. _`selectedWeapon`: tes3combatSession/selectedWeapon.html
.. _`selectionPriority`: tes3combatSession/selectionPriority.html

Methods
----------------------------------------------------------------------------------------------------

`selectAlchemyWithEffect`_ (`number`_)
    Selects the alchemy item with the greatest value, for a given effect ID and loads it into the selectedAlchemy property.

.. toctree::
    :hidden:

    tes3combatSession/selectAlchemyWithEffect

.. _`selectAlchemyWithEffect`: tes3combatSession/selectAlchemyWithEffect.html

.. _`number`: ../../lua/type/number.html
.. _`tes3alchemy`: ../../lua/type/tes3alchemy.html
.. _`tes3equipmentStack`: ../../lua/type/tes3equipmentStack.html
.. _`tes3mobileActor`: ../../lua/type/tes3mobileActor.html
.. _`tes3spell`: ../../lua/type/tes3spell.html
