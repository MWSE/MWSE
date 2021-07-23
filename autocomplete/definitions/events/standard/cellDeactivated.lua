return{
    description = "This event is triggered when a cell is deactivated.",
    eventData = {
        ["cell"] = {
            type = "tes3cell",
            readOnly = true,
            description = "The cell which was deactivated.",
        },
    },
    filter = "cell",
}
