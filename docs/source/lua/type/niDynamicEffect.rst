niDynamicEffect
====================================================================================================

Base class for dynamic effects such as NiLights or projected textures effects.

Properties
----------------------------------------------------------------------------------------------------

`affectedNodes`_ (`niNodeLinkedList`_)
    The list of nodes that a given dynamic effect will affect.

`appCulled`_ (`boolean`_)
    A flag indicating if this object is culled. When culled, it will not render, and raycasts ignore it.

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

`translation`_ (`tes3vector3`_)
    The object's local translation vector.

`type`_ (`number`_)
    The enumerated type of a given dynamic effect. Types: `0 - niAmbientLight`, `1 - niDirectionalLight`, `2 - niPointLight`, `3 - niSpotLight`, `4 - niTextureEffect`.

.. toctree::
    :hidden:

    niDynamicEffect/affectedNodes
    niDynamicEffect/appCulled
    niDynamicEffect/enabled
    niDynamicEffect/flags
    niDynamicEffect/name
    niDynamicEffect/parent
    niDynamicEffect/properties
    niDynamicEffect/references
    niDynamicEffect/rotation
    niDynamicEffect/runTimeTypeInformation
    niDynamicEffect/scale
    niDynamicEffect/translation
    niDynamicEffect/type

.. _`affectedNodes`: niDynamicEffect/affectedNodes.html
.. _`appCulled`: niDynamicEffect/appCulled.html
.. _`enabled`: niDynamicEffect/enabled.html
.. _`flags`: niDynamicEffect/flags.html
.. _`name`: niDynamicEffect/name.html
.. _`parent`: niDynamicEffect/parent.html
.. _`properties`: niDynamicEffect/properties.html
.. _`references`: niDynamicEffect/references.html
.. _`rotation`: niDynamicEffect/rotation.html
.. _`runTimeTypeInformation`: niDynamicEffect/runTimeTypeInformation.html
.. _`scale`: niDynamicEffect/scale.html
.. _`translation`: niDynamicEffect/translation.html
.. _`type`: niDynamicEffect/type.html

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
