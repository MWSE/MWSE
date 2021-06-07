tes3nonDynamicData
====================================================================================================

A child container from tes3dataHandler, where game data is stored.

Properties
----------------------------------------------------------------------------------------------------

`cells`_ (`tes3stlList`_)
    A collection of all cell objects.

`classes`_ (`tes3iterator`_)
    A collection of all class objects.

`dialogues`_ (`tes3iterator`_)
    A collection of all dialogue objects.

`factions`_ (`tes3iterator`_)
    A collection of all faction objects.

`globals`_ (`tes3iterator`_)
    A collection of all global variable objects.

`magicEffects`_ (`table`_)
    A table of references to all 143 magic effects.

`objects`_ (`tes3linkedList`_)
    A collection of all other game objects.

`races`_ (`tes3iterator`_)
    A collection of all race objects.

`regions`_ (`tes3iterator`_)
    A collection of all region objects.

`scripts`_ (`tes3iterator`_)
    A collection of all script objects.

`skills`_ (`table`_)
    A table of references to all 27 skills.

`soundGenerators`_ (`tes3iterator`_)
    A collection of all sound generator objects.

`sounds`_ (`tes3iterator`_)
    A collection of all sound objects.

`spells`_ (`tes3linkedList`_)
    A collection of all spell objects.

`startScripts`_ (`tes3iterator`_)
    A collection of all tes3startScript objects.

.. toctree::
    :hidden:

    tes3nonDynamicData/cells
    tes3nonDynamicData/classes
    tes3nonDynamicData/dialogues
    tes3nonDynamicData/factions
    tes3nonDynamicData/globals
    tes3nonDynamicData/magicEffects
    tes3nonDynamicData/objects
    tes3nonDynamicData/races
    tes3nonDynamicData/regions
    tes3nonDynamicData/scripts
    tes3nonDynamicData/skills
    tes3nonDynamicData/soundGenerators
    tes3nonDynamicData/sounds
    tes3nonDynamicData/spells
    tes3nonDynamicData/startScripts

.. _`cells`: tes3nonDynamicData/cells.html
.. _`classes`: tes3nonDynamicData/classes.html
.. _`dialogues`: tes3nonDynamicData/dialogues.html
.. _`factions`: tes3nonDynamicData/factions.html
.. _`globals`: tes3nonDynamicData/globals.html
.. _`magicEffects`: tes3nonDynamicData/magicEffects.html
.. _`objects`: tes3nonDynamicData/objects.html
.. _`races`: tes3nonDynamicData/races.html
.. _`regions`: tes3nonDynamicData/regions.html
.. _`scripts`: tes3nonDynamicData/scripts.html
.. _`skills`: tes3nonDynamicData/skills.html
.. _`soundGenerators`: tes3nonDynamicData/soundGenerators.html
.. _`sounds`: tes3nonDynamicData/sounds.html
.. _`spells`: tes3nonDynamicData/spells.html
.. _`startScripts`: tes3nonDynamicData/startScripts.html

Methods
----------------------------------------------------------------------------------------------------

`addNewObject`_ (`boolean`_)
    Inserts a newly created object into the proper collections.

`deleteObject`_
    Removes an object from the proper collections.

`findDialogue`_ (`tes3dialogue`_)
    Locates a dialogue for a given ID.

`findFirstCloneOfActor`_ (`tes3reference`_)
    Locates the first reference for a given object ID.

`findGlobalVariable`_ (`tes3globalVariable`_)
    Locates a global variable for a given ID.

`findScript`_ (`tes3script`_)
    Locates a script for a given ID.

`findSound`_ (`tes3sound`_)
    Locates a sound for a given ID.

`resolveObject`_ (`tes3baseObject`_)
    Locates a general object for a given ID.

.. toctree::
    :hidden:

    tes3nonDynamicData/addNewObject
    tes3nonDynamicData/deleteObject
    tes3nonDynamicData/findDialogue
    tes3nonDynamicData/findFirstCloneOfActor
    tes3nonDynamicData/findGlobalVariable
    tes3nonDynamicData/findScript
    tes3nonDynamicData/findSound
    tes3nonDynamicData/resolveObject

.. _`addNewObject`: tes3nonDynamicData/addNewObject.html
.. _`deleteObject`: tes3nonDynamicData/deleteObject.html
.. _`findDialogue`: tes3nonDynamicData/findDialogue.html
.. _`findFirstCloneOfActor`: tes3nonDynamicData/findFirstCloneOfActor.html
.. _`findGlobalVariable`: tes3nonDynamicData/findGlobalVariable.html
.. _`findScript`: tes3nonDynamicData/findScript.html
.. _`findSound`: tes3nonDynamicData/findSound.html
.. _`resolveObject`: tes3nonDynamicData/resolveObject.html

.. _`tes3globalVariable`: ../../lua/type/tes3globalVariable.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
.. _`tes3sound`: ../../lua/type/tes3sound.html
.. _`tes3dialogue`: ../../lua/type/tes3dialogue.html
.. _`boolean`: ../../lua/type/boolean.html
.. _`table`: ../../lua/type/table.html
.. _`tes3baseObject`: ../../lua/type/tes3baseObject.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
