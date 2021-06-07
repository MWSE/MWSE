niPointLight
====================================================================================================

Represents a point light source in a scene. Point lights have a specific location in space and a distance attenuation function. Point lights project light in all directions from their position. They can be moved by changing the translation of the light.

Properties
----------------------------------------------------------------------------------------------------

`affectedNodes`_ (`niNodeLinkedList`_)
    The list of nodes that a given dynamic effect will affect.

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

`linearAttenuation`_ (`number`_)
    The linear attenuation factor.

`name`_ (`string`_)
    The human-facing name of the given object.

`parent`_ (`niNode`_)
    The object's parent. It may not have one if it is not attached to the scene.

`properties`_ (`niPropertyLinkedList`_)
    The list of properties attached to this niAVObject.

`quadraticAttenuation`_ (`number`_)
    The quadratic attenuation factor.

`references`_ (`string`_)
    The number of references that exist for the given object. When this value hits zero, the object's memory is freed.

`rotation`_ (`tes3matrix33`_)
    The object's local rotation matrix.

`runTimeTypeInformation`_ (`niRTTI`_)
    The runtime type information for this object.

`scale`_ (`number`_)
    The object's local uniform scaling factor.

`specular`_ (`niColor`_)
    The specular settings for the light.

`translation`_ (`tes3vector3`_)
    The object's local translation vector.

`type`_ (`number`_)
    The enumerated type of a given dynamic effect. Types: `0 - niAmbientLight`, `1 - niDirectionalLight`, `2 - niPointLight`, `3 - niSpotLight`, `4 - niTextureEffect`.

.. toctree::
    :hidden:

    niPointLight/affectedNodes
    niPointLight/ambient
    niPointLight/appCulled
    niPointLight/constantAttenuation
    niPointLight/diffuse
    niPointLight/dimmer
    niPointLight/enabled
    niPointLight/flags
    niPointLight/linearAttenuation
    niPointLight/name
    niPointLight/parent
    niPointLight/properties
    niPointLight/quadraticAttenuation
    niPointLight/references
    niPointLight/rotation
    niPointLight/runTimeTypeInformation
    niPointLight/scale
    niPointLight/specular
    niPointLight/translation
    niPointLight/type

.. _`affectedNodes`: niPointLight/affectedNodes.html
.. _`ambient`: niPointLight/ambient.html
.. _`appCulled`: niPointLight/appCulled.html
.. _`constantAttenuation`: niPointLight/constantAttenuation.html
.. _`diffuse`: niPointLight/diffuse.html
.. _`dimmer`: niPointLight/dimmer.html
.. _`enabled`: niPointLight/enabled.html
.. _`flags`: niPointLight/flags.html
.. _`linearAttenuation`: niPointLight/linearAttenuation.html
.. _`name`: niPointLight/name.html
.. _`parent`: niPointLight/parent.html
.. _`properties`: niPointLight/properties.html
.. _`quadraticAttenuation`: niPointLight/quadraticAttenuation.html
.. _`references`: niPointLight/references.html
.. _`rotation`: niPointLight/rotation.html
.. _`runTimeTypeInformation`: niPointLight/runTimeTypeInformation.html
.. _`scale`: niPointLight/scale.html
.. _`specular`: niPointLight/specular.html
.. _`translation`: niPointLight/translation.html
.. _`type`: niPointLight/type.html

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
.. _`niColor`: ../../lua/type/niColor.html
