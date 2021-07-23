niTriShape
====================================================================================================

An object that allows the sharing of vertices between triangles to create shapes.

Properties
----------------------------------------------------------------------------------------------------

`alphaProperty`_ (`niAlphaProperty`_, `nil`_)
    Convenient access to this object's alpha property. Setting this value to be nil will erase the property, while setting it to a valid alpha property will set (or replace) it.

`appCulled`_ (`boolean`_)
    A flag indicating if this object is culled. When culled, it will not render, and raycasts ignore it.

`data`_ (`niTriShapeData`_)
    The NiTriShapeData of the object. Manipulation should not be done directly. Instead, consider copying the data and assigning it to this shape.

`flags`_ (`number`_)
    Flags, dependent on the specific object type.

`fogProperty`_ (`niFogProperty`_, `nil`_)
    Convenient access to this object's fog property. Setting this value to be nil will erase the property, while setting it to a valid fog property will set (or replace) it.

`materialProperty`_ (`niMaterialProperty`_, `nil`_)
    Convenient access to this object's material property. Setting this value to be nil will erase the property, while setting it to a valid material property will set (or replace) it.

`name`_ (`string`_)
    The human-facing name of the given object.

`normals`_ (`tes3vector3`_)
    The normals of the object.

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

`vertices`_ (`tes3vector3`_)
    The vertices of the object.

`zBufferProperty`_ (`niZBufferProperty`_, `nil`_)
    Convenient access to this object's z-buffer property. Setting this value to be nil will erase the property, while setting it to a valid z-buffer property will set (or replace) it.

.. toctree::
    :hidden:

    niTriShape/alphaProperty
    niTriShape/appCulled
    niTriShape/data
    niTriShape/flags
    niTriShape/fogProperty
    niTriShape/materialProperty
    niTriShape/name
    niTriShape/normals
    niTriShape/parent
    niTriShape/properties
    niTriShape/references
    niTriShape/rotation
    niTriShape/runTimeTypeInformation
    niTriShape/scale
    niTriShape/stencilProperty
    niTriShape/texturingProperty
    niTriShape/translation
    niTriShape/vertexColorProperty
    niTriShape/vertices
    niTriShape/zBufferProperty

.. _`alphaProperty`: niTriShape/alphaProperty.html
.. _`appCulled`: niTriShape/appCulled.html
.. _`data`: niTriShape/data.html
.. _`flags`: niTriShape/flags.html
.. _`fogProperty`: niTriShape/fogProperty.html
.. _`materialProperty`: niTriShape/materialProperty.html
.. _`name`: niTriShape/name.html
.. _`normals`: niTriShape/normals.html
.. _`parent`: niTriShape/parent.html
.. _`properties`: niTriShape/properties.html
.. _`references`: niTriShape/references.html
.. _`rotation`: niTriShape/rotation.html
.. _`runTimeTypeInformation`: niTriShape/runTimeTypeInformation.html
.. _`scale`: niTriShape/scale.html
.. _`stencilProperty`: niTriShape/stencilProperty.html
.. _`texturingProperty`: niTriShape/texturingProperty.html
.. _`translation`: niTriShape/translation.html
.. _`vertexColorProperty`: niTriShape/vertexColorProperty.html
.. _`vertices`: niTriShape/vertices.html
.. _`zBufferProperty`: niTriShape/zBufferProperty.html

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

    niTriShape/attachProperty
    niTriShape/clearTransforms
    niTriShape/clone
    niTriShape/detachProperty
    niTriShape/getObjectByName
    niTriShape/getProperty
    niTriShape/isInstanceOfType
    niTriShape/isOfType
    niTriShape/prependController
    niTriShape/removeAllControllers
    niTriShape/removeController
    niTriShape/update
    niTriShape/updateEffects
    niTriShape/updateProperties

.. _`attachProperty`: niTriShape/attachProperty.html
.. _`clearTransforms`: niTriShape/clearTransforms.html
.. _`clone`: niTriShape/clone.html
.. _`detachProperty`: niTriShape/detachProperty.html
.. _`getObjectByName`: niTriShape/getObjectByName.html
.. _`getProperty`: niTriShape/getProperty.html
.. _`isInstanceOfType`: niTriShape/isInstanceOfType.html
.. _`isOfType`: niTriShape/isOfType.html
.. _`prependController`: niTriShape/prependController.html
.. _`removeAllControllers`: niTriShape/removeAllControllers.html
.. _`removeController`: niTriShape/removeController.html
.. _`update`: niTriShape/update.html
.. _`updateEffects`: niTriShape/updateEffects.html
.. _`updateProperties`: niTriShape/updateProperties.html

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
.. _`niTriShapeData`: ../../lua/type/niTriShapeData.html
.. _`niVertexColorProperty`: ../../lua/type/niVertexColorProperty.html
.. _`nil`: ../../lua/type/nil.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`tes3matrix33`: ../../lua/type/tes3matrix33.html
.. _`tes3vector3`: ../../lua/type/tes3vector3.html
