--- @meta

--- A 3 by 3 matrix.
--- @class tes3matrix33
--- @field x tes3vector3 The first row of the matrix.
--- @field y tes3vector3 The second row of the matrix.
--- @field z tes3vector3 The third row of the matrix.
tes3matrix33 = {}

--- Creates a copy of the matrix.
--- @return tes3matrix33 result No description yet available.
function tes3matrix33:copy() end

--- Fills the matrix with values from euler coordinates.
--- @param x number No description yet available.
--- @param y number No description yet available.
--- @param z number No description yet available.
function tes3matrix33:fromEulerXYZ(x, y, z) end

--- Fills the matrix with values from euler coordinates.
--- @param z number No description yet available.
--- @param y number No description yet available.
--- @param x number No description yet available.
function tes3matrix33:fromEulerZYX(z, y, x) end

--- Fill the matrix by converting a quaternion.
--- @param quaternion niQuaternion No description yet available.
function tes3matrix33:fromQuaternion(quaternion) end

--- Inverts the matrix.
--- @return tes3matrix33 matrix No description yet available.
--- @return boolean valid No description yet available.
function tes3matrix33:invert() end

--- Reorthogonalizes the matrix.
--- @return boolean result No description yet available.
function tes3matrix33:reorthogonalize() end

--- Converts the matrix to a vector with 3 Euler angles.
--- @return boolean isUnique No description yet available.
--- @return tes3vector3 vector3 No description yet available.
function tes3matrix33:toEulerXYZ() end

--- Converts the matrix to a vector with 3 Euler angles.
--- @return boolean isUnique No description yet available.
--- @return tes3vector3 vector3 No description yet available.
function tes3matrix33:toEulerZYX() end

--- Converts the matrix to the identity matrix's values.
function tes3matrix33:toIdentity() end

--- Convert the matrix into a quaternion.
--- @return niQuaternion result No description yet available.
function tes3matrix33:toQuaternion() end

--- No description yet available.
--- @param angle number No description yet available.
--- @param x number No description yet available.
--- @param y number No description yet available.
--- @param z number No description yet available.
function tes3matrix33:toRotation(angle, x, y, z) end

--- No description yet available.
--- @param x number No description yet available.
function tes3matrix33:toRotationX(x) end

--- No description yet available.
--- @param y number No description yet available.
function tes3matrix33:toRotationY(y) end

--- No description yet available.
--- @param z number No description yet available.
function tes3matrix33:toRotationZ(z) end

--- Zeroes out all values in the matrix.
function tes3matrix33:toZero() end

--- No description yet available.
--- @return tes3matrix33 result No description yet available.
function tes3matrix33:transpose() end

