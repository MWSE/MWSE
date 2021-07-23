niTexturingProperty
====================================================================================================

A rendering property that controls the methods used to filter texture pixels, and blend texture colors and vertex colors.

Properties
----------------------------------------------------------------------------------------------------

`applyMode`_ (`niTexturingPropertyApplyMode`_)
    The texture apply mode. The texture apply mode controls how the vertex colors are blended with the texture color.

`baseMap`_ (`niTexturingPropertyMap`_, `nil`_)
    Gets or set the base map. Can be set to nil to delete it. Note that doing so will free the memory of the map, causing all existing handles to it to become invalid.

`bumpMap`_ (`niTexturingPropertyMap`_, `nil`_)
    Gets or set the bump map. Can be set to nil to delete it. Note that doing so will free the memory of the map, causing all existing handles to it to become invalid.

`canAddDecal`_ (`boolean`_)
    Read-only. If true, the texturing property supports additional decal maps.

`darkMap`_ (`niTexturingPropertyMap`_, `nil`_)
    Gets or set the dark map. Can be set to nil to delete it. Note that doing so will free the memory of the map, causing all existing handles to it to become invalid.

`decalCount`_ (`number`_)
    Read-only. The number of decals currently on the texturing property.

`detailMap`_ (`niTexturingPropertyMap`_, `nil`_)
    Gets or set the detail map. Can be set to nil to delete it. Note that doing so will free the memory of the map, causing all existing handles to it to become invalid.

`glossMap`_ (`niTexturingPropertyMap`_, `nil`_)
    Gets or set the gloss map. Can be set to nil to delete it. Note that doing so will free the memory of the map, causing all existing handles to it to become invalid.

`glowMap`_ (`niTexturingPropertyMap`_, `nil`_)
    Gets or set the glow map. Can be set to nil to delete it. Note that doing so will free the memory of the map, causing all existing handles to it to become invalid.

`maps`_ (`niTexturingPropertyMapTArray`_)
    The maps of texture property. Accessible as an array.

`name`_ (`string`_)
    The human-facing name of the given object.

`references`_ (`string`_)
    Read-only. The number of references that exist for the given object. When this value hits zero, the object's memory is freed.

`runTimeTypeInformation`_ (`niRTTI`_)
    The runtime type information for this object.

`type`_ (`niPropertyType`_)
    The unique class identifier number of the given rendering property.

.. toctree::
    :hidden:

    niTexturingProperty/applyMode
    niTexturingProperty/baseMap
    niTexturingProperty/bumpMap
    niTexturingProperty/canAddDecal
    niTexturingProperty/darkMap
    niTexturingProperty/decalCount
    niTexturingProperty/detailMap
    niTexturingProperty/glossMap
    niTexturingProperty/glowMap
    niTexturingProperty/maps
    niTexturingProperty/name
    niTexturingProperty/references
    niTexturingProperty/runTimeTypeInformation
    niTexturingProperty/type

.. _`applyMode`: niTexturingProperty/applyMode.html
.. _`baseMap`: niTexturingProperty/baseMap.html
.. _`bumpMap`: niTexturingProperty/bumpMap.html
.. _`canAddDecal`: niTexturingProperty/canAddDecal.html
.. _`darkMap`: niTexturingProperty/darkMap.html
.. _`decalCount`: niTexturingProperty/decalCount.html
.. _`detailMap`: niTexturingProperty/detailMap.html
.. _`glossMap`: niTexturingProperty/glossMap.html
.. _`glowMap`: niTexturingProperty/glowMap.html
.. _`maps`: niTexturingProperty/maps.html
.. _`name`: niTexturingProperty/name.html
.. _`references`: niTexturingProperty/references.html
.. _`runTimeTypeInformation`: niTexturingProperty/runTimeTypeInformation.html
.. _`type`: niTexturingProperty/type.html

Methods
----------------------------------------------------------------------------------------------------

`addDecalMap`_
    Attempts to create a new decal map. If successful, it returns both the new map and the index it was created in.

`clone`_ (`niObject`_)
    Creates a copy of this object.

`isInstanceOfType`_ (`boolean`_)
    Determines if the object is of a given type, or of a type derived from the given type. Types can be found in the tes3.niType table.

`isOfType`_ (`boolean`_)
    Determines if the object is of a given type. Types can be found in the tes3.niType table.

`prependController`_
    Add a controller to the object as the first controller.

`removeAllControllers`_
    Removes all controllers.

`removeController`_
    Removes a controller from the object.

`removeDecalMap`_
    Attempts to remove a decal at a given index.

.. toctree::
    :hidden:

    niTexturingProperty/addDecalMap
    niTexturingProperty/clone
    niTexturingProperty/isInstanceOfType
    niTexturingProperty/isOfType
    niTexturingProperty/prependController
    niTexturingProperty/removeAllControllers
    niTexturingProperty/removeController
    niTexturingProperty/removeDecalMap

.. _`addDecalMap`: niTexturingProperty/addDecalMap.html
.. _`clone`: niTexturingProperty/clone.html
.. _`isInstanceOfType`: niTexturingProperty/isInstanceOfType.html
.. _`isOfType`: niTexturingProperty/isOfType.html
.. _`prependController`: niTexturingProperty/prependController.html
.. _`removeAllControllers`: niTexturingProperty/removeAllControllers.html
.. _`removeController`: niTexturingProperty/removeController.html
.. _`removeDecalMap`: niTexturingProperty/removeDecalMap.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niObject`: ../../lua/type/niObject.html
.. _`niRTTI`: ../../lua/type/niRTTI.html
.. _`niTexturingPropertyMap`: ../../lua/type/niTexturingPropertyMap.html
.. _`nil`: ../../lua/type/nil.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
