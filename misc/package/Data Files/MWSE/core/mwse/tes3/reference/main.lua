---@param ref1 tes3reference
---@param ref2 tes3reference
function tes3reference.intersects(ref1, ref2)
    local bb1, bb2 = ref1.object.boundingBox, ref2.object.boundingBox
    if not bb1 or not bb2 then return false end
    local bb1min = bb1.min + ref1.position
    local bb1max = bb1.max + ref1.position
    local bb2min = bb2.min + ref2.position
    local bb2max = bb2.max + ref2.position
    return  bb1max.x >= bb2min.x and bb2max.x >= bb1min.x
        and bb1max.y >= bb2min.y and bb2max.y >= bb1min.y
        and bb1max.z >= bb2min.z and bb2max.z >= bb1min.z
end