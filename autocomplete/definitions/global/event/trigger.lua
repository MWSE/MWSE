return {
	type = "function",
	description = [[Triggers an event. This can be used to trigger custom events with specific data.]],
	arguments = {
		{ name = "eventId", type = "string|tes3.event" },
		{ name = "payload", type = "table", optional = true },
		{
			name = "options",
			type = "table",
			optional = true,
			tableParams = {
				{ name = "filter", type = "userdata|string|number|tes3baseObject", optional = true, description = "Assigning a filter will make the event callbacks with filters matching this one to be executed first. All the other unfiltered callbacks are executed after." }, --try other types: "userdata|any"...
			}
		},
	},
	examples = {
		["blockableEvent"] = {
			title = "Custom event with `e.block` support",
		},
	},
	returns = { name = "resultPayload", type = "table", description = "This is the modified payload after all the callback functions registered on the triggered event are executed. Returning `true` from a callback function will set both `payload.block` and `payload.claim` to `true`. After an event has been claimed by a certain function (by setting the `claim` in eventData to `true`) no other registered callback functions will be executed on this event trigger. This is useful if you wish to implement blocking system for your event. In addition, this can be used to the same effect as some MWSE's events allow changing some of the `eventData` values to modify the behavior of the vanilla mechanics." }
}