-- This example shows how to perform a cosine calculation inside mwscript. It
-- demonstrates how to execute lua functions from mwscript, and how to pass
-- variables between the languages.

-- TESCS script definition:
-- The script body is intentionally empty, the logic will be implemented in lua.
-- For typical usage we only need to define the variables that we will use.
--
--[[
	begin CalculateCosine
	float input
	float result
	StopScript CalculateCosine ; Not required, but good practice.
	end
]]

-- Using our function:
-- This is what using our script would look like. Typically from another script,
-- or from a dialogue result. This example assumes a `var` variable was defined.
--
--[[
	set CalculateCosine.input to 15
	StartScript CalculateCosine
	set var to CalculateCosine.result
]]

-- The actual lua implementation:

---@param e startGlobalScriptEventData
local function onStartGlobalScript(e)
	if e.script.id == "CalculateCosine" then
		local context = e.script.context
		context.result = math.deg(math.cos(math.rad(context.input)))
		-- Note: We return false here so the actual script never executes.
		-- This is usually what you want! Otherwise the script would begin
		-- running and subsequent `StartScript` calls would not trigger our
		-- event.
		return false
	end
end
event.register(tes3.event.startGlobalScript, onStartGlobalScript)
