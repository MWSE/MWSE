niProperty
====================================================================================================

A base class representing all rendering properties.

Properties
----------------------------------------------------------------------------------------------------

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

    niProperty/name
    niProperty/references
    niProperty/runTimeTypeInformation
    niProperty/type

.. _`name`: niProperty/name.html
.. _`references`: niProperty/references.html
.. _`runTimeTypeInformation`: niProperty/runTimeTypeInformation.html
.. _`type`: niProperty/type.html

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

    niProperty/clone
    niProperty/isInstanceOfType
    niProperty/isOfType
    niProperty/prependController
    niProperty/removeAllControllers
    niProperty/removeController

.. _`clone`: niProperty/clone.html
.. _`isInstanceOfType`: niProperty/isInstanceOfType.html
.. _`isOfType`: niProperty/isOfType.html
.. _`prependController`: niProperty/prependController.html
.. _`removeAllControllers`: niProperty/removeAllControllers.html
.. _`removeController`: niProperty/removeController.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niObject`: ../../lua/type/niObject.html
.. _`niRTTI`: ../../lua/type/niRTTI.html
.. _`string`: ../../lua/type/string.html
