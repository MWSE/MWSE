return{
    description = "This event is triggered when a cell is activated.",
    eventData = {
        ["cell"] = {
            type = "tes3cell",
            readOnly = true,
            description = "The cell which was activated.",
        },
    },
    filter = "cell",
}
