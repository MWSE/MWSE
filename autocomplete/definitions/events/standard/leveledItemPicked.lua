return{
    description = "This event is called after any leveled item list is resolved.",
    eventData = {
        ["list"] = {
            type = "tes3leveledItem",
            readOnly = true,
            description = "The list responsible for creating a pick.",
        },
        ["pick"] = {
            type = "tes3item",
            --readOnly = true,
            description = "The result of the pick by vanilla mechanism.", -- This can be modified to change the result to something else.
        },
    },
    filter = "list",
}
