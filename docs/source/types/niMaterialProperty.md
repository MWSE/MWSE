# niMaterialProperty

A rendering property that controls the surface properties of objects affected by the property. These surface properties include translucency, ambient reflective color, diffuse reflective color, emissive color intensity, and specular color.

This type inherits the following: [niProperty](../../types/niProperty), [niObjectNET](../../types/niObjectNET), [niObject](../../types/niObject)
## Properties

### `alpha`

The alpha value of the material. Value should be between 0 - 1.

**Returns**:

* `result` (number)

***

### `ambient`

The ambient material color.

**Returns**:

* `result` ([niColor](../../types/niColor))

***

### `controller`

*Read-only*. The first controller available on the object.

**Returns**:

* `result` ([niTimeController](../../types/niTimeController))

***

### `diffuse`

The diffuse material color.

**Returns**:

* `result` ([niColor](../../types/niColor))

***

### `emissive`

The emissive material color.

**Returns**:

* `result` ([niColor](../../types/niColor))

***

### `extraData`

*Read-only*. The first extra data available on the object.

**Returns**:

* `result` (niExtraData)

***

### `name`

The human-facing name of the given object.

**Returns**:

* `result` (string)

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

### `shininess`

The shine of the material. Value should be >= 0.

**Returns**:

* `result` (number)

***

### `specular`

The specular material color.

**Returns**:

* `result` ([niColor](../../types/niColor))

***

### `type`

The unique class identifier number of the given rendering property.

**Returns**:

* `result` (niPropertyType)

***

## Methods

### `clone`

Creates a copy of this object.

```lua
local result = myObject:clone()
```

**Returns**:

* `result` ([niObject](../../types/niObject))

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
