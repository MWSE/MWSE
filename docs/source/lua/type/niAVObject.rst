niAVObject
====================================================================================================

The typical base type for most NetImmerse scene graph objects.

Properties
----------------------------------------------------------------------------------------------------

`alphaProperty`_ (`niAlphaProperty`_, `nil`_)
    Convenient access to this object's alpha property. Setting this value to be nil will erase the property, while setting it to a valid alpha property will set (or replace) it.

`appCulled`_ (`boolean`_)
    A flag indicating if this object is culled. When culled, it will not render, and raycasts ignore it.

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

    niAVObject/alphaProperty
    niAVObject/appCulled
    niAVObject/flags
    niAVObject/fogProperty
    niAVObject/materialProperty
    niAVObject/name
    niAVObject/parent
    niAVObject/properties
    niAVObject/references
    niAVObject/rotation
    niAVObject/runTimeTypeInformation
    niAVObject/scale
    niAVObject/stencilProperty
    niAVObject/texturingProperty
    niAVObject/translation
    niAVObject/vertexColorProperty
    niAVObject/zBufferProperty

.. _`alphaProperty`: niAVObject/alphaProperty.html
.. _`appCulled`: niAVObject/appCulled.html
.. _`flags`: niAVObject/flags.html
.. _`fogProperty`: niAVObject/fogProperty.html
.. _`materialProperty`: niAVObject/materialProperty.html
.. _`name`: niAVObject/name.html
.. _`parent`: niAVObject/parent.html
.. _`properties`: niAVObject/properties.html
.. _`references`: niAVObject/references.html
.. _`rotation`: niAVObject/rotation.html
.. _`runTimeTypeInformation`: niAVObject/runTimeTypeInformation.html
.. _`scale`: niAVObject/scale.html
.. _`stencilProperty`: niAVObject/stencilProperty.html
.. _`texturingProperty`: niAVObject/texturingProperty.html
.. _`translation`: niAVObject/translation.html
.. _`vertexColorProperty`: niAVObject/vertexColorProperty.html
.. _`zBufferProperty`: niAVObject/zBufferProperty.html

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

    niAVObject/attachProperty
    niAVObject/clearTransforms
    niAVObject/clone
    niAVObject/detachProperty
    niAVObject/getObjectByName
    niAVObject/getProperty
    niAVObject/isInstanceOfType
    niAVObject/isOfType
    niAVObject/prependController
    niAVObject/removeAllControllers
    niAVObject/removeController
    niAVObject/update
    niAVObject/updateEffects
    niAVObject/updateProperties

.. _`attachProperty`: niAVObject/attachProperty.html
.. _`clearTransforms`: niAVObject/clearTransforms.html
.. _`clone`: niAVObject/clone.html
.. _`detachProperty`: niAVObject/detachProperty.html
.. _`getObjectByName`: niAVObject/getObjectByName.html
.. _`getProperty`: niAVObject/getProperty.html
.. _`isInstanceOfType`: niAVObject/isInstanceOfType.html
.. _`isOfType`: niAVObject/isOfType.html
.. _`prependController`: niAVObject/prependController.html
.. _`removeAllControllers`: niAVObject/removeAllControllers.html
.. _`removeController`: niAVObject/removeController.html
.. _`update`: niAVObject/update.html
.. _`updateEffects`: niAVObject/updateEffects.html
.. _`updateProperties`: niAVObject/updateProperties.html

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
