--- @meta

-- This file is autogenerated. Do not edit this file manually. Your changes will be ignored.
-- More information: https://github.com/MWSE/MWSE/tree/master/docs

--- A rendering property that allows the application to control the method used to compute colors for each vertex in a geometry object. This class enables effects such as static pre-lighting, dynamic lighting, etc.
--- @class niVertexColorProperty : niProperty, niObjectNET, niObject
--- @field lighting ni.lightingMode Describes how vertex colors influence lighting. Maps to values in [`ni.lightingMode`](https://mwse.github.io/MWSE/references/ni/lighting-modes/) table.
--- @field source ni.sourceVertexMode Determines how vertex and material colors are mixed on subclasses of niAVObject. Maps to values in [`ni.sourceVertexMode`](https://mwse.github.io/MWSE/references/ni/source-vertex-modes/) table.
niVertexColorProperty = {}

--- Creates a new niVertexColorProperty with `lighting` set to `ni.lightingMode.emiAmbDif` and `source` set to `ni.sourceVertexMode.ignore`.
--- @return niVertexColorProperty property No description yet available.
function niVertexColorProperty.new() end

