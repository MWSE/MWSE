# niCamera

Object that represent a camera. Position and orientation is determined by parent properties.

This type inherits the following: [niAVObject](../../types/niAVObject), [niObjectNET](../../types/niObjectNET), [niObject](../../types/niObject)
## Properties

### `alphaProperty`

Convenient access to this object's alpha property. Setting this value to be nil will erase the property, while setting it to a valid alpha property will set (or replace) it.

**Returns**:

* `result` ([niAlphaProperty](../../types/niAlphaProperty), nil)

***

### `appCulled`

A flag indicating if this object is culled. When culled, it will not render, and raycasts ignore it.

**Returns**:

* `result` (boolean)

***

### `controller`

*Read-only*. The first controller available on the object.

**Returns**:

* `result` ([niTimeController](../../types/niTimeController))

***

### `cullingPlanes`

The culling planes of the camera.

**Returns**:

* `result` ([tes3vector4](../../types/tes3vector4)[])

***

### `extraData`

*Read-only*. The first extra data available on the object.

**Returns**:

* `result` (niExtraData)

***

### `flags`

Flags, dependent on the specific object type.

**Returns**:

* `result` (number)

***

### `fogProperty`

Convenient access to this object's fog property. Setting this value to be nil will erase the property, while setting it to a valid fog property will set (or replace) it.

**Returns**:

* `result` ([niFogProperty](../../types/niFogProperty), nil)

***

### `lodAdjust`

The level-of-detail adjustment value for the camera. A higher value will lower the LOD of the objects drawn by the camera.

**Returns**:

* `result` (number)

***

### `materialProperty`

Convenient access to this object's material property. Setting this value to be nil will erase the property, while setting it to a valid material property will set (or replace) it.

**Returns**:

* `result` ([niMaterialProperty](../../types/niMaterialProperty), nil)

***

### `name`

The human-facing name of the given object.

**Returns**:

* `result` (string)

***

### `parent`

The object's parent. It may not have one if it is not attached to the scene.

**Returns**:

* `result` ([niNode](../../types/niNode))

***

### `port`

The port on the backbuffer of the camera.

**Returns**:

* `result` ([tes3vector4](../../types/tes3vector4))

***

### `properties`

The list of properties attached to this `niAVObject`.

**Returns**:

* `result` ([niPropertyLinkedList](../../types/niPropertyLinkedList))

***

### `refCount`

*Read-only*. The number of references that exist for this object. When this value reaches zero, the object will be deleted.

**Returns**:

* `result` (number)

***

### `references`

*Read-only*. The number of references that exist for the given object. When this value hits zero, the object's memory is freed.

**Returns**:

* `result` (string)

***

### `renderer`

*Read-only*. The renderer of the camera.

**Returns**:

* `result` ([niObject](../../types/niObject))

***

### `rotation`

The object's local rotation matrix.

**Returns**:

* `result` ([tes3matrix33](../../types/tes3matrix33))

***

### `RTTI`

*Read-only*. The runtime type information for this object. This is an alias for the `.runTimeTypeInformation` property.

**Returns**:

* `result` ([niRTTI](../../types/niRTTI))

***

### `runTimeTypeInformation`

*Read-only*. The runtime type information for this object.

**Returns**:

* `result` ([niRTTI](../../types/niRTTI))

***

### `scale`

The object's local uniform scaling factor.

**Returns**:

* `result` (number)

***

### `scene`

*Read-only*. The scene of the camera.

**Returns**:

* `result` ([niObject](../../types/niObject))

***

### `stencilProperty`

Convenient access to this object's stencil property. Setting this value to be nil will erase the property, while setting it to a valid stencil property will set (or replace) it.

**Returns**:

* `result` ([niStencilProperty](../../types/niStencilProperty), nil)

***

### `texturingProperty`

Convenient access to this object's texturing property. Setting this value to be nil will erase the property, while setting it to a valid texturing property will set (or replace) it.

**Returns**:

* `result` ([niTexturingProperty](../../types/niTexturingProperty), nil)

***

### `translation`

The object's local translation vector.

**Returns**:

* `result` ([tes3vector3](../../types/tes3vector3))

***

### `vertexColorProperty`

Convenient access to this object's vertex coloring property. Setting this value to be nil will erase the property, while setting it to a valid vertex coloring property will set (or replace) it.

**Returns**:

* `result` ([niVertexColorProperty](../../types/niVertexColorProperty), nil)

***

### `viewDistance`

The view distance of the camera.

**Returns**:

* `result` (number)

***

### `worldDirection`

Defines the X-axis of the camera.

**Returns**:

* `result` ([tes3vector3](../../types/tes3vector3))

***

### `worldRight`

Defines the Z-axis of the camera.

**Returns**:

* `result` ([tes3vector3](../../types/tes3vector3))

***

### `worldToCamera`

The world-to-camera transform matrix in row-major order.

**Returns**:

* `result` ([tes3matrix44](../../types/tes3matrix44))

***

### `worldUp`

Defines the Y-axis of the camera.

**Returns**:

