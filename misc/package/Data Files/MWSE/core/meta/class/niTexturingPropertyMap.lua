--- @meta

--- A combination of a texture, a filter mode, a clamping mode, and an index to a set of texture coordinates.
--- @class niTexturingPropertyMap
--- @field clampMode niTexturingPropertyClampMode The clamp mode for the Map.
--- @field filterMode niTexturingPropertyFilterMode The filter mode for the Map.
--- @field texCoordSet number The texture coordinate set for the Map.
--- @field texture niTexture The texture for the Map.
niTexturingPropertyMap = {}

--- Creates a new basic or bump map.
--- @param params niTexturingPropertyMap.new.params This table accepts the following values:
--- 
--- ``texture``: niTexture — If provided, sets the map texture to the given value.
--- 
--- ``clampMode``: number — The clamp mode to set the map to use.
--- 
--- ``filterMode``: number — The filter mode to set the map to use.
--- 
--- ``textCoords``: number — The texture coordinates to set the map to use.
--- 
--- ``isBumpMap``: boolean — If true, a bump map will be created instead.
--- @return any result No description yet available.
function niTexturingPropertyMap.new(params) end

---Table parameter definitions for ``niTexturingPropertyMap.new``.
--- @class niTexturingPropertyMap.new.params
--- @field texture niTexture If provided, sets the map texture to the given value.
--- @field clampMode number The clamp mode to set the map to use.
--- @field filterMode number The filter mode to set the map to use.
--- @field textCoords number The texture coordinates to set the map to use.
--- @field isBumpMap boolean If true, a bump map will be created instead.

