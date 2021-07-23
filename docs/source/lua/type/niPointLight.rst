niPointLight
====================================================================================================

Represents a point light source in a scene. Point lights have a specific location in space and a distance attenuation function. Point lights project light in all directions from their position. They can be moved by changing the translation of the light.

Properties
----------------------------------------------------------------------------------------------------

`affectedNodes`_ (`niNodeLinkedList`_)
    The list of nodes that a given dynamic effect will affect.

`alphaProperty`_ (`niAlphaProperty`_, `nil`_)
    Convenient access to this object's alpha property. Setting this value to be nil will erase the property, while setting it to a valid alpha property will set (or replace) it.

`ambient`_ (`niColor`_)
    The ambient settings for the light.

`appCulled`_ (`boolean`_)
    A flag indicating if this object is culled. When culled, it will not render, and raycasts ignore it.

`constantAttenuation`_ (`number`_)
    The constant attenuation factor.

`diffuse`_ (`niColor`_)
    The defuse settings for the light.

`dimmer`_ (`niColor`_)
    The dimmer settings for the light.

`enabled`_ (`boolean`_)
    The enabled state of a given dynamic effect.

`flags`_ (`number`_)
    Flags, dependent on the specific object type.

`fogProperty`_ (`niFogProperty`_, `nil`_)
    Convenient access to this object's fog property. Setting this value to be nil will erase the property, while setting it to a valid fog property will set (or replace) it.

`linearAttenuation`_ (`number`_)
    The linear attenuation factor.

`materialProperty`_ (`niMaterialProperty`_, `nil`_)
    Convenient access to this object's material property. Setting this value to be nil will erase the property, while setting it to a valid material property will set (or replace) it.

`name`_ (`string`_)
    The human-facing name of the given object.

`parent`_ (`niNode`_)
    The object's parent. It may not have one if it is not attached to the scene.

`properties`_ (`niPropertyLinkedList`_)
    The list of properties attached to this niAVObject.

`quadraticAttenuation`_ (`number`_)
    The quadratic attenuation factor.

`references`_ (`string`_)
    Read-only. The number of references that exist for the given object. When this value hits zero, the object's memory is freed.

`rotation`_ (`tes3matrix33`_)
    The object's local rotation matrix.

`runTimeTypeInformation`_ (`niRTTI`_)
    The runtime type information for this object.

`scale`_ (`number`_)
    The object's local uniform scaling factor.

`specular`_ (`niColor`_)
    The specular settings for the light.

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

    niPointLight/affectedNodes
    niPointLight/alphaProperty
    niPointLight/ambient
    niPointLight/appCulled
    niPointLight/constantAttenuation
    niPointLight/diffuse
    niPointLight/dimmer
    niPointLight/enabled
    niPointLight/flags
    niPointLight/fogProperty
    niPointLight/linearAttenuation
    niPointLight/materialProperty
    niPointLight/name
    niPointLight/parent
    niPointLight/properties
    niPointLight/quadraticAttenuation
    niPointLight/references
    niPointLight/rotation
    niPointLight/runTimeTypeInformation
    niPointLight/scale
    niPointLight/specular
    niPointLight/stencilProperty
    niPointLight/texturingProperty
    niPointLight/translation
    niPointLight/type
    niPointLight/vertexColorProperty
    niPointLight/zBufferProperty

