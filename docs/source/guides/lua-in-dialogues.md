# Lua in Dialogues

It is possible to use lua inside the mwscript results pane in the dialogue menu of the Construction Set.

Additional functions are available to extend the usefulness of this feature, including [`tes3ui.choice`](https://mwse.github.io/MWSE/apis/tes3ui/#tes3uichoice) and [`tes3.applyTextDefines`](https://mwse.github.io/MWSE/apis/tes3/#tes3applytextdefines).

!!! note
	The execution order of the Results pane is not mixed. All lua commands will be executed before the mwscript is parsed and executed.

!!! tip
	The keyboard shortcut Control+A does not work in the Construction Set. Right-clicking still allows selecting all text for quick replacement.


## Environment

All commands from the same results text share the same lua environment. This allows persistent values between lines. To do this however, the `local` keyword cannot be used. This does not pollute the normal global namespace.

!!! example "Example: Multi Line Values"

	```lua
	;lua playerName = tes3.player.object.name
	;lua tes3.messageBox("Hello %s", playerName)
	```

Additionally, some information has been provided in the environment. These values are `reference`, `dialogue`, and `info` for the specifics of what result is being executed, and on who.

!!! example "Example: Print Results Context"

	```lua
	;lua tes3.messageBox("Reference: %s; Dialogue/INFO: %s/%s;", reference, dialogue, info.id)
	```

The environment can also be extended through the [`dialogueEnvironmentCreated`](https://mwse.github.io/MWSE/events/dialogueEnvironmentCreated/) event. This allows libraries to extend what functionality is available to dialogue scripters more easily.

!!! example "Example: Extending the Dialogue Environment"

	In your library's main.lua file, create the event hook:

	```lua
	local function onDialogueEnvironmentCreated(e)
		-- Cache the environment variables outside the function for easier access.
		-- Dialogue scripters shouldn't have to constantly pass these to the functions anyway.
		local env = e.environment
		local reference = env.reference
		local dialogue = env.dialogue
		local info = env.info

		-- Define the "global" function.
		function env.DisplayDialogueContext()
			tes3.messageBox("Reference: %s; Dialogue/INFO: %s/%s;", reference, dialogue, info.id)
		end
	end
	event.register(tes3.event.dialogueEnvironmentCreated, onDialogueEnvironmentCreated)
	```

	Then, any dialogue scripts can make use of the new function:

	```lua
	;lua DisplayDialogueContext()
	```


## Usage

## Basics

To write lines with lua, prefix them with a mwscript comment starting with `;lua `. This allows the lua commands to be backwards-compatible for users that do not have MWSE, including users of OpenMW.

!!! example "Example: Goodbye with a Message"

	```
	; The next line will execute lua.
	;lua tes3.messageBox("This will run as lua!")

	; The next line will just be normal mwscript.
	goodbye
	```

!!! note
	All lua logic is contained to a single line. This a limitation of the approach to keep things as compatible as possible.


### Executing a Lua File

It may be useful to move the majority of your script logic to an actual .lua file, and use `dofile`.

!!! example "Example: Execute Lua File from Dialogue"

	```lua
	;lua dofile("myMod.onSomeDialogueResult")
	```

The environment variables are not inherited with `dofile`. To pass the data, it is best to make a call from a module.

!!! example "Example: Passing Results Context to a Lua Module"

	Define your module with a function to be called:

	```lua
	local myDialogueModule = {}

	--- A function designed to be called from a dialogue result.
	--- @param reference tes3reference The reference the dialogue is running on.
	--- @param dialogue tes3dialogue The parent dialogue for our info.
	--- @param info tes3dialogueInfo The specific INFO object that is being executed.
	function myDialogueModule.myFunction(reference, dialogue, info)
		tes3.messageBox("Reference: %s; Dialogue/INFO: %s/%s;", reference, dialogue, info.id)
	end

	return myDialogueModule
	```

	Call your module from the dialogue result:

	```lua
	;lua myDialogueModule = require("myMod.myDialogueModule")
	;lua myDialogueModule.myFunction(reference, dialogue, info)
	```


## Preventing Default Scripts

For better compatibility with OpenMW, it can be useful to prevent any vanilla result behavior from happening. This can be done by setting the environment variable `noMorrowindScript`.


!!! example "Example: Preventing Morrowind Script"

	```lua
	;lua noMorrowindScript = true
	```

This will not prevent other associated events, but will prevent the mwscript from the result box from being executed. This can be done conditionally as well.

!!! example "Example: Sometimes Preventing Morrowind Script"

	```lua
	;lua myDialogueModule = require("myMod.myDialogueModule")
	;lua myDialogueModule.myFunction(reference, dialogue, info)
	;lua if (myDialogueModule.wasSuccessful()) then noMorrowindScript = true end
	```
