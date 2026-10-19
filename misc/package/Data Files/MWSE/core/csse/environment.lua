--
-- Load the test environment passed by CSSE. Shared by the CSSE core mod and
-- startLuaMods.lua, which reads it before any mod runs.
--

local this = {}

local commandLine = os.getCommandLine()
local flagIndex = table.find(commandLine, "--fromCSSE")
if (not flagIndex) then
	return this
end

-- Other tools can pass in an alternative path after the flag.
local environmentPath = commandLine[flagIndex + 1]
if (environmentPath == nil or environmentPath:sub(1, 1) == "-") then
	environmentPath = "csse.toml"
end

-- Load the test environment and error out if it doesn't exist.
local csse_toml = toml.loadFile(environmentPath)
if (csse_toml == nil) then
	mwse.log("[CSSE] Could not load csse data from %s to change Morrowind execution!", environmentPath)
	return this
end

if (csse_toml.test_environment == nil) then
	mwse.log("[CSSE] No test environment exists!")
	return this
end

--- @type table?
this.test_environment = csse_toml.test_environment

return this
