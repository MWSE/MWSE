--- @meta

--- A nested class of NiPick that stores the results of previous picking operations for use by the game engine.
--- @class niPickRecord
--- @field color number The vertex color at the point of intersection. The color is valid only when `returnColor = true`.
--- @field distance number The distance of the triangle intersection from the ray origin.
--- @field intersection tes3vector3 The intersection point on the triangle.
--- @field normal tes3vector3 The normal vector at the point of intersection. The normal is valid only when `returnNormal = true`.
--- @field object niAVObject The object of the record.
--- @field parent niAVObject The parent of the record.
--- @field reference tes3reference Quick reference to the associated object's reference.
--- @field texture tes3vector2 The texture coordinates at the point of intersection. The coordinates are valid only when `returnTexture = true`.
--- @field triangleIndex number The index of the intersected triangle.
--- @field vertexIndex number The index of the vertex of the intersected triangle.
niPickRecord = {}

