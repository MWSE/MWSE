function tes3reference.intersects(ref1, ref2)
    --[[ the basic idea is as follows:
        boxes intersect iff their projections onto each coordinate axis intersect.
        the projection onto each coordinate axis is a closed interval of real numbers,
        so we can detect intersections by comparing endpoints.
    ]]
    local bb1, bb2 = ref1.object.boundingBox, ref2.object.boundingBox
    -- make sure bounding boxes exist
    if not bb1 or not bb2 then return false end
    -- offset bounding boxes by current position of `ref1` and `ref2`.
    local bb1min = bb1.min + ref1.position
    local bb1max = bb1.max + ref1.position
    local bb2min = bb2.min + ref2.position
    local bb2max = bb2.max + ref2.position
    -- actually check if the boxes intersect
    return  bb1max.x >= bb2min.x and bb2max.x >= bb1min.x
        and bb1max.y >= bb2min.y and bb2max.y >= bb1min.y
        and bb1max.z >= bb2min.z and bb2max.z >= bb1min.z
end