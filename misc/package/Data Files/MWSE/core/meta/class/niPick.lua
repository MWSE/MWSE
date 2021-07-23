--- @meta

--- Class used in the picking intersection system within the game engine.  Using a ray, the engine performs picking operations on a scene graph or any subtree. Given a ray and a subtree, the subtree is traversed and matching nodes are added to an array.
--- @class niPick
--- @field coordinateType niPickCoordinateType The coordinate type for the object.
--- @field frontOnly boolean If true, back face triangles (those with clockwise winding order) are culled prior to intersection calculations. If false, intersections of ray with triangles are computed regardless of triangle orientation.
---     
--- @field intersectType niPickIntersectType The intersect type for the object.
--- @field lastAddedRecord niPickRecord The pick record that was last added to the object.
--- @field observeAppCullFlag boolean If true, the pick stops if it encounters an object with the 'app culled' flag set.  If false, the pick will continue over 'app culled' objects.
---     
--- @field pickType niPickType The pick type for the object.
--- @field results niPickRecordTArray The array of pick records obtained by the last call to PickObjects.
--- @field returnColor boolean If true, a vertex color value is returned for the point of intersection of triangles during picking.
--- @field returnNormal boolean If true, a unit-length normal is returned for the point of intersection of triangles intersected during the picking.
--- @field returnSmoothNormal boolean If true and if vertex normals exist, then a unit-length normal vector is interpolated from the vertex normals and returned during the picking operation.
--- @field returnTexture boolean If true, texture coordinates are returned for the point of intersection of triangles during the picking operation.
--- @field root niPickType The root pick type for the object.
--- @field sortType niPickSortType The sort type for the object.
niPick = {}

--- Clears the results of any previous picking operation.
function niPick:clearResults() end

--- Performs the picking operation.
--- @param origin tes3vector3 No description yet available.
--- @param distance tes3vector3 No description yet available.
--- @param append boolean Default value: false
--- @param maxDistance number Default value: 0.0F
function niPick:pickObjects(origin, distance, append, maxDistance) end

