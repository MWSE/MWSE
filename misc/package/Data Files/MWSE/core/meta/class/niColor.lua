--- @meta

--- An object that represents a RGB color.
--- @class niColor
--- @field b number The blue value of the color.
--- @field blue number Alias for the blue value of the color.
--- @field g number The green value of the color.
--- @field green number Alias for the green value of the color.
--- @field r number The red value of the color.
--- @field red number Alias for the red value of the color.
niColor = {}

--- Clamps the color object.
function niColor:clamp() end

--- Creates a copy of the color object.
--- @return niColor result No description yet available.
function niColor:copy() end

--- Calculates the interpolated color between this color and another, given a transition.
--- @param tocolor niColor The color to interpolate towards.
--- @param transition number The interpolation value. Must be between 0.0 (closer to this color) and 1.0 (closer to the other color).
--- @return any result No description yet available.
function niColor:lerp(tocolor, transition) end

--- Creates a vector3 of the color object.
--- @return tes3vector3 result No description yet available.
function niColor:toVector3() end

