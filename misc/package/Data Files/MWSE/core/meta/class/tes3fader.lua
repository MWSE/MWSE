--- @meta

--- An object that applies a graphical effect on the screen, such as screen glare or damage coloring.
--- @class tes3fader
--- @field active boolean The activation state for the fader. Setting this effectively calls activate/deactivate.
tes3fader = {}

--- Creates a new fader, and adds it to the fader system.
--- @return tes3fader result No description yet available.
function tes3fader.new() end

--- Activates a deactivated fader.
function tes3fader:activate() end

--- Deactivates an activated fader.
function tes3fader:deactivate() end

--- Transitions the fader to a value of 1 over a given duration.
--- @param params tes3fader.fadeIn.params This table accepts the following values:
--- 
--- ``duration``: number — The time it takes to fade, in seconds.
function tes3fader:fadeIn(params) end

---Table parameter definitions for ``tes3fader.fadeIn``.
--- @class tes3fader.fadeIn.params
--- @field duration number The time it takes to fade, in seconds.

--- Transitions the fader to a value of 0 over a given duration.
--- @param params tes3fader.fadeOut.params This table accepts the following values:
--- 
--- ``duration``: number — The time it takes to fade, in seconds.
function tes3fader:fadeOut(params) end

---Table parameter definitions for ``tes3fader.fadeOut``.
--- @class tes3fader.fadeOut.params
--- @field duration number The time it takes to fade, in seconds.

--- Transitions the fader to a value over a given duration.
--- @param params tes3fader.fadeTo.params This table accepts the following values:
--- 
--- ``value``: number — The value to fade to.
--- 
--- ``duration``: number — The time it takes to fade, in seconds.
function tes3fader:fadeTo(params) end

---Table parameter definitions for ``tes3fader.fadeTo``.
--- @class tes3fader.fadeTo.params
--- @field value number The value to fade to.
--- @field duration number The time it takes to fade, in seconds.

--- Applies a coloring effect to the fader.
--- @param params tes3fader.setColor.params This table accepts the following values:
--- 
--- ``color``: tes3vector3|table — The RGB values to set.
--- 
--- ``flag``: boolean — No description yet available.
--- @return boolean result No description yet available.
function tes3fader:setColor(params) end

---Table parameter definitions for ``tes3fader.setColor``.
--- @class tes3fader.setColor.params
--- @field color tes3vector3|table The RGB values to set.
--- @field flag boolean No description yet available.

--- Updates the fader for the current frame.
--- @param path string A path for the texture that will be displayed on screen.
function tes3fader:setTexture(path) end

--- Updates the fader for the current frame.
function tes3fader:update() end

--- Updates the fader for the current frame.
--- @param value number No description yet available.
function tes3fader:updateMaterialProperty(value) end

