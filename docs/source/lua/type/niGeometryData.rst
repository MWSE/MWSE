niGeometryData
====================================================================================================

        NiGeometryData objects contain the geometry data necessary to render an NiGeometry object. When an NiGeometry-based object is created, the actual geometry data is stored in an attached NiGeometryData object. 
    

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

`uniqueID`_ (`number`_)
    A unique ID for this model, assigned at model creation.

`vertexCount`_ (`number`_)
    Read-only. The vertex count for the object.

`vertices`_ (`tes3vector3`_)
    Read-only. The array of vertex position data.

.. toctree::
    :hidden:

    niGeometryData/bounds
    niGeometryData/colors
    niGeometryData/normals
    niGeometryData/references
    niGeometryData/runTimeTypeInformation
    niGeometryData/texCoords
    niGeometryData/textureSets
    niGeometryData/textures
    niGeometryData/uniqueID
    niGeometryData/vertexCount
    niGeometryData/vertices

.. _`bounds`: niGeometryData/bounds.html
.. _`colors`: niGeometryData/colors.html
.. _`normals`: niGeometryData/normals.html
.. _`references`: niGeometryData/references.html
.. _`runTimeTypeInformation`: niGeometryData/runTimeTypeInformation.html
.. _`texCoords`: niGeometryData/texCoords.html
.. _`textureSets`: niGeometryData/textureSets.html
.. _`textures`: niGeometryData/textures.html
.. _`uniqueID`: niGeometryData/uniqueID.html
.. _`vertexCount`: niGeometryData/vertexCount.html
.. _`vertices`: niGeometryData/vertices.html

Methods
----------------------------------------------------------------------------------------------------

`clone`_ (`niObject`_)
    Creates a copy of this object.

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

    niGeometryData/clone
    niGeometryData/isInstanceOfType
    niGeometryData/isOfType
    niGeometryData/markAsChanged
    niGeometryData/updateModelBound

.. _`clone`: niGeometryData/clone.html
.. _`isInstanceOfType`: niGeometryData/isInstanceOfType.html
.. _`isOfType`: niGeometryData/isOfType.html
.. _`markAsChanged`: niGeometryData/markAsChanged.html
.. _`updateModelBound`: niGeometryData/updateModelBound.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niColorA`: ../../lua/type/niColorA.html
.. _`niObject`: ../../lua/type/niObject.html
.. _`niRTTI`: ../../lua/type/niRTTI.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`tes3vector2`: ../../lua/type/tes3vector2.html
.. _`tes3vector3`: ../../lua/type/tes3vector3.html
