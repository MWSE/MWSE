# Event Definitions Guide

Event definitions are formatted as regular Lua tables. The available fields are:
| Field | Type | Description |
| ----- | ---- | ----------- |
| type  | `string` |  This flag is used when generating syntax highlighting files. This should always be `"event"` for event definitions. |
| description | `string` | The description for the event. |
| eventData | `table` | A table describing the data made available by the event. |
| filter | `string` | This allow specifying by which of the eventData parameters the event can be filtered. The name of the parameter should be enclosed in `""`. |
| blockable | `boolean` | Only needs to be present when the event indeed is blockable. In that case `true` should be passed here. |
| examples | `table` | A table with entries that are the names of the files included as examples. Each entry is a table itself with on available field, `title`. The title will be shown as the title of the example on the documentation page. |


eventData table entries can have the following fields:
| Field | Type | Description |
| ----- | ---- | ----------- |
| type  | `string` | This allows to specify the type of the value in `evenData`. You can put string names for basic Lua types: `number`, `boolean` and `string`, or objects exposed by MWSE, such as `tes3reference`. If the value can be of two or more types, then you should pass all the types split by `|`. For example, `tes3mobileCreature|tes3mobileNPC|nil`. |
| readOnly | `boolean` | This will mark the value as read-only. |
| description | `string` | The description for the value. You can pass a string with `""` or `[[]]`. |


An example of a event definition is the `activate` event definition file found in autocomplete\definitions\events\standard:

```Lua
-- autocomplete\definitions\events\standard\activate.lua
return {
	type = "event",
	description = [[This event is invoked whenever something is activated, typically by the player. Activation is usually done with the associated activate/use key, but may also be forced by scripts.

Non-player actors may also activate objects, such as when opening doors, or via MWSE functions like [`tes3.activate()`](https://mwse.github.io/MWSE/apis/tes3/#tes3activate).

Some examples of when the activate event fires includes:

- When a door is used.
- When an item is picked up.
- When someone attempts to open a container.]],
	eventData = {
		["activator"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The actor attempting to trigger the event.",
		},
		["target"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The reference that is being activated.",
		},
	},
	filter = "target",
	blockable = true,
	examples = {
		["ShowActivationTarget"] = {
			title = "Show a message for what the player is activating",
		},
		["PreventActivatingNPCs"] = {
			title = "Prevent the player from activating NPCs",
		},
	},

}
```
