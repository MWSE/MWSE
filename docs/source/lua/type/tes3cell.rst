tes3cell
====================================================================================================

An exterior or interior game area.

Properties
----------------------------------------------------------------------------------------------------

`activators`_ (`tes3referenceList`_)
    One of the three reference collections for a cell.

`actors`_ (`tes3referenceList`_)
    One of the three reference collections for a cell.

`ambientColor`_ (`tes3packedColor`_)
    The cell's ambient color. Only available on interior cells.

`behavesAsExterior`_ (`boolean`_)
    If true, the cell behaves as an exterior instead of an interior for certain properties. Only available on interior cells.

`cellFlags`_ (`number`_)
    A numeric representation of the packed bit flags for the cell, typically accessed from other properties.

`deleted`_ (`boolean`_)
    The deleted state of the object.

`disabled`_ (`boolean`_)
    The disabled state of the object.

`fogColor`_ (`tes3packedColor`_)
    The cell's fog color. Only available on interior cells.

`fogDensity`_ (`number`_)
    The cell's fog density. Only available on interior cells.

`gridX`_ (`number`_)
    The cell's X grid coordinate. Only available on exterior cells.

`gridY`_ (`number`_)
    The cell's Y grid coordinate. Only available on exterior cells.

`hasWater`_ (`boolean`_)
    If true, the cell has water. Only applies to interior cells.

`id`_ (`string`_)
    The unique identifier for the object.

`isInterior`_ (`boolean`_)
    If true, the cell is an interior.

`modified`_ (`boolean`_)
    The modification state of the object since the last save.

`name`_ (`string`_)
    The name and id of the cell.

`objectFlags`_ (`number`_)
    The raw flags of the object.

`objectType`_ (`number`_)
    The type of object. Maps to values in tes3.objectType.

`region`_ (`tes3region`_)
    The region associated with the cell. Only available on exterior cells, or interior cells that behave as exterior cells.

`restingIsIllegal`_ (`boolean`_)
    If true, the player may not rest in the cell.

`sourceMod`_ (`string`_)
    The filename of the mod that owns this object.

`statics`_ (`tes3referenceList`_)
    One of the three reference collections for a cell.

`sunColor`_ (`tes3packedColor`_)
    The cell's sun color. Only available on interior cells.

`waterLevel`_ (`number`_)
    The water level in the cell. Only available on interior cells.

.. toctree::
    :hidden:

    tes3cell/activators
    tes3cell/actors
    tes3cell/ambientColor
    tes3cell/behavesAsExterior
    tes3cell/cellFlags
    tes3cell/deleted
    tes3cell/disabled
    tes3cell/fogColor
    tes3cell/fogDensity
    tes3cell/gridX
    tes3cell/gridY
    tes3cell/hasWater
    tes3cell/id
    tes3cell/isInterior
    tes3cell/modified
    tes3cell/name
    tes3cell/objectFlags
    tes3cell/objectType
    tes3cell/region
    tes3cell/restingIsIllegal
    tes3cell/sourceMod
    tes3cell/statics
    tes3cell/sunColor
    tes3cell/waterLevel

.. _`activators`: tes3cell/activators.html
.. _`actors`: tes3cell/actors.html
.. _`ambientColor`: tes3cell/ambientColor.html
.. _`behavesAsExterior`: tes3cell/behavesAsExterior.html
.. _`cellFlags`: tes3cell/cellFlags.html
.. _`deleted`: tes3cell/deleted.html
.. _`disabled`: tes3cell/disabled.html
.. _`fogColor`: tes3cell/fogColor.html
.. _`fogDensity`: tes3cell/fogDensity.html
.. _`gridX`: tes3cell/gridX.html
.. _`gridY`: tes3cell/gridY.html
.. _`hasWater`: tes3cell/hasWater.html
.. _`id`: tes3cell/id.html
.. _`isInterior`: tes3cell/isInterior.html
.. _`modified`: tes3cell/modified.html
.. _`name`: tes3cell/name.html
.. _`objectFlags`: tes3cell/objectFlags.html
.. _`objectType`: tes3cell/objectType.html
.. _`region`: tes3cell/region.html
.. _`restingIsIllegal`: tes3cell/restingIsIllegal.html
.. _`sourceMod`: tes3cell/sourceMod.html
.. _`statics`: tes3cell/statics.html
.. _`sunColor`: tes3cell/sunColor.html
.. _`waterLevel`: tes3cell/waterLevel.html

Methods
----------------------------------------------------------------------------------------------------

`iterateReferences`_
    Used in a for loop, iterates over objects in the cell.

.. toctree::
    :hidden:

    tes3cell/iterateReferences

.. _`iterateReferences`: tes3cell/iterateReferences.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`string`: ../../lua/type/string.html
.. _`number`: ../../lua/type/number.html
.. _`tes3region`: ../../lua/type/tes3region.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
.. _`tes3packedColor`: ../../lua/type/tes3packedColor.html
