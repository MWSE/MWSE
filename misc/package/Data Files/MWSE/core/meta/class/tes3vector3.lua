--- @meta

--- A simple trio of floating-point numbers.
--- @class tes3vector3
--- @field b number The third value in the vector. An alias for z.
--- @field g number The second value in the vector. An alias for y.
--- @field r number The first value in the vector. An alias for x.
--- @field x number The first value in the vector.
--- @field y number The second value in the vector.
--- @field z number The third value in the vector.
tes3vector3 = {}

--- Creates a copy of the vector.
--- @return tes3vector3 result No description yet available.
function tes3vector3:copy() end

--- Calculates the cross product with another vector.
--- @param vec tes3vector3 No description yet available.
--- @return tes3vector3 result No description yet available.
function tes3vector3:cross(vec) end

--- Calculates the distance to another vector.
--- @param vec tes3vector3 No description yet available.
--- @return number result No description yet available.
function tes3vector3:distance(vec) end

--- Calculates the dot product with another vector.
--- @param vec tes3vector3 No description yet available.
--- @return tes3vector3 result No description yet available.
function tes3vector3:dot(vec) end

--- Calculates the vertical distance to another vector.
--- @param vec tes3vector3 No description yet available.
--- @return number result No description yet available.
function tes3vector3:heightDifference(vec) end

--- Calculates the interpolated position against the target vector using the distance parameter.
--- @param targetPoint tes3vector3 No description yet available.
--- @param distance float No description yet available.
--- @return tes3vector3 result No description yet available.
function tes3vector3:interpolate(targetPoint, distance) end

--- Calculates the length of the vector.
--- @return number result No description yet available.
function tes3vector3:length() end

--- Calculates the interpolated vector between this vector and another, given a transition.
--- @param toVector tes3vector3 The vector to interpolate towards.
--- @param transition number The interpolation value. Must be between 0.0 (closer to this vector) and 1.0 (closer to the other vector).
--- @return any result No description yet available.
function tes3vector3:lerp(toVector, transition) end

--- Negates all values in the vector.
function tes3vector3:negate() end

--- Normalize the vector in-place, or set its components to zero if normalization is not possible. Returns true if the vector was successfully normalized.
--- @return bool result No description yet available.
function tes3vector3:normalize() end

--- Get a normalized copy of the vector.
--- @return tes3vector3 result No description yet available.
function tes3vector3:normalized() end

--- Calculates the outer product with another vector.
--- @param vec tes3vector3 No description yet available.
--- @return tes3matrix33 result No description yet available.
function tes3vector3:outerProduct(vec) end

