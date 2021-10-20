# niQuaternion

A rotation in quaternion representation.

## Properties

### `w`

The W component of the quaternion.

**Returns**:

* `result` (number)

***

### `x`

The X component of the quaternion.

**Returns**:

* `result` (number)

***

### `y`

The Y component of the quaternion.

**Returns**:

* `result` (number)

***

### `z`

The Z component of the quaternion.

**Returns**:

* `result` (number)

***

## Methods

### `copy`

Creates a copy of the quaternion.

```lua
local result = niQuaternion:copy()
```

**Returns**:

* `result` ([niQuaternion](../../types/niQuaternion))

***

### `fromAxisAngle`

Fill the quaternion by converting an angle-axis rotation. The angle must be within the interval [0, PI] and the axis must be unit length.

```lua
niQuaternion:fromAxisAngle(angle, axis)
```

**Parameters**:

* `angle` (float)
* `axis` ([tes3vector3](../../types/tes3vector3))

***

### `fromRotation`

Fill the quaternion by converting a rotation matrix.

```lua
niQuaternion:fromRotation(matrix)
```

**Parameters**:

* `matrix` ([tes3matrix33](../../types/tes3matrix33))

***

### `slerp`

Calculates the spherical linear interpolate between this quaternion and another.

```lua
local result = niQuaternion:slerp(target, transition)
```

**Parameters**:

* `target` ([niQuaternion](../../types/niQuaternion)): The quaternion to interpolate towards.
* `transition` (number): The interpolation value. Must be between `0.0` (closer to this quaternion) and `1.0` (closer to the other quaternion).

**Returns**:

* `result` ([niQuaternion](../../types/niQuaternion)): The calculated result.

***

### `toAxisAngle`

Convert this quaternion into an angle-axis rotation.

```lua
local angle, axis = niQuaternion:toAxisAngle()
```

**Returns**:

* `angle` (float)
* `axis` ([tes3vector3](../../types/tes3vector3))

***

### `toRotation`

Convert this quaternion into a rotation matrix.

```lua
local result = niQuaternion:toRotation()
```

**Returns**:

* `result` ([tes3matrix33](../../types/tes3matrix33))

***

