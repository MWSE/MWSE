return{
    description = "",
    eventData = {
        ["sound"] = {
            type = "tes3sound",
            --readOnly = true,
            description = "",
        },
        ["flags"] = {
            type = "number",
            --readOnly = true,
            description = "",
        },
        ["volume"] = {
            type = "number",
            --readOnly = true,
            description = "",
        },
        ["pitch"] = {
            type = "number",
            --readOnly = true,
            description = "",
        },
        ["isNot3D"] = {
            type = "boolean",
            --readOnly = true,
            description = "",
        },
    },
    filter = { "sound", "flags", "volume", "pitch", "isNot3D" }, -- Is this line ok?,
}
