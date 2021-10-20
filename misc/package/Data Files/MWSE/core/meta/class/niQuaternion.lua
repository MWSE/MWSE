--- @meta

--- A rotation in quaternion representation.
--- @class niQuaternion
--- @field w number The W component of the quaternion.
--- @field x number The X component of the quaternion.
--- @field y number The Y component of the quaternion.
--- @field z number The Z component of the quaternion.
niQuaternion = {}

--- Creates a copy of the quaternion.
--- @return niQuaternion result No description yet available.
function niQuaternion:copy() end

--- Fill the quaternion by converting an angle-axis rotation. The angle must be within the interval [0, PI] and the axis must be unit length.
--- @param angle float No description yet available.
--- @param axis tes3vector3 No description yet available.
function niQuaternion:fromAxisAngle(angle, axis) end

--- Fill the quaternion by converting a rotation matrix.
--- @param matrix tes3matrix33 No description yet available.
function niQuaternion:fromRotation(matrix) end

--- Calculates the spherical linear interpolate between this quaternion and another.
--- @param target niQuaternion The quaternion to interpolate towards.
--- @param transition number The interpolation value. Must be between `0.0` (closer to this quaternion) and `1.0` (closer to the other quaternion).
--- @return niQuaternion result The calculated result.
function niQuaternion:slerp(target, transition) end

--- Convert this quaternion into an angle-axis rotation.
--- @return float angle No description yet available.
--- @return tes3vector3 axis No description yet available.
function niQuaternion:toAxisAngle() end

--- Convert this quaternion into a rotation matrix.
--- @return tes3matrix33 result No description yet available.
function niQuaternion:toRotation() end

