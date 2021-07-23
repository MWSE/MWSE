return{
    description = "This event is triggered when a reference is deactivated.",
    eventData = {
        ["reference"] = {
            type = "tes3reference",
            readOnly = true,
            description = "The reference which was deactivated.",
        },
    },
    filter = "reference",
}
