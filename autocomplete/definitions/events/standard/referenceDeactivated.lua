return{
    description = "This event is triggered when a reference is deactivated.",
	related = { "referenceActivated", "referenceDeactivated" },
    eventData = {
        ["reference"] = {
            type = "tes3reference",
            readOnly = true,
            description = "The reference which was deactivated.",
        },
    },
    filter = "reference",
}
