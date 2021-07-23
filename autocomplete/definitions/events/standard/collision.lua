return{
    description = "This event is triggered on a collision.",
    eventData = {
        ["mobile"] = {
            type = "tes3mobileObject",
            readOnly = true,
            description = "Mobile object which collided with its target reference.",
        },
        ["reference"] = {
            type = "tes3reference",
            readOnly = true,
            description = "Mobile object's associated reference.",
        },
        ["target"] = {
            type = "tes3reference",
            readOnly = true,
            description = "Reference which mobile object has collided with.",
        },
    },
    filter = { "mobile", "target" },
}

--[[
-- Add this as an example
local function onCollision(e)
    if e.reference == tes3.player and
        e.target.object.objectType == tes3.objectType.door then
        if not e.target.destination then
            tes3.messageBox("I TRY")
            return false
        end
    end
end
]]
