return {
	type = "class",
	description = [[This button allows the player to bind a key combination for use with hotkeys. The binder allows specifying if mouse buttons and scroll wheel bindings are allowed and whether modifier keys Shift, Alt and Ctrl are allowed.

When the player presses the button with current hotkey, a prompt asks them to press a new key (or key combination using Shift, Ctrl or Alt) to bind.

If this KeyBinder is set to only accept keyboard keys, key combos are stored in the following format ([mwseKeyCombo](../types/mwseKeyCombo.md)):

```lua linenums="1"
{
	keyCode = tes3.scanCode.{key},
	isShiftDown = true,
	isAltDown = false,
	isControlDown = false,
},
```

On the other hand, if the KeyBinder allows binding mouse keys in addition to keyboard keys, key combos are stored in the following format ([mwseKeyMouseCombo](../types/mwseKeyMouseCombo.md)):

```lua linenums="1"
{
	keyCode = tes3.scanCode.{key},
	isShiftDown = true,
	isAltDown = false,
	isControlDown = false,
	mouseWheel = -1 - down, 1 - up, nil
	mouseButton = number|nil
},
```
]],
	inherits = "mwseMCMBinder",
	examples = {
		["..\\..\\..\\global\\tes3\\isKeyEqual\\filtering"] = {
			title = "Filtering out key presses that aren't equal to the bound key combination"
		}
	},
}
