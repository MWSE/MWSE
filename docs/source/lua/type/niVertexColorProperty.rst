niVertexColorProperty
====================================================================================================

A rendering property that allows the application to control the method used to compute colors for each vertex in a geometry object.  This class enables effects such as static pre-lighting, dynamic lighting, etc.

Properties
----------------------------------------------------------------------------------------------------

`lighting`_ (`number`_)
    The lighting mode.

`name`_ (`string`_)
    The human-facing name of the given object.

`references`_ (`string`_)
    Read-only. The number of references that exist for the given object. When this value hits zero, the object's memory is freed.

`runTimeTypeInformation`_ (`niRTTI`_)
    The runtime type information for this object.

`source`_ (`number`_)
    The source vertex mode.

`type`_ (`niPropertyType`_)
    The unique class identifier number of the given rendering property.

.. toctree::
    :hidden:

    niVertexColorProperty/lighting
    niVertexColorProperty/name
    niVertexColorProperty/references
    niVertexColorProperty/runTimeTypeInformation
    niVertexColorProperty/source
    niVertexColorProperty/type

.. _`lighting`: niVertexColorProperty/lighting.html
.. _`name`: niVertexColorProperty/name.html
.. _`references`: niVertexColorProperty/references.html
.. _`runTimeTypeInformation`: niVertexColorProperty/runTimeTypeInformation.html
.. _`source`: niVertexColorProperty/source.html
.. _`type`: niVertexColorProperty/type.html

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

    niVertexColorProperty/clone
    niVertexColorProperty/isInstanceOfType
    niVertexColorProperty/isOfType
    niVertexColorProperty/prependController
    niVertexColorProperty/removeAllControllers
    niVertexColorProperty/removeController

.. _`clone`: niVertexColorProperty/clone.html
.. _`isInstanceOfType`: niVertexColorProperty/isInstanceOfType.html
.. _`isOfType`: niVertexColorProperty/isOfType.html
.. _`prependController`: niVertexColorProperty/prependController.html
.. _`removeAllControllers`: niVertexColorProperty/removeAllControllers.html
.. _`removeController`: niVertexColorProperty/removeController.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niObject`: ../../lua/type/niObject.html
.. _`niRTTI`: ../../lua/type/niRTTI.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
