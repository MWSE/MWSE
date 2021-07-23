--- @meta

--- Represents all texture objects that are created from a file or a block of in-memory pixel data. NiSourceTexture objects represent both static and dynamic content, as NiSourceTexture data objects can have their pixel data modified on the fly to implement dynamic texture behavior.
--- @class niSourceTexture : niObjectNET, niObject
--- @field fileName string The platform-independent version of the filename from which the image was created, or NULL if the image was created from pixel data.
--- @field formatPrefs niFormatPrefs The `NiFormatPrefs` of the texture.
--- @field height number The height of the texture.
--- @field isStatic boolean The static flag.
--- @field pixelData niPixelData The app-level pixel data.
--- @field platformFilename string The platform-specific version of the filename.
--- @field width number The width of the texture.
niSourceTexture = {}

--- Creates an NiSourceTexture from the given filepath.
--- @param path string The filepath of the texture to load.
function niSourceTexture.createFromPath(path) end

--- Detaches any pixel data associated with this texture. Any render-specific data will be maintained, and remain in the GPU's memory.
function niSourceTexture:clearPixelData() end

--- Loads the file associated with the texture into memory, and makes it accessible from the pixelData property.
function niSourceTexture:loadPixelDataFromFile() end

