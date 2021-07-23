return {
    description = "This event is triggered when a sound is player by the game or when tes3.playSound is called. This event can be blocked.",
    eventData = {
        ["sound"] = {
            type = "tes3sound",
            description = "The sound going to be played.",
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
            description = "The volume of the sound. Volume = 1 means 100 % loudness. Values lower than 1 the sound quiter.",
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
    },
    -- TODO - Add an example: return false, e.block = true, or e.volume = 0
}
