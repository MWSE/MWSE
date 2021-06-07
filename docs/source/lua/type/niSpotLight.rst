niSpotLight
====================================================================================================

Represents a spotlight in a scene. Spotlights have a specific location and direction, as well as a distance attenuation and angle attenuation functions. The light direction is handled in the same way as in `niDirectionalLight`.

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

`direction`_ (`tes3vector3`_)
    The direction of the spotlight.

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

`spotAngle`_ (`number`_)
    The spotlight angle, in degrees.

`spotExponent`_ (`number`_)
    The spotlight angular attenuation exponent. This value may be ignored.

`translation`_ (`tes3vector3`_)
    The object's local translation vector.

`type`_ (`number`_)
    The enumerated type of a given dynamic effect. Types: `0 - niAmbientLight`, `1 - niDirectionalLight`, `2 - niPointLight`, `3 - niSpotLight`, `4 - niTextureEffect`.

.. toctree::
    :hidden:

    niSpotLight/affectedNodes
    niSpotLight/ambient
    niSpotLight/appCulled
    niSpotLight/constantAttenuation
    niSpotLight/diffuse
    niSpotLight/dimmer
    niSpotLight/direction
    niSpotLight/enabled
    niSpotLight/flags
    niSpotLight/linearAttenuation
    niSpotLight/name
    niSpotLight/parent
    niSpotLight/properties
    niSpotLight/quadraticAttenuation
    niSpotLight/references
    niSpotLight/rotation
    niSpotLight/runTimeTypeInformation
    niSpotLight/scale
    niSpotLight/specular
    niSpotLight/spotAngle
    niSpotLight/spotExponent
    niSpotLight/translation
    niSpotLight/type

.. _`affectedNodes`: niSpotLight/affectedNodes.html
.. _`ambient`: niSpotLight/ambient.html
.. _`appCulled`: niSpotLight/appCulled.html
.. _`constantAttenuation`: niSpotLight/constantAttenuation.html
.. _`diffuse`: niSpotLight/diffuse.html
.. _`dimmer`: niSpotLight/dimmer.html
.. _`direction`: niSpotLight/direction.html
.. _`enabled`: niSpotLight/enabled.html
.. _`flags`: niSpotLight/flags.html
.. _`linearAttenuation`: niSpotLight/linearAttenuation.html
.. _`name`: niSpotLight/name.html
.. _`parent`: niSpotLight/parent.html
.. _`properties`: niSpotLight/properties.html
.. _`quadraticAttenuation`: niSpotLight/quadraticAttenuation.html
.. _`references`: niSpotLight/references.html
.. _`rotation`: niSpotLight/rotation.html
.. _`runTimeTypeInformation`: niSpotLight/runTimeTypeInformation.html
.. _`scale`: niSpotLight/scale.html
.. _`specular`: niSpotLight/specular.html
.. _`spotAngle`: niSpotLight/spotAngle.html
.. _`spotExponent`: niSpotLight/spotExponent.html
.. _`translation`: niSpotLight/translation.html
.. _`type`: niSpotLight/type.html

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

    niSpotLight/attachProperty
    niSpotLight/clearTransforms
    niSpotLight/clone
    niSpotLight/detachProperty
    niSpotLight/getObjectByName
    niSpotLight/getProperty
    niSpotLight/isInstanceOfType
    niSpotLight/isOfType
    niSpotLight/prependController
    niSpotLight/removeAllControllers
    niSpotLight/removeController
    niSpotLight/setAttenuationForRadius
    niSpotLight/setRadius
    niSpotLight/update
    niSpotLight/updateEffects
    niSpotLight/updateProperties

.. _`attachProperty`: niSpotLight/attachProperty.html
.. _`clearTransforms`: niSpotLight/clearTransforms.html
.. _`clone`: niSpotLight/clone.html
.. _`detachProperty`: niSpotLight/detachProperty.html
.. _`getObjectByName`: niSpotLight/getObjectByName.html
.. _`getProperty`: niSpotLight/getProperty.html
.. _`isInstanceOfType`: niSpotLight/isInstanceOfType.html
.. _`isOfType`: niSpotLight/isOfType.html
.. _`prependController`: niSpotLight/prependController.html
.. _`removeAllControllers`: niSpotLight/removeAllControllers.html
.. _`removeController`: niSpotLight/removeController.html
.. _`setAttenuationForRadius`: niSpotLight/setAttenuationForRadius.html
.. _`setRadius`: niSpotLight/setRadius.html
.. _`update`: niSpotLight/update.html
.. _`updateEffects`: niSpotLight/updateEffects.html
.. _`updateProperties`: niSpotLight/updateProperties.html

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
