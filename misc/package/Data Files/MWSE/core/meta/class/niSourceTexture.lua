--- @meta
--- @diagnostic disable:undefined-doc-name

--- A texture that represent both static and dynamic content, as NiSourceTexture data objects can have their pixel data modified on the fly to implement dynamic texture behavior.
--- @class niSourceTexture : niTexture, niObjectNET, niObject
--- @field fileName string *Read-only*. The platform-independent version of the filename from which the image was created, or NULL if the image was created from pixel data.
--- @field isStatic boolean The static flag.
--- @field pixelData niPixelData The app-level pixel data.
--- @field platformFilename string *Read-only*. The platform-specific version of the filename.
niSourceTexture = {}

--- Creates an NiSourceTexture from the given filepath.
--- @param path string The filepath of the texture to load.
--- @param useCached boolean? *Default*: `true`. If true, the texture will be stored in the normal texture source cache, so that multiple calls to the same path will return the same object. This behavior can be disabled if necessary.
function niSourceTexture.createFromPath(path, useCached) end

--- Detaches any pixel data associated with this texture. Any render-specific data will be maintained, and remain in the GPU's memory.
function niSourceTexture:clearPixelData() end

--- Loads the file associated with the texture into memory, and makes it accessible from the pixelData property.
function niSourceTexture:loadPixelDataFromFile() end