* `result` ([tes3vector3](../../types/tes3vector3))

***

### `zBufferProperty`

Convenient access to this object's z-buffer property. Setting this value to be nil will erase the property, while setting it to a valid z-buffer property will set (or replace) it.

**Returns**:

* `result` (niZBufferProperty, nil)

***

## Methods

### `attachProperty`

Attach a property to this object.

```lua
myObject:attachProperty(property)
```

**Parameters**:

* `property` ([niProperty](../../types/niProperty))

***

### `clearTransforms`

Resets the object's local transform.

```lua
myObject:clearTransforms()
```

***

### `clone`

Creates a copy of this object.

```lua
local result = myObject:clone()
```

**Returns**:

* `result` ([niObject](../../types/niObject))

***

### `detachProperty`

Detaches and returns a property from the object which matches the given property type.

```lua
local result = myObject:detachProperty(type)
```

**Parameters**:

* `type` (number)

**Returns**:

* `result` ([niProperty](../../types/niProperty))

***

### `getGameReference`

Searches for an niExtraData on this object to see if it has one that holds a related reference.

```lua
local reference = myObject:getGameReference(searchParents)
```

**Parameters**:

* `searchParents` (boolean): *Default*: `false`. If true, all parent objects (if applicable) are also searched.

**Returns**:

* `reference` ([tes3reference](../../types/tes3reference))

***

### `getObjectByName`

Searches this node and all child nodes recursively for a node with a name that matches the argument.

```lua
local result = myObject:getObjectByName(name)
```

**Parameters**:

* `name` (string)

**Returns**:

* `result` ([niAVObject](../../types/niAVObject))

***

### `getProperty`

Gets an attached property by property type.

```lua
local result = myObject:getProperty(type)
```

**Parameters**:

* `type` (number)

**Returns**:

* `result` ([niProperty](../../types/niProperty))

***

### `isInstanceOfType`

Determines if the object is of a given type, or of a type derived from the given type. Types can be found in the tes3.niType table.

```lua
local result = myObject:isInstanceOfType(type)
```

**Parameters**:

* `type` (number)

**Returns**:

* `result` (boolean)

***

### `isOfType`

Determines if the object is of a given type. Types can be found in the tes3.niType table.

```lua
local result = myObject:isOfType(type)
```

**Parameters**:

* `type` (number)

**Returns**:

* `result` (boolean)

***

### `prependController`

Add a controller to the object as the first controller.

```lua
myObject:prependController(type)
```

**Parameters**:

* `type` ([niTimeController](../../types/niTimeController))

***

### `removeAllControllers`

Removes all controllers.

```lua
myObject:removeAllControllers()
```

***

### `removeController`

Removes a controller from the object.

```lua
myObject:removeController(controller)
```

**Parameters**:

* `controller` ([niTimeController](../../types/niTimeController))

***

### `saveBinary`

Serializes the object, and writes it to the given file.

```lua
local success = myObject:saveBinary(path)
```

**Parameters**:

* `path` (string): The path to write the file at, relative to the Morrowind installation folder.

**Returns**:

* `success` (boolean): If true the object was successfully serialized.

***

### `setFlag`

Sets a given flag in the niObjectNET flag data. The specifics use of the flag is dependent on the real underlying type.

```lua
myObject:setFlag(state, index)
```

**Parameters**:

* `state` (boolean)
* `index` (number)

***

### `update`

Updates the world transforms of this node and its children, which makes changes visible for rendering. Use after changing any local rotation, translation, scale, or bounds.

```lua
myObject:update()
```

***

### `updateEffects`

Update all attached effects.

```lua
myObject:updateEffects()
```

***

### `updateProperties`

Update all attached properties.

```lua
myObject:updateProperties()
```

***

### `windowPointToRay`

Given a screen space position, calculates the world position and outlook direction. This can be useful when trying to find a reference under a UI element, such as the cusor.

```lua
local origin, direction = myObject:windowPointToRay(point)
```

**Parameters**:

* `point` ([tes3vector2](../../types/tes3vector2)): The screen position to calculate a world ray for. Screen space is measured as ([-screenWidth/2, screenWidth/2], [-screenHeight/2, screenHeight/2]) with up-right being positive and an origin at the center of the screen.

**Returns**:

* `origin` ([tes3vector3](../../types/tes3vector3)): The world point that the given screen position looks out from.
* `direction` ([tes3vector3](../../types/tes3vector3)): The look direction of the camera from the given origin point.

***

### `worldPointToScreenPoint`

Given a world coordinate position, calculates where that point relates to the screen space.

```lua
local screenPosition = myObject:worldPointToScreenPoint(point)
```

**Parameters**:

* `point` ([tes3vector3](../../types/tes3vector3)): The world position to calculate screen position for.

**Returns**:

* `screenPosition` ([tes3vector2](../../types/tes3vector2)): The screen space position. Screen space is measured as ([-screenWidth/2, screenWidth/2], [-screenHeight/2, screenHeight/2]) with up-right being positive and an origin at the center of the screen.

***
