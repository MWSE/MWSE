niTriShapeData
====================================================================================================

Contains the geometry data for an `NiTriShape` object.

Properties
----------------------------------------------------------------------------------------------------

`bounds`_ (`niBound`_)
    The bounds of the object.

`colors`_ (`niColorA`_)
    Read-only. The color for the object.

`normals`_ (`tes3vector3`_)
    Read-only. The normals list for the object.

`references`_ (`string`_)
    Read-only. The number of references that exist for the given object. When this value hits zero, the object's memory is freed.

`runTimeTypeInformation`_ (`niRTTI`_)
    The runtime type information for this object.

`texCoords`_ (`tes3vector2`_)
    Read-only. The array of texture coordinates.

`textureSets`_ (`number`_)
    The number of texture coordinate sets in the data.

`textures`_ (`tes3vector2`_)
    Read-only. The array of texture coordinates.

`triangleCount`_ (`number`_)
    The triangle count of the object.

`triangleCount`_ (`number`_)
    The triangle count of the object.

`uniqueID`_ (`number`_)
    A unique ID for this model, assigned at model creation.

`vertexCount`_ (`number`_)
    Read-only. The vertex count for the object.

`vertices`_ (`tes3vector3`_)
    Read-only. The array of vertex position data.

.. toctree::
    :hidden:

    niTriShapeData/bounds
    niTriShapeData/colors
    niTriShapeData/normals
    niTriShapeData/references
    niTriShapeData/runTimeTypeInformation
    niTriShapeData/texCoords
    niTriShapeData/textureSets
    niTriShapeData/textures
    niTriShapeData/triangleCount
    niTriShapeData/triangleCount
    niTriShapeData/uniqueID
    niTriShapeData/vertexCount
    niTriShapeData/vertices

.. _`bounds`: niTriShapeData/bounds.html
.. _`colors`: niTriShapeData/colors.html
.. _`normals`: niTriShapeData/normals.html
.. _`references`: niTriShapeData/references.html
.. _`runTimeTypeInformation`: niTriShapeData/runTimeTypeInformation.html
.. _`texCoords`: niTriShapeData/texCoords.html
.. _`textureSets`: niTriShapeData/textureSets.html
.. _`textures`: niTriShapeData/textures.html
.. _`triangleCount`: niTriShapeData/triangleCount.html
.. _`triangleCount`: niTriShapeData/triangleCount.html
.. _`uniqueID`: niTriShapeData/uniqueID.html
.. _`vertexCount`: niTriShapeData/vertexCount.html
.. _`vertices`: niTriShapeData/vertices.html

Methods
----------------------------------------------------------------------------------------------------

`clone`_ (`niObject`_)
    Creates a copy of this object.

`copy`_
    Creates a copy of the data. An optional table of filters can be passed to remove information from the copy.

`isInstanceOfType`_ (`boolean`_)
    Determines if the object is of a given type, or of a type derived from the given type. Types can be found in the tes3.niType table.

`isOfType`_ (`boolean`_)
    Determines if the object is of a given type. Types can be found in the tes3.niType table.

`markAsChanged`_
    Tells the renderer that the object has changed. Should be called after you have finished changing any vertex data.

If you have altered vertex positions, you may need to also call `updateModelBound`_. You should call it if vertices have been moved outside the bounds of the original model.

`updateModelBound`_
    Updates the geometry bounds to match the vertex data. You should call it if vertices have been moved outside the bounds of the original model, or if the effective bounds have become significantly smaller. 

If you already know the effective radius of the vertex data, you could more efficiently set the bounds directly instead of calling this function.

.. toctree::
    :hidden:

    niTriShapeData/clone
    niTriShapeData/copy
    niTriShapeData/isInstanceOfType
    niTriShapeData/isOfType
    niTriShapeData/markAsChanged
    niTriShapeData/updateModelBound

.. _`clone`: niTriShapeData/clone.html
.. _`copy`: niTriShapeData/copy.html
.. _`isInstanceOfType`: niTriShapeData/isInstanceOfType.html
.. _`isOfType`: niTriShapeData/isOfType.html
.. _`markAsChanged`: niTriShapeData/markAsChanged.html
.. _`updateModelBound`: niTriShapeData/updateModelBound.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niColorA`: ../../lua/type/niColorA.html
.. _`niObject`: ../../lua/type/niObject.html
.. _`niRTTI`: ../../lua/type/niRTTI.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`tes3vector2`: ../../lua/type/tes3vector2.html
.. _`tes3vector3`: ../../lua/type/tes3vector3.html