.. _`affectedNodes`: niPointLight/affectedNodes.html
.. _`alphaProperty`: niPointLight/alphaProperty.html
.. _`ambient`: niPointLight/ambient.html
.. _`appCulled`: niPointLight/appCulled.html
.. _`constantAttenuation`: niPointLight/constantAttenuation.html
.. _`diffuse`: niPointLight/diffuse.html
.. _`dimmer`: niPointLight/dimmer.html
.. _`enabled`: niPointLight/enabled.html
.. _`flags`: niPointLight/flags.html
.. _`fogProperty`: niPointLight/fogProperty.html
.. _`linearAttenuation`: niPointLight/linearAttenuation.html
.. _`materialProperty`: niPointLight/materialProperty.html
.. _`name`: niPointLight/name.html
.. _`parent`: niPointLight/parent.html
.. _`properties`: niPointLight/properties.html
.. _`quadraticAttenuation`: niPointLight/quadraticAttenuation.html
.. _`references`: niPointLight/references.html
.. _`rotation`: niPointLight/rotation.html
.. _`runTimeTypeInformation`: niPointLight/runTimeTypeInformation.html
.. _`scale`: niPointLight/scale.html
.. _`specular`: niPointLight/specular.html
.. _`stencilProperty`: niPointLight/stencilProperty.html
.. _`texturingProperty`: niPointLight/texturingProperty.html
.. _`translation`: niPointLight/translation.html
.. _`type`: niPointLight/type.html
.. _`vertexColorProperty`: niPointLight/vertexColorProperty.html
.. _`zBufferProperty`: niPointLight/zBufferProperty.html

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

`setAttenuationForRadius`_
    Sets the attenuation for the radius.

`setRadius`_
    Sets both the light's influence radius, and its lighting attenuation to match the radius.

`update`_
    Updates the world transforms of this node and its children, which makes changes visible for rendering. Use after changing any local rotation, translation, scale, or bounds.

`updateEffects`_
    Update all attached effects.

`updateProperties`_
    Update all attached properties.

.. toctree::
    :hidden:

    niPointLight/attachProperty
    niPointLight/clearTransforms
    niPointLight/clone
    niPointLight/detachProperty
    niPointLight/getObjectByName
    niPointLight/getProperty
    niPointLight/isInstanceOfType
    niPointLight/isOfType
    niPointLight/prependController
    niPointLight/removeAllControllers
    niPointLight/removeController
    niPointLight/setAttenuationForRadius
    niPointLight/setRadius
    niPointLight/update
    niPointLight/updateEffects
    niPointLight/updateProperties

.. _`attachProperty`: niPointLight/attachProperty.html
.. _`clearTransforms`: niPointLight/clearTransforms.html
.. _`clone`: niPointLight/clone.html
.. _`detachProperty`: niPointLight/detachProperty.html
.. _`getObjectByName`: niPointLight/getObjectByName.html
.. _`getProperty`: niPointLight/getProperty.html
.. _`isInstanceOfType`: niPointLight/isInstanceOfType.html
.. _`isOfType`: niPointLight/isOfType.html
.. _`prependController`: niPointLight/prependController.html
.. _`removeAllControllers`: niPointLight/removeAllControllers.html
.. _`removeController`: niPointLight/removeController.html
.. _`setAttenuationForRadius`: niPointLight/setAttenuationForRadius.html
.. _`setRadius`: niPointLight/setRadius.html
.. _`update`: niPointLight/update.html
.. _`updateEffects`: niPointLight/updateEffects.html
.. _`updateProperties`: niPointLight/updateProperties.html

Functions
----------------------------------------------------------------------------------------------------

`attachAffectedNode`_
    Adds a node to the dynamic effect's affected nodes list. The node's ``:updateEffects()`` function should be called afterwards to recognize the change.

`detachAffectedNode`_
    Removes a node from the dynamic effect's affected nodes list. The node's ``:updateEffects()`` function should be called afterwards to recognize the change.

.. toctree::
    :hidden:

    niPointLight/attachAffectedNode
    niPointLight/detachAffectedNode

.. _`attachAffectedNode`: niPointLight/attachAffectedNode.html
.. _`detachAffectedNode`: niPointLight/detachAffectedNode.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niAVObject`: ../../lua/type/niAVObject.html
.. _`niAlphaProperty`: ../../lua/type/niAlphaProperty.html
.. _`niColor`: ../../lua/type/niColor.html
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
