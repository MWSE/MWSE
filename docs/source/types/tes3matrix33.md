# tes3matrix33

A 3 by 3 matrix.

## Properties

### `x`

The first row of the matrix.

**Returns**:

* `result` ([tes3vector3](../../types/tes3vector3))

***

### `y`

The second row of the matrix.

**Returns**:

* `result` ([tes3vector3](../../types/tes3vector3))

***

### `z`

The third row of the matrix.

**Returns**:

* `result` ([tes3vector3](../../types/tes3vector3))

***

## Methods

### `copy`

Creates a copy of the matrix.

```lua
local result = tes3matrix33:copy()
```

**Returns**:

* `result` ([tes3matrix33](../../types/tes3matrix33))

***

### `fromEulerXYZ`

Fills the matrix with values from euler coordinates.

```lua
tes3matrix33:fromEulerXYZ(x, y, z)
```

**Parameters**:

* `x` (number)
* `y` (number)
* `z` (number)

***

### `fromEulerZYX`

Fills the matrix with values from euler coordinates.

```lua
tes3matrix33:fromEulerZYX(z, y, x)
```

**Parameters**:

* `z` (number)
* `y` (number)
* `x` (number)

***

### `fromQuaternion`

Fill the matrix by converting a quaternion.

```lua
tes3matrix33:fromQuaternion(quaternion)
```

**Parameters**:

* `quaternion` ([niQuaternion](../../types/niQuaternion))

***

### `invert`

Inverts the matrix.

```lua
local matrix, valid = tes3matrix33:invert()
```

**Returns**:

* `matrix` ([tes3matrix33](../../types/tes3matrix33))
* `valid` (boolean)

***

### `reorthogonalize`

Reorthogonalizes the matrix.

```lua
local result = tes3matrix33:reorthogonalize()
```

**Returns**:

* `result` (boolean)

***

### `toEulerXYZ`

Converts the matrix to a vector with 3 Euler angles.

```lua
local isUnique, vector3 = tes3matrix33:toEulerXYZ()
```

**Returns**:

* `isUnique` (boolean)
* `vector3` ([tes3vector3](../../types/tes3vector3))

***

### `toEulerZYX`

Converts the matrix to a vector with 3 Euler angles.

```lua
local isUnique, vector3 = tes3matrix33:toEulerZYX()
```

**Returns**:

* `isUnique` (boolean)
* `vector3` ([tes3vector3](../../types/tes3vector3))

***

### `toIdentity`

Converts the matrix to the identity matrix's values.

```lua
tes3matrix33:toIdentity()
```

***

### `toQuaternion`

Convert the matrix into a quaternion.

```lua
local result = tes3matrix33:toQuaternion()
```

**Returns**:

* `result` ([niQuaternion](../../types/niQuaternion))

***

### `toRotation`

No description yet available.

```lua
tes3matrix33:toRotation(angle, x, y, z)
```

**Parameters**:

* `angle` (number)
* `x` (number)
* `y` (number)
* `z` (number)

***

### `toRotationX`

No description yet available.

```lua
tes3matrix33:toRotationX(x)
```

**Parameters**:

* `x` (number)

***

### `toRotationY`

No description yet available.

```lua
tes3matrix33:toRotationY(y)
```

**Parameters**:

* `y` (number)

***

### `toRotationZ`

No description yet available.

```lua
tes3matrix33:toRotationZ(z)
```

**Parameters**:

* `z` (number)

***

### `toZero`

Zeroes out all values in the matrix.

```lua
tes3matrix33:toZero()
```

***

### `transpose`

No description yet available.

```lua
local result = tes3matrix33:transpose()
```

**Returns**:

* `result` ([tes3matrix33](../../types/tes3matrix33))

***

