niCollisionSwitch
====================================================================================================

An object that specifies if the collision system is enabled.

Properties
----------------------------------------------------------------------------------------------------

`alphaProperty`_ (`niAlphaProperty`_, `nil`_)
    Convenient access to this object's alpha property. Setting this value to be nil will erase the property, while setting it to a valid alpha property will set (or replace) it.

`appCulled`_ (`boolean`_)
    A flag indicating if this object is culled. When culled, it will not render, and raycasts ignore it.

`children`_ (`niAVObjectTArray`_)
    Read-only. The children of the node.

`collisionActive`_ (`boolean`_)
    The enabled state of collision on the collision switch.

`effectList`_ (`niDynamicEffectLinkedList`_)
    Read-only. The effect list of the node.

`flags`_ (`number`_)
    Flags, dependent on the specific object type.

`fogProperty`_ (`niFogProperty`_, `nil`_)
    Convenient access to this object's fog property. Setting this value to be nil will erase the property, while setting it to a valid fog property will set (or replace) it.

`materialProperty`_ (`niMaterialProperty`_, `nil`_)
    Convenient access to this object's material property. Setting this value to be nil will erase the property, while setting it to a valid material property will set (or replace) it.

`name`_ (`string`_)
    The human-facing name of the given object.

`parent`_ (`niNode`_)
    The object's parent. It may not have one if it is not attached to the scene.

`properties`_ (`niPropertyLinkedList`_)
    The list of properties attached to this niAVObject.

`references`_ (`string`_)
    Read-only. The number of references that exist for the given object. When this value hits zero, the object's memory is freed.

`rotation`_ (`tes3matrix33`_)
    The object's local rotation matrix.

`runTimeTypeInformation`_ (`niRTTI`_)
    The runtime type information for this object.

`scale`_ (`number`_)
    The object's local uniform scaling factor.

`stencilProperty`_ (`niStencilProperty`_, `nil`_)
    Convenient access to this object's stencil property. Setting this value to be nil will erase the property, while setting it to a valid stencil property will set (or replace) it.

`texturingProperty`_ (`niTexturingProperty`_, `nil`_)
    Convenient access to this object's texturing property. Setting this value to be nil will erase the property, while setting it to a valid texturing property will set (or replace) it.

`translation`_ (`tes3vector3`_)
    The object's local translation vector.

`vertexColorProperty`_ (`niVertexColorProperty`_, `nil`_)
    Convenient access to this object's vertex coloring property. Setting this value to be nil will erase the property, while setting it to a valid vertex coloring property will set (or replace) it.

`zBufferProperty`_ (`niZBufferProperty`_, `nil`_)
    Convenient access to this object's z-buffer property. Setting this value to be nil will erase the property, while setting it to a valid z-buffer property will set (or replace) it.

.. toctree::
    :hidden:

    niCollisionSwitch/alphaProperty
    niCollisionSwitch/appCulled
    niCollisionSwitch/children
    niCollisionSwitch/collisionActive
    niCollisionSwitch/effectList
    niCollisionSwitch/flags
    niCollisionSwitch/fogProperty
    niCollisionSwitch/materialProperty
    niCollisionSwitch/name
    niCollisionSwitch/parent
    niCollisionSwitch/properties
    niCollisionSwitch/references
    niCollisionSwitch/rotation
    niCollisionSwitch/runTimeTypeInformation
    niCollisionSwitch/scale
    niCollisionSwitch/stencilProperty
    niCollisionSwitch/texturingProperty
    niCollisionSwitch/translation
    niCollisionSwitch/vertexColorProperty
    niCollisionSwitch/zBufferProperty

