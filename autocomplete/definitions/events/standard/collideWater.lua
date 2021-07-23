return{
    description = "This event is triggered when a mobile object collides with water.",
    eventData = {
        ["mobile"] = {
            type = "tes3mobileObject",
            readOnly = true,
            description = "Mobile object which collided.",
        },
        ["inWater"] = {
            type = "boolean",
            readOnly = true,
            description = "",
        },
    },
    filter = { "mobile", "inWater" },
}
