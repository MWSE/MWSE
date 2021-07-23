tes3combatSession
====================================================================================================

A structure that keeps track of combat session data.

Properties
----------------------------------------------------------------------------------------------------

`alchemyPriority`_ (`number`_)
    No description available.

`data`_
    No description available.

`distance`_ (`number`_)
    No description available.

`lastUseTimestamp`_ (`number`_)
    No description available.

`mobile`_ (`tes3mobileActor`_)
    Read-only. No description available.

`potionUseFlag`_ (`number`_)
    No description available.

`selectedAction`_ (`number`_)
    No description available.

`selectedAlchemy`_ (`tes3alchemy`_)
    No description available.

`selectedItem`_ (`tes3itemStack`_)
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
    tes3combatSession/data
    tes3combatSession/distance
    tes3combatSession/lastUseTimestamp
    tes3combatSession/mobile
    tes3combatSession/potionUseFlag
    tes3combatSession/selectedAction
    tes3combatSession/selectedAlchemy
    tes3combatSession/selectedItem
    tes3combatSession/selectedShield
    tes3combatSession/selectedSpell
    tes3combatSession/selectedWeapon
    tes3combatSession/selectionPriority

.. _`alchemyPriority`: tes3combatSession/alchemyPriority.html
.. _`data`: tes3combatSession/data.html
.. _`distance`: tes3combatSession/distance.html
.. _`lastUseTimestamp`: tes3combatSession/lastUseTimestamp.html
.. _`mobile`: tes3combatSession/mobile.html
.. _`potionUseFlag`: tes3combatSession/potionUseFlag.html
.. _`selectedAction`: tes3combatSession/selectedAction.html
.. _`selectedAlchemy`: tes3combatSession/selectedAlchemy.html
.. _`selectedItem`: tes3combatSession/selectedItem.html
.. _`selectedShield`: tes3combatSession/selectedShield.html
.. _`selectedSpell`: tes3combatSession/selectedSpell.html
.. _`selectedWeapon`: tes3combatSession/selectedWeapon.html
.. _`selectionPriority`: tes3combatSession/selectionPriority.html

Methods
----------------------------------------------------------------------------------------------------

`changeEquipment`_
    This method equips an item to the mobile in this combat sesion.

`selectAlchemyWithEffect`_ (`number`_)
    Selects the alchemy item with the greatest value, for a given effect ID and loads it into the selectedAlchemy property.

.. toctree::
    :hidden:

    tes3combatSession/changeEquipment
    tes3combatSession/selectAlchemyWithEffect

.. _`changeEquipment`: tes3combatSession/changeEquipment.html
.. _`selectAlchemyWithEffect`: tes3combatSession/selectAlchemyWithEffect.html

.. _`number`: ../../lua/type/number.html
.. _`tes3alchemy`: ../../lua/type/tes3alchemy.html
.. _`tes3equipmentStack`: ../../lua/type/tes3equipmentStack.html
.. _`tes3itemStack`: ../../lua/type/tes3itemStack.html
.. _`tes3mobileActor`: ../../lua/type/tes3mobileActor.html
.. _`tes3spell`: ../../lua/type/tes3spell.html
