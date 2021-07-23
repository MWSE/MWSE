return {
    description = "",
    eventData = {
        ["path"] = {
            type = "string",
            description = "The path to the sound to play, relative to Data Files\\Sounds",
        },
        ["reference"] = {
            type = "tes3reference",
            description = "The reference which is emiting the sound.",
        },
        ["flags"] = {
            type = "number",
            readOnly = true,
            description = "",
        },
        ["volume"] = {
            type = "number",
            description = "The volume of the sound. Volume = 1 means 100 % loudness. Values lower than 1 the sound quiter."
        },
        ["pitch"] = {
            type = "number",
            --readOnly = true,
            description = "Pitch of the sound.",
        },
        ["isVoiceover"] = {
            type = "boolean",
            --readOnly = true,
            description = "",
        },
        ["sound"] = {
            type = "tes3sound",
            description = "",
        },
    },
    filter = "sound",
}
