--- @meta

---         NiGeometryData objects contain the geometry data necessary to render an NiGeometry object. When an NiGeometry-based object is created, the actual geometry data is stored in an attached NiGeometryData object. 
---     
--- @class niGeometryData : niObject
--- @field bounds niBound The bounds of the object.
--- @field colors niColorA The color for the object.
--- @field normals tes3vector3 The normals list for the object.
--- @field texCoords tes3vector2 The array of texture coordinates.
--- @field textures tes3vector2 The array of texture coordinates.
--- @field textureSets number The number of texture coordinate sets in the data.
--- @field uniqueID number A unique ID for this model, assigned at model creation.
--- @field vertexCount number The vertex count for the object.
--- @field vertices tes3vector3 The array of vertex position data.
niGeometryData = {}

--- Tells the renderer that the object has changed. Should be called after you have finished changing any vertex data.
--- 
--- If you have altered vertex positions, you may need to also call `updateModelBound`_. You should call it if vertices have been moved outside the bounds of the original model.
function niGeometryData:markAsChanged() end

--- Updates the geometry bounds to match the vertex data. You should call it if vertices have been moved outside the bounds of the original model, or if the effective bounds have become significantly smaller. 
--- 
--- If you already know the effective radius of the vertex data, you could more efficiently set the bounds directly instead of calling this function.
function niGeometryData:updateModelBound() end

