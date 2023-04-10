-- This file is autogenerated. Do not edit this file manually. Your changes will be ignored.
-- More information: https://github.com/MWSE/MWSE/tree/master/docs

--- @meta
--- The Morrowind's renderer class.
--- @class niRenderer : niObject
niRenderer = {}

--- This method changes the active rendering target to the provided texture.
--- @param texture niRenderedTexture? *Default*: `nil`. Set to `nil` to reset to the default rendering target.
--- @return boolean result No description yet available.
function niRenderer:setRenderTarget(texture) end

--- This method returns the last rendered frame in the form of niPixelData.
--- @param bounds integer[] These four values are used to take only a specific sub region (in pixels) from the framebuffer. If non provided, the taken screenshot will include the whole screen.
--- @return niPixelData screenshot No description yet available.
function niRenderer:takeScreenshot(bounds) end
