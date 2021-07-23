niCamera
====================================================================================================

Object that represent a camera. Position and orientation is determined by parent properties.

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

`lodAdjust`_ (`number`_)
    The level-of-detail adjustment value for the camera. A higher value will lower the LOD of the objects drawn by the camera.

`materialProperty`_ (`niMaterialProperty`_, `nil`_)
    Convenient access to this object's material property. Setting this value to be nil will erase the property, while setting it to a valid material property will set (or replace) it.

`name`_ (`string`_)
    The human-facing name of the given object.

`parent`_ (`niNode`_)
    The object's parent. It may not have one if it is not attached to the scene.

`port`_ (`tes3vector4`_)
    The port on the backbuffer of the camera.

`properties`_ (`niPropertyLinkedList`_)
    The list of properties attached to this niAVObject.

`references`_ (`string`_)
    Read-only. The number of references that exist for the given object. When this value hits zero, the object's memory is freed.

`renderer`_ (`niObject`_)
    Read-only. The renderer of the camera.

`rotation`_ (`tes3matrix33`_)
    The object's local rotation matrix.

`runTimeTypeInformation`_ (`niRTTI`_)
    The runtime type information for this object.

`scale`_ (`number`_)
    The object's local uniform scaling factor.

`scene`_ (`niObject`_)
    Read-only. The scene of the camera.

`stencilProperty`_ (`niStencilProperty`_, `nil`_)
    Convenient access to this object's stencil property. Setting this value to be nil will erase the property, while setting it to a valid stencil property will set (or replace) it.

`texturingProperty`_ (`niTexturingProperty`_, `nil`_)
    Convenient access to this object's texturing property. Setting this value to be nil will erase the property, while setting it to a valid texturing property will set (or replace) it.

`translation`_ (`tes3vector3`_)
    The object's local translation vector.

`vertexColorProperty`_ (`niVertexColorProperty`_, `nil`_)
    Convenient access to this object's vertex coloring property. Setting this value to be nil will erase the property, while setting it to a valid vertex coloring property will set (or replace) it.

`viewDistance`_ (`number`_)
    The view distance of the camera.

`worldDirection`_ (`tes3vector3`_)
    Defines the X-axis of the camera.

`worldRight`_ (`tes3vector3`_)
    Defines the Z-axis of the camera.

`worldToCamera`_ (`tes3matrix44`_)
    The world-to-camera transform matrix in row-major order.

`worldUp`_ (`tes3vector3`_)
    Defines the Y-axis of the camera.

`zBufferProperty`_ (`niZBufferProperty`_, `nil`_)
    Convenient access to this object's z-buffer property. Setting this value to be nil will erase the property, while setting it to a valid z-buffer property will set (or replace) it.

.. toctree::
    :hidden:

    niCamera/alphaProperty
    niCamera/appCulled
    niCamera/flags
    niCamera/fogProperty
    niCamera/lodAdjust
    niCamera/materialProperty
    niCamera/name
    niCamera/parent
    niCamera/port
    niCamera/properties
    niCamera/references
    niCamera/renderer
    niCamera/rotation
    niCamera/runTimeTypeInformation
    niCamera/scale
    niCamera/scene
    niCamera/stencilProperty
    niCamera/texturingProperty
    niCamera/translation
    niCamera/vertexColorProperty
    niCamera/viewDistance
    niCamera/worldDirection
    niCamera/worldRight
    niCamera/worldToCamera
    niCamera/worldUp
    niCamera/zBufferProperty

.. _`alphaProperty`: niCamera/alphaProperty.html
.. _`appCulled`: niCamera/appCulled.html
.. _`flags`: niCamera/flags.html
.. _`fogProperty`: niCamera/fogProperty.html
.. _`lodAdjust`: niCamera/lodAdjust.html
.. _`materialProperty`: niCamera/materialProperty.html
.. _`name`: niCamera/name.html
.. _`parent`: niCamera/parent.html
.. _`port`: niCamera/port.html
.. _`properties`: niCamera/properties.html
.. _`references`: niCamera/references.html
.. _`renderer`: niCamera/renderer.html
.. _`rotation`: niCamera/rotation.html
.. _`runTimeTypeInformation`: niCamera/runTimeTypeInformation.html
.. _`scale`: niCamera/scale.html
.. _`scene`: niCamera/scene.html
.. _`stencilProperty`: niCamera/stencilProperty.html
.. _`texturingProperty`: niCamera/texturingProperty.html
.. _`translation`: niCamera/translation.html
.. _`vertexColorProperty`: niCamera/vertexColorProperty.html
.. _`viewDistance`: niCamera/viewDistance.html
.. _`worldDirection`: niCamera/worldDirection.html
.. _`worldRight`: niCamera/worldRight.html
.. _`worldToCamera`: niCamera/worldToCamera.html
.. _`worldUp`: niCamera/worldUp.html
.. _`zBufferProperty`: niCamera/zBufferProperty.html

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

`windowPointToRay`_
    Given a screen space position, calculates the world position and outlook direction. This can be useful when trying to find a reference under a UI element, such as the cusor.

`worldPointToScreenPoint`_
    Given a world coordinate position, calculates where that point relates to the screen space.

.. toctree::
    :hidden:

    niCamera/attachProperty
    niCamera/clearTransforms
    niCamera/clone
    niCamera/detachProperty
    niCamera/getObjectByName
    niCamera/getProperty
    niCamera/isInstanceOfType
    niCamera/isOfType
    niCamera/prependController
    niCamera/removeAllControllers
    niCamera/removeController
    niCamera/update
    niCamera/updateEffects
    niCamera/updateProperties
    niCamera/windowPointToRay
    niCamera/worldPointToScreenPoint

.. _`attachProperty`: niCamera/attachProperty.html
.. _`clearTransforms`: niCamera/clearTransforms.html
.. _`clone`: niCamera/clone.html
.. _`detachProperty`: niCamera/detachProperty.html
.. _`getObjectByName`: niCamera/getObjectByName.html
.. _`getProperty`: niCamera/getProperty.html
.. _`isInstanceOfType`: niCamera/isInstanceOfType.html
.. _`isOfType`: niCamera/isOfType.html
.. _`prependController`: niCamera/prependController.html
.. _`removeAllControllers`: niCamera/removeAllControllers.html
.. _`removeController`: niCamera/removeController.html
.. _`update`: niCamera/update.html
.. _`updateEffects`: niCamera/updateEffects.html
.. _`updateProperties`: niCamera/updateProperties.html
.. _`windowPointToRay`: niCamera/windowPointToRay.html
.. _`worldPointToScreenPoint`: niCamera/worldPointToScreenPoint.html

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
.. _`tes3matrix44`: ../../lua/type/tes3matrix44.html
.. _`tes3vector3`: ../../lua/type/tes3vector3.html
.. _`tes3vector4`: ../../lua/type/tes3vector4.html
