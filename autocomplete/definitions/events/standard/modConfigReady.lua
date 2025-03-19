return {
	type = "event",
	description = [[This event fires once MWSE's internal mod configuration menu code has been initialized.

This event is used to register the settings configuration menu. That can be achieved by using any of the following:

- [template:register()](../types/mwseMCMTemplate.md#register)
- [mwse.mcm.register()](../apis/mwse.md#mwsemcmregister)
]],
	examples = {
		["mcmExample"] = {
			title = "Basic MCM.",
		}
	}
}