
-- An example of a simple configuration setup
local defaultConfig = {
	---@type mwseKeyMouseCombo
	combo = {
		-- Alt + Left mouse button
		mouseButton = 0,
		isAltDown = true,
		isControlDown = false,
		isShiftDown = false,
	},
}
local config = mwse.loadConfig("myModConfig", defaultConfig)

local function registerModConfig()
	local template = mwse.mcm.createTemplate({
		name = "Test Mod",
		config = config
	})
	template:register()

	local page = template:createSideBarPage({ label = "Settings" })

	page:createKeyBinder({
		label = "My combo",
		description = "This combo does...",
		allowMouse = true,
		configKey = "combo",
	})
end
event.register(tes3.event.modConfigReady, registerModConfig)


--- @param e keyDownEventData|mouseButtonDownEventData|mouseWheelEventData
local function sayHi(e)
	if not tes3.isKeyEqual({ expected = config.combo, actual = e }) then
		-- Nothing to do if the pressed combination isn't equal to our expected combination.
		return
	end

	-- Now do our logic
	tes3.messageBox("Hi!")
end

event.register(tes3.event.keyDown, sayHi)
event.register(tes3.event.mouseButtonDown, sayHi)
event.register(tes3.event.mouseWheel, sayHi)
