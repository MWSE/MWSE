niAlphaProperty
====================================================================================================

A rendering property that manages alpha blending, enabling and disabling it, and setting blending functions. The property affects alpha testing, which can provide a performance boost.

Properties
----------------------------------------------------------------------------------------------------

`alphaTestRef`_ (`string`_)
    No description.

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

    niAlphaProperty/alphaTestRef
    niAlphaProperty/name
    niAlphaProperty/references
    niAlphaProperty/runTimeTypeInformation
    niAlphaProperty/type

.. _`alphaTestRef`: niAlphaProperty/alphaTestRef.html
.. _`name`: niAlphaProperty/name.html
.. _`references`: niAlphaProperty/references.html
.. _`runTimeTypeInformation`: niAlphaProperty/runTimeTypeInformation.html
.. _`type`: niAlphaProperty/type.html

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

    niAlphaProperty/clone
    niAlphaProperty/isInstanceOfType
    niAlphaProperty/isOfType
    niAlphaProperty/prependController
    niAlphaProperty/removeAllControllers
    niAlphaProperty/removeController

.. _`clone`: niAlphaProperty/clone.html
.. _`isInstanceOfType`: niAlphaProperty/isInstanceOfType.html
.. _`isOfType`: niAlphaProperty/isOfType.html
.. _`prependController`: niAlphaProperty/prependController.html
.. _`removeAllControllers`: niAlphaProperty/removeAllControllers.html
.. _`removeController`: niAlphaProperty/removeController.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niObject`: ../../lua/type/niObject.html
.. _`niRTTI`: ../../lua/type/niRTTI.html
.. _`string`: ../../lua/type/string.html
