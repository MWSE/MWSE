niObject
====================================================================================================

The base-most object from which almost all NetImmerse structures are derived from.

Properties
----------------------------------------------------------------------------------------------------

`references`_ (`string`_)
    Read-only. The number of references that exist for the given object. When this value hits zero, the object's memory is freed.

`runTimeTypeInformation`_ (`niRTTI`_)
    The runtime type information for this object.

.. toctree::
    :hidden:

    niObject/references
    niObject/runTimeTypeInformation

.. _`references`: niObject/references.html
.. _`runTimeTypeInformation`: niObject/runTimeTypeInformation.html

Methods
----------------------------------------------------------------------------------------------------

`clone`_ (`niObject`_)
    Creates a copy of this object.

`isInstanceOfType`_ (`boolean`_)
    Determines if the object is of a given type, or of a type derived from the given type. Types can be found in the tes3.niType table.

`isOfType`_ (`boolean`_)
    Determines if the object is of a given type. Types can be found in the tes3.niType table.

.. toctree::
    :hidden:

    niObject/clone
    niObject/isInstanceOfType
    niObject/isOfType

.. _`clone`: niObject/clone.html
.. _`isInstanceOfType`: niObject/isInstanceOfType.html
.. _`isOfType`: niObject/isOfType.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niObject`: ../../lua/type/niObject.html
.. _`niRTTI`: ../../lua/type/niRTTI.html
.. _`string`: ../../lua/type/string.html
