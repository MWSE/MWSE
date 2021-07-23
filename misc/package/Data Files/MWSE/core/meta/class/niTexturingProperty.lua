--- @meta

--- A rendering property that controls the methods used to filter texture pixels, and blend texture colors and vertex colors.
--- @class niTexturingProperty : niProperty, niObjectNET, niObject
--- @field applyMode niTexturingPropertyApplyMode The texture apply mode. The texture apply mode controls how the vertex colors are blended with the texture color.
--- @field baseMap niTexturingPropertyMap|nil Gets or set the base map. Can be set to nil to delete it. Note that doing so will free the memory of the map, causing all existing handles to it to become invalid.
--- @field bumpMap niTexturingPropertyMap|nil Gets or set the bump map. Can be set to nil to delete it. Note that doing so will free the memory of the map, causing all existing handles to it to become invalid.
--- @field canAddDecal boolean If true, the texturing property supports additional decal maps.
--- @field darkMap niTexturingPropertyMap|nil Gets or set the dark map. Can be set to nil to delete it. Note that doing so will free the memory of the map, causing all existing handles to it to become invalid.
--- @field decalCount number The number of decals currently on the texturing property.
--- @field detailMap niTexturingPropertyMap|nil Gets or set the detail map. Can be set to nil to delete it. Note that doing so will free the memory of the map, causing all existing handles to it to become invalid.
--- @field glossMap niTexturingPropertyMap|nil Gets or set the gloss map. Can be set to nil to delete it. Note that doing so will free the memory of the map, causing all existing handles to it to become invalid.
--- @field glowMap niTexturingPropertyMap|nil Gets or set the glow map. Can be set to nil to delete it. Note that doing so will free the memory of the map, causing all existing handles to it to become invalid.
--- @field maps niTexturingPropertyMapTArray The maps of texture property. Accessible as an array.
niTexturingProperty = {}

--- Attempts to create a new decal map. If successful, it returns both the new map and the index it was created in.
--- @param texture niTexture The texture to assign to the new decal.
--- @return niTexturingPropertyMap|nil map A newly created decal map.
--- @return number|nil index The index of the newly added decal map.
function niTexturingProperty:addDecalMap(texture) end

--- Attempts to remove a decal at a given index.
--- @param index number The index of the decal to remove.
--- @return any result No description yet available.
function niTexturingProperty:removeDecalMap(index) end

