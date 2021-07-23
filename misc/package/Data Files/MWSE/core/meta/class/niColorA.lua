--- @meta

--- An object that represents a RGB color with alpha channel support.
--- @class niColorA
--- @field a number The alpha value of the color.
--- @field alpha number The alpha value of the color.
--- @field b number The blue value of the color.
--- @field blue number Alias for the blue value of the color.
--- @field g number The green value of the color.
--- @field green number Alias for the green value of the color.
--- @field r number The red value of the color.
--- @field red number Alias for the red value of the color.
niColorA = {}

--- Creates a copy of the color object.
--- @return niColorA result No description yet available.
function niColorA:copy() end

--- Calculates the interpolated color between this color and another, given a transition.
--- @param tocolor niColorA The color to interpolate towards.
--- @param transition number The interpolation value. Must be between 0.0 (closer to this color) and 1.0 (closer to the other color).
--- @return niColorA lerpedcolor The calculated value.
function niColorA:lerp(tocolor, transition) end

