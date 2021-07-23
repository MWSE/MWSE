--- @meta

--- This library is an interface to the standard C math library. It provides all its functions inside the table math.
--- @class mathlib
math = {}

--- Returns a value, limited by upper and lower bounds.
--- @param value number No description yet available.
--- @param min number No description yet available.
--- @param max number No description yet available.
--- @return number result No description yet available.
function math.clamp(value, min, max) end

--- Performs linear interpolation between values v0 and v1. Returns a value that is t percent between them.
--- @param v0 number First value.
--- @param v1 number Second value.
--- @param t number The decimal percentage used to calculate a point between v0 and v1.
--- @return number result No description yet available.
function math.lerp(v0, v1, t) end

--- Returns a value, scaled from expected values [lowIn, highIn] to [lowOut, highOut].
--- 
--- For example, a value of 7 remapped from [0,10] to [0,100] would be 70.
--- @param value number No description yet available.
--- @param lowIn number No description yet available.
--- @param highIn number No description yet available.
--- @param lowOut number No description yet available.
--- @param highOut number No description yet available.
--- @return number result No description yet available.
function math.remap(value, lowIn, highIn, lowOut, highOut) end

--- Rounds a number to a given count of digits.
--- @param value number No description yet available.
--- @param digits number No description yet available.
--- @return number result No description yet available.
function math.round(value, digits) end

