niStencilProperty
====================================================================================================

A rendering property that controls the use of a stencil buffer when rendering. It also includes a draw-mode setting to allows the game engine to control the culling mode of a set of geometry.

Properties
----------------------------------------------------------------------------------------------------

`drawMode`_ (`number`_)
    The drawing mode used to draw the object.

`enabled`_ (`boolean`_)
    The value of the stencil buffer enable flag.

`failAction`_ (`number`_)
    The action that is taken in the stencil buffer when the stencil test fails.

`mask`_ (`number`_)
    The mask value of the stencil buffer.

`name`_ (`string`_)
    The human-facing name of the given object.

`passAction`_ (`number`_)
    The action that is taken in the stencil buffer when the stencil test passes and the pixel passes the Z-buffer test.

`reference`_ (`number`_)
    The stencil reference.

`references`_ (`string`_)
    Read-only. The number of references that exist for the given object. When this value hits zero, the object's memory is freed.

`runTimeTypeInformation`_ (`niRTTI`_)
    The runtime type information for this object.

`testFunc`_ (`number`_)
    The stencil buffer test function used to test the reference value against the buffer value.

`type`_ (`niPropertyType`_)
    The unique class identifier number of the given rendering property.

`zFailAction`_ (`number`_)
    The action that is taken in the stencil buffer when the stencil test passes but the pixel fails the Z-buffer test.

.. toctree::
    :hidden:

    niStencilProperty/drawMode
    niStencilProperty/enabled
    niStencilProperty/failAction
    niStencilProperty/mask
    niStencilProperty/name
    niStencilProperty/passAction
    niStencilProperty/reference
    niStencilProperty/references
    niStencilProperty/runTimeTypeInformation
    niStencilProperty/testFunc
    niStencilProperty/type
    niStencilProperty/zFailAction

.. _`drawMode`: niStencilProperty/drawMode.html
.. _`enabled`: niStencilProperty/enabled.html
.. _`failAction`: niStencilProperty/failAction.html
.. _`mask`: niStencilProperty/mask.html
.. _`name`: niStencilProperty/name.html
.. _`passAction`: niStencilProperty/passAction.html
.. _`reference`: niStencilProperty/reference.html
.. _`references`: niStencilProperty/references.html
.. _`runTimeTypeInformation`: niStencilProperty/runTimeTypeInformation.html
.. _`testFunc`: niStencilProperty/testFunc.html
.. _`type`: niStencilProperty/type.html
.. _`zFailAction`: niStencilProperty/zFailAction.html

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

    niStencilProperty/clone
    niStencilProperty/isInstanceOfType
    niStencilProperty/isOfType
    niStencilProperty/prependController
    niStencilProperty/removeAllControllers
    niStencilProperty/removeController

.. _`clone`: niStencilProperty/clone.html
.. _`isInstanceOfType`: niStencilProperty/isInstanceOfType.html
.. _`isOfType`: niStencilProperty/isOfType.html
.. _`prependController`: niStencilProperty/prependController.html
.. _`removeAllControllers`: niStencilProperty/removeAllControllers.html
.. _`removeController`: niStencilProperty/removeController.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niObject`: ../../lua/type/niObject.html
.. _`niRTTI`: ../../lua/type/niRTTI.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
