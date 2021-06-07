niSwitchNode
====================================================================================================

Represents groups of multiple scene graph subtrees, only one of which is visible at any given time. They are useful for showing different states of a model depending on engine / lua logic. If you detach the active subtree, the switch node will set the active subtree to none, or to an index of -1.

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

`switchIndex`_ (`number`_)
    The value of the active child index. This index represents the index of the active child in the array of the NiSwitchNode. If this index is –1, then no child is active.

`translation`_ (`tes3vector3`_)
    The object's local translation vector.

.. toctree::
    :hidden:

    niSwitchNode/appCulled
    niSwitchNode/children
    niSwitchNode/effectList
    niSwitchNode/flags
    niSwitchNode/name
    niSwitchNode/parent
    niSwitchNode/properties
    niSwitchNode/references
    niSwitchNode/rotation
    niSwitchNode/runTimeTypeInformation
    niSwitchNode/scale
    niSwitchNode/switchIndex
    niSwitchNode/translation

.. _`appCulled`: niSwitchNode/appCulled.html
.. _`children`: niSwitchNode/children.html
.. _`effectList`: niSwitchNode/effectList.html
.. _`flags`: niSwitchNode/flags.html
.. _`name`: niSwitchNode/name.html
.. _`parent`: niSwitchNode/parent.html
.. _`properties`: niSwitchNode/properties.html
.. _`references`: niSwitchNode/references.html
.. _`rotation`: niSwitchNode/rotation.html
.. _`runTimeTypeInformation`: niSwitchNode/runTimeTypeInformation.html
.. _`scale`: niSwitchNode/scale.html
.. _`switchIndex`: niSwitchNode/switchIndex.html
.. _`translation`: niSwitchNode/translation.html

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

    niSwitchNode/attachChild
    niSwitchNode/attachProperty
    niSwitchNode/clearTransforms
    niSwitchNode/clone
    niSwitchNode/detachChild
    niSwitchNode/detachChildAt
    niSwitchNode/detachProperty
    niSwitchNode/getEffect
    niSwitchNode/getObjectByName
    niSwitchNode/getProperty
    niSwitchNode/isInstanceOfType
    niSwitchNode/isOfType
    niSwitchNode/prependController
    niSwitchNode/removeAllControllers
    niSwitchNode/removeController
    niSwitchNode/update
    niSwitchNode/updateEffects
    niSwitchNode/updateProperties

.. _`attachChild`: niSwitchNode/attachChild.html
.. _`attachProperty`: niSwitchNode/attachProperty.html
.. _`clearTransforms`: niSwitchNode/clearTransforms.html
.. _`clone`: niSwitchNode/clone.html
.. _`detachChild`: niSwitchNode/detachChild.html
.. _`detachChildAt`: niSwitchNode/detachChildAt.html
.. _`detachProperty`: niSwitchNode/detachProperty.html
.. _`getEffect`: niSwitchNode/getEffect.html
.. _`getObjectByName`: niSwitchNode/getObjectByName.html
.. _`getProperty`: niSwitchNode/getProperty.html
.. _`isInstanceOfType`: niSwitchNode/isInstanceOfType.html
.. _`isOfType`: niSwitchNode/isOfType.html
.. _`prependController`: niSwitchNode/prependController.html
.. _`removeAllControllers`: niSwitchNode/removeAllControllers.html
.. _`removeController`: niSwitchNode/removeController.html
.. _`update`: niSwitchNode/update.html
.. _`updateEffects`: niSwitchNode/updateEffects.html
.. _`updateProperties`: niSwitchNode/updateProperties.html

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