.. _`alphaProperty`: niCollisionSwitch/alphaProperty.html
.. _`appCulled`: niCollisionSwitch/appCulled.html
.. _`children`: niCollisionSwitch/children.html
.. _`collisionActive`: niCollisionSwitch/collisionActive.html
.. _`effectList`: niCollisionSwitch/effectList.html
.. _`flags`: niCollisionSwitch/flags.html
.. _`fogProperty`: niCollisionSwitch/fogProperty.html
.. _`materialProperty`: niCollisionSwitch/materialProperty.html
.. _`name`: niCollisionSwitch/name.html
.. _`parent`: niCollisionSwitch/parent.html
.. _`properties`: niCollisionSwitch/properties.html
.. _`references`: niCollisionSwitch/references.html
.. _`rotation`: niCollisionSwitch/rotation.html
.. _`runTimeTypeInformation`: niCollisionSwitch/runTimeTypeInformation.html
.. _`scale`: niCollisionSwitch/scale.html
.. _`stencilProperty`: niCollisionSwitch/stencilProperty.html
.. _`texturingProperty`: niCollisionSwitch/texturingProperty.html
.. _`translation`: niCollisionSwitch/translation.html
.. _`vertexColorProperty`: niCollisionSwitch/vertexColorProperty.html
.. _`zBufferProperty`: niCollisionSwitch/zBufferProperty.html

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

    niCollisionSwitch/attachChild
    niCollisionSwitch/attachProperty
    niCollisionSwitch/clearTransforms
    niCollisionSwitch/clone
    niCollisionSwitch/detachChild
    niCollisionSwitch/detachChildAt
    niCollisionSwitch/detachProperty
    niCollisionSwitch/getEffect
    niCollisionSwitch/getObjectByName
    niCollisionSwitch/getProperty
    niCollisionSwitch/isInstanceOfType
    niCollisionSwitch/isOfType
    niCollisionSwitch/prependController
    niCollisionSwitch/removeAllControllers
    niCollisionSwitch/removeController
    niCollisionSwitch/update
    niCollisionSwitch/updateEffects
    niCollisionSwitch/updateProperties

.. _`attachChild`: niCollisionSwitch/attachChild.html
.. _`attachProperty`: niCollisionSwitch/attachProperty.html
.. _`clearTransforms`: niCollisionSwitch/clearTransforms.html
.. _`clone`: niCollisionSwitch/clone.html
.. _`detachChild`: niCollisionSwitch/detachChild.html
.. _`detachChildAt`: niCollisionSwitch/detachChildAt.html
.. _`detachProperty`: niCollisionSwitch/detachProperty.html
.. _`getEffect`: niCollisionSwitch/getEffect.html
.. _`getObjectByName`: niCollisionSwitch/getObjectByName.html
.. _`getProperty`: niCollisionSwitch/getProperty.html
.. _`isInstanceOfType`: niCollisionSwitch/isInstanceOfType.html
.. _`isOfType`: niCollisionSwitch/isOfType.html
.. _`prependController`: niCollisionSwitch/prependController.html
.. _`removeAllControllers`: niCollisionSwitch/removeAllControllers.html
.. _`removeController`: niCollisionSwitch/removeController.html
.. _`update`: niCollisionSwitch/update.html
.. _`updateEffects`: niCollisionSwitch/updateEffects.html
.. _`updateProperties`: niCollisionSwitch/updateProperties.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niAVObject`: ../../lua/type/niAVObject.html
.. _`niAlphaProperty`: ../../lua/type/niAlphaProperty.html
.. _`niDynamicEffect`: ../../lua/type/niDynamicEffect.html
.. _`niDynamicEffectLinkedList`: ../../lua/type/niDynamicEffectLinkedList.html
.. _`niFogProperty`: ../../lua/type/niFogProperty.html
.. _`niMaterialProperty`: ../../lua/type/niMaterialProperty.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`niObject`: ../../lua/type/niObject.html
.. _`niProperty`: ../../lua/type/niProperty.html
.. _`niPropertyLinkedList`: ../../lua/type/niPropertyLinkedList.html
.. _`niRTTI`: ../../lua/type/niRTTI.html
.. _`niStencilProperty`: ../../lua/type/niStencilProperty.html
.. _`niTexturingProperty`: ../../lua/type/niTexturingProperty.html
.. _`niVertexColorProperty`: ../../lua/type/niVertexColorProperty.html
.. _`nil`: ../../lua/type/nil.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`tes3matrix33`: ../../lua/type/tes3matrix33.html
.. _`tes3vector3`: ../../lua/type/tes3vector3.html
