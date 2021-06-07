niLight
====================================================================================================

Base class that represents light sources in a scene graph. This includes the ambient, diffuse, and specular colors of a light, as well as its intensity.

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

    niLight/affectedNodes
    niLight/ambient
    niLight/appCulled
    niLight/diffuse
    niLight/dimmer
    niLight/enabled
    niLight/flags
    niLight/name
    niLight/parent
    niLight/properties
    niLight/references
    niLight/rotation
    niLight/runTimeTypeInformation
    niLight/scale
    niLight/specular
    niLight/translation
    niLight/type

.. _`affectedNodes`: niLight/affectedNodes.html
.. _`ambient`: niLight/ambient.html
.. _`appCulled`: niLight/appCulled.html
.. _`diffuse`: niLight/diffuse.html
.. _`dimmer`: niLight/dimmer.html
.. _`enabled`: niLight/enabled.html
.. _`flags`: niLight/flags.html
.. _`name`: niLight/name.html
.. _`parent`: niLight/parent.html
.. _`properties`: niLight/properties.html
.. _`references`: niLight/references.html
.. _`rotation`: niLight/rotation.html
.. _`runTimeTypeInformation`: niLight/runTimeTypeInformation.html
.. _`scale`: niLight/scale.html
.. _`specular`: niLight/specular.html
.. _`translation`: niLight/translation.html
.. _`type`: niLight/type.html

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

    niLight/attachProperty
    niLight/clearTransforms
    niLight/clone
    niLight/detachProperty
    niLight/getObjectByName
    niLight/getProperty
    niLight/isInstanceOfType
    niLight/isOfType
    niLight/prependController
    niLight/removeAllControllers
    niLight/removeController
    niLight/update
    niLight/updateEffects
    niLight/updateProperties

.. _`attachProperty`: niLight/attachProperty.html
.. _`clearTransforms`: niLight/clearTransforms.html
.. _`clone`: niLight/clone.html
.. _`detachProperty`: niLight/detachProperty.html
.. _`getObjectByName`: niLight/getObjectByName.html
.. _`getProperty`: niLight/getProperty.html
.. _`isInstanceOfType`: niLight/isInstanceOfType.html
.. _`isOfType`: niLight/isOfType.html
.. _`prependController`: niLight/prependController.html
.. _`removeAllControllers`: niLight/removeAllControllers.html
.. _`removeController`: niLight/removeController.html
.. _`update`: niLight/update.html
.. _`updateEffects`: niLight/updateEffects.html
.. _`updateProperties`: niLight/updateProperties.html

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
