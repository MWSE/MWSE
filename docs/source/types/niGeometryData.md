# niGeometryData
<div class="search_terms" style="display: none">nigeometrydata, geometrydata</div>

<!---
	This file is autogenerated. Do not edit this file manually. Your changes will be ignored.
	More information: https://github.com/MWSE/MWSE/tree/master/docs
-->

niGeometryData objects contain the geometry data necessary to render a niGeometry object. When a niGeometry-based object is created, the actual geometry data is stored in an attached niGeometryData object.

The distinction between niGeometry and niGeometryData (and other pairs of NetImmerse classes with similarly distinguished names, such as niTriShape and niTriShapeData) is that niGeometry stores data that cannot be shared when an object is instanced, and niGeometryData stores data that can be shared when an object is instanced. So, for example, when a scene graph is cloned, duplicate copies of the niGeometry objects in the original scene graph are created for the new scene graph, but new niGeometryData objects are not created. Instead, the newly-created niGeometry objects refer to the same niGeometryData objects referred to by the original scene graph.

This type inherits the following: [niObject](../types/niObject.md).
## Properties

### `bounds`
<div class="search_terms" style="display: none">bounds</div>

The model-space bounding sphere of the object.

**Returns**:

* `result` ([niBound](../types/niBound.md))

***

### `colors`
<div class="search_terms" style="display: none">colors</div>

*Read-only*. The vertex colors for the object. The length of the array is equal to `vertexCount`.

**Returns**:

* `result` ([niPackedColor](../types/niPackedColor.md)[])

***

### `normals`
<div class="search_terms" style="display: none">normals</div>

*Read-only*. The list of unitized, model-space vertex normals for the object. The length of the array is equal to `vertexCount`.

**Returns**:

* `result` ([tes3vector3](../types/tes3vector3.md)[])

***

### `refCount`
<div class="search_terms" style="display: none">refcount</div>

*Read-only*. The number of references that exist for this object. When this value reaches zero, the object will be deleted.

**Returns**:

* `result` (number)

***

### `RTTI`
<div class="search_terms" style="display: none">rtti</div>

*Read-only*. The runtime type information for this object. This is an alias for the `.runTimeTypeInformation` property.

**Returns**:

* `result` ([niRTTI](../types/niRTTI.md))

***

### `runTimeTypeInformation`
<div class="search_terms" style="display: none">runtimetypeinformation</div>

*Read-only*. The runtime type information for this object.

**Returns**:

* `result` ([niRTTI](../types/niRTTI.md))

***

### `texCoords`
<div class="search_terms" style="display: none">texcoords</div>

*Read-only*. The array of texture coordinates. The length of the array is equal to `vertexCount` times `textureSets`.

**Returns**:

* `result` ([tes3vector2](../types/tes3vector2.md)[])

***

### `textures`
<div class="search_terms" style="display: none">textures</div>

*Read-only*. The array of texture coordinates. The length of the array is equal to `vertexCount` times `textureSets`.

**Returns**:

* `result` ([tes3vector2](../types/tes3vector2.md)[])

***

### `textureSets`
<div class="search_terms" style="display: none">texturesets</div>

The number of texture coordinate sets in the data.

**Returns**:

* `result` (number)

***

### `uniqueID`
<div class="search_terms" style="display: none">uniqueid</div>

*Read-only*. A unique ID for this model, assigned at model creation.

**Returns**:

* `result` (number)

***

### `vertexCount`
<div class="search_terms" style="display: none">vertexcount</div>

*Read-only*. The vertex count for the object.

**Returns**:

* `result` (number)

***

### `vertices`
<div class="search_terms" style="display: none">vertices</div>

*Read-only*. The array of vertex position data. The length of the array is equal to `vertexCount`.

**Returns**:

* `result` ([tes3vector3](../types/tes3vector3.md)[])

***

## Methods

### `clone`
<div class="search_terms" style="display: none">clone</div>

Creates a copy of this object.

```lua
local result = myObject:clone()
```

**Returns**:

* `result` ([niObject](../types/niObject.md))

***

### `isInstanceOfType`
<div class="search_terms" style="display: none">isinstanceoftype, instanceoftype</div>

Determines if the object is of a given type, or of a type derived from the given type.

```lua
local result = myObject:isInstanceOfType(type)
```

**Parameters**:

* `type` ([ni.type](../references/ni/types.md)): Use values in the [`ni.type`](https://mwse.github.io/MWSE/references/ni/types/) table.

**Returns**:

* `result` (boolean)

***

### `isOfType`
<div class="search_terms" style="display: none">isoftype, oftype</div>

Determines if the object is of a given type.

```lua
local result = myObject:isOfType(type)
```

**Parameters**:

* `type` ([ni.type](../references/ni/types.md)): Use values in the [`ni.type`](https://mwse.github.io/MWSE/references/ni/types/) table.

**Returns**:

* `result` (boolean)

***

### `markAsChanged`
<div class="search_terms" style="display: none">markaschanged</div>

Tells the renderer that the object has changed. Should be called after you have finished changing any vertex data.

If you have altered vertex positions, you may need to also call `updateModelBound`. You should call it if vertices have been moved outside the bounds of the original model.

```lua
myObject:markAsChanged()
```

***

### `saveBinary`
<div class="search_terms" style="display: none">savebinary</div>

Serializes the object, and writes it to the given file.

```lua
local success = myObject:saveBinary(path)
```

**Parameters**:

* `path` (string): The path to write the file at, relative to the Morrowind installation folder. The `.nif` extension needs to be specified manually.

**Returns**:

* `success` (boolean): If true the object was successfully serialized.

***

### `updateModelBound`
<div class="search_terms" style="display: none">updatemodelbound, modelbound</div>

Updates the geometry bounds to match the vertex data. You should call it if vertices have been moved outside the bounds of the original model, or if the effective bounds have become significantly smaller. 

If you already know the effective radius of the vertex data, you could more efficiently set the bounds directly instead of calling this function.

```lua
myObject:updateModelBound()
```

