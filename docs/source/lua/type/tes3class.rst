tes3class
====================================================================================================

A core object representing a character class.

Properties
----------------------------------------------------------------------------------------------------

`attributes`_ (`table`_)
    Read-only. An array-style table of the two attribute IDs associated with the class.

`bartersAlchemy`_ (`boolean`_)
    If true, the class will barter alchemy items.

`bartersApparatus`_ (`boolean`_)
    If true, the class will barter apparatus items.

`bartersArmor`_ (`boolean`_)
    If true, the class will barter armor items.

`bartersBooks`_ (`boolean`_)
    If true, the class will barter book items.

`bartersClothing`_ (`boolean`_)
    If true, the class will barter clothing items.

`bartersEnchantedItems`_ (`boolean`_)
    If true, the class will barter enchanted items.

`bartersIngredients`_ (`boolean`_)
    If true, the class will barter ingredient items.

`bartersLights`_ (`boolean`_)
    If true, the class will barter light items.

`bartersLockpicks`_ (`boolean`_)
    If true, the class will barter lockpick items.

`bartersMiscItems`_ (`boolean`_)
    If true, the class will barter misc items.

`bartersProbes`_ (`boolean`_)
    If true, the class will barter probe items.

`bartersRepairTools`_ (`boolean`_)
    If true, the class will barter repair items.

`bartersWeapons`_ (`boolean`_)
    If true, the class will barter weapon items.

`blocked`_ (`boolean`_)
    The blocked state of the object.

`deleted`_ (`boolean`_)
    Read-only. The deleted state of the object.

`description`_ (`string`_)
    Loads from disk and returns the description of the class.

`disabled`_ (`boolean`_)
    Read-only. The disabled state of the object.

`id`_ (`string`_)
    Read-only. The unique identifier for the object.

`majorSkills`_ (`table`_)
    Read-only. An array-style table of the 5 skills IDs associated with the class' major skills.

`minorSkills`_ (`table`_)
    Read-only. An array-style table of the 5 skills IDs associated with the class' major skills.

`modified`_ (`boolean`_)
    The modification state of the object since the last save.

`name`_ (`string`_)
    The player-facing name for the object.

`objectFlags`_ (`number`_)
    Read-only. The raw flags of the object.

`objectType`_ (`number`_)
    Read-only. The type of object. Maps to values in tes3.objectType.

`offersEnchanting`_ (`boolean`_)
    If true, the class will offer repair services.

`offersRepairs`_ (`boolean`_)
    If true, the class will offer enchanting services.

`offersSpellmaking`_ (`boolean`_)
    If true, the class will offer spellmaking services.

`offersSpells`_ (`boolean`_)
    If true, the class will offer spell selling services.

`offersTraining`_ (`boolean`_)
    If true, the class will offer spell training services.

`persistent`_ (`boolean`_)
    The persistent flag of the object.

`playable`_ (`boolean`_)
    If true, the class is selectable at character generation.

`services`_ (`number`_)
    The services offered by the class. This is a bit field, and its values should typically be accessed through values such as bartersAlchemy.

`skills`_ (`table`_)
    Read-only. An array-style table of the 10 skills IDs associated with the class. For major or minor skills specifically, use the majorSkills and MinorSkills properties.

`sourceMod`_ (`string`_)
    Read-only. The filename of the mod that owns this object.

`sourceless`_ (`boolean`_)
    The soruceless flag of the object.

`specialization`_ (`number`_)
    The specialization for the class. Maps to the tes3.specialization table.

`supportsLuaData`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

.. toctree::
    :hidden:

    tes3class/attributes
    tes3class/bartersAlchemy
    tes3class/bartersApparatus
    tes3class/bartersArmor
    tes3class/bartersBooks
    tes3class/bartersClothing
    tes3class/bartersEnchantedItems
    tes3class/bartersIngredients
    tes3class/bartersLights
    tes3class/bartersLockpicks
    tes3class/bartersMiscItems
    tes3class/bartersProbes
    tes3class/bartersRepairTools
    tes3class/bartersWeapons
    tes3class/blocked
    tes3class/deleted
    tes3class/description
    tes3class/disabled
    tes3class/id
    tes3class/majorSkills
    tes3class/minorSkills
    tes3class/modified
    tes3class/name
    tes3class/objectFlags
    tes3class/objectType
    tes3class/offersEnchanting
    tes3class/offersRepairs
    tes3class/offersSpellmaking
    tes3class/offersSpells
    tes3class/offersTraining
    tes3class/persistent
    tes3class/playable
    tes3class/services
    tes3class/skills
    tes3class/sourceMod
    tes3class/sourceless
    tes3class/specialization
    tes3class/supportsLuaData

.. _`attributes`: tes3class/attributes.html
.. _`bartersAlchemy`: tes3class/bartersAlchemy.html
.. _`bartersApparatus`: tes3class/bartersApparatus.html
.. _`bartersArmor`: tes3class/bartersArmor.html
.. _`bartersBooks`: tes3class/bartersBooks.html
.. _`bartersClothing`: tes3class/bartersClothing.html
.. _`bartersEnchantedItems`: tes3class/bartersEnchantedItems.html
.. _`bartersIngredients`: tes3class/bartersIngredients.html
.. _`bartersLights`: tes3class/bartersLights.html
.. _`bartersLockpicks`: tes3class/bartersLockpicks.html
.. _`bartersMiscItems`: tes3class/bartersMiscItems.html
.. _`bartersProbes`: tes3class/bartersProbes.html
.. _`bartersRepairTools`: tes3class/bartersRepairTools.html
.. _`bartersWeapons`: tes3class/bartersWeapons.html
.. _`blocked`: tes3class/blocked.html
.. _`deleted`: tes3class/deleted.html
.. _`description`: tes3class/description.html
.. _`disabled`: tes3class/disabled.html
.. _`id`: tes3class/id.html
.. _`majorSkills`: tes3class/majorSkills.html
.. _`minorSkills`: tes3class/minorSkills.html
.. _`modified`: tes3class/modified.html
.. _`name`: tes3class/name.html
.. _`objectFlags`: tes3class/objectFlags.html
.. _`objectType`: tes3class/objectType.html
.. _`offersEnchanting`: tes3class/offersEnchanting.html
.. _`offersRepairs`: tes3class/offersRepairs.html
.. _`offersSpellmaking`: tes3class/offersSpellmaking.html
.. _`offersSpells`: tes3class/offersSpells.html
.. _`offersTraining`: tes3class/offersTraining.html
.. _`persistent`: tes3class/persistent.html
.. _`playable`: tes3class/playable.html
.. _`services`: tes3class/services.html
.. _`skills`: tes3class/skills.html
.. _`sourceMod`: tes3class/sourceMod.html
.. _`sourceless`: tes3class/sourceless.html
.. _`specialization`: tes3class/specialization.html
.. _`supportsLuaData`: tes3class/supportsLuaData.html

Methods
----------------------------------------------------------------------------------------------------

`__tojson`_ (`string`_)
    Serializes the object to json.

.. toctree::
    :hidden:

    tes3class/__tojson

.. _`__tojson`: tes3class/__tojson.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`table`: ../../lua/type/table.html
