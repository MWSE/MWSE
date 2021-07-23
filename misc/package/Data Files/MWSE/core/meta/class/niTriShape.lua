--- @meta

--- An object that allows the sharing of vertices between triangles to create shapes.
--- @class niTriShape : niTriBasedGeometry, niGeometry, niAVObject, niObjectNET, niObject
--- @field data niTriShapeData The NiTriShapeData of the object. Manipulation should not be done directly. Instead, consider copying the data and assigning it to this shape.
--- @field normals tes3vector3 The normals of the object.
--- @field vertices tes3vector3 The vertices of the object.
niTriShape = {}

