niTexturingProperty
====================================================================================================

A rendering property that controls the methods used to filter texture pixels, and blend texture colors and vertex colors.

Properties
----------------------------------------------------------------------------------------------------

`applyMode`_ (`niTexturingPropertyApplyMode`_)
    The texture apply mode. The texture apply mode controls how the vertex colors are blended with the texture color.

`maps`_ (`niTexturingPropertyMapTArray`_)
    The maps of texture property. Accessible as an array.

`name`_ (`string`_)
    The human-facing name of the given object.

`references`_ (`string`_)
    The number of references that exist for the given object. When this value hits zero, the object's memory is freed.

`runTimeTypeInformation`_ (`niRTTI`_)
    The runtime type information for this object.

`type`_ (`niPropertyType`_)
    The unique class identifier number of the given rendering property.

.. toctree::
    :hidden:

    niTexturingProperty/applyMode
    niTexturingProperty/maps
    niTexturingProperty/name
    niTexturingProperty/references
    niTexturingProperty/runTimeTypeInformation
    niTexturingProperty/type

.. _`applyMode`: niTexturingProperty/applyMode.html
.. _`maps`: niTexturingProperty/maps.html
.. _`name`: niTexturingProperty/name.html
.. _`references`: niTexturingProperty/references.html
.. _`runTimeTypeInformation`: niTexturingProperty/runTimeTypeInformation.html
.. _`type`: niTexturingProperty/type.html

Methods
----------------------------------------------------------------------------------------------------

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

.. toctree::
    :hidden:

    niTexturingProperty/clone
    niTexturingProperty/isInstanceOfType
    niTexturingProperty/isOfType
    niTexturingProperty/prependController
    niTexturingProperty/removeAllControllers
    niTexturingProperty/removeController

.. _`clone`: niTexturingProperty/clone.html
.. _`isInstanceOfType`: niTexturingProperty/isInstanceOfType.html
.. _`isOfType`: niTexturingProperty/isOfType.html
.. _`prependController`: niTexturingProperty/prependController.html
.. _`removeAllControllers`: niTexturingProperty/removeAllControllers.html
.. _`removeController`: niTexturingProperty/removeController.html

.. _`niObject`: ../../lua/type/niObject.html
.. _`boolean`: ../../lua/type/boolean.html
.. _`string`: ../../lua/type/string.html
.. _`niRTTI`: ../../lua/type/niRTTI.html
