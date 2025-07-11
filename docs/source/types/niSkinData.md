# niSkinData
<div class="search_terms" style="display: none">niskindata, skindata</div>

<!---
	This file is autogenerated. Do not edit this file manually. Your changes will be ignored.
	More information: https://github.com/MWSE/MWSE/tree/master/docs
-->

Contains skinning data that may be shared by multiple `NiSkinInstance` objects.

This type inherits the following: [niObject](../types/niObject.md).
## Properties

### `boneData`
<div class="search_terms" style="display: none">bonedata</div>

*Read-only*. An array of objects containing one entry for each bone that influences vertices in the skinned mesh. The order of the entries in the `boneData` array corresponds to the order of the bones in the bone array in the `niSkinInstance` objects that point to this `niSkinData`. Each object in this array contains all the data needed to deform vertices by a single bone.

**Returns**:

* `result` ([niSkinDataBoneData](../types/niSkinDataBoneData.md)[])

***

### `partition`
<div class="search_terms" style="display: none">partition</div>

Access to the container with the skinning data optimized for hardware skinning. If the `niSkinData` object has not been partitioned, this property is `nil`.

**Returns**:

* `result` ([niSkinPartition](../types/niSkinPartition.md), nil)

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

### `transform`
<div class="search_terms" style="display: none">transform</div>

Defines the transform of the root bone in the bind pose from the parent node of the root bone to the coordinate system of the skinned object.

**Returns**:

* `result` ([tes3transform](../types/tes3transform.md))

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

