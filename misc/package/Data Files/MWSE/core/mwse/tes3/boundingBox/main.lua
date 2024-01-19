-- Determines whether two bounding boxes intersect or not.
---@param bb1 tes3boundingBox
---@param bb2 tes3boundingBox
function tes3boundingBox.intersects(bb1, bb2)
    -- boxes intersect iff their projections onto each coordinate axis intersect.
    -- the projection onto each coordinate axis is a closed interval of real numbers,
    -- so we can detect intersections by comparing endpoints.
    return  bb1.max.x >= bb2.min.x and bb2.max.x >= bb1.min.x
        and bb1.max.y >= bb2.min.y and bb2.max.y >= bb1.min.y
        and bb1.max.z >= bb2.min.z and bb2.max.z >= bb1.min.z
end