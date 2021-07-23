--- @meta

--- The base-most object from which almost all NetImmerse structures are derived from.
--- @class niObject
--- @field references string The number of references that exist for the given object. When this value hits zero, the object's memory is freed.
--- @field runTimeTypeInformation niRTTI The runtime type information for this object.
niObject = {}

--- Creates a copy of this object.
--- @return niAlphaProperty|niAmbientLight|niCamera|niCollisionSwitch|niDirectionalLight|niDynamicEffect|niFogProperty|niGeometry|niGeometryData|niLight|niMaterialProperty|niNode|niPixelData|niPointLight|niSourceTexture|niSpotLight|niStencilProperty|niSwitchNode|niTexturingProperty|niTimeController|niTriBasedGeometry|niTriBasedGeometryData|niTriShape|niTriShapeData|niVertexColorProperty result No description yet available.
function niObject:clone() end

--- Determines if the object is of a given type, or of a type derived from the given type. Types can be found in the tes3.niType table.
--- @param type number No description yet available.
--- @return boolean result No description yet available.
function niObject:isInstanceOfType(type) end

--- Determines if the object is of a given type. Types can be found in the tes3.niType table.
--- @param type number No description yet available.
--- @return boolean result No description yet available.
function niObject:isOfType(type) end

