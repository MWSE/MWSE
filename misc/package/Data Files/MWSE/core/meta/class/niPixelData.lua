--- @meta

--- Represents 2D arrays of pixel values, as well as pyramids of multiple mipmap levels, each of 2D arrays of pixel values. Also contains information representing the format of the pixels and dimensions of the arrays.
--- @class niPixelData : niObject
--- @field bytesPerPixel number The number of bytes per pixel.
--- @field mipMapLevels number The number of MipMap levels.
niPixelData = {}

--- Creates a new NiPixelData object.
--- @param width number No description yet available.
--- @param height number No description yet available.
function niPixelData.new(width, height) end

--- Creates a source texture from the current NiPixelData object.
--- @return niSourceTexture result No description yet available.
function niPixelData:createSourceTexture() end

--- Returns the height of the mipmap level at the given index, where level 1 is the largest.
--- @param mipMapLevel number Default value: 1
--- @return number result No description yet available.
function niPixelData:getHeight(mipMapLevel) end

--- Returns the width of the mipmap level at the given index, where level 1 is the largest.
--- @param mipMapLevel number Default value: 1
--- @return number result No description yet available.
function niPixelData:getWidth(mipMapLevel) end

--- Sets the pixel data from byte data.
--- @param data table The byte data to be set to.
--- @param mipMapLevel number Default value: 1
function niPixelData:setPixelsByte(data, mipMapLevel) end

--- Sets the pixel data from float data.
--- @param data table The float data to be set to.
--- @param mipMapLevel number Default value: 1
function niPixelData:setPixelsFloat(data, mipMapLevel) end

