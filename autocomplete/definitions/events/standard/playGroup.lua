return{
    description = "",
    eventData = {
        ["animationData"] = {
            type = "tes3animationData",
            --readOnly = true,
            description = "",
        },
        ["reference"] = {
            type = "tes3reference",
            --readOnly = true,
            description = "",
        },
        ["group"] = {
            type = "number",
            --readOnly = true,
            description = "",
        },
        ["index"] = {
            type = "number",
            --readOnly = true,
            description = "",
        },
        ["flags"] = {
            type = "number",
            --readOnly = true,
            description = "",
        },
        ["loopCount"] = {
            type = "number",
            --readOnly = true,
            description = "",
        },
    },
    filter = { "animationData", "group", "index", "flags", "loopCount" }, -- Is this ok?
}
