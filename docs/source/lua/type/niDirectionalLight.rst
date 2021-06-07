niDirectionalLight
====================================================================================================

An object that represents a light with light traveling in a given direction. The light may be pointed in any direction by rotating the light.

Properties
----------------------------------------------------------------------------------------------------

`affectedNodes`_ (`niNodeLinkedList`_)
    The list of nodes that a given dynamic effect will affect.

`ambient`_ (`niColor`_)
    The ambient settings for the light.

`appCulled`_ (`boolean`_)
    A flag indicating if this object is culled. When culled, it will not render, and raycasts ignore it.

`diffuse`_ (`niColor`_)
    The defuse settings for the light.

`dimmer`_ (`niColor`_)
    The dimmer settings for the light.

`direction`_ (`tes3vector3`_)
    The direction of the directional light.

`enabled`_ (`boolean`_)
    The enabled state of a given dynamic effect.

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

`specular`_ (`niColor`_)
    The specular settings for the light.

`translation`_ (`tes3vector3`_)
    The object's local translation vector.

`type`_ (`number`_)
    The enumerated type of a given dynamic effect. Types: `0 - niAmbientLight`, `1 - niDirectionalLight`, `2 - niPointLight`, `3 - niSpotLight`, `4 - niTextureEffect`.

.. toctree::
    :hidden:

    niDirectionalLight/affectedNodes
    niDirectionalLight/ambient
    niDirectionalLight/appCulled
    niDirectionalLight/diffuse
    niDirectionalLight/dimmer
    niDirectionalLight/direction
    niDirectionalLight/enabled
    niDirectionalLight/flags
    niDirectionalLight/name
    niDirectionalLight/parent
    niDirectionalLight/properties
    niDirectionalLight/references
    niDirectionalLight/rotation
    niDirectionalLight/runTimeTypeInformation
    niDirectionalLight/scale
    niDirectionalLight/specular
    niDirectionalLight/translation
    niDirectionalLight/type

.. _`affectedNodes`: niDirectionalLight/affectedNodes.html
.. _`ambient`: niDirectionalLight/ambient.html
.. _`appCulled`: niDirectionalLight/appCulled.html
.. _`diffuse`: niDirectionalLight/diffuse.html
.. _`dimmer`: niDirectionalLight/dimmer.html
.. _`direction`: niDirectionalLight/direction.html
.. _`enabled`: niDirectionalLight/enabled.html
.. _`flags`: niDirectionalLight/flags.html
.. _`name`: niDirectionalLight/name.html
.. _`parent`: niDirectionalLight/parent.html
.. _`properties`: niDirectionalLight/properties.html
.. _`references`: niDirectionalLight/references.html
.. _`rotation`: niDirectionalLight/rotation.html
.. _`runTimeTypeInformation`: niDirectionalLight/runTimeTypeInformation.html
.. _`scale`: niDirectionalLight/scale.html
.. _`specular`: niDirectionalLight/specular.html
.. _`translation`: niDirectionalLight/translation.html
.. _`type`: niDirectionalLight/type.html

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

    niDirectionalLight/attachProperty
    niDirectionalLight/clearTransforms
    niDirectionalLight/clone
    niDirectionalLight/detachProperty
    niDirectionalLight/getObjectByName
    niDirectionalLight/getProperty
    niDirectionalLight/isInstanceOfType
    niDirectionalLight/isOfType
    niDirectionalLight/prependController
    niDirectionalLight/removeAllControllers
    niDirectionalLight/removeController
    niDirectionalLight/update
    niDirectionalLight/updateEffects
    niDirectionalLight/updateProperties

.. _`attachProperty`: niDirectionalLight/attachProperty.html
.. _`clearTransforms`: niDirectionalLight/clearTransforms.html
.. _`clone`: niDirectionalLight/clone.html
.. _`detachProperty`: niDirectionalLight/detachProperty.html
.. _`getObjectByName`: niDirectionalLight/getObjectByName.html
.. _`getProperty`: niDirectionalLight/getProperty.html
.. _`isInstanceOfType`: niDirectionalLight/isInstanceOfType.html
.. _`isOfType`: niDirectionalLight/isOfType.html
.. _`prependController`: niDirectionalLight/prependController.html
.. _`removeAllControllers`: niDirectionalLight/removeAllControllers.html
.. _`removeController`: niDirectionalLight/removeController.html
.. _`update`: niDirectionalLight/update.html
.. _`updateEffects`: niDirectionalLight/updateEffects.html
.. _`updateProperties`: niDirectionalLight/updateProperties.html

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
