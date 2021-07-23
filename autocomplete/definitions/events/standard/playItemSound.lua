return{
    description = "",
    eventData = {
        ["item"] = {
            type = "tes3baseObject",
            --readOnly = true,
            description = "",
        },
        ["state"] = {
            type = "number",
            --readOnly = true,
            description = "",
        },
        ["reference"] = {
            type = "tes3reference",
            --readOnly = true,
            description = "",
        },
    },
    filter = { "item", "state", "reference" }, -- Is this line ok?
}
