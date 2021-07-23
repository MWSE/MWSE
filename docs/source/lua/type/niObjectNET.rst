niObjectNET
====================================================================================================

An object that has a name, extra data, and controllers.

Properties
----------------------------------------------------------------------------------------------------

`name`_ (`string`_)
    The human-facing name of the given object.

`references`_ (`string`_)
    Read-only. The number of references that exist for the given object. When this value hits zero, the object's memory is freed.

`runTimeTypeInformation`_ (`niRTTI`_)
    The runtime type information for this object.

.. toctree::
    :hidden:

    niObjectNET/name
    niObjectNET/references
    niObjectNET/runTimeTypeInformation

.. _`name`: niObjectNET/name.html
.. _`references`: niObjectNET/references.html
.. _`runTimeTypeInformation`: niObjectNET/runTimeTypeInformation.html

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

    niObjectNET/clone
    niObjectNET/isInstanceOfType
    niObjectNET/isOfType
    niObjectNET/prependController
    niObjectNET/removeAllControllers
    niObjectNET/removeController

.. _`clone`: niObjectNET/clone.html
.. _`isInstanceOfType`: niObjectNET/isInstanceOfType.html
.. _`isOfType`: niObjectNET/isOfType.html
.. _`prependController`: niObjectNET/prependController.html
.. _`removeAllControllers`: niObjectNET/removeAllControllers.html
.. _`removeController`: niObjectNET/removeController.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niObject`: ../../lua/type/niObject.html
.. _`niRTTI`: ../../lua/type/niRTTI.html
.. _`string`: ../../lua/type/string.html
