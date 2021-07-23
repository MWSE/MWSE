niDynamicEffect
====================================================================================================

Base class for dynamic effects such as NiLights or projected textures effects.

Properties
----------------------------------------------------------------------------------------------------

`affectedNodes`_ (`niNodeLinkedList`_)
    The list of nodes that a given dynamic effect will affect.

`alphaProperty`_ (`niAlphaProperty`_, `nil`_)
    Convenient access to this object's alpha property. Setting this value to be nil will erase the property, while setting it to a valid alpha property will set (or replace) it.

`appCulled`_ (`boolean`_)
    A flag indicating if this object is culled. When culled, it will not render, and raycasts ignore it.

`enabled`_ (`boolean`_)
    The enabled state of a given dynamic effect.

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

`type`_ (`number`_)
    The enumerated type of a given dynamic effect. Types: `0 - niAmbientLight`, `1 - niDirectionalLight`, `2 - niPointLight`, `3 - niSpotLight`, `4 - niTextureEffect`.

`vertexColorProperty`_ (`niVertexColorProperty`_, `nil`_)
    Convenient access to this object's vertex coloring property. Setting this value to be nil will erase the property, while setting it to a valid vertex coloring property will set (or replace) it.

`zBufferProperty`_ (`niZBufferProperty`_, `nil`_)
    Convenient access to this object's z-buffer property. Setting this value to be nil will erase the property, while setting it to a valid z-buffer property will set (or replace) it.

.. toctree::
    :hidden:

    niDynamicEffect/affectedNodes
    niDynamicEffect/alphaProperty
    niDynamicEffect/appCulled
    niDynamicEffect/enabled
    niDynamicEffect/flags
    niDynamicEffect/fogProperty
    niDynamicEffect/materialProperty
    niDynamicEffect/name
    niDynamicEffect/parent
    niDynamicEffect/properties
    niDynamicEffect/references
    niDynamicEffect/rotation
    niDynamicEffect/runTimeTypeInformation
    niDynamicEffect/scale
    niDynamicEffect/stencilProperty
    niDynamicEffect/texturingProperty
    niDynamicEffect/translation
    niDynamicEffect/type
    niDynamicEffect/vertexColorProperty
    niDynamicEffect/zBufferProperty

.. _`affectedNodes`: niDynamicEffect/affectedNodes.html
.. _`alphaProperty`: niDynamicEffect/alphaProperty.html
.. _`appCulled`: niDynamicEffect/appCulled.html
.. _`enabled`: niDynamicEffect/enabled.html
.. _`flags`: niDynamicEffect/flags.html
.. _`fogProperty`: niDynamicEffect/fogProperty.html
.. _`materialProperty`: niDynamicEffect/materialProperty.html
.. _`name`: niDynamicEffect/name.html
.. _`parent`: niDynamicEffect/parent.html
.. _`properties`: niDynamicEffect/properties.html
.. _`references`: niDynamicEffect/references.html
.. _`rotation`: niDynamicEffect/rotation.html
.. _`runTimeTypeInformation`: niDynamicEffect/runTimeTypeInformation.html
.. _`scale`: niDynamicEffect/scale.html
.. _`stencilProperty`: niDynamicEffect/stencilProperty.html
.. _`texturingProperty`: niDynamicEffect/texturingProperty.html
.. _`translation`: niDynamicEffect/translation.html
.. _`type`: niDynamicEffect/type.html
.. _`vertexColorProperty`: niDynamicEffect/vertexColorProperty.html
.. _`zBufferProperty`: niDynamicEffect/zBufferProperty.html

Methods
----------------------------------------------------------------------------------------------------

`attachProperty`_
    Attach a property to this object.

`clearTransforms`_
    Resets the object's local transform.

`clone`_ (`niObject`_)
    Creates a copy of this object.

`detachProperty`_ (`niProperty`_)
    Detaches and returns a property from the object which matches the given property type.

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

    niDynamicEffect/attachProperty
    niDynamicEffect/clearTransforms
    niDynamicEffect/clone
    niDynamicEffect/detachProperty
    niDynamicEffect/getObjectByName
    niDynamicEffect/getProperty
    niDynamicEffect/isInstanceOfType
    niDynamicEffect/isOfType
    niDynamicEffect/prependController
    niDynamicEffect/removeAllControllers
    niDynamicEffect/removeController
    niDynamicEffect/update
    niDynamicEffect/updateEffects
    niDynamicEffect/updateProperties

.. _`attachProperty`: niDynamicEffect/attachProperty.html
.. _`clearTransforms`: niDynamicEffect/clearTransforms.html
.. _`clone`: niDynamicEffect/clone.html
.. _`detachProperty`: niDynamicEffect/detachProperty.html
.. _`getObjectByName`: niDynamicEffect/getObjectByName.html
.. _`getProperty`: niDynamicEffect/getProperty.html
.. _`isInstanceOfType`: niDynamicEffect/isInstanceOfType.html
.. _`isOfType`: niDynamicEffect/isOfType.html
.. _`prependController`: niDynamicEffect/prependController.html
.. _`removeAllControllers`: niDynamicEffect/removeAllControllers.html
.. _`removeController`: niDynamicEffect/removeController.html
.. _`update`: niDynamicEffect/update.html
.. _`updateEffects`: niDynamicEffect/updateEffects.html
.. _`updateProperties`: niDynamicEffect/updateProperties.html

Functions
----------------------------------------------------------------------------------------------------

`attachAffectedNode`_
    Adds a node to the dynamic effect's affected nodes list. The node's ``:updateEffects()`` function should be called afterwards to recognize the change.

`detachAffectedNode`_
    Removes a node from the dynamic effect's affected nodes list. The node's ``:updateEffects()`` function should be called afterwards to recognize the change.

.. toctree::
    :hidden:

    niDynamicEffect/attachAffectedNode
    niDynamicEffect/detachAffectedNode

.. _`attachAffectedNode`: niDynamicEffect/attachAffectedNode.html
.. _`detachAffectedNode`: niDynamicEffect/detachAffectedNode.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niAVObject`: ../../lua/type/niAVObject.html
.. _`niAlphaProperty`: ../../lua/type/niAlphaProperty.html
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
