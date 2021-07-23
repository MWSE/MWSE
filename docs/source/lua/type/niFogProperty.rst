niFogProperty
====================================================================================================

A rendering property that controls the appearance of fogging.

Properties
----------------------------------------------------------------------------------------------------

`color`_ (`niColor`_)
    Read-only. The fog color.

`density`_ (`number`_)
    The fog density.

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

    niFogProperty/color
    niFogProperty/density
    niFogProperty/name
    niFogProperty/references
    niFogProperty/runTimeTypeInformation
    niFogProperty/type

.. _`color`: niFogProperty/color.html
.. _`density`: niFogProperty/density.html
.. _`name`: niFogProperty/name.html
.. _`references`: niFogProperty/references.html
.. _`runTimeTypeInformation`: niFogProperty/runTimeTypeInformation.html
.. _`type`: niFogProperty/type.html

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

    niFogProperty/clone
    niFogProperty/isInstanceOfType
    niFogProperty/isOfType
    niFogProperty/prependController
    niFogProperty/removeAllControllers
    niFogProperty/removeController

.. _`clone`: niFogProperty/clone.html
.. _`isInstanceOfType`: niFogProperty/isInstanceOfType.html
.. _`isOfType`: niFogProperty/isOfType.html
.. _`prependController`: niFogProperty/prependController.html
.. _`removeAllControllers`: niFogProperty/removeAllControllers.html
.. _`removeController`: niFogProperty/removeController.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niColor`: ../../lua/type/niColor.html
.. _`niObject`: ../../lua/type/niObject.html
.. _`niRTTI`: ../../lua/type/niRTTI.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
