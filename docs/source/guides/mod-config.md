
# Mod Configuration Menu (MCM)

MWSE comes with a built-in framework for easily making a configuration menu for your mod. All configuration menus made in this way are accessible by pressing the "Mod Config" button in the pause menu. This guide covers the basic concepts and syntax of the Mod Configuration Menu (MCM), and provides some examples to help you get started. More in-depth information can be found on the [`mwse.mcm` API page](../apis/mwse.md#mwsemcmcreateactiveinfo).

!!! note
	The `mwse.mcm` API acts as a complete replacement for the [easyMCM](https://easymcm.readthedocs.io/en/latest/) framework. It is backwards compatible with easyMCM, and provides several new functionalities.
	It's recommended that you use `mwse.mcm` instead of easyMCM.

## Simplest Example
Here is the simplest code required to create a functional mod config:

```lua linenums="1"
-- Default settings for your mod.
local myDefaultConfig = {
	enabled = true
}

-- The name of the config json file of your mod.
-- This is used to save/load your config, so that changes persist
-- between game launches.
local configPath = "My Awesome Mod"

-- Loaded config, taking into account any setting changes made by the user.
local myConfig = mwse.loadConfig(configPath, defaultConfig)

-- When the mod config menu is ready to start accepting registrations,
-- register this mod.
local function registerModConfig()
	-- Create the top level component Template.
	-- This is basically a box that holds all the other parts of the menu.
	local template = mwse.mcm.createTemplate({
		-- This will be displayed in the mod list on the lefthand pane.
		name = "My Awesome Mod",
		-- This makes all settings update the values stored in this table.
		config = myConfig
	})

	-- This tells MWSE to add this config menu to the list.
	template:register()

	-- Saves the config to a file whenever the menu is closed.
	template:saveOnClose(configPath, myConfig)

	-- Create a simple container Page under Template.
	local page = template:createPage({ label = "Settings" })

	-- Create a button under that controls the `"enabled"`
	-- setting in `myConfig`.
	-- Clicking on this button will change `"enabled"` from `true`
	-- to `false, and vice-versa.
	page:createYesNoButton({
		-- The `label` is the text that will be shown in your config menu.
		-- It lets users know what the button does.
		label = "Enable Mod",
		configKey = "enabled"
	})
end
event.register(tes3.event.modConfigReady, registerModConfig)
```

After loading up the game, you should see "My Awesome Mod" in the mod config menu, with a Yes/No button with label "Enable Mod" in its configuration pane.


## Other Sorts of Settings
In practice, most mods need more than simple "yes" and "no" buttons. Fortunately, MWSE includes many different sorts of settings that help to cover pretty much any use-case.

Here's an example that comes up fairly frequently: some part of your mod involves making a new UI element, and you'd like to let the user control where things show up on the screen.
The [`PercentageSlider`](../types/mwseMCMPercentageSlider.md) type is well suited to this task. Let's see how we can use it to extend the previous example.

```lua hl_lines="4 5 6 28 29 30 31 32 33 34 35 36 37 38 39 40"
-- Default settings for your mod.
local myDefaultConfig = {
	enabled = true,
	-- These values should be between 0 and 1.
	xPosition = 0.5, -- 0 means "left-most", and 1 means "right-most".
	yPosition = 0.5, -- 0 means "top-most", and 1 means "bottom-most".
}

local configPath = "My Awesome Mod"
local myConfig = mwse.loadConfig(configPath, defaultConfig)

-- When the mod config menu is ready to start accepting registrations,
-- register this mod.
local function registerModConfig()

	local template = mwse.mcm.createTemplate({
		name = "My Awesome Mod",
		config = myConfig
	})
	template:register()
	template:saveOnClose(configPath, myConfig)

	-- Create a simple container Page under Template.
	local page = template:createPage({ label = "Settings" })

	page:createYesNoButton({ label = "Enable Mod", configKey = "enabled" })

	-- Create some sliders that let the user control the positioning of whatever menu element
	-- your mod is making.
	-- Notice how the syntax is exactly the same as the syntax used to create new buttons.
	-- The only difference is we're now telling the page we want a `PercentageSlider`
	-- instead of a `YesNoButton`.
	page:createPercentageSlider({
		label = "Menu X Position",
		configKey = "xPosition"
	})
	page:createPercentageSlider({
		label = "Menu Y Position",
		configKey = "yPosition"
	})
end
event.register(tes3.event.modConfigReady, registerModConfig)
```
You should now see two additional sliders added to your configuration menu.

It's fairly common for mod authors to want certain things to happen whenever a certain key is pressed.
So it would be nice if the MCM had some easy way of managing keybinds... Luckily, it does!

```lua hl_lines="8 9 10 11 12 13 14 15 46 47"
-- Default settings for your mod.
local myDefaultConfig = {
	enabled = true,
	-- These values should be between 0 and 1.
	xPosition = 0.5, -- 0 means "left-most", and 1 means "right-most".
	yPosition = 0.5, -- 0 means "top-most", and 1 means "bottom-most".

	-- This stores the key combination that opens your mods UI element.
	-- This value corresponds to holding `Alt + H`.
	menuKeyCombo = {
		keyCode = tes3.scanCode.h,
		isAltDown = true,
		isControlDown = false,
		isShiftDown = false
	},
}

local configPath = "My Awesome Mod"
local myConfig = mwse.loadConfig(configPath, defaultConfig)

-- When the mod config menu is ready to start accepting registrations,
-- register this mod.
local function registerModConfig()
	-- Create the top level component Template. This is basically a box that holds all the other parts of the menu.

	local template = mwse.mcm.createTemplate({ name = "My Awesome Mod", config = myConfig })
	template:register()
	template:saveOnClose(configPath, myConfig)

	-- Create a simple container Page under Template.
	local page = template:createPage({ label = "Settings" })

	-- Create a button under Page that toggles a variable between true and false
	page:createYesNoButton({ label = "Enable Mod", configKey = "enabled" })

	-- Controls the position of the menu.
	page:createPercentageSlider({
		label = "Menu X Position",
		configKey = "xPosition"
	})
	page:createPercentageSlider({
		label = "Menu Y Position",
		configKey = "yPosition"
	})

	-- Controls the key that opens the menu.
	page:createKeyBinder({ label = "Menu key", configKey = "menuKeyCombo" })
end
event.register(tes3.event.modConfigReady, registerModConfig)
```


## Understanding the Terminology
Now that we've covered a few examples, it might be helpful to explain the big picture a bit.

The MCM is made up of things called **components**. In the most simple setup, the MCM consists of three different types of components:

1. Various **Settings**. These are the reason you wanted to make a menu in the first place.
	- They let users change various parts of a `config` table by interacting with various UI elements.
	- There are many different types of settings: you can have dropdown menus, sliders, buttons, keybinders, and more.
2. A few **Pages**. These hold various settings, and let you group them into separate pages.
	- In the simple examples we covered above, we only had at most four settings, so there wasn't really a need for multiple pages.
		- But as your mod grows, you might find yourself adding a lot of settings to the MCM.
		- In those situations, it can be nice to split things up across several pages.
	- There are a few different types of `Pages`, each suited to different use-cases.
3. One **Template**. This is the thing that holds all the various pages of your mod, as well as some important information about your mod.
	- It needs to know the `name` of your mod, so it can be properly placed in the list on the left side of the menu.
	- It can store your mods `config` table, and can facilitate the process of saving changes to a file whenever the menu closes.

All MCMs will consist of those three types of components. They're building blocks of any mod's config menu.
But there are a couple other components that can be useful:

4. **Categories**. These basically work like subsections of a `Page`.
	- You can add a `Category` to a page as a way to bundle similar settings together.
	- You can even make categories inside of other categories.
5. **Infos**. These can sometimes show up in specific situations as a way to present information that's not directly tied to a specific config setting.

## "Advanced" Examples




### Side Bar Page

MCM has a lot of features, and one of the most commonly used components is the `SideBarPage`. When a component is moused over in a `SideBarPage`, the right panel will display the `description` of that component. It is recommended to use a `SideBarPage` instead of a regular `Page` whenever a mod has a lot of settings.

```lua title="My Awesome Mod/main.lua"
	local settings = template:createSideBarPage({
		label = "Settings",
		-- This description will be displayed at the right panel if no component is moused over.
		-- (The `\z` character lets you define a string across multiple lines.)
		description =
			"My Awesome Mod\n\z
			Version 1.0.0\n\z
			\n\z
			The mod is very awesome. And this is its awesome description.\n",
	})

	settings:createYesNoButton({
		label = "Enable Mod",
		configKey = "enabled",
		-- This description will be displayed at the right panel if the button is moused over.
		description =
			"If this setting is enabled, the mod is enabled.\n\z
			\n\z
			If this setting is disabled, the mod is disabled."
	})
```

### Displaying Default Values of Settings
As your config menu grows, you may want to include some information about what the default values for various settings are.
Fortunately, the MCM comes with a very straight-forward solution to this problem:
```lua hl_lines="4 5"
local template = mwse.mcm.createTemplate({
	name = "My Awesome Mod",
	config = myConfig,
	defaultConfig = myDefaultConfig,
	showDefaultSetting = true,
})
```

This will result in the default values of settings automatically being displayed underneath their descriptions, provided those settings are in a `SideBarPage`.

### Separate config.lua

As your default config gets long, you might want to create a separate Lua file for it.

```lua linenums="1" title="My Awesome Mod/defaultConfig.lua"
-- Define the defaultConfig in a separate file so we can access it later.
local defaultConfig = {
	enabled = true,
	keybind = {
		keyCode = tes3.scanCode.k,
		isShiftDown = false,
		isAltDown = false,
		isControlDown = true,
	}, -- Ctrl + K
	nonEdible = {
		["ingred_diamond_01"] = true,
		["ingred_scrap_metal_01"] = true,
	}
}
return defaultConfig
```

```lua linenums="1" title="My Awesome Mod/config.lua"
local defaultConfig = require("My Awesome Mod.defaultConfig")
local config = mwse.loadConfig("My Awesome Mod", defaultConfig)
return config
```

We can then use the `require` function to import the `config` and `defaultConfig` in `main.lua`.

```lua linenums="1" title="My Awesome Mod/main.lua"
local config = require("My Awesome Mod.config")
local defaultConfig = require("My Awesome Mod.defaultConfig")
```

!!! note
	Having a dedicated config file is important when your mod has multiple files.
	When using a dedicated config file, you can import it from every file of your mod using `require`, and all of those files
	will use a sychronized copy of your config.

### Exclusions Page

Let's say your Awesome Mod makes certain ingredients non-edible. Using an Exclusions Page, you can allow users to manually add what ingredients should be non-edible in game.

```lua linenums="1" title="My Awesome Mod/main.lua"
template:createExclusionsPage({
	label = "Non-Edible Ingredients",
	description = "Ingredients that can't be directly consumed via equipping.",
	leftListLabel = "Non-Edible Ingredients",
	rightListLabel = "Ingredients",
	variable = mwse.mcm.createTableVariable{
		id = "nonEdible",
		table = config,
	},
	-- This filters the right list by ingredient object
	filters = {
		{
			label = "Ingredients",
			type = "Object",
			objectType = tes3.objectType.ingredient
		},
	},
})
```

There is also a more customized way to filter the right list. The following code filters the right list by ingredients with value higher than 100 gold.

```lua linenums="1" title="My Awesome Mod/main.lua"
	filters = {
		{
			label = "Ingredients",
			callback = function()
				local ingreds = {}
				--- @param ingred tes3ingredient
				for ingred in tes3.iterateObjects(tes3.objectType.ingredients) do
					if ingred.value >= 100 then
						table.insert(ingreds, ingred)
					end
				end
				table.sort(ingreds)
				return ingreds
			end
		},
	},
```