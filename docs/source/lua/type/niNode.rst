niNode
====================================================================================================

Base class that represents the nodes of a scene graph. A node can have any number of child nodes.

Properties
----------------------------------------------------------------------------------------------------

`appCulled`_ (`boolean`_)
    A flag indicating if this object is culled. When culled, it will not render, and raycasts ignore it.

`children`_ (`niAVObjectTArray`_)
    The children of the node.

`effectList`_ (`niDynamicEffectLinkedList`_)
    The effect list of the node.

`flags`_ (`number`_)
    Flags, dependent on the specific object type.

`name`_ (`string`_)
    The human-facing name of the given object.

`parent`_ (`niNode`_)
    The object's parent. It may not have one if it is not attached to the scene.

`properties`_ (`niPropertyLinkedList`_)
    The list of properties attached to this niAVObject.

`references`_ (`string`_)
    The number of references that exist for the given object. When this value hits zero, the object's memory is freed.

`rotation`_ (`tes3matrix33`_)
    The object's local rotation matrix.

`runTimeTypeInformation`_ (`niRTTI`_)
    The runtime type information for this object.

`scale`_ (`number`_)
    The object's local uniform scaling factor.

`translation`_ (`tes3vector3`_)
    The object's local translation vector.

.. toctree::
    :hidden:

    niNode/appCulled
    niNode/children
    niNode/effectList
    niNode/flags
    niNode/name
    niNode/parent
    niNode/properties
    niNode/references
    niNode/rotation
    niNode/runTimeTypeInformation
    niNode/scale
    niNode/translation

.. _`appCulled`: niNode/appCulled.html
.. _`children`: niNode/children.html
.. _`effectList`: niNode/effectList.html
.. _`flags`: niNode/flags.html
.. _`name`: niNode/name.html
.. _`parent`: niNode/parent.html
.. _`properties`: niNode/properties.html
.. _`references`: niNode/references.html
.. _`rotation`: niNode/rotation.html
.. _`runTimeTypeInformation`: niNode/runTimeTypeInformation.html
.. _`scale`: niNode/scale.html
.. _`translation`: niNode/translation.html

Methods
----------------------------------------------------------------------------------------------------

`attachChild`_
    Attachs the child to the children list of the node.

`attachProperty`_
    Attach a property to this object.

`clearTransforms`_
    Resets the object's local transform.

`clone`_ (`niObject`_)
    Creates a copy of this object.

`detachChild`_ (`niAVObject`_)
    Detaches the child from the children list of the node. Returns the detached child.

`detachChildAt`_ (`niAVObject`_)
    Detaches the child at the specified index from the children list of the node. Returns the detached child.

`detachProperty`_ (`niProperty`_)
    Detaches and returns a property from the object which matches the given property type.

`getEffect`_ (`niDynamicEffect`_)
    Gets the effect for the given type.

`getObjectByName`_ (`niAVObject`_)
    Searches this node and all child nodes recursively for a node with a name that matches the argument.

`getProperty`_ (`niProperty`_)
    Gets an attached property by property type.

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

`update`_
    Updates the world transforms of this node and its children, which makes changes visible for rendering. Use after changing any local rotation, translation, scale, or bounds.

`updateEffects`_
    Update all attached effects.

`updateProperties`_
    Update all attached properties.

.. toctree::
    :hidden:

    niNode/attachChild
    niNode/attachProperty
    niNode/clearTransforms
    niNode/clone
    niNode/detachChild
    niNode/detachChildAt
    niNode/detachProperty
    niNode/getEffect
    niNode/getObjectByName
    niNode/getProperty
    niNode/isInstanceOfType
    niNode/isOfType
    niNode/prependController
    niNode/removeAllControllers
    niNode/removeController
    niNode/update
    niNode/updateEffects
    niNode/updateProperties

.. _`attachChild`: niNode/attachChild.html
.. _`attachProperty`: niNode/attachProperty.html
.. _`clearTransforms`: niNode/clearTransforms.html
.. _`clone`: niNode/clone.html
.. _`detachChild`: niNode/detachChild.html
.. _`detachChildAt`: niNode/detachChildAt.html
.. _`detachProperty`: niNode/detachProperty.html
.. _`getEffect`: niNode/getEffect.html
.. _`getObjectByName`: niNode/getObjectByName.html
.. _`getProperty`: niNode/getProperty.html
.. _`isInstanceOfType`: niNode/isInstanceOfType.html
.. _`isOfType`: niNode/isOfType.html
.. _`prependController`: niNode/prependController.html
.. _`removeAllControllers`: niNode/removeAllControllers.html
.. _`removeController`: niNode/removeController.html
.. _`update`: niNode/update.html
.. _`updateEffects`: niNode/updateEffects.html
.. _`updateProperties`: niNode/updateProperties.html

.. _`niObject`: ../../lua/type/niObject.html
.. _`boolean`: ../../lua/type/boolean.html
.. _`string`: ../../lua/type/string.html
.. _`number`: ../../lua/type/number.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`tes3matrix33`: ../../lua/type/tes3matrix33.html
.. _`niAVObject`: ../../lua/type/niAVObject.html
.. _`niRTTI`: ../../lua/type/niRTTI.html
.. _`niPropertyLinkedList`: ../../lua/type/niPropertyLinkedList.html
.. _`niProperty`: ../../lua/type/niProperty.html
.. _`tes3vector3`: ../../lua/type/tes3vector3.html
.. _`niDynamicEffect`: ../../lua/type/niDynamicEffect.html
.. _`niDynamicEffectLinkedList`: ../../lua/type/niDynamicEffectLinkedList.html
